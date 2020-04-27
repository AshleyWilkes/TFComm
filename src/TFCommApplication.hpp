#pragma once
#include<string>

class TFCommApplication {
  public:
    virtual void setOpenSessionReserved(const std::string&) = 0;
    virtual void setStoplist(const std::string&) = 0;
    virtual void notifyTFCommError(int) = 0;
    virtual ~TFCommApplication(){}
};
