// BSD 2-Clause License, see github.com/ma16/monte

#include "AboutWidget.h"

#ifndef RELEASE_NAME
#define RELEASE_NAME "1 (beta)"
#endif

#ifndef RELEASE_DATE
#define RELEASE_DATE __DATE__ " " __TIME__
#endif

static char const message[] = "\
This is the program 'monte'. A monitor for temperatures that are recorded on a DS18B20 chip. Please refer to https://github.com/ma16/monte for program details.\
\n\n\
The sources and the documentation are provided under the BSD 2-Clause License. See https://opensource.org/licenses/bsd-license.php for details.\
\n\n\
The program uses Qt (https://www.qt.io/). Pre-built binaries may adhere to additional license conditions. See http://doc.qt.io/qt-5/licenses-used-in-qt.html.\
\n\n\
Release: " RELEASE_NAME " @ " RELEASE_DATE ;

AboutWidget::AboutWidget(QWidget *parent) : QTextEdit(parent)
{
    this->setReadOnly(true) ;
    this->QTextEdit::append(message) ;
}
