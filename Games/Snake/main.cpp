#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#if _WIN32
#include <conio.h>      // for Getch. (get users input)
#elif __linux__
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif
#include <time.h>
#include <string.h>

int i, j, height = 17, width = 30;
int flag, x, y, fruitx, fruity;
int gameover, score;

#if __linux__
int getch()
{
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
    return (buf);
}

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}
#endif
// Function to draw a boundary
void draw()
{
    system("clear");
    printf("\n");
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
            {
                if (j == 0)
                {
                    printf("%2d", i);
                }
                printf("#");
            }
            else
            {
                if (i == x && j == y)
                {
                    printf("0");
                }
                else if (i == fruitx && j == fruity)
                {
                    printf("*");
                }
                else
                {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
    for (int k = -1; k < width; k++)
    {
        printf("%d", k % 10);
    }
    printf("\nScore: %d\nPress x to quit game\n", score);
}

void setup()
{
    gameover = 0;
    // Stores height and width
    x = height / 2;
    y = width / 2;
    /* initialize random seed: */
    srand(time(NULL));
    do {
        fruitx = rand() % (height - 2) + 1;
        printf("%d ", fruitx);
    } while (fruitx == 0);
    do {
        fruity = rand() % (width - 2) + 1;
        printf("%d", fruity);
    } while (fruity == 0);
    score = 0;
}

void input()
{
    // if (_kbhit())
    // {
        char key = getch();
        // std::cout << key;
        switch (key)
        {
        case 'a':
            flag = 1;
            break;
        case 's':
            flag = 2;
            break;
        case 'd':
            flag = 3;
            break;
        case 'w':
            flag = 4;
            break;
        case 'x':
            gameover = 1;
            break;
        }
    // }
}

void logic()
{
    // usleep(100000);
    switch (flag)
    {
    case 1:
        y--;
        break;
    case 2:
        x++;
        break;
    case 3:
        y++;
        break;
    case 4:
        x--;
        break;
    default:
        break;
    }

    // If the game is over
    if (x <= 0 || x >= height - 1 || y <= 0 || y >= width - 1)
    {
        gameover = 1;
    }
    // If snake reaches the fruit
    // then update the score
    if (x == fruitx && y == fruity)
    {
        /* initialize random seed: */
        srand(time(NULL));
        do
        {
            fruitx = rand() % (height - 2) + 1;
            printf("%d ", fruitx);
        } while(fruitx == 0);
        // After eating the fruit
        // generate new fruit
        do
        {
            fruity = rand() % (width - 2) + 1;
            printf("%d", fruity);
        } while(fruitx == 0);
        score += 10;
    }
}

// Driver Code
int main(int argc, char const *argv[])
{
    // Generate boundary
    setup();
    // Until the game is over
    while (!gameover)
    {
        // Function call
        draw();
        input();
        logic();
    }
    return 0;
}