#ifndef LINALG_H
#define LINALG_H

class Point {
 public:
    Point() {}
    Point(float xIn, float yIn, float zIn);
    float x; float y; float z;
};

class Vector {
 public:
    Vector() {}
    Vector(float xIn, float yIn, float zIn);
    float x; float y; float z;
    void normalize();
};

Vector* add(Vector* v1, Vector* v2);
/* v1 - v2 */
Vector* sub(Vector* v1, Vector* v2);
Point* add(Point* p, Vector* v);
Point* add(Point* p1, Point* p2);
Vector* mult(Vector* v, float scalar);
Point* mult(Point* p, float scalar);
float dot(Vector* v1, Vector* v2);
/* returns a pointer to v1 x v2 */
Vector* cross(Vector* v1, Vector* v2);
/* returns a pointer to the vector p2 - p1 */
Vector* newVector(Point* p1, Point* p2);

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
    float getVal(int m, int n);
    void setVal(int m, int n, float val);
    void print();
    Matrix* invert();
    Matrix* transpose();
	//Matrix* makeTransl(float tx, float ty, float tz);
	void setMatrix(float nmat[4][4]);
	float determinant();
};

Matrix* makeTransl(float tx, float ty, float tz);
Matrix* makeRot(float rx, float ry, float rz);
Matrix* makeScale(float sx, float sy, float sz);
Matrix* compose(Matrix* a, Matrix* b);
float determinant(Matrix* m);

Point* matrixmult(Matrix* a, Point* b);
Vector* matrixmult(Matrix* a, Vector* b);

#endif
