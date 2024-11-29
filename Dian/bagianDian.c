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
    pthread_t thread1, thread2;
    int frig_data[2] = {0, 0}; // frig_data[0] is milk, frig_data[1] is note

    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    // Create two threads to simulate two people trying to buy milk
    pthread_create(&thread1, NULL, buy_milk, frig_data);
    pthread_create(&thread2, NULL, buy_milk, frig_data);

    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
