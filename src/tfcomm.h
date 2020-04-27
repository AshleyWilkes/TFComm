extern int initModule();//jednorazovy init pri startu aplikace -- inicializuje singletona pomoci getSingleton()
extern int initState();//uvedeni modulu do stavu, kdy je pripraven pokusit se o spojeni; vola se pri startu a vzdy po ukonceni spojeni; vola obdobnou metodu singletona


//commsStart(&handle, uri, options)
//commsConnect(handle)
extern int connect();//navazat spojeni s COMM serverem

//commsClose(handle)
//commsStop(&handle)
extern int disconnect();//ukoncit spojeni s COMM serverem, obnovit defaultni stav pomoci initState

//struktura nasledujicich 6 metod (rikejme jim Exchanges) je vicemene identicka:
//- vytvor request,
//- odesli a pak prijimej data, dokud nemas celou odpoved
//- vysledkem je, ze vsechno je OK cili nic
//- openSession() a recvStopList() maji zpristupnit dodatecna data -- mejme dve metody, ktere ona data vraceji, prislusne exchange data setnou

//Exchange je tedy abstraktni predek, jednotlive exchange maji kazda sveho potomka
//metody, ktere nasleduji, funguji takto:
//vytvor instanci prislusneho potomka Exchange
//return singleton.performExchange(ta instance)
//
//metoda performExchange(Exchange& exchange) dela toto:
//int err = exchange.checkPrerequisites(this);//virtualni metoda; kdyz nevrati 0, return; je to error, ktery musi resit klient TFComma
//do {
//  clearBuffers();
//  exchange.fillRequest(this);//virtualni metoda; muze volat gettery thisu
//  sendRequestWait();//nevirtualni metoda
//  exchange.processResponse(this);//virtualni metoda; muze volat i settery thisu; muze taky volat recvBufferWait(length, buffer), resp. v predkovi implementovane sofistikovanejsi verze ???recv1B, recv2B???
//} while( exchange.shouldResend() );
//


extern int openSession();//prihlaseni terminalu
//request:  0x0010, PROTOCOL_VERSION(1B), LEN1(1B), POSID(LEN1), LEN2(1B), PASSWORD(LEN2)
//response: 0x8010, STATUS(2B), LEN(2B), RESERVED(LEN)
class ExchangeOpenSession : Exchange {
  public:
    ExchangeOpenSession( char protocolVersion, const std::string& terminalId, const std::string& password ) 
      : Exchange{ CMD_OPEN_SESSION }, m_protocolVersion{ protocolVersion }, m_terminalId{ terminalId }, m_password{ password } {}

    int checkPrerequisites( const TFComm& comm ) const {
      return comm.isSessionOpened() ? ERR_SESSION_ALREADY_OPENED : 0;
    }

    void fillRequestTail( TFComm& comm ) const {
      comm.fill1B( m_protocolVersion );
      comm.fillString( 1, m_terminalId );
      comm.fillString( 1, m_password );
    }

    void processResponseTail( TFComm& comm ) {
      std::string reserved = comm.recvString( 2 );
      comm.setOpenSessionReserved( reserved );
    }
  private:
    char m_protocolVersion;
    std::string m_terminalId, m_password;
};

extern int closeSession();//odhlaseni terminalu
//request:  0x0012, LEN(2B), RESERVED(LEN)
//response: 0x8012, STATUS(2B)
class ExchangeCloseSession : Exchange {
  public:
    ExchangeCloseSession( const std::string& reservedData = "" ) : Exchange{ CMD_CLOSE_SESSION }, m_reservedData{ reservedData } {}

    int checkPrerequisites( const TFComm& comm ) const {
      return comm.isSessionOpened() ? 0 : ERR_SESSION_NOT_OPENED;
    }

    void fillRequestTail( TFComm& comm ) const {
      comm.fillString( 2, m_reservedData );
    }

    void processResponseTail( TFComm& comm ) {}
  private:
    std::string m_reservedData;
};

