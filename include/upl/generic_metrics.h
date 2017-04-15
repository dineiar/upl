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
  Name        : generic_metrics.h
  Author      : Dalvan Griebler <dalvangriebler@gmail.com>
  Version     : 1.0 (20/10/2016)
  Copyright   : GNU General Public License
  Description : This file provides the basic functions to measure performance
  				in C/C++ source codes by reading Linux Kernel files  
 ============================================================================
*/

#ifndef GENERIC_MERICS_H_INCLUDED
#define GENERIC_MERICS_H_INCLUDED

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <inttypes.h>
#include <asm/unistd.h>
#include <unistd.h>
#include <sched.h>



/** 
 *  \brief Returns the ID of the calling thread
 */
long UPL_getThreadID();

/** 
 *  \brief Returns the CPU frequency.
 *	It will return '-1' when there is a error.
 *  
 */
unsigned long UPL_getCpuFreq();

/**
 *  \brief Returns the number of cores in the system.
 *	It will return '-1' when there is a error.
 *
 */
ssize_t UPL_getNumOfCores();

/**
 *  \brief Returns the real number of cores in the system without considering Hyper-threading.
 *	It will return '-1' when there is a error.
 *
 */
ssize_t UPL_getRealNumCores();

/**
 *  \brief Returns the number of CPUs (physical sockets) on the system.
 *	It will return '-1' when there is a error.
 */
ssize_t UPL_getNumSockets();

/** 
 *  \brief Returns the ID of the core where a thread is running.
 *	It will return '-1' when there is a error.
 */
ssize_t UPL_getCoreID();

/**
 *  \brief Returns the cache line size in Linux systems.
 *	It will return '-1' when there is a error.
 */
size_t UPL_getCacheLineSize();

/**
 *  \brief Returns the process PID in Linux systems
 *
 */
ssize_t UPL_getProcID();

/**
 *  \brief Returns as string a Linux command output.
 *	It will return 'NULL' when there is a error.
 *
 */
char *UPL_getCommandResult(const char *cmd);

/**
 *  \brief Returns the process memory usage (VmHWM) in kB.
 *	It will return '-1' when there is a error.
 *
 */
ssize_t UPL_getProcMemUsage();

/**
 *  \brief Returns the process memory usage (VmHWM) in kB of a specific pid.
 *	It will return '-1' when there is a error.
 *
 */
ssize_t UPL_getProcMemUsage_pid(size_t mypid);

/**
 *  \brief Returns the voluntary context switches of the current process.
 *	It will return '-1' when there is a error.
 */
ssize_t UPL_getProcVoluntary_ctx_switches();

/**
 *  \brief Returns the non-voluntary context switches of the current process.
 *	It will return '-1' when there is a error.
 *
 */
ssize_t UPL_getProcNonVoluntary_ctx_switches();

/**
 *  \brief Return as string the status of the current process.
 *	It will return 'NULL' when there is a error.
 *
 */
char *UPL_getProcStat();

/**
 *  \brief Returns the last core ID.
 *	It will return '-1' when there is a error.
 *
 */
ssize_t UPL_getLastCoreID();

/**
 *  \brief Returns the total number of threads created by the current process. 
 *	It will return '-1' when there is a error.
 */
ssize_t UPL_getProcTotThreads();

/**
 *  \brief Returns as a string the system memory information. 
 *	It will return 'NULL' when there is a error.
 *
 */
char *UPL_getSysMemInfo();

/**
 *  \brief Initialize base line for cores load monitoring by using the input arguments.
 *	It will return 'NULL' when there is a error.
 *
 */
void *UPL_init_cores_load_monitoring(unsigned long long int *idle, unsigned long long int *total_tick);

/**
 *  \brief Returns a pointer to a list, where each element is the current load (%) of the CPU core (0-N).
 *	It will return NULL when there is a error.
 *  Warning-1: Please, do not forget to call first UPL_init_cores_load_monitoring(...);
 *  Warning-2: Please, let monitoring at least one second to call this function (which gives more precision).
 *
 */
double *UPL_get_cores_load_average(unsigned long long int *idle, unsigned long long int *total_tick);

/**
 *  \brief  Initialize base line for cache-misses monitoring by using the input argument.
 *	It will return '-1' when there is a error.
 *
 */
int UPL_init_cache_miss_monitoring(int *cache_fd);

/**
 *  \brief Returns the cache-miss rate.
 *	It will return '-1' when there is a error.
 *  Warning-1: Please, do not forget to call first UPL_init_cache_miss_monitoring(...);
 *  Warning-2: Please, let monitoring at least some computation between these two function calls.
 *
 */
long long UPL_get_cache_miss(int cache_fd);

/**
 *  \brief  Initialize base line for context-switches monitoring by using the input argument.
 *	It will return '-1' when there is a error.
 *
 */
int UPL_init_ctx_switches_monitoring(int *ctx_fd);

/**
 *  \brief Returns the number of context-switches.
 *	It will return '-1' when there is a error.
 *  Warning-1: Please, do not forget to call first UPL_init_ctx_switches_monitoring(...);
 *  Warning-2: Please, let monitoring at least some computation between these two function calls.
 *
 */
long long UPL_get_ctx_switches(int ctx_fd);

#endif
