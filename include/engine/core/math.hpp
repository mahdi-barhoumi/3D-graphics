#pragma once
#include <cmath>
#include <cassert>
#include <numbers>
#include <algorithm>
#include <immintrin.h>

namespace Engine::Math
{
    // Constants

    inline constexpr float E = std::numbers::e;
    inline constexpr float Pi = std::numbers::pi;

    // Standard math functions

    inline constexpr float Square(float scalar) noexcept { return scalar * scalar; }
    inline constexpr float SquareRoot(float scalar) noexcept { return std::sqrt(scalar); }
    inline constexpr float InverseSquareRoot(float scalar) noexcept { return 1.0f / std::sqrt(scalar); }
    inline constexpr float Radians(float degrees) noexcept { return degrees * std::numbers::pi / 180.0f; }
    inline constexpr float Degrees(float radians) noexcept { return radians * 180.0f / std::numbers::pi; }
    inline constexpr float Sin(float radians) noexcept { return std::sin(radians); }
    inline constexpr float Cos(float radians) noexcept { return std::cos(radians); }
    inline constexpr float Tan(float radians) noexcept { return std::tan(radians); }
    inline constexpr float Arcsin(float scalar) noexcept { return std::asin(scalar); }
    inline constexpr float Arccos(float scalar) noexcept { return std::acos(scalar); }
    inline constexpr float Arctan(float scalar) noexcept { return std::atan(scalar); }
    inline constexpr float Min(float x, float y) noexcept { return std::min(x, y); }
    inline constexpr float Max(float x, float y) noexcept { return std::max(x, y); }
    inline constexpr float Abs(float scalar) noexcept { return std::abs(scalar); }
    inline constexpr float Exp(float scalar) noexcept { return std::exp(scalar); }
    inline constexpr float Pow(float scalar, float exponent) noexcept { return std::pow(scalar, exponent); }
    inline constexpr float Sign(float scalar) noexcept { return 1.0f - std::signbit(scalar) * 2.0f; }
    inline constexpr float InverseSign(float scalar) noexcept { return 1.0f - !std::signbit(scalar) * 2.0f; }
    inline constexpr float Clamp(float scalar, float min, float max) noexcept { return std::clamp(scalar, min, max); }
    inline constexpr float Floor(float scalar) noexcept { return std::floor(scalar); }
    inline constexpr float Ceiling(float scalar) noexcept { return std::ceil(scalar); }
    inline constexpr float Round(float scalar) noexcept { return std::round(scalar); }

    // Forward declarations

    struct alignas(8)  Vector2;
    struct alignas(16) Vector3;
    struct alignas(16) Vector4;
    struct alignas(16) Matrix2;
    struct alignas(64) Matrix3;
    struct alignas(64) Matrix4;
    struct alignas(16) Quaternion;

    // Declarations

    // Vector2

    struct alignas(8) Vector2
    {
        union
        {
            float data[2];
            struct { float x, y; };
        };
        
        inline constexpr Vector2() noexcept = default;
        inline constexpr Vector2(const Vector2& other) noexcept = default;
        inline constexpr Vector2(Vector2&& other) noexcept = default;
        inline constexpr Vector2& operator=(const Vector2& other) noexcept = default;
        inline constexpr Vector2& operator=(Vector2&& other) noexcept = default;
        inline constexpr ~Vector2() noexcept = default;

        inline constexpr explicit Vector2(float scalar) noexcept;
        inline constexpr explicit Vector2(float x, float y) noexcept;

        inline constexpr float operator[](size_t index) const noexcept;
        inline constexpr float& operator[](size_t index) noexcept;
        inline constexpr Vector2& operator+=(const Vector2& vector) noexcept;
        inline constexpr Vector2& operator-=(const Vector2& vector) noexcept;
        inline constexpr Vector2& operator*=(float scalar) noexcept;
        inline constexpr Vector2& operator*=(const Matrix2& matrix) noexcept;
        inline constexpr Vector2& operator/=(float scalar) noexcept;
    };

    inline constexpr Vector2 operator+(const Vector2& u, const Vector2& v) noexcept;
    inline constexpr Vector2 operator-(const Vector2& u, const Vector2& v) noexcept;
    inline constexpr Vector2 operator-(const Vector2& vector) noexcept;
    inline constexpr Vector2 operator*(const Vector2& vector, float scalar) noexcept;
    inline constexpr Vector2 operator*(float scalar, const Vector2& vector) noexcept;
    inline constexpr Vector2 operator*(const Matrix2& matrix, const Vector2& vector) noexcept;
    inline constexpr Vector2 operator*(const Vector2& vector, const Matrix2& matrix) noexcept;
    inline constexpr Vector2 operator/(const Vector2& vector, float scalar) noexcept;

    inline constexpr float Dot(const Vector2& u, const Vector2& v) noexcept;
    inline constexpr float Length(const Vector2& vector) noexcept;
    inline constexpr float LengthSquared(const Vector2& vector) noexcept;
    inline constexpr float Distance(const Vector2& a, const Vector2& b) noexcept;
    inline constexpr float DistanceSquared(const Vector2& a, const Vector2& b) noexcept;
    inline constexpr Vector2 Hadamard(const Vector2& u, const Vector2& v) noexcept;
    inline constexpr Vector2 Normalized(const Vector2& vector) noexcept;
    inline constexpr Vector2& Normalize(Vector2& vector) noexcept;

    // Vector3

    struct alignas(16) Vector3
    {
        union
        {
            __m128 internal;
            float data[3];
            struct { float x, y, z; };
        };
        
        inline constexpr Vector3() noexcept = default;
        inline constexpr Vector3(const Vector3& other) noexcept = default;
        inline constexpr Vector3(Vector3&& other) noexcept = default;
        inline constexpr Vector3& operator=(const Vector3& other) noexcept = default;
        inline constexpr Vector3& operator=(Vector3&& other) noexcept = default;
        inline constexpr ~Vector3() noexcept = default;

        inline constexpr explicit Vector3(__m128 internal) noexcept;
        inline constexpr explicit Vector3(float scalar) noexcept;
        inline constexpr explicit Vector3(float x, float y, float z) noexcept;
        inline constexpr explicit Vector3(const Vector2& xy, float z) noexcept;
        inline constexpr explicit Vector3(float x, const Vector2& yz) noexcept;

        inline constexpr operator Vector2() const noexcept;
        
        inline constexpr float operator[](size_t index) const noexcept;
        inline constexpr float& operator[](size_t index) noexcept;
        inline constexpr Vector3& operator+=(const Vector3& vector) noexcept;
        inline constexpr Vector3& operator-=(const Vector3& vector) noexcept;
        inline constexpr Vector3& operator*=(float scalar) noexcept;
        inline constexpr Vector3& operator*=(const Matrix3& matrix) noexcept;
        inline constexpr Vector3& operator/=(float scalar) noexcept;
    };

    inline constexpr Vector3 operator+(const Vector3& u, const Vector3& v) noexcept;
    inline constexpr Vector3 operator-(const Vector3& u, const Vector3& v) noexcept;
    inline constexpr Vector3 operator-(const Vector3& vector) noexcept;
    inline constexpr Vector3 operator*(const Vector3& vector, float scalar) noexcept;
    inline constexpr Vector3 operator*(float scalar, const Vector3& vector) noexcept;
    inline constexpr Vector3 operator*(const Matrix3& matrix, const Vector3& vector) noexcept;
    inline constexpr Vector3 operator*(const Vector3& vector, const Matrix3& matrix) noexcept;
    inline constexpr Vector3 operator/(const Vector3& vector, float scalar) noexcept;

    inline constexpr float Dot(const Vector3& u, const Vector3& v) noexcept;
    inline constexpr float Length(const Vector3& vector) noexcept;
    inline constexpr float LengthSquared(const Vector3& vector) noexcept;
    inline constexpr float Distance(const Vector3& a, const Vector3& b) noexcept;
    inline constexpr float DistanceSquared(const Vector3& a, const Vector3& b) noexcept;
    inline constexpr Vector3 Hadamard(const Vector3& u, const Vector3& v) noexcept;
    inline constexpr Vector3 Cross(const Vector3& u, const Vector3& v) noexcept;
    inline constexpr Vector3 Normalized(const Vector3& vector) noexcept;
    inline constexpr Vector3& Normalize(Vector3& vector) noexcept;
    inline constexpr Vector3 Rotated(const Vector3& vector, const Quaternion& quaternion) noexcept;
    inline constexpr Vector3& Rotate(Vector3& vector, const Quaternion& quaternion) noexcept;

    // Vector4

    struct alignas(16) Vector4
    {
        union
        {
            __m128 internal;
            float data[4];
            struct { float x, y, z, w; };
        };
        
