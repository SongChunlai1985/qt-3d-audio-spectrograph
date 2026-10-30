#ifndef AUD_H
#define AUD_H

#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QAudioInput>

#include <QTimer>
#include <QMainWindow>
#include <QDebug>

#include <QAudioOutput>
#include <unistd.h>
class aud
{
public:
    aud();
    QAudioInput* audioInput;
    QIODevice* streamIn;

    QAudioOutput* audioOutput;
    QIODevice* streamOut;
};

#endif // AUD_H
