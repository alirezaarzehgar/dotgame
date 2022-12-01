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
    int direc, row, col, player = 1, sumscore = 0,
        mode = NORMAL_MODE - 1, scores[NPLAYERS] = {0};
    char *line;
    size_t sline;

    if (argc == 2 && !strncmp("pro", argv[1], 4))
        mode = PRO_MODE - 1;

    for (;;) {
        for (int i = 0; i <= mode; i++) {
            for (int j = 0; j <= mode; j++)
                printf("*%s", matr[i][j][HOR] ? "--" : "  ");
            putchar('\n');

            for (int j = 0; j <= mode; j++)
                printf("%c%c ", matr[i][j][VER] ? '|' : ' ',
                                matr[i][j][WIN] ? I2C(matr[i][j][WIN]) : ' ');
            putchar('\n');
        }

    if (sumscore == mode * mode)
        break;

    retry:
        line = NULL;
        printf("Player %c turn.Enter coordinates:\n", I2C(player));
        getline(&line, &sline, stdin);
        if (feof(stdin))
            return (0);
        sscanf(line, "%d%d%d", &direc, &row, &col);

        row--, col--;
        if (!(direc == 0 || direc == 1)
            || (direc && row >= mode)
            || (!direc && col >= mode)
            || row < 0 || row > mode
            || col < 0 || col > mode
            || matr[row][col][direc]) {
            fprintf(stderr, "Invalid input. retry\n");
            goto retry;
        }
        matr[row][col][direc] = player;

        if (find_square(direc, &row, &col)) {
            matr[row][col][WIN] = player;
            scores[player-1]++;
            sumscore++;
            continue;
        }

        if (++player > NPLAYERS)
            player = 1;
    }

    for (int i = 0; i < NPLAYERS; i++)
        printf("score(%c): %d; ", 'A'+i, scores[i]);
    putchar('\n');
    return (0);
}
