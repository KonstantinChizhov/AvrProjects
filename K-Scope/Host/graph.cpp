#include "graph.h"
#include <QtGui/QPainter>
#include <QtGui/QPen>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainterPath>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include "mytransform.h"
#include "exception.h"
//----------------------------------------------------------------------------


int greed_cnty=10;
int greed_cntx=10;

//----------------------------------------------------------------------------	
MyGraph::MyGraph(QWidget *p)
    :QWidget(p)
{
    small_grid=0;
    m_scale=1.0;
    x_org=0.0;
    y_org=0.0;
    vert=new QScrollBar(Qt::Vertical, this);
    hor=new QScrollBar(Qt::Horizontal, this);
    updateScrollBars();
    connect(vert, SIGNAL(valueChanged(int)),
            this,  SLOT(scrollY(int)));
    connect(hor, SIGNAL(valueChanged(int)),
            this,  SLOT(scrollX(int)));

    setFocusPolicy(Qt::StrongFocus);
    small_grid_pen.setColor(Qt::lightGray);

    small_grid_pen.setWidth(0);
    grid_pen.setColor(Qt::black);
    grid_pen.setWidth(2);
    _dataRect = QRectF(0, 500, 1000, -1000);
    _dataXOffset =0.0;
    _dataXScale =1.0;
}
//----------------------------------------------------------------------------
void MyGraph::paintEvent ( QPaintEvent * event )
{
    QPainter p(this);
    this->autoFillBackground();
    //p.setBrush(QBrush(QColor(80, 120, 90)));
    //p.drawRect(rect());
    p.scale(m_scale, m_scale);
    p.translate(-x_org, -y_org);
    Draw(p, rect());
}
//----------------------------------------------------------------------------
void MyGraph::Draw(QPainter &p, const QRectF &rect)
{

    if(m_data.size()<=0) return;
    QRectF dataRect = QRectF(_dataXOffset*_dataXScale,
                             _dataRect.top(),
                             _dataRect.width()*_dataXScale,
                             _dataRect.height());

    MyTransform trans(rect, dataRect);

    drawGrid(p, dataRect, rect);
    //p.setRenderHint (QPainter::Antialiasing, true);
    p.setFont(ax_font);
    drawAxis(p, dataRect, rect);
    draw_arg(p, dataRect, rect);
    drawCurves(p, trans);
    p.setPen(grid_pen);
    p.setFont(header_font);
    p.drawText(QRectF(rect.left(), rect.top(),
                      rect.width(), p.fontMetrics().height()+5),Qt::AlignCenter, m_name);

}
//----------------------------------------------------------------------------
void MyGraph::resizeEvent ( QResizeEvent * event )
{
    updateScrollBars();
}
//----------------------------------------------------------------------------
void MyGraph::mouseMoveEvent ( QMouseEvent * event)
{
    if(event->buttons()&Qt::LeftButton)
    {
        hor->setValue((int)(old_x_org+(click_x-event->x())/m_scale));
        vert->setValue((int)(old_y_org+(click_y-event->y())/m_scale));
        update();
    }
}
//----------------------------------------------------------------------------
void MyGraph::mousePressEvent ( QMouseEvent * event)
{
    if(event->buttons()&Qt::LeftButton)
    {
        click_x=event->x();
        click_y=event->y();
        old_x_org=x_org;
        old_y_org=y_org;
    }
}
//----------------------------------------------------------------------------
void MyGraph::keyPressEvent ( QKeyEvent * event )
{
    switch (event->key()) {
    case Qt::Key_Left:
        hor->setValue(hor->value()-10);
        //x_org-=10;
        break;
    case Qt::Key_Right:
        hor->setValue(hor->value()+10);
        //x_org+=10;
        break;
    case Qt::Key_Down:
        vert->setValue(vert->value()+10);
        //y_org+=10;
        break;
    case Qt::Key_Up:
        vert->setValue(vert->value()-10);
        //y_org-=10;
        break;
    case Qt::Key_Plus:
        ZoomIn();
        break;
    case Qt::Key_Minus:
        ZoomOut();
        break;
        /*case Qt::Key_P:
        small_grid_pen.setColor(Qt::black);
        Print(this);
        small_grid_pen.setColor(Qt::lightGray);
            break;*/
    default:
        QWidget::keyPressEvent(event);
    }

}
//----------------------------------------------------------------------------
void MyGraph::ZoomIn()
{
    if(m_scale<10.0)
        m_scale*=1.25;
    updateScrollBars();
}
//----------------------------------------------------------------------------
void MyGraph::ZoomOut()
{
    if(m_scale>0.1)
        m_scale*=0.8;
    updateScrollBars();
}
//----------------------------------------------------------------------------	
void MyGraph::wheelEvent ( QWheelEvent * event )
{
    if(event->delta()>0)
        ZoomIn();
    else
        ZoomOut();

}
//----------------------------------------------------------------------------
void MyGraph::updateScrollBars()
{
    int w=15;
    vert->setGeometry(rect().right()-w, rect().top(), w, rect().bottom()-w);
    hor->setGeometry(0, rect().bottom()-w, rect().right()-w, w );
    scroll_visible = m_scale>1.01;
    vert->setVisible(scroll_visible);
    hor->setVisible(scroll_visible);

    vert->setMaximum((int)qMax(height()*(m_scale-1)/m_scale,1.0));
    hor->setMaximum((int)qMax(width()*(m_scale-1)/m_scale,1.0));

    vert->setMinimum(0);
    hor->setMinimum(0);

    update();
}
//----------------------------------------------------------------------------
void MyGraph::scrollX(int v)
{
    x_org=v;
    update();
}
//----------------------------------------------------------------------------
void MyGraph::scrollY(int v)
{
    y_org=v;
    update();
}
//----------------------------------------------------------------------------
void MyGraph::drawGrid(QPainter &p, const QRectF &dataRect, const QRectF &viewRect)
{
    if(viewRect.width()<=0.0 || viewRect.height()<=0.0) return;
    double x, y, dx, dy, endx, endy, sx, sy;
    double greed_stepx=(viewRect.width())/greed_cntx;
    double greed_stepy=(viewRect.height())/greed_cnty;
    endx=viewRect.right();
    endy=viewRect.bottom();
    sx=viewRect.left();
    sy=viewRect.top();

    if(small_grid && viewRect.width()>2*greed_cntx*greed_cntx &&
       viewRect.height()>2*greed_cnty*greed_cnty)
    {
        p.setPen(small_grid_pen);
        dx=greed_stepx/greed_cntx;
        dy=greed_stepy/greed_cnty;
        for(x=sx; x<=endx;x+=dx)
        {
            p.drawLine(QLineF(x, sy, x, endy));
        }
        for(y=sy; y<=endy;y+=dy)
        {
            p.drawLine(QLineF(sx, y, endx, y));
        }
    }

    p.setPen(grid_pen);

    for(x=sx; x<=endx;x+=greed_stepx)
    {
        p.drawLine(QLineF(x, sy, x, endy));
    }
    for(y=sy; y<=endy+1;y+=greed_stepy)
    {
        p.drawLine(QLineF(sx, y, endx, y));
    }
}
//----------------------------------------------------------------------------
int MyGraph::CurvesCount()
{
    return m_data.size();
}
//----------------------------------------------------------------------------
void MyGraph::drawCurves(QPainter &p, const MyTransform &transform)
{
    for(int i=0; i<CurvesCount(); i++)
    {
        CurveAt(i).draw(p, transform);
    }
}
//----------------------------------------------------------------------------
void MyGraph::drawAxis(QPainter &p, const QRectF &dataRect, const QRectF &viewRect)
{

}
//----------------------------------------------------------------------------
void MyGraph::draw_arg(QPainter &p, const QRectF &dataRect, const QRectF &viewRect)
{

}

