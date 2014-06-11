/*
 * vector_filterMain.c
 *
 *  Created on: Nov 12, 2013
 *      Author: hxin
 */

#include <string>
#include <sys/times.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emmintrin.h>
#include <iostream>

#define BATCH_RUN 1000000 

using namespace std;

int main(int argc, char* argv[]) {

	tms start_time;
	tms end_time;
	tms elp_time;
	double time_elapsed = 0.0;

	elp_time.tms_stime = 0;
	elp_time.tms_utime = 0;
	elp_time.tms_cstime = 0;
	elp_time.tms_cutime = 0;

		       	elp_time.tms_stime = 0;
			elp_time.tms_utime = 0;
			elp_time.tms_cstime = 0;
			elp_time.tms_cutime = 0;
			
			times(&start_time);

			for(int i = 0; i < 100000000; i++)
			  

		       times(&end_time);

			times(&start_time); 
for(int i = 0; i < 1000000; i++)

			times(&end_time);

		       elp_time.tms_stime += end_time.tms_stime - start_time.tms_stime;
		       elp_time.tms_utime += end_time.tms_utime - start_time.tms_utime;

		       time_elapsed += (double) elp_time.tms_utime / sysconf(_SC_CLK_TCK);
		       
		       cout << time_elapsed << endl;
	return 0;

}
