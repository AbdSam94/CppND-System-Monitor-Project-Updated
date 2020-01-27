#include "ncurses_display.h"
#include "system.h"


#include <iostream>
#include "linux_parser.h"
/*#include <iostream>
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
namespace x{


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
            for(int i = replacements.size() - 1; i >= 0; i--)
            {
              replace(value.begin(), value.end(), replacements[i][1], replacements[i][0]);
            }
            result.push_back(value);
          }
        }
      }
    }
  }
  filestream.close();
  return result;
}
}*/
int main() {
  /*System system;
  NCursesDisplay::Display(system);*/
  /*vector<string> result;
  result = x::parser("/etc/os-release", {{' ', '_'}, {'=', ' '}, {'"', ' '}},{"PRETTY_NAME"});
  std::cout << result[0] << "\n";*/
  std::cout << LinuxParser::UpTime() << "\n";
  std::cout << LinuxParser::MemoryUtilization() << "\n";
  std::cout << LinuxParser::OperatingSystem() << "\n";
  std::cout << LinuxParser::Kernel() << "\n";
  std::cout << LinuxParser::Jiffies() << "\n";
}