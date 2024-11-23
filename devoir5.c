#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

pthread_t* tab_thread;        
sem_t* tab_sem;               
int nb_thread;           

void* thread(void* arg) 
{
    int id = *(int*)arg; 

    while (1) 
    {
        sem_wait(&tab_sem[id]); 
        printf("Hello ! Je suis le thread %d. Je viens de m'executer. \n", id + 1);
        sleep(1);
        sem_post(&tab_sem[(id + 1) % nb_thread]); 
    }

    return NULL;
}

int main() 
{
    printf("Entrez le nombre de threads : ");
    scanf("%d", &nb_thread);

    if (nb_thread < 1) 
    {
        printf("Le nombre de threads doit etre au moins 1.\n");
        return 1;
    }

    tab_thread = malloc(nb_thread * sizeof(pthread_t));
    tab_sem = malloc(nb_thread * sizeof(sem_t));

    if (!tab_thread || !tab_sem) 
    {
        printf("Erreur d'allocation memoire.\n");
        free(tab_thread);
        free(tab_sem);
        return 1;
    }

    for (int i = 0; i < nb_thread; i++) 
    {
        sem_init(&tab_sem[i], 0, 0); 
    }
    sem_post(&tab_sem[0]); 

    int* tab_num = malloc(nb_thread * sizeof(int));
    if (!tab_num) 
    {
        printf("Erreur d'allocation mémoire pour les numeros des threads.\n");
        free(tab_thread);
        free(tab_sem);
        return 1;
    }

    for (int i = 0; i < nb_thread; i++) 
    {
        tab_num[i] = i;
        pthread_create(&tab_thread[i], NULL, thread, &tab_num[i]);
    }

    for (int i = 0; i < nb_thread; i++) 
    {
        pthread_join(tab_thread[i], NULL);
    }

    for (int i = 0; i < nb_thread; i++) 
    {
        sem_destroy(&tab_sem[i]);
    }
    free(tab_thread);
    free(tab_sem);
    free(tab_num);

    return 0;
}

















