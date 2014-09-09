/*
Le programme suivant est fourni a titre d'exemple pour illustrer un article.
Les instructions superflues telles que le test des codes de retour des
fonctions ont ete volontairement expurgees pour simplifier la lecture.

Rachid Koucha
*/
#define _GNU_SOURCE
#include <sched.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <linux/futex.h>

#define futex_wait(addr, val) \
           syscall(SYS_futex, addr, FUTEX_WAIT, val, NULL)
#define futex_wakeup(addr, nb) \
           syscall(SYS_futex, addr, FUTEX_WAKE, nb)

int futex_var;

unsigned int nb_loops;

int policy = SCHED_FIFO;

pthread_barrier_t barrier;

static void nop(void)
{
  // Sur Intel
  asm("rep; nop;");
  //asm("nop;");
}

static unsigned int calibration(unsigned int duree_demandee)
{
struct timeval t, t0, t1;
unsigned int   i;
unsigned int   duree;
unsigned int   nb;

#define NB_LOOP_BASE 100000

  nb = 0;
  gettimeofday(&t0, NULL);
  do
  {
    for (i = 0; i < NB_LOOP_BASE; i ++)
    {
      nop();
    }
    gettimeofday(&t1, NULL);
    timersub(&t1, &t0, &t);
    nb += NB_LOOP_BASE;
    duree = t.tv_sec * 1000 + t.tv_usec / 1000000;
  } while (duree < duree_demandee);

  return nb;
}

void *thread_1(void *p)
{
unsigned int        i;
struct sched_param  param;
cpu_set_t           cpu;

  CPU_ZERO(&cpu);
  CPU_SET(0, &cpu);
  pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu); 

  param.sched_priority = 1;
  pthread_setschedparam(pthread_self(), policy, &param);

  pthread_barrier_wait(&barrier);

  for (i = 0; i < nb_loops; i ++)
  {
    nop();
  }

  futex_var = 1;
  futex_wakeup(&futex_var, 1);

  printf("Thread_1 termine\n");

  return NULL;
}

void *thread_2(void *p)
{
unsigned int        i;
struct sched_param  param;
cpu_set_t           cpu;

  CPU_ZERO(&cpu);
  CPU_SET(0, &cpu);
  pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu); 

  param.sched_priority = 50;
  pthread_setschedparam(pthread_self(), policy, &param);

  pthread_barrier_wait(&barrier);

  for (i = 0; i < nb_loops; i ++)
  {
    nop();
  }

  printf("Thread_2 termine\n");

  return NULL;
}

void *thread_3(void *p)
{
struct timeval     t0, t1, t;
unsigned int       latence;
struct sched_param param;
cpu_set_t          cpu;

  CPU_ZERO(&cpu);
  CPU_SET(0, &cpu);
  pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu); 

  param.sched_priority = 97;
  pthread_setschedparam(pthread_self(), policy, &param);

  pthread_barrier_wait(&barrier);

  gettimeofday(&t0, NULL);

  futex_wait(&futex_var, 0);

  gettimeofday(&t1, NULL);

  timersub(&t1, &t0, &t);

  latence = t.tv_sec * 1000 + t.tv_usec / 1000000;

  printf("Latence : %u ms\n", latence);

  printf("Thread_3 termine\n");

  return NULL;
}

int main(int ac, char *av[])
{
pthread_t          tid1, tid2, tid3;
struct sched_param param;
cpu_set_t          cpu;

  CPU_ZERO(&cpu);
  CPU_SET(0, &cpu);
  pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu); 

  param.sched_priority = 99;  
  sched_setscheduler(0, policy, &param);

  // Calibration
  nb_loops = calibration(atoi(av[1]));

  printf("Nombre d'iterations pour %u ms : %u\n", atoi(av[1]), nb_loops);

  pthread_barrier_init(&barrier, NULL, 3);

  pthread_create(&tid1, NULL, thread_1, NULL);
  pthread_create(&tid2, NULL, thread_2, NULL);
  pthread_create(&tid3, NULL, thread_3, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  pthread_join(tid3, NULL);

  return 0;
}

