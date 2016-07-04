## Overview
OX is an image/file/video server, written by pure C.

## Building
* cd luwak
* ./rebuild

## Run
1. copy "luwak", "conf/luwak.xml", "conf/redis.xml", "conf/rabbit.xml", to your project dir
2. config xml file
3. run with "./luwak"

## Test

## TODO
* support kafka MQ


## Log
+++++++++++++++++++++++++++++++++20160704 更新日志++++++++++++++++++++++++++++++++++++++
* 2.0上线
* 全新架构设计
* 引入日志系统
* 添加rabbitmq支持
* rabbitmq监听可以设置多线程开多个消费者Threads
