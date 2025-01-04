#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <time.h>

#define HEIGHT 20
#define WIDTH 40

struct Score
{
    char name[100];
    char date[100];
    int score;
};

// Koordinat ekor ular
int snakeTailX[100], snakeTailY[100];

// Panjang ekor ular
int snakeTailLen;

// skor dan flags
int gameover, key, score;

// Koordinat kepala dan buah
int x, y, fruitx, fruity;

// Banyaknya trap dan koordinat trap
int trapCount, trapx[3], trapy[3];

// Fungsi-fungsi
void setup();
void draw();
void input();
void logic();
void menu();
void PlayGame();
void ScoreSort();
void ScoreDisplay();
void FileUpdate(int score, char *name);
int condition(int x, int y);

int main()
{
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    int choice;
    setup();

    // Menu game
    printf("Welcome to Bite and Beware game\n");
    menu();
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        PlayGame();
        break;
    case 2:
        ScoreDisplay();
        main();
    case 3:
        return 0;
    default:
        printf("Pilihan ini tidak tersedia!\n");
        Sleep(1000);
    }

    do
    {
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif
        printf("Welcome back!!\nDo you want to play the game again?\n");
        menu();
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            setup();
            PlayGame();
            break;
        case 2:
            ScoreDisplay();
            break;
        case 3:
            break;
        default:
            printf("Pilihan ini tidak tersedia!\n");
            Sleep(1000);
        }
    } while (choice != 3);

    return 0;
}

void menu()
{
    printf("\t1. Start the game\n");
    printf("\t2. Scoreboard\n");
    printf("\t3. Exit\n");
    printf("Enter your choice here : ");
}

void setup()
{
    gameover = 0;
    snakeTailLen = 0;
    key = '\0';
    trapCount = 0;

    // Koordinat awal ular
    x = WIDTH / 2;
    y = HEIGHT / 2;

    // Koordinat awal buah
    fruitx = rand() % WIDTH;
    fruity = rand() % HEIGHT;
    while (fruitx == 0)
        fruitx = rand() % WIDTH;

    while (fruity == 0)
        fruity = rand() % HEIGHT;

    // Koordinat awal ekor ular
    for (int i = 0; i < snakeTailLen; i++)
    {
        snakeTailX[i] = -1;
        snakeTailY[i] = -1;
    }

    // Koorinat awal trap
    for (int i = 0; i < 3; i++)
    {
        trapx[i] = -1;
        trapy[i] = -1;
    }

    // Koordinat awal trap
    // trapx = rand() % WIDTH;
    // trapy = rand() % HEIGHT;
    // while (trapx == 0)
    //     trapx = rand() % WIDTH;
    // while (trapy == 0)
    //     trapy = rand() % HEIGHT;
    // while (trapx == fruitx && trapy == fruity)
    // {
    //     trapx = rand() % WIDTH;
    //     trapy = rand() % HEIGHT;
    //     while (trapx == 0)
    //         trapx = rand() % WIDTH;
    //     while (trapy == 0)
    //         trapy = rand() % HEIGHT;
    // }

    score = 0;
}

void draw()
{
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif

    // Dinding atas
    for (int i = 0; i < WIDTH + 2; i++)
        printf("-");
    printf("\n");

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j <= WIDTH; j++)
        {
            // Dinding samping
            if (j == 0 || j == WIDTH)
                printf("#");

            // Kepala ular
            if (i == y && j == x)
                printf("O");

            // Munculin buah
            else if (i == fruity && j == fruitx)
                printf("*");

            // Munculin trap
            else if (i == trapy[0] && j == trapx[0])
                printf("X");

            else if (i == trapy[1] && j == trapx[1])
                printf("X");

            else if (i == trapy[2] && j == trapx[2])
                printf("X");

            // Badan ular
            else
            {
                int prTail = 0;
                for (int k = 0; k < snakeTailLen; k++)
                {
                    if (snakeTailX[k] == j && snakeTailY[k] == i)
                    {
                        printf("o");
                        prTail = 1;
                    }

                }

                // for (int k = 0; k < trapCount; k++)
                // {
                //     if (trapy[k] == i && trapx[k] == j)
                //     {
                //         printf("X");
                //         prTail = 1;
                //     }
                // }

                if (!prTail)
                    printf(" ");
            }

        }
        printf("\n");
    }

    // Dinding bawah
    for (int i = 0; i < WIDTH + 2; i++)
        printf("-");
    printf("\n");

    // Skor dan cara bermain
    printf("score = %d", score);
    printf("\n");
    printf("Press W, A, S, D for movement.\n");
    printf("Press X to quit the game.\n");
}

void input()
{
    if (kbhit())
    {
        switch (tolower(getch()))
        {
        case 'a':
            if (key != 2)
                key = 1;
            break;
        case 'd':
            if (key != 1)
                key = 2;
            break;
        case 'w':
            if (key != 4)
                key = 3;
            break;
        case 's':
            if (key != 3)
                key = 4;
            break;
        case 'x':
            gameover = 1;
            break;
        }
    }
}

