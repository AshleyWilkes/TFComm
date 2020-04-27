#pragma once
#include "TFCommApplication.hpp"
#include "TFCommRequest.hpp"
#include "TFCommResponse.hpp"
#include "TFCommSender.hpp"

class Exchange {
  public:
    Exchange( int cmd ) : m_cmd{ cmd }, m_cmdResp{ cmd | 0x8000 } {}

    bool perform(const TFCommSender&, TFCommApplication&);

    virtual int checkPrerequisites( const TFCommSender& ) const = 0;
    virtual void fillRequestTail( TFCommRequest& ) const = 0;
    virtual void processResponseTail( TFCommResponse& ) = 0;
    virtual void processErrorStatusInner( int, TFCommApplication& ) {};

    std::string createRequestString() const; 
    bool processResponse( TFCommResponse&, TFCommApplication& );
    void processErrorStatus( int status, TFCommApplication& );
    bool shouldResend() const { return resend; }

    virtual ~Exchange(){}
  private:
    int m_cmd, m_cmdResp;
    bool resend{ false };
};

#define CMD_ECHO              0x0000
#define CMD_OPEN_SESSION      0x0010
#define CMD_CLOSE_SESSION     0x0012
#define CMD_SEND_TRANSACTION  0x0020
#define CMD_GET_STOPLIST      0x0032
#define CMD_SEND_LOGFILE      0x0040

#define ERR_NOT_CONNECTED           10
#define ERR_SESSION_ALREADY_OPENED  11
#define ERR_SESSION_NOT_OPENED      12

//extern int echo();//overeni funkcnosti spojeni
//request:  0x0000, (data)
//response: 0x8000, STATUS(2B), (data)
class ExchangeEcho : public Exchange {
  public:
    ExchangeEcho( const std::string& testData = "" ) : Exchange{ CMD_ECHO }, m_testData{ testData } {}

    int checkPrerequisites( const TFCommSender& sender ) const { 
      return sender.isConnected() ? 0 : ERR_NOT_CONNECTED;
    }

    void fillRequestTail( TFCommRequest& request ) const {
      request.fillString( 2, m_testData );
    }

    void processResponseTail( TFCommResponse& response ) {
      std::string returnedTestData = response.recvString( 2 );
      if ( returnedTestData.compare( m_testData ) ) {
        //return ERR_INVALID_ECHO_RESPONSE;
      }
    }
  private:
    std::string m_testData;
};

