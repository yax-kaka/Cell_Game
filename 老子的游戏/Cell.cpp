#include "Cell.h"
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include<iostream>
using namespace std;



///////////////ȫ�ֱ���/////////////////////
const int Width = 640;                //��Ļ��
const int Height = 480;               //��Ļ��
const int Cell_num = 300;             //ϸ����
const int Pha_num = 50;               //����ϸ����
const int Cancer_num = 30;       // ���ϸ����
const int number_of_change = 10;   //ϸ���ɸı����
static clock_t start_time, end_time;     //ʱ��������
int DNA = 9;
int Sum = 0;
int last_cell = Cell_num;
int last_cancer;
int last_number_of_change = number_of_change;        //ʣ��ϸ���ɸı����
const double PI = 3.1415926;
#define DISTANCE(x1, y1, x2, y2)	(sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)))
Cell* cell = new Cell[Cell_num];
Phagocyte* phagocyte = new Phagocyte[Pha_num];
Cancer_Cell* cancer_cell = new Cancer_Cell[Cancer_num];
IMAGE		Map(Width, Height);

///////////ϸ�����ʵ�ֺ���//////////////
Cell::Cell() { life = 1; color = GREEN; x = 0; y = 0; }         //ϸ�����ʵ�ֺ���

Cell::~Cell() {};

void Cell::draw() {
	if (life == 0) return;
	setfillcolor(color);		// ��ϸ��
	fillcircle((int)x, (int)y, 2);
}

void Cell::move() {
	if (life == 0) return;
	switch (rand() % 4) {
	case 0: y -= 1; break;
	case 1:x += 1; break;
	case 2:y += 1; break;
	case 3:x -= 1; break;
	default:break;
	}
	if (x <= 0) x += 1;
	if (x >= Width) x -= 1;
	if (y <= 0)y += 1;
	if (y >= Height) y -= 1;
}

//////////////����ϸ����ĺ���////////////////////
Phagocyte::Phagocyte() :Cell() { color = BLUE; }

Phagocyte::~Phagocyte() {}

void Phagocyte::draw() {
	if (life == 0) return;
	setfillcolor(color);		// ������ϸ��
	fillcircle((int)x, (int)y, 3);
}

void Phagocyte::move() {
	if (life == 0) return;
	switch (rand() % 4) {
	case 0: y -= 2; break;
	case 1:x += 2; break;
	case 2:y += 2; break;
	case 3:x -= 2; break;
	}
	if (x <= 0) x += 2;
	if (x >= Width) x -= 2;
	if (y <= 0)y += 2;
	if (y >= Height) y -= 2;
}

void Phagocyte::attack(Cancer_Cell& a) {
	if (DISTANCE(x, y, a.x, a.y) <= 3 && a.life && life)
	{
		a.life = 0; life = 0;
	}
}

/////////ϸ����ʵ�ֺ���////////////
Cancer_Cell::Cancer_Cell() :Cell()             //ϸ����ĺ���
{
	color = RGB(rand() % 255, rand() % 255, rand() % 255);
	set_change(false);
}

Cancer_Cell::~Cancer_Cell() {}

void Cancer_Cell::move() {
	if (life == 0) return;
	double min_distance = 1000000;
	int nice = -1;
	//Զ��Tϸ��
	if (cancer_cell[0].change == true) {
		for (int i = 0; i < Pha_num; i++) {
			if (DISTANCE(x, y, phagocyte[i].x, phagocyte[i].y) < min_distance) {
				min_distance = DISTANCE(x, y, phagocyte[i].x, phagocyte[i].y);
				nice = i;
			}
		}
		if (x > phagocyte[nice].x&&x>0&&x<Width) x++;
		else x--;
		if (y > phagocyte[nice].y&&y>0&&y<Height) y++;
		else y--;
		return;
	}
	//�����ʻ�ϸ��
	for (int i = 0; i < Cell_num; i++)
	{
		if (DISTANCE(x, y, cell[i].x, cell[i].y) < min_distance && cell[i].life)
		{
			min_distance = DISTANCE(x, y, cell[i].x, cell[i].y);
			nice = i;
		}
	}
	if (x > cell[nice].x) x--;
	else x++;
	if (y > cell[nice].y) y--;
	else y++;
}

