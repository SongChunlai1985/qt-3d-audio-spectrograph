#include "openglwidget.h"
#include "QtGui/QWheelEvent"
#include "QtGui/QMouseEvent"
#include <QOpenGLExtraFunctions>
#include <QtConcurrent>
#include <math.h>

void OpenGLWidget::fun1()
{
#if 0
    QJsonObject fnames = dat.d["人名"].toObject();
    fnames.insert("fda20221017155528008", "宋春来");
    dat.d.insert("人名", fnames);
    dat.write();
#endif

#ifdef usedlib
    while(run)
    {

        cap.read(ma);
        std::vector<dlib::matrix<float,0,1>> face_descriptors;
        std::vector<dlib::rectangle> Rtgs = fr.rtgs(dlib::cv_image<dlib::rgb_pixel>(ma), face_descriptors);
        foreach (dlib::rectangle face, Rtgs) {
            cv::rectangle(ma, cv::Point(face.left(),face.top()),
                          cv::Point(face.right(), face.bottom()), cv::Scalar(255,0,0,1));
        }

        QJsonArray matchj;
        if(face_descriptors.size())
        {
            int q = 0;
            foreach (auto face_descriptor, face_descriptors)
            {
                float cmc = 0.0f;
                QString ckey;
                int cp = 0;

                foreach (QString key, dat.d.keys())
                {
                    std::vector<dlib::matrix<float,0,1>> descriptors = QJsonArray2face_descriptors(dat.d[key].toArray());
                    int p = 0;
                    foreach (auto descriptor, descriptors)
                    {
                        float mc = (1 - dlib::length(descriptor - face_descriptor)) * 100;
                        if( mc > cmc )
                        {
                            cmc = mc;
                            ckey = key;
                            cp = p;
                        }
                        p++;
                    }
                }
                QJsonObject matcho;
                matcho.insert("id", q);
                matcho.insert("key", ckey);
                matcho.insert("subid", cp);
                matcho.insert("match", double(cmc));

                matchj.push_back(matcho);
                q++;
            }
            qDebug() << "--------";
        }

        foreach (QJsonValue m, matchj) {
            QJsonObject mc = m.toObject();
            double match = mc["match"].toDouble();
            qDebug() << mc["id"].toInt() << "/" << matchj.size()
                                         << mc["key"].toString() <<" : "<< mc["subid"].toInt()
                                         << " match = " << match << "%" ;
            if(match < 60)
            {
                qDebug() << "UNKNOW ADD NEW";
                std::vector<dlib::matrix<float,0,1>> face_descriptors2;
                face_descriptors2.push_back(face_descriptors[uint(mc["id"].toInt())]);
                QJsonArray fda = face_descriptors2QJsonArray(face_descriptors);
                dat.d.insert("fda" + QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz"), fda);
                dat.write();
            }
            else
            {
                qDebug() << dat.d["人名"].toObject()[mc["key"].toString()].toString() << "你好！";
            }
        }

        cv::imshow("ma", ma);

        cv::waitKey(1);
    }
#endif
    endrun ++;
}

void OpenGLWidget::fun2()
{
#if 0
    while(run)
    {
        cap.read(mb);
        cv::imshow("mb", mb);
        cv::waitKey(1);
    }
#endif
    endrun ++;
}

void OpenGLWidget::fun3()
{

}

int QByteArray2short(QByteArray sp)
{
    int sps = sp[0] & 0x000000FF;
    sps |= ((sp[1] << 8) & 0x0000FF00);
    return  sps;
}

QByteArray short2QByteArray(int sps)
{
    QByteArray bfof;
    sps = sps > 65535 ? 65535 : sps;
    sps = sps < -65535 ? -65535 : sps;
    bfof.push_back(char(0x000000ff & sps));
    bfof.push_back(char((0x0000ff00 & sps) >> 8));
    return  bfof;
}

int OpenGLWidget::QByteArray2fqmat(QByteArray &buf, int width, int mov, cv::Mat &fq, int r)
{
    if(!buf.size())return 0;
    if(hanmingFilter.rows != width / 4)
    {
        float alpha = 0.46f;
        hanmingFilter = cv::Mat(width / 4, 1, CV_32FC1);
        for (int i = 0; i < width / 4; ++i)
        {
            hanmingFilter.at<float>(0, i) =
                    (1.0f - alpha) - alpha * float(cos(2.0f * float(M_PI) * i / (width / 4 - 1.0f)));
        }
    }

    int last = (buf.size() - width) / mov;

    cv::Mat aui0;
    cv::Mat alf;
    cv::Mat al;
    cv::Mat aui;
    QByteArray win;
    for (int i = 0; i < last; ++i)
    {
        win = buf.mid(i * mov, width);
        aui = cv::Mat(win.size()/4, 2, CV_16SC1, win.data());
        aui0 = aui.col(r);
        aui0.convertTo(alf, CV_32FC1);
        alf = alf.mul(hanmingFilter);

        if(mod == 0)
        {
            fq.push_back(alf.t());
        }

        if(mod == 1)
        {
            cv::dft(alf, al);
            fq.push_back(al.t());
        }

        if(mod == 2)
        {
            cv::dft(alf, al);
            cv::dct(al, alf);
            fq.push_back(alf.t());
        }

    }
    cv::flip(fq, fq, 1);

    fq = fq.t();
    //    cv::imshow("al8", fql);
    //    cv::imshow("ar8", fqr);
    return 0;
}

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_vao(),
      m_vao2(),
      m_vao3(),
      m_vao4(),
      m_vbo(QOpenGLBuffer::VertexBuffer),
      m_ebo(QOpenGLBuffer::IndexBuffer),
      m_vbo2(QOpenGLBuffer::VertexBuffer),
      m_ebo2(QOpenGLBuffer::IndexBuffer),
      m_vbo3(QOpenGLBuffer::VertexBuffer),
      m_ebo3(QOpenGLBuffer::IndexBuffer),
      m_vbo4(QOpenGLBuffer::VertexBuffer),
      m_ebo4(QOpenGLBuffer::IndexBuffer)
{
    m_timer->start(10);
    m_timerc->start(40);
    m_timerf->start(100);
    dat.read();
    //cap.open(0);
#if 0
    ma = cv::imread("D:\\b.jpg");
    qDebug() << "ma: " << ma.size().width << ma.size().height;
#endif
    QtConcurrent::run(this, &OpenGLWidget::fun1);
    QtConcurrent::run(this, &OpenGLWidget::fun2);

    connect(m_timer, &QTimer::timeout, this, [this]()
    {
        m_scale += 0.1f;
        m_xmove += 0.1f;
        m_ymove += 0.1f;
        m_zmove += 0.1f;
        m_rotate += 0.1f;
        px -= 10;
    });

    connect(m_timerc, &QTimer::timeout, this, [this]()
    {
        for (int i = 0; i < 2; ++i) {
            vertices2.pop_back();
            vertices2.pop_back();
            indices2.pop_back();
            indices2.pop_back();
        }

        line(dot(ps(px, 1 * cm, - 200), cl(1, 1, 0)), dot(ps(px, 1 * cm, ml.rows * 5 + 200), cl(0, 1, 1)));
        int pxe = - int(pend / 441 * 5 + (6 * cm));
        line(dot(ps(pxe, 1 * cm, - 200), cl(1, 0, 0)), dot(ps(pxe, 1 * cm, ml.rows * 5 + 200), cl(1, 0, 0)));
        makeO2();
        update();
    });

    connect(m_timerf, &QTimer::timeout, this, [this]()
    {
        if(rcdc && rcdcf)
        {
            mc = cv::Mat();
            QByteArray2fqmat(bfc, 44100 * 4 * 25 / 1000, 44100 * 4 * 10 / 1000, mc, 0);
            makeO3(5, 5, mc, vertices4, indices4, m_vao4, m_vbo4, m_ebo4);

            gWorld4.setToIdentity();
            gWorld4.translate(QVector3D(mc.cols * 5 * -1 - 6 * cm, 0, 0));
        }
    });

    connect(au.streamIn, &QIODevice::readyRead, this, [this]()
    {
        QByteArray buf = au.streamIn->readAll();
#if 0
        int sz = buf.size();
        int szs = sz/4;
        QByteArray bufo;
        for (int i = 0; i < szs; ++i) {
            QByteArray bfof ;
            int spls = QByteArray2short(buf.mid(i * 4, 2));
            bufo.push_back(short2QByteArray(spls * 1));

            int sprs = QByteArray2short(buf.mid(i * 4 + 2, 2));
            bufo.push_back(short2QByteArray(sprs * 1));
        }
        au.streamOut->write(bufo);
#endif
        if(rcd == 1)
        {
            bf += buf;
            qDebug() << "录音:" << bf.size();
        }

        int len = 4410 * 50;
        if(rcdc == 1)
        {
            bfc += buf;
            if(bfc.length() > len)
            {
                bfc = bfc.right(len);
                rcdcf = 1;
            }
        }

    });
    setMouseTracking(true);

    for (int i = 0; i < 25; ++i)
    {
        iboxs.push_back(ibox(QVector3D(-3 * cm, -1 * cm, i * 5 * cm)));
    }

}

OpenGLWidget::~OpenGLWidget()
{
    m_vao.destroy();
    m_vbo.destroy();
    m_ebo.destroy();
    m_vao2.destroy();
    m_vbo2.destroy();
    m_ebo2.destroy();
    m_vao3.destroy();
    m_vbo3.destroy();
    m_ebo3.destroy();
    m_vao4.destroy();
    m_vbo4.destroy();
    m_ebo4.destroy();

    m_shaderProgram->release();

    run = 0;
    while(endrun < 2)
    {
        cv::waitKey(1);
    }
}

static const char *vertexShader =
        "#version 330 core\n"
        "in mediump vec3 vPosition;\n"
        "in mediump vec3 vColor;\n"
        "out mediump vec4 outColor;\n"
        "uniform mediump mat4 gWorld;\n"
        "uniform mediump mat4 gCam;\n"
        "uniform mediump mat4 gProj;\n"
        "void main(){\n"
        "   gl_Position = gProj * (gCam * (gWorld * vec4(vPosition, 1.0f)));\n"
        "   outColor = vec4(vColor, 1.0f);\n"
        "}\n";

static const char *fragmentShader =
        "#version 330 core\n"
        "in mediump vec4 outColor;\n"
        "out mediump vec4 FragColor;\n"
        "void main(){\n"
        "   FragColor = outColor;\n"
        "}\n";

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    f->glClearDepthf(1.0);
    f->glEnable(GL_DEPTH_TEST);
    f->glDepthFunc(GL_LESS);
    makeCurrent();

    m_vao.create();
    m_vbo.create();
    m_ebo.create();

    m_vao2.create();
    m_vbo2.create();
    m_ebo2.create();

    m_vao3.create();
    m_vbo3.create();
    m_ebo3.create();

    m_vao4.create();
    m_vbo4.create();
    m_ebo4.create();

    m_shaderProgram = new QOpenGLShaderProgram();
    m_shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader);
    m_shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader);

    m_shaderProgram->bind();

    makeObject();
    makeObject2();
    makeObject3();

    gCam.setToIdentity();
    gCam.lookAt(eye, center, my);

    m_shaderProgram->setUniformValue("gCam", gCam);

    gWorld.setToIdentity();
    gWorld.translate(center);

    gWorld4.setToIdentity();
    gWorld4.translate(QVector3D(-20 * cm, 0, 0));
}

