#include "ray_tracer.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <optional>

namespace
{
struct Ray
{
    rt::Vec3 origin{};
    rt::Vec3 direction{};
    rt::Color color{};
};

constexpr auto intersection(const Ray &ray, const rt::Sphere &sphere) noexcept
    -> std::optional<double>
{
    const auto co = ray.origin - sphere.center;
    const auto d_dot_co = ray.direction * co;
    const auto d_squared = ray.direction * ray.direction;
    const auto delta = d_dot_co * d_dot_co -
                       d_squared * (co * co - sphere.radius * sphere.radius);

    if (delta < 0.0)
        return {};

    const auto sqrt_delta = std::sqrt(delta);
    const auto minus_b = -d_dot_co;
    const auto t_minus = (minus_b - sqrt_delta) / d_squared;
    const auto t_plus = (minus_b + sqrt_delta) / d_squared;

    constexpr auto epsilon = 1e-6;
    if (t_minus > epsilon)
        return t_minus;
    if (t_plus > epsilon)
        return t_plus;

    return {};
}

constexpr auto reflect_ray(const rt::Vec3 &incident_direction,
                           const rt::Vec3 &hit_point,
                           const rt::Vec3 &normal) noexcept
{
    const auto i_perp =
        (incident_direction * normal) / (normal * normal) * normal;
    const auto reflected_direction = incident_direction - 2 * i_perp;
    return Ray{hit_point, reflected_direction};
}

void compute_ray_color(Ray &ray, const rt::Scene &scene) noexcept
{
    constexpr auto max_double = std::numeric_limits<double>::max();
    double t_min{max_double};
    std::size_t closest_sphere_index{};
    for (std::size_t i{}; const auto &sphere : scene.spheres)
    {
        const auto t = intersection(ray, sphere);
        if (t.has_value() && t.value() < t_min)
        {
            t_min = t.value();
            closest_sphere_index = i;
        }
        ++i;
    }
    if (t_min < max_double)
    {
        constexpr auto fraction = 0.7;
        ray.color = fraction * ray.color +
                    (1 - fraction) * scene.spheres[closest_sphere_index].color;
    }
}

auto compute_pixel_color(double u, double v, double aspect_ratio,
                         const rt::Scene &scene) noexcept
{
    const auto proj_cam_v_x = (u - 0.5) * scene.camera.v_x;
    const auto proj_cam_v_y = ((v - 0.5) / aspect_ratio) * scene.camera.v_y;
    const auto proj_cam_v_z =
        (0.5 / std::tan(scene.camera.fov * 0.5)) * scene.camera.v_z;
    const auto ray_direction = proj_cam_v_x + proj_cam_v_y + proj_cam_v_z;

    Ray ray{scene.camera.position, ray_direction};

    constexpr auto max_double = std::numeric_limits<double>::max();
    double t_min{max_double};
    std::size_t closest_sphere_index{};
    for (std::size_t i{}; const auto &sphere : scene.spheres)
    {
        const auto t = intersection(ray, sphere);
        if (t.has_value() && t.value() < t_min)
        {
            t_min = t.value();
            closest_sphere_index = i;
        }
        ++i;
    }
    if (t_min < max_double)
    {
        const auto &closest_sphere = scene.spheres[closest_sphere_index];
        const auto i = ray.direction;
        const auto p = ray.origin + t_min * i;
        const auto n = p - closest_sphere.center;
        auto reflected_ray = reflect_ray(i, p, n);
        reflected_ray.color =
            closest_sphere.color * (-(i * n) / (norm(i) * norm(n)));
        compute_ray_color(reflected_ray, scene);

        const Ray shadow_ray{p, scene.light - p};

        for (const auto &sphere : scene.spheres)
        {
            const auto t = intersection(shadow_ray, sphere);
            if (t.has_value())
            {
                reflected_ray.color = reflected_ray.color * 0.3;
            }
        }

        return reflected_ray.color;
    }

    return rt::Color{};
}
} // namespace

namespace rt
{
void draw_image(unsigned int width, unsigned int height,
                const std::string &filename, const Scene &scene)
{
    std::ofstream image(filename);
    if (!image.is_open())
        return;

    constexpr auto color_scale = 255;

    image << "P3\n" << width << ' ' << height << '\n' << color_scale << '\n';

    for (unsigned int i{}; i < height; ++i)
    {
        for (unsigned int j{}; j < width; ++j)
        {
            const auto width_d = static_cast<double>(width);
            const auto height_d = static_cast<double>(height);
            const auto color = compute_pixel_color(
                static_cast<double>(j) / width_d,
                static_cast<double>(i) / height_d, width_d / height_d, scene);
            image << static_cast<unsigned int>(color.x * color_scale) << ' '
                  << static_cast<unsigned int>(color.y * color_scale) << ' '
                  << static_cast<unsigned int>(color.z * color_scale) << ' ';
        }
        image << '\n';
    }

    image.close();
}
} // namespace rt
