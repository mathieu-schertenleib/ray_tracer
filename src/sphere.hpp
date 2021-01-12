#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "vec3.hpp"

namespace rt
{
struct Sphere
{
    Vec3 center{};
    double radius{};
};
} // namespace rt

#endif