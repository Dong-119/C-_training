//提笔乾坤
//简单模式：被反转过的宫格将被标记并反转颜色
//普通模式：被反转过的宫格将被反转颜色但不会被标记
//困难模式：被反转过的宫格将不会有任何变化，需玩家记忆


/*《最强大脑》中的“提笔乾坤”规则如下：

1.游戏界面：游戏在一个六边形盘面中进行，盘面上乱序分布着黑白两种颜色的宫格。
2.操作方式：玩家需要通过连续的一笔触摸，连接这些宫格。
3.宫格变化：每个宫格被触碰后会变为反色,且每个宫格最多只能反转一次。
4.胜利条件：当每圈所有宫格颜色相同时，答题成功。（共有5圈）

该游戏考验玩家的逻辑思维能力和空间想象力，需要玩家仔细观察和规划操作路径，以达到目标。*/


//先用电脑生成路径作为答案，然后每一圈按照路径与非路径随机填色

#include <stdio.h>
#include <graphics.h>
#include <string>
#include <conio.h>
#pragma comment(lib,"MSIMG32.LIB")
using namespace std;

inline void put_png(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}



//设置画布
int w = 1360, h = 765;

//声明背景图和返回贴图
IMAGE background,rule_background,play_background;
IMAGE back;

void button_png(IMAGE* png,int x,int y,int width,int height) {
	//把png图以宽weight高height放在background为背景的位置上，扣掉与第一块像素颜色相同部分
	IMAGE sum,sumpng=* png;
	getimage(&sum, x, y, width, height);
	putimage(x, y, png);
	COLORREF judge = getpixel(x, y);
	for (int i = x; i <= x+width; i++) {
		for (int j = y; j <= y+height; j++) {
			if(getpixel(i,j)== judge){
				putimage(x, y, &sum);
				COLORREF color = getpixel(i, j);
				putimage(x, y, &sumpng);
				putpixel(i, j, color);
				getimage(&sumpng, x, y, width, height);
			}
		}
	}
}

void menu_background() {
	//画面放置背景图
	loadimage(&background, "assets//background.png", w, h);
	putimage(0, 0, &background);
}


//按键中间输入文字
void center_text(int wbutton, int hbutton, int xtop_left, int ytop_left, const char str[]) {
	int xchar = (wbutton - textwidth(str)) / 2;
	int ychar = (hbutton - textheight(str)) / 2;
	outtextxy(xtop_left + xchar, ytop_left + ychar, str);
}

int main() {
	
	initgraph(w, h);
	//加载贴图
	IMAGE quit, play, load, how_to_play, setting, pic,grid0,grid1;
	COLORREF color;

	int wquit = 60, hquit = 60, wbutton = 315, hbutton = 70, wback = 90, hback = 60;;//定义退出键,返回键和一般键大小

	loadimage(&quit, "assets//quit.jpg", wquit, hquit);
	loadimage(&play, "assets//UItemplate2.png", wbutton, hbutton);
	loadimage(&load, "assets//UItemplate1.png", wbutton, hbutton);
	loadimage(&how_to_play, "assets//UItemplate1.png", wbutton, hbutton);
	loadimage(&setting, "assets//UItemplate1.png", wbutton, hbutton);
	loadimage(&back, "assets//back.png", wback, hback);//载入返回贴图
	loadimage(&pic, "assets//menu_pic.png", 500, 500);
	loadimage(&grid0, "assets//grid0.png", 0, 0); 
	loadimage(&grid1, "assets//grid1.png", 0, 0);

	//主菜单


menu:
	//主菜单背景图
	menu_background();


	//画面右下角放置退出图标，左侧放置开始游戏，继续游戏，玩法介绍，设置
	

	//定义不同按键左上角位置
	int xquit = 1260,yquit = 665;
	int xplay = 68, yplay = 306;
	int xload = xplay, yload = yplay + (h - yplay) / 4;
	int xhow_to_play = xplay, yhow_to_play = yplay + 2*(h - yplay) / 4;
	int xsetting = xplay, ysetting = yplay + 3*(h - yplay) / 4;


	//根据第一个按键位置放置标题
	settextstyle(200, 0, "华文行楷", 0, 0, 200, false, false, false);
	IMAGE title;
	loadimage(&title, "assets//title.png", textwidth("提笔乾坤"), textheight("提笔乾坤"));
	put_png((w - textwidth("提笔乾坤")) / 2, (yplay - textheight("提笔乾坤")) / 2, &title);
	put_png(xplay + wbutton+200, yplay-80, &pic);

	//放置按键贴图
	putimage(xquit, yquit, &quit);
	button_png(&play,  xplay, yplay, wbutton, hbutton);//putimage(xplay, yplay, &play);
	button_png(&load, xload, yload, wbutton, hbutton);//putimage(xload, yload, &load);
	button_png(&how_to_play, xhow_to_play, yhow_to_play, wbutton, hbutton);// putimage(xhow_to_play, yhow_to_play, &how_to_play);
	button_png(&setting,  xsetting, ysetting, wbutton, hbutton); //putimage(xsetting, ysetting, &setting);

	settextstyle(50, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);


	center_text(wbutton, hbutton, xplay, yplay, "新游戏");

	settextcolor(BLACK);

	center_text(wbutton, hbutton, xload, yload, "继续游戏");

	center_text(wbutton, hbutton, xhow_to_play, yhow_to_play, "游戏规则");

	center_text(wbutton, hbutton, xsetting, ysetting, "设置");

	//主菜单页面消息处理
	//定义消息结构体变量
	ExMessage msg = { 0 };
	//持续捕捉鼠标信息
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if ((msg.x > xquit && msg.x < xquit+wquit) && (msg.y > yquit && msg.y < yquit+hquit)) {
					return 0;//点击退出按钮将会结束进程        
				}
				if ((msg.x > xplay && msg.x < xplay + wbutton) && (msg.y > yplay && msg.y < yplay + hbutton)) {
					goto play;//点击开始游戏按钮将会跳转至游戏界面
				}
				if ((msg.x > xload && msg.x < xload + wbutton) && (msg.y > yload && msg.y < yload + hbutton)) {
					goto load;//点击读档按钮将会跳转至读档（主菜单）界面
				}
				if ((msg.x > xhow_to_play && msg.x < xhow_to_play + wbutton) && (msg.y > yhow_to_play && msg.y < yhow_to_play + hbutton)) {
					goto how_to_play;//点击游戏规则按钮将会跳转至游戏规则界面
				}
				if ((msg.x > xsetting && msg.x < xsetting + wbutton) && (msg.y > ysetting && msg.y < ysetting + hbutton)) {
					goto setting;//点击x设置按钮将会跳转至设置界面
				}
			}
		}
	}


	//游戏规则界面


