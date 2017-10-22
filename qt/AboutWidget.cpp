// BSD 2-Clause License, see github.com/ma16/monte

#include "AboutWidget.h"

#ifndef RELEASE_NAME
#define RELEASE_NAME "nondescript"
#endif

#ifndef RELEASE_DATE
#define RELEASE_DATE __DATE__ " " __TIME__
#endif

static char const message[] = "\
This is the program 'monte'. A monitor for temperatures that are recorded on a DS18B20 chip. Please refer to https://github.com/ma16/monte for program details.\
\n\n\
The program, its sources and the documentation are provided under the BSD 2-Clause License. See https://opensource.org/licenses/bsd-license.php for details.\
\n\n\
Release: " RELEASE_NAME " @ " RELEASE_DATE ;

AboutWidget::AboutWidget(QWidget *parent) : QTextEdit(parent)
{
    this->setReadOnly(true) ;
    this->QTextEdit::append(message) ;
}
