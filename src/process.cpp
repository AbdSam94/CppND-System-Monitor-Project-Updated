#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::istringstream;
using std::vector;

Process::Process(int Pid):_pid(Pid)
{
    _command = LinuxParser::Command(Pid);
    _user = LinuxParser::User(Pid);
    _ram = LinuxParser::Ram(Pid);
}

// Returns this process's ID
int Process::Pid() 
{ 
    return _pid; 
}

// Returns this process's CPU utilization
float Process::CpuUtilization() 
{ 
    return LinuxParser::ActiveJiffies(_pid); 
}

// Returns the command that generated this process
string Process::Command() 
{ 
    return LinuxParser::Command(this->_pid); 
}

// Returns this process's memory utilization
string Process::Ram() 
{ 
    _ram = LinuxParser::Ram(_pid);
    return _ram; 
}

// Returns the user (name) that generated this process
string Process::User() 
{ 
    return _user;
}

// Returns the age of this process (in seconds)
long int Process::UpTime() 
{ 
    return LinuxParser::UpTime(_pid); 
}

// Overloads the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const 
{ 
    if(this->RamMB(_ram) < a.RamMB(_ram))
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

long Process::RamMB(string ram) 
{
    istringstream streamer(ram);
    streamer >> ram;
    return stol(ram);
}