############################################
#File Name: build.sh
#Brief:
#Author: frank
#Email: frank0903@aliyun.com
#Created Time:2018-08-23 22:50:06
#Blog: http://www.cnblogs.com/black-mamba
#Github: https://github.com/suonikeyinsuxiao
############################################
#!/bin/bash

gcc main.c xmg_mutex.c ../../pthread/api/xmg_thread.c -I ../../pthread/api -lpthread -Wall -O2 -o music_player 

