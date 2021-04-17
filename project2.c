/*
 * Producer / Consumer problem
 */

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

  pnum = atoi(argv[1]);
  if ((semid = semget(ipc_key, 6, IPC_CREAT | IPC_EXCL | 0660)) != -1)
  {
    arg.array = start_val;

    if (semctl(semid, 0, SETALL, arg) == -1)
    {
      perror("semctl -- p0 -- initialization");
      exit(1);
    }
  }
  else if ((semid = semget(ipc_key, 3, 0)) == -1)
  {
    perror("semget -- p1-p5 -- obtaining semaphore");
    exit(2);
  }

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
      printf("P0 start moving to G\n");
      sleep(3);
      printf("P0 start moving to H\n");
      sleep(4);
      release.sem_num = p3;
      if (semop(semid, &release, 1) == -1)
      {
        perror("P0 releases p3");
        exit(4);
      }
      acquire.sem_num = p0;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p0 -- waiting p3");
        exit(5);
      }
      printf("P0 start moving back to G\n");
      sleep(4);
      acquire.sem_num = p0;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p0 -- waiting p3");
        exit(5);
      }
      printf("P0 start moving back to A\n");
      sleep(3);
      printf("\tP0 finished\t\n");
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
      printf("P1 start moving to  G\n");
      sleep(3);
      printf("P1 start moving to H\n");
      sleep(4);
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

      printf("P1 start moving to I\n");
      sleep(4);

      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- waiting p0");
        exit(14);
      }
      printf("P1 start moving back to H\n");
      sleep(4);

      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- waiting p3");
        exit(15);
      }
      printf("P1 start moving back to G\n");
      sleep(4);

      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- waiting p4");
        exit(16);
      }
      printf("P1 start moving back to B\n");
      sleep(3);
      printf("\tP1 finished\t\n");

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
      printf("P2 start moving to G\n");
      sleep(3);

      printf("P2 start moving to H\n");
      sleep(4);

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
      printf("P2 start moving to I\n");
      sleep(4);

      acquire.sem_num = p2;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p2 -- waiting p0");
        exit(20);
      }
      printf("P2 start moving back to H\n");
      sleep(4);

      acquire.sem_num = p2;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p2 -- waiting p3");
        exit(21);
      }
      printf("P2 start moving back to G\n");
      sleep(4);

      acquire.sem_num = p2;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p2 -- waiting p4");
        exit(22);
      }
      printf("P2 start moving back to C\n");
      sleep(3);
      printf("\tP2 finished\t\n");
      printf("\n\t------Itaration # %d finished--------\t\n\n", i + 1);

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
      printf("P3 start moving to H\n");
      sleep(6);

      release.sem_num = p0;
      if (semop(semid, &release, 1) == -1)
      {
        perror("P3 releases p0");
        exit(24);
      }

      acquire.sem_num = p3;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p3 -- waiting p4");
        exit(25);
      }
      printf("P3 start moving to I\n");
      sleep(4);

      release.sem_num = p0;
      if (semop(semid, &release, 1) == -1)
      {
        perror("P3 releaases p0");
        exit(26);
      }

      acquire.sem_num = p3;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p3 -- waiting p0");
        exit(27);
      }
      printf("P3 start moving back to H\n");
      sleep(4);

      acquire.sem_num = p3;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p3 -- waiting p5");
        exit(28);
      }
      printf("P3 start moving back to D\n");
      sleep(6);
      printf("\tP3 finished\t\n");

      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("P3 releases p1");
        exit(29);
      }
      release.sem_num = p2;
      if (semop(semid, &release, 1) == -1)
      {
        perror("P3 releases p2");
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
      printf("P4 start moving to H\n");
      sleep(8);

      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("P4 releases p1");
        exit(32);
      }
      release.sem_num = p2;
      if (semop(semid, &release, 1) == -1)
      {
        perror("P4 releases p2");
        exit(33);
      }
      release.sem_num = p3;
      if (semop(semid, &release, 1) == -1)
      {
        perror("P4 releases p3");
        exit(34);
      }
      printf("P4 start moving to I\n");
      sleep(4);
      acquire.sem_num = p4;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p4 -- waiting p0");
        exit(37);
      }
      printf("P4 start moving back to H\n");
      sleep(4);

      acquire.sem_num = p4;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p4 -- waiting p5");
        exit(38);
      }
      printf("P4 start moving back to E\n");
      sleep(8);
      printf("\tP4 finished\t\n");

      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("P4 releases p1");
        exit(39);
      }
      release.sem_num = p2;
      if (semop(semid, &release, 1) == -1)
      {
        perror("P4 releases p2");
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
      printf("P5 start moving to I\n");
      sleep(7);

      acquire.sem_num = p5;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p5 -- waiting p0");
        exit(42);
      }
      printf("P5 start moving back to F\n");
      sleep(7);
      printf("\tP5 finished\n");

      release.sem_num = p3;
      if (semop(semid, &release, 1) == -1)
      {
        perror("P5 releases p3");
        exit(43);
      }
      release.sem_num = p4;
      if (semop(semid, &release, 1) == -1)
      {
        perror("P5 releases p4");
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
      printf("\t semaphore removed \t\n");
    }
    break;
  }

  exit(0);
}