void OpenGLWidget::makeObject()
{
    makeCurrent();

    vertices.push_back(dot(ps(-1000.0f, -1000.0f, -1000.0f), cl(1.0f, 1.0f, 1.0f)));
    vertices.push_back(dot(ps(+1000.0f, -1000.0f, -1000.0f), cl(1.0f, 1.0f, 0.0f)));
    vertices.push_back(dot(ps(-1000.0f, +1000.0f, -1000.0f), cl(1.0f, 0.0f, 1.0f)));
    vertices.push_back(dot(ps(+1000.0f, +1000.0f, -1000.0f), cl(1.0f, 0.0f, 0.0f)));

    vertices.push_back(dot(ps(-1000.0f, -1000.0f, +1000.0f), cl(0.0f, 1.0f, 1.0f)));
    vertices.push_back(dot(ps(+1000.0f, -1000.0f, +1000.0f), cl(0.0f, 1.0f, 0.0f)));
    vertices.push_back(dot(ps(-1000.0f, +1000.0f, +1000.0f), cl(0.0f, 0.0f, 1.0f)));
    vertices.push_back(dot(ps(+1000.0f, +1000.0f, +1000.0f), cl(0.0f, 0.0f, 0.0f)));

    indices.push_back(tgl(0, 2, 1));
    indices.push_back(tgl(1, 2, 3));
    indices.push_back(tgl(4, 5, 6));
    indices.push_back(tgl(6, 5, 7));

    indices.push_back(tgl(4, 6, 0));
    indices.push_back(tgl(0, 6, 2));
    indices.push_back(tgl(5, 1, 7));
    indices.push_back(tgl(7, 1, 3));

    indices.push_back(tgl(0, 5, 4));
    indices.push_back(tgl(5, 0, 1));
    indices.push_back(tgl(2, 6, 7));
    indices.push_back(tgl(7, 3, 2));

    m_vao.bind();
    m_vbo.bind();
    m_ebo.bind();

    m_vbo.allocate(vertices.data(), vertices.size() * 6 * int(sizeof(GLfloat)));
    m_ebo.allocate(indices.data(), indices.size() * 3 * int(sizeof(GLuint)));

    int attr;

    attr = m_shaderProgram->attributeLocation("vPosition");
    m_shaderProgram->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
    m_shaderProgram->enableAttributeArray(attr);

    attr = m_shaderProgram->attributeLocation("vColor");
    m_shaderProgram->setAttributeBuffer(attr, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 6);
    m_shaderProgram->enableAttributeArray(attr);

}