        inline constexpr Vector4() noexcept = default;
        inline constexpr Vector4(const Vector4& other) noexcept = default;
        inline constexpr Vector4(Vector4&& other) noexcept = default;
        inline constexpr Vector4& operator=(const Vector4& other) noexcept = default;
        inline constexpr Vector4& operator=(Vector4&& other) noexcept = default;
        inline constexpr ~Vector4() noexcept = default;

        inline constexpr explicit Vector4(__m128 internal) noexcept;
        inline constexpr explicit Vector4(float scalar) noexcept;
        inline constexpr explicit Vector4(float x, float y, float z, float w) noexcept;
        inline constexpr explicit Vector4(const Vector2& xy, float z, float w) noexcept;
        inline constexpr explicit Vector4(float x, const Vector2& yz, float w) noexcept;
        inline constexpr explicit Vector4(float x, float y, const Vector2& zw) noexcept;
        inline constexpr explicit Vector4(const Vector2& xy, const Vector2& zw) noexcept;
        inline constexpr explicit Vector4(const Vector3& xyz, float w) noexcept;
        inline constexpr explicit Vector4(float x, const Vector3& yzw) noexcept;

        inline constexpr operator Vector2() const noexcept;
        inline constexpr operator Vector3() const noexcept;

        inline constexpr float operator[](size_t index) const noexcept;
        inline constexpr float& operator[](size_t index) noexcept;
        inline constexpr Vector4& operator+=(const Vector4& vector) noexcept;
        inline constexpr Vector4& operator-=(const Vector4& vector) noexcept;
        inline constexpr Vector4& operator*=(float scalar) noexcept;
        inline constexpr Vector4& operator*=(const Matrix4& matrix) noexcept;
        inline constexpr Vector4& operator/=(float scalar) noexcept;
    };

    inline constexpr Vector4 operator+(const Vector4& u, const Vector4& v) noexcept;
    inline constexpr Vector4 operator-(const Vector4& u, const Vector4& v) noexcept;
    inline constexpr Vector4 operator-(const Vector4& vector) noexcept;
    inline constexpr Vector4 operator*(const Vector4& vector, float scalar) noexcept;
    inline constexpr Vector4 operator*(float scalar, const Vector4& vector) noexcept;
    inline constexpr Vector4 operator*(const Matrix4& matrix, const Vector4& vector) noexcept;
    inline constexpr Vector4 operator*(const Vector4& vector, const Matrix4& matrix) noexcept;
    inline constexpr Vector4 operator/(const Vector4& vector, float scalar) noexcept;

    inline constexpr float Dot(const Vector4& u, const Vector4& v) noexcept;
    inline constexpr float Length(const Vector4& vector) noexcept;
    inline constexpr float LengthSquared(const Vector4& vector) noexcept;
    inline constexpr float Distance(const Vector4& a, const Vector4& b) noexcept;
    inline constexpr float DistanceSquared(const Vector4& a, const Vector4& b) noexcept;
    inline constexpr Vector4 Hadamard(const Vector4& u, const Vector4& v) noexcept;
    inline constexpr Vector4 Normalized(const Vector4& vector) noexcept;
    inline constexpr Vector4& Normalize(Vector4& vector) noexcept;

    // Matrix2

    struct alignas(16) Matrix2
    {
        union
        {
            float data[4];
            Vector2 columns[2];
        };

        inline constexpr Matrix2() noexcept = default;
        inline constexpr Matrix2(const Matrix2& other) noexcept = default;
        inline constexpr Matrix2(Matrix2&& other) noexcept = default;
        inline constexpr Matrix2& operator=(const Matrix2& other) noexcept = default;
        inline constexpr Matrix2& operator=(Matrix2&& other) noexcept = default;
        inline constexpr ~Matrix2() noexcept = default;

        inline constexpr explicit Matrix2(float scalar) noexcept;
        inline constexpr explicit Matrix2(float x1, float y1, float x2, float y2) noexcept;
        inline constexpr explicit Matrix2(const Vector2& diagonal) noexcept;
        inline constexpr explicit Matrix2(const Vector2& column1, const Vector2& column2) noexcept;

        inline constexpr const Vector2& operator[](size_t index) const noexcept;
        inline constexpr Vector2& operator[](size_t index) noexcept;
        inline constexpr Matrix2& operator+=(const Matrix2& matrix) noexcept;
        inline constexpr Matrix2& operator-=(const Matrix2& matrix) noexcept;
        inline constexpr Matrix2& operator*=(float scalar) noexcept;
        inline constexpr Matrix2& operator*=(const Matrix2& matrix) noexcept;
        inline constexpr Matrix2& operator/=(float scalar) noexcept;
    };

    inline constexpr Matrix2 operator+(const Matrix2& a, const Matrix2& b) noexcept;
    inline constexpr Matrix2 operator-(const Matrix2& a, const Matrix2& b) noexcept;
    inline constexpr Matrix2 operator-(const Matrix2& matrix) noexcept;
    inline constexpr Matrix2 operator*(const Matrix2& matrix, float scalar) noexcept;
    inline constexpr Matrix2 operator*(float scalar, const Matrix2& matrix) noexcept;
    inline constexpr Matrix2 operator*(const Matrix2& a, const Matrix2& b) noexcept;
    inline constexpr Matrix2 operator/(const Matrix2& matrix, float scalar) noexcept;

    inline constexpr float Determinant(const Matrix2& matrix) noexcept;
    inline constexpr Matrix2 Hadamard(const Matrix2& a, const Matrix2& b) noexcept;
    inline constexpr Matrix2 Transposed(const Matrix2& matrix) noexcept;
    inline constexpr Matrix2& Transpose(Matrix2& matrix) noexcept;
    inline constexpr Matrix2 Inversed(const Matrix2& matrix) noexcept;
    inline constexpr Matrix2& Inverse(Matrix2& matrix) noexcept;

    // Matrix3

    struct alignas(64) Matrix3
    {
        union
        {
            float data[9];
            Vector3 columns[3];
        };

        inline constexpr Matrix3() noexcept = default;
        inline constexpr Matrix3(const Matrix3& other) noexcept = default;
        inline constexpr Matrix3(Matrix3&& other) noexcept = default;
        inline constexpr Matrix3& operator=(const Matrix3& other) noexcept = default;
        inline constexpr Matrix3& operator=(Matrix3&& other) noexcept = default;
        inline constexpr ~Matrix3() noexcept = default;

        inline constexpr explicit Matrix3(float scalar) noexcept;
        inline constexpr explicit Matrix3(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) noexcept;
        inline constexpr explicit Matrix3(const Vector3& diagonal) noexcept;
        inline constexpr explicit Matrix3(const Vector3& column1, const Vector3& column2, const Vector3& column3) noexcept;
        inline constexpr explicit Matrix3(const Quaternion& quaternion) noexcept;

        inline constexpr const Vector3& operator[](size_t index) const noexcept;
        inline constexpr Vector3& operator[](size_t index) noexcept;
        inline constexpr Matrix3& operator+=(const Matrix3& matrix) noexcept;
        inline constexpr Matrix3& operator-=(const Matrix3& matrix) noexcept;
        inline constexpr Matrix3& operator*=(float scalar) noexcept;
        inline constexpr Matrix3& operator*=(const Matrix3& matrix) noexcept;
        inline constexpr Matrix3& operator/=(float scalar) noexcept;
    };

    inline constexpr Matrix3 operator+(const Matrix3& a, const Matrix3& b) noexcept;
    inline constexpr Matrix3 operator-(const Matrix3& a, const Matrix3& b) noexcept;
    inline constexpr Matrix3 operator-(const Matrix3& matrix) noexcept;
    inline constexpr Matrix3 operator*(const Matrix3& matrix, float scalar) noexcept;
    inline constexpr Matrix3 operator*(float scalar, const Matrix3& matrix) noexcept;
    inline constexpr Matrix3 operator*(const Matrix3& a, const Matrix3& b) noexcept;
    inline constexpr Matrix3 operator/(const Matrix3& matrix, float scalar) noexcept;

    inline constexpr float Determinant(const Matrix3& matrix) noexcept;
    inline constexpr Matrix3 Hadamard(const Matrix3& a, const Matrix3& b) noexcept;
    inline constexpr Matrix3 Transposed(const Matrix3& matrix) noexcept;
    inline constexpr Matrix3& Transpose(Matrix3& matrix) noexcept;
    inline constexpr Matrix3 Inversed(const Matrix3& matrix) noexcept;
    inline constexpr Matrix3& Inverse(Matrix3& matrix) noexcept;

    // Matrix4

    struct alignas(64) Matrix4
    {
        union
        {
            float data[16];
            Vector4 columns[4];
        };

