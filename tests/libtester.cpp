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
  Name        : libtester.cpp
  Author      : Dalvan Griebler <dalvangriebler@gmail.com>
  Version     : 1.0 (20/10/2016)
  Copyright   : GNU General Public License
  Description : This file test all functions of the UPL library in C++ style  
 ============================================================================
*/


#include <upl.h>
#include <iostream>


void test_all();

int main(int argc, char const *argv[]){

	UPL_print_default_metrics();

	test_all();

	return 0;
}


void test_all(){

	std::cout << "UPL_getThreadID " << UPL_getThreadID() << std::endl;

	std::cout << "UPL_getCpuFreq " << UPL_getCpuFreq() << std::endl;

	std::cout << "UPL_getNumOfCores " << UPL_getNumOfCores() << std::endl;

	std::cout << "UPL_getRealNumCores " << UPL_getRealNumCores() << std::endl;

	std::cout << "UPL_getNumSockets " << UPL_getNumSockets() << std::endl;

	std::cout << "UPL_getCoreID " << UPL_getCoreID() << std::endl;

	std::cout << "UPL_getCacheLineSize " << UPL_getCacheLineSize() << std::endl;

	std::cout << "UPL_getProcID " << UPL_getProcID() << std::endl;


	char buff[256];
	sprintf(buff,"cat /proc/%ld/status",UPL_getProcID());
	char *dat = UPL_getCommandResult(buff);
	std::cout << "==== UPL-BEGIN -> UPL_getProcStatus ====" << std::endl;
	std::cout << dat << std::endl;
	std::cout << "==== UPL-END -> UPL_getProcStatus ====" << std::endl;
	
	dat = UPL_getCommandResult("lscpu");
	std::cout << "==== UPL-BEGIN -> UPL_getlsCPU ====" << std::endl;
	std::cout << dat << std::endl;
	std::cout << "==== UPL-END -> UPL_getlsCPU ====" << std::endl;
	
	dat = UPL_getCommandResult("numactl --hardware");
	std::cout << "==== UPL-BEGIN -> UPL_getNumaHardware ====" << std::endl;
	std::cout << dat << std::endl;
	std::cout << "==== UPL-END -> UPL_getNumaHardware ====" << std::endl;
	

	std::cout << "UPL_getProcMemUsage " << UPL_getProcMemUsage() << std::endl;

	std::cout << "UPL_getProcVoluntary_ctx_switches " << UPL_getProcVoluntary_ctx_switches() << std::endl;

	std::cout << "UPL_getProcNonVoluntary_ctx_switches " << UPL_getProcNonVoluntary_ctx_switches() << std::endl;

	std::cout << "==== UPL-BEGIN -> getProcStat ====" << std::endl;
	std::cout << UPL_getProcStat() << std::endl;
	std::cout << "==== UPL-END -> getProcStat ====" << std::endl;

	std::cout << "UPL_getLastCoreID " << UPL_getLastCoreID() << std::endl;

	std::cout << "UPL_getProcTotThreads " << UPL_getProcTotThreads() << std::endl;

	
	std::cout << "==== UPL-BEGIN -> getSysMemInfo ====" << std::endl;
	std::cout << UPL_getSysMemInfo() << std::endl;
	std::cout << "==== UPL-END -> getSysMemInfo ====" << std::endl;

	ssize_t max_cores = UPL_getNumOfCores();
	
	unsigned long long int *total_ticks =  new unsigned long long int [max_cores];
	unsigned long long int *idle = new unsigned long long int [max_cores];

	if (UPL_init_cores_load_monitoring(idle, total_ticks) == NULL){
		std::cout << "Error when UPL_init_cores_load_monitoring(...)" << std::endl;
	}

	sleep(2);

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

	//Warning: do not forget to run ->	sudo modprobe msr

	int *rapl_fd = new int[4];
	if(UPL_init_count_rapl(rapl_fd) == 0){
		std::cout << "Error when UPL_init_count_rapl(...)" << std::endl;
	}
	
	sleep(2);

	if(UPL_finalize_count_rapl(rapl_fd) == 0){
		std::cout << "Error when UPL_finalize_count_rapl(...)" << std::endl;
	}
	delete rapl_fd;
	
	double package_before=0.0, cores_before=0.0, gpu_before=0.0, dram_before=0.0;
	
	if(UPL_init_count_rapl_msr(&package_before, &cores_before, &gpu_before, &dram_before) == 0){
		std::cout << "Error when UPL_init_count_rapl_msr(...)" << std::endl;
	}
	
	sleep(2);
	
	if(UPL_finalize_count_rapl_msr(package_before, cores_before, gpu_before, dram_before) == 0){
		std::cout << "Error when UPL_finalize_count_rapl_msr(...)" << std::endl;
	}

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