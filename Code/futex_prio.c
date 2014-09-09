/*
Le programme suivant est fourni a titre d'exemple pour illustrer un article.
Les instructions superflues telles que le test des codes de retour des
fonctions ont ete volontairement expurgees pour simplifier la lecture.

Rachid Koucha
*/
#include <unistd.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sched.h>

#define futex_wait(addr, val) \
           syscall(SYS_futex, addr, FUTEX_WAIT, val, NULL)

#define futex_wakeup(addr, nb) \
           syscall(SYS_futex, addr, FUTEX_WAKE, nb)

int *futex_var;


int main(int ac, char *av[])
{
pid_t               pid;
unsigned int        i;
int                 prio;
int                 policy;
struct sched_param  param;
const char         *policy_str;

  // Segment de memoire partagee pour le futex
  futex_var = (int *)mmap(0, sizeof(int),
                          PROT_READ | PROT_WRITE,
                          MAP_SHARED | MAP_ANONYMOUS,
                          -1, 0);

  *futex_var = 0;

  for (i = 1; i < ac; i ++)
  {
    switch(av[i][0])
    {
      case 'r':
      {
        policy = SCHED_RR;
        prio = atoi(&(av[i][1]));
        policy_str = ", RR";
      }
      break;
      case 'f':
      {
        policy = SCHED_FIFO;
        prio = atoi(&(av[i][1]));
        policy_str = ", FIFO";
      }
      break;
      default :
      {
        prio = 0;
        policy_str = "";
      }
    }

    pid = fork();

    // Processus fils
    if (0 == pid)
    {
      if (prio)
      {
        param.sched_priority = prio;
        sched_setscheduler(0, policy, &param);
      }

      futex_wait(futex_var, 0);

      printf("Fils#%d reveille\n", getpid());

      exit(0);
    }

    printf("Fils#%d, priorite %d%s\n", pid, prio, policy_str);
  } // End for

  // On s'assure que tous les fils sont bloques sur le futex
  sleep(1);

  printf("Changement d'etat du futex\n");

  *futex_var = 1;

  for (i = 1; i < ac; i ++)
  {
    futex_wakeup(futex_var, 1);

    // On laisse le temps au fils d'afficher son message
    sleep(1);
  } // End for

  return 0;
} // main

