#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
using std::string;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  string User();                      // TODO: See src/process.cpp
  string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  Process(int pid);


 private:
  int pid;
  string user;
  string command;
  float cpuUtilization;
  string ram;
  long int uptime;
};

#endif