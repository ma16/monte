// BSD 2-Clause License, see github.com/ma16/monte

#ifndef INCLUDE_ChartWidget_h
#define INCLUDE_ChartWidget_h

#include <boost/optional.hpp>
#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

class ChartWidget : public QWidget
{
    Q_OBJECT

public:

    explicit ChartWidget(QWidget *parent=nullptr)
	: QWidget(parent)
	, view     (this)
	{}

    void init() ;

    QtCharts::QChart* chart() { return this->view.chart() ; }

    unsigned counts() const { return counts_ ; }
    unsigned missed() const { return missed_ ; }
    
public slots:

    void clear() { series->clear() ; counts_=missed_=0 ; }
    
    void append(boost::optional<double> y)
    {
	if (y)
	    series->append(counts_,*y) ;
	else
	    ++missed_ ;
	++counts_ ;
    }

private:
    
    QtCharts::QChartView view ; 
    // QGraphicsWidget is a light-weight QWidget. Its coordinates and
    // geometry are defined with doubles instead of integers. A subset
    // of QWidget attributes is supported: custom drag, drop,
    // framework, etc. The QGraphicsWidget class is designed to be
    // added in a QGraphicsScene class, a high-performance Qt component
    // used to draw hundreds of items on screen at the same time.

    QtCharts::QLineSeries *series = nullptr ;

    unsigned counts_ = 0 ;
    unsigned missed_ = 0 ;
} ;

#endif // INCLUDE_ChartWidget_h
