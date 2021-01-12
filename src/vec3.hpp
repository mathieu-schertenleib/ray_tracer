#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>

namespace rt
{
struct Vec3
{
    double x;
    double y;
    double z;

    [[nodiscard]] constexpr auto
    operator<=>(const Vec3 &v) const noexcept = default;

    [[nodiscard]] constexpr auto operator+=(const Vec3 &v) noexcept -> Vec3 &
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    [[nodiscard]] constexpr auto operator-=(const Vec3 &v) noexcept -> Vec3 &
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    [[nodiscard]] constexpr auto operator*=(double t) noexcept -> Vec3 &
    {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    [[nodiscard]] constexpr auto operator/=(double t) noexcept -> Vec3 &
    {
        x /= t;
        y /= t;
        z /= t;
        return *this;
    }
};

[[nodiscard]] constexpr auto operator+(const Vec3 &u, const Vec3 &v) noexcept
{
    return Vec3{u.x + v.x, u.y + v.y, u.z + v.z};
}

[[nodiscard]] constexpr auto operator-(const Vec3 &u, const Vec3 &v) noexcept
{
    return Vec3{u.x - v.x, u.y - v.y, u.z - v.z};
}

// Dot product
[[nodiscard]] constexpr auto operator*(const Vec3 &u, const Vec3 &v) noexcept
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

// Cross product
[[nodiscard]] constexpr auto operator^(const Vec3 &u, const Vec3 &v) noexcept
{
    return Vec3{u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x};
}

[[nodiscard]] constexpr auto operator*(double t, const Vec3 &v) noexcept
{
    return Vec3{t * v.x, t * v.y, t * v.z};
}

[[nodiscard]] constexpr auto operator*(const Vec3 &v, double t) noexcept
{
    return t * v;
}

[[nodiscard]] constexpr auto operator/(const Vec3 &v, double t) noexcept
{
    return Vec3{v.x / t, v.y / t, v.z / t};
}

[[nodiscard]] constexpr auto norm(const Vec3 &v) noexcept
{
    return std::sqrt(v * v);
}

[[nodiscard]] constexpr auto normalised(const Vec3 &v) noexcept
{
    [[likely]] if (const auto norm_v = norm(v); norm_v != 0.0)
    {
        return v / norm_v;
    }
    return Vec3{};
}
} // namespace rt

#endif
