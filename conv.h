#ifndef CONV_H
#define CONV_H
#include <Mathematics/GteConvertCoordinates.h>
typedef gte::Matrix<4, 4, double> gm44d;
typedef gte::Vector<4, double> gm4d;
typedef gte::ConvertCoordinates<4, double> gcc4d;

struct p3d
{
    double x, y, z;
    p3d(){}
    p3d(double X, double Y, double Z)
    { x = X; y = Y; z = Z;}
    p3d operator + (p3d b)
    {
        return p3d(x + b.x, y + b.y, z + b.z);
    }
    p3d operator - (p3d b)
    {
        return p3d(x - b.x, y - b.y, z - b.z);
    }

};

gm44d gset(double m00, double m10, double m20, double m30, double m01, double m11, double m21, double m31, double m02,
           double m12, double m22, double m32, double m03, double m13, double m23, double m33);

gm44d gset(p3d X, p3d Y, p3d Z, p3d M, double m03 = 0, double m13 = 0, double m23 = 0, double m33 = 1);
gcc4d mkcvt(p3d a0, p3d b0, p3d c0, p3d m0, p3d a1, p3d b1, p3d c1, p3d m1,
            bool vectorOnRightU = false, bool vectorOnRightV = false);

struct fdot
{
    p3d o, x, y, z;

    fdot() { x = y = z = o = p3d(0, 0, 0); }

    fdot(p3d x_, p3d y_, p3d z_, p3d o_)
    {
        x = x_;
        y = y_;
        z = z_;
        o = o_;
    }

    gm44d operator<<(fdot src)
    {
        gcc4d cvt = mkcvt(src.x - src.o, src.y - src.o, src.z - src.o, src.o - src.o, x - o, y - o,
                          z - o, o - o);
        gm44d mo = gset(p3d(1, 0, 0), p3d(0, 1, 0), p3d(0, 0, 1), src.o - o);
        return cvt.GetC() * mo;
    }

    gm44d operator>>(fdot src)
    {
        gcc4d cvt = mkcvt(src.x - src.o, src.y - src.o, src.z - src.o, src.o - src.o, x - o, y - o,
                          z - o, o - o);
        gm44d mo = gset(p3d(1, 0, 0), p3d(0, 1, 0), p3d(0, 0, 1), src.o - o);
        return mo * cvt.GetC();
    }

};

class conv
{
public:
    conv();
};

#endif // CONV_H
