
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int milk = 0;
int flag[3] = {0,0,0};
int turn = 0;

void* buyMilk(void* arg){
    int id = *(int*)arg;
    flag[id] = 1;
    turn = ( id+1 ) % 3;

    while(flag[( id+1 ) % 3] && turn == ( id+1 ) % 3){
        //busy wait
    }
    int bought = 0;
    if(milk == 0 && !bought){
        milk = 1;
        bought = 1;
        printf("Thread %d (tid: %lu) bought milk +1",id+1, pthread_self());
    }
    
    flag[id] = 0;
    return NULL;
}    

int main(){
    pthread_t thread1, thread2, thread3;
    int thread_id[3] = {0,1,2};

    printf("Jumlah susu di awal: %d\n", milk);
   
    pthread_create(&thread1, NULL, buyMilk, &thread_id[0]);
    pthread_create(&thread2, NULL, buyMilk, &thread_id[1]);
    pthread_create(&thread3, NULL, buyMilk, &thread_id[2]);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    printf("\n");
    printf("Jumlah susu di akhir: %d\n", milk);

    return 0;
}