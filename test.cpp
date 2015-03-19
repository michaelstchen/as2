#include <gtest/gtest.h>
#include <stdio.h>
#include "linalg.h"
#include "property.h"
#include "light.h"
#include "geomobj.h"
#include "raytracer.h"
#include "ray.h"
#include <cmath>

/* Vector test suite */

TEST(VectorTest, VectorAddition) {
    Vector* v1 = new Vector(1.0, 2.0, 3.0);
    Vector* v2 = new Vector(0.0, 5.0, 6.0);
    Vector* sum = add(v1, v2);
    ASSERT_FLOAT_EQ(1.0, sum->x);
    ASSERT_FLOAT_EQ(7.0, sum->y);
    ASSERT_FLOAT_EQ(9.0, sum->z);
    delete v1; delete v2; delete sum;
}

TEST(VectorTest, VectorNormalize) {
    Vector* v1 = new Vector(1.0, 2.0, 3.0);
    v1->normalize();
    ASSERT_FLOAT_EQ(0.26726124, v1->x);
    ASSERT_FLOAT_EQ(0.53452248, v1->y);
    ASSERT_FLOAT_EQ(0.80178373, v1->z);
    delete v1;
}

TEST(VectorTest, VectorDot) {
    Vector* v1 = new Vector(1.0, 2.0, 3.0);
    Vector* v2 = new Vector(0.0, 5.0, 6.0);
    float d = dot(v1, v2);
    ASSERT_FLOAT_EQ(28.0, d);
    delete v1; delete v2;
}

TEST(VectorTest, VectorCross) {
    Vector* v1 = new Vector(1.0, 2.0, 3.0);
    Vector* v2 = new Vector(0.0, 5.0, 6.0);
    Vector* d = cross(v1, v2);
    ASSERT_FLOAT_EQ(-3.0, d->x);
    ASSERT_FLOAT_EQ(-6.0, d->y);
    ASSERT_FLOAT_EQ(5.0, d->z);
    delete v1; delete v2; delete d;    
}

TEST(VectorTest, VectorFromPoint) {
    Point* p1 = new Point(1.1, 0.23, 8.67);
    Point* p2 = new Point(3.23, 5.76, 1.08);
    Vector* r = newVector(p1, p2);
    ASSERT_FLOAT_EQ(2.13, r->x);
    ASSERT_FLOAT_EQ(5.53, r->y);
    ASSERT_FLOAT_EQ(-7.59, r->z);
    delete p1; delete p2; delete r;
}

/* Color test suite */
TEST(ColorTest, ColorAdd) {
    Color* c1 = new Color(0.2, 0.3, 0.4);
    Color* c2 = new Color(0.0, 0.1, 0.15);
    c1->add(c2);
    ASSERT_FLOAT_EQ(0.2, c1->r);
    ASSERT_FLOAT_EQ(0.4, c1->g);
    ASSERT_FLOAT_EQ(0.55, c1->b);
    delete c1; delete c2;
}

TEST(ColorTest, ColorMult) {
    Color* c1 = new Color(0.2, 0.3, 0.4);
    c1->mult(0.11);
    ASSERT_FLOAT_EQ(0.022, c1->r);
    ASSERT_FLOAT_EQ(0.033, c1->g);
    ASSERT_FLOAT_EQ(0.044, c1->b);
    delete c1;
}

TEST(ColorTest, Color8Bit) {
    Color* c1 = new Color(0.2, 0.3, 0.35);
    ASSERT_EQ(51, c1->bit8R());
    ASSERT_EQ(76, c1->bit8G()); //actual is 76.5
    ASSERT_EQ(89, c1->bit8B()); //actual is 89.25
    delete c1;
}

/* Shape Intersection Tests */

TEST(IntersectionTest, SphereTest0) {
    Matrix* m = new Matrix();
    Point* c = new Point(0.0, 0.0, 0.0);
    Sphere* s = new Sphere(c, 1, NULL, m, NULL);

    Point* p = new Point(2, 0, 0);
    Vector* d = new Vector(0, 1, 0);
    EyeRay* ray = new EyeRay(p, d);

    Point* i = NULL;
    float t = s->intersect(ray, &i);

    ASSERT_EQ(NULL, i);
    
    delete c; delete s; delete p; delete d; delete ray; delete i;
}


