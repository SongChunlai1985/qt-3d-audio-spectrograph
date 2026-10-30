#include "conv.h"

conv::conv()
{

}

gm44d gset(double m00, double m10, double m20, double m30, double m01, double m11, double m21, double m31, double m02,
           double m12, double m22, double m32, double m03, double m13, double m23, double m33)
{
    gm44d U;
    U.SetRow(0, gm4d{m00, m10, m20, m30});
    U.SetRow(1, gm4d{m01, m11, m21, m31});
    U.SetRow(2, gm4d{m02, m12, m22, m32});
    U.SetRow(3, gm4d{m03, m13, m23, m33});
    return U;
}

gm44d gset(p3d X, p3d Y, p3d Z, p3d M, double m03, double m13, double m23, double m33)
{
    return (gset(X.x, Y.x, Z.x, M.x, X.y, Y.y, Z.y, M.y, X.z, Y.z, Z.z, M.z, m03, m13, m23, m33));
}

gcc4d mkcvt(p3d a0, p3d b0, p3d c0, p3d m0, p3d a1, p3d b1, p3d c1, p3d m1,
            bool vectorOnRightU, bool vectorOnRightV)
{
    gm44d U, V;
    gcc4d convert;
    U = gset(a1, b1, c1, m1);
    V = gset(a0, b0, c0, m0);
    convert(U, vectorOnRightU, V, vectorOnRightV);
    return convert;
}
