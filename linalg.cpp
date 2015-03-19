#include "linalg.h"
#include <cmath>
#include <cstddef>
#include <stdio.h>

Point::Point(float xIn, float yIn, float zIn) {
    x = xIn; y = yIn; z = zIn;
}

void Point::print(){
	printf("x: %f",this->x);
	printf("\ny: %f", this->y);
	printf("\nz: %f", this->z);
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
    ret->y = (v1->z)*(v2->x) - (v1->x)*(v2->z);
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
	return mat[i - 1][j - 1];
}

void Matrix::setVal(int i, int j, float val){
	mat[i - 1][j - 1] = val;
}

Matrix* Matrix::scale(float n){
	Matrix* smatrix = new Matrix();
	for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                smatrix->setVal(i + 1, j + 1, this->getVal(i + 1, j + 1)*n);
            }
	}
	return smatrix;
}

Matrix* Matrix::copy() {
    Matrix* ret = new Matrix();
    for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
                    ret->setVal(i + 1, j + 1, this->getVal(i + 1, j + 1));
		}
	}
    return ret;
}

void Matrix::print(){
	printf("row 1: %f %f %f %f" ,this->getVal(1,1), this->getVal(1,2),this->getVal(1,3),this->getVal(1,4));
	printf("\nrow 2: %f %f %f %f" ,this->getVal(2,1), this->getVal(2,2),this->getVal(2,3),this->getVal(2,4));
	printf("\nrow 3: %f %f %f %f" ,this->getVal(3,1), this->getVal(3,2),this->getVal(3,3),this->getVal(3,4));
	printf("\nrow 4: %f %f %f %f\n" ,this->getVal(4,1), this->getVal(4,2),this->getVal(4,3),this->getVal(4,4));
}

Matrix* compose(Matrix* m1, Matrix* m2){
	Matrix* cmatrix = new Matrix();

	float mat[4][4] = 
        {
           	{0,0,0,0},
          	{0,0,0,0},
        	{0,0,0,0},
        	{0,0,0,0}
        };
    cmatrix->setMatrix(mat);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//cmatrix->setVal(i, j, 0);
			for (int k = 0; k < 4; k++){
				cmatrix->setVal(i + 1, j + 1, 
                                                cmatrix->getVal(i + 1, j + 1)
                                                + m1->getVal(i + 1, k + 1)*m2->getVal(k + 1, j + 1));
			}
		}
	}
	return cmatrix;
}

