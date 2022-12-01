#include <stdio.h>
#include <string.h>

#define NORMAL_MODE     4
#define PRO_MODE        6
#define NPLAYERS        2
#define I2C(I)           (I + 'A' - 1)

int matr[PRO_MODE+1][PRO_MODE][3] = {0};
enum {HOR, VER, WIN};

void print_matrix(int mode) {
    for (int i = 0; i <= mode; i++) {
        for (int j = 0; j <= mode; j++)
            printf("*%s", matr[i][j][HOR] ? "--" : "  ");
        putchar('\n');

        for (int j = 0; j <= mode; j++)
            printf("%c%c ", matr[i][j][VER] ? '|' : ' ',
                            matr[i][j][WIN] ? I2C(matr[i][j][WIN]) : ' ');
        putchar('\n');
    }
}

int is_square(int row, int col)
{
    return matr[row][col][HOR] && matr[row][col][VER]
            && matr[row][col+1][VER] && matr[row+1][col][HOR];
}

int main(int argc, char const *argv[])
{
    int direc, row, col, wins, retry = 0, player = 1,
        sumscore = 0, mode = NORMAL_MODE - 1, scores[NPLAYERS] = {0};
    char *line;
    size_t sline;

    if (argc == 2 && !strncmp("pro", argv[1], 4))
        mode = PRO_MODE - 1;

    for (;;) {
        if (!retry)
            print_matrix(mode);

        if (sumscore == mode * mode)
            break;

        wins = 0;
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
            retry = 1;
            continue;
        }
        retry = 0;
        matr[row][col][direc] = player;

        if (is_square(row, col)) {
            matr[row][col][WIN] = player;
            wins++;
        }
        if (direc) {
            if (col-- >= 0 && !matr[row][col][WIN]
                && is_square(row, col)) {
                matr[row][col][WIN] = player;
                wins++;
            }
        } else {
            if (row-- >= 0 && !matr[row][col][WIN]
                && is_square(row, col)) {
                matr[row][col][WIN] = player;
                wins++;
            }
        }
        scores[player-1] += wins;
        sumscore += wins;

        if (!wins && ++player > NPLAYERS)
            player = 1;
    }

    for (int i = 0; i < NPLAYERS; i++)
        printf("score(%c): %d; ", 'A' + i, scores[i]);
    putchar('\n');
    return (0);
}
