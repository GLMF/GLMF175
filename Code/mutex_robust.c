/*
Le programme suivant est fourni a titre d'exemple pour illustrer un article.
Les instructions superflues telles que le test des codes de retour des
fonctions ont ete volontairement expurgees pour simplifier la lecture.

Rachid Koucha
*/
#include <stdio.h>
#define __USE_GNU
#include <pthread.h>
#include <string.h>
#include <errno.h>

pthread_mutex_t mutex;

void *thread(void *p)
{
int rc = pthread_mutex_lock(&mutex);

 printf("Thread#%x, rc = %d\n", (unsigned int)pthread_self(), rc);

  if (EOWNERDEAD == rc)
  {
    printf("Le proprietaire precedent du mutex est termine sans deverrouillage\n");

    pthread_mutex_unlock(&mutex);
  }

  return NULL;
}

int main(int ac, char *av[])
{
pthread_t           tid1, tid2;
pthread_mutexattr_t attr;

  // Initialisation du mutex
  memset(&attr, 0, sizeof(attr));
  pthread_mutexattr_setrobust_np(&attr, PTHREAD_MUTEX_ROBUST_NP);
  pthread_mutex_init(&mutex, &attr);

  pthread_create(&tid1, NULL, thread, NULL);
  pthread_create(&tid2, NULL, thread, NULL);

  printf("Attente du 1er thread#%x\n", (unsigned int)tid1);
  pthread_join(tid1, NULL);

  printf("Attente du 2nd thread#%x\n", (unsigned int)tid2);
  pthread_join(tid2, NULL);

  return 0;
} // main

