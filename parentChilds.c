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
            char str[1];
            sprintf(str, "%d", i);
            char *arg_list[] = {"project2", str, NULL};
            if (execv("/home/project2", arg_list) == -1)
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