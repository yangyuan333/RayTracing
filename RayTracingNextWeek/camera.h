#pragma once

#include "rtweekend.h"

class camera {
public:
    camera(
        point3 lookfrom, // camera position in the world
        point3 lookat, // camera look at, decide the directioin '-z'
        vec3   vup, // decide the direction 'up' or rotation of '-z'
        double vfov,
        double aspect_ratio,
        double focal_length = 1,
        double _time0 = 0,
        double _time1 = 0
    ) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2) * focal_length;
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        auto w = unit_vector(lookfrom - lookat); // z axis of camera
        auto u = unit_vector(cross(vup, w)); // x axis of camera
        auto v = cross(w, u); // y axis of camera

        origin = lookfrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - w * focal_length;
    
        time0 = _time0;
        time1 = _time1;
    }

    ray get_ray(double s, double t) const {
        return ray(
            origin,
            lower_left_corner + s * horizontal + t * vertical - origin,
            random_double(time0, time1));
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    double time0;
    double time1;
};

class lenscamera {
public:
    lenscamera(
        point3 lookfrom, // camera position in the world
        point3 lookat, // camera look at, decide the directioin '-z'
        vec3   vup, // decide the direction 'up' or rotation of '-z'
        double vfov,
        double aspect_ratio,
        double aperture,
        double focus_dist,
        double focal_length = 1.0,
        double _time0 = 0,
        double _time1 = 0
    ) {
        // 假设成像平面与相机中心的深度为1
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = unit_vector(lookfrom - lookat); // z axis of camera
        u = unit_vector(cross(vup, w)); // x axis of camera
        v = cross(w, u); // y axis of camera

        zs = 1.0 / (1.0 / focal_length - 1.0 / focus_dist); // 实际深度

        origin = lookfrom;
        horizontal = zs * viewport_width * u;
        vertical = zs * viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 + zs * w;

        lens_radius = aperture / 2;
        zo = focus_dist;

        time0 = _time0;
        time1 = _time1;

    }

    ray get_ray(double s, double t) const {
        s = 1.0 - s;
        t = 1.0 - t;
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();

        vec3 x1 = lower_left_corner + horizontal * s + vertical * t;
        vec3 x2 = origin + offset;

        vec3 n = unit_vector(origin - x1);
        vec3 x3 = origin + n * (-zo / dot(n, w));

        vec3 direction = unit_vector(x3 - x2);

        return ray(
            x2,
            direction,
            random_double(time0, time1)
        );
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
    double zs;
    double zo;
    double time0;
    double time1;
};