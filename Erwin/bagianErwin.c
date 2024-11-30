#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void buy_milk1(void *arg){
    int *fridge_data = (int *)arg;
    int *milk = &fridge_data[0];
    int *note = &fridge_data[1];//index 1 isinya note 

    if(*milk == 0){
        if(*note == 0){
            *note = 1;
            //entering critical zone

            *milk += 1;
            printf("Milk bought by thread: %d total milk: %d", pthread_self(), *milk);
            
            //exiting critical zone

            *note = 1;
        }
    }

    return NULL;
}

void buy_milk2(void *arg){
    int *fridge_data = (int *)arg;
    int *milk = &fridge_data[0];
    int *note = &fridge_data[1];//index 1 isinya note 

    if(*milk == 0){
        if(*note == 0){
            *note = 1;
            //entering critical zone

            *milk += 1;
            printf("Milk bought by thread: %d total milk: %d", pthread_self(), *milk);
            
            //exiting critical zone

            *note = 1;
        }
    }

    return NULL;
}

void buy_milk3(void *arg){
    int *fridge_data = (int *)arg;
    int *milk = &fridge_data[0];
    int *note = &fridge_data[1];//index 1 isinya note 

    if(*milk == 0){
        if(*note == 0){
            *note = 1;
            //entering critical zone

            *milk += 1;
            printf("Milk bought by thread: %d total milk: %d", pthread_self(), *milk);
            
            //exiting critical zone

            *note = 1;
        }
    }

    return NULL;
}

int main(){
    pthread_t thread1, thread2, thread3;
    //shared data
    int fridge_data[2] = {0,0};

    pthread_create(&thread1, NULL, buy_milk1, fridge_data);
    pthread_create(&thread2, NULL, buy_milk2, fridge_data);
    pthread_create(&thread3, NULL, buy_milk3, fridge_data);

    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    pthread_join(thread3,NULL);

    return 0;
}
