#include "openglwidget.h"
#include <QApplication>
#include <QImage>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(8192);
    fmt.setStencilBufferSize(8);
    fmt.setVersion(3, 3);
    fmt.setSamples(4);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);

    OpenGLWidget l;
    l.setWindowTitle("Opgl_1");
    l.showNormal();

    return a.exec();
}