Matrix* Matrix::invert() {
	//B_11
	float b_11_1 = this->getVal(2,2) * this->getVal(3,3) * this->getVal(4,4);
	float b_11_2 = this->getVal(2,3) * this->getVal(3,4) * this->getVal(4,2);
	float b_11_3 = this->getVal(2,4) * this->getVal(3,2) * this->getVal(4,3);
	float b_11_4 = this->getVal(2,2) * this->getVal(3,4) * this->getVal(4,3);
	float b_11_5 = this->getVal(2,3) * this->getVal(3,2) * this->getVal(4,4);
	float b_11_6 = this->getVal(2,4) * this->getVal(3,3) * this->getVal(4,2);
	float b_11 = b_11_1 + b_11_2 + b_11_3 - b_11_4 - b_11_5 - b_11_6;

	//B_12
	float b_12_1 = this->getVal(1,2) * this->getVal(3,4) * this->getVal(4,3);
	float b_12_2 = this->getVal(1,3) * this->getVal(3,2) * this->getVal(4,4);
	float b_12_3 = this->getVal(1,4) * this->getVal(3,3) * this->getVal(4,2);
	float b_12_4 = this->getVal(1,2) * this->getVal(3,3) * this->getVal(4,4);
	float b_12_5 = this->getVal(1,3) * this->getVal(3,4) * this->getVal(4,2);
	float b_12_6 = this->getVal(1,4) * this->getVal(3,2) * this->getVal(4,3);
	float b_12 = b_12_1 + b_12_2 + b_12_3 - b_12_4 - b_12_5 - b_12_6;

	//B_13
	float b_13_1 = this->getVal(1,2) * this->getVal(2,3) * this->getVal(4,4);
	float b_13_2 = this->getVal(1,3) * this->getVal(2,4) * this->getVal(4,2);
	float b_13_3 = this->getVal(1,4) * this->getVal(2,2) * this->getVal(4,3);
	float b_13_4 = this->getVal(1,2) * this->getVal(2,4) * this->getVal(4,3);
	float b_13_5 = this->getVal(1,3) * this->getVal(2,2) * this->getVal(4,4);
	float b_13_6 = this->getVal(1,4) * this->getVal(2,3) * this->getVal(4,2);
	float b_13 = b_13_1 + b_13_2 + b_13_3 - b_13_4 - b_13_5 - b_13_6;

	//B_14
	float b_14_1 = this->getVal(1,2) * this->getVal(2,4) * this->getVal(3,3);
	float b_14_2 = this->getVal(1,3) * this->getVal(2,2) * this->getVal(3,4);
	float b_14_3 = this->getVal(1,4) * this->getVal(2,3) * this->getVal(3,2);
	float b_14_4 = this->getVal(1,2) * this->getVal(2,3) * this->getVal(3,4);
	float b_14_5 = this->getVal(1,3) * this->getVal(2,4) * this->getVal(3,2);
	float b_14_6 = this->getVal(1,4) * this->getVal(2,2) * this->getVal(3,3);
	float b_14 = b_14_1 + b_14_2 + b_14_3 - b_14_4 - b_14_5 - b_14_6;

	//B_21
	float b_21_1 = this->getVal(2,1) * this->getVal(3,4) * this->getVal(4,3);
	float b_21_2 = this->getVal(2,3) * this->getVal(3,1) * this->getVal(4,4);
	float b_21_3 = this->getVal(2,4) * this->getVal(3,3) * this->getVal(4,1);
	float b_21_4 = this->getVal(2,1) * this->getVal(3,3) * this->getVal(4,4);
	float b_21_5 = this->getVal(2,3) * this->getVal(3,4) * this->getVal(4,1);
	float b_21_6 = this->getVal(2,4) * this->getVal(3,1) * this->getVal(4,3);
	float b_21 = b_21_1 + b_21_2 + b_21_3 - b_21_4 - b_21_5 - b_21_6;

	//B_22 
	float b_22_1 = this->getVal(1,1) * this->getVal(3,3) * this->getVal(4,4);
	float b_22_2 = this->getVal(1,3) * this->getVal(3,4) * this->getVal(4,1);
	float b_22_3 = this->getVal(1,4) * this->getVal(3,1) * this->getVal(4,3);
	float b_22_4 = this->getVal(1,1) * this->getVal(3,4) * this->getVal(4,3);
	float b_22_5 = this->getVal(1,3) * this->getVal(3,1) * this->getVal(4,4);
	float b_22_6 = this->getVal(1,4) * this->getVal(3,3) * this->getVal(4,1);
	float b_22 = b_22_1 + b_22_2 + b_22_3 - b_22_4 - b_22_5 - b_22_6;

	//B_23 
	float b_23_1 = this->getVal(1,1) * this->getVal(2,4) * this->getVal(4,3);
	float b_23_2 = this->getVal(1,3) * this->getVal(2,1) * this->getVal(4,4);
	float b_23_3 = this->getVal(1,4) * this->getVal(2,3) * this->getVal(4,1);
	float b_23_4 = this->getVal(1,1) * this->getVal(2,3) * this->getVal(4,4);
	float b_23_5 = this->getVal(1,3) * this->getVal(2,4) * this->getVal(4,1);
	float b_23_6 = this->getVal(1,4) * this->getVal(2,1) * this->getVal(4,3);
	float b_23 = b_23_1 + b_23_2 + b_23_3 - b_23_4 - b_23_5 - b_23_6;

	//B_24
	float b_24_1 = this->getVal(1,1) * this->getVal(2,3) * this->getVal(3,4);
	float b_24_2 = this->getVal(1,3) * this->getVal(2,4) * this->getVal(3,1);
	float b_24_3 = this->getVal(1,4) * this->getVal(2,1) * this->getVal(3,3);
	float b_24_4 = this->getVal(1,1) * this->getVal(2,4) * this->getVal(3,3);
	float b_24_5 = this->getVal(1,3) * this->getVal(2,1) * this->getVal(3,4);
	float b_24_6 = this->getVal(1,4) * this->getVal(2,3) * this->getVal(3,1);
	float b_24 = b_24_1 + b_24_2 + b_24_3 - b_24_4 - b_24_5 - b_24_6;

	//B_31
	float b_31_1 = this->getVal(2,1) * this->getVal(3,2) * this->getVal(4,4);
	float b_31_2 = this->getVal(2,2) * this->getVal(3,4) * this->getVal(4,1);
	float b_31_3 = this->getVal(2,4) * this->getVal(3,1) * this->getVal(4,2);
	float b_31_4 = this->getVal(2,1) * this->getVal(3,4) * this->getVal(4,2);
	float b_31_5 = this->getVal(2,2) * this->getVal(3,1) * this->getVal(4,4);
	float b_31_6 = this->getVal(2,4) * this->getVal(3,2) * this->getVal(4,1);
	float b_31 = b_31_1 + b_31_2 + b_31_3 - b_31_4 - b_31_5 - b_31_6;

	//B_32 
	float b_32_1 = this->getVal(1,1) * this->getVal(3,4) * this->getVal(4,2);
	float b_32_2 = this->getVal(1,2) * this->getVal(3,1) * this->getVal(4,4);
	float b_32_3 = this->getVal(1,4) * this->getVal(3,2) * this->getVal(4,1);
	float b_32_4 = this->getVal(1,1) * this->getVal(3,2) * this->getVal(4,4);
	float b_32_5 = this->getVal(1,2) * this->getVal(3,4) * this->getVal(4,1);
	float b_32_6 = this->getVal(1,4) * this->getVal(3,1) * this->getVal(4,2);
	float b_32 = b_32_1 + b_32_2 + b_32_3 - b_32_4 - b_32_5 - b_32_6;

	//B_33
	float b_33_1 = this->getVal(1,1) * this->getVal(2,2) * this->getVal(4,4);
	float b_33_2 = this->getVal(1,2) * this->getVal(2,4) * this->getVal(4,1);
	float b_33_3 = this->getVal(1,4) * this->getVal(2,1) * this->getVal(4,2);
	float b_33_4 = this->getVal(1,1) * this->getVal(2,4) * this->getVal(4,2);
	float b_33_5 = this->getVal(1,2) * this->getVal(2,1) * this->getVal(4,4);
	float b_33_6 = this->getVal(1,4) * this->getVal(2,2) * this->getVal(4,1);
	float b_33 = b_33_1 + b_33_2 + b_33_3 - b_33_4 - b_33_5 - b_33_6;

	//B_34
	float b_34_1 = this->getVal(1,1) * this->getVal(2,4) * this->getVal(3,2);
	float b_34_2 = this->getVal(1,2) * this->getVal(2,1) * this->getVal(3,4);
	float b_34_3 = this->getVal(1,4) * this->getVal(2,2) * this->getVal(3,1);
	float b_34_4 = this->getVal(1,1) * this->getVal(2,2) * this->getVal(3,4);
	float b_34_5 = this->getVal(1,2) * this->getVal(2,4) * this->getVal(3,1);
	float b_34_6 = this->getVal(1,4) * this->getVal(2,1) * this->getVal(3,2);
	float b_34 = b_34_1 + b_34_2 + b_34_3 - b_34_4 - b_34_5 - b_34_6;

	//B_41
	float b_41_1 = this->getVal(2,1) * this->getVal(3,3) * this->getVal(4,2);
	float b_41_2 = this->getVal(2,2) * this->getVal(3,1) * this->getVal(4,3);
	float b_41_3 = this->getVal(2,3) * this->getVal(3,2) * this->getVal(4,1);
	float b_41_4 = this->getVal(2,1) * this->getVal(3,2) * this->getVal(4,3);
	float b_41_5 = this->getVal(2,2) * this->getVal(3,3) * this->getVal(4,1);
	float b_41_6 = this->getVal(2,3) * this->getVal(3,1) * this->getVal(4,2);
	float b_41 = b_41_1 + b_41_2 + b_41_3 - b_41_4 - b_41_5 - b_41_6;

	//B_42
	float b_42_1 = this->getVal(1,1) * this->getVal(3,2) * this->getVal(4,3);
	float b_42_2 = this->getVal(1,2) * this->getVal(3,3) * this->getVal(4,1);
	float b_42_3 = this->getVal(1,3) * this->getVal(3,1) * this->getVal(4,2);
	float b_42_4 = this->getVal(1,1) * this->getVal(3,3) * this->getVal(4,2);
	float b_42_5 = this->getVal(1,2) * this->getVal(3,1) * this->getVal(4,3);
	float b_42_6 = this->getVal(1,3) * this->getVal(3,2) * this->getVal(4,1);
	float b_42 = b_42_1 + b_42_2 + b_42_3 - b_42_4 - b_42_5 - b_42_6;

	//B_43
	float b_43_1 = this->getVal(1,1) * this->getVal(2,3) * this->getVal(4,2);
	float b_43_2 = this->getVal(1,2) * this->getVal(2,1) * this->getVal(4,3);
	float b_43_3 = this->getVal(1,3) * this->getVal(2,2) * this->getVal(4,1);
	float b_43_4 = this->getVal(1,1) * this->getVal(2,2) * this->getVal(4,3);
	float b_43_5 = this->getVal(1,2) * this->getVal(2,3) * this->getVal(4,1);
	float b_43_6 = this->getVal(1,3) * this->getVal(2,1) * this->getVal(4,2);
	float b_43 = b_43_1 + b_43_2 + b_43_3 - b_43_4 - b_43_5 - b_43_6;

	//B_44
	float b_44_1 = this->getVal(1,1) * this->getVal(2,2) * this->getVal(3,3);
	float b_44_2 = this->getVal(1,2) * this->getVal(2,3) * this->getVal(3,1);
	float b_44_3 = this->getVal(1,3) * this->getVal(2,1) * this->getVal(3,2);
	float b_44_4 = this->getVal(1,1) * this->getVal(2,3) * this->getVal(3,2);
	float b_44_5 = this->getVal(1,2) * this->getVal(2,1) * this->getVal(3,3);
	float b_44_6 = this->getVal(1,3) * this->getVal(2,2) * this->getVal(3,1);
	float b_44 = b_44_1 + b_44_2 + b_44_3 - b_44_4 - b_44_5 - b_44_6;

	float m2[4][4] = {
        {b_11,b_12,b_13,b_14},
        {b_21,b_22,b_23,b_24},
        {b_31,b_32,b_33,b_34},
        {b_41,b_42,b_43,b_44}
    };

	Matrix* imatrix = new Matrix();
	imatrix->setMatrix(m2);
	imatrix = imatrix->scale(determinant(this));
	return imatrix;

}

