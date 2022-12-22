/*Aluno: Richard Fereira Salviano
  Matricula: 202011250024
  Data: 28/10/2022
  Atividade: Atividade Prática - Exclusão Mútua
  Descrição: Alterar o código do produtor-consumidor apresentado em sala para que o buffer tenha mais de 1 posição.
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX 1000000000
pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;

int buffer[MAX];
int contador = 1;
int i = 0;

const clock_t MAXDELAY = 2000000;

void delay(clock_t ticks) { /* a "busy" delay */
  clock_t start = clock();
  do
    ; while (clock() < start + ticks);
}

void *producer(void *ptr) {
    /* usado para sinalizacao */
    /* buffer usado entre produtor e consumidor */
    /* dados do produtor */
    for(;;) {
        pthread_mutex_lock(&the_mutex); /* obtem acesso exclusivo ao buffer */
        while (i == MAX){
            // Pode acabar a fatia de tempo?
            pthread_cond_wait(&condp, &the_mutex);
        }
        i++;   
        buffer[i] = contador; /*coloca item no buffer */
        printf("Colocando item %d no buffer\n", buffer[i]);
        
        contador++;
        
        delay(MAXDELAY);
        pthread_cond_signal(&condc); /* acorda consumidor */
        pthread_mutex_unlock(&the_mutex);/* libera acesso ao buffer */
    }

    pthread_exit(0);
}

void *consumer(void *ptr) {
    /* quantos numeros produzir */
    /* dados do consumidor */
    for(;;) {
        pthread_mutex_lock(&the_mutex); /* obtem acesso exclusivo ao buffer */
        while (i < MAX) {
            pthread_cond_wait(&condc, &the_mutex);
        }
        
        printf("Retirando item %d no buffer\n", buffer[i]);
        
        i--;
        
        delay(MAXDELAY);
        /* acorda o produtor */
        pthread_cond_signal(&condp);
        pthread_mutex_unlock(&the_mutex);/* libera acesso ao buffer */
    }
    pthread_exit(0);

}

int main(int argc, char **argv)
{
    pthread_t pro, con;
    pthread_mutex_init(&the_mutex, 0);
    pthread_cond_init(&condc, 0);
    pthread_cond_init(&condp, 0);
    pthread_create(&con, 0, consumer, 0);
    pthread_create(&pro, 0, producer, 0);
    pthread_join(pro, 0);
    pthread_join(con, 0);
    pthread_cond_destroy(&condc);
    pthread_cond_destroy(&condp);
    pthread_mutex_destroy(&the_mutex);
}
