#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
using std::string;
using std::to_string;
using std::vector;


Process::Process(int pid){
  this->pid = pid;
}


int Process::Pid() {
  return pid;
}

float Process::CpuUtilization() {
  long system_up_time = LinuxParser::UpTime();
  long process_start_time = LinuxParser::UpTime(pid);
  long hertz = sysconf(_SC_CLK_TCK);
  long total_time_in_ticks = LinuxParser::ActiveJiffies(pid);
  long seconds = system_up_time - (process_start_time/hertz);
  float total_time_in_seconds = (float)(total_time_in_ticks) / hertz;
  cpuUtilization =  total_time_in_seconds/(float)seconds;
  return cpuUtilization;
}

string Process::Command() {
  command = LinuxParser::Command(pid);
  return command;
}

string Process::Ram() {
  ram = LinuxParser::Ram(pid);
  return ram;
}

string Process::User() {
  user = LinuxParser::User(pid);
  return user;
}

long int Process::UpTime() {
  uptime = LinuxParser::UpTime(pid);
  return uptime;
}

bool Process::operator<(Process const& a) const {
  return a.cpuUtilization < this->cpuUtilization;
}