TEST(IntersectionTest, SphereTest1) {
    Matrix* m = new Matrix();
    Point* c = new Point(0.0, 0.0, 0.0);
    Sphere* s = new Sphere(c, 1, NULL, m, NULL);
    
    Point* p = new Point(2, 1, 0);
    Vector* d = new Vector(-1, 0, 0);
    EyeRay* ray = new EyeRay(p, d);

    Point* i = NULL;
    float t = s->intersect(ray, &i);

    ASSERT_FLOAT_EQ(0.0, i->x);
    ASSERT_FLOAT_EQ(1.0, i->y);
    ASSERT_FLOAT_EQ(0.0, i->z);
    
    delete c; delete s; delete p; delete d; delete ray; delete i;
}

TEST(IntersectionTest, SphereTest2) {
    Matrix* m = new Matrix();
    Point* c = new Point(0.0, 0.0, 0.0);
    Sphere* s = new Sphere(c, 1, NULL, m, NULL);
    
    Point* p = new Point(2, 0, 0);
    Vector* d = new Vector(-1, 0, 0);
    EyeRay* ray = new EyeRay(p, d);

    Point* i = NULL;
    float t = s->intersect(ray, &i);

    ASSERT_FLOAT_EQ(1.0, i->x);
    ASSERT_FLOAT_EQ(0.0, i->y);
    ASSERT_FLOAT_EQ(0.0, i->z);
    
    delete c; delete s; delete p; delete d; delete ray; delete i;
}

/* Testing getPixelPos() for ImgPlane */
TEST(ImgPlaneTest, getPixelPos) {
    Point* ll = new Point(0.0, 0.0, 0.0);
    Point* lr = new Point(5.0, 0.0, 0.0);
    Point* ul = new Point(0.0, 7.0, 0.0);
    Point* ur = new Point(5.0, 7.0, 0.0);
    ImgPlane* img = new ImgPlane(ll, lr, ul, ur, 5, 7);

    Point* t0 = img->getPixelPos(0, 6);
    ASSERT_FLOAT_EQ(0.5, t0->x);
    ASSERT_FLOAT_EQ(0.5, t0->y);
    ASSERT_FLOAT_EQ(0.0, t0->z);
    delete t0;

    Point* t1 = img->getPixelPos(4, 6);
    ASSERT_FLOAT_EQ(4.5, t1->x);
    ASSERT_FLOAT_EQ(0.5, t1->y);
    ASSERT_FLOAT_EQ(0.0, t1->z);
    delete t1;

    Point* t2 = img->getPixelPos(0, 0);
    ASSERT_FLOAT_EQ(0.5, t2->x);
    ASSERT_FLOAT_EQ(6.5, t2->y);
    ASSERT_FLOAT_EQ(0.0, t2->z);
    delete t2;
    
    Point* t3 = img->getPixelPos(4, 0);
    ASSERT_FLOAT_EQ(4.5, t3->x);
    ASSERT_FLOAT_EQ(6.5, t3->y);
    ASSERT_FLOAT_EQ(0.0, t3->z);
    delete t3;

    delete ul; delete ll; delete ur; delete lr;
    delete img;

}

TEST(MatrixTest, determinant){
    Matrix* m1 = new Matrix();
    float nm1[4][4] = {
        {1,2,3,4},
        {2,14,7,8},
        {9,10,11,12},
        {13,14,15,13}
    };
    m1->setMatrix(nm1);
    float det = determinant(m1);
    ASSERT_FLOAT_EQ(det, 456);
    delete m1;
}

TEST(MatrixTest, getVal){
    Matrix* m1 = new Matrix();
    float nm1[4][4] = {
        {1,2,3,4},
        {2,14,7,8},
        {9,10,11,12},
        {13,14,15,13}
    };
    m1->setMatrix(nm1);
    float f1 = m1->getVal(2,1);
    ASSERT_FLOAT_EQ(f1, 2);
    delete m1;
}

