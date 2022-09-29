#pragma once

#include "vec3.h"

#include <iostream>

// ����д����ͼ����д������ֵ---�����滻Ϊopencv����stb_image.h
void write_color(std::ostream& out, color pixel_color) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

// ���Ͻ�Ϊ(0,0)���ȴ������ң��ٴ�������
// color����Ϊ0~255
void write_color(void* image, color pixel_color, int row, int col, int width) {
    // Write the translated [0,255] value of each color component.
    const int channel = 3;
    unsigned char* data = static_cast<unsigned char*>(image);
    data[row * channel * width + col * channel + 0] = static_cast<unsigned char>(sqrt(pixel_color[0] / 255) * 255);
    data[row * channel * width + col * channel + 1] = static_cast<unsigned char>(sqrt(pixel_color[1] / 255) * 255);
    data[row * channel * width + col * channel + 2] = static_cast<unsigned char>(sqrt(pixel_color[2] / 255) * 255);
}