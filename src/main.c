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
    memset(matr, -1, sizeof(matr));

    /* start game */
    for (;;)
    {
        int direction, row, col;
        char *line = NULL;
        size_t sline = 0;

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
        printf("Player %c turn.Enter coordinates:\n", 'A' + player);
        getline(&line, &sline, stdin);
        if (feof(stdin))
        {
            fprintf(stderr, "end game: stdin was closed\n");
            _exit(EXIT_FAILURE);
        }
        sscanf(line, "%d%d%d", &direction, &row, &col);

        /* analyze */

        /* next player */
        player = (player + 1) % NPLAYERS;
    }

    /* result */

    return (EXIT_SUCCESS);
}
