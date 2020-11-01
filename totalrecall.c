#define _GNU_SOURCE
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#include <signal.h>
#include <sched.h>

#include <arpa/inet.h>
#include <sys/socket.h>


#include <sys/time.h>
#include <sched.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/mman.h>

//#define NTRIALS 100
//#define BYTES   72*1024*1024 //+16 +17;//*1024
  

uint64_t gltime() {
 struct timeval tv;
  
 gettimeofday(&tv,0);
 return (uint64_t)(tv.tv_sec)*1000000+(uint64_t)tv.tv_usec;   
}

int main () {

  // run this in temimal first else EPERM error
  // echo $$ >>  /sys/fs/cgroup/cpu/tasks
#ifdef RT
  struct sched_param sp = { .sched_priority = 99 };
  int ret = sched_setscheduler(0, SCHED_FIFO, &sp);
  if (ret == -1) {
    perror("sched_setscheduler");
    return 1;
  }
#endif
  
  cpu_set_t my_set;        /* Define your cpu_set bit mask. */
  CPU_ZERO(&my_set);       /* Initialize it all to 0, i.e. no CPUs selected. */
  CPU_SET(13, &my_set);     /* set the bit that represents core 13. */
  sched_setaffinity(0, sizeof(cpu_set_t), &my_set); /* Set affinity of this process to */
                                                    /* the defined mask, i.e. only 7. */


  // using specific version of memcpy, makes no difference 
  //  __asm__(".symver memcpy,memcpy@GLIBC_2.2.5");
  //  __asm__(".symver memcpy,memcpy@GLIBC_2.14");
  
#ifndef FOREVER
  uint64_t t_start[NTRIALS], t_stop[NTRIALS];
#endif
  uint64_t diff;
  double total_gbytes, total_seconds   ;
  char *mem_handle_src, *mem_handle_dest;

  printf("BYTES: %llu (%.1f MB, %1.f KB)\n",
	 BYTES,
	 (double)BYTES/(1024*1024),
	 (double)BYTES/1024);

  int dummy_val=0;
#ifdef FOREVER  
  while(1) {
#else
    for( int i=0; i<NTRIALS; i++ ) {
#endif
    mem_handle_src  = malloc(BYTES); //volatile
    mem_handle_dest = malloc(BYTES);  

#ifndef FOREVER
    memset(mem_handle_src, 0xab*i, BYTES);
    memset(mem_handle_dest, 0xfa*i*i, BYTES);
#else
    memset(mem_handle_src, 0xab, BYTES);
    memset(mem_handle_dest, 0xfa, BYTES);
#endif    

#ifdef DROP_CACHE
    FILE * cache_file = fopen("/proc/sys/vm/drop_caches", "wr");
    fprintf(cache_file, "3\n");
    fclose(cache_file);
#endif
    
#ifndef FOREVER    
    t_start[i]=gltime();
    memcpy(mem_handle_dest, mem_handle_src, BYTES);
    t_stop[i]=gltime();
#else
    memcpy(mem_handle_dest, mem_handle_src, BYTES);
#endif

    // prevent optimizing away /w O1 & higher...
    dummy_val = mem_handle_dest[0]; 

    free(mem_handle_src);
    free(mem_handle_dest);
  }
#ifndef FOREVER
  diff=0;
  for( int i=0; i<NTRIALS; i++ ) 
    diff += (t_stop[i] - t_start[i]);
  total_gbytes = (double)BYTES*NTRIALS/(1024*1024*1024); // GB
  total_seconds = ((double)diff)/1000000;
  printf("memcpy %.3f GB in %.3f s, rate: %f bufsize: %.1f\n",
	 total_gbytes,total_seconds,
	 total_gbytes/total_seconds,
	 (double)BYTES/(1024*1024));
#endif


#ifdef DO_MEMSET
  //----------------------------------------------------------------------------------
  for( int i=0; i<NTRIALS; i++ ) {
    mem_handle_src  = malloc(BYTES); //volatile
    mem_handle_dest = malloc(BYTES);
    
    t_start[i]=gltime();    
    memset(mem_handle_src, 0xef-i, BYTES);
    memset(mem_handle_dest, 0xab+i, BYTES);  
    t_stop[i]=gltime();

    // prevent optimizing away /w O1 & higher...
    dummy_val += mem_handle_src[0];
    dummy_val += mem_handle_dest[0];    
    
    free(mem_handle_src);
    free(mem_handle_dest);
  }
  diff=0;
  for( int i=0; i<NTRIALS; i++ ) 
    diff += (t_stop[i] - t_start[i]);  
  total_gbytes = (double)BYTES*NTRIALS*2/(1024*1024*1024); // GB
  total_seconds = ((double)diff)/1000000;
  printf("memset %.3f GB in %.3f s, rate: %f bufsize: %.1f\n",
	 total_gbytes,total_seconds,
	 total_gbytes/total_seconds,
	 (double)BYTES/(1024*1024));	 
  //----------------------------------------------------------------------------------
#endif


  

#ifdef DO_RAMDISK
  //----------------------------------------------------------------------------------
  FILE* file_handle; 
  for( int i=0; i<NTRIALS; i++ ) {
    int fd = open(RAMDISK_FILE,  O_CREAT | O_WRONLY | O_NOATIME , 00666); //| O_SYNC
    file_handle = fdopen(fd, "wb");    
    mem_handle_src  = malloc(BYTES); //volatile
    memset(mem_handle_src, 0xba-i, BYTES);
    
    t_start[i]=gltime();
    fwrite(mem_handle_src, BYTES, 1, file_handle);
    t_stop[i]=gltime();

    free(mem_handle_src);    
    fclose( file_handle );
    close(fd);
  }

  diff=0;
  for( int i=0; i<NTRIALS; i++ ) {
    diff += (t_stop[i] - t_start[i]);
  }
  total_gbytes = (double)BYTES*NTRIALS/(1024*1024*1024); // GB
  total_seconds = ((double)diff)/1000000;
  printf("fwrite %.3f GB in %.3f s, rate: %f bufsize: %.1f\n",
	 total_gbytes,total_seconds,
	 total_gbytes/total_seconds,
	 (double)BYTES/(1024*1024));	 
  //----------------------------------------------------------------------------------
#endif

  return dummy_val;
}
