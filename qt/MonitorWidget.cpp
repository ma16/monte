// BSD 2-Clause License, see github.com/ma16/monte

#include "MonitorWidget.h"
#include <Neat/Logger.h>
#include <ctime>
#include <Neat/cast.h>
#include <QPushButton>
#include <Rpi/Counter.h>
#include <Rpi/Gpio.h>

void MonitorWidget::init(Rpi::Peripheral *rpi)
{
    this->rpi = rpi ;
    
    this->ui = new Ui::MonitorWidget ;
    this->ui->setupUi(this) ;
    this->ui->chartW->init() ;
    
    AxisControl::Config::Data dataX(+0,10,0,10,false) ;
    // ...[todo] make user data configurable
    this->controlX.reset(new AxisControl(
			     AxisControl::Config::make(dataX),
			     this->ui->xZoom, 
			     this->ui->xScroll,
			     this->ui->chartW->chart()->axisX())) ;
    
    AxisControl::Config::Data dataY(-55,+125-(-55),0-(-55),40,true) ;
    // ...[todo] make user data configurable
    this->controlY.reset(new AxisControl(
			     AxisControl::Config::make(dataY),
			     this->ui->yZoom, 
			     this->ui->yScroll,
			     this->ui->chartW->chart()->axisY())) ;

    this->ui->counterPrescaler->setRange(0,255) ;
    this->ui->gpio->setRange(0,31) ;
    
    this->query() ;
    
    this->ui->start->setText("Start") ;
    this->connect(this->ui->start,&QPushButton::clicked,
		  [this]
		  {
		      if (this->recorder == nullptr) this->start() ;
		      else                           this-> stop() ;
		  }) ;
    
    typedef void (QSpinBox::*ValueChanged)(int) ;
    this->connect(this->ui->counterPrescaler,
		  static_cast<ValueChanged>(&QSpinBox::valueChanged),
		  [this](int i)
		  {
		      auto div = static_cast<Rpi::Counter::Div>(i) ;
		      Rpi::Counter(this->rpi).prescale(div) ;
		      this->query() ;
		  }) ;

    this->connect(this->ui->counterActivate,&QCheckBox::stateChanged,
		  [this](int state)
		  {
		      auto enabled = state != Qt::Unchecked ;
		      Rpi::Counter(this->rpi).enable(enabled) ;
		      this->query() ;
		  }) ;
    
}

void MonitorWidget::clear()
{
    AxisControl::Config::Data dataX(+0,10,0,10,false) ; 
    // ...[todo] make user data configurable
    auto configX = AxisControl::Config::make(dataX) ;
    emit this->controlX->set(configX) ;

    AxisControl::Config::Data dataY(-55,+125-(-55),0-(-55),40,true) ;
    // ...[todo] make user data configurable
    auto configY = AxisControl::Config::make(dataY) ;
    emit this->controlY->set(configY) ;

    this->ui->counts->setNum(0) ;
    this->ui->missed->setNum(0) ;
    emit this->ui->chartW->clear() ;
}

static QString now() // [todo] deduplicate (also in LogWidget.cpp)
{
    auto t = std::time(nullptr) ;
    auto l = std::localtime(&t) ;
    char buffer[0x100] ;
    auto n = std::strftime(buffer,sizeof(buffer),"%Y-%m-%d %X",l) ;
    assert(n > 0) ; (void)n ;
    return buffer ;
}

void MonitorWidget::append(boost::optional<double> y)
{
    emit this->ui->chartW->append(y) ;
    auto counts = Neat::to_signed(this->ui->chartW->counts()) ;
    this->ui->counts->setNum(counts) ;
    auto missed = Neat::to_signed(this->ui->chartW->missed()) ;
    this->ui->missed->setNum(missed) ;
    auto data = this->controlX->config().data() ;
    if (counts >= data.size+2)
    {
	if (data.pos + data.section == data.size)
	    ++data.pos ; // rolling chart
	++data.size ;
	auto config = AxisControl::Config::make(data) ;
	emit this->controlX->set(config) ;
    }
    this->ui->endDate->setText(now()) ;
}

static void enable(QLayout *layout,bool enabled)
{
    for (int i=0 ; i<layout->count() ; ++i)
    {
	auto itemW = layout->itemAt(i)->widget() ;
	if (itemW != nullptr)
	{
	    itemW->setEnabled(enabled) ;
	    continue ;
	}
	auto itemL = layout->itemAt(i)->layout() ;
	if (itemL != nullptr)
	{
	    enable(itemL,enabled) ;
	    continue ;
	}
    }
}

void MonitorWidget::start()
{
    enable(this->ui->setup,false) ;
    this->clear() ;
    auto pin = Neat::as_unsigned(this->ui->gpio->value()) ;
    this->recorder = Recorder::make(this->rpi,Rpi::Pin::make(pin)) ;
    this->connect(&this->timer, &QTimer::timeout,
		  [this]
		  {
		      auto sample = this->recorder->sample() ;
		      this->append(sample) ;
		  }) ;
    this->ui->start->setText("Stop") ;
    this->timer.start(1000) ; // [todo] make configurable
    this->ui->startDate->setText(now()) ;
    auto sample = this->recorder->sample() ;
    this->append(sample) ;
}

void MonitorWidget::stop()
{
    this->timer.disconnect() ;
    this->recorder.reset() ;
    this->ui->start->setText("Start") ;
    enable(this->ui->setup,true) ;
}

void MonitorWidget::query()
{
    Rpi::Counter counter(rpi) ;

    auto enabled = counter.enabled() ;
    this->ui->counterActivate->setCheckState(enabled
					? Qt::Checked
					: Qt::Unchecked) ;
    this->ui->counterFrequency->setEnabled(enabled) ;
    this->ui->counterPrescaler->setEnabled(enabled) ;
    this->ui->start->setEnabled(enabled) ;

    auto div = counter.prescaler() ;
    std::ostringstream os ;
    os.setf(std::ios::scientific) ;
    os.precision(2) ;
    os << 250e+6/(div+1) << "/s" ; // [todo] 250e+6 should be in Rpi::Counter
    this->ui->counterFrequency->setText(QString::fromStdString(os.str())) ;
    this->ui->counterPrescaler->setValue(div) ;
}