//----------------------------------------------------------------------------
void MyGraph::Clear()
{
    for(int i=0; i<m_data.size(); i++)
    {
        delete m_data[i];
    }
    m_data.clear();
}
//----------------------------------------------------------------------------
Curve &MyGraph::CreateNewCurve(AX_POS attachPos)
{
    Curve *ngr=new Curve();

    switch(attachPos)
    {
    case LEFT:
        //sc_left.Add(ngr);
        break;
    case RIGHT:
        //sc_right.Add(ngr);
        break;
    }
    m_data.push_back(ngr);
    ngr->setPen(QPen(QBrush(Qt::green), 2, Qt::SolidLine, Qt::FlatCap));
    return *ngr;
}

//----------------------------------------------------------------------------
QString MyGraph::ArgUnits()
{
    return arg_units;
}
//----------------------------------------------------------------------------
QString MyGraph::ArgName()
{
    return arg_name;
}
//----------------------------------------------------------------------------
void MyGraph::Remove(int n)
{
    delete m_data.at(n);
    m_data.remove(n);
}
//----------------------------------------------------------------------------	

void MyGraph::setHeaderFont(const QFont &f)
{
    header_font=f;
}
//----------------------------------------------------------------------------
void MyGraph::setAxisFont(const QFont &f)
{
    ax_font=f;
}

void MyGraph::setGridPen(const QPen &pen)
{
    grid_pen=pen;
}


void MyGraph::setArgUnits(const QString &str){arg_units=str;}
void MyGraph::setArgName(const QString &str){arg_name=str;}
void MyGraph::setName(const QString &name){m_name=name;}

Curve &MyGraph::CurveAt(int n)
{
    return *m_data.at(n);
}

void MyGraph::SetDataScaleX(double value)
{
    _dataXScale = value;
}

void MyGraph::SetDataOffsetX(double value)
{
    _dataXOffset = value;
}

void MyGraph::SetDataRect(const QRectF &value)
{
    _dataRect = value;
}
