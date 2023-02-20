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
#include "signal.h"
int fd;
int idx =  0;
void sigio_handler(int sig){
    printf("in %s , idx is %d\n",__FUNCTION__ ,idx++);
    struct input_event event;
    int len;
    while( sizeof(event) == read(fd,&event,sizeof (event)) ){
            printf("line %d : type = 0x%x, code = 0x%x, value = 0x%x\n", idx++,event.type, event.code, event.value);
    }
}
int main(int argc ,char** argv){

    //check and open fd
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



    //fasync

    signal(SIGIO,sigio_handler);

    fcntl(fd,F_SETOWN,getpid());

    unsigned int old_flags = fcntl(fd,F_GETFL);
    fcntl(fd,F_SETFL,old_flags|FASYNC);

    while (1);
}