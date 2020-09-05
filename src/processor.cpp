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
  if(cpu_total>0){
    utilization =  ((float)cpu_total-idle)/cpu_total;
    }
//  long total_diff = cpu_total-prev_total;
//  long idle_diff  = idle-prev_idle;
//  utilization = (float)(total_diff-idle_diff)/total_diff;
//  prev_total = cpu_total;
//  prev_idle   = idle;
  return utilization;
}