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
#include <sys/wait.h>

#define futex_wait(addr, val) \
           syscall(SYS_futex, addr, FUTEX_WAIT, val, NULL)

#define futex_wakeup(addr, nb) \
           syscall(SYS_futex, addr, FUTEX_WAKE, nb)

int *futex_var;

int main(int ac, char *av[])
{
pid_t pid1, pid2;

  // Segment de memoire partagee pour le futex
  futex_var = (int *)mmap(0, sizeof(int),
                          PROT_READ | PROT_WRITE,
                          MAP_SHARED | MAP_ANONYMOUS,
                          -1, 0);

  *futex_var = 0;

  pid1 = fork();

  // Le premier processus fils se met en attente sur le futex
  if (0 == pid1)
  {
    futex_wait(futex_var, 0);

    printf("Fils#%d reveille\n", getpid());

    exit(0);
  }

  // On s'assure que le fils est bloque sur le futex
  sleep(1);

  pid2 = fork();

  // Le second processus fils passe le futex à la valeur 1
  // mais ne reveille pas le premier fils avant de se terminer !
  if (0 == pid2)
  {
    *futex_var = 1;

    printf("Changement d'etat du futex : 0x%x\n", *futex_var);

    exit(0);
  }

  printf("Attente du second fils#%d\n", pid2);
  waitpid(pid2, NULL, 0);

  printf("Second fils#%d termine\n", pid2);

  printf("Attente du premier fils#%d\n", pid1);
  waitpid(pid1, NULL, 0);

  printf("Premier fils#%d termine\n", pid1);

  return 0;
} // main

