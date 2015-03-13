#include <gtest/gtest.h>
#include "linalg.h"

TEST(VectorTest, VectorAddition) {
    Vector* v1 = new Vector(1.0, 2.0, 3.0);
    Vector* v2 = new Vector(0.0, 5.0, 6.0);
    Vector* sum = add(v1, v2);
    ASSERT_EQ(1.0, sum->x);
    ASSERT_EQ(7.0, sum->y);
    ASSERT_EQ(9.0, sum->z);
    delete v1;
    delete v2;
    delete sum;
}

TEST(VectorTest, VectorNormalize) {
    Vector* v1 = new Vector(1.0, 2.0, 3.0);
    v1->normalize();
    ASSERT_FLOAT_EQ(0.26726124, v1->x);
    ASSERT_FLOAT_EQ(0.53452248, v1->y);
    ASSERT_FLOAT_EQ(0.80178373, v1->z);
    delete v1;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