        inline constexpr Matrix4() noexcept = default;
        inline constexpr Matrix4(const Matrix4& other) noexcept = default;
        inline constexpr Matrix4(Matrix4&& other) noexcept = default;
        inline constexpr Matrix4& operator=(const Matrix4& other) noexcept = default;
        inline constexpr Matrix4& operator=(Matrix4&& other) noexcept = default;
        inline constexpr ~Matrix4() noexcept = default;

        inline constexpr explicit Matrix4(float scalar) noexcept;
        inline constexpr explicit Matrix4(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3, float x4, float y4, float z4, float w4) noexcept;
        inline constexpr explicit Matrix4(const Vector4& diagonal) noexcept;
        inline constexpr explicit Matrix4(const Vector4& column1, const Vector4& column2, const Vector4& column3, const Vector4& column4) noexcept;
        inline constexpr explicit Matrix4(const Quaternion& quaternion) noexcept;

        inline constexpr const Vector4& operator[](size_t index) const noexcept;
        inline constexpr Vector4& operator[](size_t index) noexcept;
        inline constexpr Matrix4& operator+=(const Matrix4& matrix) noexcept;
        inline constexpr Matrix4& operator-=(const Matrix4& matrix) noexcept;
        inline constexpr Matrix4& operator*=(float scalar) noexcept;
        inline constexpr Matrix4& operator*=(const Matrix4& matrix) noexcept;
        inline constexpr Matrix4& operator/=(float scalar) noexcept;
    };

    inline constexpr Matrix4 operator+(const Matrix4& a, const Matrix4& b) noexcept;
    inline constexpr Matrix4 operator-(const Matrix4& a, const Matrix4& b) noexcept;
    inline constexpr Matrix4 operator-(const Matrix4& matrix) noexcept;
    inline constexpr Matrix4 operator*(const Matrix4& matrix, float scalar) noexcept;
    inline constexpr Matrix4 operator*(float scalar, const Matrix4& matrix) noexcept;
    inline constexpr Matrix4 operator*(const Matrix4& a, const Matrix4& b) noexcept;
    inline constexpr Matrix4 operator/(const Matrix4& matrix, float scalar) noexcept;

    inline constexpr float Determinant(const Matrix4& matrix) noexcept;
    inline constexpr Matrix4 Hadamard(const Matrix4& a, const Matrix4& b) noexcept;
    inline constexpr Matrix4 Transposed(const Matrix4& matrix) noexcept;
    inline constexpr Matrix4& Transpose(Matrix4& matrix) noexcept;
    inline constexpr Matrix4 Inversed(const Matrix4& matrix) noexcept;
    inline constexpr Matrix4& Inverse(Matrix4& matrix) noexcept;

    // Quaternion

    struct alignas(16) Quaternion
    {
        union
        {
            __m128 internal;
            struct { float a, b, c, d; };
        };

        inline constexpr Quaternion() noexcept = default;
        inline constexpr Quaternion(const Quaternion& other) noexcept = default;
        inline constexpr Quaternion(Quaternion&& other) noexcept = default;
        inline constexpr Quaternion& operator=(const Quaternion& other) noexcept = default;
        inline constexpr Quaternion& operator=(Quaternion&& other) noexcept = default;
        inline constexpr ~Quaternion() noexcept = default;

        inline constexpr explicit Quaternion(__m128 internal) noexcept;
        inline constexpr explicit Quaternion(float alpha, float beta, float gamma) noexcept;
        inline constexpr explicit Quaternion(float a, float b, float c, float d) noexcept;
        inline constexpr explicit Quaternion(const Vector3& axis, float angle) noexcept;
        inline constexpr explicit Quaternion(const Vector4& vector) noexcept;

        inline constexpr operator Matrix3() const noexcept;
        inline constexpr operator Matrix4() const noexcept;

        inline constexpr Quaternion& operator+=(const Quaternion& quaternion) noexcept;
        inline constexpr Quaternion& operator-=(const Quaternion& quaternion) noexcept;
		inline constexpr Quaternion& operator*=(float scalar) noexcept;
        inline constexpr Quaternion& operator*=(const Quaternion& quaternion) noexcept;
        inline constexpr Quaternion& operator/=(float scalar) noexcept;
    };

    inline constexpr Quaternion operator+(const Quaternion& p, const Quaternion& q) noexcept;
    inline constexpr Quaternion operator-(const Quaternion& p, const Quaternion& q) noexcept;
    inline constexpr Quaternion operator-(const Quaternion& quaternion) noexcept;
    inline constexpr Quaternion operator*(const Quaternion& quaternion, float scalar) noexcept;
    inline constexpr Quaternion operator*(float scalar, const Quaternion& quaternion) noexcept;
    inline constexpr Quaternion operator*(const Quaternion& p, const Quaternion& q) noexcept;
    inline constexpr Quaternion operator/(const Quaternion& quaternion, float scalar) noexcept;

    inline constexpr float Dot(const Quaternion& p, const Quaternion& q) noexcept;
    inline constexpr float Length(const Quaternion& quaternion) noexcept;
    inline constexpr float LengthSquared(const Quaternion& quaternion) noexcept;
    inline constexpr Quaternion Conjugated(const Quaternion& quaternion) noexcept;
    inline constexpr Quaternion& Conjugate(Quaternion& quaternion) noexcept;
    inline constexpr Quaternion Normalized(const Quaternion& quaternion) noexcept;
    inline constexpr Quaternion& Normalize(Quaternion& quaternion) noexcept;
    inline constexpr Quaternion Inversed(const Quaternion& quaternion) noexcept;
    inline constexpr Quaternion& Inverse(Quaternion& quaternion) noexcept;

    // Definitions

    // Vector2

    inline constexpr Vector2::Vector2(float scalar) noexcept : x(scalar), y(scalar) {}
    inline constexpr Vector2::Vector2(float x, float y) noexcept : x(x), y(y) {}

    inline constexpr float Vector2::operator[](size_t index) const noexcept
    {
        assert(index < 2);
        return data[index];
    }
    inline constexpr float& Vector2::operator[](size_t index) noexcept
    {
        assert(index < 2);
        return data[index];
    }
    inline constexpr Vector2& Vector2::operator+=(const Vector2& vector) noexcept
    {
        x += vector.x;
        y += vector.y;
        return *this;
    }
    inline constexpr Vector2& Vector2::operator-=(const Vector2& vector) noexcept
    {
        x -= vector.x;
        y -= vector.y;
        return *this;
    }
    inline constexpr Vector2& Vector2::operator*=(float scalar) noexcept
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    inline constexpr Vector2& Vector2::operator*=(const Matrix2& matrix) noexcept { return *this = *this * matrix; }
    inline constexpr Vector2& Vector2::operator/=(float scalar) noexcept
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    inline constexpr Vector2 operator+(const Vector2& u, const Vector2& v) noexcept { return Vector2(u.x + v.x, u.y + v.y); }
    inline constexpr Vector2 operator-(const Vector2& u, const Vector2& v) noexcept { return Vector2(u.x - v.x, u.y - v.y); }
    inline constexpr Vector2 operator-(const Vector2& vector) noexcept { return Vector2(-vector.x, -vector.y); }
    inline constexpr Vector2 operator*(const Vector2& vector, float scalar) noexcept { return Vector2(vector.x * scalar, vector.y * scalar); }
    inline constexpr Vector2 operator*(float scalar, const Vector2& vector) noexcept { return vector * scalar; }
    inline constexpr Vector2 operator*(const Matrix2& matrix, const Vector2& vector) noexcept
    {
        return matrix.columns[0] * vector.x + matrix.columns[1] * vector.y;
    }
    inline constexpr Vector2 operator*(const Vector2& vector, const Matrix2& matrix) noexcept
    {
        return Vector2(
            Dot(vector, matrix.columns[0]),
            Dot(vector, matrix.columns[1])
        );
    }
    inline constexpr Vector2 operator/(const Vector2& vector, float scalar) noexcept { return Vector2(vector.x / scalar, vector.y / scalar); }

    inline constexpr float Dot(const Vector2& u, const Vector2& v) noexcept { return u.x * v.x + u.y * v.y; }
    inline constexpr float Length(const Vector2& vector) noexcept { return SquareRoot(Dot(vector, vector)); }
    inline constexpr float LengthSquared(const Vector2& vector) noexcept { return Dot(vector, vector); }
    inline constexpr float Distance(const Vector2& a, const Vector2& b) noexcept { return Length(a - b); }
    inline constexpr float DistanceSquared(const Vector2& a, const Vector2& b) noexcept { return LengthSquared(a - b); }
    inline constexpr Vector2 Hadamard(const Vector2& u, const Vector2& v) noexcept { return Vector2(u.x * v.x, u.y * v.y); }
    inline constexpr Vector2 Normalized(const Vector2& vector) noexcept { return vector / Length(vector); }
    inline constexpr Vector2& Normalize(Vector2& vector) noexcept { return vector /= Length(vector); }

    // Vector3

