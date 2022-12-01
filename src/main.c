#include <stdio.h>
#include <string.h>

#define NORMAL_MODE     4
#define PRO_MODE        6
#define NPLAYERS        2
#define I2C(I)           (I + 'A' - 1)

int matr[PRO_MODE+1][PRO_MODE][3] = {0};
enum {HOR, VER, WIN};

#define SQUARE_COND(row, col)   (matr[row][col][HOR] && matr[row][col][VER] \
        && matr[row][col+1][VER] && matr[row+1][col][HOR])

int find_square(int direc, int *row, int *col)
{
    if (SQUARE_COND(*row, *col))
        return 1;

    if (direc)
        if ((*col)-- > 0)
            return SQUARE_COND(*row, *col);

    if ((*row)-- > 0)
        return SQUARE_COND(*row, *col);
}

int main(int argc, char const *argv[])
{
    /* declaration */
    int direc, row, col, player = 1, mode = NORMAL_MODE;
    char *line;
    size_t sline;

    /* parse arguments */
    if (argc == 2 && !strncmp("pro", argv[1], 4))
        mode = PRO_MODE;

    /* start game */
    for (;;) {
        /* print matrix */
        for (int i = 0; i < mode; i++) {
            for (int j = 0; j < mode; j++)
                printf("*%s", matr[i][j][HOR] ? "--" : "  ");
            putchar('\n');

            for (int j = 0; j < mode; j++)
                printf("%c%c ", matr[i][j][VER] ? '|' : ' ',
                                matr[i][j][WIN] ? I2C(matr[i][j][WIN]) : ' ');
            putchar('\n');
        }

    retry:
        /* get data */
        line = NULL;
        printf("Player %c turn.Enter coordinates:\n", I2C(player));
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
            || matr[row][col][direc]) {
            fprintf(stderr, "Invalid input. retry\n");
            goto retry;
        }
        matr[row][col][direc] = player;

        /* show score */
        if (find_square(direc, &row, &col))
            matr[row][col][WIN] = player;

        /* check for continue */

        /* next player */
        if (++player > NPLAYERS)
            player = 1;
    }

    /* result */

    return (0);
}
