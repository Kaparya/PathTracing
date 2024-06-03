#ifndef COLOR_H
#define COLOR_H

#include "../GeometryObjects/vec3.h"

#include <iostream>

using color = vec3;

inline double linear_to_gamma(double color_component) {
    return sqrt(color_component);
}

//void write_color(std::ostream &out, color pixel_color) {
//    // Write the translated [0,255] value of each color component.
//    out << static_cast<int>(255.99999f * pixel_color.x()) << ' '
//        << static_cast<int>(255.99999f * pixel_color.y()) << ' '
//        << static_cast<int>(255.99999f * pixel_color.z()) << '\n';
//}

#endif