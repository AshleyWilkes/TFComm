#pragma once
#include<sstream>
#include<iomanip>

class TFCommRequest {
  public:
    void fill1B( char value ) { fill(value, 1); }
    void fill2B( int value ) { fill(value, 2); }
    void fill4B( long value ) { fill(value, 4); }
    void fillString( int lengthSize, const std::string& string ) {
      fill(string.size(), lengthSize);
      m_ss << string;
    }
    std::string getRequestString() {
      std::stringstream ss;
      std::string str = m_ss.str();
      m_ss = std::stringstream{};
      ss << std::setw(4) << std::setfill('0') << str.size() << str;
      return ss.str();
    }
  private:
    std::stringstream m_ss;
    void fill(int value, int size){ m_ss << std::setw(size) << std::setfill('0') << value; }
};

//dale tady bude metoda getRequestString(), ktera vrati na zacatku 4B delku toho ss a na konci jeho obsah
//ta metoda se nekde potrebuje
