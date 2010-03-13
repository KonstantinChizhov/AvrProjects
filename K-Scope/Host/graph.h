#ifndef GRAPH_H
#define GRAPH_H

#include <QtGui/QWidget>
#include <QtGui/QScrollBar>
#include <QtCore/Qstring>
#include <QtCore/QVector>
#include <QtGui/QPolygonF>
#include <QtGui/QPen>
#include <math.h>
#include "gr_data.h"

class MyGraph :public QWidget{
    Q_OBJECT
public:
    MyGraph(QWidget *p=0);
    enum AX_POS{LEFT, RIGHT};
    Curve &MyGraph::CreateNewCurve(AX_POS attachPos);
    void Clear();
    int CurvesCount();
    void setGridPen(const QPen &pen);
    void Draw(QPainter &p, const QRectF &rect);
    void setArgUnits(const QString &str);
    void setArgName(const QString &str);
    QString ArgUnits();
    QString ArgName();

    void setName(const QString &name);
    void setHeaderFont(const QFont &f);
    void setAxisFont(const QFont &f);
    void Remove(int n);
    Curve &CurveAt(int n);

    void SetDataScaleX(double value);
    void SetDataOffsetX(double value);

    void SetDataRect(const QRectF &value);

public slots:
    void scrollX(int v);
    void scrollY(int v);
    void ZoomIn();
    void ZoomOut();
protected:
    void paintEvent ( QPaintEvent * event );
    void resizeEvent ( QResizeEvent * event );
    void keyPressEvent ( QKeyEvent * event );
    void mouseMoveEvent ( QMouseEvent * event );
    void mousePressEvent ( QMouseEvent * event);
    void wheelEvent ( QWheelEvent * event );
    void drawGrid(class QPainter &p, const QRectF &dataRect, const QRectF &viewRect);
    void drawCurves(class QPainter &p, const MyTransform &transform);
    void drawAxis(class QPainter &p, const QRectF &dataRect, const QRectF &viewRect);
    void draw_arg(QPainter &p, const QRectF &dataRect, const QRectF &viewRect);

    void updateScrollBars();
    QVector<Curve*> m_data;

    bool small_grid;
    qreal m_scale;
    qreal x_org, y_org, old_x_org, old_y_org;
    QString arg_units, arg_name;
    QString m_name;
    QFont ax_font, header_font;
    QScrollBar *vert, *hor;
    bool scroll_visible;
    QPen grid_pen, small_grid_pen;
    int click_x, click_y;
    QRectF _dataRect;
    double _dataXScale, _dataXOffset;
};


#endif
