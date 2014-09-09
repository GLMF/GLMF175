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

#define futex_lock_pi(addr) \
  syscall(SYS_futex, addr, FUTEX_LOCK_PI, 0, 0)

#define futex_unlock_pi(addr) \
  syscall(SYS_futex, addr, FUTEX_UNLOCK_PI, 0, 0)

#define gettid() syscall(__NR_gettid)

int futex_var;

unsigned int nb_loops;

int policy = SCHED_FIFO;

pthread_barrier_t barrier;

static void nop(void)
{
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
int                 old;
int                 mytid = gettid();

  CPU_ZERO(&cpu);
  CPU_SET(0, &cpu);
  pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu); 

  param.sched_priority = 0;
  pthread_setschedparam(pthread_self(), SCHED_OTHER, &param);

  futex_var = mytid;

  pthread_barrier_wait(&barrier);

  for (i = 0; i < nb_loops; i ++)
  {
    nop();
  }

  old = __sync_val_compare_and_swap(&futex_var, mytid, 0);

  if (mytid == old)
  {
    // Rien à faire car il n'y a pas de threads en attente sur le futex 
    printf("Pas de thread en attente sur le futex\n");
  }
  else
  {
    // Il y a un thread en attente sur le futex (futex_var = FUTEX_WAITERS | mytid)
    futex_unlock_pi(&futex_var);
  }

  printf("0x%x : Thread_1 termine\n", mytid);

  return NULL;
}

void *thread_2(void *p)
{
unsigned int        i;
struct sched_param  param;
cpu_set_t           cpu;
int                 mytid = gettid();

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

  printf("0x%x : Thread_2 termine\n", mytid);

  return NULL;
}

void *thread_3(void *p)
{
struct timeval     t0, t1, t;
unsigned int       latence;
struct sched_param param;
cpu_set_t          cpu;
int                old;
int                mytid = gettid();

  CPU_ZERO(&cpu);
  CPU_SET(0, &cpu);
  pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu); 

  param.sched_priority = 97;
  pthread_setschedparam(pthread_self(), policy, &param);

  pthread_barrier_wait(&barrier);

  gettimeofday(&t0, NULL);

  old = __sync_val_compare_and_swap(&futex_var, 0, mytid);

  printf("0x%x : La valeur du futex avant LOCK est : 0x%x\n", mytid, futex_var);

  switch(old)
  {
    case 0 : // Le futex etait libre
    {
      // Desormais le thread courant detient le futex
    }
    break;

    default : // Le futex n'est pas libre
    {
      futex_lock_pi(&futex_var);
    }
  }

  printf("0x%x : La valeur du futex après LOCK est : 0x%x\n", mytid, futex_var);

  futex_unlock_pi(&futex_var);

  printf("0x%x : La valeur du futex après UNLOCK est : 0x%x\n", mytid, futex_var);

  gettimeofday(&t1, NULL);

  timersub(&t1, &t0, &t);

  latence = t.tv_sec * 1000 + t.tv_usec / 1000000;

  printf("0x%x : Latence : %u ms\n", mytid, latence);

  printf("0x%x : Thread_3 termine\n", mytid);

  return NULL;
}

int main(int ac, char *av[])
{
pthread_t          tid1, tid2, tid3;
struct sched_param param;
int                mytid = gettid();
cpu_set_t          cpu;

  CPU_ZERO(&cpu);
  CPU_SET(0, &cpu);
  pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu); 

  param.sched_priority = 99;  
  sched_setscheduler(0, policy, &param);

  // Calibration
  nb_loops = calibration(atoi(av[1]));

  printf("0x%x : Nombre d'iterations pour %u ms : %u\n", mytid, atoi(av[1]), nb_loops);

  pthread_barrier_init(&barrier, NULL, 3);

  pthread_create(&tid1, NULL, thread_1, NULL);
  pthread_create(&tid2, NULL, thread_2, NULL);
  pthread_create(&tid3, NULL, thread_3, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  pthread_join(tid3, NULL);

  return 0;
}

