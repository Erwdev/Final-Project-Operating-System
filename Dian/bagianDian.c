#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;  // Mutex to protect shared resources

void* buy_milk(void* arg) {
    int* frig_data = (int*)arg;
    int* milk = &frig_data[0];
    int* note = &frig_data[1];

    // Lock the mutex to prevent race conditions
    pthread_mutex_lock(&mutex);

    if (*milk == 0) {        // Check if there's no milk
        if (*note == 0) {    // Check if there's no note
            *note = 1;       // Leave a note
            *milk += 1;      // Buy milk
            printf("Milk bought, total milk: %d\n", *milk);
            *note = 0;       // Remove the note
        }
    }

    // Unlock the mutex after modifying shared resources
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t threads[100];  // Create an array to hold thread IDs
    int frig_data[2] = {0, 0}; // frig_data[0] is milk, frig_data[1] is note

    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    // Create 10 threads to simulate 10 people trying to buy milk
    for (int i = 0; i < 100; i++) {
        pthread_create(&threads[i], NULL, buy_milk, frig_data);
    }

    // Wait for all threads to finish
    for (int i = 0; i < 100; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
