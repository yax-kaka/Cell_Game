#pragma once
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <math.h>

//////////////细胞基类////////////
class Cell
{
public:
	bool life;                       //是否存活
	COLORREF color;                  //颜色
	float x, y;                      //坐标

	Cell();
	~Cell();
	virtual void draw();
	virtual void move();
};
/////////// //细菌类//////////////
class Cancer_Cell :public Cell {     //细菌类
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

/////////////// //吞噬细胞类/////////////////
class Phagocyte :public Cell {        //吞噬细胞类
public:

	Phagocyte();
	~Phagocyte();
	void move();
	void draw();
	void attack(Cancer_Cell& a);
};



////////////其他函数声名/////////////
void set();
void delay(DWORD ms);
void start();
void Draw();
void run();
void set_can();
