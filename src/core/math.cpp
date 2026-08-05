#include "zingvid/math.h"
#include <cmath>

namespace zv {

Mat4 Mat4::identity() {
    Mat4 r;
    r.m[0] = r.m[5] = r.m[10] = r.m[15] = 1.0f;
    return r;
}

Mat4 Mat4::perspective(float fov, float aspect, float near, float far) {
    Mat4 r;
    float tanHalfFov = std::tan(fov * 3.14159265f / 360.0f);
    r.m[0] = 1.0f / (aspect * tanHalfFov);
    r.m[5] = 1.0f / tanHalfFov;
    r.m[10] = -(far + near) / (far - near);
    r.m[11] = -1.0f;
    r.m[14] = -(2.0f * far * near) / (far - near);
    return r;
}

Mat4 Mat4::lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
    Vec3 f = {center.x - eye.x, center.y - eye.y, center.z - eye.z};
    float flen = std::sqrt(f.x*f.x + f.y*f.y + f.z*f.z);
    f.x/=flen; f.y/=flen; f.z/=flen;

    Vec3 s = {f.y*up.z - f.z*up.y, f.z*up.x - f.x*up.z, f.x*up.y - f.y*up.x};
    float slen = std::sqrt(s.x*s.x + s.y*s.y + s.z*s.z);
    s.x/=slen; s.y/=slen; s.z/=slen;

    Vec3 u = {s.y*f.z - s.z*f.y, s.z*f.x - s.x*f.z, s.x*f.y - s.y*f.x};

    Mat4 r;
    r.m[0]=s.x; r.m[4]=s.y; r.m[8]=s.z; r.m[12]=-(s.x*eye.x+s.y*eye.y+s.z*eye.z);
    r.m[1]=u.x; r.m[5]=u.y; r.m[9]=u.z; r.m[13]=-(u.x*eye.x+u.y*eye.y+u.z*eye.z);
    r.m[2]=-f.x; r.m[6]=-f.y; r.m[10]=-f.z; r.m[14]=(f.x*eye.x+f.y*eye.y+f.z*eye.z);
    r.m[15]=1.0f;
    return r;
}

Mat4 Mat4::translate(const Vec3& t) {
    Mat4 r = identity();
    r.m[12] = t.x; r.m[13] = t.y; r.m[14] = t.z;
    return r;
}

Mat4 Mat4::rotate(float angle, const Vec3& axis) {
    Mat4 r;
    float c = std::cos(angle), s = std::sin(angle);
    float x = axis.x, y = axis.y, z = axis.z;
    float len = std::sqrt(x*x + y*y + z*z);
    x/=len; y/=len; z/=len;
    r.m[0]=c+x*x*(1-c); r.m[4]=x*y*(1-c)-z*s; r.m[8]=x*z*(1-c)+y*s;
    r.m[1]=y*x*(1-c)+z*s; r.m[5]=c+y*y*(1-c); r.m[9]=y*z*(1-c)-x*s;
    r.m[2]=z*x*(1-c)-y*s; r.m[6]=z*y*(1-c)+x*s; r.m[10]=c+z*z*(1-c);
    r.m[15]=1.0f;
    return r;
}

Mat4 Mat4::scale(const Vec3& s) {
    Mat4 r;
    r.m[0]=s.x; r.m[5]=s.y; r.m[10]=s.z; r.m[15]=1.0f;
    return r;
}

Mat4 Mat4::operator*(const Mat4& o) const {
    Mat4 r;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            r.m[i*4+j] = m[i*4+0]*o.m[0*4+j] + m[i*4+1]*o.m[1*4+j] + 
                         m[i*4+2]*o.m[2*4+j] + m[i*4+3]*o.m[3*4+j];
    return r;
}

Vec3 Mat4::transform(const Vec3& v) const {
    float x = m[0]*v.x + m[4]*v.y + m[8]*v.z + m[12];
    float y = m[1]*v.x + m[5]*v.y + m[9]*v.z + m[13];
    float z = m[2]*v.x + m[6]*v.y + m[10]*v.z + m[14];
    float w = m[3]*v.x + m[7]*v.y + m[11]*v.z + m[15];
    return {x/w, y/w, z/w};
}

bool AABB::contains(const Vec3& p) const {
    return p.x >= min.x && p.x <= max.x &&
           p.y >= min.y && p.y <= max.y &&
           p.z >= min.z && p.z <= max.z;
}

AABB AABB::merge(const AABB& o) const {
    return {{std::min(min.x,o.min.x), std::min(min.y,o.min.y), std::min(min.z,o.min.z)},
            {std::max(max.x,o.max.x), std::max(max.y,o.max.y), std::max(max.z,o.max.z)}};
}

} // namespace zv
