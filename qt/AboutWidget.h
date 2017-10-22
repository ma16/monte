// BSD 2-Clause License, see github.com/ma16/monte

#ifndef INCLUDE_AboutWidget_h
#define INCLUDE_AboutWidget_h

#include <QTextEdit>

class AboutWidget : public QTextEdit
{
    Q_OBJECT
  
public:
  
    explicit AboutWidget(QWidget *parent=nullptr) ;
} ;

#endif // INCLUDE_AboutWidget_h
