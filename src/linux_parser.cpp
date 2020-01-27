#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"
#include <iostream>

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
        for(int i = replacements.size() - 1; i >= 0; i--)
        {
          replace(value.begin(), value.end(), replacements[i][1], replacements[i][0]);
        }
        result.push_back(value);
        break;
      }
      else
      {
        linestream >> key;
        if(keys.end() != find(keys.begin(), keys.end(), key))
        {
          while (linestream >> value)
          {
            for(int i = replacements.size() - 1; i >= 0; i--)
            {
              replace(value.begin(), value.end(), replacements[i][1], replacements[i][0]);
            }
            result.push_back(value);
          }
        }
      }
    }
    filestream.close();
    return result;
  }
}
// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() 
{
  vector<string> result;
  vector<vector<char>> const replacements ={{' ', '_'}, {'=', ' '}, {'"', ' '}};
  vector<string> keys = {"PRETTY_NAME"};
  result = parser(kOSPath, replacements, keys, 0);
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

// Reads and returns the system memory utilization
float LinuxParser::MemoryUtilization() 
{
  vector<string> result;
  vector<string> keys = {"MemTotal:", "MemFree:"};
  result = parser(kProcDirectory + kMeminfoFilename, {}, keys);
  return stof(result[0]) - stof(result[2]);
}

// Reads and returns the system uptime
long LinuxParser::UpTime() 
{ 
  vector<string> result;
  result = parser(kProcDirectory + kUptimeFilename, {}, {}, 1);
  return stol(result[0]);
}

// Reads and returns the number of jiffies for the system
long LinuxParser::Jiffies() 
{ 
  vector<string> result;
  long totalJiffies = 0;
  result = parser(kProcDirectory + kStatFilename, {}, {"cpu"});
  for (auto& res : result)
  {
    totalJiffies += stol(res);
  }
  return totalJiffies;
}

// Reads and returns the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{  
  vector<string> result;
  long activeJiffies = 0;
  result = parser(kProcDirectory + kStatFilename, {}, {"cpu"});
  int indexes [6]= {0, 1, 2, 5, 6, 7};
  for(int index : indexes)
  {
    activeJiffies += stol(result[index]);
  }
  return activeJiffies;
}

// Reads and returns the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{ 
  vector<string> result;
  long idleJiffies = 0;
  result = parser(kProcDirectory + kStatFilename, {}, {"cpu"});
  int indexes [2]= {3, 4};
  for(int index : indexes)
  {
    idleJiffies += stol(result[index]);
  }
  return idleJiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// Reads and returns the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  vector<string> result;
  long idleJiffies = 0;
  result = parser(kProcDirectory + kStatFilename, {}, {"processes"});
  return stoi(result[0]);
}

// Reads and returns the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  vector<string> result;
  long idleJiffies = 0;
  result = parser(kProcDirectory + kStatFilename, {}, {"procs_running"});
  return stoi(result[0]);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{ 
  vector<string> result;
  vector<vector<char>> const replacements = {{' ', '_'}};
  string path = kProcDirectory + to_string(pid) + kCmdlineFilename;
  result = parser(path, replacements, {}, 1);
  return result[0]; 
}

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