void Cancer_Cell::draw() {
	if (life == 0) return;
	setfillcolor(color);
	fillcircle((int)x, (int)y, 4);
}

void Cancer_Cell::infect(Cell& a) {
	if (life == 0) return;
	if (DISTANCE(x, y, a.x, a.y) <= 2)
	{
		a.life = 0;

	}

}

void Cancer_Cell::to_change() {
	end_time = clock();
	if ((end_time - start_time) > 1000) {
		change = false;
	}
}

/////////////////��������//////////////////
void set() {
	srand((unsigned)time(NULL));		// �����
	SetWorkingImage(&Map);
	setbkcolor(WHITE);					// ��ɫ����
	cleardevice();						// ��ʼ������
	SetWorkingImage();
	for (int i = 0; i < Cell_num; i++)
	{
		cell[i].x = (float)(rand() % Width);
		cell[i].y = (float)(rand() % Height);
	}
	for (int i = 0; i < Pha_num; i++)
	{
		phagocyte[i].x = (float)(rand() % Width);
		phagocyte[i].y = (float)(rand() % Height);
	}
	for (int i = 0; i < Cancer_num; i++)
	{
		cancer_cell[i].x = (float)(rand() % Width);
		cancer_cell[i].y = (float)(rand() % Height);
		cancer_cell[i].life = 0;
	}
}
/////////��ʱ����////////
void delay(DWORD ms) {
	static DWORD oldtime = GetTickCount();

	if (GetTickCount() - oldtime < ms)
		Sleep(10);
	oldtime = GetTickCount();

}
//////////��ʼ����///////////
void start() {
	setbkcolor(WHITE);		// ��ɫ����
	cleardevice();			// ��ʼ������
	settextcolor(RED);		// ������
	setbkmode(TRANSPARENT);
	settextstyle(20, 0, _T("����"));
	outtextxy(10, 10, _T("ϸ��.exev0.0.1(��������棩"));
	settextcolor(BLUE);		// ������
	outtextxy(60, 240, _T("����ȫ����������ڷ������ϸ�����������Ϳգ�Ӯ��ʤ����"));
	outtextxy(50, 290, _T("��С����ɫ������ϸ������������ͬ���ھ�������ֻ��9��DNA��"));
	outtextxy(70, 340, _T("������ʮ�λ��ḳ�����ϸ��������״̬������һ�룩"));
	outtextxy(100, 370, _T("���������������DNA���Ҽ��ı�ϸ��״̬��"));
	settextcolor(BLACK);	//������
	settextstyle(15, 0, _T("����"));
	outtextxy(250, 390, _T("���������ʼ��Ϸ"));
	settextstyle(20, 0, _T("����"));
	outtextxy(10, 450, _T("���� QQ: 308620434"));
	MOUSEMSG m;
	bool t = true;
	while (t) {
		if (MouseHit()) {
			m = GetMouseMsg();
			switch (m.uMsg)
			{
			case WM_LBUTTONDOWN:
			{t = false; }
			}
		}
	}
	
}
/////////////��ͼ����///////////
void out_text_test() {
	TCHAR s[5];
	_stprintf_s(s, _T("%d"), last_cell);
	settextstyle(20, 0, _T("����"));
	outtext(_T("��Ȼ����ϸ������"));
	outtext(s);
	_stprintf_s(s, _T("%d"), DNA);
	//settextstyle(10, 0, _T("����"));
	outtext( _T(" ʣ��DNA����"));
	outtext( s);
	_stprintf_s(s, _T("%d"), last_cancer);
	//settextstyle(10, 0, _T("����"));
	outtext( _T("  ���ϸ��������"));
	outtext( s);
	_stprintf_s(s, _T("%d"), last_number_of_change);
	outtext(_T(" ʣ�������"));
	outtext(s);
}
void Draw() {
	//SetWorkingImage(&Map);
	cleardevice();
	for (int i = 0; i < Cell_num; i++)
		cell[i].draw();
	for (int i = 0; i < Pha_num; i++)
		phagocyte[i].draw();
	for (int i = 0; i < Cancer_num; i++)
		cancer_cell[i].draw();
	out_text_test();
	/*
	TCHAR s[5];
	_stprintf_s(s, _T("%d"), last_cell);
	settextstyle(10, 0, _T("����"));
	outtextxy(10, 10, _T("��Ȼ����ϸ������"));
	outtext(s);
	outtextxy(100, 10, s);
	_stprintf_s(s, _T("%d"), DNA);
	settextstyle(10, 0, _T("����"));
	outtextxy(580, 20, _T("ʣ��DNA����"));
	outtextxy(630, 20, s);
	_stprintf_s(s, _T("%d"), last_cancer);
	settextstyle(10, 0, _T("����"));
	outtextxy(560, 10, _T("���ϸ��������"));
	outtextxy(630, 10, s);
	*/
}
//////////���к���///////
void run() {
	/// �ж�ϸ��״̬////
	cancer_cell[0].to_change();
	/////////////ϸ����ɫ/////////
	for (int i = 0; i < Cancer_num; i++)
	{
		if (cancer_cell[0].retn_change() == true) cancer_cell[i].color = RED;
		else cancer_cell[i].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	}
	////////////////ʣ���ʻ�ϸ����///////////
	last_cell = 0;
	for (int i = 0; i < Cell_num; i++)
	{
		if (cell[i].life) last_cell++;
	}
	//////////////////ͨ���ж�///////////
	int s = 0;
	for (int i = 0; i < Cell_num; i++)
	{
		if (cell[i].life) s++;
	}
	if (s == 0) {
		MessageBox(NULL, _T("��ϲͨ��"), _T("��ϲͨ��,��Ϸ����"), MB_OK | MB_ICONEXCLAMATION);	// ͨ����Ϸ������
		closegraph();
		exit(0);
	}
	/////////////DNA����/////////////
	last_cancer = 0;
	for (int i = 0; i < Cancer_num; i++)
	{
		if (cancer_cell[i].life) last_cancer++;
	}
	if (DNA == 0 && last_cancer == 0) {
		MessageBox(NULL, _T("DNA�����˷�����"), _T("DNA�����˷����ˣ���Ϸ����"), MB_OK | MB_ICONEXCLAMATION);	// DNA�Ĺ��ˣ�����
		closegraph();
		exit(0);
	}

	////////����/////////
	for (int i = 0; i < Cancer_num; i++)
	{
		if (cancer_cell[i].life == 0) continue;
		for (int j = 0; j < Cell_num; j++)
			cancer_cell[i].infect(cell[j]);
	}
	for (int i = 0; i < Pha_num; i++)
	{
		for (int j = 0; j < Cancer_num; j++)
			phagocyte[i].attack(cancer_cell[j]);
	}
	////////�ƶ�����/////////
	for (int i = 0; i < Cell_num; i++) {
		if (cell[i].life)
		{
			cell[i].move();
		}

	}
	for (int i = 0; i < Pha_num; i++)
	{
		if (phagocyte[i].life)
		{
			phagocyte[i].move();
		}
	}
	for (int i = 0; i < Cancer_num; i++)
	{
		cancer_cell[i].move();
	}

}
//////////���û�ı�ϸ��///////
void set_can() {
	MOUSEMSG m;
	if (DNA > 0|| last_cancer>0) {
		if (MouseHit()) {
			m = GetMouseMsg();
			switch (m.uMsg)
			{
			case WM_RBUTTONDOWN:{
				if (last_cancer > 0 && last_number_of_change > 0 && cancer_cell[0].retn_change() != true) {
					cancer_cell[0].set_change(true);
					last_number_of_change--;
					start_time = clock();
				}
				break;
			}
			case WM_LBUTTONDOWN:
			{
				if (DNA <= 0)break;
				cancer_cell[Sum].x = m.x;
				cancer_cell[Sum].y = m.y;
				cancer_cell[Sum].life = 1;
				Sum++; DNA--; break;
			}
			default:break;
			}
		}
	}
	FlushMouseMsgBuffer();
}