TEST(MatrixTest, transpose){
    Matrix* m1 = new Matrix();
    float nm1[4][4] = {
        {1,2,3,4},
        {2,14,7,8},
        {9,10,11,12},
        {13,14,15,13}
    };
    m1->setMatrix(nm1);

    Matrix* m2 = new Matrix();
    float nm2[4][4] = {
        {1,2,9,13},
        {2,14,10,14},
        {3,7,11,15},
        {4,8,12,13}
    };
    m2->setMatrix(nm2);

    Matrix* m3 = m1->transpose();

    float f1 = m2->getVal(2,1);
    float f2 = m3->getVal(2,1);
    ASSERT_FLOAT_EQ(f1, f2);

    f1 = m2->getVal(3,1);
    f2 = m3->getVal(3,1);
    ASSERT_FLOAT_EQ(f1, f2);


    f1 = m2->getVal(4,1);
    f2 = m3->getVal(4,1);
    ASSERT_FLOAT_EQ(f1, f2);
    delete m1; delete m2; delete m3;
}

TEST(MatrixTest, translation) {
    Matrix* mt = makeTransl(2.1, 3, 4);

    ASSERT_FLOAT_EQ(2.1, mt->getVal(1, 4));
    ASSERT_FLOAT_EQ(3, mt->getVal(2, 4));
    ASSERT_FLOAT_EQ(4, mt->getVal(3, 4));

    Vector* v = new Vector(2.5, 3, -5);
    Vector* vt = mLeftV(mt, v);

    ASSERT_FLOAT_EQ(2.5, vt->x); 
    ASSERT_FLOAT_EQ(3, vt->y); 
    ASSERT_FLOAT_EQ(-5, vt->z); 

    Point* p = new Point(2.5, 3, -5);
    Point* pt = mLeftP(mt, p);

    ASSERT_FLOAT_EQ(4.6, pt->x); 
    ASSERT_FLOAT_EQ(6, pt->y); 
    ASSERT_FLOAT_EQ(-1, pt->z); 

    delete mt; delete v; delete vt;
}

TEST(MatrixTest, scale) {
    Matrix* mt = makeScale(2.1, 3.2, -1.5);

    ASSERT_FLOAT_EQ(2.1, mt->getVal(1,1));
    ASSERT_FLOAT_EQ(3.2, mt->getVal(2,2));
    ASSERT_FLOAT_EQ(-1.5, mt->getVal(3,3));

    Vector* v = new Vector(2.5, 3, -5);
    Vector* vt = mLeftV(mt, v);

    ASSERT_FLOAT_EQ(5.25, vt->x);
    ASSERT_FLOAT_EQ(9.6, vt->y);
    ASSERT_FLOAT_EQ(7.5, vt->z);
}

TEST(MatrixTest, rotation) {
    Matrix* mt = makeRot(0, 30, 0);

    ASSERT_FLOAT_EQ(0.86602539, mt->getVal(1,1));
    ASSERT_FLOAT_EQ(0, mt->getVal(1,2));
    ASSERT_FLOAT_EQ(0.5, mt->getVal(1,3));
    ASSERT_FLOAT_EQ(0, mt->getVal(1,4));

    ASSERT_FLOAT_EQ(0, mt->getVal(2,1));
    ASSERT_FLOAT_EQ(1.0, mt->getVal(2,2));
    ASSERT_FLOAT_EQ(0, mt->getVal(2,3));
    ASSERT_FLOAT_EQ(0, mt->getVal(2,4));

    ASSERT_FLOAT_EQ(-0.5, mt->getVal(3,1));
    ASSERT_FLOAT_EQ(0, mt->getVal(3,2));
    ASSERT_FLOAT_EQ(0.86602539, mt->getVal(3,3));
    ASSERT_FLOAT_EQ(0, mt->getVal(3,4));

    ASSERT_FLOAT_EQ(0, mt->getVal(4,1));
    ASSERT_FLOAT_EQ(0, mt->getVal(4,2));
    ASSERT_FLOAT_EQ(0, mt->getVal(4,3));
    ASSERT_FLOAT_EQ(1, mt->getVal(4,4));

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
