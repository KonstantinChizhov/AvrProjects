#ifndef MYTRANSFORM_H
#define MYTRANSFORM_H
#include <math.h>
#include <QString>
#include <QRectF>
#include <QLineF>

class ScaleFunc{
        public:
    ScaleFunc(){}
    virtual double F(double val)const=0;
    virtual QString Name()const=0;
    virtual QString Units(QString in)const=0;
    virtual ScaleFunc* Clone()const=0;
    virtual int Type()=0;
};

class LineScale :public ScaleFunc{
        public:
    LineScale(){}
    virtual double F(double val)const{return val;}
    virtual QString Name()const{return QString::fromLocal8Bit("Linear");}
    virtual QString Units(QString in)const{return in;}
    virtual LineScale* Clone()const{return new LineScale();}
    virtual int Type(){return 0;}
};

class LogScale :public ScaleFunc{
        public:
    LogScale(){}
    virtual double F(double v)const{if(v<=0.0) return 0.0; return  10*log10(v);}
    virtual QString Name()const{return QString::fromLocal8Bit("Logarothmic");}
    virtual QString Units(QString in)const{return QString::fromLocal8Bit("dB");}
    virtual LogScale* Clone()const{return new LogScale();}
    virtual int Type(){return 1;}
};

class TempScale :public ScaleFunc{
        public:
    TempScale(){}
    virtual double F(double v)const{return 293.0*v-293.0;}
    virtual QString Name()const{return QString::fromLocal8Bit("Noise temperature");}
    virtual QString Units(QString in)const{return QString::fromLocal8Bit("K");}
    virtual TempScale* Clone()const{return new TempScale();}
    virtual int Type(){return 2;}
};

class SinScale :public ScaleFunc{
        public:
    SinScale(double a=1.0, double f=1.0, double psy=0.0){_a=a; _f=f; _psy = psy;}
    virtual double F(double v)const{return _a*sin(v*_f + _psy);}
    virtual QString Name()const{return QString::fromLocal8Bit("Sinus scale");}
    virtual QString Units(QString in)const{return QString::fromLocal8Bit("");}
    virtual SinScale* Clone()const{return new SinScale();}
    virtual int Type(){return 3;}
    double _a, _f, _psy;
};

class SqrScale :public ScaleFunc{
        public:
    SqrScale(){}
    virtual double F(double v)const{return v*v*v;}
    virtual QString Name()const{return QString::fromLocal8Bit("Sqr");}
    virtual QString Units(QString in)const{return QString::fromLocal8Bit("");}
    virtual SqrScale* Clone()const{return new SqrScale();}
    virtual int Type(){return 4;}
};


class MyTransform
{
public:
    ~MyTransform();
    MyTransform(const QRectF &view, const QRectF &data, const ScaleFunc &scaleFuncX=LineScale(), const ScaleFunc &scaleFuncY=LineScale());
    QPointF map(const QPointF &point)const;
    QLineF map(const QLineF &line)const;

    void SetXScale(double value)
    {scaleX = value; }
    void SetYScale(double value)
    {scaleY = value; }
    void SetXOffset(double value)
    {dx = value; }
    void SetYOffset(double value)
    {dy = value; }

protected:
    ScaleFunc *funcX, *funcY;
    double scaleX, scaleY, dx, dy;
};

#endif // MYTRANSFORM_H
