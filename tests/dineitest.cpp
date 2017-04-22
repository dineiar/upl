/* ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as 
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  As a special exception, you may use this file as part of a free software
 *  library without restriction.  Specifically, if other files instantiate
 *  templates or use macros or inline functions from this file, or you compile
 *  this file and link it with other files to produce an executable, this
 *  file does not by itself cause the resulting executable to be covered by
 *  the GNU General Public License.  This exception does not however
 *  invalidate any other reasons why the executable file might be covered by
 *  the GNU General Public License.
 *
 *****************************************************************************
 * 
 *	UPL: This file is part of the Utility Performance Library (UPL)
 * 
 *
 ============================================================================  
  Name        : dineitest.cpp
  Author      : Dinei Rockenbach <dineiar@gmail.com>
  Version     : 0.1 (22/04/2017)
  Copyright   : GNU General Public License
  Description : This is my first C/C++ file. It's a Hello World.
 ============================================================================
*/


#include <upl.h>
#include <iostream>


void print_all(ssize_t pid);

int main(int argc, char const *argv[]){

    if (argc > 1) {
        ssize_t pid = strtol(argv[1], NULL, 10);
	    printf("Output for pid: %ld \n", pid);
        print_all(pid);
    } else {
        std::cout << "Wrong usage, pass the PID as the second parameter" << std::endl;
    }

	return 0;
}

void print_all(ssize_t pid){
    
    printf("UPLib -> Memory usage (KB): %ld \n", UPL_getProcMemUsage_pid(pid));

	char buff[256];
	sprintf(buff,"cat /proc/%ld/status", pid);

	char *dat = UPL_getCommandResult(buff);
	std::cout << "==== UPL-BEGIN -> UPL_getProcStatus ====" << std::endl;
	std::cout << dat << std::endl;
	std::cout << "==== UPL-END -> UPL_getProcStatus ====" << std::endl;

	ssize_t max_cores = UPL_getNumOfCores();
	
	unsigned long long int *total_ticks =  new unsigned long long int [max_cores];
	unsigned long long int *idle = new unsigned long long int [max_cores];

	if (UPL_init_cores_load_monitoring(idle, total_ticks) == NULL){
		std::cout << "Error when UPL_init_cores_load_monitoring(...)" << std::endl;
	}

	sleep(1);

	double *data = UPL_get_cores_load_average(idle, total_ticks);
	
	if (data != NULL){
		for (int i = 0; i < UPL_getNumOfCores(); ++i){
			printf("	CPU%d Usage: %3.2lf%%\n", i, data[i]);
		}
	}else{
		std::cout << "Error when UPL_get_cores_load_average(...)" << std::endl;
	}
	delete idle;
	delete total_ticks;
	delete data;

	int fd_cache;
	if(UPL_init_cache_miss_monitoring(&fd_cache) == 0){
		std::cout << "Error when UPL_init_cache_miss_monitoring(...)" << std::endl;
	}

	sleep(1);

	long long r_cache = UPL_get_cache_miss(fd_cache);
	if(r_cache < 0){
		std::cout << "Error when UPL_get_cache_miss(...)" << std::endl;
	}

	std::cout << "UPLib -> Total cache-miss(KB): " << r_cache << std::endl;

	int fd_ctx;
	if(UPL_init_ctx_switches_monitoring(&fd_ctx) == 0){
		std::cout << "Error when UPL_init_ctx_switches_monitoring(...)" << std::endl;
	}

	sleep(1);

	long long r_ctx = UPL_get_ctx_switches(fd_ctx);
	if(r_ctx < 0){
		std::cout << "Error when UPL_get_ctx_switches(...)" << std::endl;
	}

	std::cout << "UPLib -> Total ctx_switches: " << r_ctx << std::endl;
}
