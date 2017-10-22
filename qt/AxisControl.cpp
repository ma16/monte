// BSD 2-Clause License, see github.com/ma16/monte

#include "AxisControl.h"
#include <Neat/cast.h>
#include <Neat/Error.h>

AxisControl::Config AxisControl::Config::make(Data data)
{
    std::string id = "qt.AxisControl::Config:" ;
    
    if (data.size <= 0)
	throw Neat::Error(id+"size must be positive") ;
    if (data.section <= 0) 
	throw Neat::Error(id+"section must be positive") ;
    if (data.pos < 0) 
	throw Neat::Error(id+"pos must not be negative") ;
    
    if (data.start + data.size <= data.start) 
	throw Neat::Error(id+"integer overflow in start+size") ;
    if (data.section > data.size) 
	throw Neat::Error(id+"section bigger than size") ;
    if (data.pos > data.size - data.section) 
	throw Neat::Error(id+"pos+section bigger than size") ;

    if (data.reverse)
	data.pos = data.size - data.section - data.pos ;
    
    return Config(data) ;
}

void AxisControl::set(Config const &config)
{
    config_ = config ;
    
    zoomSlider->setRange(1,config_.data_.size) ;
    zoomSlider->setValue(config_.data_.section) ; 
    // ...[todo] we may want to use a logarithmic range instead

    this->updateView() ;
    this->updateScrollbar() ;
}

AxisControl::AxisControl(
    Config          const &config,
    QAbstractSlider   *zoomSlider,
    QAbstractSlider *scrollSlider,
    QtCharts::QAbstractAxis *axis)
    
    : config_           (config)
    , zoomSlider    (zoomSlider)
    , scrollSlider(scrollSlider)
    , axis                (axis)
      
{
    this->set(config) ;
    
    connect(zoomSlider,  &QAbstractSlider::valueChanged,
	    this,                    &AxisControl::zoom) ;
    connect(scrollSlider,&QAbstractSlider::valueChanged,
	    this,                  &AxisControl::scroll) ;
}

void AxisControl::scroll(int pos)
{
    assert(0 <= pos && pos+config_.data_.section <= config_.data_.size) ;
    
    config_.data_.pos = pos ;
    this->updateView() ;
}

void AxisControl::zoom(int section)
{
    assert(0 < section && section <= config_.data_.size) ;

    config_.data_.pos -= (section - config_.data_.section) / 2 ;
    // ...[todo] if delta is -1 or +1 the symmetric change of the
    // visible section won't work properly: pos remains as it is
    // but the section size changes
    config_.data_.section = section ;
    
    if (config_.data_.pos < 0)
	config_.data_.pos = 0 ;
    else if (config_.data_.pos > config_.data_.size - config_.data_.section)
	config_.data_.pos = config_.data_.size - config_.data_.section ;

    this->updateScrollbar() ;
    this->updateView() ;
}

void AxisControl::updateView()
{
    auto pos = config_.data_.start ;
    if (config_.data_.reverse)
	pos += config_.data_.size - config_.data_.section - config_.data_.pos ;
    else
	pos += config_.data_.pos ;
    axis->setRange(pos,pos+config_.data_.section) ;
}

void AxisControl::updateScrollbar()
{
    scrollSlider->setRange(0,config_.data_.size-config_.data_.section) ;
    scrollSlider->setPageStep(config_.data_.section) ;
    scrollSlider->setValue(config_.data_.pos) ;
}
