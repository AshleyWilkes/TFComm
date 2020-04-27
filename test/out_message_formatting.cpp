#include "gtest/gtest.h"
#include "mock_TFApplication.hpp"
#include "mock_TFSender.hpp"
#include "TFComm.hpp"

namespace {
  
  using ::testing::NiceMock;
  using ::testing::Return;

  TEST(OutMessageFormatting, echo) {
    MockTFCommSender& sender = MockTFCommSender::getInstance();
    MockTFApplication application;
    std::string testString = "test&@!)";
    std::string testRequestString = "00120008" + testString;

    EXPECT_CALL(sender, isConnected())
      .WillOnce(Return(true));
    EXPECT_CALL(sender, sendRequestWait(testRequestString))
      .WillOnce(Return(testRequestString));

    TFComm comm(sender, application);
    EXPECT_EQ(comm.echo(testString), true);
  }

}
