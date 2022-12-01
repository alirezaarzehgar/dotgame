#include <stdio.h>
#include <string.h>

#define NORMAL_MODE     4
#define PRO_MODE        6
#define NPLAYERS        2
#define HOR             0
#define VER             1

int main(int argc, char const *argv[])
{
    /* declaration */
    int direc, row, col, player = 1,
        mode = NORMAL_MODE, matr[PRO_MODE+1][PRO_MODE][2] = {0};
    char *line;
    size_t sline;

    /* parse arguments */
    if (argc == 2 && !strncmp("pro", argv[1], 4))
        mode = PRO_MODE;

    /* start game */
    for (;;)
    {
        /* print matrix */
        for (int i = 0; i < mode; i++)
        {
            /* print dots */
            for (int j = 0; j < mode; j++)
                printf("*%s", matr[i][j][HOR] ? "--" : "  ");
            putchar('\n');

            /* print horizental lines */
            for (int j = 0; j < mode; j++)
                printf("%c  ", matr[i][j][VER] ? '|' : ' ');
            putchar('\n');
        }

    retry:
        /* get data */
        line = NULL;
        printf("Player %c turn.Enter coordinates:\n", 'A' + player - 1);
        getline(&line, &sline, stdin);
        if (feof(stdin))
            return (0);
        sscanf(line, "%d%d%d", &direc, &row, &col);

        /* input validation */
        row--, col--;
        if (!(direc == 0 || direc == 1)
            || (direc && row >= mode - 1)
            || (!direc && col >= mode - 1)
            || row < 0 || col < 0
            || matr[row][col][direc])
        {
            fprintf(stderr, "Invalid input. retry\n");
            goto retry;
        }
        matr[row][col][direc] = player;

        /* show score */

        /* check for continue */

        /* next player */
        if (++player > NPLAYERS)
            player = 1;
    }

    /* result */

    return (0);
}
