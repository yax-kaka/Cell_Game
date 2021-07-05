#include "Cell.h"
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include<iostream>
using namespace std;



///////////////全局变量/////////////////////
const int Width = 640;                //屏幕长
const int Height = 480;               //屏幕宽
const int Cell_num = 300;             //细胞数
const int Pha_num = 50;               //吞噬细胞数
const int Cancer_num = 30;       // 最大癌细胞数
const int number_of_change = 10;   //细菌可改变次数
static clock_t start_time, end_time;     //时间间隔变量
int DNA = 9;
int Sum = 0;
int last_cell = Cell_num;
int last_cancer;
int last_number_of_change = number_of_change;        //剩余细菌可改变次数
const double PI = 3.1415926;
#define DISTANCE(x1, y1, x2, y2)	(sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)))
Cell* cell = new Cell[Cell_num];
Phagocyte* phagocyte = new Phagocyte[Pha_num];
Cancer_Cell* cancer_cell = new Cancer_Cell[Cancer_num];
IMAGE		Map(Width, Height);

///////////细胞类的实现函数//////////////
Cell::Cell() { life = 1; color = GREEN; x = 0; y = 0; }         //细胞类的实现函数

Cell::~Cell() {};

void Cell::draw() {
	if (life == 0) return;
	setfillcolor(color);		// 画细胞
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

//////////////吞噬细胞类的函数////////////////////
Phagocyte::Phagocyte() :Cell() { color = BLUE; }

Phagocyte::~Phagocyte() {}

void Phagocyte::draw() {
	if (life == 0) return;
	setfillcolor(color);		// 画吞噬细胞
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

/////////细菌类实现函数////////////
Cancer_Cell::Cancer_Cell() :Cell()             //细菌类的函数
{
	color = RGB(rand() % 255, rand() % 255, rand() % 255);
	set_change(false);
}

Cancer_Cell::~Cancer_Cell() {}

void Cancer_Cell::move() {
	if (life == 0) return;
	double min_distance = 1000000;
	int nice = -1;
	//远离T细胞
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
	//靠近鲜活细胞
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

/////////////////其他函数//////////////////
void set() {
	srand((unsigned)time(NULL));		// 随机数
	SetWorkingImage(&Map);
	setbkcolor(WHITE);					// 白色背景
	cleardevice();						// 初始化背景
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
/////////延时函数////////
void delay(DWORD ms) {
	static DWORD oldtime = GetTickCount();

	if (GetTickCount() - oldtime < ms)
		Sleep(10);
	oldtime = GetTickCount();

}
//////////开始界面///////////
void start() {
	setbkcolor(WHITE);		// 白色背景
	cleardevice();			// 初始化背景
	settextcolor(RED);		// 改字体
	setbkmode(TRANSPARENT);
	settextstyle(20, 0, _T("宋体"));
	outtextxy(10, 10, _T("细菌.exev0.0.1(抢先体验版）"));
	settextcolor(BLUE);		// 改字体
	outtextxy(60, 240, _T("在完全随机的人体内放置你的细菌，把人体掏空，赢得胜利！"));
	outtextxy(50, 290, _T("（小心蓝色的吞噬细胞，它会与你同归于尽！而你只有9个DNA）"));
	outtextxy(70, 340, _T("（你有十次机会赋予你地细菌进入躲避状态，持续一秒）"));
	outtextxy(100, 370, _T("（鼠标左键放置你的DNA，右键改变细菌状态）"));
	settextcolor(BLACK);	//改字体
	settextstyle(15, 0, _T("宋体"));
	outtextxy(250, 390, _T("单击左键开始游戏"));
	settextstyle(20, 0, _T("宋体"));
	outtextxy(10, 450, _T("作者 QQ: 308620434"));
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
/////////////绘图函数///////////
void out_text_test() {
	TCHAR s[5];
	_stprintf_s(s, _T("%d"), last_cell);
	settextstyle(20, 0, _T("宋体"));
	outtext(_T("仍然存活的细胞数："));
	outtext(s);
	_stprintf_s(s, _T("%d"), DNA);
	//settextstyle(10, 0, _T("宋体"));
	outtext( _T(" 剩余DNA数："));
	outtext( s);
	_stprintf_s(s, _T("%d"), last_cancer);
	//settextstyle(10, 0, _T("宋体"));
	outtext( _T("  存活细菌相数："));
	outtext( s);
	_stprintf_s(s, _T("%d"), last_number_of_change);
	outtext(_T(" 剩余次数："));
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
	settextstyle(10, 0, _T("宋体"));
	outtextxy(10, 10, _T("仍然存活的细胞数："));
	outtext(s);
	outtextxy(100, 10, s);
	_stprintf_s(s, _T("%d"), DNA);
	settextstyle(10, 0, _T("宋体"));
	outtextxy(580, 20, _T("剩余DNA数："));
	outtextxy(630, 20, s);
	_stprintf_s(s, _T("%d"), last_cancer);
	settextstyle(10, 0, _T("宋体"));
	outtextxy(560, 10, _T("存活细菌相数："));
	outtextxy(630, 10, s);
	*/
}
//////////运行函数///////
void run() {
	/// 判定细菌状态////
	cancer_cell[0].to_change();
	/////////////细菌变色/////////
	for (int i = 0; i < Cancer_num; i++)
	{
		if (cancer_cell[0].retn_change() == true) cancer_cell[i].color = RED;
		else cancer_cell[i].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	}
	////////////////剩余鲜活细胞数///////////
	last_cell = 0;
	for (int i = 0; i < Cell_num; i++)
	{
		if (cell[i].life) last_cell++;
	}
	//////////////////通关判断///////////
	int s = 0;
	for (int i = 0; i < Cell_num; i++)
	{
		if (cell[i].life) s++;
	}
	if (s == 0) {
		MessageBox(NULL, _T("恭喜通关"), _T("恭喜通关,游戏结束"), MB_OK | MB_ICONEXCLAMATION);	// 通关游戏，结束
		closegraph();
		exit(0);
	}
	/////////////DNA耗完/////////////
	last_cancer = 0;
	for (int i = 0; i < Cancer_num; i++)
	{
		if (cancer_cell[i].life) last_cancer++;
	}
	if (DNA == 0 && last_cancer == 0) {
		MessageBox(NULL, _T("DNA被你浪费完了"), _T("DNA被你浪费完了，游戏结束"), MB_OK | MB_ICONEXCLAMATION);	// DNA耗光了，结束
		closegraph();
		exit(0);
	}

	////////互动/////////
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
	////////移动部分/////////
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
//////////放置或改变细菌///////
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
