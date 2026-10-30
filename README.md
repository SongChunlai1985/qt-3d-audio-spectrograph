# OpGl‑Audio‑3D‑Visualizer
> Qt + OpenGL3.3 CoreProfile + OpenCV + Dlib 实现的**音频3D可视化 + 人脸识别存储**桌面程序。
[![Qt](https://img.shields.io/badge/Qt‑5.15‑41CD52.svg)](https://www.qt.io)
[![OpenGL](https://img.shields.io/badge/OpenGL‑3.3‑blue.svg)](https://www.opengl.org)
[![OpenCV](https://img.shields.io/badge/OpenCV‑4.x‑red.svg)](https://opencv.org)
[![Dlib](https://img.shields.io/badge/Dlib‑FaceRecognition‑orange.svg)](http://dlib.net)

## 📖 项目简介
本项目是一个基于Qt OpenGL Widget的桌面演示程序，两大核心能力：
1. **音频采集‑录制‑播放‑3D频谱可视化**
    - 44100Hz 双声道16bit PCM音频采集
    - 汉明窗预处理，支持原始波形 / DFT频谱 / DCT变换三种模式渲染为3D曲面
    - 录音、回放、程序生成合成音频；鼠标可以在频谱曲面上选择片段播放
2. **Dlib人脸识别（可选编译）**
    - 摄像头人脸检测，提取128维人脸特征向量
    - 将人脸特征持久化保存到JSON文件，做简单人脸比对匹配
3. **3D交互**
    - WASD / RF 键盘移动相机；鼠标左键/中键/右键旋转平移；滚轮缩放；场景立方体UI按钮交互

> ⚠️ 人脸识别模块通过宏`usedlib`条件编译，不定义该宏可以不需要dlib依赖。

## ✨ 主要特性
- ✅ OpenGL 3.3 CoreProfile，VAO/VBO/EBO 现代着色器渲染
- ✅ QAudioInput/QAudioOutput 底层PCM音频流读写
- ✅ Hamming窗 + OpenCV DFT / DCT 音频时频转换
- ✅ 音频数据转3D高度场曲面，颜色根据幅值渐变
- ✅ JSON持久化类`dt`，读写人脸特征配置文件
- ✅ QtConcurrent多线程：人脸循环检测、音频播放后台线程
- ✅ 场景内交互式小方块按钮，鼠标悬停高亮点击触发功能
- ✅ 支持手动生成合成音频波形数据
- ✅ 相机自由漫游，透视投影

## 📦 依赖库
- **Qt5** (推荐5.15)：`QtCore` `QtGui` `QtWidgets` `QtOpenGL` `QtMultimedia` `QtConcurrent`
- **OpenCV 4.x**：core, imgproc, videoio
- **Dlib(可选)**：开启`usedlib`宏才需要，人脸识别
- C++11及以上编译器

> dlib模型文件（需要自行下载放到`D:\pro\`）
> 1. `shape_predictor_5_face_landmarks.dat`
> 2. `dlib_face_recognition_resnet_model_v1.dat`

## 📁 代码文件结构
```
├── main.cpp                # 程序入口，配置OpenGL surface format，启动窗口
├── openglwidget.h/cpp      # 主OpenGL窗口，音频逻辑、渲染、交互全部在这里
├── dt.h/cpp                # JSON读写工具类，保存人脸特征到 d:/opgldata.json
├── aud.h/cpp               # Qt音频输入输出封装(QAudioInput/QAudioOutput)
├── dlibfr.h/cpp            # dlib人脸识别封装(条件编译 usedlib)
├── conv.h/cpp              # 坐标转换工具（GTE几何数学库）
```

### JSON存储说明
- 默认路径：`D:/opgldata.json`
- 存储内容：人脸128维特征数组，人名映射表。

## 🚀 编译运行
1. 配置好Qt+OpenCV开发环境；如需人脸识别，编译部署dlib
2. 如需开启人脸识别：pro项目文件添加 `DEFINES += usedlib`
3. 如果开启usedlib，把dlib两个模型文件放到`D:\pro\`目录
4. qmake / cmake构建项目
5. 运行程序
> 程序运行会直接调用系统默认麦克风，请允许麦克风权限。输出JSON文件在D盘根目录。

## 🎮 操作说明
### 键盘
- `W S A D`：前后左右移动相机
- `R F`：上下移动相机

### 鼠标
- **左键拖动**：平移场景
- **中键拖动**：Z轴平移 + 绕Z旋转
- **右键拖动**：俯仰+偏航旋转
- **滚轮**：相机远近缩放
- 鼠标悬浮场景中小方块会放大高亮，**左键点击方块触发功能**：
    - 0：开始录音
    - 1：停止录音
    - 2：播放录音
    - 3：生成合成声音并播放
    - 4：清空录音缓存
    - 5：放大频谱颜色量程
    - 6：缩小频谱颜色量程
    - 7：模式0 原始波形
    - 8：模式1 DFT频谱
    - 9：模式2 DCT变换
    - 10：开启实时麦克风音频可视化
    - 11：关闭实时麦克风音频可视化

> 在3D频谱曲面上，**左键点击位置 = 设置播放起始点；右键点击 = 设置播放结束点**，自动播放选中音频片段。

## ⚠️ 已知注意事项 & 坑点
1. Windows下JSON输出硬编码路径`D:/opgldata.json`，注意D盘可写权限；可以修改`dt.h`中`path`成员修改保存路径。
2. dlib模型路径硬编码为`D:\pro\`，开启人脸识别务必放置模型，否则启动直接异常。
3. `QtConcurrent::run`后台线程，析构等待线程结束，不要直接强制关闭窗口。
4. OpenGL上下文：多线程不能随意调用OpenGL接口；所有VAO/VBO修改必须在`makeCurrent()`之后。
5. 音频依赖QtMultimedia模块，pro需要加入 `QT += multimedia concurrent opengl widgets core gui`
6. 仅支持OpenGL3.3 CoreProfile，老旧显卡会启动失败。

## 📝 Pro项目片段参考（.pro）
```pro
QT += core gui opengl widgets multimedia concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# 开启人脸识别dlib模块
# DEFINES += usedlib

INCLUDEPATH += $$PWD
SOURCES += main.cpp\
    openglwidget.cpp \
    dt.cpp \
    aud.cpp \
    dlibfr.cpp \
    conv.cpp

HEADERS  += openglwidget.h \
    dt.h \
    aud.h \
    dlibfr.h \
    conv.h

# OpenCV lib path 根据自己环境修改
win32{
INCLUDEPATH += D:/opencv/include
LIBS += -LD:/opencv/lib -lopencv_world4xx
}
```

## 📃 License
MIT License
> 仅供学习研究使用，dlib模型文件请遵守dlib官方许可协议。

---
如果你需要，我可以帮你再生成：
1. 中文简体精简版README
2. 添加截图占位、gitignore文件、cmake配置模板。
