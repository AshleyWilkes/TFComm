#pragma once

class TFCommResponse {
  public:
    virtual char recv1B() = 0;
    virtual int recv2B() = 0;
    virtual long recv4B() = 0;
    virtual std::string recvString( int lengthSize ) = 0;
    virtual ~TFCommResponse();
};


