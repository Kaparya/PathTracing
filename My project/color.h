#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

using color = vec3;

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    pixel_color[0] /= samples_per_pixel;
    pixel_color[1] /= samples_per_pixel;
    pixel_color[2] /= samples_per_pixel;

    static const interval borders(0.f, 1 - INF_SMALL);

    out << static_cast<int>(255.999 *  borders.clamp(pixel_color.x())) << ' '
        << static_cast<int>(255.999 * borders.clamp(pixel_color.y())) << ' '
        << static_cast<int>(255.999 * borders.clamp(pixel_color.z())) << '\n';
}

#endif