void OpenGLWidget::makeO2()
{
    makeCurrent();

    m_vao2.bind();
    m_vbo2.bind();
    m_ebo2.bind();

    m_vbo2.allocate(vertices2.data(), vertices2.size() * 6 * int(sizeof(GLfloat)));
    m_ebo2.allocate(indices2.data(), indices2.size() * int(sizeof(GLuint)));

    int attr;

    attr = m_shaderProgram->attributeLocation("vPosition");
    m_shaderProgram->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
    m_shaderProgram->enableAttributeArray(attr);

    attr = m_shaderProgram->attributeLocation("vColor");
    m_shaderProgram->setAttributeBuffer(attr, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 6);
    m_shaderProgram->enableAttributeArray(attr);
}

void OpenGLWidget::makeObject2()
{
    line(dot(ps(0, 0, 0), cl(0, 0, 0)), dot(ps(mx), cl(1, 0, 0)));
    line(dot(ps(0, 0, 0), cl(0, 0, 0)), dot(ps(my), cl(0, 1, 0)));
    line(dot(ps(0, 0, 0), cl(0, 0, 0)), dot(ps(mz), cl(0, 0, 1)));

    makeO2();
}

cl p2cl(float p)
{
    cl c;
    if(p >=0.75f             ){ c.r = 1.0f;           c.g = 1.0f - (p - 0.75f) * 4; c.b = 0.0f;                   return c;}
    if(p >= 0.5f && p < 0.75f){ c.r = (p - 0.5f) * 4; c.g = 1.0f;                   c.b = 0.0f;                   return c;}
    if(p >=0.25f && p <  0.5f){ c.r = 0.0f;           c.g = 1.0f;                   c.b = 1.0f - (p - 0.25f) * 4; return c;}
    if(p < 0.25f             ){ c.r = 0.0f;           c.g = p * 4;                  c.b = 1.0f;                   return c;}
    return c;
}

