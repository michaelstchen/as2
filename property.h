#ifndef PROPERTY_H
#define PROPERTY_H

//*************************************************
// Contains classes that store color data.
//
// The Classes:
//
//     --Color: stores color data in floating point
//              RGB values.
//     --Material: stores a shape's material lighting
//                 and reflectivity properties as
//                 Color values.
//
//************************************************


class Color {
 public:
    Color() {};
    Color(float rIn, float gIn, float bIn);
    float r; float g; float b;
    /* returns integer representation of color from 0-255
     * rounded down to the nearest integer */
    int bit8R(); int bit8B(); int bit8G();
    /* adds the RGB values of COLOR to this instance's RGB */
    void add(Color* color);
    void mult(float scalar);
    void mult(Color* c);
};

class Material {
 public:
    Material() {}
    Material(Color* kaIN, Color* kdIN, Color* ksIN, Color* krIN, float kspIN);
    Color* ka; Color* kd; Color* ks; Color* kr;
    float ksp;
};

#endif
