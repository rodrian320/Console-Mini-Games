/*
#include <graphics.h>
#include <stdio.h>
#include <ctime>


#define nWidth 1200
#define nHeight 800
#define item_column_height 120		// �������߶�
#define time_unit 1000
#define max_slot 3
#define max_level 3
#define rand_buff_num 3
#define magic_lifetime 10
#define kind_of_magic 8


int nLevel = 1;
int currentslot1 = 0, currentslot2 = max_slot + 1;
int nTimeCounter = 40;
float ball_speed_controller = 3.0;
bool slot_ert[5], slot_123[5];
bool pause = false;
bool GameOver = true;


struct Board
{
	int thick, len;
	float y, speed;
	COLORREF color;
};
Board board1 = { 20, 120, 400, 0.5, WHITE }, board2 = { 20, 120, 400, 0.5, WHITE };


struct Ball
{
	int rad;
	float x, y, xspeed, yspeed;
	COLORREF color;
};
Ball ball = { 11, 400, 500, 0.2, 0.1, RED };


struct Player
{
	int score, mul;
	int buffs[10][2];
};
Player player1 = { 0, 1 }, player2 = { 0, 1 };


void Init();
void StartPage();
void Check();
void Restart();
void EndPage();
void ResetLevel();
void BetweenLevels();
void DrawEnvironment();
void TimeCount();
void DrawBoard();
void DrawBall();
void MoveBoard();
void MoveBall(); 
void DischargeMagic();
void SwitchMagic(int magic_id, int player_id);
void ResetMagic(int magic_id, int player_id);
void LoadItemImage(int id, int x, int y);
void DrawButtonOutline(int x, int y, COLORREF col = WHITE);
void PrintScore();
void PrintLevel();
void PrintTime();



int main()
{
	Init();

	while (!GameOver)
	{
		// ��Esc������ͣ�˵�
		if (GetAsyncKeyState(VK_ESCAPE))	
			EndPage();
	
		BeginBatchDraw();
		cleardevice();

		// �������
		TimeCount();
		DrawEnvironment();
		PrintTime();
		DrawBall();
		DrawBoard();

		// ���Ĺ���
		DischargeMagic();
		MoveBall();
		MoveBoard();

		EndBatchDraw();
		Check();
	}

	closegraph();
	return 0;
}


void Init()
{
	srand((unsigned int)time(NULL));
	initgraph(nWidth, nHeight);

	for (int i = 1; i <= max_slot; i++)
	{
		player1.buffs[i][0] = -1;
		player2.buffs[i][0] = -1;
	}

	StartPage();
	PrintLevel();
}


void StartPage()
{
	setfillcolor(WHITE);
	// P
	solidrectangle(200, 200, 230, 500);
	solidrectangle(231, 170, 351, 200);
	solidrectangle(352, 200, 382, 320);
	solidrectangle(231, 320, 351, 350);
	// o
	solidrectangle(431, 320, 511, 350);
	solidrectangle(400, 350, 430, 470);
	solidrectangle(431, 471, 511, 501);
	solidrectangle(512, 350, 542, 470);
	// n
	solidrectangle(600, 320, 630, 501);
	solidrectangle(631, 350, 661, 380);
	solidrectangle(662, 320, 712, 350);
	solidrectangle(713, 350, 743, 501);
	// g
	solidrectangle(831, 320, 911, 350);
	solidrectangle(800, 350, 830, 470);
	solidrectangle(831, 471, 911, 501);
	solidrectangle(912, 350, 942, 580);
	solidrectangle(831, 581, 911, 611);
	solidrectangle(800, 550, 830, 580);

	settextstyle(30, 0, "p");
	outtextxy(nWidth / 2 - textwidth("�� [Space] ��ʼ��Ϸ") / 2, 20, "�� [Space] ��ʼ��Ϸ");
	while (1) 
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			break;
	};

	cleardevice();
	GameOver = false;
	Sleep(20);
}


void DrawEnvironment()
{
	// ������
	setfillcolor(RGB(160 , 160, 160));
	solidrectangle(592, 0 , 606, 70);
	solidrectangle(592, 120, 606, 190);
	solidrectangle(592, 240, 606, 310);
	solidrectangle(592, 360, 606, 430);
	solidrectangle(592, 480, 606, 550);
	solidrectangle(592, 600, 606, 670);

	// ���Ƶ�����
	setfillcolor(RGB(0, 40, 80));
	solidrectangle(0, nHeight - item_column_height, nWidth, nHeight);
	char hint1[4] = "ERT";
	char hint2[4] = "123";
	char hint3[4] = "M,.";
	settextstyle(30, 0, "E");
	settextcolor(WHITE);
	for (int i = 0; i < rand_buff_num; i++)
	{
		DrawButtonOutline(80 + i * 100, nHeight - item_column_height/2 - 10, RGB(160, 230, 255));
		outtextxy(75 + i * 100, nHeight - 30, hint1[i]);
		if (player1.buffs[i][0] >= 0)
			LoadItemImage(player1.buffs[i+1][0], (i + 1) * 100 - 20, nHeight - 10 - item_column_height / 2);
	}
	for (int i = rand_buff_num; i > 0; i--)
	{
		DrawButtonOutline(nWidth + 20 - i * 100, nHeight - item_column_height / 2 - 10, RGB(230, 160, 255));
		outtextxy(nWidth + 5 - i * 100, nHeight - 30, hint2[rand_buff_num - i]);
		outtextxy(nWidth + 20 - i * 100, nHeight - 30, "/");
		outtextxy(nWidth + 35 - i * 100, nHeight - 30, hint3[rand_buff_num - i]);
		if (player2.buffs[rand_buff_num - i + 1][0] >= 0)
			LoadItemImage(player2.buffs[rand_buff_num - i + 1][0], nWidth + 20 - i * 100, nHeight - 10 - item_column_height / 2);
	}

	PrintScore();

}


void TimeCount()
{
	static int tidx = 0, star = 0;
	tidx++, star++;
	if (tidx == time_unit)
	{
		nTimeCounter--;
		tidx = 0;
	}
}


void PrintTime()
{
	char num[3];
	sprintf_s(num, "%d", nTimeCounter);
	settextstyle(60, 0, num);
	settextcolor(RED);
	outtextxy(nWidth / 2 - textwidth(num) / 2, nHeight - 100, num);

	settextstyle(20, 0, "p");
	settextcolor(WHITE);
	outtextxy(nWidth / 2 - textwidth("��Esc������ͣ�˵�") / 2, nHeight - 25, "��Esc������ͣ�˵�");
}


void MoveBoard()	
{
	// P1 WS���ƣ� P2 ��������
	if ((GetAsyncKeyState('w') & 0x8000 || GetAsyncKeyState('W') & 0x8000) && board1.y - board1.len >= 0 && board1.y <= nHeight - item_column_height)
		board1.y -= board1.speed;

	if ((GetAsyncKeyState('s') & 0x8000 || GetAsyncKeyState('S') & 0x8000) && board1.y <= nHeight - item_column_height && board1.y - board1.len >= 0)
		board1.y += board1.speed;

	if (GetAsyncKeyState(VK_UP) & 0x8000 && board2.y - board2.len >= 0 && board2.y <= nHeight - item_column_height)
		board2.y -= board2.speed;

	if (GetAsyncKeyState(VK_DOWN) & 0x8000 && board2.y <= nHeight - item_column_height && board2.y - board2.len >= 0)
		board2.y += board2.speed;

	// ����Խ�磬��λ
	if (board1.y - board1.len < 0)
		board1.y = board1.len + 1;

	if (board1.y > nHeight - item_column_height)
		board1.y = nHeight - item_column_height - 1;

	if (board2.y - board2.len < 0)
		board2.y = board2.len + 1;

	if (board2.y > nHeight - item_column_height)
		board2.y = nHeight - item_column_height - 1;
}


void MoveBall()
{
	if (pause)
	{
		Sleep(200);
		pause = false;
	}

	bool is1Miss = false, is2Miss = false;
	ball.x += ball.xspeed * ball_speed_controller;
	ball.y += ball.yspeed * ball_speed_controller;
	
	if (ball.y - ball.rad <= 0.01 || ball.y + ball.rad >= nHeight - item_column_height - 0.01)
		ball.yspeed = -ball.yspeed;

	if (board1.thick - 1 < ball.x - ball.rad &&ball.x - ball.rad < board1.thick + 1)
	{
		if(ball.y >= board1.y - board1.len - 4 && ball.y <= board1.y + 4)
			ball.xspeed = -ball.xspeed;
	}
	else if (ball.x + ball.rad > nWidth - board2.thick - 3 && ball.x + ball.rad < nWidth - board2.thick + 3)
	{
		if (ball.y >= board2.y - board2.len - 4 && ball.y <= board2.y + 4)
			ball.xspeed = -ball.xspeed;
	}
	else if (ball.x - ball.rad < board1.thick - 2 && (ball.y < board1.y - board1.len - 3 || ball.y > board1.y + 3))
	{
		if (ball.x - ball.rad <= 0.2)
		{
			is1Miss = true;
			player2.score += player2.mul;
		}
	}
	else if (ball.x + ball.rad > nWidth - board2.thick + 4 && (ball.y < board2.y - board2.len - 3 || ball.y > board2.y + 3))
	{
		if (ball.x + ball.rad >= nWidth - 1)
		{
			is2Miss = true;
			player1.score += player1.mul;
		}
	}

	// һ��ʧ�򣬴�ʧ��һ������
	if (is1Miss)
	{
		ball.x = rand() % 10 + board1.thick + 20;
		ball.y = rand() % 40 + board1.y - board2.len / 2;
		ball.xspeed = -ball.xspeed;
		pause = true;
	}
	else if (is2Miss)
	{
		ball.x = rand() % 10 + (nWidth - board2.thick - 25);
		ball.y = rand() % 40 + board2.y - board2.len / 2;
		ball.xspeed = -ball.xspeed;
		pause = true;
	}
}


void DischargeMagic()
{
	// P1�ż��ܣ�E,R,T��
	if ((GetAsyncKeyState(69) & 0x8000) && player1.buffs[1][0] >= 0 && slot_ert[1] == false)
	{
		slot_ert[1] = true;
		SwitchMagic(player1.buffs[1][0], 1);
	}
	if ((GetAsyncKeyState(82) & 0x8000) && player1.buffs[2][0] >= 0 && slot_ert[2] == false)
	{
		slot_ert[2] = true;
		SwitchMagic(player1.buffs[2][0], 1);
	}
	if ((GetAsyncKeyState(84) & 0x8000) && player1.buffs[3][0] >= 0 && slot_ert[3] == false)
	{
		slot_ert[3] = true;
		SwitchMagic(player1.buffs[3][0], 1);
	}
	// P2�ż��ܣ�1��2��3��
	if ((GetAsyncKeyState(VK_NUMPAD1) & 0x8000 || GetAsyncKeyState(77) & 0x8000) && player2.buffs[1][0] >= 0 && slot_123[1] == false)
	{
		slot_123[1] = true;
		SwitchMagic(player2.buffs[1][0], 2);
	}
	if ((GetAsyncKeyState(VK_NUMPAD2) & 0x8000 || GetAsyncKeyState(VK_OEM_COMMA) & 0x8000) && player2.buffs[2][0] >= 0 && slot_123[2] == false)
	{
		slot_123[2] = true;
		SwitchMagic(player2.buffs[2][0], 2);
	}
	if ((GetAsyncKeyState(VK_NUMPAD3) & 0x8000 || GetAsyncKeyState(VK_OEM_PERIOD) & 0x8000) && player2.buffs[3][0] >= 0 && slot_123[3] == false)
	{
		slot_123[3] = true;
		SwitchMagic(player2.buffs[3][0], 2);
	}

	for (int i = 1; i <= max_slot; i++)
	{
		if (slot_ert[i])
		{
			player1.buffs[i][1]--;
			setlinecolor(RGB(50, 50, 50));
			line(i * 100 - 60, 690, i * 100 + 20, 770);
			if (player1.buffs[i][1] == 0)
			{
				slot_ert[i] = false;
				line(i * 100 - 60, 690, i * 100 + 20, 770);
				ResetMagic(player1.buffs[i][0], 1);
				player1.buffs[i][1] = magic_lifetime * time_unit;
			}
		}
		if (slot_123[i])
		{
			player2.buffs[i][1]--;
			setlinecolor(RGB(50, 50, 50));
			line(nWidth - (max_slot - i) * 100 - 120, 690, nWidth - (max_slot - i) * 100 - 40, 770);
			if (player2.buffs[i][1] == 0)
			{
				slot_123[i] = false;
				line(nWidth - (max_slot - i) * 100 - 120, 690, nWidth - (max_slot - i) * 100 - 40, 770);
				ResetMagic(player2.buffs[i][0], 2);
				player2.buffs[i][1] = magic_lifetime * time_unit;
			}
		}
	}
}


void SwitchMagic(int magic_id, int player_id)
{
	settextcolor(WHITE);
	switch (magic_id)
	{
	case 0:
		if (player_id == 1)
			board1.len = 160;
		else
			board2.len = 160;
		break;
	case 1:
		if (player_id == 1)
			board2.len = 90;
		else
			board1.len = 90;
		break;
	case 2:
		if (player_id == 1)
			player1.mul = 3;
		else
			player2.mul = 3;
		break;
	case 3:
		if (player_id == 1)
			player2.mul = 0;
		else
			player1.mul = 0;
		break;
	case 4:
		if (player_id == 1)
			board2.color = BLACK;
		else
			board1.color = BLACK;
		break;
	case 5:
		if (player_id == 1)
			board2.speed = -board2.speed;
		else
			board1.speed = -board1.speed;
		break;
	case 6:
		if (player_id == 1)
			board1.speed = 1.0;
		else
			board2.speed = 1.0;
		break;
	case 7:
		if (player_id == 1)
			board2.speed = 0.2;
		else
			board1.speed = 0.2;
		break;
	case 8:
		ball.xspeed = -ball.xspeed;
		break;
	}
}


void ResetMagic(int magic_id, int player_id)
{
	switch (magic_id)
	{
	case 0:
		if (player_id == 1)
			board1.len = 120;
		else
			board2.len = 120;
		break;
	case 1:
		if (player_id == 1)
			board2.len = 120;
		else
			board1.len = 120;
		break;
	case 2:
		if (player_id == 1)
			player1.mul = 1;
		else
			player2.mul = 1;
		break;
	case 3:
		if (player_id == 1)
			player2.mul = 1;
		else
			player1.mul = 1;
		break;
	case 4:
		if (player_id == 1)
			board2.color = WHITE;
		else
			board1.color = WHITE;
		break;
	case 5:
		if (player_id == 1)
			board2.speed = -board2.speed;
		else
			board1.speed = -board1.speed;
		break;
	case 6:
		if (player_id == 1)
			board1.speed = 0.6;
		else
			board2.speed = 0.6;
		break;
	case 7:
		if (player_id == 1)
			board2.speed = 0.6;
		else
			board1.speed = 0.6;
		break;
	case 8:
		break;
	}
}


void PrintScore()
{
	char num1[5];
	char num2[5];
	sprintf_s(num1, "%d", player1.score);
	sprintf_s(num2, "%d", player2.score);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(50, 0, num1);
	outtextxy(nWidth / 4 - 25, 25, num1);
	settextstyle(50, 0, num2);
	outtextxy(nWidth / 4 * 3 - 25, 25, num2);
}


void BetweenLevels()
{
	cleardevice();
	DrawEnvironment();

	// ����������� * 3
	char hint1[4] = "ERT";
	char hint2[4] = "123";
	char hint3[4] = "769";

	settextstyle(20, 0, "p");
	outtextxy(150, nHeight / 2 + 27, "�� [Q] ȷ������ѡ��");
	outtextxy(830, nHeight / 2 + 27, "�� [0 / enter] ȷ������ѡ��");
	outtextxy(760, nHeight / 2 - 160, "���û��С���̣�123����[M] [,] [.] ����");

	settextstyle(30, 0, "E");
	settextcolor(RGB(200, 0, 0));
	outtextxy(nWidth/2 - textwidth("һ��ѡ�����Ų��˻�")/2, 150, "һ��ѡ�����Ų��˻�");
	outtextxy(nWidth / 2 - textwidth("����������Ϊ3") / 2, nHeight-60, "����������Ϊ3");
	settextcolor(WHITE);

	for (int i = 1; i <= rand_buff_num; i++)
	{
		DrawButtonOutline(50 + i * 100, nHeight/2 - 40);
		outtextxy(40 + i * 100, nHeight / 2, hint1[i-1]);
		outtextxy(40 + i * 100, nHeight / 2 - 120, hint3[i - 1]);
	}
	outtextxy(30, nHeight / 2 - 120, "cost: ");
	for (int i = rand_buff_num; i > 0; i--)
	{
		DrawButtonOutline(nWidth - 50 - i * 100, nHeight / 2 - 40);
		outtextxy(nWidth - 60 - i * 100, nHeight / 2, hint2[rand_buff_num - i]);
		outtextxy(nWidth - 60 - i * 100, nHeight / 2 - 120, hint3[i - 1]);
	}
	outtextxy(nWidth - 460, nHeight / 2 - 120, "cost: ");
	
	int rand1_id[5], rand2_id[5];
	for (int i = 1; i <= rand_buff_num; i++)
	{
		rand1_id[i] = rand() % (kind_of_magic + 1);
		rand2_id[i] = rand() % (kind_of_magic + 1);
		LoadItemImage(rand1_id[i], 50 + i * 100, nHeight / 2 - 40);
		LoadItemImage(rand2_id[i], nWidth - (rand_buff_num - i + 1) * 100 - 50, nHeight / 2 - 40);
	}

	// ������
	bool chosen_e = false, chosen_r = false, chosen_t = false, chosen_1 = false, chosen_2 = false, chosen_3 = false;
	bool p1_have_chosen = false, p2_have_chosen = false;

	while (nTimeCounter == 0) 
	{
		BeginBatchDraw();
		// P1ѡ��E,R,T)
		if ((GetAsyncKeyState(69) & 0x8000) && chosen_e == false && currentslot1 < 3 && p1_have_chosen == false)
		{
			chosen_e = true;
			currentslot1++;
			player1.buffs[currentslot1][0] = rand1_id[1];
			player1.buffs[currentslot1][1] = magic_lifetime * time_unit;
			player1.score -= 7;

			DrawButtonOutline(150, nHeight / 2 - 40, GREEN);
			LoadItemImage(rand1_id[1], currentslot1 * 100 - 20, nHeight - 10 - item_column_height / 2);
		}

		if ((GetAsyncKeyState(82) & 0x8000) && chosen_r == false && currentslot1 < 3 && p1_have_chosen == false)
		{
			chosen_r = true;
			currentslot1++;
			player1.buffs[currentslot1][0] = rand1_id[2];
			player1.buffs[currentslot1][1] = magic_lifetime * time_unit;
			player1.score -= 6;

			DrawButtonOutline(250, nHeight / 2 - 40, GREEN);
			LoadItemImage(rand1_id[2], currentslot1 * 100 - 20, nHeight - 10 - item_column_height / 2);
		}

		if ((GetAsyncKeyState(84) & 0x8000) && chosen_t == false && currentslot1 < 3 && p1_have_chosen == false)
		{
			chosen_t = true;
			currentslot1++;
			player1.buffs[currentslot1][0] = rand1_id[3];
			player1.buffs[currentslot1][1] = magic_lifetime * time_unit;
			player1.score -= 9;

			DrawButtonOutline(350, nHeight / 2 - 40, GREEN);
			LoadItemImage(rand1_id[3], currentslot1 * 100 - 20, nHeight - 10 - item_column_height / 2);
		}

		// P2ѡ��1��2��3��
		if ((GetAsyncKeyState(VK_NUMPAD1) & 0x8000 || GetAsyncKeyState(77) & 0x8000) && chosen_1 == false && currentslot2 > 1 && p2_have_chosen == false)
		{
			chosen_1 = true;
			currentslot2--;
			player2.buffs[max_slot - currentslot2 + 1][0] = rand2_id[1];
			player2.buffs[max_slot - currentslot2 + 1][1] = magic_lifetime * time_unit;
			player2.score -= 9;

			DrawButtonOutline(850, nHeight / 2 - 40, GREEN);
			LoadItemImage(rand2_id[1], nWidth+20 - currentslot2 * 100, nHeight - 10 - item_column_height / 2);
		}

		if ((GetAsyncKeyState(VK_NUMPAD2) & 0x8000 || GetAsyncKeyState(VK_OEM_COMMA) & 0x8000)  && chosen_2 == false && currentslot2 > 1 && p2_have_chosen == false)
		{
			chosen_2 = true;
			currentslot2--;
			player2.buffs[max_slot - currentslot2 + 1][0] = rand2_id[2];
			player2.buffs[max_slot - currentslot2 + 1][1] = magic_lifetime * time_unit;
			player2.score -= 6;

			DrawButtonOutline(950, nHeight / 2 - 40, GREEN);
			LoadItemImage(rand2_id[2], nWidth + 20 - currentslot2 * 100, nHeight - 10 - item_column_height / 2);
		}

		if ((GetAsyncKeyState(VK_NUMPAD3) & 0x8000 || GetAsyncKeyState(VK_OEM_PERIOD) & 0x8000) && chosen_3 == false && currentslot2 > 1 && p2_have_chosen == false)
		{
			chosen_3 = true;
			currentslot2--;
			player2.buffs[max_slot - currentslot2 + 1][0] = rand2_id[3];
			player2.buffs[max_slot - currentslot2 + 1][1] = magic_lifetime * time_unit;
			player2.score -= 7;

			DrawButtonOutline(1050, nHeight / 2 - 40, GREEN);
			LoadItemImage(rand2_id[3], nWidth + 20 - currentslot2 * 100, nHeight - 10 - item_column_height / 2);
		}

		// P1��P2ȷ��ѡ�����
		if ((GetAsyncKeyState(81) & 0x8000) && nTimeCounter == 0)
		{
			p1_have_chosen = true;
			settextstyle(30, 0, "p");
			settextcolor(RGB(230, 255, 145));
			outtextxy(100, nHeight / 2 + 50, "Player 1 ѡ�����");
		}

		if ((GetAsyncKeyState(VK_NUMPAD0) & 0x8000 || GetAsyncKeyState(VK_RETURN) & 0x8000 ) && nTimeCounter == 0)
		{
			p2_have_chosen = true;
			settextstyle(30, 0, "p");
			settextcolor(RGB(255, 230, 145));
			outtextxy(800, nHeight / 2 + 50, "Player 2 ѡ�����");
		}

		setfillcolor(BLACK);
		solidrectangle(250, 0, 350, 100);
		solidrectangle(850, 0, 950, 100);
		PrintScore();
		EndBatchDraw();

		if (p1_have_chosen && p2_have_chosen)
		{
			nTimeCounter = 60;
			ResetLevel();
			return;
		}
	}
}


void DrawBoard()
{
	setfillcolor(board1.color);
	solidrectangle(0, board1.y - board1.len, board1.thick, board1.y);
	setfillcolor(board2.color);
	solidrectangle(nWidth - board2.thick, board2.y - board2.len, nWidth, board2.y);
}


void DrawBall()
{
	setfillcolor(ball.color);
	solidcircle(ball.x, ball.y, ball.rad);
}


void DrawButtonOutline(int x, int y, COLORREF col)
{
	setlinestyle(PS_SOLID, 5);
	setlinecolor(col);
	rectangle(x-40, y-40, x+40, y+40);
}


void LoadItemImage(int id, int x, int y)
{
	// ���ؼ���ͼ��
	settextstyle(30, 0, "p");
	settextcolor(WHITE);
	switch (id)
	{
	case 0:
		setfillcolor(RGB(190, 0, 20));
		solidrectangle(x - 40, y - 40, x + 40, y + 40);
		outtextxy(x - 30, y - 35, "�ұ�");
		outtextxy(x - 30, y + 5, "����");
		break;
	case 1:
		setfillcolor(RGB(0, 130, 185));
		solidrectangle(x - 40, y - 40, x + 40, y + 40);
		outtextxy(x - 30, y - 35, "���");
		outtextxy(x - 30, y + 5, "����");
		break;
	case 2:
		setfillcolor(RGB(0, 45, 90));
		solidrectangle(x - 40, y - 40, x + 40, y + 40);
		outtextxy(x - 30, y - 35, "����");
		outtextxy(x - 30, y + 5, "�ӱ�");
		break;
	case 3:
		setfillcolor(RGB(250, 120, 30));
		solidrectangle(x - 40, y - 40, x + 40, y + 40);
		outtextxy(x - 30, y - 35, "�޵�");
		outtextxy(x - 30, y + 5, "����");
		break;
	case 4:
		setfillcolor(RGB(150, 0, 70));
		solidrectangle(x - 40, y - 40, x + 40, y + 40);
		outtextxy(x - 30, y - 35, "����");
		outtextxy(x - 30, y + 5, "����");
		break;
	case 5:
		setfillcolor(RGB(120, 170, 0));
		solidrectangle(x - 40, y - 40, x + 40, y + 40);
		outtextxy(x - 30, y - 35, "��ͷ");
		outtextxy(x - 30, y + 5, "ת��");
		break;
	case 6:
		setfillcolor(RGB(90, 0, 150));
		solidrectangle(x - 40, y - 40, x + 40, y + 40);
		outtextxy(x - 30, y - 35, "����");
		outtextxy(x - 30, y + 5, "����");
		break;
	case 7:
		setfillcolor(RGB(0, 120, 130));
		solidrectangle(x - 40, y - 40, x + 40, y + 40);
		outtextxy(x - 30, y - 35, "����");
		outtextxy(x - 30, y + 5, "��");
		break;
	case 8:
		setfillcolor(RGB(190, 0, 80));
		solidrectangle(x - 40, y - 40, x + 40, y + 40);
		outtextxy(x - 30, y - 35, "С��");
		outtextxy(x - 30, y + 5, "��ͷ");
		break;
	}
}


void ResetLevel()
{
	Sleep(300);
	ball.x = nWidth / 2;
	ball.y = nHeight / 2;
	
	cleardevice();
	if (nLevel <= max_level)
		PrintLevel();
}


void PrintLevel()
{
	char num[3];
	sprintf_s(num, "%d", nLevel);
	settextstyle(50, 0, "Level ");
	outtextxy(nWidth / 2 - 100, nHeight / 2 - 30, "Level ");
	outtextxy(nWidth / 2 + 50, nHeight / 2 - 30, num);
	Sleep(1000);
}


void Check()
{
	// �Ƿ񵽴����һ��
	if (nTimeCounter == 0 && nLevel <= max_level)
	{
		nLevel++;
		if (nLevel == max_level + 1)
			EndPage();
		if(GameOver == false)
			BetweenLevels();
		ball_speed_controller += 1.0;
	}
}


void EndPage()
{
	cleardevice();
	settextcolor(WHITE);
	settextstyle(30, 0, "p");
	setbkmode(TRANSPARENT);
	if (nLevel < max_level + 1)		// û�������һ�أ�����ѡ�������Ϸ
	{
		setfillcolor(RGB(255, 125, 45));
		solidrectangle(525, 300, 675, 380);
		outtextxy(600 - textwidth("������Ϸ") / 2, 325, "������Ϸ");
		outtextxy(600 - textwidth("�� [C]") / 2, 265, "�� [C]");
		settextstyle(20, 0, "p");
		outtextxy(600 - textwidth("С��ʾ��û����������Ϊ��������������") / 2, 25, "С��ʾ��û����������Ϊ��������������");
		outtextxy(600 - textwidth("���⣬��ܰ��ʾ��������ͨ����[esc]�˳��˲˵�") / 2, 50, "���⣬��ܰ��ʾ��������ͨ����[esc]�˳��˲˵�");
	}
	else		// �������һ�أ���ʾ Game Over
	{
		outtextxy(nWidth / 2 - textwidth("��Ϸ���� !") / 2, 300, "��Ϸ���� !");
		if (player1.score > player2.score)
			outtextxy(nWidth / 2 - textwidth("Player 1 Ӯ�� !!!") / 2, 350, "Player 1 Ӯ�� !!!");
		else if (player1.score < player2.score)
			outtextxy(nWidth / 2 - textwidth("Player 2 Ӯ�� !!!") / 2, 350, "Player 2 Ӯ�� !!!");
		else
			outtextxy(nWidth / 2 - textwidth("�ðɣ���ƽ��") / 2, 350, "�ðɣ���ƽ��");
	}

	settextstyle(30, 0, "p");
	setfillcolor(RGB(0, 130, 60));
	solidrectangle(400, 410, 550, 490);
	setfillcolor(RGB(145, 25, 0));
	solidrectangle(650, 410, 800, 490);
	outtextxy(475 - textwidth("�����ؿ�") / 2, 435, "�����ؿ�");
	outtextxy(475 - textwidth("�� [R]") / 2, 495, "�� [R]");
	outtextxy(725 - textwidth("ү������") / 2, 435, "ү������");
	outtextxy(725 - textwidth("�� [E]") / 2, 495, "�� [E]");
	settextstyle(20, 0, "p");
	outtextxy(nWidth / 2 - textwidth("������������������") / 2, 550, "������������������");
	outtextxy(nWidth / 2 - textwidth("���ȱ𼱣��Է�����") / 2, 580, "���ȱ𼱣��Է�����");
	outtextxy(nWidth / 2 - textwidth("��ͷת�򣺶Է����Ʒ���") / 2, 610, "��ͷת�򣺶Է����Ʒ���");
	outtextxy(nWidth / 2 - textwidth("�����ӱ��������ӷ�*3") / 2, 640, "�����ӱ��������ӷ�*3");
	outtextxy(nWidth / 2 - textwidth("����������ʹ�Է�Ŀä") / 2, 670, "����������ʹ�Է�Ŀä");

	while (1)
	{
		if ((GetAsyncKeyState('c') & 0x8000 || GetAsyncKeyState('C') & 0x8000))
			return;
		else if ((GetAsyncKeyState('e') & 0x8000 || GetAsyncKeyState('E') & 0x8000))
		{
			GameOver = true;
			return;
		}
		else if ((GetAsyncKeyState('r') & 0x8000 || GetAsyncKeyState('R') & 0x8000))
		{
			Restart();
			return;
		}
	}
}


void Restart()
{
	// ��ԭ��������
	cleardevice();
	GameOver = true;
	nLevel = 1;
	nTimeCounter = 40;
	player1.score = 0, player1.mul = 1, player2.score = 0, player2.mul = 1;
	ball.x = 400, ball.y = 500, ball.xspeed = 0.2, ball.yspeed = 0.1;
	for (int i = 1; i <= max_slot; i++)
	{
		player1.buffs[i][0] = -1;
		player2.buffs[i][0] = -1;
		slot_ert[i] = false, slot_123[i] = false;
	}
	board1.len = 120, board2.len = 120, board1.speed = 0.5, board2.speed = 0.5, board1.color = WHITE, board2.color = WHITE;
	ball_speed_controller = 3;
	currentslot1 = 0, currentslot2 = max_slot + 1;
	StartPage();
	PrintLevel();
}

*/