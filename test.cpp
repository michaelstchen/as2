#include <gtest/gtest.h>
#include "linalg.h"
#include "property.h"
#include "light.h"
#include "geomobj.h"

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
    ASSERT_FLOAT_EQ(6.0, d->y);
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



int main(int argc, char **argv) {
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
