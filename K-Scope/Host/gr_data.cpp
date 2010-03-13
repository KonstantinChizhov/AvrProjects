#include "gr_data.h"
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include "mytransform.h"

Curve::Curve()
{
    _offset = 0.0;
    _scale = 1.0;
    _interpolate=INT_LINE;
    _pen.setColor(Qt::black);
}

Curve::~Curve()
{

}

void Curve::SetData(const QVector<double> &data)
{
    SetData(data.data(), data.size());
}

void Curve::SetData(const double *data, int size)
{
    if(size<0) return;
    resize(size);
    for(int i=0; i<size; i++)
    {
        (*this)[i]= QPointF(i, data[i]);
    }
}

void Curve::SetData(const QVector<QPointF> &data)
{
    if(data.size()<=0) return;

    resize(data.size());
    for(int i=0; i<data.size(); i++)
    {
        (*this)[i]=data[i];
    }
}

QRectF Curve::BoundingRect()
{
    double max=(*this)[0].y();
    double min=max;

    double maxX=(*this)[0].x();
    double minX=maxX;

    for(int i=0; i<size(); i++)
    {
        if((*this)[i].x()>maxX) maxX = (*this)[i].x();
        if((*this)[i].x()<minX) minX = (*this)[i].x();
        if((*this)[i].y()>max) max = (*this)[i].y();
        if((*this)[i].y()<min) min = (*this)[i].y();
    }
    return QRectF(minX, max, maxX-minX, max-min);
}

Curve::Curve(const QVector<QPointF> &graph)
{
    if(graph.size()<=0) return;
    _interpolate=INT_LINE;
    SetData(graph);
    _offset = 0.0;
    _scale = 1.0;
    _pen.setColor(Qt::black);
}
//----------------------------------------------------------------------------
void Curve::draw(QPainter &p, const MyTransform &trans)
{
    p.setPen(Pen());
    p.setBrush(Qt::NoBrush);
    switch(_interpolate){
    case INT_LINE:
        drawLines(p, trans);
        break;
    case INT_CUBIC:
        drawLines(p, trans);
        break;
    }
}
//----------------------------------------------------------------------------
void Curve::drawLines(QPainter &p, const MyTransform &trans)const
{
    QPolygonF result(size());

    for(int i=0; i<size(); i++)
    {
        result[i]=trans.map(QPointF((*this)[i].x(), (*this)[i].y()*_scale+_offset));
    }
    p.drawPolyline(result);
}

QPen Curve::Pen()const{return _pen;}
void Curve::setPen(const QPen &p){_pen=p;}
QString Curve::Name()const{return _name;}
void Curve::setName(QString n){_name=n;}
QString Curve::Units()const{return _units;}
void Curve::setUnits(QString n){_units=n;}
QString Curve::NameAndUnits()const{return _name + ", " + Units();}

void Curve::SetInterpolate(int inter)
{
    _interpolate=(INTERPOLATE)inter;
}



void Curve::Filter()
{

}

void Curve::SetScale(double value)
{
    this->_scale = value;
}

void Curve::SetOffset(double value)
{
    this->_offset = value;
}
