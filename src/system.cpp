#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "linux_parser.h"
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::sort;
using std::set;
using std::size_t;
using std::string;
using std::vector;


System::System()
{
    cpu_ = Processor();
    vector<int> pIds = LinuxParser::Pids();
    for(int pId : pIds)
    {
        Process p(pId);
        if (p.Ram() != "0.000000 MB")
        {
          processes_.push_back(p);
        }
    }
    kernel_ = LinuxParser::Kernel();
    os_ = LinuxParser::OperatingSystem();
    uptime_ =LinuxParser::UpTime();
}

// Returns the system's CPU
Processor& System::Cpu() 
{ 
    return cpu_;
}

// Returns a container composed of the system's processes
vector<Process>& System::Processes() 
{ 
    return processes_; 
}

// Returns the system's kernel identifier (string)
std::string System::Kernel() 
{ 
    return kernel_; 
}

// Returns the system's memory utilization
float System::MemoryUtilization() 
{ 
    return LinuxParser::MemoryUtilization(); 
}

// Returns the operating system name
std::string System::OperatingSystem() 
{ 
    return os_; 
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() 
{ 
    return LinuxParser::RunningProcesses(); 
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() 
{ 
    return LinuxParser::TotalProcesses(); 
}

// TODO: Return the number of seconds since the system started running
long System::UpTime() 
{ 
    return uptime_; 
}