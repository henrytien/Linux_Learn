/*************************************************************************
	> File Name: sem_main.c
	> Author:henryberlin 
	> Mail:henrytien@yahoo.com 
	> Created Time: Sun 05 Feb 2017 06:57:28 PM CST
 ************************************************************************/
#include"func.h"
#define N 1000000

int main(){


    int shmid;
    shmid=shmget(1234,4096,IPC_CREAT|0600);
    if(-1==shmid){
        perror("shmget");
        return -1;
    }
    
    // create semid 
    int semid;
    semid=semget(1234,1,IPC_CREAT|0600);
    if(-1==semid){
        perror("semget");
        return -1;
    }
    int ret; 
    ret=semctl(semid,0,SETVAL,1);
    if(-1==ret)
    {
        perror("semctl");
        return -1;
    }

    if(fork()==0){ // child progress
        int *p;
        p=(int *)shmat(shmid,NULL,0);
                  if((int*)-1==p){
                      perror("shmat");
                      return -1;
                  }
        int i;
         struct sembuf Sem_P,Sem_V;
        memset(&Sem_P,0,sizeof(struct sembuf));
        memset(&Sem_V,0,sizeof(struct sembuf));
       Sem_P.sem_num=0;
        Sem_P.sem_op=-1;
        Sem_P.sem_flg=SEM_UNDO;
        Sem_V.sem_num=0;
        Sem_V.sem_op=1;
        Sem_V.sem_flg=SEM_UNDO;
                  for(i=0;i<N;i++){
                      ret=semop(semid,&Sem_P,1);
                      if(-1==semop){
                          perror("semop");
                          return -1;
                      }
                    *p=*p+1;
                      semop(semid,&Sem_V,1);
                  }
                  return 0;
    }else{ // parent progress
          int *p;
          p=(int *)shmat(shmid,NULL,0);
          if((int *)-1==p){
              perror("shmat");
              return -1;
          }

          int i;
          struct sembuf sp,sv;
          memset(&sp,0,sizeof(sp));
          memset(&sv,0,sizeof(sv));
          sp.sem_num=0;
          sp.sem_op=-1;
          sp.sem_flg=SEM_UNDO;
          sv.sem_num=0;
          sv.sem_op=1;
          sv.sem_flg=SEM_UNDO;
          for(i=0;i<N;i++){
              ret=semop(semid,&sp,1);
              if(-1==ret)
              {
                  perror("semop");
                  return -1;
              }
              *p=*p+1;
              semop(semid,&sv,1);
          }
          wait(NULL);
          printf("the value is %d\n",*p);
          return 0;
    }
    return 0;
}
