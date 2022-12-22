/*Aluno: Richard Fereira Salviano
  Matricula: 202011250024
  Data: 04/09/2022
  Atividade: Atividade Prática - Jantar dos filosofos
  Descrição: Implementar a solução apresentada no livro
 */

#include "stdio.h"
#include <stdlib.h>
#include <pthread.h>

#define N 5             //numero de filosofos
#define LEFT (i+N-1)%N  //número do vizinho esquerdo de i
#define RIGHT (i+N)%N   //número do vizinho direito de i
#define THINKING 0      //filosofo está pensando
#define HUNGRY 1        //filosofo está tentando pegar os garfos
#define EATING 2        //filosofo está comendo
int state[N];           //array para acompanhar o estado de todos
pthread_mutex_t mutex;  //exclusão mútua para regiões críticas
pthread_mutex_t s[N];   //um semáforo por filósofo

const clock_t MAXDELAY = 10000000; //maximo de tempo 

//função para passar um tempo
void tempo(clock_t ticks) { 
  clock_t start = clock();
  do
    ; while (clock() < start + ticks);
}

void test(int i) { //i: número do filosofo de 0 a N-1
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
    state[i] = EATING;
    printf("O filosofo %d está comendo\n",i);
    pthread_mutex_unlock(&s[i]); //up(&s[i]); sai da zona crítica
    }
}
//filosofo pensa
void think(int i){ //i: número do filosofo de 0 a N-1
  printf("O filosofo %d está pensando \n", i); 
  tempo(MAXDELAY);
}
//filosofo tenta pegar os garfos
void take_forks(int i) { //i: número do filosofo de 0 a N-1
    pthread_mutex_lock(&mutex); //down(&mutex); entra na região critica
    state[i] = HUNGRY;          //registro que o filosofo i está com fome 
    printf("O filosofo %d está com fome\n",i); 
    test(i); //tenta adquirir 2 garfos
    pthread_mutex_unlock(&mutex); //up(&mutex); sai da região cŕtica
    pthread_mutex_lock(&s[i]); // down(&s[i]); bloquear se os garfos não foram adquiridos
}
//filosofo come
void eat(int i){ //i: número do filosofo de 0 a N-1
    printf("O filosofo %d está comendo\n", i);
    tempo(MAXDELAY*2); //tempo comendo
}
//solta os garfos
void put_forks(int i) { //i: número do filosofo de 0 a N-1
    pthread_mutex_lock(&mutex); //down(&mutex); entra na região critica
    state[i] = THINKING; //registro que o filosofo i está pensando
    printf("O filosofo %d está pensando\n",i); 
    test(LEFT); //veja se o vizinho esquerdo pode comer agora
    test(RIGHT);//veja se o vizinho direito pode comer agora
    pthread_mutex_unlock(&mutex); //up(&s[i]); sai da zona crítica
}
//função que chama todas as outras
void philosopher(int i) {
    while(1) {          //repete infinitamente
        think(i);       //filosofo está pensando
        take_forks(i);  //adquirir dois garfos ou bloquear
        eat(i);         //comendo o espaguete
        put_forks(i);   //coloca os dois garfos na mesa
   }
}
//função main onde vai rodar tudo
int main(void){
    pthread_t philo[N];

    //Inicio dos semáforos
    pthread_mutex_init(&mutex, NULL);
    for (int a = 0; a < N; a++)
        pthread_mutex_init(&s[a], NULL);

    //Inicio das threads
    for (int b = 0; b < N; b++)
    {
        pthread_create(&philo[b], NULL, (void *) philosopher, b);
    }
    for (int c = 0; c < N; c++)
    {
        pthread_join(philo[c], NULL);
    }
    for (int d = 0; d < N; d++)
    {
        pthread_mutex_destroy(&s[d]);
    }
    pthread_mutex_destroy(&mutex);
}
