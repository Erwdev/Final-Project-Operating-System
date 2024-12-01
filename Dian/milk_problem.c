#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdatomic.h>
#include <time.h>

typedef struct {
    atomic_int milk;
    atomic_int note;
    atomic_int buy_attempts;
    atomic_int successful_buys;
} FridgeData;

void* buy_milk(void* arg) {
    FridgeData* fridge = (FridgeData*)arg;
    struct timespec delay = {0, rand() % 10000};  // Random delay between 0-10 microseconds

    int expected_milk = 0;
    int expected_note = 0;

    if (atomic_compare_exchange_strong(&fridge->milk, &expected_milk, 1) &&
        atomic_compare_exchange_strong(&fridge->note, &expected_note, 1)) {
        
        nanosleep(&delay, NULL);
        
        atomic_fetch_add(&fridge->successful_buys, 1);
        atomic_store(&fridge->note, 0);
        
        printf("Milk bought, total milk: %d\n", atomic_load(&fridge->milk));
    }
    
    atomic_fetch_add(&fridge->buy_attempts, 1);
    return NULL;
}

int main() {
    #define NUM_THREADS 1000
    #define NUM_ITERATIONS 1000000

    srand(time(NULL));  // Initialize random seed

    FridgeData fridge = {0};
    pthread_t threads[NUM_THREADS];
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, buy_milk, &fridge);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Total buy attempts: %d\n", atomic_load(&fridge.buy_attempts));
    printf("Successful buys: %d\n", atomic_load(&fridge.successful_buys));
    printf("Final milk count: %d\n", atomic_load(&fridge.milk));
    
    return 0;
}
