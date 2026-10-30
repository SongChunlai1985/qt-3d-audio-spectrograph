
#include "dlibfr.h"
dlibfr::dlibfr()
{
    detector = dlib::get_frontal_face_detector();
    dlib::deserialize("D:\\pro\\shape_predictor_5_face_landmarks.dat") >> sp;
    dlib::deserialize("D:\\pro\\dlib_face_recognition_resnet_model_v1.dat") >> net;
}

std::vector<dlib::rectangle> dlibfr::rtgs(dlib::cv_image<dlib::rgb_pixel> img2,
                                          std::vector<dlib::matrix<float,0,1>> &face_descriptors)
{
    assign_image(img, img2);

    std::vector<dlib::matrix<dlib::rgb_pixel>> faces;
    std::vector<dlib::rectangle> Rtgs;
    for (auto face : detector(img))
    {
        auto shape = sp(img, face);
        dlib::matrix<dlib::rgb_pixel> face_chip;
        extract_image_chip(img, get_face_chip_details(shape,150,0.25), face_chip);
        faces.push_back(std::move(face_chip));
        Rtgs.push_back(face);
    }

    if (faces.size() > 0)
    {
        face_descriptors = net(faces);
    }

    return Rtgs;
}

QJsonArray face_descriptors2QJsonArray(std::vector<dlib::matrix<float,0,1>> face_descriptors)
{
    QJsonArray fda;
    uint sz = face_descriptors.size();
    for (uint s = 0; s < sz; ++s)
    {
        QJsonArray ja;
        for (int i = 0; i < 128; i++)
        {
            ja.push_back(double(face_descriptors[s](0, i)));
        }
        fda.push_back(ja);
    }
    return fda;
}

std::vector<dlib::matrix<float,0,1>> QJsonArray2face_descriptors(QJsonArray fda)
{
    std::vector<dlib::matrix<float,0,1>> face_descriptors;
    foreach (QJsonValue fd, fda)
    {
        dlib::matrix<float,0,1> face_descriptor(128);
        for (int i = 0; i < 128; i++)
        {
            face_descriptor(0, i) = float(fd[i].toDouble());
        }
        face_descriptors.push_back(face_descriptor);
    }
    return face_descriptors;
}
