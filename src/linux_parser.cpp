#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::ifstream;
using std::replace;
using std::getline;
using std::find;
using std::istringstream;

vector<string> parser(string path, vector<vector<char>> replacements, vector<string> keys = {}, int index = 0)
{
  vector<string> result;
  string line, key, value;
  ifstream filestream(path);
  if (filestream.is_open()) 
  {
    while (getline(filestream, line)) 
    {
      for(auto& replacement : replacements)
      {
        replace(line.begin(), line.end(), replacement[0], replacement[1]);
      }
      istringstream linestream(line);
      if (keys.empty())
      {
        for(int i=0; i<index; i++)
        {
          linestream >> value;
        }
        result.push_back(value);
      }
      else
      {
        while (linestream >> key >> value) 
        {
          if(keys.end() != find(keys.begin(), keys.end(), key))
          {
            for(auto& replacement : replacements)
            {
              replace(value.begin(), value.end(), replacement[1], replacement[0]);
            }
            result.push_back(value);
          }
        }
      }
      filestream.close();
      return result;
    }
  }
}
// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() 
{
  vector<string> result;
  result = parser(kOSPath, {{' ', '_'}, {'=', ' '}, {'"', ' '}},{"PRETTY_NAME"});
  return result[0];
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() 
{
  vector<string> result;
  result = parser(kProcDirectory + kVersionFilename, {}, {}, 3);
  return result[0];
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{
  int memTotal, memFree;
  string line;
  string key;
  string value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) 
  {
    while (std::getline(stream, line)) 
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value) 
      {
        if (key == "MemTotal:") 
        {
          memTotal = stoi(value);
        }
        else if(key == "MemFree:")
        {
          memFree =stoi(value);
        }
      }
    }
    return memTotal - memFree;
  }
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { return 0; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }