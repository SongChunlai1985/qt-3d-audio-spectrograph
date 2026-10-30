#ifndef DT_H
#define DT_H
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

class dt
{
public:
    dt();
    QJsonObject d;
    QString path = "d:/opgldata.json";
    void write();
    void read();
};

#endif // DT_H
