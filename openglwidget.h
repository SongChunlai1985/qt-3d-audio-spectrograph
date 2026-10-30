#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include "QtCore/QTimer"

#include <opencv2/core.hpp>
//#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>

#ifdef usedlib
#include <dlibfr.h>
#endif

#include <dt.h>

#include <aud.h>

//#include <conv.h>

static float cm = 39.6158f;

struct ps
{
    float x, y, z;
    ps(){ x = 0; y = 0; z = 0;}
    ps(float X, float Y, float Z){ x = X; y = Y; z = Z;}
    ps(QVector3D v){ x = v.x(); y = v.y(); z = v.z();}
};

struct cl
{
    float r, g, b;
    cl(){}
    cl(float R, float G, float B){ r = R; g = G; b = B;}
};

struct dot
{
    ps p;
    cl  c;
    dot() {}
    dot(ps P, cl C){ p = P; c = C;}
};

struct tgl
{
    GLuint a, b, c;
    tgl(){}
    tgl(GLuint A, GLuint B, GLuint C){ a = A; b = B; c = C; }
};

struct ibox
{
    QVector3D mv;
    float rv;
    QVector3D rx;
    float sc;
    QVector3D boxo;
    ibox(QVector3D mv_ = QVector3D(0,0,0),
         float rv_ = 1,
         QVector3D rx_ = QVector3D(1,1,1),
         float sc_ = 0.01f)
    {
        mv = mv_;
        rv = rv_;
        rx = rx_;
        sc = sc_;
    }
};

class OpenGLWidget
        : public QOpenGLWidget,
        protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget() override;

    float lr = 0;
    void fun1();
    void fun2();
    void fun3();
    void makeObject();
    void makeO2();
    void makeObject2();
    void makeO3(uint colstp,
                 uint rowstp,
                 cv::Mat pic,
                 QVector<dot> &verticesN,
                 QVector<tgl> &indicesN,
                 QOpenGLVertexArrayObject &m_vaoN,
                 QOpenGLBuffer &m_vboN,
                 QOpenGLBuffer &m_eboN);
    void makeObject3();

    void line(dot a, dot b);
    void box( QVector3D gMovef,
              float agl,
              QVector3D gRotatef,
              float gScalef);
    void dbox(QVector3D gMovev,
              float Rotatevalue,
              QVector3D gRotatev,
              float gScalef);

    void play();
    int QByteArray2fqmat(QByteArray &buf, int width, int mov,
                         cv::Mat &fq, int r = 0);
    void redrawi();
    void redrawo();

protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QOpenGLVertexArrayObject m_vao;
    QOpenGLVertexArrayObject m_vao2;
    QOpenGLVertexArrayObject m_vao3;
    QOpenGLVertexArrayObject m_vao4;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ebo;
    QOpenGLBuffer m_vbo2;
    QOpenGLBuffer m_ebo2;
    QOpenGLBuffer m_vbo3;
    QOpenGLBuffer m_ebo3;
    QOpenGLBuffer m_vbo4;
    QOpenGLBuffer m_ebo4;

    float m_scale = 1.0;
    float m_xmove = 0.0;
    float m_ymove = 0.0;
    float m_zmove = 0.0;
    float m_rotate = 0;

    QVector<dot> vertices;
    QVector<tgl> indices;

    QMatrix4x4 gCam;
    QMatrix4x4 gWorld;
    QMatrix4x4 gProj;

    QTimer *m_timer = new QTimer;
    QTimer *m_timerf = new QTimer;
    QTimer *m_timerc = new QTimer;
    QOpenGLShaderProgram *m_shaderProgram;

    QPoint m_lastPos;

    QVector3D mo = QVector3D(0, 0, 0);

    QVector3D mz = QVector3D(0, 0, 10 * cm);
    QVector3D my = QVector3D(0, 10 * cm, 0);
    QVector3D mx = QVector3D(10 * cm, 0, 0);

    QVector3D center = QVector3D(0, 0, 45 * cm);
    QVector3D eye = QVector3D(0, 0, 0);

    cv::Mat ma;
    cv::Mat mb;
   // cv::VideoCapture cap;
#ifdef usedlib
    dlibfr fr;
#endif

    dt dat;

    aud au;
    bool run = 1;
    int endrun = 0;

    cv::Mat el;
    cv::Mat er;
    QVector<dot> vertices2;
    QVector<GLuint> indices2;

    int rcd = -1;

    QVector<dot> vertices3;
    QVector<tgl> indices3;
    int playing = 0;

    float cln = 220.0f;
    cv::Mat hanmingFilter;

    void makesound();

    int winw = 640;
    int winh = 480;

    QVector<ibox> iboxs;
    QVector3D gmo;
    QVector3D gwt;

    int btn = -1;

    int mod = 1;

    QVector<dot> vertices4;
    QVector<tgl> indices4;

    cv::Mat ml;
    cv::Mat mr;
    cv::Mat mc;

    QMatrix4x4 gWorld4;

    float px = 0;

    int pstart = 0;
    int pend = 0;
    int plen = -1;

    QVector3D py;

    QByteArray bf;
    QByteArray bfo;
    QByteArray bfc;

    int rcdc = 0;
    int rcdcf = 0;
};

#endif /*OPENGLWIDGET_H*/
