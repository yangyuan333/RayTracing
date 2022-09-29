#pragma once

#include "vec3.h"

#include <iostream>

// 得重写，往图像中写入像素值---后续替换为opencv或者stb_image.h
void write_color(std::ostream& out, color pixel_color) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

// 左上角为(0,0)，先从左往右，再从上往下
// color必须为0~255
void write_color(void* image, color pixel_color, int row, int col, int width) {
    // Write the translated [0,255] value of each color component.
    const int channel = 3;
    unsigned char* data = static_cast<unsigned char*>(image);
    data[row * channel * width + col * channel + 0] = static_cast<unsigned char>(sqrt(pixel_color[0] / 255) * 255);
    data[row * channel * width + col * channel + 1] = static_cast<unsigned char>(sqrt(pixel_color[1] / 255) * 255);
    data[row * channel * width + col * channel + 2] = static_cast<unsigned char>(sqrt(pixel_color[2] / 255) * 255);
}