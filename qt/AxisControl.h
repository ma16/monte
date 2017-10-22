// BSD 2-Clause License, see github.com/ma16/monte

#ifndef INCLUDE_AxisControl_h
#define INCLUDE_AxisControl_h

//           size
//    <------------------>
//     pos  section
//   >-----<-------->
// ---+----+--------+-----+---
//    ^
//  start
//
// the interval (start,start+size) represents the "world"
//
// the interval (pos,pos+section) represents the peek into the world
// this is done by a scrollbar 
//
// the size of the section that is peeked into itself can be changed
// by a slider between 1 and size
	
#include <QAbstractSlider>
#include <QObject>
#include <QtCharts/QAbstractAxis>
#include <cassert>

class AxisControl : public QObject
{
    Q_OBJECT
    
public:

    struct Config
    {
	struct Data
	{
	    int    start ; // supported values: all (signed)
	    int     size ; //    only positive values
	    int      pos ; //    only not-signed values
	    int  section ; //    only positive values
	    bool reverse ; // view upside down
	    
	    Data(int start,int size,int pos,int section,bool reverse)
		: start    (start)
		, size      (size)
		, pos        (pos)
		, section(section)
		, reverse(reverse)
		{ }
	} ;
	
	static Config make(Data) ;

	Data const& data() const { return data_ ; } 
	
    private:
	
	friend AxisControl ;

	Data data_ ; Config(Data const &data) : data_(data) {}
    } ;
    
    AxisControl(
	Config          const &config,
	QAbstractSlider   *zoomSlider,
	QAbstractSlider *scrollSlider,
	QtCharts::QAbstractAxis *axis) ;

    Config const& config() const { return config_ ; }
				       
public slots:
	
    void set(Config const&) ;
				       
private slots:
    
    void scroll(int) ;
    
    void zoom(int) ;
    
private:

    Config                config_ ;
    QAbstractSlider   *zoomSlider ;
    QAbstractSlider *scrollSlider ; 
    QtCharts::QAbstractAxis *axis ;

private:

    void updateScrollbar() ;

    void updateView() ;
} ;

#endif // INCLUDE_AxisControl_h
