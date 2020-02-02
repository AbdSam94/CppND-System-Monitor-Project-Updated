#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>
#include <chrono>
#include <thread>

using std::string;
using std::vector;
using namespace std::this_thread;     
using namespace std::chrono_literals; 
using std::chrono::system_clock;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{ 
    return cpuUti_;
}

Processor::Processor()
{
    long prevIdle, idle, prevTotal, total, totalD, idleD;
    double cpuUti;

    vector<string> prevResult = LinuxParser::CpuUtilization();
    prevIdle = LinuxParser::IdleJiffies();
    prevTotal = LinuxParser::Jiffies();
    sleep_until(system_clock::now() + 2s);
    vector<string> result = LinuxParser::CpuUtilization();
    idle = LinuxParser::IdleJiffies();
    total = LinuxParser::Jiffies();
    totalD = total - prevTotal;
    idleD = idle - prevIdle;
    cpuUti = static_cast<double>(totalD - idleD) / totalD;
    cpuUti_ = cpuUti; 

}