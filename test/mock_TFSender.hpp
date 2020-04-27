#pragma once
#include "gmock/gmock.h"
#include "TFCommSender.hpp"
#include "Exchange.hpp"

namespace {

  class MockTFCommSender : public TFCommSender {
    public:
      static MockTFCommSender& getInstance() {
        static MockTFCommSender instance;
        return instance;
      }

      //MOCK_METHOD(bool, isConnected, (), (const, override));
      MOCK_CONST_METHOD0(isConnected, bool());
      //MOCK_METHOD(bool, isSessionOpened, (), (const, override));
      MOCK_CONST_METHOD0(isSessionOpened, bool());
      //MOCK_METHOD(std::unique_ptr<TFCommResponse>, sendRequestWait, (const std::string&), (const, override));
      MOCK_CONST_METHOD1(sendRequestWait, std::unique_ptr<TFCommResponse>(const std::string&));
      //MOCK_METHOD(std::unique_ptr<TFCommResponse>, resendRequestWait, (const TFCommRequest&), (const, override));
      MOCK_CONST_METHOD1(resendRequestWait, std::unique_ptr<TFCommResponse>(const std::string&));
      //MOCK_METHOD(void, notifyExpectedDisconnect, (), (override));
      MOCK_METHOD0(notifyExpectedDisconnect, void());
  };

}
