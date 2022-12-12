#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#define NORMAL_MODE     4
#define PRO_MODE        6
#define NPLAYERS        2
#define I2C(I)          (I + 'A' - 1)

int matr[PRO_MODE][PRO_MODE][3] = {0};
enum {HOR, VER, WIN};

char *colored(char *msg, int id)
{
    /* Max lenght for colored double dash or pipe */
    static char text[20];
    sprintf(text, "\033[%dm%s\033[0m", 30 + id, msg);
    return text;
}

void print_matrix(int mode) {
    char *line, *name;

    for (int row = 0; row <= mode; row++) {
        for (int col = 0; col <= mode; col++) {
            line = colored("--", matr[row][col][HOR]);
            printf("*%s", matr[row][col][HOR] ? line : "  ");
        }
        putchar('\n');

        for (int col = 0; col <= mode; col++) {
            line = colored("|", matr[row][col][VER]);
            printf("%s%c ", matr[row][col][VER] ? line : " ",
                            matr[row][col][WIN] ? I2C(matr[row][col][WIN]) : ' ');
        }
        putchar('\n');
    }
}

int is_square(int row, int col)
{
    return matr[row][col][HOR] && matr[row][col][VER]   /* current point */
            && matr[row][col+1][VER]    /* right point */
            && matr[row+1][col][HOR]    /* down point */
            && !matr[row][col][WIN];
}

int is_invalid_input(int mode, int direc, int row, int col)
{
    return (direc != HOR && direc != VER)
            || (direc == VER && row > mode-1)
            || (direc == HOR && col > mode-1)
            || (row < 0 || row > mode)
            || (col < 0 || col > mode)
            || matr[row][col][direc] > 0;
}

int seeded_random(int max)
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    srand(ts.tv_nsec + rand());
    return rand() % max;
}

int check_sides(int row, int col,
                    int *orow, int *ocol, int *odirec)
{
    int side = 0;

    *orow = row;
    *ocol = col;

    if (matr[row][col][HOR])
        side++;
    else
        *odirec = HOR;

    if (matr[row][col][VER])
        side++;
    else
        *odirec = VER;

    if (matr[row][col + 1][VER]) {
        side++;
    } else {
        *ocol = col + 1;
        *odirec = VER;
    }

    if (matr[row + 1][col][HOR]) {
        side++;
    } else {
        *orow = row + 1;
        *odirec = HOR;
    }

    (*orow)++, (*ocol)++;
    return side;
}

void fake_player(int mode, int *odirec, int *orow, int *ocol)
{
    for (int row = 0; row < mode; row++) {
        for (int col = 0; col < mode; col++) {
            if (check_sides(row, col, orow, ocol, odirec) == 3)
                return;
        }
    }

    *odirec = seeded_random(2);
    *orow = seeded_random(mode + 2);
    *ocol = seeded_random(mode + 2);
}

int main(int argc, char *const *argv)
{
    int direc, row, col, wins, retry = 0, player = 1, faker = 0, opt,
        sumscore = 0, mode = NORMAL_MODE - 1, scores[NPLAYERS] = {0};
    char *line;
    size_t sline;

    while ((opt = getopt(argc, argv, "pf")) != -1)
    {
        switch (opt)
        {
        case 'p':
            mode = PRO_MODE - 1;
            break;

        case 'f':
            faker = 1;
            break;

        case '?':
            fprintf(stderr, "Usage: %s\n"
            "  -p\t\tEnable professional mode\n"
            "  -f\t\tEnable fake player\n", argv[0]);
            return -1;
        }
    }

    for (;;) {
        if (!retry && player != faker)
            print_matrix(mode);

        if (sumscore == mode * mode)
            break;

        if (player == faker) {
            fake_player(mode, &direc, &row, &col);
        } else {
            line = NULL;
            printf("Player %c turn.Enter coordinates:\n", I2C(player));
            getline(&line, &sline, stdin);
            sscanf(line, "%d%d%d", &direc, &row, &col);
            if (feof(stdin))
                return (0);
        }

        row--, col--;
        if (is_invalid_input(mode, direc, row, col)) {
            if (player != faker)
                fprintf(stderr, "Invalid input. retry\n");
            retry = 1;
            continue;
        }
        retry = 0;
        matr[row][col][direc] = player;

        wins = 0;
        if (is_square(row, col)) {
            matr[row][col][WIN] = player;
            wins++;
        }
        if (direc) {
            if (col-- >= 0 && is_square(row, col)) {
                matr[row][col][WIN] = player;
                wins++;
            }
        } else {
            if (row-- >= 0 && is_square(row, col)) {
                matr[row][col][WIN] = player;
                wins++;
            }
        }
        scores[player-1] += wins;
        sumscore += wins;

        if (!wins && ++player > NPLAYERS)
            player = 1;
    }

    for (int id = 0; id < NPLAYERS; id++)
        printf("score(%c): %d; ", 'A' + id, scores[id]);
    putchar('\n');
    return 0;
}
