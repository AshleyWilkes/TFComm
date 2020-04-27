#pragma once
#include "gmock/gmock.h"
#include "TFCommApplication.hpp"

namespace {

  class MockTFApplication : public TFCommApplication {
    public:
      //MOCK_METHOD(void, setOpenSessionReserved, (const std::string&), (override));
      MOCK_METHOD1(setOpenSessionReserved, void(const std::string&));
      //MOCK_METHOD(void, setStoplist, (const std::string&), (override));
      MOCK_METHOD1(setStoplist, void(const std::string&));
      //MOCK_METHOD(void, notifyTFCommError, (int), (override));
      MOCK_METHOD1(notifyTFCommError, void(int));
  };

};
