#ifndef PROPERTY_H
#define PROPERTY_H

class Color {
 public:
    float r; float g; float b;
    int bit8R(); int bit8B(); int bit8G();
    void add(Color* color);
    void mult(float scalar);
};

class Material {
 public:
    Color ka; Color kd; Color ks; Color kr;
    float ksp;
};

#endif