how_to_play:
	//换背景
	cleardevice();
	loadimage(&rule_background, "assets//rule_background.png", w, h);
	putimage(0, 0, &rule_background);

	//粘贴返回贴图
	button_png(&back,  40, 40, wback, hback);

	//写字
	settextstyle(120, 0, "华文行楷", 0, 0, 200, false, false, false);
	color = RGB(110, 180, 200);
	settextcolor(color);
	setbkmode(TRANSPARENT);
	center_text(w, h/3, 0, 0, "提笔乾坤  游戏规则");
	settextstyle(29, 0, "华文行楷", 0, 0, 0, false, false, false);
	color = RGB(200, 120, 45);
	settextcolor(color);

	center_text(w, h / 12, 0, h/3, "游戏在一个六边形盘面中进行，盘面上乱序分布着绿白两种颜色的宫格。");
	center_text(w, h / 12, 0, h / 3 + h / 12, "玩家需要通过连续的一笔触摸，连接这些宫格。");
	center_text(w, h / 12, 0, h / 3 + h / 6, "每个宫格被触碰后会变为反色, 且每个宫格最多只能反转一次。");
	center_text(w, h / 12, 0, h / 3 + h / 4, "当每圈所有宫格颜色相同时，答题成功。");

	//开始游戏按钮
	color = RGB(200, 0, 0);
	settextcolor(color);
	settextstyle(50, 0, "华文行楷", 0, 0, 100, false, false, false);
	//int wplay_from_rule = textwidth("我已了解 开始游戏"), hplay_from_rule = textheight("我已了解 开始游戏");
	center_text(w-100, h / 4, 100, 3*h/4, "开始游戏");

	//持续捕捉鼠标信息
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if ((msg.x > 40 && msg.x < 40+wback) && (msg.y > 40 && msg.y < 40+hback)) {
					goto menu;//点击返回按钮将会返回主菜单
				}
				if ((msg.x > 100+(w-textwidth("开始游戏")-100)/2 && msg.x < w- (w-100 - textwidth("开始游戏")) / 2) && (msg.y > 3 * h / 4+ (h/4 - textheight("开始游戏")) / 2 && msg.y < h- (h / 4 - textheight("开始游戏")) / 2)) {
					goto play;//点击返回按钮将会返回主菜单
				}
			}
		}
	}


	//设置界面


setting:
	//粘贴返回贴图
	button_png(&back,  40, 40, wback, hback);

	//持续捕捉鼠标信息
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if ((msg.x > 40 && msg.x < 40 + wback) && (msg.y > 40 && msg.y < 40 + hback)) {
					goto menu;//点击返回按钮将会返回主菜单
				}
			}
		}
	}


	//读档界面（主菜单）


load:
	//粘贴返回贴图
	button_png(&back,  40, 40, wback, hback);

	//持续捕捉鼠标信息
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if ((msg.x > 40 && msg.x < 40 + wback) && (msg.y > 40 && msg.y < 40 + hback)) {
					goto menu;//点击返回按钮将会返回主菜单
				}
			}
		}
	}


	//游戏界面


play:
	//换背景
	cleardevice();
	loadimage(&play_background, "assets//play_background.png", w, h);
	putimage(0, 0, &play_background);

	//粘贴返回贴图
	button_png(&back,  40, 40, wback, hback);

	int deltax = 72, deltay = 77;//六边形紧密拼凑需要的横纵坐标差
	int x0 = (w - grid0.getwidth()) / 2, y0 = (h - grid0.getheight()) / 2;//最内圈六边形贴图的左上角坐标

	//生成盘面贴图
	for (int i = -4; i <= 4; i++) {
		for (int j = 1; j <= (i<0 ? (9+i):(9-i)) ; j++) {
			put_png(x0+i*deltax, y0-(i < 0 ? (9 + i) : (9 - i))*deltay/2+j*deltay-deltay/2, &grid0);
		}
	}
	

	//持续捕捉鼠标信息
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if ((msg.x > 40 && msg.x < 40 + wback) && (msg.y > 40 && msg.y < 40 + hback)) {
					goto menu;//点击返回按钮将会返回主菜单
				}
			}
		}
	}
}
