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
 ****************************************************************************
 * 
 *	UPL: This file is part of the Utility Performance Library (UPL)
 * 
 *
 ============================================================================ 
  Name        : energy_metrics.h
  Author      : Dalvan Griebler <dalvangriebler@gmail.com>
  Version     : 1.0 (20/10/2016)
  Copyright   : GNU General Public License
  Description : This file provides the basic functions to measure energy consumption
  				in C/C++ source codes by reading Linux Kernel energy counters  
 ============================================================================
*/


#ifndef ENERGY_METRICS_H_INCLUDED
#define ENERGY_METRICS_H_INCLUDED

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

#define MSR_RAPL_POWER_UNIT		0x606
/*
 * Platform specific RAPL Domains.
 * Note that PP1 RAPL Domain is supported on 062A only
 * And DRAM RAPL Domain is supported on 062D only
 */
/* Package RAPL Domain */
#define MSR_PKG_RAPL_POWER_LIMIT	0x610
#define MSR_PKG_ENERGY_STATUS		0x611
#define MSR_PKG_PERF_STATUS		0x613
#define MSR_PKG_POWER_INFO		0x614

/* PP0 RAPL Domain */
#define MSR_PP0_POWER_LIMIT		0x638
#define MSR_PP0_ENERGY_STATUS		0x639
#define MSR_PP0_POLICY			0x63A
#define MSR_PP0_PERF_STATUS		0x63B

/* PP1 RAPL Domain, may reflect to uncore devices */
#define MSR_PP1_POWER_LIMIT		0x640
#define MSR_PP1_ENERGY_STATUS		0x641
#define MSR_PP1_POLICY			0x642

/* DRAM RAPL Domain */
#define MSR_DRAM_POWER_LIMIT		0x618
#define MSR_DRAM_ENERGY_STATUS		0x619
#define MSR_DRAM_PERF_STATUS		0x61B
#define MSR_DRAM_POWER_INFO		0x61C

/* RAPL UNIT BITMASK */
#define POWER_UNIT_OFFSET	0
#define POWER_UNIT_MASK		0x0F

#define ENERGY_UNIT_OFFSET	0x08
#define ENERGY_UNIT_MASK	0x1F00

#define TIME_UNIT_OFFSET	0x10
#define TIME_UNIT_MASK		0xF000

/**
 *  \brief This function initialize the base line for measuring energy counters by updating the input arguments.
 *	It will return '1' when everything runs OK and '0' when there is a error.
 *	Warning: the following variable should be passed by reference to be used in the 'UPL_finalize_count_rapl_msr' function: 
 *	 package_before
 *	 cores_before
 *	 gpu_before
 *	 dram_before
 *  Use case example:
 * 		UPL_init_count_rapl_msr(&package_before, &cores_before, &gpu_before, &dram_before)
 */
int UPL_init_count_rapl_msr(double *package_before, double *cores_before, double *gpu_before, double *dram_before);

/**
 *  \brief This function finalize the measuring of energy counters and calculate the energy consumption by using the input arguments.
 *	It will return '1' when everything runs OK and '0' when there is a error.
 *
 */
int UPL_finalize_count_rapl_msr(double package_before, double cores_before, double gpu_before, double dram_before);

/**
 *  \brief This function initialize the base line for measuring energy counters by updating the input argument.
 *	It will return '1' when everything runs OK and '0' when there is a error.
 *	Warning: the following 'rapl_fd' variable should be allocated before call this function such as:
 *  int *rapl_fd = (int *) malloc(sizeof(int) * 4);
 */
int UPL_init_count_rapl(int *rapl_fd);


/**
 *  \brief This function finalize the measuring of energy counters and calculate the energy consumption by using the input argument.
 *	It will return '1' when everything runs OK and '0' when there is a error.
 *
 */
int UPL_finalize_count_rapl(int *rapl_fd);


#endif
