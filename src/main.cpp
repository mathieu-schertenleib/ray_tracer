#include "ray_tracer.hpp"

auto main() -> int
{
    const auto spheres = {
        rt::Colored_sphere{rt::Vec3{0, -10, 100}, 15.0, rt::Color{0, 0.6, 0}},
        rt::Colored_sphere{rt::Vec3{25, 0, 90}, 10.0, rt::Color{0, 0, 0.6}},
        rt::Colored_sphere{rt::Vec3{-20, -5, 80}, 5.0, rt::Color{0.5, 0, 0}},
        rt::Colored_sphere{rt::Vec3{0, 1000, 0}, 993.0, rt::Color{1, 1, 1}}};

    rt::Scene scene{rt::Camera{}, spheres, rt::Vec3{-60, -100, 90}};
    rt::draw_image(2560, 1440, "image.ppm", scene);

    return 0;
}
