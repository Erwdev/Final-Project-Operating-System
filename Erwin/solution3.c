#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_THREAD 3
#define NUM_TEST 10000

int milk = 0;
int flag[3] = {0, 0, 0};
int turn = 0;

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
        bought = 1; printf("Thread %d (tid: %lu) bought milk +1",id+1, pthread_self());
    }
    flag[id] = 0;
    
    return NULL;
}    


void driver(pthread_t *threads, int *thread_ids, int num_threads){
    milk = 0;
    for(int i = 0; i < 3; i++){
        flag[i] = 0;
    }
    turn = 0;

    printf("Jumlah susu di awal: %d\n", milk);
    int error_count = 0;
    
        for (int i = 0; i < num_threads; i++)
        {
            thread_ids[i] = i % 3; // buat alternating 0,1,2
            if (pthread_create(&threads[i], NULL, buyMilk, &thread_ids[i]) != 0)
            {
                printf("Failed to create thread %d\n", i);
            }
        }

        for (int i = 0; i < num_threads; i++)
        {
        if(pthread_join(threads[i], NULL) != 0){
            printf("Error: fail to join thread %d\n", i);
            error_count++;
        }
            
        }

        printf("\n");
        printf("Jumlah susu di akhir: %d\n", milk);

    return error_count;
}

int main(){
    pthread_t threads[NUM_THREAD];
    int thread_ids[NUM_THREAD];
    int total_errors = 0;

    for(int j = 0 ; j < NUM_TEST; j++){
         printf("Jumlah susu di awal: %d\n", milk);

    for(int i = 0 ; i < NUM_THREAD; i++){
        thread_ids[i] = i % 3; //buat alternating 0,1,2
        if(pthread_create(&threads[i], NULL, buyMilk, &thread_ids[i]) != 0){
            printf("Failed to create thread %d\n", i);
        }
    }

    for(int i = 0; i < NUM_THREAD; i++){
        pthread_join(threads[i],NULL);
        if(error != 0){
            printf("Something happened between the thread, test no: %d, (error: %d), continuing", j,error)
        }
    }

        printf("\n");
        printf("Jumlah susu di akhir: %d\n", milk_c=du);
    }
`   

    // pthread_create(&thread1, NULL, buyMilk, &thread_id[0]);
    // pthread_create(&thread2, NULL, buyMilk, &thread_id[1]);
    // pthread_create(&thread3, NULL, buyMilk, &thread_id[2]);

    // pthread_join(thread1, NULL);
    // pthread_join(thread2, NULL);
    // pthread_join(thread3, NULL);
    printf("Total error(race condition) detected: %d", total_errors);
    return 0;
}