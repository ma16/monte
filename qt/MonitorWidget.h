// BSD 2-Clause License, see github.com/ma16/monte

#ifndef INCLUDE_MonitorWidget_h
#define INCLUDE_MonitorWidget_h

#include "ui_MonitorWidget.h"
#include "AxisControl.h"
#include <Device/Ds18b20/Recorder.h>
#include <QWidget>
#include <QTimer>
#include <Rpi/Peripheral.h>

class MonitorWidget : public QWidget
{
    Q_OBJECT

public:
    
    explicit MonitorWidget(QWidget *parent=nullptr) : QWidget(parent) {}

    void init(Rpi::Peripheral *rpi) ;

    ~MonitorWidget() override { delete this->ui ; }

private:
    
    Ui::MonitorWidget *ui = nullptr ;

    std::unique_ptr<AxisControl> controlX ;
    std::unique_ptr<AxisControl> controlY ;

    QTimer timer ; 

    using Recorder = Device::Ds18b20::Recorder ;
  
    std::unique_ptr<Recorder> recorder ;

    Rpi::Peripheral *rpi = nullptr ;

    void append(boost::optional<double> y) ;
    void clear() ;
    void query() ;
    void start() ;
    void stop() ;
} ;

#endif // INCLUDE_MonitorWidget_h
