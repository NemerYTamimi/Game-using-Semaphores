/*
 * semaphores Game
 */
// colors
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"
// end of colors
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/* This declaration is *MISSING* is many solaris environments.
   It should be in the <sys/sem.h> file but often is not! If 
   you receive a duplicate definition error message for semun
   then comment out the union declaration.
   */

union semun
{
  int val;
  struct semid_ds *buf;
  ushort *array;
};

void main(int argc, char *argv[])
{
  FILE *fptr;
  static struct sembuf acquire = {0, -1, SEM_UNDO},
                       release = {0, 1, SEM_UNDO};

  pid_t c_pid;
  key_t ipc_key;
  static ushort start_val[6] = {1, 1, 1, 0, 0, 0};
  int semid, pnum, i, n, p_sleep, c_sleep;
  union semun arg;
  enum
  {
    p0,
    p1,
    p2,
    p3,
    p4,
    p5
  };

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s child id\n", argv[0]);
    exit(-1);
  }

  ipc_key = ftok(".", 'S');

  /*
   * Create the semaphore
   */
  //get the child id from argument
  pnum = atoi(argv[1]);

  if ((semid = semget(ipc_key, 6, IPC_CREAT | IPC_EXCL | 0660)) != -1)
  {
    arg.array = start_val;

    if (semctl(semid, 0, SETALL, arg) == -1)
    {
      perror("semctl  -- initialization");
      exit(1);
    }
  }
  else if ((semid = semget(ipc_key, 3, 0)) == -1)
  {
    perror("semget -- obtaining semaphore");
    exit(2);
  }

  if (pnum < 3)
    printf("\n");

  /*
each proccess have one case by its number
for example child #0 run case 0
*/

  switch (pnum)
  {
  case 0:
    for (i = 0; i < 10; i++)
    {
      acquire.sem_num = p0;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p0 -- intital step");
        exit(3);
      }
      printf("\t\t\tP0 start moving forword to G\n");
      sleep(1);
      printf("\t\t\t\t%sP0 arrived G%s\n", KMAG, KNRM);
      printf("\t\t\tP0 start moving forword to H\n");
      sleep(2);
      printf("\t\t\t\t%sP0 arrived H%s\n", KMAG, KNRM);

      release.sem_num = p3;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP0 releases p3");
        exit(4);
      }
      acquire.sem_num = p0;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p0 -- waiting p3");
        exit(5);
      }
      printf("\t\t\tP0 start moving back to G\n");
      sleep(2);
      printf("\t\t\t\t%sP0 arrived G%s\n", KMAG, KNRM);

      acquire.sem_num = p0;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p0 -- waiting p3");
        exit(5);
      }
      printf("\t\t\tP0 start moving back to A\n");
      sleep(1);
      printf("\t\t\t\t%sP0 arrived A%s\n", KMAG, KNRM);

      printf("\t\t\t\t%sP0 finished%s\t\n", KGRN, KNRM);
      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p1");
        exit(6);
      }
      release.sem_num = p2;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p2");
        exit(7);
      }
      release.sem_num = p3;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p3");
        exit(8);
      }
      release.sem_num = p4;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p4");
        exit(9);
      }
      release.sem_num = p5;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p5");
        exit(10);
      }
      acquire.sem_num = p0;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p0 -- waiting p2");
        exit(16);
      }
    }
    break;
  case 1:
    for (i = 0; i < 10; i++)
    {
      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- initial point");
        exit(11);
      }
      printf("\t\t\tP1 start moving forword to G\n");
      sleep(1);
      printf("\t\t\t\t%sP1 arrived G%s\n", KMAG, KNRM);

      printf("\t\t\tP1 start moving forword to H\n");
      sleep(2);
      printf("\t\t\t\t%sP1 arrived H%s\n", KMAG, KNRM);

      release.sem_num = p4;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- p1 releases p4");
        exit(12);
      }

      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- waiting p4");
        exit(13);
      }

      printf("\t\t\tP1 start moving forword to I\n");
      sleep(2);
      printf("\t\t\t\t%sP1 arrived I%s\n", KMAG, KNRM);

      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- waiting p0");
        exit(14);
      }
      printf("\t\t\tP1 start moving back to H\n");
      sleep(2);
      printf("\t\t\t\t%sP1 arrived H%s\n", KMAG, KNRM);

      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- waiting p3");
        exit(15);
      }
      printf("\t\t\tP1 start moving back to G\n");
      sleep(2);
      printf("\t\t\t\t%sP1 arrived G%s\n", KMAG, KNRM);

      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- waiting p4");
        exit(16);
      }
      printf("\t\t\tP1 start moving back to B\n");
      sleep(1);
      printf("\t\t\t\t%sP1 arrived B%s\n", KMAG, KNRM);

      printf("\t\t\t\t%sP1 finished%s\t\n", KGRN, KNRM);

      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- waiting p2");
        exit(16);
      }
    }
    break;
  case 2:
    for (i = 0; i < 10; i++)
    {

      acquire.sem_num = p2;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p2 -- initial state");
        exit(17);
      }
      printf("\t\t\tP2 start moving forword to G\n");
      sleep(1);
      printf("\t\t\t\t%sP2 arrived G%s\n", KMAG, KNRM);

      printf("\t\t\tP2 start moving forword to H\n");
      sleep(2);
      printf("\t\t\t\t%sP2 arrived H%s\n", KMAG, KNRM);

      release.sem_num = p5;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- p2 release p5");
        exit(18);
      }

      acquire.sem_num = p2;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p2 -- waiting p4");
        exit(19);
      }
      printf("\t\t\tP2 start moving forword to I\n");
      sleep(2);
      printf("\t\t\t\t%sP2 arrived I%s\n", KMAG, KNRM);

      acquire.sem_num = p2;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p2 -- waiting p0");
        exit(20);
      }
      printf("\t\t\tP2 start moving back to H\n");
      sleep(2);
      printf("\t\t\t\t%sP2 arrived H%s\n", KMAG, KNRM);

      acquire.sem_num = p2;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p2 -- waiting p3");
        exit(21);
      }
      printf("\t\t\tP2 start moving back to G\n");
      sleep(2);
      printf("\t\t\t\t%sP2 arrived G%s\n", KMAG, KNRM);

      acquire.sem_num = p2;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p2 -- waiting p4");
        exit(22);
      }
      printf("\t\t\tP2 start moving back to C\n");
      sleep(1);
      printf("\t\t\t\t%sP2 arrived C%s\n", KMAG, KNRM);

      printf("\t\t\t\t%sP2 finished%s\t\n", KGRN, KNRM);
      printf("\n\t\t---------%sItaration # %d finished%s-----------\n\n", KBLU, i + 1, KNRM);

      release.sem_num = p0;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p0");
        exit(10);
      }
      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p1");
        exit(10);
      }
      release.sem_num = p3;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p3");
        exit(10);
      }
      release.sem_num = p4;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p4");
        exit(10);
      }
      release.sem_num = p5;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p5");
        exit(10);
      }
      release.sem_num = p2;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p2");
        exit(10);
      }
      release.sem_num = p0;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p0");
        exit(10);
      }
      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p1");
        exit(10);
      }
    }
    break;
  case 3:
    for (i = 0; i < 10; i++)
    {

      acquire.sem_num = p3;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p3 -- waiting p0");
        exit(23);
      }
      printf("\t\t\tP3 start moving forword to H\n");
      sleep(3);
      printf("\t\t\t\t%sP3 arrived H%s\n", KMAG, KNRM);

      release.sem_num = p0;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP3 releases p0");
        exit(24);
      }

      acquire.sem_num = p3;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p3 -- waiting p4");
        exit(25);
      }
      printf("\t\t\tP3 start moving forword to I\n");
      sleep(2);
      printf("\t\t\t\t%sP3 arrived I%s\n", KMAG, KNRM);

      release.sem_num = p0;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP3 releaases p0");
        exit(26);
      }

      acquire.sem_num = p3;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p3 -- waiting p0");
        exit(27);
      }
      printf("\t\t\tP3 start moving back to H\n");
      sleep(2);
      printf("\t\t\t\t%sP3 arrived H%s\n", KMAG, KNRM);

      acquire.sem_num = p3;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p3 -- waiting p5");
        exit(28);
      }
      printf("\t\t\tP3 start moving back to D\n");
      sleep(3);
      printf("\t\t\t\t%sP3 arrived D%s\n", KMAG, KNRM);

      printf("\t\t\t\t%sP3 finished%s\t\n", KGRN, KNRM);

      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP3 releases p1");
        exit(29);
      }
      release.sem_num = p2;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP3 releases p2");
        exit(30);
      }
      acquire.sem_num = p3;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p3 -- waiting p0");
        exit(23);
      }
    }
    break;
  case 4:
    for (i = 0; i < 10; i++)
    {
      acquire.sem_num = p4;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p4 -- waiting p2");
        exit(31);
      }
      printf("\t\t\tP4 start moving forword to H\n");
      sleep(4);
      printf("\t\t\t\t%sP4 arrived H%s\n", KMAG, KNRM);

      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP4 releases p1");
        exit(32);
      }
      release.sem_num = p2;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP4 releases p2");
        exit(33);
      }
      release.sem_num = p3;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP4 releases p3");
        exit(34);
      }
      printf("\t\t\tP4 start moving forword to I\n");
      sleep(2);
      printf("\t\t\t\t%sP4 arrived I%s\n", KMAG, KNRM);

      acquire.sem_num = p4;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p4 -- waiting p0");
        exit(37);
      }
      printf("\t\t\tP4 start moving back to H\n");
      sleep(2);
      printf("\t\t\t\t%sP4 arrived H%s\n", KMAG, KNRM);

      acquire.sem_num = p4;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p4 -- waiting p5");
        exit(38);
      }
      printf("\t\t\tP4 start moving back to E\n");
      sleep(4);
      printf("\t\t\t\t%sP4 arrived E%s\n", KMAG, KNRM);

      printf("\t\t\t\t%sP4 finished%s\t\n", KGRN, KNRM);

      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP4 releases p1");
        exit(39);
      }
      release.sem_num = p2;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP4 releases p2");
        exit(40);
      }
      acquire.sem_num = p4;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p4 -- waiting p2");
        exit(23);
      }
    }
    break;
  case 5:
    for (i = 0; i < 10; i++)
    {
      acquire.sem_num = p5;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p5 -- waiting p2");
        exit(41);
      }
      printf("\t\t\tP5 start moving forword to I\n");
      sleep(3);
      printf("\t\t\t\t%sP5 arrived I%s\n", KMAG, KNRM);

      acquire.sem_num = p5;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p5 -- waiting p0");
        exit(42);
      }
      printf("\t\t\tP5 start moving back to F\n");
      sleep(3);
      printf("\t\t\t\t%sP5 arrived F%s\n", KMAG, KNRM);

      printf("\t\t\t\t%sP5 finished%s\n", KGRN, KNRM);

      release.sem_num = p3;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP5 releases p3");
        exit(43);
      }
      release.sem_num = p4;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP5 releases p4");
        exit(44);
      }
      acquire.sem_num = p5;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p5 -- waiting p2");
        exit(23);
      }
    }

    if (semctl(semid, 0, IPC_RMID, 0) != -1)
    {
      printf("\t\t     %sGame Ended and semaphores are removed%s\n", KYEL, KNRM);
    }
    break;
  }
}
