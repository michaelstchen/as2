#include "linalg.h"
#include <cmath>
#include <cstddef>
#include <stdio.h>

Point::Point(float xIn, float yIn, float zIn) {
    x = xIn; y = yIn; z = zIn;
}

Vector::Vector(float xIn, float yIn, float zIn) {
    x = xIn; y = yIn; z = zIn;
}

void Vector::normalize() {
    float mag = sqrt(x*x + y*y + z*z);
    if (mag == 0) return;
    x = x / mag; y = y / mag; z = z / mag;
}

Vector* add(Vector* v1, Vector* v2) {
    Vector* ret = new Vector();
    ret->x = v1->x + v2->x;
    ret->y = v1->y + v2->y;
    ret->z = v1->z + v2->z;
    return ret;
}

Vector* sub(Vector* v1, Vector* v2) {
    Vector* ret = new Vector();
    ret->x = v1->x - v2->x;
    ret->y = v1->y - v2->y;
    ret->z = v1->z - v2->z;
    return ret;
}

Point* add(Point* p, Vector* v) {
    Point* ret = new Point();
    ret->x = p->x + v->x;
    ret->y = p->y + v->y;
    ret->z = p->z + v->z;
    return ret;
}

Point* add(Point* p1, Point* p2) {
    Point* ret = new Point();
    ret->x = p1->x + p2->x;
    ret->y = p1->y + p2->y;
    ret->z = p1->z + p2->z;
    return ret;
}

Vector* mult(Vector* v, float scalar) {
    Vector* ret = new Vector();
    ret->x = v->x * scalar;
    ret->y = v->y * scalar;
    ret->z = v->z * scalar;
    return ret;
}

Point* mult(Point* p, float scalar) {
    Point* ret = new Point();
    ret->x = p->x * scalar;
    ret->y = p->y * scalar;
    ret->z = p->z * scalar;
    return ret;
}

float dot(Vector* v1, Vector* v2) {
    return (v1->x)*(v2->x) + (v1->y)*(v2->y) + (v1->z)*(v2->z);
}

Vector* cross(Vector* v1, Vector* v2) {
    Vector* ret = new Vector();
    ret->x = (v1->y)*(v2->z) - (v1->z)*(v2->y);
    ret->y = (v1->x)*(v2->z) - (v1->z)*(v2->x);
    ret->z = (v1->x)*(v2->y) - (v1->y)*(v2->x);
    return ret;
}

Vector* newVector(Point* p1, Point* p2) {
    Vector* ret = new Vector();
    ret->x = p2->x - p1->x;
    ret->y = p2->y - p1->y;
    ret->z = p2->z - p1->z;
    return ret;
}

//-------------------------------------------------------------------------------------------------
//NEEDS TESTING FOR ALL CODE UNDER HERE vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

float Matrix::getVal(int i, int j){
	return mat[i][j];
}

void Matrix::setVal(int i, int j, float val){
	mat[i][j] = val;
}

Matrix* compose(Matrix* m1, Matrix* m2){
	Matrix* cmatrix = new Matrix();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cmatrix->setVal(i, j, 0);
			for (int k = 0; k < 4; k++){
				cmatrix->setVal(i, j, cmatrix->getVal(i, j) + m1->getVal(i, k)*m2->getVal(k, j));
			}
		}
	}
	return cmatrix;
}

Matrix* Matrix::invert() {
	return NULL;
}

Matrix* Matrix::transpose() {
	Matrix* tmatrix = new Matrix();
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			tmatrix->setVal(i, j, this->getVal(j, i));
		}
	}
	return tmatrix;
}

void Matrix::setMatrix(float nmat[4][4]){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			mat[i][j] = nmat[i][j];
		}
	}
}

//REALLY REALLY NEEDS TESTING
//left-multiplies a transformation matrix created using tx, ty, and tz to "this"
Matrix* makeTransl(float tx, float ty, float tz){
	Matrix* transMatrix = new Matrix();
	transMatrix->setVal(3, 0, tx);
	transMatrix->setVal(3, 1, ty);
	transMatrix->setVal(3, 2, tz);

	//transMatrix->setMatrix(nmat);
	return transMatrix;
}

