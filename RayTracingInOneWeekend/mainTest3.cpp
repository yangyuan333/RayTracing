#include "color.h"
#include "ray.h"
#include "vec3.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define __STDC_LIB_EXT1__
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include <string>

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto half_b = dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return (-half_b - sqrt(discriminant)) / (a);
    }
}

 color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0)
    {
        vec3 normal = unit_vector(r.at(t) - point3(0, 0, -1));
        return (normal + vec3(1, 1, 1)) / 2.0;
    }
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    unsigned char* image = new unsigned char[image_width * image_height * 3];
    std::string outputPath = "./temp/img1.png";
    // Camera
    // 等价于提供 fov和radio
    // 这样直接指定不好，最好是根据相机内参进行转换
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    double x_offset = viewport_width / image_width;
    double y_offset = viewport_height / image_height;
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length) + vec3(x_offset / 2, y_offset / 2, 0);

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";


    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            ray r(origin, lower_left_corner + i * vec3(x_offset, 0, 0) + j * vec3(0, y_offset, 0) - origin);
            color pixel_color = ray_color(r);
        }
    }
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            color pixel_color = ray_color(r);
            write_color(image, pixel_color * 255, image_height - 1 - j, i, image_width);
        }
    }

    stbi_write_png(outputPath.c_str(), image_width, image_height, 3, image, 0);

    std::cerr << "\nDone.\n";
}