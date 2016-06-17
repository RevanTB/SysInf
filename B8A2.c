#include <pthread.h>
#include <stdio.h>
#include <traffic.h>
#include "map1.h"

pthread_mutex_t go;
criticalPosX = 1;
criticalPosY = 1;

void* thread_func(void* arg) {
    int car;
    if (pthread_self() == 1) {
        car = putCar(0,0,S,0);
    }
    else {
        car = putCar(2,3,W,0);
    }
    printf("Car #%lu just started\n", pthread_self());
    while(1) {
        map_position_t route = routeCar(car);
        if(route.x == criticalPosX && route.y == criticalPosY) {
            pthread_mutex_lock(&go);
            printf("locked by TID %lu\n", pthread_self());
            moveCar(car, route.dir);
            moveCar(car, routeCar(car).dir);
            pthread_mutex_unlock(&go);
        }
        else {
            moveCar(car, route.dir);
        }
    }
    pthread_exit(NULL);
}

int mainfunc(void* arg) {
    pthread_mutex_init(&go, NULL);
    pthread_mutex_unlock(&go);
    pthread_t threads[2];
    int i;
    for (i=0; i<2; i++) {
        pthread_create(&threads[i], NULL, &thread_func, &i);
    }
    for (i=0; i<2; i++) {
        pthread_join(threads[i], NULL);
    }
    waitAndClose();
    pthread_mutex_destroy(&go);
    return 0;
}
/*
map_position_t getRight(int x, int y, directions dir) {
    switch(dir) {
    case N:

    }
    return NULL;
}
*/
int main(int argc, char* argv[]) {
    createMap (map_def , map_width , map_height , mainfunc ) ;
    return 0 ;
}
