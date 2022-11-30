#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define NORMAL_MODE 4
#define PRO_MODE 6
#define NPLAYERS 2

int main(int argc, char const *argv[])
{
    /* declaration */
    int player = 0, mode = NORMAL_MODE;
    int matr[PRO_MODE][PRO_MODE];

    /* parse arguments */
    if (argc == 2 && !strncmp("pro", argv[1], 4))
        mode = PRO_MODE;

    /* initialization */
    memset(matr, 0, sizeof(matr));

    /* start game */
    for (;;)
    {
        /* print matrix */
        for (int i = 0; i < mode; i++)
        {
            /* print dots */
            for (int j = 0; j < mode; j++)
            {
                putchar('*');
                /* -- or "  " */
            }

            /* print horizental lines */
            for (int i = 0; i < mode; i++)
            {
                /* "   " or "|  " */
            }

            putchar('\n');
        }

        /* get data */
        /* analyze */
    }

    /* result */

    return (EXIT_SUCCESS);
}
