#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

// void* buy_milk1(void *arg){
//     int *fridge_data = (int *)arg;
//     int *milk = &fridge_data[0];
//     int *noteA = &fridge_data[1];//index 1 isinya note 
//     int *noteB = &fridge_data[2];
    

//     *noteA = 1;
//     while(*noteB){
//         //do nothing busy waiting here
//     }//rentan terhadap priority inversion dan juga inf loop
//     if(*milk == 0){   
//         //entering critical zone
//         *milk += 1;
//         printf("Milk bought by thread: %d total milk: %d", pthread_self(),*milk);  
//         //exiting critical zone   
//     }
//     *noteA = 1;

//     return NULL;
// }

int lock = 0; //indikasi kulkas lagi gak dikunci

bool testSet(int *lock) {
    bool old_value = *lock;
    *lock = 1;
    return old_value;
}//using this function naively will still expose this part to the switching done by the operating system

//bakal input nilai awal dari fridge lock bit

void* buyMilkLock1(void *arg){
    int *fridge_data = (int *)arg;
    int *milk = &fridge_data[0];

    while (testSet(&lock)){
        //busy waiting here might use another method like breaking a loop 
    }
    //critical segment
    if(*milk == 0){
        *milk += 1;
        printf("Buying milk from: %d, now there are: %d\n", pthread_self(), *milk);
    }
    //critical segment
    lock = 0;
    return NULL;
}

int main(){
    int THREAD_NUM = 3;
    pthread_t thread1, thread2, thread3;
    //shared data
    int fridge_data[2] = {0,0};
    //index 0 milknya, index 1 note a 

    printf("Initial milk in fridge: %d\n", fridge_data[0]);

    
    pthread_create(&thread1, NULL, buyMilkLock1, fridge_data);
    pthread_create(&thread2, NULL, buyMilkLock1, fridge_data);
    pthread_create(&thread3, NULL, buyMilkLock1, fridge_data);

    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    pthread_join(thread3,NULL);
    
    // pthread_join(thread2,NULL);
    // pthread_join(thread3,NULL);

    return 0;
}

/**
 * TODO : design and implement a locking algorithm similiar to solution 3 limit the locking algorithm using atomic operation like test and set or other method and also value assignment dan conditional
 */

/**
 * Pseudo code 
 * 
 * while(test and set(&lock)); trapping the other thread here busy waiting
 * 
 * critical section
 * 
 * lock false
 */


