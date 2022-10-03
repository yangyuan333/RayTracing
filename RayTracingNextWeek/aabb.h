#pragma once

#include "rtweekend.h"

struct general {

};

class aabb {
public:
    aabb() {}
    aabb(const point3& a, const point3& b) :minimum(a), maximum(b) {}
    aabb(const point3& c, double x, double y, double z) :minimum(c - point3(x / 2, y / 2, z / 2)), maximum(c + point3(x / 2, y / 2, z / 2)) {}

    point3 min() const { return minimum; }
    point3 max() const { return maximum; }
    // t_min:Լ���ر���Ľ���---��ֵ���������
    // t_max:������weekend�е����ã���ǰ��֦
    bool hit(const ray& r, double t_min, double t_max, general g) const {
        for (int a = 0; a < 3; a++) {
            auto t0 = fmin((minimum[a] - r.origin()[a]) / r.direction()[a],
                (maximum[a] - r.origin()[a]) / r.direction()[a]);
            auto t1 = fmax((minimum[a] - r.origin()[a]) / r.direction()[a],
                (maximum[a] - r.origin()[a]) / r.direction()[a]);
            t_min = fmax(t0, t_min);
            t_max = fmin(t1, t_max);
            if (t_max <= t_min)
                return false;
        }
        return true;
    }
    // �����ظ�������
    inline bool hit(const ray& r, double t_min, double t_max) const {
        for (int a = 0; a < 3; a++) {
            auto invD = 1.0f / r.direction()[a];
            auto t0 = (min()[a] - r.origin()[a]) * invD;
            auto t1 = (max()[a] - r.origin()[a]) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if (t_max <= t_min)
                return false;
        }
        return true;
    }

public:
    point3 minimum;
    point3 maximum;
};

inline aabb surrounding_box(aabb box0, aabb box1) {
    point3 miminimum = point3(
        std::min(box0.minimum[0], box1.minimum[0]),
        std::min(box0.minimum[1], box1.minimum[1]),
        std::min(box0.minimum[2], box1.minimum[2]));
    point3 maximum = point3(
        std::max(box0.maximum[0], box1.maximum[0]),
        std::max(box0.maximum[1], box1.maximum[1]),
        std::max(box0.maximum[2], box1.maximum[2]));
    return aabb(miminimum, maximum);
}