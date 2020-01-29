#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"
#include <iostream>
#include <unistd.h>

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::ifstream;
using std::replace;
using std::getline;
using std::find;
using std::istringstream;

void replaceChars(string& String, vector<vector<char>> replacements, bool reverse=false)
{
  if(reverse)
  {
    for(int i = replacements.size() - 1; i >= 0; i--)
    {
      replace(String.begin(), String.end(), replacements[i][1], replacements[i][0]);
    }
  }
  else
  {
    for(auto& replacement : replacements)
    {
      replace(String.begin(), String.end(), replacement[0], replacement[1]);
    }    
  }
  
}

void parseWithIndexes(vector<string>& result, vector<int> indexes, istringstream& linestream, vector<vector<char>> replacements)
{
  int counter = 0;
  string value;
  for(int i = 1; i<=indexes[indexes.size() - 1]; i++)
  {
    linestream >> value;
    if(i == indexes[counter])
    {
      replaceChars(value, replacements, true);
      result.push_back(value);
      counter++;
    }
  }
}

vector<string> superParser(string path, vector<vector<char>> replacements, vector<string> keys = {}, vector<int> indexes = {})
{
  vector<string> result;
  string line, key, value;
  ifstream filestream(path);
  if (filestream.is_open()) 
  {
    while (getline(filestream, line)) 
    {
      replaceChars(line, replacements);
      istringstream linestream(line);
      if (keys.empty())
      {
        parseWithIndexes(result, indexes, linestream, replacements);
        break;
      }
      else
      {
        linestream >> key;
        if(keys.end() != find(keys.begin(), keys.end(), key))
        {
          while (linestream >> value)
          {
            replaceChars(value, replacements, true);
            result.push_back(value);
          }
        }
      }
    }
    filestream.close();
  }
  if(result.size() == 0)
  {
    result = {"0"};
  }
  return result;
}
// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() 
{
  vector<string> result;
  vector<vector<char>> const replacements ={{' ', '_'}, {'=', ' '}, {'"', ' '}};
  vector<string> keys = {"PRETTY_NAME"};
  result = superParser(kOSPath, replacements, keys);
  return result[0];
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() 
{
  vector<string> result;
  result = superParser(kProcDirectory + kVersionFilename, {}, {}, {3});
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
  result = superParser(kProcDirectory + kMeminfoFilename, {}, keys);
  return stof(result[0]) - stof(result[2]);
}

// Reads and returns the system uptime
long LinuxParser::UpTime() 
{ 
  vector<string> result;
  result = superParser(kProcDirectory + kUptimeFilename, {}, {}, {1});
  return stol(result[0]);
}

// Reads and returns the number of jiffies for the system
long LinuxParser::Jiffies() 
{ 
  vector<string> result;
  long totalJiffies = 0;
  result = superParser(kProcDirectory + kStatFilename, {}, {"cpu"});
  for (auto& res : result)
  {
    totalJiffies += stol(res);
  }
  return totalJiffies;
}

// Reads and returns the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid)  
{ 
  vector<string> result;
  long totalTime, seconds;
  string path = kProcDirectory + to_string(pid) + kStatFilename;
  result = superParser(path, {}, {}, {14, 15, 16, 17, 22});
  totalTime = stol(result[0]) + stol(result[1]) + stol(result[2]) + stol(result[3]);
  seconds = UpTime() - (stol(result[3]) / sysconf(_SC_CLK_TCK));
  return 100 * ((totalTime/sysconf(_SC_CLK_TCK))/seconds);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{  
  vector<string> result;
  long activeJiffies = 0;
  result = superParser(kProcDirectory + kStatFilename, {}, {"cpu"});
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
  result = superParser(kProcDirectory + kStatFilename, {}, {"cpu"});
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
  result = superParser(kProcDirectory + kStatFilename, {}, {"processes"});
  return stoi(result[0]);
}

// Reads and returns the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  vector<string> result;
  result = superParser(kProcDirectory + kStatFilename, {}, {"procs_running"});
  return stoi(result[0]);
}

// Reads and returns the command associated with a process
string LinuxParser::Command(int pid) 
{ 
  vector<string> result;
  vector<vector<char>> const replacements = {{' ', '_'}};
  string path = kProcDirectory + to_string(pid) + kCmdlineFilename;
  result = superParser(path, replacements, {}, {1});
  return result[0]; 
}

// Reads and returns the memory used by a process
string LinuxParser::Ram(int pid) 
{ 
  vector<string> result;
  vector<vector<char>> const replacements = {{' ', '_'}};
  string path = kProcDirectory + to_string(pid) + kStatusFilename;
  result = superParser(path, {}, {"VmSize:"});
  return to_string(stol(result[0])/1000) + " " + result[1];
}

// Reads and returns the user ID associated with a process
string LinuxParser::Uid(int pid) 
{ 
  vector<string> result;
  string path = kProcDirectory + to_string(pid) + kStatusFilename;
  result = superParser(path, {}, {"Uid:"});
  return result[0]; 
}

// Reads and returns the user associated with a process
string LinuxParser::User(int pid) 
{ 
  string line, userName, x, Id;
  string Uid = LinuxParser::Uid(pid);
  ifstream filestream(kPasswordPath);
  if (filestream.is_open()) 
  {
    while (getline(filestream, line)) 
    {
      replaceChars(line, {{':', ' '}});
      istringstream linestream(line);
      linestream >> userName >> x >> Id;
      if(Uid == Id)
      {
        break;
      }
    }
    filestream.close();
  }
  return (userName == "") ? "": userName;
}

// Reads and returns the uptime of a process
long LinuxParser::UpTime(int pid) 
{ 
  vector<string> result;
  string path = kProcDirectory + to_string(pid) + kStatFilename;
  result = superParser(path, {}, {}, {14});
  return stol(result[0])/sysconf(_SC_CLK_TCK); 
}