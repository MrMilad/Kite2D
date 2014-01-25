#include "Kite/graphic/ktransform.h"
#include <algorithm>
#include <cmath>

#ifdef KITE_USE_SSE
    #define __SSE__
    #include <xmmintrin.h>
#endif

namespace Kite{
    const KTransform KTransform::UnitMatrix;
    KTransform::KTransform(){
        // unit matrix (default)
        _kmatrix[0] = 1.f; _kmatrix[4] = 0.f; _kmatrix[8]  = 0.f; _kmatrix[12] = 0.f;
        _kmatrix[1] = 0.f; _kmatrix[5] = 1.f; _kmatrix[9]  = 0.f; _kmatrix[13] = 0.f;
        _kmatrix[2] = 0.f; _kmatrix[6] = 0.f; _kmatrix[10] = 1.f; _kmatrix[14] = 0.f;
        _kmatrix[3] = 0.f; _kmatrix[7] = 0.f; _kmatrix[11] = 0.f; _kmatrix[15] = 1.f;
    }

    KTransform::KTransform(F32 *MatrixArray){
        std::copy(MatrixArray, MatrixArray+16, _kmatrix);
    }

    KTransform::KTransform(F32 A00, F32 A01, F32 A02,
                         F32 A10, F32 A11, F32 A12,
                         F32 A20, F32 A21, F32 A22)
    {
        _kmatrix[0] = A00; _kmatrix[4] = A01; _kmatrix[8]  = 0.f; _kmatrix[12] = A02;
        _kmatrix[1] = A10; _kmatrix[5] = A11; _kmatrix[9]  = 0.f; _kmatrix[13] = A12;
        _kmatrix[2] = 0.f; _kmatrix[6] = 0.f; _kmatrix[10] = 1.f; _kmatrix[14] = 0.f;
        _kmatrix[3] = A20; _kmatrix[7] = A21; _kmatrix[11] = 0.f; _kmatrix[15] = A22;
    }

    KTransform KTransform::getGeneralInverse() const {
        // Compute the determinant
        F32 det = _kmatrix[0] * (_kmatrix[15] * _kmatrix[5] - _kmatrix[7] * _kmatrix[13]) -
                    _kmatrix[1] * (_kmatrix[15] * _kmatrix[4] - _kmatrix[7] * _kmatrix[12]) +
                    _kmatrix[3] * (_kmatrix[13] * _kmatrix[4] - _kmatrix[5] * _kmatrix[12]);

        // Compute the inverse if the determinant is not zero
        // (don't use an epsilon because the determinant may *really* be tiny)
        if (det != 0.f)
        {
            return KTransform( (_kmatrix[15] * _kmatrix[5] - _kmatrix[7] * _kmatrix[13]) / det,
                             -(_kmatrix[15] * _kmatrix[4] - _kmatrix[7] * _kmatrix[12]) / det,
                              (_kmatrix[13] * _kmatrix[4] - _kmatrix[5] * _kmatrix[12]) / det,
                             -(_kmatrix[15] * _kmatrix[1] - _kmatrix[3] * _kmatrix[13]) / det,
                              (_kmatrix[15] * _kmatrix[0] - _kmatrix[3] * _kmatrix[12]) / det,
                             -(_kmatrix[13] * _kmatrix[0] - _kmatrix[1] * _kmatrix[12]) / det,
                              (_kmatrix[7]  * _kmatrix[1] - _kmatrix[3] * _kmatrix[5])  / det,
                             -(_kmatrix[7]  * _kmatrix[0] - _kmatrix[3] * _kmatrix[4])  / det,
                              (_kmatrix[5]  * _kmatrix[0] - _kmatrix[1] * _kmatrix[4])  / det);
        }
        else
        {
            return UnitMatrix;
        }
    }

//    KTransform &KTransform::getEuclideanInverse(){
//        // transpose 3x3 rotation matrix part
//        F32 tmp;
//        tmp = _kmatrix[1];  _kmatrix[1] = _kmatrix[4];  _kmatrix[4] = tmp;
//        tmp = _kmatrix[2];  _kmatrix[2] = _kmatrix[8];  _kmatrix[8] = tmp;
//        tmp = _kmatrix[6];  _kmatrix[6] = _kmatrix[9];  _kmatrix[9] = tmp;

//        // compute translation part -R^T * T
//        F32 x = _kmatrix[3];
//        F32 y = _kmatrix[7];
//        F32 z = _kmatrix[11];
//        _kmatrix[3]  = -(_kmatrix[0] * x + _kmatrix[1] * y + _kmatrix[2] * z);
//        _kmatrix[7]  = -(_kmatrix[4] * x + _kmatrix[5] * y + _kmatrix[6] * z);
//        _kmatrix[11] = -(_kmatrix[8] * x + _kmatrix[9] * y + _kmatrix[10]* z);
//        // last row should be unchanged (0,0,0,1)

//        return *this;
//    }

