#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* buyMilk(void* arg){
    int* frig_data = (int*)arg;
    int* milk = &frig_data[0];
    int* noteA = &frig_data[1];
    int* noteB = &frig_data[2];
    int* noteC = &frig_data[3];

    *noteA = 1;

    while(*noteB || *noteC){};
    if(*milk == 0){
        *milk +=1;
        printf("Thread %d membeli susu", pthread_self());
    }
    *noteA = 0;

    return NULL;
}

void* buyMilkB(void* arg){
    int* frig_data = (int*)arg;
    int* milk = &frig_data[0];
    int* noteA = &frig_data[1];
    int* noteB = &frig_data[2];
    int* noteC = &frig_data[3];

    *noteB = 1;
    
    if(noteA == 0){
        if(*milk = 0){
            *milk += 1;
            printf("This thread: %d is adding 1 milk", pthread_self());

        }
    }
    *noteB = 0;

    return NULL;
}

void* buyMilkC(void* arg){
    int* frig_data = (int*)arg;
    int* milk = &frig_data[0];
    int* noteA = &frig_data[1];
    int* noteb = &frig_data[2];
    int* noteC = &frig_data[3];

    //some sort of algorithm that allow this and the other to maintain its exclusivity

    if(*milk == 0){
        *milk += 1;
        printf("Thread: %d is adding a milk +1", pthread_self());

    }

    return NULL;
}
int main(){
    pthread_t thread1, thread2, thread3;
    int fridge_data[3] = {0,0,0,0};

    printf("Jumlah susu di awal: %d\n", fridge_data[0]);
   
    pthread_create(&thread1, NULL, buyMilk, fridge_data);
    pthread_create(&thread2, NULL, buyMilkB, fridge_data);
    pthread_create(&thread3, NULL, buyMilkC, fridge_data);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    printf("\n");
    printf("Jumlah susu di akhir: %d\n", fridge_data[0]);


    return 0;
}