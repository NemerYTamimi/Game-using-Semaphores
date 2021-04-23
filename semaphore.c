#include "local.h"

void main(int argc, char *argv[])
{
  FILE *fptr;
  static struct sembuf acquire = {0, -1, SEM_UNDO},
                       release = {0, 1, SEM_UNDO};

  pid_t c_pid;
  key_t ipc_key;
  static ushort start_val[6] = {1, 1, 1, 0, 0, 0};
  int semid, pnum, i, n, sleep_time, wait_flag1 = 1, wait_flag2 = 1, wait_flag3 = 1, wait_flag4 = 1, sleep_limit = 5;
  union semun arg;

  srand((unsigned)getpid());
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
  /*
  * first child will create the semaphore and initialize it with {1,1,1,0,0,0}
  * that makes p0,p1 and p2 starts directly
  */
  if ((semid = semget(ipc_key, 6, IPC_CREAT | IPC_EXCL | 0660)) != -1)
  {
    arg.array = start_val;

    if (semctl(semid, 0, SETALL, arg) == -1)
    {
      perror("semctl  -- initialization");
      exit(-1);
    }
  }
  else if ((semid = semget(ipc_key, 3, 0)) == -1)
  {
    perror("semget -- obtaining semaphore");
    exit(-1);
  }

  /*
* this \n just for style 
  */
  if (pnum < 3)
    printf("\n");

  /*
each proccess have one case by its number
for example child #0 run case 0
*/

  switch (pnum)
  {
    /* P0 code start */
  case 0:
    for (i = 0; i < 10; i++)
    {
      acquire.sem_num = p0; //initial acguire
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p0 -- intital step");
        exit(-1);
      }
      release.sem_num = p0;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases himself");
        exit(-1);
      }

      printf("\t\t\tP0 start moving forword to G\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP0 arrived G after %dseconds %s\n", KMAG, sleep_time, KNRM);
      acquire.sem_num = p0;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p0 -- tell p1 & p2 I arrived");
        exit(-1);
      }
      /* 
      here p0 will waits until p1 & p2 are arrived G when changes its semaphore value from 2 to 0
      */
      wait_flag1 = 1;
      wait_flag2 = 1;
      while (wait_flag1 != 0)
      {
        while (wait_flag2 != 0)
        {
          if ((wait_flag2 = semctl(semid, 1, GETVAL, 0)) == -1)
          {
            perror("semctl: GETVAL P1");
            exit(-1);
          }
        }
        if ((wait_flag1 = semctl(semid, 2, GETVAL, 0)) == -1)
        {
          perror("semctl: GETVAL P2");
          exit(-1);
        }
      }
      release.sem_num = p0;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases himself");
        exit(-1);
      }
      printf("\t\t\tP0 start moving forword to H\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP0 arrived H after %dseconds%s\n", KMAG, sleep_time, KNRM);
      acquire.sem_num = p0;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p0 -- say I arrived");
        exit(-1);
      }
      wait_flag1 = 1;
      wait_flag2 = 1;
      while (wait_flag1 != 0)
      {
        while (wait_flag2 != 0)
        {
          if ((wait_flag2 = semctl(semid, 1, GETVAL, 0)) == -1)
          {
            perror("semctl: GETVAL P1");
            exit(-1);
          }
        }
        if ((wait_flag1 = semctl(semid, 2, GETVAL, 0)) == -1)
        {
          perror("semctl: GETVAL P2");
          exit(-1);
        }
      }
      acquire.sem_num = p0;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p0 -- waiting p3 or p4");
        exit(-1);
      }
      printf("\t\t\tP0 start moving back to G\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP0 arrived G after %dseconds%s\n", KMAG, sleep_time, KNRM);

      acquire.sem_num = p0;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p0 -- waiting p1-p3 {p3 must release it}");
        exit(-1);
      }
      printf("\t\t\tP0 start moving back to A\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP0 arrived A after %dseconds%s\n", KMAG, sleep_time, KNRM);

      printf("\t\t\t\t%sP0 finished%s\t\n", KGRN, KNRM);
      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p1");
        exit(-1);
      }
      release.sem_num = p2;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p2");
        exit(-1);
      }
      release.sem_num = p3;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p3");
        exit(-1);
      }
      release.sem_num = p4;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p4");
        exit(-1);
      }
      release.sem_num = p5;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p5");
        exit(-1);
      }
      acquire.sem_num = p0;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p0 -- waiting p2");
        exit(-1);
      }
    } /* P0 code end */
    break;
    /* P1 code start */
  case 1:
    for (i = 0; i < 10; i++)
    {
      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- initial point");
        exit(-1);
      }
      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases himself");
        exit(-1);
      }
      printf("\t\t\tP1 start moving forword to G\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP1 arrived G after %dseconds%s\n", KMAG, sleep_time, KNRM);
      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- tell p0 & p2 I arrived");
        exit(-1);
      }
      /* 
      here p1 will waits until p0 & p2 are arrived G when changes its semaphore value from 2 to 0
      */
      wait_flag1 = 1;
      wait_flag2 = 1;
      while (wait_flag1 != 0)
      {
        while (wait_flag2 != 0)
        {
          if ((wait_flag2 = semctl(semid, 2, GETVAL, 0)) == -1)
          {
            perror("semctl: GETVAL P2");
            exit(-1);
          }
        }
        if ((wait_flag1 = semctl(semid, 0, GETVAL, 0)) == -1)
        {
          perror("semctl: GETVAL P0");
          exit(-1);
        }
      }
      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases himself");
        exit(-1);
      }
      printf("\t\t\tP1 start moving forword to H\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP1 arrived H after %dseconds%s\n", KMAG, sleep_time, KNRM);
      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- tell others i arrived");
        exit(-1);
      }
      while (wait_flag1 != 0)
      {
        while (wait_flag2 != 0)
        {
          if ((wait_flag2 = semctl(semid, 2, GETVAL, 0)) == -1)
          {
            perror("semctl: GETVAL P2");
            exit(-1);
          }
        }
        if ((wait_flag1 = semctl(semid, 0, GETVAL, 0)) == -1)
        {
          perror("semctl: GETVAL P0");
          exit(-1);
        }
      }
      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- waiting p4");
        exit(-1);
      }
      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases himself");
        exit(-1);
      }
      printf("\t\t\tP1 start moving forword to I\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP1 arrived I after %dseconds%s\n", KMAG, sleep_time, KNRM);
      wait_flag1 = 1;
      if (wait_flag1 != 0)
      {
        acquire.sem_num = p1;
        if (semop(semid, &acquire, 1) == -1)
        {
          perror("semop -- p1 -- tell others i finished");
          exit(-1);
        }
        if ((wait_flag1 = semctl(semid, 1, GETVAL, 0)) == -1)
        {
          perror("semctl: GETVAL P1");
          exit(-1);
        }
      }
      wait_flag1 = 1;
      wait_flag2 = 1;
      wait_flag3 = 1;
      wait_flag4 = 1;

      while (wait_flag1 != 0)
      {
        while (wait_flag2 != 0)
        {
          while (wait_flag3 != 0)
          {
            while (wait_flag4 != 0)
            {
              if ((wait_flag4 = semctl(semid, 2, GETVAL, 0)) == -1)
              {
                perror("semctl: GETVAL P2");
                exit(4);
              }
            }
            if ((wait_flag3 = semctl(semid, 3, GETVAL, 0)) == -1)
            {
              perror("semctl: GETVAL P3");
              exit(4);
            }
          }
          if ((wait_flag2 = semctl(semid, 4, GETVAL, 0)) == -1)
          {
            perror("semctl: GETVAL P4");
            exit(4);
          }
        }
        if ((wait_flag1 = semctl(semid, 5, GETVAL, 0)) == -1)
        {
          perror("semctl: GETVAL P5");
          exit(4);
        }
      }

      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- waiting p0");
        exit(-1);
      }
      printf("\t\t\tP1 start moving back to H\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);

      printf("\t\t\t%sP1 arrived H after %dseconds%s\n", KMAG, sleep_time, KNRM);

      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- waiting p3");
        exit(-1);
      }
      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases himself");
        exit(-1);
      }
      printf("\t\t\tP1 start moving back to G\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP1 arrived G after %dseconds%s\n", KMAG, sleep_time, KNRM);
      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- tell others i arrived");
        exit(-1);
      }
      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- waiting p4");
        exit(-1);
      }
      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases himself");
        exit(-1);
      }
      printf("\t\t\tP1 start moving back to B\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP1 arrived B after %dseconds%s\n", KMAG, sleep_time, KNRM);
      printf("\t\t\t\t%sP1 finished%s\t\n", KGRN, KNRM);
      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- tell others i arrived");
        exit(-1);
      }
      acquire.sem_num = p1;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p1 -- waiting p2");
        exit(-1);
      }
    }
    /* P1 code end */
    break;
    /* P2 code start */
  case 2:
    for (i = 0; i < 10; i++)
    {

      acquire.sem_num = p2;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p2 -- initial state");
        exit(-1);
      }
      release.sem_num = p2;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p2");
        exit(-1);
      }
      printf("\t\t\tP2 start moving forword to G\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP2 arrived G after %dseconds%s\n", KMAG, sleep_time, KNRM);
      acquire.sem_num = p2;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p2 -- tell p1 & p2 I arrived");
        exit(-1);
      }
      /* 
      here p2 will waits until p0 & p1 are arrived G when changes its semaphore value from 2 to 0
      */
      wait_flag1 = 1;
      wait_flag2 = 1;
      while (wait_flag1 != 0)
      {
        while (wait_flag2 != 0)
        {
          if ((wait_flag2 = semctl(semid, 0, GETVAL, 0)) == -1)
          {
            perror("semctl: GETVAL P0");
            exit(-1);
          }
        }
        if ((wait_flag1 = semctl(semid, 1, GETVAL, 0)) == -1)
        {
          perror("semctl: GETVAL P1");
          exit(-1);
        }
      }
      release.sem_num = p2;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- p2 releases himself");
        exit(-1);
      }
      printf("\t\t\tP2 start moving forword to H\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP2 arrived H after %dseconds%s\n", KMAG, sleep_time, KNRM);
      acquire.sem_num = p2;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p2 -- tell others I arrived");
        exit(-1);
      }
      wait_flag1 = 1;
      wait_flag2 = 1;
      while (wait_flag1 != 0)
      {
        while (wait_flag2 != 0)
        {
          if ((wait_flag2 = semctl(semid, 0, GETVAL, 0)) == -1)
          {
            perror("semctl: GETVAL P0");
            exit(-1);
          }
        }
        if ((wait_flag1 = semctl(semid, 1, GETVAL, 0)) == -1)
        {
          perror("semctl: GETVAL P1");
          exit(-1);
        }
      }
      release.sem_num = p3;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- p2 release p3");
        exit(-1);
      }
      release.sem_num = p4;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- p2 release p4");
        exit(-1);
      }
      release.sem_num = p5;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- p2 release p5");
        exit(-1);
      }

      acquire.sem_num = p2;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p2 -- waiting p4");
        exit(-1);
      }
      release.sem_num = p2;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases himself");
        exit(-1);
      }
      printf("\t\t\tP2 start moving forword to I\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP2 arrived I after %dseconds%s\n", KMAG, sleep_time, KNRM);
      wait_flag1 = 1;
      if (wait_flag1 != 0)
      {
        acquire.sem_num = p2;
        if (semop(semid, &acquire, 1) == -1)
        {
          perror("semop -- p2 -- tell others i finished");
          exit(-1);
        }
        if ((wait_flag1 = semctl(semid, 2, GETVAL, 0)) == -1)
        {
          perror("semctl: GETVAL P2");
          exit(-1);
        }
      }
      wait_flag1 = 1;
      wait_flag2 = 1;
      wait_flag3 = 1;
      wait_flag4 = 1;

      while (wait_flag1 != 0)
      {
        while (wait_flag2 != 0)
        {
          while (wait_flag3 != 0)
          {
            while (wait_flag4 != 0)
            {
              if ((wait_flag4 = semctl(semid, 1, GETVAL, 0)) == -1)
              {
                perror("semctl: GETVAL P1");
                exit(-1);
              }
            }
            if ((wait_flag3 = semctl(semid, 3, GETVAL, 0)) == -1)
            {
              perror("semctl: GETVAL P3");
              exit(-1);
            }
          }
          if ((wait_flag2 = semctl(semid, 4, GETVAL, 0)) == -1)
          {
            perror("semctl: GETVAL P4");
            exit(-1);
          }
        }
        if ((wait_flag1 = semctl(semid, 5, GETVAL, 0)) == -1)
        {
          perror("semctl: GETVAL P5");
          exit(-1);
        }
      }

      acquire.sem_num = p2;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p2 -- waiting p0");
        exit(-1);
      }
      printf("\t\t\tP2 start moving back to H\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP2 arrived H after %dseconds%s\n", KMAG, sleep_time, KNRM);
      acquire.sem_num = p2;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p2 -- waiting p3");
        exit(-1);
      }
      printf("\t\t\tP2 start moving back to G\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP2 arrived G after %dseconds%s\n", KMAG, sleep_time, KNRM);

      acquire.sem_num = p2;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p2 -- waiting p4");
        exit(-1);
      }
      printf("\t\t\tP2 start moving back to C\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP2 arrived C after %dseconds%s\n", KMAG, sleep_time, KNRM);

      printf("\t\t\t\t%sP2 finished%s\t\n", KGRN, KNRM);
      wait_flag1 = 1;
      while (wait_flag1 != 0)
      {
        if ((wait_flag1 = semctl(semid, 1, GETVAL, 0)) == -1)
        {
          perror("semctl: GETVAL P1");
          exit(-1);
        }
      }
      printf("\n\t\t---------%sItaration # %d finished%s-----------\n\n", KBLU, i + 1, KNRM);

      release.sem_num = p3;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p3");
        exit(-1);
      }
      release.sem_num = p4;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p4");
        exit(-1);
      }
      release.sem_num = p5;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p5");
        exit(-1);
      }
      release.sem_num = p0;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p0");
        exit(-1);
      }
      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p1");
        exit(-1);
      }
      release.sem_num = p0;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p0");
        exit(-1);
      }
      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p1");
        exit(-1);
      }
      release.sem_num = p2;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases p2");
        exit(-1);
      }
    }
    /* P2 code end */

    break;
    /* P3 code start */

  case 3:
    for (i = 0; i < 10; i++)
    {

      acquire.sem_num = p3;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p3 -- waiting p0");
        exit(-1);
      }
      release.sem_num = p3;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP3 releases himself");
        exit(-1);
      }
      printf("\t\t\tP3 start moving forword to H\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP3 arrived H after %dseconds%s\n", KMAG, sleep_time, KNRM);
      acquire.sem_num = p3;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p3 -- tell others i arrived");
        exit(-1);
      }
      wait_flag1 = 1;
      if ((wait_flag1 = semctl(semid, 4, GETVAL, 0)) == -1)
      {
        perror("semctl: GETVAL P4");
        exit(-1);
      }
      if (wait_flag1 != 0)
      { /*check if p4 finished or not*/
        release.sem_num = p0;
        if (semop(semid, &release, 1) == -1)
        {
          perror("\t\t\tP3 releases p0");
          exit(-1);
        }
      }
      else
      {
        release.sem_num = p1;
        if (semop(semid, &release, 1) == -1)
        {
          perror("\t\t\tP3 releases p1");
          exit(-1);
        }
        release.sem_num = p2;
        if (semop(semid, &release, 1) == -1)
        {
          perror("\t\t\tP3 releases p2");
          exit(-1);
        }
        release.sem_num = p3;
        if (semop(semid, &release, 1) == -1)
        {
          perror("\t\t\tP3 releases p3");
          exit(-1);
        }
        release.sem_num = p4;
        if (semop(semid, &release, 1) == -1)
        {
          perror("\t\t\tP3 releases p4");
          exit(-1);
        }
      }

      acquire.sem_num = p3;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p3 -- waiting p4 or himself");
        exit(-1);
      }
      release.sem_num = p3;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases himself");
        exit(-1);
      }
      printf("\t\t\tP3 start moving forword to I\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP3 arrived I after %dseconds%s\n", KMAG, sleep_time, KNRM);
      wait_flag1 = 1;
      if (wait_flag1 != 0)
      {
        acquire.sem_num = p3;
        if (semop(semid, &acquire, 1) == -1)
        {
          perror("semop -- p3 -- tell others i finished");
          exit(-1);
        }
        if ((wait_flag1 = semctl(semid, 3, GETVAL, 0)) == -1)
        {
          perror("semctl: GETVAL P3");
          exit(-1);
        }
      }
      wait_flag1 = 1;
      wait_flag2 = 1;
      wait_flag3 = 1;
      wait_flag4 = 1;

      while (wait_flag1 != 0)
      {
        while (wait_flag2 != 0)
        {
          while (wait_flag3 != 0)
          {
            while (wait_flag4 != 0)
            {
              if ((wait_flag4 = semctl(semid, 1, GETVAL, 0)) == -1)
              {
                perror("semctl: GETVAL P1");
                exit(-1);
              }
            }
            if ((wait_flag3 = semctl(semid, 2, GETVAL, 0)) == -1)
            {
              perror("semctl: GETVAL P2");
              exit(-1);
            }
          }
          if ((wait_flag2 = semctl(semid, 4, GETVAL, 0)) == -1)
          {
            perror("semctl: GETVAL P4");
            exit(-1);
          }
        }
        if ((wait_flag1 = semctl(semid, 5, GETVAL, 0)) == -1)
        {
          perror("semctl: GETVAL P5");
          exit(-1);
        }
      }
      release.sem_num = p0;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP3 releaases p0");
        exit(-1);
      }
      acquire.sem_num = p3;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p3 -- waiting p0");
        exit(-1);
      }
      printf("\t\t\tP3 start moving back to H\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP3 arrived H after %dseconds%s\n", KMAG, sleep_time, KNRM);

      acquire.sem_num = p3;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p3 -- waiting p5");
        exit(-1);
      }
      printf("\t\t\tP3 start moving back to D\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP3 arrived D after %dseconds%s\n", KMAG, sleep_time, KNRM);

      printf("\t\t\t\t%sP3 finished%s\t\n", KGRN, KNRM);

      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP3 releases p1");
        exit(-1);
      }
      release.sem_num = p2;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP3 releases p2");
        exit(-1);
      }
      acquire.sem_num = p3;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p3 -- waiting p0");
        exit(-1);
      }
    }
    /* P3 code end */

    break;
    /* P4 code start */

  case 4:
    for (i = 0; i < 10; i++)
    {
      acquire.sem_num = p4;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p4 -- waiting p2");
        exit(-1);
      }
      release.sem_num = p4;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP4 releases himself");
        exit(-1);
      }
      printf("\t\t\tP4 start moving forword to H\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP4 arrived H after %dseconds%s\n", KMAG, sleep_time, KNRM);
      acquire.sem_num = p4;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p4 -- tell others i arrived");
        exit(-1);
      }
      wait_flag1 = 1;
      if ((wait_flag1 = semctl(semid, 3, GETVAL, 0)) == -1)
      {
        perror("semctl: GETVAL P3");
        exit(-1);
      }
      if (wait_flag1 != 0)
      { /*check if p3 finished or not*/
        release.sem_num = p0;
        if (semop(semid, &release, 1) == -1)
        {
          perror("\t\t\tP4 releases p0");
          exit(-1);
        }
      }
      else
      {
        release.sem_num = p1;
        if (semop(semid, &release, 1) == -1)
        {
          perror("\t\t\tP4 releases p1");
          exit(-1);
        }
        release.sem_num = p2;
        if (semop(semid, &release, 1) == -1)
        {
          perror("\t\t\tP4 releases p2");
          exit(-1);
        }
        release.sem_num = p3;
        if (semop(semid, &release, 1) == -1)
        {
          perror("\t\t\tP4 releases p3");
          exit(-1);
        }
        release.sem_num = p4;
        if (semop(semid, &release, 1) == -1)
        {
          perror("\t\t\tP4 releases p4");
          exit(-1);
        }
      }

      acquire.sem_num = p4;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p4 -- waiting p3 or himself");
        exit(-1);
      }

      release.sem_num = p4;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases himself");
        exit(-1);
      }
      printf("\t\t\tP4 start moving forword to I\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP4 arrived I after %dseconds%s\n", KMAG, sleep_time, KNRM);
      wait_flag1 = 1;
      if (wait_flag1 != 0)
      {
        acquire.sem_num = p4;
        if (semop(semid, &acquire, 1) == -1)
        {
          perror("semop -- p4 -- tell others i finished");
          exit(-1);
        }
        if ((wait_flag1 = semctl(semid, 4, GETVAL, 0)) == -1)
        {
          perror("semctl: GETVAL P4");
          exit(-1);
        }
      }

      wait_flag1 = 1;
      wait_flag2 = 1;
      wait_flag3 = 1;
      wait_flag4 = 1;

      while (wait_flag1 != 0)
      {
        while (wait_flag2 != 0)
        {
          while (wait_flag3 != 0)
          {
            while (wait_flag4 != 0)
            {
              if ((wait_flag4 = semctl(semid, 1, GETVAL, 0)) == -1)
              {
                perror("semctl: GETVAL P1");
                exit(-1);
              }
            }
            if ((wait_flag3 = semctl(semid, 2, GETVAL, 0)) == -1)
            {
              perror("semctl: GETVAL P2");
              exit(-1);
            }
          }
          if ((wait_flag2 = semctl(semid, 3, GETVAL, 0)) == -1)
          {
            perror("semctl: GETVAL P3");
            exit(-1);
          }
        }
        if ((wait_flag1 = semctl(semid, 5, GETVAL, 0)) == -1)
        {
          perror("semctl: GETVAL P5");
          exit(-1);
        }
      }

      acquire.sem_num = p4;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p4 -- waiting p0");
        exit(-1);
      }

      printf("\t\t\tP4 start moving back to H\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP4 arrived H after %dseconds%s\n", KMAG, sleep_time, KNRM);

      acquire.sem_num = p4;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p4 -- waiting p5");
        exit(-1);
      }
      printf("\t\t\tP4 start moving back to E\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP4 arrived E after %dseconds%s\n", KMAG, sleep_time, KNRM);

      printf("\t\t\t\t%sP4 finished%s\t\n", KGRN, KNRM);

      release.sem_num = p1;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP4 releases p1");
        exit(-1);
      }
      release.sem_num = p2;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP4 releases p2");
        exit(-1);
      }
      acquire.sem_num = p4;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p4 -- waiting p2");
        exit(-1);
      }
    }
    /* P4 code end */

    break;
    /* P5 code start */

  case 5:
    for (i = 0; i < 10; i++)
    {
      acquire.sem_num = p5;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p5 -- waiting p2");
        exit(-1);
      }
      release.sem_num = p5;
      if (semop(semid, &release, 1) == -1)
      {
        perror("error -- releases himself");
        exit(-1);
      }
      printf("\t\t\tP5 start moving forword to I\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP5 arrived I after %dseconds%s\n", KMAG, sleep_time, KNRM);
      wait_flag1 = 1;
      if (wait_flag1 != 0)
      {
        acquire.sem_num = p5;
        if (semop(semid, &acquire, 1) == -1)
        {
          perror("semop -- p5 -- tell others i finished");
          exit(-1);
        }
        if ((wait_flag1 = semctl(semid, 5, GETVAL, 0)) == -1)
        {
          perror("semctl: GETVAL P5");
          exit(-1);
        }
      }
      wait_flag1 = 1;
      wait_flag2 = 1;
      wait_flag3 = 1;
      wait_flag4 = 1;

      while (wait_flag1 != 0)
      {
        while (wait_flag2 != 0)
        {
          while (wait_flag3 != 0)
          {
            while (wait_flag4 != 0)
            {
              if ((wait_flag4 = semctl(semid, 1, GETVAL, 0)) == -1)
              {
                perror("semctl: GETVAL P1");
                exit(-1);
              }
            }
            if ((wait_flag3 = semctl(semid, 2, GETVAL, 0)) == -1)
            {
              perror("semctl: GETVAL P2");
              exit(-1);
            }
          }
          if ((wait_flag2 = semctl(semid, 3, GETVAL, 0)) == -1)
          {
            perror("semctl: GETVAL P4");
            exit(-1);
          }
        }
        if ((wait_flag1 = semctl(semid, 4, GETVAL, 0)) == -1)
        {
          perror("semctl: GETVAL P4");
          exit(-1);
        }
      }

      acquire.sem_num = p5;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p5 -- waiting p0");
        exit(-1);
      }

      printf("\t\t\tP5 start moving back to F\n");
      sleep_time = rand() % sleep_limit + 1;
      sleep(sleep_time);
      printf("\t\t\t%sP5 arrived F after %dseconds%s\n", KMAG, sleep_time, KNRM);

      printf("\t\t\t\t%sP5 finished%s\n", KGRN, KNRM);

      release.sem_num = p3;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP5 releases p3");
        exit(-1);
      }
      release.sem_num = p4;
      if (semop(semid, &release, 1) == -1)
      {
        perror("\t\t\tP5 releases p4");
        exit(-1);
      }
      acquire.sem_num = p5;
      if (semop(semid, &acquire, 1) == -1)
      {
        perror("semop -- p5 -- waiting p2");
        exit(-1);
      }
    }
    /* P5 code end */
    break;
  }
  exit(0);
}