Matrix* makeScale(float sx, float sy, float sz){
	Matrix* scaleMatrix = new Matrix();

	scaleMatrix->setVal(0, 0, sx);
	scaleMatrix->setVal(1, 1, sy);
	scaleMatrix->setVal(2, 2, sz);

	//scaleMatrix->setMatrix(nmat);
	return scaleMatrix;
}

Matrix* makeRot(float rx, float ry, float rz){
	Matrix* rotMatrix = new Matrix();
	float norm = pow(pow(rx, 2) + pow(ry, 2) + pow(rz, 2), 0.5);
	float r_31 = rx / norm;
	float r_32 = ry / norm;
	float r_33 = rz / norm;
	return rotMatrix;
}


float determinant(Matrix* m){
	float f1 = m->getVal(1, 1) * m->getVal(2, 2) * m->getVal(3, 3) * m->getVal(4, 4);
	float f2 = m->getVal(1, 1) + m->getVal(2, 3) + m->getVal(3, 4) + m->getVal(4, 2);
	float f3 = m->getVal(1, 1) + m->getVal(2, 4) + m->getVal(3, 2) + m->getVal(4, 3);
	float r1 = f1 + f2 + f3;

	f1 = m->getVal(1, 2) + m->getVal(2, 1) + m->getVal(3, 4) + m->getVal(4, 3);
	f2 = m->getVal(1, 2) + m->getVal(2, 3) + m->getVal(3, 1) + m->getVal(4, 4);
	f3 = m->getVal(1, 2) * m->getVal(2, 4) * m->getVal(3, 3) * m->getVal(4, 1);
	float r2 = f1 + f2 + f3;

	f1 = m->getVal(1, 3) + m->getVal(2, 1) + m->getVal(3, 2) + m->getVal(4, 4);
	f2 = m->getVal(1, 3) + m->getVal(2, 2) + m->getVal(3, 4) + m->getVal(4, 1);
	f3 = m->getVal(1, 3) * m->getVal(2, 4) * m->getVal(3, 1) * m->getVal(4, 2);
	float r3 = f1 + f2 + f3;

	f1 = m->getVal(1, 4) + m->getVal(2, 1) + m->getVal(3, 3) + m->getVal(4, 2);
	f2 = m->getVal(1, 4) + m->getVal(2, 2) + m->getVal(3, 1) + m->getVal(4, 3);
	f3 = m->getVal(1, 4) * m->getVal(2, 3) * m->getVal(3, 2) * m->getVal(4, 1);
	float r4 = f1 + f2 + f3;

	f1 = m->getVal(1, 1) + m->getVal(2, 2) + m->getVal(3, 4) + m->getVal(4, 3);
	f2 = m->getVal(1, 1) + m->getVal(2, 3) + m->getVal(3, 2) + m->getVal(4, 4);
	f3 = m->getVal(1, 1) * m->getVal(2, 4) * m->getVal(3, 3) * m->getVal(4, 2);
	float r5 = f1 + f2 + f3;

	f1 = m->getVal(1, 2) + m->getVal(2, 1) + m->getVal(3, 3) + m->getVal(4, 4);
	f2 = m->getVal(1, 2) + m->getVal(2, 3) + m->getVal(3, 4) + m->getVal(4, 1);
	f3 = m->getVal(1, 2) * m->getVal(2, 4) * m->getVal(3, 1) * m->getVal(4, 3);
	float r6 = f1 + f2 + f3;

	f1 = m->getVal(1, 3) + m->getVal(2, 1) + m->getVal(3, 4) + m->getVal(4, 2);
	f2 = m->getVal(1, 3) + m->getVal(2, 2) + m->getVal(3, 1) + m->getVal(4, 4);
	f3 = m->getVal(1, 3) * m->getVal(2, 4) * m->getVal(3, 2) * m->getVal(4, 1);
	float r7 = f1 + f2 + f3;

	f1 = m->getVal(1, 4) + m->getVal(2, 1) + m->getVal(3, 2) + m->getVal(4, 3);
	f2 = m->getVal(1, 4) + m->getVal(2, 2) + m->getVal(3, 3) + m->getVal(4, 1);
	f3 = m->getVal(1, 4) * m->getVal(2, 3) * m->getVal(3, 1) * m->getVal(4, 2);
	float r8 = f1 + f2 + f3;

	return r1 + r2 + r3 + r4 - r5 - r6 - r7 - r8;
}