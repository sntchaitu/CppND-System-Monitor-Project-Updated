#include <string>

#include "format.h"
#include "linux_parser.h"
using std::string;

//// INPUT: Long int measuring seconds
//// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long int  seconds){

  long hour = seconds/3600;
  long min = (seconds/60)%60;
  long sec = seconds%60;
  return LongToString(hour)+":"+LongToString(min)+":"+LongToString(sec);
}

string Format::LongToString(long val){
  if(val<10) return "0"+std::to_string(val);
  return std::to_string(val);
}
