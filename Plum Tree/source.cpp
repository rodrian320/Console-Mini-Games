#include <graphics.h>
#include <time.h>
#include <cmath>

float pi = 3.1415926;
float offsetAngle = pi / 6;
float shortenRate = 0.65;
int anim = 0;

void Init();
float Mapping(float input, float inputMin, float inputMax, float outputMin, float outputMax);
void Branch(float s_x, float s_y, float len, float angle, float thick, int gen);
float Rand(float min, float max);
void Click();

int main()
{
	srand(time(0));
	Init();

	while (1) 
	{
		Click();
	};

	EndBatchDraw();
	return 0;
}
void Init()
{
	initgraph(800, 600);
	cleardevice();
	BeginBatchDraw();
	Branch(400, 600, 0.45 * 600 * shortenRate, -pi / 2, 15 * shortenRate, 1);
	FlushBatchDraw();
}

float Mapping(float input, float inputMin, float inputMax, float outputMin, float outputMax)		// πÈ“ªªØ”≥…‰
{
	float output;
	if (abs(input - inputMin) < 0.000001)
		output = outputMin;
	else
		output = (input - inputMin) * (outputMax - outputMin) / (inputMax - inputMin) + outputMin;
	return output;
}

float Rand(float min, float max)
{
	float t = rand() / double(RAND_MAX);
	float out = Mapping(t, 0, 1, min, max);
	return out;
}

void Branch(float s_x, float s_y, float len, float angle, float thick, int gen)
{
	float e_x = s_x + len * cos(angle);
	float e_y = s_y + len * sin(angle);

	setlinestyle(PS_SOLID, thick);
	setlinecolor(HSVtoRGB(15, 0.65, 0.2 + gen * 0.05));
	line(s_x, s_y, e_x, e_y);

	int child_gen = gen + 1;
	float child_len = len * shortenRate;
	float lchild_len = child_len * Rand(0.9, 1.1);
	float rchild_len = child_len * Rand(0.9, 1.1);
	float mchild_len = child_len * Rand(0.9, 1.1);
	
	if (child_gen <= 9 && child_len >= 2)
	{
		float child_thick = thick * 0.8;
		if (child_thick < 2)
			child_thick = 2;
		if (Rand(0, 1) < 0.95)
			Branch(e_x, e_y, lchild_len, angle + offsetAngle*Rand(0.5, 1), child_thick, child_gen);
		if(Rand(0,1) < 0.95)
			Branch(e_x, e_y, rchild_len, angle - offsetAngle * Rand(0.5, 1), child_thick, child_gen);
		if (Rand(0, 1) < 0.85)
			Branch(e_x, e_y, mchild_len, angle + offsetAngle / 5 * Rand(-1, 1), child_thick, child_gen);
	}
	else
	{
		setlinestyle(PS_SOLID, 1);
		setfillcolor(HSVtoRGB(Rand(300, 350), Rand(0.2, 0.3), 1));
		if (child_len <= 4)
		{
			int r = rand() % 3 + 1;
			solidcircle(e_x, e_y, r);
		}
		else
		{
			int r = rand() % (int)child_len / 2 + 2;
			solidcircle(e_x, e_y, child_len / 2);
		}


	}
	if (anim == 1)
	{
		FlushBatchDraw();
		Sleep(0.9);
	}

}

void Click()
{
	ExMessage msg;
	if (peekmessage(&msg, EM_MOUSE))
	{
		switch (msg.message)
		{
		case WM_MOUSEMOVE:
			offsetAngle = Mapping(msg.x, 0, 800, pi / 10, pi / 4);
			shortenRate = Mapping(msg.y, 0, 600, 0.7, 0.3);
			break;
		case WM_LBUTTONDOWN:
			cleardevice();
			Branch(400, 600, 0.45 * 600 * shortenRate, -pi / 2, 15 * shortenRate, 1);
			FlushBatchDraw();
			break;
		case WM_RBUTTONDOWN:
			if (anim)
				anim = 0;
			else
				anim = 1;
			break;
		default:
			break;
		}
	}
	
}