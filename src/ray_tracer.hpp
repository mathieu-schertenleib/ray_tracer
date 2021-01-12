#ifndef RAY_TRACER_HPP
#define RAY_TARCER_HPP

#include "sphere.hpp"
#include "vec3.hpp"

#include <cmath>
#include <string>
#include <vector>

namespace rt
{
using Color = Vec3;

struct Colored_sphere : Sphere
{
    Color color{};
};

struct Camera
{
    Vec3 position{0, -35, 30};
    Vec3 v_x{1.0, 0.0, 0.0};
    Vec3 v_y{0.0, std::cos(-M_PI / 6), std::sin(-M_PI / 6)};
    Vec3 v_z{0.0, -std::sin(-M_PI / 6), std::cos(-M_PI / 6)};
    double fov{M_PI / 2.0};
};

struct Scene
{
    Camera camera{};
    std::vector<Colored_sphere> spheres{};
    Vec3 light{};
};

void draw_image(unsigned int width, unsigned int height,
                const std::string &filename, const Scene &scene);
} // namespace rt

#endif