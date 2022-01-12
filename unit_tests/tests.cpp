#include <gtest/gtest.h>
#include "../src/ThreadPool.hpp"

int pow(int x) {
    return x*x;
}

TEST(ThreadSafeQueueTests, PushPopEmpty) {
    ASSERT_EQ(4, pow(2));
}

TEST(ThreadSafeQueueTests, COsinnego) {
    ASSERT_EQ(0, pow(0));
}

 int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
 }
