#include "dt.h"

dt::dt()
{

}

void dt::write()
{
    QFile f(path);
    f.open(QIODevice::WriteOnly);
    QJsonDocument doc;
    doc.setObject(d);
    f.write(doc.toJson(QJsonDocument::Indented));
    f.close();
}

void dt::read()
{
    QFile f(path);
    f.open(QIODevice::ReadOnly);
    QJsonDocument doc(QJsonDocument::fromJson(f.readAll()));
    d = doc.object();
    f.close();
}