void logic()
{
    int prevX = snakeTailX[0];
    int prevY = snakeTailY[0];
    int prev2X, prev2Y;
    snakeTailX[0] = x;
    snakeTailY[0] = y;
    for (int i = 1; i < snakeTailLen; i++)
    {
        prev2X = snakeTailX[i];
        prev2Y = snakeTailY[i];
        snakeTailX[i] = prevX;
        snakeTailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Ganti arah ular
    switch (key)
    {
    case 1:
        x--;
        break;
    case 2:
        x++;
        break;
    case 3:
        y--;
        break;
    case 4:
        y++;
        break;
    default:
        break;
    }

    // Game Over
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        gameover = 1;

    // Check kalau nabrak badan ular atau trap
    for (int i = 0; i < snakeTailLen; i++)
    {
        if (snakeTailX[i] == x && snakeTailY[i] == y)
            gameover = 1;

        for(int j = 0; j < trapCount; j++)
        {
            if (snakeTailX[i] == trapx[j] && snakeTailY[i] == trapy[j])
                gameover = 1;
        }
    }

    // Check kalau ular nyampe buah dan update skor
    if (x == fruitx && y == fruity)
    {
        fruitx = rand() % WIDTH;
        fruity = rand() % HEIGHT;
        do
        {
            while (fruitx == 0)
                fruitx = rand() % WIDTH;

            while (fruity == 0)
                fruity = rand() % HEIGHT;
        } while(condition(fruitx, fruity));
        score += 20;
        snakeTailLen++;

        // check untuk nambahin trap
        if (score - 40 % 120 == 0)
        {
            trapx[trapCount] = rand() % WIDTH;
            trapy[trapCount] = rand() % HEIGHT;
            while (trapx[trapCount] == 0)
                trapx[trapCount] = rand() % WIDTH;
            while (trapy[trapCount] == 0)
                trapy[trapCount] = rand() % HEIGHT;
            while (trapx[trapCount] == fruitx && trapy[trapCount] == fruity)
            {
                trapx[trapCount] = rand() % WIDTH;
                trapy[trapCount] = rand() % HEIGHT;
                while (trapx[trapCount] == 0)
                    trapx[trapCount] = rand() % WIDTH;
                while (trapy[trapCount] == 0)
                    trapy[trapCount] = rand() % HEIGHT;
            }
            trapCount++;
            if (trapCount == 3)
                trapCount = 0;
        }
    }
}

int condition(int x, int y)
{
    if (x == trapx[0] && y == trapy[0])
        return 1;
    
    else if (x == trapx[1] && y == trapy[1])
        return 1;

    else if (x == trapx[2] && y == trapy[2])
        return 1;

    return 0;
}

void PlayGame()
{
    char name[100];
    while (!gameover)
    {
        draw();
        input();
        logic();
        Sleep(50);
    }
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printf("--Game Over--!\n");
    printf("Your final score is %d\n", score);
    printf("Enter your username here : ");
    scanf("%s", name);
    getchar();

    FileUpdate(score, name);

    printf("Press any key to continue\n");
    getch();
}

void ScoreDisplay()
{
    FILE *f = fopen("score.txt", "r");

    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    struct Score temp;
    printf("%-25s  %-30s  %-7s\n", "Name", "Date", "Score");
    printf("------------------------------------------------------------------\n");
    while (fscanf(f, "%[^,],%[^,],%d\n", temp.name, temp.date, &temp.score) != EOF)
    {
        printf("%-25s  %-30s  %-7d\n", temp.name, temp.date, temp.score);
    }

    fclose(f);

    printf("Press any key to back to main menu...");
    getch();
}

void ScoreSort()
{
    FILE *f = fopen("score.txt", "r");

    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    struct Score *scores;
    int size = 0;

    struct Score temp;
    while (fscanf(f, "%[^,],%[^,],%d\n", temp.name, temp.date, &temp.score) != EOF)
    {
        struct Score *new_scores = (struct Score *)realloc(scores, (size + 1) * sizeof(struct Score));
        if (new_scores == NULL)
        {
            printf("Error: gagal mengalokasikan memori\n");
            free(scores);
            fclose(f);
        }

        scores = new_scores;

        scores[size] = temp;
        size++;
    }

    fclose(f);

    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (scores[i].score < scores[j].score)
            {
                struct Score temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }

    f = fopen("score.txt", "w");

    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    for (int i = 0; i < size; i++)
    {
        fprintf(f, "%s,%s,%d\n", scores[i].name, scores[i].date, scores[i].score);
    }
}

void FileUpdate(int score, char *name)
{
    FILE *f = fopen("score.txt", "a");

    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    time_t current_time;
    time(&current_time);

    struct tm *local_time = localtime(&current_time);

    fprintf(f, "%s,%d-%02d-%02d %02d:%02d:%02d,%d\n",
            name,
            local_time->tm_year + 1900, // Year since 1900
            local_time->tm_mon + 1,     // Month (0-11, so add 1)
            local_time->tm_mday,
            local_time->tm_hour,
            local_time->tm_min,
            local_time->tm_sec,
            score);

    fclose(f);
}