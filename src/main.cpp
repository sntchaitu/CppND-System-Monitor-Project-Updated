#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"
#include "iostream"
#include "format.h"
using namespace std;
int main() {
  System system;
  NCursesDisplay::Display(system);
//  std::cout<<"memory utilization: "<<LinuxParser::MemoryUtilization();

//std::cout<<"Elaspsed time"<<Format::ElapsedTime(10999);
//return 0;
}