void OpenGLWidget::makeO3( uint colstp,
                           uint rowstp,
                           cv::Mat pic,
                           QVector<dot> &verticesN,
                           QVector<tgl> &indicesN,
                           QOpenGLVertexArrayObject &m_vaoN,
                           QOpenGLBuffer &m_vboN,
                           QOpenGLBuffer &m_eboN
                           )
{
    makeCurrent();
    verticesN.clear();
    indicesN.clear();
    uint col = uint(pic.cols);
    uint row = uint(pic.rows);
    if(!col || !row) return;
    for (float z = 0; z < row; z++)
    {
        for(float x = 0; x < col; x++)
        {
            float p = pic.at<float>(int(row - z - 1), int(col - x - 1));
            float y = 0.0;
            if(mod == 1)
                y = p < 0.0f ? -sqrt(-p) / 2 : sqrt(p) / 2;
            if(mod == 0 || mod == 2)
                y = p < 0.0f ? -sqrt(-p) * 2 : sqrt(p) * 2;
            verticesN.push_back(dot(ps(x * colstp, y, z * rowstp), p2cl(y / cln)));
        }
    }
    for (uint j = 0; j < row - 1; ++j)
    {
        for (uint i =  j * col; i <  col - 1 + j * col; ++i)
        {
            indicesN.push_back(tgl(  i,    i + 1,  i + col + 1));
            indicesN.push_back(tgl(  i + col + 1,  i + col,  i));
        }
    }
#if 0
    for (int i = 0; i < indices3.size(); ++i)
    {
        std::cout << indices3[i].a << " "
                  << indices3[i].b << " "
                  << indices3[i].c << ", ";
    }
    std::cout << std::endl;
#endif

    m_vaoN.bind();
    m_vboN.bind();
    m_eboN.bind();

    m_vboN.allocate(verticesN.data(), verticesN.size() * 6 * int(sizeof(GLfloat)));
    m_eboN.allocate(indicesN.data(), indicesN.size() * 3 * int(sizeof(GLuint)));

    int attr;

    attr = m_shaderProgram->attributeLocation("vPosition");
    m_shaderProgram->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
    m_shaderProgram->enableAttributeArray(attr);

    attr = m_shaderProgram->attributeLocation("vColor");
    m_shaderProgram->setAttributeBuffer(attr, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 6);
    m_shaderProgram->enableAttributeArray(attr);

}

