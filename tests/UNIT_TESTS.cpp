#include <gtest/gtest.h>
#include "tests/common.h"
#include "startup.h"


TEST(SMC_40CR1, validateSystemClockSpeed) {
  ASSERT_FALSE(validateSystemClockSpeed(static_cast<System_Clock_Speeds_t>(SYS_CLOCK_SPEED_UNDEFINED-1)));
  ASSERT_FALSE(validateSystemClockSpeed(SYS_CLOCK_SPEED_UNDEFINED));
  ASSERT_TRUE(validateSystemClockSpeed(SYS_CLOCK_SPEED_160M));
  ASSERT_TRUE(validateSystemClockSpeed(SYS_CLOCK_SPEED_80M));
  ASSERT_TRUE(validateSystemClockSpeed(SYS_CLOCK_SPEED_40M));
  ASSERT_TRUE(validateSystemClockSpeed(SYS_CLOCK_SPEED_20M));
  ASSERT_TRUE(validateSystemClockSpeed(SYS_CLOCK_SPEED_10M));
  ASSERT_TRUE(validateSystemClockSpeed(SYS_CLOCK_SPEED_5M));
  ASSERT_TRUE(validateSystemClockSpeed(SYS_CLOCK_SPEED_2_5M));
  ASSERT_TRUE(validateSystemClockSpeed(SYS_CLOCK_SPEED_1_25M));
  ASSERT_FALSE(validateSystemClockSpeed(static_cast<System_Clock_Speeds_t>(SYS_CLOCK_SPEED_MAX_ENUM_VAL+1)));
}

TEST(SMC_40CR2, validateBusClockDivider) {
    ASSERT_TRUE(validateBusClockDivider(2));
    ASSERT_TRUE(validateBusClockDivider(4));
    ASSERT_FALSE(validateBusClockDivider(0));
    ASSERT_FALSE(validateBusClockDivider(999));
}

TEST(SMC_40CR2, switchDefaultClock) {
    ASSERT_TRUE(switchDefaultClock());
}
TEST(SMC_40CR2, configurePLL) {
  ASSERT_FALSE(configurePLL(static_cast<System_Clock_Speeds_t>(SYS_CLOCK_SPEED_UNDEFINED-1)));
  ASSERT_FALSE(configurePLL(SYS_CLOCK_SPEED_UNDEFINED));
  ASSERT_TRUE(configurePLL(SYS_CLOCK_SPEED_160M));
  ASSERT_EQ(RCC_PLLCFGR_MUL_1, getRCC()->PLLCFGR & RCC_PLLCFGR_MUL);
  ASSERT_TRUE(configurePLL(SYS_CLOCK_SPEED_80M));
  ASSERT_EQ(RCC_PLLCFGR_MUL_0, getRCC()->PLLCFGR & RCC_PLLCFGR_MUL);
  ASSERT_TRUE(configurePLL(SYS_CLOCK_SPEED_40M));
  ASSERT_EQ(0, getRCC()->PLLCFGR & RCC_PLLCFGR_MUL);
  ASSERT_TRUE(configurePLL(SYS_CLOCK_SPEED_20M));
  ASSERT_EQ(RCC_PLLCFGR_DIV_0, getRCC()->PLLCFGR & RCC_PLLCFGR_DIV);
  ASSERT_TRUE(configurePLL(SYS_CLOCK_SPEED_10M));
  ASSERT_EQ(RCC_PLLCFGR_DIV_1, getRCC()->PLLCFGR & RCC_PLLCFGR_DIV);
  ASSERT_TRUE(configurePLL(SYS_CLOCK_SPEED_5M));
  ASSERT_EQ(RCC_PLLCFGR_DIV_2, getRCC()->PLLCFGR & RCC_PLLCFGR_DIV);
  ASSERT_TRUE(configurePLL(SYS_CLOCK_SPEED_2_5M));
  ASSERT_EQ(RCC_PLLCFGR_DIV_2, getRCC()->PLLCFGR & RCC_PLLCFGR_DIV);
  ASSERT_TRUE(configurePLL(SYS_CLOCK_SPEED_1_25M));
  ASSERT_EQ(RCC_PLLCFGR_DIV_2, getRCC()->PLLCFGR & RCC_PLLCFGR_DIV);
  ASSERT_FALSE(configurePLL(static_cast<System_Clock_Speeds_t>(SYS_CLOCK_SPEED_MAX_ENUM_VAL+1)));
}

TEST(SMC_40CR2, turnOnPLLFailsWithTimeout) {
  setTimerCallback(nullptr);
  ASSERT_FALSE(turnOnPLL());
  ASSERT_EQ(RCC_CR_PLLON, getRCC()->CR & RCC_CR_PLLON);
}

TEST(SMC_40CR2, turnOnPLLSuccess) {
  setTimerCallback([](uint32_t timeoutCycles){
      if(timeoutCycles >= 5) {
          getRCC()->CR |= RCC_CR_PLL_RDY;
      }
  });
  ASSERT_TRUE(turnOnPLL());
  ASSERT_EQ(RCC_CR_PLLON, getRCC()->CR & RCC_CR_PLLON);
}

