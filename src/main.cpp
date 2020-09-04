#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"
#include "format.h"

using namespace std;
int main() {
  System system;
  NCursesDisplay::Display(system);
}

