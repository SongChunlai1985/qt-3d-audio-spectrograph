#include "aud.h"

aud::aud()
{
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);   //设定声道数目，mono(平声道)的声道数目是1；stero(立体声)的声道数目是2
    format.setSampleSize(16);
    format.setCodec("audio/pcm");   //编码器
    format.setByteOrder(QAudioFormat::LittleEndian);   //设定高低位,LittleEndian（低位优先）/LargeEndian(高位优先)
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();

    if (!info.isFormatSupported(format))
    {
        format = info.nearestFormat(format);
    }

    audioInput = new QAudioInput(format, nullptr);

    streamIn = audioInput->start();

    QAudioDeviceInfo infoo = QAudioDeviceInfo::defaultOutputDevice();

    if (!infoo.isFormatSupported(format))
    {
        format = info.nearestFormat(format);
    }

    audioOutput = new QAudioOutput(format, nullptr);

    streamOut = audioOutput->start();

}
