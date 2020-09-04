#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel,version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  float mem_total{1};
  float mem_free{0};
  float buffers{0};
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        mem_total = std::stof(value);
      } else if (key == "MemFree:") {
        mem_free = std::stof(value);
      } else if (key == "Buffers:") {
        buffers = std::stof(value);
      }
    }
  }
  return 1- mem_free/(mem_total-buffers);
}

long LinuxParser::UpTime() {

  string line;
  long uptime = 0.0;
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> line) {
          if(!line.empty()){
             uptime = std::stol(line);
             return uptime;
          }

        }
      }
    }
    return uptime;
  }

long LinuxParser::Jiffies() {
return UpTime()*sysconf(_SC_CLK_TCK);
}

long LinuxParser::ActiveJiffies(int pid) {
    string token;
    long total_time_in_ticks{0};
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
    if (filestream.is_open()) {
      int counter = 0;
      while(filestream >> token){
        if(counter==kUtime_)  total_time_in_ticks += stol(token);
        else if(counter==kStime_) total_time_in_ticks += stol(token);
        else if(counter==kCutime_) total_time_in_ticks += stol(token);
        else if(counter==kCstime_){
          total_time_in_ticks += stol(token);
          break;
        }
        counter++;
      }
    }
    return total_time_in_ticks;
}

long LinuxParser::ActiveJiffies() {
  vector<string> cpuvalues = CpuUtilization();
  return stol(cpuvalues[CPUStates::kIdle_])+
         stol(cpuvalues[CPUStates::kIOwait_]);

}

long LinuxParser::IdleJiffies() {
  vector<string> cpuvalues = CpuUtilization();
  return stol(cpuvalues[CPUStates::kUser_])+
         stol(cpuvalues[CPUStates::kNice_])+
         stol(cpuvalues[CPUStates::kSystem_])+
         stol(cpuvalues[CPUStates::kIRQ_])+
         stol(cpuvalues[CPUStates::kSoftIRQ_])+
         stol(cpuvalues[CPUStates::kSteal_])+
         stol(cpuvalues[CPUStates::kGuest_])+
         stol(cpuvalues[CPUStates::kGuestNice_]);
}

vector<string> LinuxParser::CpuUtilization() {
  string line;
  string value;
  vector<string> cpuvalues;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while(linestream>>value)
      {
        if(value=="cpu"){
          while(linestream>>value)cpuvalues.push_back(value);
          return cpuvalues;
        }
      }
    }
  }
  return cpuvalues;
}

int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return value;
}


string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      return line;
    }
  }
  return line;
}

string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          return to_string(stoi(value)/1024);
        }
      }
    }
  }
  return "0";
}

string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::User(int pid) {
  string line;
  string user;
  string token;
  string uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      if (linestream >> token) user = token;
      while (linestream >> token) {
        if (token == uid) {
          return user;
        }
      }
    }
  }
  return user;
}

long LinuxParser::UpTime(int pid) {
  string line;
  long uptimeinticks;
  string token;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    int counter = 0;
    while(filestream >> token){
      if(counter==kStarttime_) {
        uptimeinticks = stol(token);
        break;
      }
      counter++;
    }

  }
  return uptimeinticks/sysconf(_SC_CLK_TCK);
}