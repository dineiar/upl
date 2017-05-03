#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <unistd.h>

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

using namespace std;

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != NULL)
            result += buffer.data();
    }
    return result;
}

class Monitor {
    public:
        ofstream fstream;
        string file;
      
        Monitor();
        // Monitor(string filename);

        void init(string filename);
        void close();
        void writeCmdResult(string cmd, string preffix);
};

Monitor::Monitor() { };
void Monitor::init(string filename) {
    file = filename;
    fstream.open(filename);
};

void Monitor::writeCmdResult(string cmd, string preffix) {
    fstream << preffix << exec(cmd.c_str());
};

void Monitor::close() {
    fstream.close();
};


class Monitoring {
    public:
        bool redis, riak;
        Monitor mon_iostat, mon_free, mon_df, mon_redis, mon_riak;

        //Init system monitoring
        Monitoring();
        Monitoring(string out_folder);
        
        void init(string out_folder);
        void detect();
        void close();
};

Monitoring::Monitoring() { };
Monitoring::Monitoring(string out_folder) {
    redis = false;
    riak = false;
    mon_iostat.init(out_folder + "iostat.txt");
    mon_free.init(out_folder + "free.txt");
    mon_df.init(out_folder + "df.txt");
    
    detect();
    if (redis) {
        mon_redis.init(out_folder + "redis.txt");
    }
    if (riak) {
        mon_riak.init(out_folder + "riak.txt");
    }
};

//Detect running databases
void Monitoring::detect() {
    //Redis
    redis = false;
    std::string cmd = "redis-cli ping";
    std::string expected = "PONG\n";
    if (exec(cmd.c_str()) == expected) {
        redis = true;
    }

    //Riak
    riak = false;
    cmd = "sudo riak ping";
    expected = "pong\n";
    if (exec(cmd.c_str()) == expected) {
        riak = true;
    }
};

void Monitoring::close() {
    mon_iostat.close();
    mon_free.close();
    mon_df.close();
    mon_redis.close();
    mon_riak.close();
}

int main() {

    std::string folder = "/tmp/monitoring/";
    Monitoring monitor(folder);

    cout << "Redis: " << monitor.redis << endl;
    cout << "Riak: " << monitor.riak << endl;
    
    std::string cmd = "";

    int c = 0;
    while (c++ < 10) {
        //performance de disco (I/O)
        //CPU (geral)
        //Memória
        //Detectar SWAP 

        string preffix = to_string(time(nullptr)) + "\t";

        //Get CPU and disk usage through iostat
        cmd = "iostat -c -d -m -x -y";
        //avg-cpu:  %user   %nice %system %iowait  %steal   %idle
        //Device:  rrqm/s  wrqm/s  r/s  w/s  rMB/s  wMB/s  avgrq-sz  avgqu-sz  await  r_await  w_await  svctm  %util
        monitor.mon_iostat.writeCmdResult(cmd, preffix);

        //Get memory usage through free
        cmd = "free -m -t";
        // total  used  free  shared  buff/cache  available
        // Mem, Swap, Total
        monitor.mon_free.writeCmdResult(cmd, preffix);

        //Get disk usage through df
        cmd = "df -T -l";
        // Filesystem  Type  Size  Used  Avail  Use%  Mounted on
        monitor.mon_df.writeCmdResult(cmd, preffix);

        //Get Redis info
        if (monitor.redis) {
            cmd = "redis-cli info all";
            //https://redis.io/commands/info
            monitor.mon_redis.writeCmdResult(cmd, preffix);
        }

        //Get Riak info
        if (monitor.riak) {
            cmd = "sudo riak-admin status";
            //https://redis.io/commands/info
            monitor.mon_riak.writeCmdResult(cmd, preffix);
        }

        sleep(1);
    }

    monitor.close();

    return 0;
}
