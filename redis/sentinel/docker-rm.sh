#!/bin/bash
docker stop redis-sentinel1 redis-sentinel2 redis-sentinel3 redis-slave1 redis-slave2 redis-master
docker rm redis-sentinel1 redis-sentinel2 redis-sentinel3 redis-slave1 redis-slave2 redis-master
