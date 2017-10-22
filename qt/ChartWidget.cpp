// BSD 2-Clause License, see github.com/ma16/monte

#include "ChartWidget.h"
#include <QVBoxLayout>

void ChartWidget::init()
{
    // put chart widget is into a new layout
    QVBoxLayout* layout = new QVBoxLayout(this) ;
    layout->addWidget(&this->view) ;
    // ...but why?

    // otpional chart rendering
    this->view.setRenderHint(QPainter::Antialiasing) ;
    this->view.chart()->legend()->setVisible(false) ;

    this->series = new QtCharts::QLineSeries(this) ;
    this->chart()->addSeries(this->series) ;
    this->chart()->createDefaultAxes() ;
}

#if 0
void HistoryWidget::wheelEvent(QWheelEvent *event)
{
    auto p = event->angleDelta() ;
    
    auto x = std::to_string(p.x()) ;
    auto y = std::to_string(p.y()) ;
    Neat::Logger::seize()->post("wheel event:" + x + ',' + y) ;
    if (!p.isNull())
    {
	if (p.y() > 0)
	{
	    // increase range
	}
	else if (p.y() < 0)
	{
	    // decrease range
	}
	this->chart()->axisY()->setRange(min,max) ;
    }
    event->accept();
}
#endif
