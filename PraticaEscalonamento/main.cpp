/*Aluno: Richard Fereira Salviano
  Matricula: 202011250024
  Data: 16/11/2022
  Atividade: Atividade Prática - Escalonamento
  Descrição: Criar um programa que simule o escalonamento de processos fazendo uso de classes de prioridades.
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define QUANTUM 5
#define BUFFER_SIZE 255
//Declarando um limite de processos pra usar na lista
#define limiteDeProcessos 30
//Divisor padrão que vou usar nos prints
const char divisor[2] = ";";

#define inacabado 0 
#define finalizado 1 

typedef struct
{
    char ID[5];
    int DURATION;
    int STATUS;
} Process;
//Criando lista com o limite alto para garantir que todos sejam completados
Process process[limiteDeProcessos];
//Criando uma função que adiciona o processo no final da lista depois que ele passar no quantum
void listaDeProcessos(Process *process, int swapA, int size) 
{
    Process aux = process[swapA];
    for(int i = swapA; i < size - 1; i ++)  
    {
        process[i] = process[i+1];
    }
    process[size - 1] = aux;
}
//Função principal
int main(int argc, char const *argv[])
{
    FILE *file;
  
    Process p;
    char buffer[BUFFER_SIZE];
    char *value;
    int count = 0;

    // Esse fopem serve para acessar o arquivo txt que tem os processos e adicionar a file
    file = fopen("./processos.txt", "r");

    printf("Iniciando escalonamento\n\n");
  
    while(fgets(buffer, BUFFER_SIZE, file)) 
    {
        value = strtok(buffer, divisor);
        strcpy(p.ID, value);
        value = strtok(NULL, divisor);
        p.DURATION = atoi(value);
        p.STATUS = inacabado;
        process[count] = p;
        count++;
    }
    //Fechando o file com o arquivo txt
    fclose(file);

    int naoProcessados = count;
    int currentProcIndex = 0;

    Process *currentProc;
    //Enquanto tiver itens não processados vai rodando essas condições
    while(naoProcessados > 0) 
    {
        //Indicar qual o item que está sendo trabalhado
        currentProc = &process[currentProcIndex];
        printf("Processo atual: %s\n", currentProc->ID);
        printf("Tempo de execução: %i segundos\n", currentProc->DURATION);
      //Verifica o status atual
      if(currentProc->STATUS == inacabado) 
      {
            //Verifica se deu certo a conclusão do processo
            if(currentProc->DURATION <= QUANTUM)
            {
                sleep(currentProc->DURATION);
                printf("Status: Processo %s foi completado.\n\n", currentProc->ID);
              
                naoProcessados--;
                currentProcIndex = (currentProcIndex + 1) % count;
                currentProc->STATUS = finalizado;
                currentProc->DURATION = 0;
            }
            //Notifica o processo que não finalizou
            else 
            {
                sleep(QUANTUM);
                printf("Status: Processo %s não foi completado.\n\n", currentProc->ID);
                currentProc->DURATION -= QUANTUM;
                listaDeProcessos(process, currentProcIndex, count);
            }
      }
      else 
      {
          currentProcIndex = (currentProcIndex + 1) % count;
      }
    }
    return 0;
}