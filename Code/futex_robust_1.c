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
#include <stddef.h>

#define futex_wait(addr, val) \
           syscall(SYS_futex, addr, FUTEX_WAIT, val, NULL)

#define futex_wakeup(addr, nb) \
           syscall(SYS_futex, addr, FUTEX_WAKE, nb)

#define set_robust_list(l, s) \
           syscall(SYS_set_robust_list, l, s)

struct udata_t
{
  struct robust_list link;
  int                futex_var;
} *udata;

struct robust_list_head head;

int main(int ac, char *av[])
{
pid_t pid1, pid2;

  head.list.next = (struct robust_list *)&(head.list);
  head.futex_offset = offsetof(struct udata_t, futex_var);
  head.list_op_pending = 0;
  set_robust_list(&head, sizeof(struct robust_list_head));

  // Segment de memoire partagee pour le futex
  udata = (struct udata_t *)mmap(0, sizeof(struct udata_t),
                          PROT_READ | PROT_WRITE,
                          MAP_SHARED | MAP_ANONYMOUS,
                          -1, 0);

  udata->futex_var = 0;

  pid1 = fork();

  // Le premier processus fils se met en attente sur le futex
  if (0 == pid1)
  {
    set_robust_list(&head, sizeof(struct robust_list_head));

    udata->futex_var |= FUTEX_WAITERS;

    futex_wait(&(udata->futex_var), udata->futex_var);

    printf("Fils#%d reveille : futex_var = 0x%x\n", getpid(), udata->futex_var);

    exit(0);
  }

  // On s'assure que le fils est bloque sur le futex
  sleep(1);

  pid2 = fork();

  // Le second processus fils verrouille le futex avec son pid
  // mais ne reveille pas le premier fils avant de se terminer !
  if (0 == pid2)
  {
    set_robust_list(&head, sizeof(struct robust_list_head));

    head.list_op_pending = &(udata->link);

    udata->futex_var |= getpid();

    printf("Changement d'etat du futex : 0x%x\n", udata->futex_var);

    udata->link.next = head.list.next;
    head.list.next = &(udata->link);
    head.list_op_pending = (struct robust_list *)0;

    exit(0);
  }

  printf("Attente du second fils (pid#%d)\n", pid2);
  waitpid(pid2, NULL, 0);

  printf("Second fils#%d termine\n", pid2);

  printf("Attente du premier fils#%d\n", pid1);
  waitpid(pid1, NULL, 0);

  printf("Premier fils#%d termine\n", pid1);

  return 0;
} // main

