#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SUMSIZE 100
#include <pthread.h>
#include <unistd.h>
#define BUFSIZE 8
static int buffer[BUFSIZE];
static int bufin = 0;
static int bufout = 0;
static pthread_mutex_t
        buffer_lock = PTHREAD_MUTEX_INITIALIZER;
int get_buffersize(){
    return BUFSIZE;
}
void get_item(int *itemp){
    pthread_mutex_lock(&buffer_lock);
    *itemp = buffer[bufout];
    bufout = (bufout + 1) % BUFSIZE;
    pthread_mutex_unlock(&buffer_lock);
    return;
}
void put_item(int item){
    pthread_mutex_lock(&buffer_lock);
    buffer[bufin] = item;
    bufin = (bufin + 1) % BUFSIZE;
    pthread_mutex_unlock(&buffer_lock);
    return;
}
static int sum = 0;
void *producer(void * arg1){
    int i;
    for (i = 1; i <= SUMSIZE; i++)
        put_item(i*i);
    pthread_exit(NULL);
}
void *consumer(void *arg2){
    int  i, myitem;
    for (i = 1; i <= SUMSIZE; i++) {
        get_item(&myitem);
        sum += myitem; }
    pthread_exit(NULL);
}
int main(void){
    pthread_t prodtid, constid;
    int i, total = 0;
    for (i = 1; i <= SUMSIZE; i++)
        total += i*i;
    printf("The actual sum should be %d\n", total);
    if(pthread_create(&constid, NULL, consumer, NULL)){
        perror("Could not create consumer");
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&prodtid, NULL, producer, NULL)){
        perror("Could not create producer");
        exit(EXIT_FAILURE);
    }

    pthread_join(prodtid, NULL);
    pthread_join(constid, NULL);
    printf("The threads produced the sum %d\n", sum);
    exit(EXIT_SUCCESS);
}
