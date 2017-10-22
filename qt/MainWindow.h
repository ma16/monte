// BSD 2-Clause License, see github.com/ma16/monte

#ifndef INCLUDE_MainWindow_h
#define INCLUDE_MainWindow_h

#include "ui_MainWindow.h"

#include "AboutWidget.h"
#include "MonitorWidget.h"
#include "LogWidget.h"
#include <QMainWindow>
#include <QTabWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    
    explicit MainWindow(QWidget *parent=nullptr) : QMainWindow(parent) {}

    void init(Rpi::Peripheral *rpi) ;
  
    ~MainWindow() override { delete ui ; }
    
private:
    
    Ui::MainWindow      *ui = nullptr ;
    QTabWidget        *tabW = nullptr ;
    AboutWidget     *aboutW = nullptr ;
    LogWidget         *logW = nullptr ;
    MonitorWidget *monitorW = nullptr ;
} ;

#endif // INCLUDE_MainWindow_h
