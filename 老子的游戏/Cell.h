#pragma once
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <math.h>

//////////////ϸ������////////////
class Cell
{
public:
	bool life;                       //�Ƿ���
	COLORREF color;                  //��ɫ
	float x, y;                      //����

	Cell();
	~Cell();
	virtual void draw();
	virtual void move();
};
/////////// //ϸ����//////////////
class Cancer_Cell :public Cell {     //ϸ����
private:
	bool change;
public:
	Cancer_Cell();
	~Cancer_Cell();
	void move();
	void draw();
	void infect(Cell& a);
	void to_change();
	void set_change(bool t) {
		change = t;
	}
	bool retn_change() {
		return change;
	}
};

/////////////// //����ϸ����/////////////////
class Phagocyte :public Cell {        //����ϸ����
public:

	Phagocyte();
	~Phagocyte();
	void move();
	void draw();
	void attack(Cancer_Cell& a);
};



////////////������������/////////////
void set();
void delay(DWORD ms);
void start();
void Draw();
void run();
void set_can();
