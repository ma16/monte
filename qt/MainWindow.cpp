// BSD 2-Clause License, see github.com/ma16/monte

#include "MainWindow.h"

void MainWindow::init(Rpi::Peripheral *rpi)
{
    this->ui = new Ui::MainWindow ;
    ui->setupUi(this) ;

    this->tabW = new QTabWidget() ;
    this->ui->centralWidget->layout()->addWidget(this->tabW) ;

    this->aboutW = new AboutWidget() ;
    this->tabW->addTab(this->aboutW,"About") ;

    this->logW = new LogWidget() ;
    this->logW->init() ;
    this->tabW->addTab(this->logW,"Log") ;

    this->monitorW = new MonitorWidget() ;
    this->monitorW->init(rpi) ;
    this->tabW->addTab(this->monitorW,"Monitor") ;

    this->tabW->setCurrentWidget(this->monitorW) ;
}
