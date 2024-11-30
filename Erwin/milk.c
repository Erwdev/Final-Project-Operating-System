#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <signal.h>
int* lock = 0; //setting the initial fridge to be locked
int* guard = 0;

//lock 0 = busy

typedef struct QueueNode {
    pthread_t thread;
    struct QueueNode* next;
}QueueNode;

QueueNode* wait_queue_head = NULL;
QueueNode* wait_queue_tail = NULL;

void put_thread_on_wait_queue(){
    QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
    new_node->thread = pthread_self();
    new_node->next = NULL;

    if(wait_queue_head == NULL){
        wait_queue_head = wait_queue_tail = new_node;
    }else{
        wait_queue_tail->next = new_node;
        wait_queue_tail = new_node;
    }
}

void take_someone_off_wait_queue(){
    if(wait_queue_head == NULL){
        return;
    }

    QueueNode* to_remove = wait_queue_head;
    wait_queue_head = wait_queue_head->next;
    if(wait_queue_head ==NULL){
        wait_queue_tail = NULL;
    }

    pthread_t thread_to_wake = to_remove-> thread;
    free(to_remove);
    
}

bool anyone_on_wait_queue(){
    return wait_queue_head != NULL;
}

void acquire(){
    while (testSet(guard));
    if(lock == 0){
        put_thread_on_wait_queue();
        while(lock == 0){
            if(anyone_on_wait_queue()){
                take_someone_off_wait_queue();
            }else{
                lock = 1;
            }
        }
    }
    guard = 0;
}

void release(){
    while(testSet(guard));
    if(anyone_on_wait_queue()){
        take_someone_off_wait_queue();
    }else{
        lock = 1;
    }guard= 0;
}
bool testSet(int *lock){
    bool old_value = *lock;
    *lock = 1;
    return old_value;
}
buyMilk(void *arg){
    int *fridge_data = (int *)arg;
    int *milk = &fridge_data[0];

    acquire();
    if(*milk == 0){
        *milk += 1;
        printf("Thread: %d is buying +1 milk", pthread_self());
    }
    release();
}
int main(){
    pthread_t thread1, thread2, thread3;
    int fridge_data[2] = {0,0};
    
    printf("Initial milk in fridge: %d\n", fridge_data[0]);

    pthread_create(&thread1, NULL,buyMilk, fridge_data );
    pthread_create(&thread2, NULL,buyMilk, fridge_data );
    pthread_create(&thread3, NULL,buyMilk, fridge_data );

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    return 0;
}