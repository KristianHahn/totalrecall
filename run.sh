#!/bin/sh

sizes=( 1 2 4 8 16 32 64 68 70 71 72 73 74 76 100 128 160 192 224 256 320 384 448 512 768 1024 1056 1088 1120 1152 1184 1216 1248 1280 1536 1792 2048 ); #MB

for s in ${sizes[*]}; do

    mbsize=`echo "$s*1024*1024" | bc`;
    set -x; # echo the gcc line
    gcc -std=c99 -Os -DNTRIALS=100 -DBYTES=${mbsize} -DDO_MEMSET -DDO_RAMDISK -DRAMDISK_FILE=\"/mnt/ramdisk/test\"  totalrecall.c -o totalrecall.exe;
    set +x;

    #for real time
    echo $$ >>  /sys/fs/cgroup/cpu/tasks
    echo 0 > /proc/sys/kernel/soft_watchdog 

    ./totalrecall.exe

    echo 1 > /proc/sys/kernel/soft_watchdog
    echo "";

done >& results/ryzen_stats_Os.log;


#
