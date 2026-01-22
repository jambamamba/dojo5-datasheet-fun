
#include "gtest/gtest.h"
#include "startup.h"
// #include "SMC_40CR.h"

TEST(SMC_40CR1, validateTheSystemClockSpeed) {
    ASSERT_TRUE(validateTheSystemClockSpeed(SYS_CLOCK_SPEED_160M));
}

TEST(SMC_40CR2, validateTheSystemClockSpeed) {
    ASSERT_EQ(1<<7, validateBusClockDivider(2));
    ASSERT_EQ(1<<8, validateBusClockDivider(4));
    ASSERT_EQ(0, validateBusClockDivider(0));
    ASSERT_EQ(0, validateBusClockDivider(999));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}