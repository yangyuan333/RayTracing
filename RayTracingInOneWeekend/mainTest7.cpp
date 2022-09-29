/*
* diffuse materials测试
* 半球面均匀采样---半球面cos加权采样
*/
#include "rtweekend.h"
#include "camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define __STDC_LIB_EXT1__
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>

color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;
    if (depth <= 0)
        return color(0, 0, 0);
    if (world.hit(r, 0, infinity, rec)) {
        vec3 vec = random_hemi_sphere_cosine(rec.normal);
        return 0.5 * ray_color(ray(rec.p + 0.001 * rec.normal, vec), world, depth - 1);
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
    std::string outputPath = "./temp/mainTest7_cos.png";
    const int samples_per_pixel = 100;
    const int max_depth = 50;
    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera
    camera cam;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(image, pixel_color * 255.0 / samples_per_pixel, image_height - 1 - j, i, image_width);
        }
    }
    stbi_write_png(outputPath.c_str(), image_width, image_height, 3, image, 0);

    std::cerr << "\nDone.\n";
}