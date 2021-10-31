/* DOS CREATE */

#include "user/user.h"

void strace(int mask, char *command, char *args[])
{
    // exec(command, args);

    int pid = fork();

    if (pid == 0)
    {
        trace(mask);
        exec(command, args);
        exit(0);
    }
    wait(0);
    printf("\n\n");
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(2, "Usage: strace mask command [args]...\n");
        exit(1);
    }
    strace(atoi(argv[1]), argv[2], argv + 2);
    exit(0);
}