#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int milk = 0;
int flag[3] = {0, 0, 0};
int turn = 0;

struct ThreadArgs {
    int id;
    int *thread1Count;
    int *thread2Count;
    int *thread3Count;
};

void *buyMilk(void *arg)
{
    struct ThreadArgs *args = (struct ThreadArgs *)arg;
    int id = args->id;
    flag[id] = 1;
    turn = (id + 1) % 3;

    while (flag[(id + 1) % 3] && turn == (id + 1) % 3)
    {
        // busy wait
    }
    int bought = 0;
    if (milk == 0 && !bought)
    {
        milk = 1;
        bought = 1;
        printf("Thread %d (tid: %lu) bought milk +1", id + 1, pthread_self());
        if(id == 0){
            (*args->thread1Count)++;
        }else if(id == 1){
            (*args->thread2Count)++;
        }else{
            (*args->thread3Count)++;
        }
    }

    flag[id] = 0;
    return NULL;
}
#define NUM_TEST 10000
int main()
{   
    int thread1Count = 0;
    int thread2Count = 0;
    int thread3Count = 0;

    pthread_t thread1, thread2, thread3;
    struct ThreadArgs thread_args[3] = {
        {0, &thread1Count, &thread2Count, &thread3Count},
        {1, &thread1Count, &thread2Count, &thread3Count},
        {2, &thread1Count, &thread2Count, &thread3Count}
    };
    int race_condition_count = 0;

    for (int i = 0; i < NUM_TEST; i++)
    {
        printf("\nTest #%d:\n", i + 1);
        milk = 0;
        flag[0] = flag[1] = flag[2] = 0;
        turn = 0;

        printf("Jumlah susu di awal: %d\n", milk);

        pthread_create(&thread1, NULL, buyMilk, &thread_args[0]);
        pthread_create(&thread2, NULL, buyMilk, &thread_args[1]);
        pthread_create(&thread3, NULL, buyMilk, &thread_args[2]);

        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);

        if (milk > 1)
        {
            printf("Race condition detected!, milk: %d\n", milk);
            race_condition_count++;
        }

        printf("\n");
        printf("Jumlah susu di akhir: %d\n", milk);
    }

    printf("\nTotal race condition errors detected: %d\n", race_condition_count);
    printf("Thread1 bought milk:%d times\n", thread1Count);
    printf("Thread2 bought milk:%d times\n", thread2Count);
    printf("Thread3 bought milk:%d times\n", thread3Count);

    return 0;
}