#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"
#include "format.h"
#include "iostream"

using namespace std;
int main() {
  System system;
  NCursesDisplay::Display(system);
 // Processor p;
 //std::cout<<"cpu  utilization is " <<p.Utilization();

}

