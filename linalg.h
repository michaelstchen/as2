#ifndef LINALG_H
#define LINALG_H

//*************************************************
// Contains classes for objects linear-algebra-related
// objects used in the raytracer.
//
// The Classes:
//
//     --Point: just a 3D point. there are functions for
//              for adding points, multiplying with a scalar,
//              and creating a vector form two points. 
//     --Vector: you're typical 3D vector. there are functions
//               for adding, subtracting, multiplying, and
//               dot/cross products.
//     --Matrix: a 4x4 matrix. initialized to identity. there
//               are functions for transformations and 
//               multiplication.
//
//************************************************

class Point {
 public:
    Point() {}
    Point(float xIn, float yIn, float zIn);
    float x; float y; float z;
};

/* product of point and scalar -> a new point */
Point* mult(Point* p, float scalar);
/* sum of two points -> a new point */
Point* add(Point* p1, Point* p2);

class Vector {
 public:
    Vector() {}
    Vector(float xIn, float yIn, float zIn);
    float x; float y; float z;
    /* normalizes this vector to the corresponding unit vector */
    void normalize();
};

/* sum of point and vector -> a new point */
Point* add(Point* p, Vector* v);
/* returns a pointer to the vector p2 - p1 */
Vector* newVector(Point* p1, Point* p2);

/* the sum of two vectors -> a new vector*/
Vector* add(Vector* v1, Vector* v2);
/* the difference between vectors (v1-v2) -> a new vector*/
Vector* sub(Vector* v1, Vector* v2);
/* product of vector and scalar -> new vector */
Vector* mult(Vector* v, float scalar);
/* dot product of two vectors */
float dot(Vector* v1, Vector* v2);
/* returns a pointer to v1 x v2 */
Vector* cross(Vector* v1, Vector* v2);


class Matrix {
 protected:
    float mat[4][4] = 
        {
            {1,0,0,0},
            {0,1,0,0},
            {0,0,1,0},
            {0,0,0,1}
        };
 public:
    Matrix() {}
    /* returns value at the (m, n) index. 
     * m->row. n-> col */
    float getVal(int m, int n);
    void setVal(int m, int n, float val);
    /* prints our matrix. for debugging. */
    void print();

    /* returns the inverse of this matrix. */
    Matrix* invert();
    /* returns the transpose of this matrix. */
    Matrix* transpose();
};

/* various functions used to generate transformations. */
Matrix* makeTransl(float tx, float ty, float tz);
Matrix* makeRot(float rx, float ry, float rz);
Matrix* makeScale(float sx, float sy, float sz);
/* matrix composition. */
Matrix* compose(Matrix* a, Matrix* b);

/* matrix multiplication on the right my a 
 * point -> new Point*/
Point* matrixmult(Matrix* a, Point* b);
/* matrix multiplication on the right by a
 * vector -> new vector. */
Vector* matrixmult(Matrix* a, Vector* b);

#endif
