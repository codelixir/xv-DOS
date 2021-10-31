/* DOS CREATE */

#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "Usage: setpriority priority pid\n");
        exit(1);
    }
    set_priority(atoi(argv[1]), atoi(argv[2]));
    exit(0);
}