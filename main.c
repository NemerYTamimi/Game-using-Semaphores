// colors
#include "local.h"

int i = 0;
int main()
{
    key_t ipc_key;
    ipc_key = ftok(".", 'S');
    pid_t c[6];
    int croaker, semid, status;
    for (i = 0; i < 6; i++)
    {

        c[i] = fork();
        if (c[i] == 0) // fork returns 0 to the child process so it enters "if" block
        {
            if (i == 0)
            {
                printf("\n\n \t--------------%sSemaphores game started%s---------------\n", KGRN, KNRM);
                printf("%s\t  --------------------------------------------------%s\n\n", KRED, KNRM);
            }
            char str[1];
            sprintf(str, "%d", i);
            char *arg_list[] = {"semaphore", str, NULL};
            if (execvp("./semaphore", arg_list) == -1)
            {
                perror("executing_error");
                exit(1);
            }
            exit(0);
        }
    }
    waitpid(c[1], &status, 0);
    if ((semid = semget(ipc_key, 3, 0)) == -1)
    {
        perror("semget -- obtaining semaphore");
        exit(2);
    }
    kill(c[0], 9);
    kill(c[1], 9);
    kill(c[2], 9);
    kill(c[3], 9);
    kill(c[4], 9);
    kill(c[5], 9);

    if (semctl(semid, 0, IPC_RMID, 0) != -1)
    {
        printf("\t\t     %sGame Ended and semaphores are removed%s\n", KYEL, KNRM);
    }
    exit(0);
}