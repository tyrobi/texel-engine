#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

#include <iostream>
#include <math.h>

class Vec2
{
private:
    float xVal, yVal;
public:
    Vec2();
    Vec2(float x, float y);

    Vec2& add (const Vec2& vec);
    Vec2& subtract (const Vec2& vec);
    Vec2& multiply (const Vec2& vec);
    Vec2& divide (const Vec2& vec);

    friend Vec2& operator+ (Vec2& v1, const Vec2& v2);
    friend Vec2& operator* (Vec2& v1, const Vec2& v2);
    friend Vec2& operator- (Vec2& v1, const Vec2& v2);
    friend Vec2& operator/ (Vec2& v1, const Vec2& v2);

    Vec2 operator* (const float& scalar) const;
    Vec2 operator/ (const float& scalar) const;

    Vec2& operator+= (const Vec2& vec);
    Vec2& operator-= (const Vec2& vec);
    Vec2& operator*= (const Vec2& vec);
    Vec2& operator/= (const Vec2& vec);

    Vec2& operator*= (const float& scalar);
    Vec2& operator/= (const float& scalar);

    friend std::ostream& operator<< (std::ostream& out, const Vec2& vec);

    float x() const { return xVal; }
    float y() const { return yVal; }
    void x(float val) { xVal = val; }
    void y(float val) { yVal = val; }
    float magnitude() const { return sqrt((xVal * xVal) + (yVal * yVal)); }
    void normalize()
    {
        if (this->magnitude() >= 0.01f)
            *this /= this->magnitude();
    }

    void zero() {xVal = yVal = 0.0f; }
};

// Impl

Vec2::Vec2()
{
    xVal = yVal = 0.0f;
};

Vec2::Vec2(float x, float y)
{
    xVal = x;
    yVal = y;
};

Vec2& Vec2::add(const Vec2& vec)
{
    this-> xVal += vec.x();
    this-> yVal += vec.y();
    return *this;
};
Vec2& Vec2::subtract(const Vec2& vec)
{
    this-> xVal -= vec.x();
    this-> yVal -= vec.y();
    return *this;
};
Vec2& Vec2::multiply(const Vec2& vec)
{
    this-> xVal *= vec.x();
    this-> yVal *= vec.y();
    return *this;
};
Vec2& Vec2::divide(const Vec2& vec)
{
    this-> xVal /= vec.x();
    this-> yVal /= vec.y();
    return *this;
};

Vec2& operator+(Vec2& v1, const Vec2& v2)
{
    return v1.add(v2);
};
Vec2& operator-(Vec2& v1, const Vec2& v2)
{
    return v1.subtract(v2);
};
Vec2& operator*(Vec2& v1, const Vec2& v2)
{
    return v1.multiply(v2);
};
Vec2 Vec2::operator*(const float& scalar) const
{
    return Vec2(this->x() * scalar, this->y() * scalar);
};
Vec2 Vec2::operator/(const float& scalar) const
{
    if (scalar == 0.0f)
    {
        std::cerr << "Vec2 : division by 0 scalar..." << std::endl;
        return Vec2();
    }
    return Vec2(this->x() / scalar, this->y() / scalar);
};
Vec2& operator/(Vec2& v1, const Vec2& v2)
{
    return v1.divide(v2);
};

Vec2& Vec2::operator+= (const Vec2& vec)
{
    return this-> add(vec);
};
Vec2& Vec2::operator-= (const Vec2& vec)
{
    return this-> subtract(vec);
};
Vec2& Vec2::operator*= (const Vec2& vec)
{
    return this-> multiply(vec);
};
Vec2& Vec2::operator/= (const Vec2& vec)
{
    return this-> divide(vec);
};
Vec2& Vec2::operator*= (const float& scalar)
{
    this-> xVal *= scalar;
    this-> yVal *= scalar;
    return *this;
};
Vec2& Vec2::operator/= (const float& scalar)
{
    this-> xVal /= scalar;
    this-> yVal /= scalar;
    return *this;
};
std::ostream& operator<< (std::ostream& out, const Vec2& vec)
{
    out << "Vec2 : (" << vec.xVal << ", " << vec.yVal << ") ";
    return out;
};


#endif