    inline constexpr Vector3::Vector3(__m128 internal) noexcept : internal(internal) {}
    inline constexpr Vector3::Vector3(float scalar) noexcept : internal(_mm_set1_ps(scalar)) {}
    inline constexpr Vector3::Vector3(float x, float y, float z) noexcept : internal(_mm_setr_ps(x, y, z, 0.0f)) {}
    inline constexpr Vector3::Vector3(const Vector2& xy, float z) noexcept : x(xy.x), y(xy.y), z(z) {}
    inline constexpr Vector3::Vector3(float x, const Vector2& yz) noexcept : x(x), y(yz.x), z(yz.y) {}

    inline constexpr Vector3::operator Vector2() const noexcept { return Vector2(x, y); }

    inline constexpr float Vector3::operator[](size_t index) const noexcept
    {
        assert(index < 3);
        return data[index];
    }
    inline constexpr float& Vector3::operator[](size_t index) noexcept
    {
        assert(index < 3);
        return data[index];
    }
    inline constexpr Vector3& Vector3::operator+=(const Vector3& vector) noexcept
    {
        internal = _mm_add_ps(internal, vector.internal);
        return *this;
    }
    inline constexpr Vector3& Vector3::operator-=(const Vector3& vector) noexcept
    {
        internal = _mm_sub_ps(internal, vector.internal);
        return *this;
    }
    inline constexpr Vector3& Vector3::operator*=(float scalar) noexcept
    {
        internal = _mm_mul_ps(internal, _mm_set1_ps(scalar));
        return *this;
    }
    inline constexpr Vector3& Vector3::operator*=(const Matrix3& matrix) noexcept { return *this = *this * matrix; }
    inline constexpr Vector3& Vector3::operator/=(float scalar) noexcept
    {
        internal = _mm_div_ps(internal, _mm_set1_ps(scalar));
        return *this;
    }

    inline constexpr Vector3 operator+(const Vector3& u, const Vector3& v) noexcept { return Vector3(_mm_add_ps(u.internal, v.internal)); }
    inline constexpr Vector3 operator-(const Vector3& u, const Vector3& v) noexcept { return Vector3(_mm_sub_ps(u.internal, v.internal)); }
    inline constexpr Vector3 operator-(const Vector3& vector) noexcept { return Vector3(_mm_xor_ps(vector.internal, _mm_set1_ps(-0.0f))); }
    inline constexpr Vector3 operator*(const Vector3& vector, float scalar) noexcept { return Vector3(_mm_mul_ps(vector.internal, _mm_set1_ps(scalar))); }
    inline constexpr Vector3 operator*(float scalar, const Vector3& vector) noexcept { return vector * scalar; }
    inline constexpr Vector3 operator*(const Matrix3& matrix, const Vector3& vector) noexcept
    {
        return matrix.columns[0] * vector.x + matrix.columns[1] * vector.y + matrix.columns[2] * vector.z;
    }
    inline constexpr Vector3 operator*(const Vector3& vector, const Matrix3& matrix) noexcept
    {
        return Vector3(
            Dot(vector, matrix.columns[0]),
            Dot(vector, matrix.columns[1]),
            Dot(vector, matrix.columns[2])
        );
    }
    inline constexpr Vector3 operator/(const Vector3& vector, float scalar) noexcept { return Vector3(_mm_div_ps(vector.internal, _mm_set1_ps(scalar))); }

