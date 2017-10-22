// BSD 2-Clause License, see github.com/ma16/monte

#include "LogWidget.h"
#include <cassert>
#include <ctime>

void LogWidget::init()
{
    this->ui = new Ui::LogWidget ;
    this->ui->setupUi(this) ;
    this->ui->text->setReadOnly(true) ;

    this->connect(this->ui->clear,&QPushButton::clicked,
		  this->ui->text,     &QTextEdit::clear) ;
    
    Neat::Logger::seize()->connect(this) ;
}

static QString now()
{
    auto t = std::time(nullptr) ;
    auto l = std::localtime(&t) ;
    char buffer[0x100] ;
    auto n = std::strftime(buffer,sizeof(buffer),"%Y-%m-%d %X",l) ;
    assert(n > 0) ; (void)n ;
    return buffer ;
}

void LogWidget::dispatch(std::string const &message)
{
    emit this->append(QString::fromStdString(message)) ;
}

void LogWidget::append(QString const &text)
{
    auto message = now() ;
    message.append(' ') ;
    message.append(text) ;
    this->ui->text->append(message) ;
}

// [todo] maximum size + switch on/off verbosity