    KRectF32 KTransform::transformRect(const KRectF32 &Rectangle) const{
        // Transform the 4 corners of the rectangle
        const KVector2F32 points[] =
        {
            transformPoint(KVector2F32(Rectangle.x, Rectangle.y)),
            transformPoint(KVector2F32(Rectangle.x, Rectangle.y + Rectangle.height)),
            transformPoint(KVector2F32(Rectangle.x + Rectangle.width, Rectangle.y)),
            transformPoint(KVector2F32(Rectangle.x + Rectangle.width, Rectangle.y + Rectangle.height))
        };

        // Compute the bounding rectangle of the transformed points
        F32 left = points[0].x;
        F32 top = points[0].y;
        F32 right = points[0].x;
        F32 bottom = points[0].y;
        for (I32 i = 1; i < 4; ++i)
        {
            if      (points[i].x < left)   left = points[i].x;
            else if (points[i].x > right)  right = points[i].x;
            if      (points[i].y < top)    top = points[i].y;
            else if (points[i].y > bottom) bottom = points[i].y;
        }

        return KRectF32(left, top, right - left, bottom - top);
    }

    KTransform &KTransform::combine(const KTransform &Transform){
        #ifdef KITE_USE_SSE

        #else
            const F32* a = _kmatrix;
            const F32* b = Transform._kmatrix;

            *this = KTransform(a[0] * b[0]  + a[4] * b[1]  + a[12] * b[3],
                              a[0] * b[4]  + a[4] * b[5]  + a[12] * b[7],
                              a[0] * b[12] + a[4] * b[13] + a[12] * b[15],
                              a[1] * b[0]  + a[5] * b[1]  + a[13] * b[3],
                              a[1] * b[4]  + a[5] * b[5]  + a[13] * b[7],
                              a[1] * b[12] + a[5] * b[13] + a[13] * b[15],
                              a[3] * b[0]  + a[7] * b[1]  + a[15] * b[3],
                              a[3] * b[4]  + a[7] * b[5]  + a[15] * b[7],
                              a[3] * b[12] + a[7] * b[13] + a[15] * b[15]);

            return *this;
        #endif
    }

    KTransform& KTransform::translate(const KVector2F32 &Translate){
        KTransform translation(1, 0, Translate.x,
                              0, 1, Translate.y,
                              0, 0, 1);

        return combine(translation);

//        _kmatrix[0] += _kmatrix[12] * Translate.x;   _kmatrix[1] += _kmatrix[13] * Translate.x;
//        _kmatrix[2] += _kmatrix[14] * Translate.x;   _kmatrix[3] += _kmatrix[15] * Translate.x;
//        _kmatrix[4] += _kmatrix[12] * Translate.y;   _kmatrix[5] += _kmatrix[13] * Translate.y;
//        _kmatrix[6] += _kmatrix[14] * Translate.y;   _kmatrix[7] += _kmatrix[15] * Translate.y;
//        _kmatrix[8] += _kmatrix[12] * Translate.z;   _kmatrix[9] += _kmatrix[13] * Translate.z;
//        _kmatrix[10]+= _kmatrix[14] * Translate.z;;  _kmatrix[11]+= _kmatrix[15] * Translate.z;
//        return *this;
    }

