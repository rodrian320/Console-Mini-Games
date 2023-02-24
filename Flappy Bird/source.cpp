#include <graphics.h>
#include <conio.h>
#include <ctime>
#include <stdio.h>

int nScreenWidth = 1000;
int nScreenHeight = 600;

int count = 0;
int pipex[500], rad[1000], grassx[1000];
int height[500][2];
bool GameOver = true;
float g = 9.8, t = 0;
float posx = 250, posy = 200, vy = 2;
bool isup = false;
ExMessage msg;

void DrawPipe(int x, int y, int id);
void DrawBird(float x, float y);
void DrawGrass(int x, int y, int r);

int main()
{
    srand(time(0));
    for (int i = 1; i < 480; i++)
    {
        height[i][0] = (rand() % 400) + 80;
        height[i][1] = (rand() % 400) + 80;
        if (height[i][0] + height[i][1] < 400 || height[i][0] > 450 - height[i][1])
            i--;
        pipex[i] = 1000;
        rad[i] = (rand() % 60 + 40);
        grassx[i] = 1000;
    }
    initgraph(nScreenWidth, nScreenHeight);
    setbkcolor(RGB(100, 200, 250));
    cleardevice();
    char title[20] = "Flappy Bird";
    char over[20] = "Game Over";
    settextstyle(50, 0, title);
    outtextxy(nScreenWidth / 2 - textwidth(title) / 2, nScreenHeight / 2 - textheight(title), title);
    while (1)
    {
        if (peekmessage(&msg, EM_MOUSE) && msg.message == WM_LBUTTONDOWN)
        {
            GameOver = false;
            cleardevice();
            DrawBird(posx, posy);
            break;
        }
    }
    BeginBatchDraw();
    int i = 1, flag = 0;
    
    while (!GameOver)
    {
        
        cleardevice();
        char num[5];
        sprintf(num, "%d", count);
        setbkmode(TRANSPARENT);
        settextstyle(50, 0, num);
        outtextxy(50, 50, num);
        for (int j = 1; j <= i; j++)
        {
            grassx[j] -= 1;
            DrawGrass(grassx[j], 580, rad[j]);
            pipex[j] -= 2;
            DrawPipe(pipex[j], height[j][0], 1);
            DrawPipe(pipex[j], height[j][1], 2); 
        }
        
        vy += g / 40;
        posy += vy * 0.5;
        DrawBird(250, posy);
        Sleep(10);
        for (int j = 1; j <= i; j++)
        {
            if (pipex[j] < 220)
                count = j;
            if (pipex[j] >= 240 && pipex[j] <= 280)
                if (posy + 18 >= 600 - height[j][1] - 25 || posy - 18 <= height[j][0] + 25)
                {
                    flag = 1;
                    break;
                }
        }
        if (flag)
            break;
        FlushBatchDraw();

        if (peekmessage(&msg, EM_MOUSE) && msg.message == WM_LBUTTONDOWN && posy >= 60)
            vy = -10;
        if (pipex[i] <= 750)
            i++;
        if (posy >= 580)
            break;

    }

    Sleep(800);
    EndBatchDraw();
    settextstyle(50, 0, over);
    settextcolor(RED);
    outtextxy(nScreenWidth / 2 - textwidth(over) / 2, nScreenHeight / 2 - textheight(over), over);
    Sleep(2000);
    closegraph();
    return 0;
}

void DrawPipe(int x, int h, int id)
{
    setfillcolor(RGB(80, 225, 0));
    if (id == 1)
    {
        fillrectangle(x, 0, x + 40, h);
        fillrectangle(x-10, h, x + 50, h+25);
    }
    else
    {
        fillrectangle(x, 600, x + 40, 600-h);
        fillrectangle(x - 10, 600-h, x + 50, 600- h - 25);
    }

}

void DrawBird(float x, float y)
{
    setfillcolor(RGB(255, 100, 40));
    solidcircle(x, y, 18);
}
void DrawGrass(int x, int y, int r)
{
    setfillcolor(RGB(100, 200, 30));
    solidcircle(x, y, r);
}