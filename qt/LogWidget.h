// BSD 2-Clause License, see github.com/ma16/monte

#ifndef INCLUDE_LogWidget_h
#define INCLUDE_LogWidget_h

#include "ui_LogWidget.h"
#include <Neat/Logger.h>
#include <QWidget>

class LogWidget : public QWidget, public Neat::Logger::Terminal
{
    Q_OBJECT
  
public:
  
    explicit LogWidget(QWidget *parent=nullptr) : QWidget(parent) {}

    void init() ;

    void dispatch(std::string const &message) override ;

    ~LogWidget() override { delete ui ; }

public slots:

    void append(QString const &text) ;

private:

    Ui::LogWidget *ui = nullptr ;
} ;

#endif // INCLUDE_LogWidget_h
