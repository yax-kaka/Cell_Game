/////////实现库：EasyX库(附带graphics.h) 2020 版 (2020-5-20 更新)//////////////
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include"Cell.h"

const int Width = 640;                //屏幕长
const int Height = 480;               //屏幕宽

int main() {
	initgraph(Width, Height);
	set();
	start();
	BeginBatchDraw();
	while (true)
	{
		Draw();
		run();
		set_can();
		FlushBatchDraw();	// 显示缓存的绘制内容
		delay(1000);			// 绝对延迟
	}
	_getch();
	closegraph();
	return 0;
}
