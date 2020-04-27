#include "Exchange.hpp"

bool Exchange::perform(const TFCommSender& sender, TFCommApplication& application) {
  int err = checkPrerequisites(sender);
  //tady chybi checkovani err
  std::string reqStr = createRequestString();
  std::unique_ptr<TFCommResponse> resp = sender.sendRequestWait(reqStr);
  return processResponse(*resp, application);
  //tady chybi resend, ktery si response muze vyzadat
}

std::string Exchange::createRequestString() const {
  TFCommRequest req;
  req.fill2B( m_cmd );
  fillRequestTail( req );
  return req.getRequestString();
}

bool Exchange::processResponse( TFCommResponse& resp, TFCommApplication& application ) {
  int cmdResp = resp.recv2B();
  if ( cmdResp != m_cmdResp ) {
    //return comm.invalidResponse( cmdResp, m_cmdResp );
  }

  //pokud status neni 0, nenasleduji zadna dalsi data, takze
  //pokud status je 0, volam processResponseTail() a tato metoda muze pracovat s tim, ze status je 0
  //pokud status neni 0, volam jinou metodu, jejimz parametrem je ten status a comm asi taky
  int status = resp.recv2B();
  if ( status == 0 ) {
    processResponseTail( resp );
    return true;
  } else {
    processErrorStatus( status, application );
    return false;
  }
}
//
//Statusy mohou byt:
//0 - OK (Echo, OpenSession, CloseSession, SendTransaction, GetStoplist, SendLogfile)
//1 - BAD_REQUEST_FORMAT (OpenSession, SendTransaction, GetStoplist, SendLogfile)
//2 - UNKNOWN_TERMINAL (OpenSession)
//2 - SESSION_NOT_OPENED (SendTransaction, GetStoplist, SendLogfile)
//3 - WRONG_PASSWORD (OpenSession)
//3 - NO_NEW_STOPLIST_VERSION (GetStoplist)
//5 - REPEAT (Echo, OpenSession, CloseSession, SendTransaction, GetStoplist, SendLogfile)
void Exchange::processErrorStatus( int status, TFCommApplication& application ) {
  if ( status == 5 ) {
    resend = true;
  } else {
    processErrorStatusInner( status, application );
  }
}

