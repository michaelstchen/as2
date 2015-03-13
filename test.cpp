#include <gtest/gtest.h>
#include "linalg.h"

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
    Vector* d = dot(v1, v2);
    ASSERT_FLOAT_EQ(0.0, d->x);
    ASSERT_FLOAT_EQ(10.0, d->y);
    ASSERT_FLOAT_EQ(18.0, d->z);
    delete v1; delete v2; delete d;    
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


int main(int argc, char **argv) {
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
