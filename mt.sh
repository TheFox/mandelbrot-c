#!/usr/bin/env bash

BIN_MT=./build_$(uname -s)/bin/mandelbrot_mt
BIN_CI=./build_$(uname -s)/bin/mandelbrot_ci

set +x

P_WIDTH=1024
P_HEIGHT=1024

DEPTH_MIN=1
DEPTH_MAX=100

MB_WIDTH_MID=-.5
MB_WIDTH_ZOOM=2
MB_HEIGHT_MID=0
MB_HEIGHT_MAX=2

THREAD_MAX=8


for((thread_id = 1; thread_id <= THREAD_MAX; thread_id++)); do
	echo "create thread $thread_id"
	log=logs/mbs_$thread_id.log
	$BIN_MT $P_WIDTH $P_HEIGHT $DEPTH_MIN $DEPTH_MAX $MB_WIDTH_MID $MB_WIDTH_ZOOM $MB_HEIGHT_MID $MB_HEIGHT_MAX $thread_id $THREAD_MAX 1>> $log 2>> $log &
done

for job in $(jobs -p); do
	echo "wait for $job"
    wait $job
done

$BIN_CI $P_WIDTH $P_HEIGHT $DEPTH_MIN $DEPTH_MAX $MB_WIDTH_MID $MB_WIDTH_ZOOM $MB_HEIGHT_MID $MB_HEIGHT_MAX

echo "done"
