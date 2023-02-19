#include <stdio.h>
#include <graphics.h>

int nGraphHeight = 60;
int nGraphWidth = 60;
int nCellSide = 10;
bool bGameOver = true;
bool isSet = false;
int map[1100][1100];
int temp[1100][1100];

void DrawCells(int x, int y, int isAlive);
void init();
int getAliveCell(int x, int y);
void evolve();

int main()
{
	initgraph(nGraphWidth * nCellSide, nGraphHeight * nCellSide);
	
	char s1[60] = "WELCOME to -Conway's Game of Life-";
	char s2[20] = "press 'E' to start";
	outtextxy(nGraphWidth * nCellSide / 2 - 150, nGraphHeight * nCellSide / 2 - 100, s1);
	outtextxy(nGraphWidth * nCellSide / 2 - 100, nGraphHeight * nCellSide / 2 - 80, s2);

	char key = getchar();
	if (key == 'e' || key == 'E')
	{
		cleardevice();	
		bGameOver = false;
	}
	if (!bGameOver)
	{
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)init, (LPVOID)map, NULL, NULL);
		while (1)
		{
			// Draw the initial state
			for (int i = 0; i < nGraphWidth; i++)
				for (int j = 0; j < nGraphHeight; j++)
					DrawCells(i, j, map[i][j]);
			if (isSet) break;
		}
		cleardevice();
		outtextxy(nGraphWidth * nCellSide / 2-50, nGraphHeight * nCellSide / 2 - 100, "Start evolve :)");
		Sleep(1500);
		map[0][0] = 0;
		for (int i = 0; i < nGraphWidth; i++)
			for (int j = 0; j < nGraphHeight; j++)
				DrawCells(i, j, map[i][j]);
		Sleep(400);
		while (1)
		{
			evolve();			
			for (int i = 0; i < nGraphWidth; i++)
				for (int j = 0; j < nGraphHeight; j++)
					DrawCells(i, j, map[i][j]);
			Sleep(400);
		}

	}
	return 0;
}

void DrawCells(int x, int y, int isAlive)
{
	setlinecolor(WHITE);
	if (isAlive)
		setfillcolor(WHITE);
	else
		setfillcolor(BLACK);
	fillrectangle(x * 10, y * 10, x * 10 + 10, y * 10 + 10);

}

void init()   // set the initial state
{
	MOUSEMSG msg;
	int x = 0, y = 0;
	int clicktime = 0;
	while (1)
	{
		msg = GetMouseMsg();
		x = msg.x / nCellSide;
		y = msg.y / nCellSide;
		if (msg.mkLButton)
		{

			if (x == 0 && y == 0)
				isSet = true;			// click the corner to stop setting
			clicktime++;			
			if (clicktime % 2 == 1)		// double click to cancle
				map[x][y] = 1;
			else
				map[x][y] = 0;
		}

	}
}

int getAliveCell(int x, int y)
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
		if (tempx < nGraphWidth && tempx >= 0 && tempy < nGraphHeight && tempy >= 0 && map[tempx][tempy] == 1)
			num++;
	}
	return num;
}

void evolve()
{
	int i = 0, j = 0;
	for (i = 0; i < nGraphWidth; i++)
	{
		for (j = 0; j < nGraphHeight; j++)
		{
			int state;
			state = getAliveCell(i, j);
			if (state == 2)
				temp[i][j] = map[i][j];
			else if (state == 3)
				temp[i][j] = 1;
			else
				temp[i][j] = 0;
		}

	}
	for (i = 0; i < nGraphWidth; i++)
		for (j = 0; j < nGraphHeight; j++)
			map[i][j] = temp[i][j];
}
