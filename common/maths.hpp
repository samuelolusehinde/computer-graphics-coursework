#ifndef MATHS_HPP
#define MATHS_HPP

#include <cmath>

constexpr float PI = 3.14159265358979323846f;

float toRadians(float degrees);

struct Vec3 
{
    float x, y, z;

    Vec3() = default;

    Vec3(float x, float y, float z)
        :x(x), y(y), z(z)
    {

    }

    Vec3(float All)
    {
        x = All, y = All, z = All;
    }

    Vec3 operator-(const Vec3& rhs) const 
    {
        return { x - rhs.x, y - rhs.y, z - rhs.z };
    }

    Vec3 operator+(const Vec3& rhs) const
    {
        return { x + rhs.x, y + rhs.y, z + rhs.z };
    }

    Vec3 operator*(const float& rhs) const
    {
        return { x * rhs, y * rhs, z * rhs };
    }


    Vec3 cross(const Vec3& rhs) const;

    float dot(const Vec3& rhs) const;

    Vec3 normalize() const;
};


struct Vec4 
{
    float x, y, z, w;

    Vec4() : x(0), y(0), z(0), w(0) {}
    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};


struct Mat4
{
    Vec4 cols[4]; 

    const float* data() const { return &cols[0].x; }
    float* data() { return &cols[0].x; }
};

struct Quat
{
    float x, y, z, w;

    Quat() : x(0), y(0), z(0), w(1) {}
    Quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    Quat normalize() const 
    {
        float len = std::sqrt(x * x + y * y + z * z + w * w);
        return { x / len, y / len, z / len, w / len };
    }
};


Mat4 Multiply(const Mat4& a, const Mat4& b);


Mat4 Identity();

 Mat4 QuaternionToMatrix(const Quat& qRaw);

 Quat Conjugate(const Quat& q);


 Mat4 Translate(const Mat4& m, const Vec4& v);

 Mat4 PerspectiveFov(float fovYDeg, float aspect, float zNear, float zFar);
 Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up);

#endif // MATHS_HPP