void OpenGLWidget::makeObject3()
{
    cv::Mat pic(882, 200, CV_32FC1);
    cv::randn(pic, 0, 50000);
    makeO3(5, 5, pic, vertices3, indices3, m_vao3, m_vbo3, m_ebo3);
}

void OpenGLWidget::line(dot a, dot b)
{
    vertices2.push_back(a);
    indices2.push_back(uint(indices2.size()));
    vertices2.push_back(b);
    indices2.push_back(uint(indices2.size()));
}

void OpenGLWidget::resizeGL(int w, int h)
{   
    gProj.setToIdentity();
    float gam = atan(/*6.05f * cm*/(h / 2) / (45 * cm)) / 3.1415926535f * 180 * 2;
    gProj.perspective(/*15.3144f*/ gam, GLfloat(w) / h, 0.1f, FLT_MAX);
    m_shaderProgram->setUniformValue("gProj", gProj);

    qDebug()<< "GL: " << w << h
            << "\n" << "gProj" << gProj
            << "\n" << "gCam" << gCam
            << "gam:" << gam;

    winw = w;
    winh = h;
}

void OpenGLWidget::box(QVector3D gMovev,
                       float Rotateagl,
                       QVector3D gRotatev,
                       float gScalef)
{
    QMatrix4x4 gScale, gMove, gRotate;
    gMove.setToIdentity();
    gRotate.setToIdentity();
    gScale.setToIdentity();
    gMove.translate(gMovev);
    gRotate.rotate(Rotateagl, gRotatev);
    gScale.scale(gScalef);

    QMatrix4x4 gWorldb = gMove * gRotate * gScale;
    m_shaderProgram->setUniformValue("gWorld", gWorld * gWorldb);
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    f->glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, nullptr);
}

void OpenGLWidget::dbox(QVector3D gMovev,
                        float Rotatevalue,
                        QVector3D gRotatev,
                        float gScalef)
{
    box(gMovev + QVector3D(sin(m_xmove), sin(m_ymove), sin(m_zmove)),
        Rotatevalue * m_rotate,
        gRotatev,
        gScalef * (sin(m_scale) + 2));
}

float dst(QVector3D a, QVector3D b, QVector3D p)
{
    QVector3D v0 =  b - a;
    QVector3D v1 = QVector3D::crossProduct(p - a, v0);
    return v1.length() / v0.length();
}

QVector3D py0(QVector3D a, QVector3D b,
              QVector3D o = QVector3D(0, 0, 0), QVector3D n = QVector3D(0, 1, 0))
{
    QVector3D qb = a - QVector3D::dotProduct((a - o), n) / n.length() * n;
    float cst = QVector3D::dotProduct((b - a), (qb - a)) /
            ((b - a).length() * (qb - a).length());
    float ap = (qb - a).length() / cst;
    return a + ap * (b - a) / (b - a).length();
}

