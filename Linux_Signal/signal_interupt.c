/*************************************************************************
	> File Name: signal_interupt.c
	> Author:henryberlin 
	> Mail:henrytien@yahoo.com 
	> Created Time: Sun 05 Feb 2017 05:16:47 PM CST
 ************************************************************************/
#include<stdio.h>
#include<signal.h>

void ouch(int sig){
    printf("signal :%d\n",sig);
    printf("signal :%d\n",sig);
}

int main(){
    if(SIG_ERR==signal(SIGINT,ouch)){
        perror("SIG_ERR");
        return -1;
    }
    signal(SIGQUIT,ouch);
    while(1);
    return 0;
}
