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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int i = 0;
int main()
{
    for (i = 0; i < 6; i++)
    {

        pid_t c = fork();
        if (c == 0) // fork returns 0 to the child process so it enters "if" block
        {
            if (i == 0)
            {
                printf("\n\n \t --------------%sSemaphores game started%s---------------\n", KGRN, KNRM);
                printf("%s\t--------------------------------------------------%s\n\n", KRED, KNRM);
            }
            char str[1];
            sprintf(str, "%d", i);
            char *arg_list[] = {"project2", str, NULL};
            if (execvp("./project2", arg_list) == -1)
            {
                perror("executing_error");
                exit(1);
            }
            else
            {
                exit(0);
            }
        }
    }

    return 0;
}