void OpenGLWidget::paintGL()
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

    m_shaderProgram->setUniformValue("gWorld", gWorld * gWorld4);

    m_vao4.bind();
    f->glDrawElements(GL_TRIANGLES, indices4.size() * 3, GL_UNSIGNED_INT, nullptr);

    m_shaderProgram->setUniformValue("gWorld", gWorld);

    m_vao3.bind();
    f->glDrawElements(GL_TRIANGLES, indices3.size() * 3, GL_UNSIGNED_INT, nullptr);

    m_vao2.bind();
    f->glDrawArrays(GL_LINES, 0, vertices2.size());

    m_vao.bind();
    int hbtn = 0;
    for (int i = 0; i < iboxs.size(); ++i)
    {
        if(dst(gmo, gwt, iboxs[i].mv) < 25)
        {
            iboxs[i].rv = 30;
            iboxs[i].sc = 0.024f;
            btn = i;
            hbtn ++;
        }
        else
        {
            iboxs[i].rv = 10;
            iboxs[i].sc = 0.010f;
        }
        dbox(iboxs[i].mv, iboxs[i].rv, iboxs[i].rx, iboxs[i].sc);
    }
    if(!hbtn)btn = -1;
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
    if(event->buttons() & Qt::LeftButton)
    {
        switch (btn)
        {
        case 0:
            rcd = 1;
            qDebug() << "开始录音";
            break;
        case 1:
            rcd = 0;
            qDebug() << "停止录音";
            redrawi();
            break;
        case 2:
            bfo = bf;
            qDebug() << "播放录音";
            redrawo();
            pstart = 0;
            plen = -1;
            QtConcurrent::run(this, &OpenGLWidget::play);
            break;
        case 3:
            rcd = 0;
            qDebug() << "制作声音";
            makesound();
            pstart = 0;
            plen = -1;
            QtConcurrent::run(this, &OpenGLWidget::play);
            redrawo();
            break;
        case 4:
            bf = "";
            qDebug() << "录音清除";
            redrawi();
            break;
        case 5:
            cln *= 1.05f;
            redrawi();
            break;
        case 6:
            cln /= 1.05f;
            redrawi();
            break;
        case 7:
            mod = 0;
            redrawi();
            break;
        case 8:
            mod = 1;
            redrawi();
            break;
        case 9:
            mod = 2;
            redrawi();
            break;
        case 10:
            rcdc = 1;
            qDebug() << "实时显示";
            break;
        case 11:
            rcdc = 0;
            qDebug() << "不实时显示";
            break;
        }
    }
    if(py.x() < -6 * cm && py.x() > -6 * cm - (ml.cols * 5)
            && py.z() > 0 && py.z() < ml.rows * 5)
    {

        qDebug() << "py0:" << py;
        if(event->buttons() & Qt::RightButton)
        {
            pend = - int(py.x() - (-6 * cm)) / 5 * 441;
        }
        if(event->buttons() & Qt::LeftButton)
        {
            pstart = - int(py.x() - (-6 * cm)) / 5 * 441;
            QtConcurrent::run(this, &OpenGLWidget::play);
        }
    }

}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    QMatrix4x4 gWorldi = gWorld.inverted();

    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        gWorld.translate(0.02f * dx * (gWorldi * mx - gWorldi * mo));
        gWorld.translate(0.02f * dy * (gWorldi * my - gWorldi * mo));
    }

    else if (event->buttons() & Qt::MidButton)
    {
        gWorld.translate(0.02f * dy * (gWorldi * mz - gWorldi * mo));

        gWorld.translate((gWorldi * mo));
        gWorld.rotate(0.2f * dx, gWorldi * mz - gWorldi * mo);
        gWorld.translate(-(gWorldi * mo));
    }

    else if (event->buttons() & Qt::RightButton)
    {
        gWorld.translate((gWorldi * mo));
        gWorld.rotate(0.2f * dx, gWorldi * my - gWorldi * mo);
        gWorld.rotate(0.2f * -dy, gWorldi * mx - gWorldi * mo);
        gWorld.translate(-(gWorldi * mo));
    }

    QVector3D t(-(event->x() - winw / 2), -(event->y() - winh / 2), center.z());
    gmo = gWorldi * mo;
    gwt = gWorldi * t;
    //    qDebug() << "t: " << mo << t << gmo << gwt;
    //    qDebug() << "dst:" << dst(gmo, gwt, mo);
    py = py0(gmo, gwt);
    m_lastPos = event->pos();
}

