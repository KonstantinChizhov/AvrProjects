#include "mytransform.h"
#include <QDebug>

template <class T>
inline T sign(T val){return val >= T(0) ? T(1) : T(-1);}

MyTransform::MyTransform(const QRectF &view, const QRectF &_data, const ScaleFunc &scaleFuncX, const ScaleFunc &scaleFuncY)
{
    funcX = scaleFuncX.Clone();
    funcY = scaleFuncY.Clone();

    QRectF data;
    data.setTop(funcY->F(_data.top()));
    data.setBottom(funcY->F(_data.bottom()));
    data.setLeft(funcX->F(_data.left()));
    data.setRight(funcX->F(_data.right()));

    //qDebug() << _data;
    //qDebug() << data;


    scaleY = (view.top() - view.bottom()) / (data.top()-data.bottom());
    scaleX = (view.right() - view.left()) / (data.right() - data.left());
    dx = view.left() - scaleX *  data.left();
    dy = view.bottom() - scaleY * data.bottom();

  //  qDebug() <<  "dx, dy, scX, scY" << dx << dy << scaleX << scaleY;
}

MyTransform::~MyTransform()
{
    delete funcX;
    delete funcY;
}

QPointF MyTransform::map(const QPointF &point)const
{
    return QPointF(dx + funcX->F(point.x()) * scaleX, dy + funcY->F(point.y()) * scaleY);
}

QLineF MyTransform::map(const QLineF &line)const
{
    return QLineF(map(line.p1()), map(line.p2()));
}
