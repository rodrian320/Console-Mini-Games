#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <ctime>
#include <iostream>
#include <cstdlib>

int nGraphHeight = 16;
int nGraphWidth = 30;
int nCellSide = 30;
int nMineNum = 99;
int nMineSwept = 0;
bool bGameOver = false;
int MineMap[50][30];
int map[50][30];
int ifswept[50][30];
int nMineCorrect = 0;
int flag = 0;

void SetMine();
void GetMine(int x, int y);
void DrawAnswer(int x, int y, int isMine);
void DrawCells(int x, int y);
void DrawFlag(int x, int y);
void Span(int x, int y);
void Sweep();

int main()
{
	srand(time(0));
	initgraph(nGraphWidth* nCellSide + 180, nGraphHeight* nCellSide);

	char s1[60] = "WELCOME to -Mine Sweeper-";
	char s2[20] = "press 'E' to start";
	outtextxy(nGraphWidth * nCellSide / 2 - 20, nGraphHeight * nCellSide / 2 - 100, s1);
	outtextxy(nGraphWidth * nCellSide / 2, nGraphHeight * nCellSide / 2 - 80, s2);
	char key = getchar();
	if (key == 'e' || key == 'E')
	{
		cleardevice();
		bGameOver = false;
	}

	if (!bGameOver)
	{
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Sweep, (LPVOID)map, NULL, NULL);
		SetMine();
		for (int i = 0; i < nGraphWidth; i++)
			for (int j = 0; j < nGraphHeight; j++)
				GetMine(i, j);
		while (1)
		{
			outtextxy(nGraphWidth * nCellSide + 30, nCellSide + 10, "Flags:");
			outtextxy(nGraphWidth * nCellSide + 55, 2 * nCellSide, "0");

			for (int i = 0; i < nGraphWidth; i++)
				for (int j = 0; j < nGraphHeight; j++)
					DrawCells(i, j);

			Sweep();
			break;
		}

		Sleep(1000);
		cleardevice();

		if (nMineCorrect < nMineNum || flag == 1)
		{
			// show the map
			for (int i = 0; i < nGraphWidth; i++)
				for (int j = 0; j < nGraphHeight; j++)
					DrawAnswer(i, j, MineMap[i][j]);
			Sleep(1500);
		}
	
	}
	cleardevice();
	if (nMineCorrect < nMineNum)
	{
		settextcolor(RGB(200, 50, 0));
		outtextxy(nGraphWidth * nCellSide / 2 + 20, nGraphHeight * nCellSide / 2 - 100, "Boom!!!!");
		outtextxy(nGraphWidth * nCellSide / 2, nGraphHeight * nCellSide / 2 - 80, "Game Over :(");
	}
	else
	{
		settextcolor(RGB(50, 200, 0));
		outtextxy(nGraphWidth * nCellSide / 2 + 20, nGraphHeight * nCellSide / 2 - 100, "Wow!!!!");
		outtextxy(nGraphWidth * nCellSide / 2, nGraphHeight * nCellSide / 2 - 80, "You Win :)");
	}
	Sleep(1200);
	return 0;
}

void SetMine()
{
	int tempx, tempy;
	for (int i = 0; i < nMineNum; i++)
	{
		tempx = (rand() % 29);
		tempy = (rand() % 16);
		if (MineMap[tempx][tempy])
			i--;
		MineMap[tempx][tempy] = 1;
	}
}

void GetMine(int x, int y)
{
	int num = 0;
	int tempx = x, tempy = y;
	for (int i = 0; i < 8; i++)
	{
		switch (i)
		{
		case 0:   // 左上
			tempx = x - 1;
			tempy = y - 1;
			break;
		case 1:   // 左
			tempx = x - 1;
			tempy = y;
			break;
		case 2:   // 左下
			tempx = x - 1;
			tempy = y + 1;
			break;
		case 3:   // 上
			tempx = x;
			tempy = y - 1;
			break;
		case 4:   // 右上
			tempx = x + 1;
			tempy = y - 1;
			break;
		case 5:   // 右
			tempx = x + 1;
			tempy = y;
			break;
		case 6:   // 右下
			tempx = x + 1;
			tempy = y + 1;
			break;
		case 7:   // 下
			tempx = x;
			tempy = y + 1;
			break;
		}
		if (tempx < nGraphWidth && tempx >= 0 && tempy < nGraphHeight && tempy >= 0 && MineMap[tempx][tempy] == 1)
			num++;
	}
	if (MineMap[x][y] == 0)
		map[x][y] = num;
	else
		map[x][y] = -1;
}

void DrawAnswer(int x, int y, int isMine)
{
	setlinecolor(WHITE);
	if (isMine)
		setfillcolor(RGB(255, 0, 0));		//RED
	else
	{		
		setfillcolor(RGB(150, 150, 150));		//GREY
	}
	fillrectangle(x * nCellSide, y * nCellSide, x * nCellSide + nCellSide, y * nCellSide + nCellSide);	
	if (map[x][y] > 0)				// print the num of mine around
	{
		setbkmode(TRANSPARENT);
		char num = (char)(48+map[x][y]);
		settextcolor(WHITE);
		if (num == '1')
			settextcolor(BLUE);
		else if (num=='2')
			settextcolor(GREEN);
		else if (num == '3')
			settextcolor(RED);
		else if (num == '4')
			settextcolor(RGB(62, 0, 130));
		outtextxy(x * nCellSide + nCellSide / 2 - textwidth(num) / 2, y * nCellSide + nCellSide / 2 -textheight(num) / 2, num);
	}
	//Sleep(200);
}

