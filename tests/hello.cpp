#include <upl.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

int main() {

  std::string folder = "/tmp/";
  std::ofstream fsCPU(folder + "CPU.txt");
  std::ofstream fsDisk(folder + "Disk.txt");

  int c = 0;
  while (c++ < 5) {

    //performance de disco (I/O)
    //CPU (geral)
    //Memória
    //Detectar SWAP 
    std::string cmd = "";
    int timestamp = std::time(nullptr);

    //Get CPU usage through iostat
    cmd = "iostat -c";
    //avg-cpu:  %user   %nice %system %iowait  %steal   %idle
    fsCPU << std::to_string(timestamp) << "\t" << UPL_getCommandResult(cmd.c_str());
    
    //Get disk usage through iostat
    cmd = "iostat -d -m -x -y";
    //Device:  rrqm/s  wrqm/s  r/s  w/s  rMB/s  wMB/s  avgrq-sz  avgqu-sz  await  r_await  w_await  svctm  %util
    fsDisk << std::to_string(timestamp) << "\t" << UPL_getCommandResult(cmd.c_str());

    sleep(1);
  }


  fsCPU.close();
  fsDisk.close();

  return 0;
}