    KTransform& KTransform::rotate(F32 Angle){
        #ifdef KITE_USE_SSE
//         // sse sin,cos,...
        #else
            float c = cosf(Angle * KMATH_PIsub180);
            float s = sinf(Angle * KMATH_PIsub180);
            float m0 = _kmatrix[0], m1 = _kmatrix[1], m2 = _kmatrix[2],  m3 = _kmatrix[3],
                  m4 = _kmatrix[4], m5 = _kmatrix[5], m6 = _kmatrix[6],  m7 = _kmatrix[7];

            _kmatrix[0] = m0 * c + m4 *-s;
            _kmatrix[1] = m1 * c + m5 *-s;
            _kmatrix[2] = m2 * c + m6 *-s;
            _kmatrix[3] = m3 * c + m7 *-s;
            _kmatrix[4] = m0 * s + m4 * c;
            _kmatrix[5] = m1 * s + m5 * c;
            _kmatrix[6] = m2 * s + m6 * c;
            _kmatrix[7] = m3 * s + m7 * c;
        #endif
        return *this;

    }

    KTransform& KTransform::rotate(F32 Angle, const KVector2F32& Center){
        #ifdef KITE_USE_SSE
         // sse sin,cos,...
        #else
            F32 radx = Angle * KMATH_PIsub180;
            F32 cosx = std::cos(radx);
            F32 sinx = std::sin(radx);
        #endif

        KTransform rotation(cosx, -sinx, Center.x * (1 - cosx) + Center.y * sinx,
                           sinx,  cosx, Center.y * (1 - cosx) - Center.x * sinx,
                           0,    0,   1);

        return combine(rotation);
    }

    KTransform& KTransform::scale(const KVector2F32 &Scale){
        #ifdef KITE_USE_SSE
         // sse sin,cos,...
        #else
        _kmatrix[0] = _kmatrix[0] * Scale.x;
        _kmatrix[1] = _kmatrix[1] * Scale.x;
        _kmatrix[4] = _kmatrix[4] * Scale.y;
        _kmatrix[5] = _kmatrix[5] * Scale.y;
        #endif
        return *this;
    }

    KTransform& KTransform::scale(const KVector2F32 &Scale, const KVector2F32& Center){
        KTransform scaling(Scale.x, 0,      Center.x * (1 - Scale.x),
                          0,      Scale.y, Center.y * (1 - Scale.y),
                          0,      0,      1);

        return combine(scaling);
    }

    KTransform operator*(const KTransform &left,const KTransform &right){
        return KTransform(left).combine(right);
    }

    KTransform &operator*=(KTransform &left, const KTransform &right){
        return left.combine(right);
    }


//    KTransform operator*(const KTransform& left,KTransform right){
//        //#ifdef KITE_USE_SSE
//            __m128 u1 = _mm_shuffle_ps(v,v, _MM_SHUFFLE(0,0,0,0));
//            __m128 u2 = _mm_shuffle_ps(v,v, _MM_SHUFFLE(1,1,1,1));
//            __m128 u3 = _mm_shuffle_ps(v,v, _MM_SHUFFLE(2,2,2,2));
//            __m128 u4 = _mm_shuffle_ps(v,v, _MM_SHUFFLE(3,3,3,3));

//            __m128 prod1 = _mm_mul_ps(u1, cols[0]);
//            __m128 prod2 = _mm_mul_ps(u2, cols[1]);
//            __m128 prod3 = _mm_mul_ps(u3, cols[2]);
//            __m128 prod4 = _mm_mul_ps(u4, cols[3]);

//            return _mm_add_ps(_mm_add_ps(prod1, prod2), _mm_add_ps(prod3, prod4));
//        //#else

//    }
}
