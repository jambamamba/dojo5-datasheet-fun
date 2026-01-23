#include <fff/fff.h>
#include <gtest/gtest.h>
#include "tests/common.h"
#include "startup.h"

DEFINE_FFF_GLOBALS; //Fake Func:tion Framework: Here we check if all the inner functions of SetSystemAndBusClockConfig get called

FAKE_VALUE_FUNC(int, cleanup, SMC_40CR_ERROR);
FAKE_VALUE_FUNC(bool, validateSystemClockSpeed, System_Clock_Speeds_t);
FAKE_VALUE_FUNC(bool, validateBusClockDivider, unsigned int);
FAKE_VALUE_FUNC(bool, switchDefaultClock);
FAKE_VALUE_FUNC(bool, configurePLL, System_Clock_Speeds_t);
FAKE_VALUE_FUNC(bool, turnOnPLL);
FAKE_VOID_FUNC(selectSystemClockDivider, System_Clock_Speeds_t);
FAKE_VOID_FUNC(selectBusClockDivider, unsigned int);
FAKE_VALUE_FUNC(bool, switchClock, bool);
FAKE_VALUE_FUNC(bool, isDefaultClock);
FAKE_VALUE_FUNC(bool, checkRCCUnlocked);
FAKE_VALUE_FUNC(bool, unlockRCC);
FAKE_VOID_FUNC(lockRCC);

TEST(SMC_40CR1, SetSystemAndBusClockConfig) {

    validateSystemClockSpeed_fake.return_val = true;
    validateBusClockDivider_fake.return_val = true;
    unlockRCC_fake.return_val = true;
    isDefaultClock_fake.return_val = false;
    switchDefaultClock_fake.return_val = true;
    configurePLL_fake.return_val = true;
    turnOnPLL_fake.return_val = true;
    switchClock_fake.return_val = true;

    System_Clock_Speeds_t sysClockSpeed = SYS_CLOCK_SPEED_160M;
    unsigned int busClockDivider = 2;
    bool isHsiClock = true;
    SetSystemAndBusClockConfig(sysClockSpeed, busClockDivider, isHsiClock);

    ASSERT_EQ(validateSystemClockSpeed_fake.call_count, 1);
    ASSERT_EQ(validateBusClockDivider_fake.call_count, 1);
    ASSERT_EQ(unlockRCC_fake.call_count, 1);
    ASSERT_EQ(isDefaultClock_fake.call_count, 1);
    ASSERT_EQ(switchDefaultClock_fake.call_count, 1);
    ASSERT_EQ(configurePLL_fake.call_count, 1);
    ASSERT_EQ(turnOnPLL_fake.call_count, 1);
    ASSERT_EQ(selectSystemClockDivider_fake.call_count, 1);
    ASSERT_EQ(selectBusClockDivider_fake.call_count, 1);
    ASSERT_EQ(switchClock_fake.call_count, 1);
    ASSERT_EQ(lockRCC_fake.call_count, 1);
}