void DrawCells(int x, int y)
{
	setlinecolor(WHITE);
	setfillcolor(BLACK);
	fillrectangle(x * nCellSide, y * nCellSide, x * nCellSide + nCellSide, y * nCellSide + nCellSide);
}

void DrawFlag(int x, int y)
{
	setlinecolor(WHITE);
	setfillcolor(RGB(150, 150, 150));
	setbkmode(TRANSPARENT);
	settextcolor(RED);
	fillrectangle(x * nCellSide, y * nCellSide, x * nCellSide + nCellSide, y * nCellSide + nCellSide);
	outtextxy(x * nCellSide + nCellSide / 2 - textwidth("???") / 2, y * nCellSide + nCellSide / 2 - textheight("???") / 2, "???");
}

void Sweep()   
{
	bool isSet = false;
	MOUSEMSG msg;
	int x = 0, y = 0;
	int clicktime = 0;
	while (1)
	{
		msg = GetMouseMsg();
		x = msg.x / nCellSide;
		y = msg.y / nCellSide;
		if (msg.mkLButton && x < nGraphWidth)
		{
			DrawAnswer(x, y, MineMap[x][y]);
			ifswept[x][y] = 1;
			if (MineMap[x][y])
				return;
			if (map[x][y] == 0)
			{
				ifswept[x][y] = 0;
				DrawAnswer(x, y, 0);
				Span(x, y);
				if (flag)
					return;
			}
		}
		if (msg.mkRButton && ifswept[x][y] != 1 && x < nGraphWidth)
		{
			if (ifswept[x][y] == 2)
			{
				ifswept[x][y] = 0;
				DrawCells(x, y);
				nMineSwept--;
				setlinecolor(BLACK);
				setfillcolor(BLACK);
				fillrectangle(nGraphWidth * nCellSide + 50, 2 * nCellSide, nGraphWidth * nCellSide + 70, 2 * nCellSide + nCellSide);
				outtextxy(nGraphWidth * nCellSide + 55, 2 * nCellSide, (char)(nMineSwept / 10 + 48));
				outtextxy(nGraphWidth * nCellSide + 62, 2 * nCellSide, (char)(nMineSwept % 10 + 48));
				continue;
			}
			DrawFlag(x, y);
			ifswept[x][y] = 2;
			nMineSwept++;
			setlinecolor(BLACK);
			setfillcolor(BLACK);
			fillrectangle(nGraphWidth * nCellSide + 50, 2 * nCellSide, nGraphWidth * nCellSide + 70, 2 * nCellSide + nCellSide);
			outtextxy(nGraphWidth * nCellSide + 55, 2 * nCellSide, (char)(nMineSwept / 10 + 48));
			outtextxy(nGraphWidth * nCellSide + 62, 2 * nCellSide, (char)(nMineSwept % 10 + 48));
			nMineCorrect = 0;
			for (int i = 0; i < nGraphWidth; i++)
			{
				for (int j = 0; j < nGraphHeight; j++)
				{
					if (ifswept[i][j] == 2 && MineMap[i][j] == 1)
						nMineCorrect++;
				}
			}
		}
		
		if (nMineCorrect == nMineNum)
			return;
	}
}

void Span(int x, int y)
{
	int tempx = x, tempy = y;
	if (tempx >= nGraphWidth || tempx < 0 || tempy >= nGraphHeight|| tempy < 0)
		return;
	if (map[tempx][tempy] > 0)
	{
		DrawAnswer(x, y, 0);
		return;
	}
	if (ifswept[tempx][tempy] == 1)
		return;
	ifswept[tempx][tempy] = 1;
	for (int i = 0; i < 8; i++)
	{
		switch (i)
		{
		case 0:   // 左上
			tempx = x - 1;
			tempy = y - 1;
			break;
		case 1:   // 左
			tempx = x - 1;
			tempy = y;
			break;
		case 2:   // 左下
			tempx = x - 1;
			tempy = y + 1;
			break;
		case 3:   // 上
			tempx = x;
			tempy = y - 1;
			break;
		case 4:   // 右上
			tempx = x + 1;
			tempy = y - 1;
			break;
		case 5:   // 右
			tempx = x + 1;
			tempy = y;
			break;
		case 6:   // 右下
			tempx = x + 1;
			tempy = y + 1;
			break;
		case 7:   // 下
			tempx = x;
			tempy = y + 1;
			break;
		}
		if ((i == 4 || i == 5 || i == 6) && tempx == nGraphWidth)
			continue;
		if (MineMap[tempx][tempy] && ifswept[tempx][tempy] == 2)
			flag = 1;
		DrawAnswer(tempx, tempy, MineMap[tempx][tempy]);
		Span(tempx, tempy);

	}
}