TEST(SMC_40CR2, selectSystemClockDivider) {

  getRCC()->CR = 0;
  selectSystemClockDivider(static_cast<System_Clock_Speeds_t>(SYS_CLOCK_SPEED_UNDEFINED-1));
  ASSERT_EQ(getRCC()->CR & RCC_CR_SYS_DIV, 0);

  getRCC()->CR = 0;
  selectSystemClockDivider(SYS_CLOCK_SPEED_UNDEFINED);
  ASSERT_EQ(getRCC()->CR & RCC_CR_SYS_DIV, 0);

  getRCC()->CR = 0;
  selectSystemClockDivider(SYS_CLOCK_SPEED_160M);
  ASSERT_EQ(getRCC()->CR & RCC_CR_SYS_DIV, 0);

  getRCC()->CR = 0;
  selectSystemClockDivider(SYS_CLOCK_SPEED_80M);
  ASSERT_EQ(getRCC()->CR & RCC_CR_SYS_DIV, 0);

  getRCC()->CR = 0;
  selectSystemClockDivider(SYS_CLOCK_SPEED_40M);
  ASSERT_EQ(getRCC()->CR & RCC_CR_SYS_DIV, 0);

  getRCC()->CR = 0;
  selectSystemClockDivider(SYS_CLOCK_SPEED_20M);
  ASSERT_EQ(getRCC()->CR & RCC_CR_SYS_DIV, 0);

  getRCC()->CR = 0;
  selectSystemClockDivider(SYS_CLOCK_SPEED_10M);
  ASSERT_EQ(getRCC()->CR & RCC_CR_SYS_DIV, 0);

  getRCC()->CR = 0;
  selectSystemClockDivider(SYS_CLOCK_SPEED_5M);
  ASSERT_EQ(getRCC()->CR & RCC_CR_SYS_DIV, 0);
  
  getRCC()->CR = 0;
  selectSystemClockDivider(SYS_CLOCK_SPEED_2_5M);
  ASSERT_EQ(getRCC()->CR & RCC_CR_SYS_DIV_0, RCC_CR_SYS_DIV_0);

  getRCC()->CR = 0;
  selectSystemClockDivider(SYS_CLOCK_SPEED_1_25M);
  ASSERT_EQ(getRCC()->CR & RCC_CR_SYS_DIV_1, RCC_CR_SYS_DIV_1);

  getRCC()->CR = 0;
  selectSystemClockDivider(static_cast<System_Clock_Speeds_t>(SYS_CLOCK_SPEED_MAX_ENUM_VAL+1));
  ASSERT_EQ(getRCC()->CR & RCC_CR_SYS_DIV, 0);
}

TEST(SMC_40CR2, selectBusClockDivider2) {
    selectBusClockDivider(2);
    ASSERT_EQ(getRCC()->CR & RCC_CR_BUS_DIV_0, RCC_CR_BUS_DIV_0);
}
TEST(SMC_40CR2, selectBusClockDivider4) {
    selectBusClockDivider(4);
    ASSERT_EQ(getRCC()->CR & RCC_CR_BUS_DIV_1, RCC_CR_BUS_DIV_1);
}

TEST(SMC_40CR2, switchClockInternalFailureWhenHSINotReadyAndClockSwitchTimeout) {
  setTimerCallback(nullptr);
  ASSERT_FALSE(switchClock(true));
  ASSERT_EQ(getRCC()->CR & RCC_CR_HSION, RCC_CR_HSION);
}

TEST(SMC_40CR2, switchClockInternalFailureWhenHSIReadyAndClockSwitchTimeout) {
  setTimerCallback([](uint32_t timeoutCycles){
      if(timeoutCycles >= 5) {
          getRCC()->CR |= RCC_CR_HSIRDY;
      }
  });
  ASSERT_FALSE(switchClock(true));
}

TEST(SMC_40CR2, switchClockInternalSuccess) {
  setTimerCallback([](uint32_t timeoutCycles){
      if(timeoutCycles >= 5) {
          getRCC()->CR |= RCC_CR_HSIRDY|RCC_CR_CLKSEL_0;
      }
  });
  ASSERT_TRUE(switchClock(true));
}

TEST(SMC_40CR2, switchClockExternalFailureWhenHSINotReadyAndClockSwitchTimeout) {
  setTimerCallback(nullptr);
  ASSERT_FALSE(switchClock(false));
  ASSERT_EQ(getRCC()->CR & RCC_CR_HSEON, RCC_CR_HSEON);
}

TEST(SMC_40CR2, switchClockExternalFailureWhenHSIReadyAndClockSwitchTimeout) {
  setTimerCallback([](uint32_t timeoutCycles){
      if(timeoutCycles >= 5) {
          getRCC()->CR |= RCC_CR_HSERDY;
      }
  });
  ASSERT_FALSE(switchClock(false));
}

TEST(SMC_40CR2, switchClockExternalSuccess) {
  setTimerCallback([](uint32_t timeoutCycles){
      if(timeoutCycles >= 0) {
          getRCC()->CR |= RCC_CR_HSERDY|RCC_CR_CLKSEL_1;
      }
  });
  ASSERT_TRUE(switchClock(false));
}

class TraceHandler : public testing::EmptyTestEventListener {
    // Called before a test starts.
    void OnTestStart( const testing::TestInfo& test_info ) override {
        getRCC()->CR = 0;
    }
    // Called after a test ends.
    void OnTestEnd( const testing::TestInfo& test_info ) override {
        // close the log here
    }
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    testing::TestEventListeners& listeners =
            testing::UnitTest::GetInstance()->listeners();
    // Adds a listener to the end.  googletest takes the ownership.
    listeners.Append(new TraceHandler);
    return RUN_ALL_TESTS();
}