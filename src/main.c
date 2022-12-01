#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define NORMAL_MODE     4
#define PRO_MODE        6
#define NPLAYERS        2
#define HOR             0
#define VER             1

int main(int argc, char const *argv[])
{
    /* declaration */
    int direction, row, col, *point, player = 1,
        mode = NORMAL_MODE, matr[PRO_MODE+1][PRO_MODE+1][2];
    char *line;
    size_t sline;

    /* parse arguments */
    if (argc == 2 && !strncmp("pro", argv[1], 4))
        mode = PRO_MODE;

    /* initialization */
    bzero(matr, sizeof(matr));

    /* start game */
    for (;;)
    {
        /* print matrix */
        for (int i = 0; i < mode; i++)
        {
            /* print dots */
            for (int j = 0; j < mode; j++)
                printf("*%s", matr[i][j+1][HOR] ? "--" : "  ");
            putchar('\n');

            /* print horizental lines */
            for (int j = 0; j < mode; j++)
                printf("%c  ", matr[i+1][j][VER] ? '|' : ' ');
            putchar('\n');
        }

    retry:
        /* get data */
        line = NULL;
        printf("Player %c turn.Enter coordinates:\n", 'A' + player - 1);
        getline(&line, &sline, stdin);
        if (feof(stdin))
        {
            fprintf(stderr, "end game: stdin was closed\n");
            _exit(EXIT_FAILURE);
        }
        sscanf(line, "%d%d%d", &direction, &row, &col);

        /* input validation */
        row--, col--;
        point = direction ? matr[row+1][col] : matr[row][col+1];
        if (!(direction == 0 || direction == 1)
            || (direction && row >= mode - 1)
            || (!direction && col >= mode - 1)
            || row < 0 || col < 0
            || point[direction])
        {
            fprintf(stderr, "Invalid input. retry\n");
            goto retry;
        }
        point[direction] = player;

        /* show score */

        /* check for continue */

        /* next player */
        if (++player > NPLAYERS)
            player = 1;
    }

    /* result */

    return (EXIT_SUCCESS);
}
