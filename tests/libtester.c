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
  Name        : libtester.c
  Author      : Dalvan Griebler <dalvangriebler@gmail.com>
  Version     : 1.0 (20/10/2016)
  Copyright   : GNU General Public License
  Description : This file test all functions of the UPL library in C style  
 ============================================================================
*/

#include <upl.h>

#include <stdio.h>
#include <stdlib.h>


void test_all();

int main(int argc, char const *argv[]){

	UPL_print_default_metrics();
		
	test_all();

	return 0;
}


void test_all(){

	printf("UPL_getThreadID %ld\n", UPL_getThreadID()); 

	printf("UPL_getCpuFreq %lud\n", UPL_getCpuFreq());

	printf("UPL_getNumOfCores %ld\n", UPL_getNumOfCores());

	printf("UPL_getRealNumCores %ld\n", UPL_getRealNumCores());

	printf("UPL_getNumSockets %ld\n", UPL_getNumSockets());

	printf("UPL_getCoreID %ld\n", UPL_getCoreID());

	printf("UPL_getCacheLineSize %lud\n", UPL_getCacheLineSize()); 

	printf("UPL_getProcID %ld\n", UPL_getProcID());

	char buff[256];
	sprintf(buff,"cat /proc/%ld/status",UPL_getProcID());

	char *dat = UPL_getCommandResult(buff);
	printf("==== UPL-BEGIN -> UPL_getProcStatus ====\n");
	printf("%s\n", dat);
	printf("==== UPL-END -> UPL_getProcStatus ====\n");
	
	dat = UPL_getCommandResult("lscpu");
	printf("==== UPL-BEGIN -> UPL_getlsCPU ====\n");
	printf("%s\n", dat);
	printf("==== UPL-END -> UPL_getlsCPU ====\n");
	
	dat = UPL_getCommandResult("numactl --hardware");
	printf("==== UPL-BEGIN -> UPL_getNumaHardware ====\n");
	printf("%s\n", dat);
	printf("==== UPL-END -> UPL_getNumaHardware ====\n");
	

	printf("UPL_getProcMemUsage %ld\n", UPL_getProcMemUsage());

	printf("UPL_getProcMemUsage_pid %ld\n", UPL_getProcMemUsage_pid(UPL_getProcID()));


	printf("UPL_getProcVoluntary_ctx_switches %ld\n", UPL_getProcVoluntary_ctx_switches());

	printf("UPL_getProcNonVoluntary_ctx_switches %ld\n", UPL_getProcNonVoluntary_ctx_switches());

	printf("==== UPL-BEGIN -> getProcStat ====\n");
	printf("%s\n", UPL_getProcStat());
	printf("==== UPL-END -> getProcStat ====\n");

	printf("UPL_getLastCoreID %ld\n", UPL_getLastCoreID());

	printf("UPL_getProcTotThreads %ld\n", UPL_getProcTotThreads());

	
	printf("==== UPL-BEGIN -> getSysMemInfo ====\n");
	printf("%s\n", UPL_getSysMemInfo());
	printf("==== UPL-END -> getSysMemInfo ====\n");

	ssize_t max_cores = UPL_getNumOfCores();
	
	unsigned long long int *total_ticks = (unsigned long long int *) malloc(sizeof(unsigned long long int) *max_cores);
	unsigned long long int *idle = (unsigned long long int *) malloc(sizeof(unsigned long long int) *max_cores);

	if (UPL_init_cores_load_monitoring(idle, total_ticks) == NULL){
		printf("Error when UPL_init_cores_load_monitoring(...)\n");
	}

	sleep(2);

	double *data = UPL_get_cores_load_average(idle, total_ticks);
	
	if (data != NULL){
		for (int i = 0; i < UPL_getNumOfCores(); ++i){
			printf("	CPU%d Usage: %3.2lf%%\n", i, data[i]);
		}
	}else{
		printf("Error when UPL_get_cores_load_average(...)\n");
	}
	free(idle);
	free(total_ticks);
	free(data);

	//Warning: do not forget to run ->	sudo modprobe msr

	int *rapl_fd = (int *) malloc(sizeof(int) * 4);
	if(UPL_init_count_rapl(rapl_fd) == 0){
		printf("Error when UPL_init_count_rapl(...)\n");
	}
	
	sleep(2);

	if(UPL_finalize_count_rapl(rapl_fd) == 0){
		printf("Error when UPL_finalize_count_rapl(...)\n");
	}
	free(rapl_fd);
	
	double package_before=0.0, cores_before=0.0, gpu_before=0.0, dram_before=0.0;
	
	if(UPL_init_count_rapl_msr(&package_before, &cores_before, &gpu_before, &dram_before) == 0){
		printf("Error when UPL_init_count_rapl_msr(...)\n");
	}
	
	sleep(2);
	
	if(UPL_finalize_count_rapl_msr(package_before, cores_before, gpu_before, dram_before) == 0){
		printf("Error when UPL_finalize_count_rapl_msr(...)\n");
	}

	int fd_cache;
	if(UPL_init_cache_miss_monitoring(&fd_cache) == 0){
		printf("Error when UPL_init_cache_miss_monitoring(...)\n");
	}

	sleep(1);

	long long r_cache = UPL_get_cache_miss(fd_cache);
	if(r_cache < 0){
		printf("Error when UPL_get_cache_miss(...)\n");
	}

	printf("UPLib -> Total cache-miss(KB): %lld\n", r_cache);

	int fd_ctx;
	if(UPL_init_ctx_switches_monitoring(&fd_ctx) == 0){
		printf("Error when UPL_init_ctx_switches_monitoring(...)\n");
	}

	sleep(1);

	long long r_ctx = UPL_get_ctx_switches(fd_ctx);
	if(r_ctx < 0){
		printf("Error when UPL_get_ctx_switches(...)\n");
	}

	printf("UPLib -> Total ctx_switches: %lld\n", r_ctx);
}