extern int sendOneTransaction();//odeslani jedne transakce
//request:  0x0020, XID(4B), FORMAT(1B), LEN(1B), NAME(LEN), SIZE(4B), DATA(SIZE)
//response: 0x8020, STATUS(2B), XID(4B)
class ExchangeTransaction : Exchange {
  public:
    ExchangeTransaction(long xid, char format, const std::string& name, const std::string& data) 
      : Exchange{ CMD_SEND_TRANSACTION }, m_xid{ xid }, m_format{ format }, m_name{ name }, m_data{ data } {}

    int checkPrerequisites( const TFComm& comm ) const {
      return comm.isSessionOpened() ? 0 : ERR_SESSION_NOT_OPENED;
    }

    void fillRequestTail( TFComm& comm ) const {
      comm.fill4B( m_xid );
      comm.fill1B( m_format );
      comm.fillString( 1, m_name );
      comm.fillString( 4, m_data );
    }

    void processResponseTail( TFComm& comm ) {
      long xid = comm.recv4B();
      if ( xid != m_xid ) {
        //return ERR_INVALID_XID_RETURNED;
      }
    }
  private:
    long m_xid;
    char m_format;
    const std::string& m_name, m_data;
};

extern int sendOneLogFile();
//request:  0x0040, XID(4B), LEN(1B), NAME(LEN), SIZE(4B), DATA(SIZE)
//response: 0x8040, STATUS(2B), XID(4B)
class ExchangeLogFile : Exchange {
  public:
    ExchangeLogFile( long xid, const std::string& name, const std::string& data )
      : Exchange{ CMD_SEND_LOGFILE }, m_xid{ xid }, m_name{ name }, m_data{ data } {}

    int checkPrerequisites( const TFComm& comm ) const {
      return comm.isSessionOpened() ? 0 : ERR_SESSION_NOT_OPENED;
    }

    void fillRequestTail( TFComm& comm ) const {
      comm.fill4B( m_xid );
      comm.fillString( 1, m_name );
      comm.fillString( 4, m_data );
    }

    void processResponseTail( TFComm& comm ) {
      long xid = comm.recv4B();
      if ( xid != m_xid ) {
        //return ERR_INVALID_XID_RETURNED;
      }
    }
  private:
    long m_xid;
    std::string m_name, m_data;
};

extern int recvStoplist();//prijeti stopListu
//request:  0x0032, XID(4B), LEN(1B), TERMINAL_STOPLIST_VERSION(LEN)
//response: 0x8032, STATUS(2B), XID(4B), LEN(1B), VERSION(LEN), SIZE(4B), DATA(SIZE)
class ExchangeStoplist : Exchange {
  public:
    ExchangeStoplist( long xid, const std::string& stoplistVersion )
      : Exchange{ CMD_GET_STOPLIST }, m_xid{ xid }, m_stoplistVersion{ stoplistVersion } {}

    int checkPrerequisites( const TFComm& comm ) const {
      return comm.isSessionOpened() ? 0 : ERR_SESSION_NOT_OPENED;
    }

    void fillRequestTail( TFComm& comm ) const {
      comm.fill4B( m_xid );
      comm.fillString( 1, m_stoplistVersion );
    }

    void processResponseTail( TFComm& comm ) {
      long xid = comm.recv4B();
      if ( xid != m_xid ) {
        //return ERR_INVALID_XID_RETURNED;
      }

      std::string version = comm.recvString( 1 );
      if ( version.compare( m_stoplistVersion ) != 0 ) {//toto je v realu a little bit komplikovanejsi
        //return ERR_INVALID_STOPLIST_VERSION_RETURNED;
      }

      std::string stoplistData = comm.recvString( 4 );
      comm.setStoplistData( stoplistData );
    }
  private:
    long m_xid;
    std::string m_stoplistVersion;
};



//jako parametr mas pole transakci, pro kazdou z nich zavolej sendOneTransaction()
extern int sendTransactions();//odeslani transakci

//jako parametr mas pole logFilu, pro kazdy z nich zavolej sendOneLogFile()
extern int sendLogFiles();//odeslani log filu
