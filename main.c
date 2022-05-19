#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void *producer(void *id);
void *consumer(void *id);

sem_t empty, full, mutex;

int *buffer;
int limit;
int prod, con, buffer_size;

int main(void) {

        sem_init(&empty, 0, 1);
        sem_init(&full, 0, 0);
        sem_init(&mutex, 0, 1);


        scanf("%d %d %d %d", &prod, &con, &limit, &buffer_size);

        buffer = malloc(sizeof(int) * buffer_size);

        pthread_t producers[prod];
        pthread_t consumers[con];

        int id_thread_p[prod];
        int id_thread_c[con];

        for (int i = 0; i < prod; i++) {

                id_thread_p[i] = i;

                if (pthread_create(&(producers[i]), NULL, &producer, &id_thread_p[i]) != 0) {

                        printf("failed to create producer thread\n");
                }
        }

        for (int i = 0; i < con; i++) {

                id_thread_c[i] = i;

                if (pthread_create(&(consumers[i]), NULL, &consumer, &id_thread_c[i]) != 0) {
                        printf("failed to create consumer thread\n");
                }
        }

        for (int i = 0; i < prod; i++) {
                pthread_join(producers[i], NULL);
        }

        for (int i = 0; i < con; i++) {
                pthread_join(consumers[i], NULL);
        }
        
        return 0;
}

int index_p = 0;
int x = 0;

void *producer(void *id_thread) {

        int* id = (int *) id_thread;

        while (1) {

                sem_wait(&empty);
                sem_wait(&mutex);

                if (buffer[index_p] == 0) {
                        buffer[index_p] = (2 * x) + 1;
                        printf("Produtor %d produzindo %d na posição %d\n", *id, buffer[index_p], index_p);
                        x++;

                        if (x > limit) {
                                x = 0;
                        }
                }
                index_p++;

                if (index_p >= buffer_size) {
                        index_p = 0;
                }

                sem_post(&mutex);
                sem_post(&full);
        }
}

int index_c = 0;

void *consumer(void *id_thread) {
        
        int* id = (int *) id_thread;

        while (1) {

                sem_wait(&full);
                sem_wait(&mutex);

                if (buffer[index_c] != 0) {
                        printf("Consumidor %d consumindo %d da posição %d\n", *id, buffer[index_c], index_c);
                        buffer[index_c] = 0;
                }
                index_c++;

                if (index_c >= buffer_size) {
                        index_c = 0;
                }

                sem_post(&mutex);
                sem_post(&empty);
        }

}