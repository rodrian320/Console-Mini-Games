#include <graphics.h>
#include <time.h>
#include <cmath>


int mx, my, premx, premy, mvx, mvy;
int num = 500;
float fric = 0.96f;

struct Ball
{
	float x, y;
	float vx, vy;
	COLORREF col;
	int rad;

}ball[600];

void Init();
void DrawBall();
void MouseMove();
void BallMove();
void Delay(DWORD ms);

int main()
{
	Init();

	while (1)
	{
		DrawBall();
		MouseMove();
		BallMove();
	}

	EndBatchDraw();
	closegraph();

	return 0;
}

void Init()
{
	srand(time(0));
	for (int i = 0; i < num; i++)
	{
		ball[i].x = (rand() % 900 + 100);
		ball[i].y = (rand() % 500 + 100);
		ball[i].vx = float(cos(float(i))) * (rand() % 34);
		ball[i].vy = float(sin(float(i))) * (rand() % 34);
		ball[i].col = RGB(rand() % 256, rand() % 256, rand() % 256);
		ball[i].rad = rand() % 6 + 2;
	}
	MouseMove();
	premx = mx, premy = my;

	initgraph(1000, 600);
	BeginBatchDraw();
}

void DrawBall()
{
	cleardevice();
	for (int i = 0; i < num; i++)
	{
		setfillcolor(ball[i].col);
		solidcircle(ball[i].x, ball[i].y, ball[i].rad);
	}
	FlushBatchDraw();
	Delay(5);
}

void MouseMove()
{
	MOUSEMSG msg;
	while (MouseHit())
	{
		msg = GetMouseMsg();
		if (msg.uMsg == WM_MOUSEMOVE)
		{
			mx = msg.x;
			my = msg.y;
		}
	}
}

void BallMove()
{
	float attdis = 500;
	float puldis = 30;
	float disdis = 200;
	
	mvx = mx - premx;
	mvy = my - premy;

	premx = mx, premy = my;

	for (int i = 0; i < num; i++)
	{
		float x = ball[i].x;
		float y = ball[i].y;
		float vx = ball[i].vx;
		float vy = ball[i].vy;

		float dx = x - mx;
		float dy = y - my;
		float d = sqrt(dx * dx + dy * dy);

		if (d != 0)
		{
			dx = dx / d;
			dy = dy / d;
		}
		else
		{
			dx = 0;
			dy = 0;
		}

		if (d < attdis)
		{
			float toacc = (1 - d / attdis) * 1000 * 0.0014f;
			vx = vx - dx * toacc;
			vy = vy - dy * toacc;
		}
		if (d < puldis)
		{
			float pulacc = (1 - d / puldis) * 10;
			vx = vx + dx * pulacc + 0.5f - float(rand()) / RAND_MAX;
			vy = vy + dy * pulacc + 0.5f - float(rand()) / RAND_MAX;
		}
		if (d < disdis)
		{
			float disacc = (1 - d / disdis) * 1000 * 0.00026f;
			vx = vx - dx * disacc;
			vy = vy - dy * disacc;
		}

		vx *= fric, vy *= fric;
		float avex = abs(vx);
		float avey = abs(vy);
		float avgv = (avex + avey) * 0.5f;

		if (avex < 0.1)
			vx = vx * float(rand()) / RAND_MAX * 3;
		if (avey < 0.1)
			vy = vy * float(rand()) / RAND_MAX * 3;

		float nexx = x + vx;
		float nexy = y + vy;

		if (nexx >= 1000)
		{
			nexx = 1000;
			vx = -vx;
		}
		else if (nexx <= 0)
		{
			nexx = 0;
			vx = -vx;
		}
		if (nexy >= 600)
		{
			nexy = 1000;
			vy = -vy;
		}
		else if (nexx <= 0)
		{
			nexy = 0;
			vy = -vy;
		}

		ball[i].vx = vx;
		ball[i].vy = vy;
		ball[i].x = nexx;
		ball[i].y = nexy;
	}

}

void Delay(DWORD ms)
{
	static DWORD oldtime = GetTickCount();
	while (GetTickCount() - oldtime < ms)
		Sleep(1);
	oldtime = GetTickCount();
}