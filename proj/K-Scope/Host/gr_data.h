#ifndef GR_DATA
#define GR_DATA

#include <QtCore/Qstring>
#include <QtCore/QVector>
#include <QtGui/QPolygonF>
#include <QtGui/QPen>
#include <math.h>
#include <QPolygonF>
#include "MyTransform.h"

class Curve: public QPolygonF
{
    public:
        Curve();
        Curve(const QVector<QPointF> &data);
        ~Curve();
        void SetData(const double *data, int size);
        void SetData(const QVector<double> &data);
        void SetData(const QVector<QPointF> &data);
        QRectF BoundingRect();
        QPen Pen()const;
        void setPen(const QPen &p);
        QString Name()const;
        void setName(QString n);
        QString Units()const;
        void setUnits(QString n);
        QString NameAndUnits()const;
        enum INTERPOLATE{INT_LINE, INT_CUBIC};
        void SetInterpolate(int inter);
        void draw(QPainter &p, const MyTransform &trans);
        void Filter();
        void SetScale(double value);
        void SetOffset(double value);
private:
        void drawLines(QPainter &p, const MyTransform &trans)const;
        double _offset, _scale;
        QPen _pen;
        QString _name, _units;
        enum INTERPOLATE _interpolate;
    };

#endif