    inline constexpr float Dot(const Vector3& u, const Vector3& v) noexcept { return u.x * v.x + u.y * v.y + u.z * v.z; }
    inline constexpr float Length(const Vector3& vector) noexcept { return SquareRoot(Dot(vector, vector)); }
    inline constexpr float LengthSquared(const Vector3& vector) noexcept { return Dot(vector, vector); }
    inline constexpr float Distance(const Vector3& a, const Vector3& b) noexcept { return Length(a - b); }
    inline constexpr float DistanceSquared(const Vector3& a, const Vector3& b) noexcept { return LengthSquared(a - b); }
    inline constexpr Vector3 Hadamard(const Vector3& u, const Vector3& v) noexcept { return Vector3(_mm_mul_ps(u.internal, v.internal)); }
    inline constexpr Vector3 Cross(const Vector3& u, const Vector3& v) noexcept { return Vector3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x); }
    inline constexpr Vector3 Normalized(const Vector3& vector) noexcept { return vector / Length(vector); }
    inline constexpr Vector3& Normalize(Vector3& vector) noexcept { return vector /= Length(vector); }
    inline constexpr Vector3 Rotated(const Vector3& vector, const Quaternion& quaternion) noexcept
    {
        const Vector3 imaginary(quaternion.b, quaternion.c, quaternion.d);
		const Vector3 tangent(Cross(imaginary, vector));
		const Vector3 radial(Cross(imaginary, tangent));
		return vector + ((tangent * quaternion.a) + radial) * 2.0f;
    }
    inline constexpr Vector3& Rotate(Vector3& vector, const Quaternion& quaternion) noexcept
    {
        const Vector3 imaginary(quaternion.b, quaternion.c, quaternion.d);
		const Vector3 tangent(Cross(imaginary, vector));
		const Vector3 radial(Cross(imaginary, tangent));
		return vector += ((tangent * quaternion.a) + radial) * 2.0f;
    }

    // Vector4

    inline constexpr Vector4::Vector4(__m128 internal) noexcept : internal(internal) {}
    inline constexpr Vector4::Vector4(float scalar) noexcept : internal(_mm_set1_ps(scalar)) {}
    inline constexpr Vector4::Vector4(float x, float y, float z, float w) noexcept : internal(_mm_setr_ps(x, y, z, w)) {}
    inline constexpr Vector4::Vector4(const Vector2& xy, float z, float w) noexcept : x(xy.x), y(xy.y), z(z), w(w) {}
    inline constexpr Vector4::Vector4(float x, const Vector2& yz, float w) noexcept : x(x), y(yz.x), z(yz.y), w(w) {}
    inline constexpr Vector4::Vector4(float x, float y, const Vector2& zw) noexcept : x(x), y(y), z(zw.x), w(zw.y) {}
    inline constexpr Vector4::Vector4(const Vector2& xy, const Vector2& zw) noexcept : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
    inline constexpr Vector4::Vector4(const Vector3& xyz, float w) noexcept : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
    inline constexpr Vector4::Vector4(float x, const Vector3& yzw) noexcept : x(x), y(yzw.x), z(yzw.y), w(yzw.z) {}

    inline constexpr Vector4::operator Vector2() const noexcept { return Vector2(x, y); }
    inline constexpr Vector4::operator Vector3() const noexcept { return Vector3(internal); }

    inline constexpr float Vector4::operator[](size_t index) const noexcept
    {
        assert(index < 4);
        return data[index];
    }
    inline constexpr float& Vector4::operator[](size_t index) noexcept
    {
        assert(index < 4);
        return data[index];
    }
    inline constexpr Vector4& Vector4::operator+=(const Vector4& vector) noexcept
    {
        internal = _mm_add_ps(internal, vector.internal);
        return *this;
    }
    inline constexpr Vector4& Vector4::operator-=(const Vector4& vector) noexcept
    {
        internal = _mm_sub_ps(internal, vector.internal);
        return *this;
    }
    inline constexpr Vector4& Vector4::operator*=(float scalar) noexcept
    {
        internal = _mm_mul_ps(internal, _mm_set1_ps(scalar));
        return *this;
    }
    inline constexpr Vector4& Vector4::operator*=(const Matrix4& matrix) noexcept { return *this = *this * matrix; }
    inline constexpr Vector4& Vector4::operator/=(float scalar) noexcept
    {
        internal = _mm_div_ps(internal, _mm_set1_ps(scalar));
        return *this;
    }

    inline constexpr Vector4 operator+(const Vector4& u, const Vector4& v) noexcept { return Vector4(_mm_add_ps(u.internal, v.internal)); }
    inline constexpr Vector4 operator-(const Vector4& u, const Vector4& v) noexcept { return Vector4(_mm_sub_ps(u.internal, v.internal)); }
    inline constexpr Vector4 operator-(const Vector4& vector) noexcept { return Vector4(_mm_xor_ps(vector.internal, _mm_set1_ps(-0.0f))); }
    inline constexpr Vector4 operator*(const Vector4& vector, float scalar) noexcept { return Vector4(_mm_mul_ps(vector.internal, _mm_set1_ps(scalar))); }
    inline constexpr Vector4 operator*(float scalar, const Vector4& vector) noexcept { return vector * scalar; }
    inline constexpr Vector4 operator*(const Matrix4& matrix, const Vector4& vector) noexcept
    {
        return matrix.columns[0] * vector.x + matrix.columns[1] * vector.y + matrix.columns[2] * vector.z + matrix.columns[3] * vector.w;
    }
    inline constexpr Vector4 operator*(const Vector4& vector, const Matrix4& matrix) noexcept
    {
        return Vector4(
            Dot(vector, matrix.columns[0]),
            Dot(vector, matrix.columns[1]),
            Dot(vector, matrix.columns[2]),
            Dot(vector, matrix.columns[3])
        );
    }
    inline constexpr Vector4 operator/(const Vector4& vector, float scalar) noexcept { return Vector4(_mm_div_ps(vector.internal, _mm_set1_ps(scalar))); }

    inline constexpr float Dot(const Vector4& u, const Vector4& v) noexcept { return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w; }
    inline constexpr float Length(const Vector4& vector) noexcept { return SquareRoot(Dot(vector, vector)); }
    inline constexpr float LengthSquared(const Vector4& vector) noexcept { return Dot(vector, vector); }
    inline constexpr float Distance(const Vector4& a, const Vector4& b) noexcept { return Length(a - b); }
    inline constexpr float DistanceSquared(const Vector4& a, const Vector4& b) noexcept { return LengthSquared(a - b); }
    inline constexpr Vector4 Hadamard(const Vector4& u, const Vector4& v) noexcept { return Vector4(_mm_mul_ps(u.internal, v.internal)); }
    inline constexpr Vector4 Normalized(const Vector4& vector) noexcept { return vector / Length(vector); }
    inline constexpr Vector4& Normalize(Vector4& vector) noexcept { return vector /= Length(vector); }

    // Matrix2

    inline constexpr Matrix2::Matrix2(float scalar) noexcept : columns
    {
        Vector2(scalar, 0.0f),
        Vector2(0.0f, scalar)
    } {}
    inline constexpr Matrix2::Matrix2(float x1, float y1, float x2, float y2) noexcept : columns
    {
        Vector2(x1, y1),
        Vector2(x2, y2)
    } {}
    inline constexpr Matrix2::Matrix2(const Vector2& diagonal) noexcept : columns
    {
        Vector2(diagonal.x, 0.0f),
        Vector2(0.0f, diagonal.y)
    } {}
    inline constexpr Matrix2::Matrix2(const Vector2& column1, const Vector2& column2) noexcept : columns
    {
        column1,
        column2
    } {}

    inline constexpr const Vector2& Matrix2::operator[](size_t index) const noexcept
    {
        assert(index < 2);
        return columns[index];
    }
    inline constexpr Vector2& Matrix2::operator[](size_t index) noexcept
    {
        assert(index < 2);
        return columns[index];
    }
    inline constexpr Matrix2& Matrix2::operator+=(const Matrix2& matrix) noexcept
    {
        columns[0] += matrix.columns[0];
        columns[1] += matrix.columns[1];
        return *this;
    }
    inline constexpr Matrix2& Matrix2::operator-=(const Matrix2& matrix) noexcept
    {
        columns[0] -= matrix.columns[0];
        columns[1] -= matrix.columns[1];
        return *this;
    }
    inline constexpr Matrix2& Matrix2::operator*=(float scalar) noexcept
    {
        columns[0] *= scalar;
        columns[1] *= scalar;
        return *this;
    }
    inline constexpr Matrix2& Matrix2::operator*=(const Matrix2& matrix) noexcept { return *this = *this * matrix; }
    inline constexpr Matrix2& Matrix2::operator/=(float scalar) noexcept
    {
        columns[0] /= scalar;
        columns[1] /= scalar;
        return *this;
    }

    inline constexpr Matrix2 operator+(const Matrix2& a, const Matrix2& b) noexcept
    {
        return Matrix2(
            a.columns[0] + b.columns[0],
            a.columns[1] + b.columns[1]
        );
    }
    inline constexpr Matrix2 operator-(const Matrix2& a, const Matrix2& b) noexcept
    {
        return Matrix2(
            a.columns[0] - b.columns[0],
            a.columns[1] - b.columns[1]
        );
    }
    inline constexpr Matrix2 operator-(const Matrix2& matrix) noexcept
    {
        return Matrix2(
            -matrix.columns[0],
            -matrix.columns[1]
        );
    }
    inline constexpr Matrix2 operator*(const Matrix2& matrix, float scalar) noexcept
    {
        return Matrix2(
            matrix.columns[0] * scalar,
            matrix.columns[1] * scalar
        );
    }
    inline constexpr Matrix2 operator*(float scalar, const Matrix2& matrix) noexcept { return matrix * scalar; }
    inline constexpr Matrix2 operator*(const Matrix2& a, const Matrix2& b) noexcept
    {
        return Matrix2(
            a * b.columns[0],
            a * b.columns[1]
        );
    }
    inline constexpr Matrix2 operator/(const Matrix2& matrix, float scalar) noexcept
    {
        return Matrix2(
            matrix.columns[0] / scalar,
            matrix.columns[1] / scalar
        );
    }

    inline constexpr float Determinant(const Matrix2& matrix) noexcept { return matrix.columns[0].x * matrix.columns[1].y - matrix.columns[1].x * matrix.columns[0].y; }
    inline constexpr Matrix2 Hadamard(const Matrix2& a, const Matrix2& b) noexcept
    {
        return Matrix2(
            Hadamard(a.columns[0], b.columns[0]),
            Hadamard(a.columns[1], b.columns[1])
        );
    }
    inline constexpr Matrix2 Transposed(const Matrix2& matrix) noexcept
    {
        return Matrix2(
            Vector2(matrix.columns[0].x, matrix.columns[1].x),
            Vector2(matrix.columns[0].y, matrix.columns[1].y)
        );
    }
    inline constexpr Matrix2& Transpose(Matrix2& matrix) noexcept { return matrix = Transposed(matrix); }
    inline constexpr Matrix2 Inversed(const Matrix2& matrix) noexcept
    {
        const float inverse = 1.0f / Determinant(matrix);
        return Matrix2(
            + matrix.columns[1].y * inverse, - matrix.columns[0].y * inverse,
            - matrix.columns[1].x * inverse, + matrix.columns[0].x * inverse
        );
    }
    inline constexpr Matrix2& Inverse(Matrix2& matrix) noexcept { return matrix = Inversed(matrix); }

    // Matrix3

    inline constexpr Matrix3::Matrix3(float scalar) noexcept : columns
    {
        Vector3(scalar, 0.0f, 0.0f),
        Vector3(0.0f, scalar, 0.0f),
        Vector3(0.0f, 0.0f, scalar)
    } {}
    inline constexpr Matrix3::Matrix3(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) noexcept : columns
    {
        Vector3(x1, y1, z1),
        Vector3(x2, y2, z2),
        Vector3(x3, y3, z3)
    } {}
    inline constexpr Matrix3::Matrix3(const Vector3& diagonal) noexcept : columns
    {
        Vector3(diagonal.x, 0.0f, 0.0f),
        Vector3(0.0f, diagonal.y, 0.0f),
        Vector3(0.0f, 0.0f, diagonal.z)
    } {}
    inline constexpr Matrix3::Matrix3(const Vector3& column1, const Vector3& column2, const Vector3& column3) noexcept : columns
    {
        column1,
        column2,
        column3
    } {}
    inline constexpr Matrix3::Matrix3(const Quaternion& quaternion) noexcept
    {
        const float bb = quaternion.b * quaternion.b;
		const float cc = quaternion.c * quaternion.c;
		const float dd = quaternion.d * quaternion.d;
		const float bd = quaternion.b * quaternion.d;
		const float bc = quaternion.b * quaternion.c;
		const float cd = quaternion.c * quaternion.d;
		const float ab = quaternion.a * quaternion.b;
		const float ac = quaternion.a * quaternion.c;
		const float ad = quaternion.a * quaternion.d;

        columns[0] = Vector3(1.0f - 2.0f * (cc + dd),           2.0f * (bc + ad),           2.0f * (bd - ac));
        columns[1] = Vector3(       2.0f * (bc - ad),    1.0f - 2.0f * (bb + dd),           2.0f * (cd + ab));
        columns[2] = Vector3(       2.0f * (bd + ac),           2.0f * (cd - ab),    1.0f - 2.0f * (bb + cc));
    }

    inline constexpr const Vector3& Matrix3::operator[](size_t index) const noexcept
    {
        assert(index < 3);
        return columns[index];
    }
    inline constexpr Vector3& Matrix3::operator[](size_t index) noexcept
    {
        assert(index < 3);
        return columns[index];
    }
    inline constexpr Matrix3& Matrix3::operator+=(const Matrix3& matrix) noexcept
    {
        columns[0] += matrix.columns[0];
        columns[1] += matrix.columns[1];
        columns[2] += matrix.columns[2];
        return *this;
    }
    inline constexpr Matrix3& Matrix3::operator-=(const Matrix3& matrix) noexcept
    {
        columns[0] -= matrix.columns[0];
        columns[1] -= matrix.columns[1];
        columns[2] -= matrix.columns[2];
        return *this;
    }
    inline constexpr Matrix3& Matrix3::operator*=(float scalar) noexcept
    {
        columns[0] *= scalar;
        columns[1] *= scalar;
        columns[2] *= scalar;
        return *this;
    }
    inline constexpr Matrix3& Matrix3::operator*=(const Matrix3& matrix) noexcept { return *this = *this * matrix; }
    inline constexpr Matrix3& Matrix3::operator/=(float scalar) noexcept
    {
        columns[0] /= scalar;
        columns[1] /= scalar;
        columns[2] /= scalar;
        return *this;
    }

    inline constexpr Matrix3 operator+(const Matrix3& a, const Matrix3& b) noexcept
    {
        return Matrix3(
            a.columns[0] + b.columns[0],
            a.columns[1] + b.columns[1],
            a.columns[2] + b.columns[2]
        );
    }
    inline constexpr Matrix3 operator-(const Matrix3& a, const Matrix3& b) noexcept
    {
        return Matrix3(
            a.columns[0] - b.columns[0],
            a.columns[1] - b.columns[1],
            a.columns[2] - b.columns[2]
        );
    }
    inline constexpr Matrix3 operator-(const Matrix3& matrix) noexcept
    {
        return Matrix3(
            -matrix.columns[0],
            -matrix.columns[1],
            -matrix.columns[2]
        );
    }
    inline constexpr Matrix3 operator*(const Matrix3& matrix, float scalar) noexcept
    {
        return Matrix3(
            matrix.columns[0] * scalar,
            matrix.columns[1] * scalar,
            matrix.columns[2] * scalar
        );
    }
    inline constexpr Matrix3 operator*(float scalar, const Matrix3& matrix) noexcept { return matrix * scalar; }
    inline constexpr Matrix3 operator*(const Matrix3& a, const Matrix3& b) noexcept
    {
        return Matrix3(
            a * b.columns[0],
            a * b.columns[1],
            a * b.columns[2]
        );
    }
    inline constexpr Matrix3 operator/(const Matrix3& matrix, float scalar) noexcept
    {
        return Matrix3(
            matrix.columns[0] / scalar,
            matrix.columns[1] / scalar,
            matrix.columns[2] / scalar
        );
    }

    inline constexpr float Determinant(const Matrix3& matrix) noexcept
    {
        return + matrix.columns[0].x * (matrix.columns[1].y * matrix.columns[2].z - matrix.columns[2].y * matrix.columns[1].z)
			   - matrix.columns[1].x * (matrix.columns[0].y * matrix.columns[2].z - matrix.columns[2].y * matrix.columns[0].z)
			   + matrix.columns[2].x * (matrix.columns[0].y * matrix.columns[1].z - matrix.columns[1].y * matrix.columns[0].z);
    }
    inline constexpr Matrix3 Hadamard(const Matrix3& a, const Matrix3& b) noexcept
    {
        return Matrix3(
            Hadamard(a.columns[0], b.columns[0]),
            Hadamard(a.columns[1], b.columns[1]),
            Hadamard(a.columns[2], b.columns[2])
        );
    }
    inline constexpr Matrix3 Transposed(const Matrix3& matrix) noexcept
    {
        return Matrix3(
            Vector3(matrix.columns[0].x, matrix.columns[1].x, matrix.columns[2].x),
            Vector3(matrix.columns[0].y, matrix.columns[1].y, matrix.columns[2].y),
            Vector3(matrix.columns[0].z, matrix.columns[1].z, matrix.columns[2].z)
        );
    }
    inline constexpr Matrix3& Transpose(Matrix3& matrix) noexcept { return matrix = Transposed(matrix); }
    inline constexpr Matrix3 Inversed(const Matrix3& matrix) noexcept
    {
        const float inverse = 1.0f / Determinant(matrix);
		return Matrix3(
            Vector3(
                + (matrix.columns[1].y * matrix.columns[2].z - matrix.columns[2].y * matrix.columns[1].z),
                - (matrix.columns[0].y * matrix.columns[2].z - matrix.columns[2].y * matrix.columns[0].z),
                + (matrix.columns[0].y * matrix.columns[1].z - matrix.columns[1].y * matrix.columns[0].z)
            ) * inverse,
            Vector3(
                - (matrix.columns[1].x * matrix.columns[2].z - matrix.columns[2].x * matrix.columns[1].z),
                + (matrix.columns[0].x * matrix.columns[2].z - matrix.columns[2].x * matrix.columns[0].z),
                - (matrix.columns[0].x * matrix.columns[1].z - matrix.columns[1].x * matrix.columns[0].z)
            ) * inverse,
            Vector3(
                + (matrix.columns[1].x * matrix.columns[2].y - matrix.columns[2].x * matrix.columns[1].y),
                - (matrix.columns[0].x * matrix.columns[2].y - matrix.columns[2].x * matrix.columns[0].y),
                + (matrix.columns[0].x * matrix.columns[1].y - matrix.columns[1].x * matrix.columns[0].y)
            ) * inverse
        );
    }
    inline constexpr Matrix3& Inverse(Matrix3& matrix) noexcept { return matrix = Inversed(matrix); }

    // Matrix4

    inline constexpr Matrix4::Matrix4(float scalar) noexcept : columns
    {
        Vector4(scalar, 0.0f, 0.0f, 0.0f),
        Vector4(0.0f, scalar, 0.0f, 0.0f),
        Vector4(0.0f, 0.0f, scalar, 0.0f),
        Vector4(0.0f, 0.0f, 0.0f, scalar)
    } {}
    inline constexpr Matrix4::Matrix4(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3, float x4, float y4, float z4, float w4) noexcept : columns
    {
        Vector4(x1, y1, z1, w1),
        Vector4(x2, y2, z2, w2),
        Vector4(x3, y3, z3, w3),
        Vector4(x4, y4, z4, w4)
    } {}
    inline constexpr Matrix4::Matrix4(const Vector4& diagonal) noexcept : columns
    {
        Vector4(diagonal.x, 0.0f, 0.0f, 0.0f),
        Vector4(0.0f, diagonal.y, 0.0f, 0.0f),
        Vector4(0.0f, 0.0f, diagonal.z, 0.0f),
        Vector4(0.0f, 0.0f, 0.0f, diagonal.w)
    } {}
    inline constexpr Matrix4::Matrix4(const Vector4& column1, const Vector4& column2, const Vector4& column3, const Vector4& column4) noexcept : columns
    {
        column1,
        column2,
        column3,
        column4
    } {}
    inline constexpr Matrix4::Matrix4(const Quaternion& quaternion) noexcept
    {
        const float bb = quaternion.b * quaternion.b;
		const float cc = quaternion.c * quaternion.c;
		const float dd = quaternion.d * quaternion.d;
		const float bd = quaternion.b * quaternion.d;
		const float bc = quaternion.b * quaternion.c;
		const float cd = quaternion.c * quaternion.d;
		const float ab = quaternion.a * quaternion.b;
		const float ac = quaternion.a * quaternion.c;
		const float ad = quaternion.a * quaternion.d;

        columns[0] = Vector4(1.0f - 2.0f * (cc + dd),           2.0f * (bc + ad),           2.0f * (bd - ac),                       0.0f);
        columns[1] = Vector4(       2.0f * (bc - ad),    1.0f - 2.0f * (bb + dd),           2.0f * (cd + ab),                       0.0f);
        columns[2] = Vector4(       2.0f * (bd + ac),           2.0f * (cd - ab),    1.0f - 2.0f * (bb + cc),                       0.0f);
        columns[3] = Vector4(                   0.0f,                       0.0f,                       0.0f,                       1.0f);
    }

    inline constexpr const Vector4& Matrix4::operator[](size_t index) const noexcept
    {
        assert(index < 4);
        return columns[index];
    }
    inline constexpr Vector4& Matrix4::operator[](size_t index) noexcept
    {
        assert(index < 4);
        return columns[index];
    }
    inline constexpr Matrix4& Matrix4::operator+=(const Matrix4& matrix) noexcept
    {
        columns[0] += matrix.columns[0];
        columns[1] += matrix.columns[1];
        columns[2] += matrix.columns[2];
        columns[3] += matrix.columns[3];
        return *this;
    }
    inline constexpr Matrix4& Matrix4::operator-=(const Matrix4& matrix) noexcept
    {
        columns[0] -= matrix.columns[0];
        columns[1] -= matrix.columns[1];
        columns[2] -= matrix.columns[2];
        columns[3] -= matrix.columns[3];
        return *this;
    }
    inline constexpr Matrix4& Matrix4::operator*=(float scalar) noexcept
    {
        columns[0] *= scalar;
        columns[1] *= scalar;
        columns[2] *= scalar;
        columns[3] *= scalar;
        return *this;
    }
    inline constexpr Matrix4& Matrix4::operator*=(const Matrix4& matrix) noexcept { return *this = *this * matrix; }
    inline constexpr Matrix4& Matrix4::operator/=(float scalar) noexcept
    {
        columns[0] /= scalar;
        columns[1] /= scalar;
        columns[2] /= scalar;
        columns[3] /= scalar;
        return *this;
    }

    inline constexpr Matrix4 operator+(const Matrix4& a, const Matrix4& b) noexcept
    {
        return Matrix4(
            a.columns[0] + b.columns[0],
            a.columns[1] + b.columns[1],
            a.columns[2] + b.columns[2],
            a.columns[3] + b.columns[3]
        );
    }
    inline constexpr Matrix4 operator-(const Matrix4& a, const Matrix4& b) noexcept
    {
        return Matrix4(
            a.columns[0] - b.columns[0],
            a.columns[1] - b.columns[1],
            a.columns[2] - b.columns[2],
            a.columns[3] - b.columns[3]
        );
    }
    inline constexpr Matrix4 operator-(const Matrix4& matrix) noexcept
    {
        return Matrix4(
            -matrix.columns[0],
            -matrix.columns[1],
            -matrix.columns[2],
            -matrix.columns[3]
        );
    }
    inline constexpr Matrix4 operator*(const Matrix4& matrix, float scalar) noexcept
    {
        return Matrix4(
            matrix.columns[0] * scalar,
            matrix.columns[1] * scalar,
            matrix.columns[2] * scalar,
            matrix.columns[3] * scalar
        );
    }
    inline constexpr Matrix4 operator*(float scalar, const Matrix4& matrix) noexcept { return matrix * scalar; }
    inline constexpr Matrix4 operator*(const Matrix4& a, const Matrix4& b) noexcept
    {
        return Matrix4(
            a * b.columns[0],
            a * b.columns[1],
            a * b.columns[2],
            a * b.columns[3]
        );
    }
    inline constexpr Matrix4 operator/(const Matrix4& matrix, float scalar) noexcept
    {
        return Matrix4(
            matrix.columns[0] / scalar,
            matrix.columns[1] / scalar,
            matrix.columns[2] / scalar,
            matrix.columns[3] / scalar
        );
    }
    
    inline constexpr float Determinant(const Matrix4& matrix) noexcept
    {
        const float subfactor00 = matrix.columns[2].z * matrix.columns[3].w - matrix.columns[3].z * matrix.columns[2].w;
        const float subfactor01 = matrix.columns[2].y * matrix.columns[3].w - matrix.columns[3].y * matrix.columns[2].w;
        const float subfactor02 = matrix.columns[2].y * matrix.columns[3].z - matrix.columns[3].y * matrix.columns[2].z;
        const float subfactor03 = matrix.columns[2].x * matrix.columns[3].w - matrix.columns[3].x * matrix.columns[2].w;
        const float subfactor04 = matrix.columns[2].x * matrix.columns[3].z - matrix.columns[3].x * matrix.columns[2].z;
        const float subfactor05 = matrix.columns[2].x * matrix.columns[3].y - matrix.columns[3].x * matrix.columns[2].y;

        const Vector4 coefficient(
            + (matrix.columns[1].y * subfactor00 - matrix.columns[1].z * subfactor01 + matrix.columns[1].w * subfactor02),
            - (matrix.columns[1].x * subfactor00 - matrix.columns[1].z * subfactor03 + matrix.columns[1].w * subfactor04),
            + (matrix.columns[1].x * subfactor01 - matrix.columns[1].y * subfactor03 + matrix.columns[1].w * subfactor05),
            - (matrix.columns[1].x * subfactor02 - matrix.columns[1].y * subfactor04 + matrix.columns[1].z * subfactor05)
        );

        return matrix.columns[0].x * coefficient.x + matrix.columns[0].y * coefficient.y + matrix.columns[0].z * coefficient.z + matrix.columns[0].w * coefficient.w;
    }
    inline constexpr Matrix4 Hadamard(const Matrix4& a, const Matrix4& b) noexcept
    {
        return Matrix4(
            Hadamard(a.columns[0], b.columns[0]),
            Hadamard(a.columns[1], b.columns[1]),
            Hadamard(a.columns[2], b.columns[2]),
            Hadamard(a.columns[3], b.columns[3])
        );
    }
    inline constexpr Matrix4 Transposed(const Matrix4& matrix) noexcept
    {
        return Matrix4(
            Vector4(matrix.columns[0].x, matrix.columns[1].x, matrix.columns[2].x, matrix.columns[3].x),
            Vector4(matrix.columns[0].y, matrix.columns[1].y, matrix.columns[2].y, matrix.columns[3].y),
            Vector4(matrix.columns[0].z, matrix.columns[1].z, matrix.columns[2].z, matrix.columns[3].z),
            Vector4(matrix.columns[0].w, matrix.columns[1].w, matrix.columns[2].w, matrix.columns[3].w)
        );

        // __m128 tmp0 = _mm_unpacklo_ps(matrix.m128[0], matrix.m128[1]);
        // __m128 tmp1 = _mm_unpacklo_ps(matrix.m128[2], matrix.m128[3]);
        // __m128 tmp2 = _mm_unpackhi_ps(matrix.m128[0], matrix.m128[1]);
        // __m128 tmp3 = _mm_unpackhi_ps(matrix.m128[2], matrix.m128[3]);
        
        // return Matrix4(
        //     Vector4(_mm_movelh_ps(tmp0, tmp1)),
        //     Vector4(_mm_movehl_ps(tmp1, tmp0)),
        //     Vector4(_mm_movelh_ps(tmp2, tmp3)),
        //     Vector4(_mm_movehl_ps(tmp3, tmp2))
        // );
    }
    inline constexpr Matrix4& Transpose(Matrix4& matrix) noexcept { return matrix = Transposed(matrix); }
    inline constexpr Matrix4 Inversed(const Matrix4& matrix) noexcept
    {
        const float coefficient00 = matrix.columns[2].z * matrix.columns[3].w - matrix.columns[3].z * matrix.columns[2].w;
        const float coefficient02 = matrix.columns[1].z * matrix.columns[3].w - matrix.columns[3].z * matrix.columns[1].w;
        const float coefficient03 = matrix.columns[1].z * matrix.columns[2].w - matrix.columns[2].z * matrix.columns[1].w;

        const float coefficient04 = matrix.columns[2].y * matrix.columns[3].w - matrix.columns[3].y * matrix.columns[2].w;
        const float coefficient06 = matrix.columns[1].y * matrix.columns[3].w - matrix.columns[3].y * matrix.columns[1].w;
        const float coefficient07 = matrix.columns[1].y * matrix.columns[2].w - matrix.columns[2].y * matrix.columns[1].w;

        const float coefficient08 = matrix.columns[2].y * matrix.columns[3].z - matrix.columns[3].y * matrix.columns[2].z;
        const float coefficient10 = matrix.columns[1].y * matrix.columns[3].z - matrix.columns[3].y * matrix.columns[1].z;
        const float coefficient11 = matrix.columns[1].y * matrix.columns[2].z - matrix.columns[2].y * matrix.columns[1].z;

        const float coefficient12 = matrix.columns[2].x * matrix.columns[3].w - matrix.columns[3].x * matrix.columns[2].w;
        const float coefficient14 = matrix.columns[1].x * matrix.columns[3].w - matrix.columns[3].x * matrix.columns[1].w;
        const float coefficient15 = matrix.columns[1].x * matrix.columns[2].w - matrix.columns[2].x * matrix.columns[1].w;

        const float coefficient16 = matrix.columns[2].x * matrix.columns[3].z - matrix.columns[3].x * matrix.columns[2].z;
        const float coefficient18 = matrix.columns[1].x * matrix.columns[3].z - matrix.columns[3].x * matrix.columns[1].z;
        const float coefficient19 = matrix.columns[1].x * matrix.columns[2].z - matrix.columns[2].x * matrix.columns[1].z;

        const float coefficient20 = matrix.columns[2].x * matrix.columns[3].y - matrix.columns[3].x * matrix.columns[2].y;
        const float coefficient22 = matrix.columns[1].x * matrix.columns[3].y - matrix.columns[3].x * matrix.columns[1].y;
        const float coefficient23 = matrix.columns[1].x * matrix.columns[2].y - matrix.columns[2].x * matrix.columns[1].y;

        const Vector4 factor0(coefficient00, coefficient00, coefficient02, coefficient03);
        const Vector4 factor1(coefficient04, coefficient04, coefficient06, coefficient07);
        const Vector4 factor2(coefficient08, coefficient08, coefficient10, coefficient11);
        const Vector4 factor3(coefficient12, coefficient12, coefficient14, coefficient15);
        const Vector4 factor4(coefficient16, coefficient16, coefficient18, coefficient19);
        const Vector4 factor5(coefficient20, coefficient20, coefficient22, coefficient23);

        const Vector4 vector0(matrix.columns[1].x, matrix.columns[0].x, matrix.columns[0].x, matrix.columns[0].x);
        const Vector4 vector1(matrix.columns[1].y, matrix.columns[0].y, matrix.columns[0].y, matrix.columns[0].y);
        const Vector4 vector2(matrix.columns[1].z, matrix.columns[0].z, matrix.columns[0].z, matrix.columns[0].z);
        const Vector4 vector3(matrix.columns[1].w, matrix.columns[0].w, matrix.columns[0].w, matrix.columns[0].w);

        const Vector4 inverse0(Hadamard(vector1, factor0) - Hadamard(vector2, factor1) + Hadamard(vector3, factor2));
        const Vector4 inverse1(Hadamard(vector0, factor0) - Hadamard(vector2, factor3) + Hadamard(vector3, factor4));
        const Vector4 inverse2(Hadamard(vector0, factor1) - Hadamard(vector1, factor3) + Hadamard(vector3, factor5));
        const Vector4 inverse3(Hadamard(vector0, factor2) - Hadamard(vector1, factor4) + Hadamard(vector2, factor5));

        const Vector4 sign1( 1.0f, -1.0f,  1.0f, -1.0f);
        const Vector4 sign2(-1.0f,  1.0f, -1.0f,  1.0f);

        Matrix4 inverse(
            Hadamard(inverse0, sign1),
            Hadamard(inverse1, sign2),
            Hadamard(inverse2, sign1),
            Hadamard(inverse3, sign2)
        );

        const Vector4 row0(inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0]);
        return inverse * (1.0f / Dot(row0, matrix.columns[0]));
    }
    inline constexpr Matrix4& Inverse(Matrix4& matrix) noexcept { return matrix = Inversed(matrix); }

    // Quaternion

    inline constexpr Quaternion::Quaternion(__m128 internal) noexcept : internal(internal) {}
    inline constexpr Quaternion::Quaternion(float alpha, float beta, float gamma) noexcept
    {
        Vector3 half(alpha, beta, gamma);
        half *= 0.5f;
        const Vector3 cos(Cos(half.x), Cos(half.y), Cos(half.z));
		const Vector3 sin(Sin(half.x), Sin(half.y), Sin(half.z));

		a = cos.x * cos.y * cos.z + sin.x * sin.y * sin.z;
		b = sin.x * cos.y * cos.z - cos.x * sin.y * sin.z;
		c = cos.x * sin.y * cos.z + sin.x * cos.y * sin.z;
		d = cos.x * cos.y * sin.z - sin.x * sin.y * cos.z;
    }
    inline constexpr Quaternion::Quaternion(float a, float b, float c, float d) noexcept : internal(_mm_setr_ps(a, b, c, d)) {}
    inline constexpr Quaternion::Quaternion(const Vector3& axis, float angle) noexcept
    {
        const float sin = Sin(angle * 0.5f);
        a = Cos(angle * 0.5f);
        b = axis.x * sin;
        c = axis.y * sin;
        d = axis.z * sin;
    }
    inline constexpr Quaternion::Quaternion(const Vector4& vector) noexcept : internal(vector.internal) {}

    inline constexpr Quaternion::operator Matrix3() const noexcept
    {
        const float bb = b * b;
		const float cc = c * c;
		const float dd = d * d;
		const float bd = b * d;
		const float bc = b * c;
		const float cd = c * d;
		const float ab = a * b;
		const float ac = a * c;
		const float ad = a * d;

        return Matrix3(
            1.0f - 2.0f * (cc + dd),           2.0f * (bc + ad),           2.0f * (bd - ac),
                   2.0f * (bc - ad),    1.0f - 2.0f * (bb + dd),           2.0f * (cd + ab),
                   2.0f * (bd + ac),           2.0f * (cd - ab),    1.0f - 2.0f * (bb + cc)
        );
    }
    inline constexpr Quaternion::operator Matrix4() const noexcept
    {
        const float bb = b * b;
		const float cc = c * c;
		const float dd = d * d;
		const float bd = b * d;
		const float bc = b * c;
		const float cd = c * d;
		const float ab = a * b;
		const float ac = a * c;
		const float ad = a * d;

        return Matrix4(
            1.0f - 2.0f * (cc + dd),           2.0f * (bc + ad),           2.0f * (bd - ac),                       0.0f,
                   2.0f * (bc - ad),    1.0f - 2.0f * (bb + dd),           2.0f * (cd + ab),                       0.0f,
                   2.0f * (bd + ac),           2.0f * (cd - ab),    1.0f - 2.0f * (bb + cc),                       0.0f,
                               0.0f,                       0.0f,                       0.0f,                       1.0f
        );
    }

    inline constexpr Quaternion& Quaternion::operator+=(const Quaternion& quaternion) noexcept
    {
        internal = _mm_add_ps(internal, quaternion.internal);
        return *this;
    }
    inline constexpr Quaternion& Quaternion::operator-=(const Quaternion& quaternion) noexcept
    {
        internal = _mm_sub_ps(internal, quaternion.internal);
        return *this;
    }
    inline constexpr Quaternion& Quaternion::operator*=(float scalar) noexcept
    {
        internal = _mm_mul_ps(internal, _mm_set1_ps(scalar));
        return *this;
    }
    inline constexpr Quaternion& Quaternion::operator*=(const Quaternion& quaternion) noexcept { return *this = *this * quaternion; }
    inline constexpr Quaternion& Quaternion::operator/=(float scalar) noexcept
    {
        internal = _mm_div_ps(internal, _mm_set1_ps(scalar));
        return *this;
    }

    inline constexpr Quaternion operator+(const Quaternion& p, const Quaternion& q) noexcept { return Quaternion(_mm_add_ps(p.internal, q.internal)); }
    inline constexpr Quaternion operator-(const Quaternion& p, const Quaternion& q) noexcept { return Quaternion(_mm_sub_ps(p.internal, q.internal)); }
    inline constexpr Quaternion operator-(const Quaternion& quaternion) noexcept { return Quaternion(_mm_xor_ps(quaternion.internal, _mm_set1_ps(-0.0f))); }
    inline constexpr Quaternion operator*(const Quaternion& quaternion, float scalar) noexcept { return Quaternion(_mm_mul_ps(quaternion.internal, _mm_set1_ps(scalar))); }
    inline constexpr Quaternion operator*(float scalar, const Quaternion& quaternion) noexcept { return quaternion * scalar; }
    inline constexpr Quaternion operator*(const Quaternion& p, const Quaternion& q) noexcept
    {
		return Quaternion(
            p.a * q.a - p.b * q.b - p.c * q.c - p.d * q.d,
            p.a * q.b + p.b * q.a + p.c * q.d - p.d * q.c,
            p.a * q.c + p.c * q.a + p.d * q.b - p.b * q.d,
            p.a * q.d + p.d * q.a + p.b * q.c - p.c * q.b
		);
	}
    inline constexpr Quaternion operator/(const Quaternion& quaternion, float scalar) noexcept { return Quaternion(_mm_div_ps(quaternion.internal, _mm_set1_ps(scalar))); }

    inline constexpr float Dot(const Quaternion& p, const Quaternion& q) noexcept { return p.a * q.a + p.b * q.b + p.c * q.c + p.d * q.d; }
    inline constexpr float Length(const Quaternion& quaternion) noexcept { return SquareRoot(Dot(quaternion, quaternion)); }
    inline constexpr float LengthSquared(const Quaternion& quaternion) noexcept { return Dot(quaternion, quaternion); }
    inline constexpr Quaternion Conjugated(const Quaternion& quaternion) noexcept { return Quaternion(_mm_xor_ps(quaternion.internal, _mm_setr_ps(0.0f, -0.0f, -0.0f, -0.0f))); }
    inline constexpr Quaternion& Conjugate(Quaternion& quaternion) noexcept
    {
        quaternion.internal = _mm_xor_ps(quaternion.internal, _mm_setr_ps(0.0f, -0.0f, -0.0f, -0.0f));
        return quaternion;
    }
    inline constexpr Quaternion Normalized(const Quaternion& quaternion) noexcept { return quaternion / Length(quaternion); }
    inline constexpr Quaternion& Normalize(Quaternion& quaternion) noexcept { return quaternion /= Length(quaternion); }
    inline constexpr Quaternion Inversed(const Quaternion& quaternion) noexcept { return Conjugated(quaternion) / LengthSquared(quaternion); }
    inline constexpr Quaternion& Inverse(Quaternion& quaternion) noexcept { return Conjugate(quaternion) /= LengthSquared(quaternion); }
}