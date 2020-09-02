#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

float Processor::Utilization() {
  float utilization;
  long active_jiffies = LinuxParser::ActiveJiffies();
  long idle_jiffies   = LinuxParser::IdleJiffies();
  long cpu_used = active_jiffies-prev_active_jiffies;
  long cpu_idle  = idle_jiffies-prev_idle_jiffies;
  long cpu_total = cpu_idle+cpu_used;
  utilization = ((float)cpu_used)/cpu_total;
  prev_active_jiffies = active_jiffies;
  prev_idle_jiffies   = idle_jiffies;
  return utilization;
}