void OpenGLWidget::play()
{
    if(playing)return;
    playing = 1;
    plen = (pend - pstart) * 4;
    QByteArray bf = bfo.mid(pstart * 4, plen);
    int q = 0;
    px = - 6 * cm + -(pstart / 441) * 5 ;
    while(bf.size())
    {
        qint64 n = au.streamOut->write(bf);
        if(n > 0)
        {
            bf = bf.mid(int(n));
            q += n;
        }
        usleep(2);
    }

    qDebug() << "播放完毕";
    playing = 0;
}

void OpenGLWidget::redrawi()
{
    el = cv::Mat();
    er = cv::Mat();
    QByteArray2fqmat(bf, 44100 * 4 * 25 / 1000, 44100 * 4 * 10 / 1000, el);
    makeO3(5, 5, el, vertices3, indices3, m_vao3, m_vbo3, m_ebo3);
    qDebug() <<  "cln: " << cln;
}

void OpenGLWidget::redrawo()
{
    ml = cv::Mat();
    mr = cv::Mat();
    QByteArray2fqmat(bfo, 44100 * 4 * 25 / 1000, 44100 * 4 * 10 / 1000, ml);
    makeO3(5, 5, ml, vertices4, indices4, m_vao4, m_vbo4, m_ebo4);
    qDebug() <<  "cln: " << cln;

    gWorld4.setToIdentity();
    gWorld4.translate(QVector3D(ml.cols * 5 * -1 - 6 * cm, 0, 0));
}

int rnd(int n)
{
    return (n == 0) ? 0 : (rand() % n);
}

void OpenGLWidget::makesound()
{
    bfo.clear();
    int bone = 42;

    QVector<int> pw;
    QVector<int> pwn;
    QVector<double> kt;
    QVector<double> fq;
    QVector<double> st;
    QVector<double> dr;

    double bgn = 0.010;
    double stp = 0.021;

    int smps = 30000;

    for (int j = 0; j < bone; ++j)
    {
        fq.push_back(stp * j + bgn);
        pw.push_back(0);
        pwn.push_back(0);
        kt.push_back(0);
        st.push_back(0);
        dr.push_back(smps);
    }

    pw[2] = 250;
    pw[3] = 300;

    st[2] = 8000;
    st[3] = 8000;

    pwn[34] = 100;
    kt[34] = 0.15;
    dr[34] = 20000;

    for (int i = 0; i < smps; ++i)
    {
        int vl = 0;
        for (int j = 0; j < bone; ++j)
        {
            if(i > st[j] && i < (st[j] + dr[j]))
            {
                //  混音         频率  样号  频率能量
                if(pw[j])
                {
                    vl += sin(fq[j] * i) * pw[j];
                }
                if(pwn[j])
                {
                    for (double k = 0; k < kt[j]; k += 0.01)
                    {
                        vl += sin((fq[j] + k) * i) * pwn[j];
                    }
                }
            }
        }
        QByteArray v = short2QByteArray(vl);
        bfo.push_back(v);
        bfo.push_back(v);
    }
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    QMatrix4x4 gWorldi = gWorld.inverted();

    switch (event->key())
    {
    case Qt::Key_W:
        gWorld.translate(0.01f * (gWorldi * mo - gWorldi * mz));
        break;
    case Qt::Key_S:
        gWorld.translate(0.01f * (gWorldi * mz - gWorldi * mo));
        break;
    case Qt::Key_A:
        gWorld.translate(0.01f * (gWorldi * mo - gWorldi * mx));
        break;
    case Qt::Key_D:
        gWorld.translate(0.01f * (gWorldi * mx - gWorldi * mo));
        break;
    case Qt::Key_R:
        gWorld.translate(0.01f * (gWorldi * mo - gWorldi * my));
        break;
    case Qt::Key_F:
        gWorld.translate(0.01f * (gWorldi * my - gWorldi * mo));
        break;
    }
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    QMatrix4x4 gWorldi = gWorld.inverted();
    if(event->angleDelta().y() > 0)
    {
        gWorld.translate(gWorldi * mo - gWorldi * mz);
    }
    else
    {
        gWorld.translate(gWorldi * mz - gWorldi * mo);
    }
}
