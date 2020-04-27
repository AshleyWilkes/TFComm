#pragma once
#include "Exchange.hpp"
#include "TFCommApplication.hpp"
#include "TFCommSender.hpp"

class TFComm {
  public:
    TFComm(TFCommSender& sender, TFCommApplication& application)
      : m_sender{ sender }, m_application{ application } {}

    bool performExchange(const Exchange& exchange);

    bool echo(const std::string& testData = "") {
      ExchangeEcho exchange{ testData };
      return exchange.perform(m_sender, m_application);
    }
  private:
    TFCommSender& m_sender;
    TFCommApplication& m_application;
};
