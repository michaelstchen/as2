#ifndef PROPERTY_H
#define PROPERTY_H

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
    /* multiplies this instance's RGB values by a scalar */
    void mult(float scalar);
};

class Material {
 public:
    Material() {}
    Material(Color kaIN, Color kdIN, Color ksIN, Color krIN, float kspIN);
    Color ka; Color kd; Color ks; Color kr;
    float ksp;
};

#endif
