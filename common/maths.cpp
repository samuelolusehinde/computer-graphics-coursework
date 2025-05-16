#include "maths.hpp"

 Vec3 Vec3::cross(const Vec3& rhs) const {
    return {
        y * rhs.z - z * rhs.y,
        z * rhs.x - x * rhs.z,
        x * rhs.y - y * rhs.x
    };
}

 float Vec3::dot(const Vec3& rhs) const
{
    return x * rhs.x + y * rhs.y + z * rhs.z;
}

 Vec3 Vec3::normalize() const
{
    float length = std::sqrt(x * x + y * y + z * z);
    return { x / length, y / length, z / length };
}

 float toRadians(float degrees)
 {
     return degrees * (PI / 180.0f);
 }

 Mat4 Multiply(const Mat4& a, const Mat4& b) 
 {
     Mat4 result;

     for (int col = 0; col < 4; ++col) {
         for (int row = 0; row < 4; ++row) {
             result.cols[col].x = a.cols[0].x * b.cols[col].x + a.cols[1].x * b.cols[col].y + a.cols[2].x * b.cols[col].z + a.cols[3].x * b.cols[col].w;
             result.cols[col].y = a.cols[0].y * b.cols[col].x + a.cols[1].y * b.cols[col].y + a.cols[2].y * b.cols[col].z + a.cols[3].y * b.cols[col].w;
             result.cols[col].z = a.cols[0].z * b.cols[col].x + a.cols[1].z * b.cols[col].y + a.cols[2].z * b.cols[col].z + a.cols[3].z * b.cols[col].w;
             result.cols[col].w = a.cols[0].w * b.cols[col].x + a.cols[1].w * b.cols[col].y + a.cols[2].w * b.cols[col].z + a.cols[3].w * b.cols[col].w;
         }
     }

     return result;
 }

 Mat4 Identity() {
     return Mat4{
         Vec4{ 1.0f, 0.0f, 0.0f, 0.0f },
         Vec4{ 0.0f, 1.0f, 0.0f, 0.0f },
         Vec4{ 0.0f, 0.0f, 1.0f, 0.0f },
         Vec4{ 0.0f, 0.0f, 0.0f, 1.0f }
     };
 }

 Mat4 QuaternionToMatrix(const Quat& qRaw)
 {
     Quat q = qRaw.normalize();

     float xx = q.x * q.x;
     float yy = q.y * q.y;
     float zz = q.z * q.z;
     float xy = q.x * q.y;
     float xz = q.x * q.z;
     float yz = q.y * q.z;
     float wx = q.w * q.x;
     float wy = q.w * q.y;
     float wz = q.w * q.z;

     Mat4 result;
     result.cols[0] = Vec4(1.0f - 2.0f * (yy + zz), 2.0f * (xy + wz), 2.0f * (xz - wy), 0.0f);
     result.cols[1] = Vec4(2.0f * (xy - wz), 1.0f - 2.0f * (xx + zz), 2.0f * (yz + wx), 0.0f);
     result.cols[2] = Vec4(2.0f * (xz + wy), 2.0f * (yz - wx), 1.0f - 2.0f * (xx + yy), 0.0f);
     result.cols[3] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
     return result;
 }

 Quat Conjugate(const Quat& q) 
 {
     return Quat{
         q.w,
         -q.x,
         -q.y,
         -q.z
     };
 }

 Mat4 Translate(const Mat4& m, const Vec4& v) 
 {
     Mat4 result = m;

     result.cols[3].x += m.cols[0].x * v.x + m.cols[1].x * v.y + m.cols[2].x * v.z;
     result.cols[3].y += m.cols[0].y * v.x + m.cols[1].y * v.y + m.cols[2].y * v.z;
     result.cols[3].z += m.cols[0].z * v.x + m.cols[1].z * v.y + m.cols[2].z * v.z;
     result.cols[3].w += m.cols[0].w * v.x + m.cols[1].w * v.y + m.cols[2].w * v.z;

     return result;
 }

 Mat4 PerspectiveFov(float fovYDeg, float aspect, float zNear, float zFar)
 {
     float fovYRad = toRadians(fovYDeg);
     float tanHalfFovy = std::tan(fovYRad / 2.0f);


     Mat4 result;
     result.cols[0] = Vec4(1.0f / (aspect * tanHalfFovy), 0.0f, 0.0f, 0.0f);
     result.cols[1] = Vec4(0.0f, 1.0f / tanHalfFovy, 0.0f, 0.0f);
     result.cols[2] = Vec4(0.0f, 0.0f, -(zFar + zNear) / (zFar - zNear), -1.0f);
     result.cols[3] = Vec4(0.0f, 0.0f, -(2.0f * zFar * zNear) / (zFar - zNear), 0.0f);
     return result;
 }

 Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
     Vec3 f = (center - eye).normalize();
     Vec3 s = f.cross(up).normalize();
     Vec3 u = s.cross(f);

     Mat4 result;
     result.cols[0] = Vec4(s.x, u.x, -f.x, 0.0f);
     result.cols[1] = Vec4(s.y, u.y, -f.y, 0.0f);
     result.cols[2] = Vec4(s.z, u.z, -f.z, 0.0f);
     result.cols[3] = Vec4(-s.dot(eye), -u.dot(eye), f.dot(eye), 1.0f);
     return result;
 }

 
