//
// Created by liode on 2023/2/20.
//
/*
 * Usage:
 *      ./block <dev> <block|nonblock>
 */
#include "stdio.h"
#include "fcntl.h"
#include "string.h"
#include "unistd.h"
#include "errno.h"
#include <linux/input.h>
#include "poll.h"
int main(int argc ,char** argv){
    int fd;
    if(argc == 3 && !strcmp(argv[2],"block")){
        fd = open(argv[1],O_RDONLY);
    }else if(argc == 3 && !strcmp(argv[2],"nonblock")) {
        fd = open(argv[1], O_RDONLY | O_NONBLOCK);
    }else{
        printf("Usgae: ./block <dev> <block|noblock>");
        return -1;
    }
    if(fd < 0 ){
        return -1;
    }


    struct input_event event;
    int len;
    int idx =  0;


    //poll
    struct pollfd pfds[1];
    pfds[0].fd = fd;
    pfds[0].events = POLLIN;
    pfds[0].revents = 0;
    int ret = 0;
    while (1){
        ret = poll(pfds , 1 , 5000);
        if(ret >0){
            if(pfds[0].revents == POLLIN){// 监控到了POLLIN
                len  = read(fd,&event,sizeof (event));
                if(len == sizeof (event)){
                    printf("line %d : type = 0x%x, code = 0x%x, value = 0x%x\n", idx++,event.type, event.code, event.value);
                }else{
                    printf("len is %d, errno is %d\n",len,errno);
                }
            }
        }else if(ret == 0 ){
            printf("5s timeout , no data\n");
        }else {
            printf("error ,ret is %d ,errno is %d\n",ret,errno);
        }
    }
}