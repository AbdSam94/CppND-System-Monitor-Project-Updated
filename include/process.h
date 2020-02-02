#ifndef PROCESS_H
#define PROCESS_H

#include <string>
using std::string;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int Pid);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process& a);  // TODO: See src/process.cpp
  static double RamMB(string ram);

  // TODO: Declare any necessary private members
 private:
  int _pid;
  string _command;
  string _user;
  string _ram;
};

#endif