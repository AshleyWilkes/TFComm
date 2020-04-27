#pragma once
#include "TFCommResponse.hpp"
#include <memory>

class TFCommSender {
  public:
    virtual bool isConnected() const = 0;
    virtual bool isSessionOpened() const = 0;

    virtual std::unique_ptr<TFCommResponse> sendRequestWait(const std::string&) const = 0;
    virtual std::unique_ptr<TFCommResponse> resendRequestWait(const std::string&) const = 0;

    virtual void notifyExpectedDisconnect() = 0;

    virtual ~TFCommSender(){}
};