Matrix* Matrix::transpose() {
	Matrix* tmatrix = new Matrix();
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			tmatrix->setVal(i + 1, j + 1, this->getVal(j + 1, i + 1));
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

Vector* mLeftV(Matrix* m, Vector* v){
	float s1 = m->getVal(1, 1) * v->x + m->getVal(1,2)*v->y
            + m->getVal(1,3)*v->z;
	float s2 = m->getVal(2, 1) * v->x + m->getVal(2,2)*v->y
            + m->getVal(2,3)*v->z;
	float s3 = m->getVal(3, 1) * v->x + m->getVal(3,2)*v->y
            + m->getVal(3,3)*v->z;
	return new Vector(s1, s2, s3);
}

Point* mLeftP(Matrix* m, Point* v){
	float s1 = m->getVal(1, 1) * v->x + m->getVal(1,2)*v->y + m->getVal(1,3)*v->z + m->getVal(1,4);
	float s2 = m->getVal(2, 1) * v->x + m->getVal(2,2)*v->y + m->getVal(2,3)*v->z + m->getVal(2,4);
	float s3 = m->getVal(3, 1) * v->x + m->getVal(3,2)*v->y + m->getVal(3,3)*v->z + m->getVal(3,4);
        if (m->getVal(4,4) != 1.0) {
            float normalize = 1.0 / m->getVal(4,4);
            s1 = s1 * normalize;
            s2 = s2 * normalize;
            s3 = s3 * normalize;
        }
	return new Point(s1, s2, s3);
}

//REALLY REALLY NEEDS TESTING
Matrix* makeTransl(float tx, float ty, float tz){
	Matrix* transMatrix = new Matrix();
	transMatrix->setVal(1, 4, tx);
	transMatrix->setVal(2, 4, ty);
	transMatrix->setVal(3, 4, tz);

	return transMatrix;
}

Matrix* makeScale(float sx, float sy, float sz){
	Matrix* scaleMatrix = new Matrix();

	scaleMatrix->setVal(1, 1, sx);
	scaleMatrix->setVal(2, 2, sy);
	scaleMatrix->setVal(3, 3, sz);

	//scaleMatrix->setMatrix(nmat);
	return scaleMatrix;
}

Matrix* makeRot(float rx, float ry, float rz){
	Matrix* rotMatrix = new Matrix();
	float norm = pow(pow(rx, 2) + pow(ry, 2) + pow(rz, 2), 0.5);
	Vector* r = new Vector(rx, ry, rz);
	r->normalize();
	Matrix* rr = new Matrix();
	float rrt[4][4] = 
        {
            {r->x*r->x , r->x*r->y , r->x*r->z , 0},
            {r->x*r->y , r->y*r->y , r->y*r->z , 0},
            {r->x*r->z , r->y*r->z , r->z*r->z , 0},
            {0,0,0,1}
        };
    rr->setMatrix(rrt);
    Matrix* rx2 = new Matrix();
    float rxa[4][4] =
        {
            {0,-r->z,-r->y,0},
            {r->z,0,-r->x,0},
            {-r->y,r->x,0,0},
            {0,0,0,1}
        };
    rx2->setMatrix(rxa);
    norm = (norm/180)*3.1415927;
    rotMatrix = matSum(rr, rx2->scale(sin(norm)));
    Matrix* rxrx = new Matrix();
    rxrx = compose(rx2, rx2);
    rxrx = rxrx->scale(-cos(norm));

    rotMatrix = matSum(rotMatrix, rxrx);

    delete r; delete rr; delete rxrx;
    return rotMatrix;
}


float determinant(Matrix* m){
	float f1 = m->getVal(1, 1) * m->getVal(2, 2) * m->getVal(3, 3) * m->getVal(4, 4);
	float f2 = m->getVal(1, 1) * m->getVal(2, 3) * m->getVal(3, 4) * m->getVal(4, 2);
	float f3 = m->getVal(1, 1) * m->getVal(2, 4) * m->getVal(3, 2) * m->getVal(4, 3);
	float r1 = f1 + f2 + f3;

	f1 = m->getVal(1, 2) * m->getVal(2, 1) * m->getVal(3, 4) * m->getVal(4, 3);
	f2 = m->getVal(1, 2) * m->getVal(2, 3) * m->getVal(3, 1) * m->getVal(4, 4);
	f3 = m->getVal(1, 2) * m->getVal(2, 4) * m->getVal(3, 3) * m->getVal(4, 1);
	float r2 = f1 + f2 + f3;

	f1 = m->getVal(1, 3) * m->getVal(2, 1) * m->getVal(3, 2) * m->getVal(4, 4);
	f2 = m->getVal(1, 3) * m->getVal(2, 2) * m->getVal(3, 4) * m->getVal(4, 1);
	f3 = m->getVal(1, 3) * m->getVal(2, 4) * m->getVal(3, 1) * m->getVal(4, 2);
	float r3 = f1 + f2 + f3;

	f1 = m->getVal(1, 4) * m->getVal(2, 1) * m->getVal(3, 3) * m->getVal(4, 2);
	f2 = m->getVal(1, 4) * m->getVal(2, 2) * m->getVal(3, 1) * m->getVal(4, 3);
	f3 = m->getVal(1, 4) * m->getVal(2, 3) * m->getVal(3, 2) * m->getVal(4, 1);
	float r4 = f1 + f2 + f3;

	f1 = m->getVal(1, 1) * m->getVal(2, 2) * m->getVal(3, 4) * m->getVal(4, 3);
	f2 = m->getVal(1, 1) * m->getVal(2, 3) * m->getVal(3, 2) * m->getVal(4, 4);
	f3 = m->getVal(1, 1) * m->getVal(2, 4) * m->getVal(3, 3) * m->getVal(4, 2);
	float r5 = f1 + f2 + f3;

	f1 = m->getVal(1, 2) * m->getVal(2, 1) * m->getVal(3, 3) * m->getVal(4, 4);
	f2 = m->getVal(1, 2) * m->getVal(2, 3) * m->getVal(3, 4) * m->getVal(4, 1);
	f3 = m->getVal(1, 2) * m->getVal(2, 4) * m->getVal(3, 1) * m->getVal(4, 3);
	float r6 = f1 + f2 + f3;

	f1 = m->getVal(1, 3) * m->getVal(2, 1) * m->getVal(3, 4) * m->getVal(4, 2);
	f2 = m->getVal(1, 3) * m->getVal(2, 2) * m->getVal(3, 1) * m->getVal(4, 4);
	f3 = m->getVal(1, 3) * m->getVal(2, 4) * m->getVal(3, 2) * m->getVal(4, 1);
	float r7 = f1 + f2 + f3;

	f1 = m->getVal(1, 4) * m->getVal(2, 1) * m->getVal(3, 2) * m->getVal(4, 3);
	f2 = m->getVal(1, 4) * m->getVal(2, 2) * m->getVal(3, 3) * m->getVal(4, 1);
	f3 = m->getVal(1, 4) * m->getVal(2, 3) * m->getVal(3, 1) * m->getVal(4, 2);
	float r8 = f1 + f2 + f3;

	return r1 + r2 + r3 + r4 - r5 - r6 - r7 - r8;
}

Matrix* matSum(Matrix* a, Matrix* b){
	Matrix* sum = new Matrix();
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			sum->setVal(i+1,j+1, a->getVal(i+1,j+1) + b->getVal(i+1,j+1));
		}
	}
	return sum;
}

