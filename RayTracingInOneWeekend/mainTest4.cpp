/*
* 物体类别封装测试
*/
#include "rtweekend.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define __STDC_LIB_EXT1__
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>
color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

    // Image

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    unsigned char* image = new unsigned char[image_width * image_height * 3];
    std::string outputPath = "./temp/mainTest4.png";
    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera

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

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            ray r(origin, lower_left_corner + i * vec3(x_offset, 0, 0) + j * vec3(0, y_offset, 0) - origin);
            color pixel_color = ray_color(r, world);
            write_color(image, pixel_color * 255.0, image_height - 1 - j, i, image_width);
        }
    }
    stbi_write_png(outputPath.c_str(), image_width, image_height, 3, image, 0);

    std::cerr << "\nDone.\n";
}