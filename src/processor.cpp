#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

float Processor::Utilization() {
  float utilization;
  long active = LinuxParser::ActiveJiffies();
  long idle   = LinuxParser::IdleJiffies();
  long cpu_total = active+idle;
  long total_diff = cpu_total-prev_total;
  long idle_diff  = idle-prev_idle;
  if(total_diff>0)  utilization = (float)(total_diff-idle_diff)/total_diff;
  prev_total = cpu_total;
  prev_idle   = idle;
  return utilization;
}