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

#include<iostream>
#include <stdio.h>
#include <graphics.h>
#include <vector>
#include <conio.h>
#pragma comment(lib,"MSIMG32.LIB")
using namespace std;

inline void put_png(int x, int y, IMAGE* img)
{//贴含透明背景的png
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

//设置画布参数
int w = 1360, h = 765;

//声明背景图和贴图
IMAGE title;
IMAGE background,rule_background,play_background;
IMAGE back;
IMAGE quit, play, load, how_to_play, setting, pic, grid0, grid1;
COLORREF color;

//定义消息结构体变量
ExMessage msg = { 0 };

//定义退出键,返回键和一般键大小
int wquit = 60, hquit = 60, wbutton = 315, hbutton = 70, wback = 90, hback = 60;

//定义函数
void display_menu();
void display_how_to_play();
void display_setting();
void display_load();
void display_play();
void display_you_lose();

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

//按键中间输入文字
void center_text(int wbutton, int hbutton, int xtop_left, int ytop_left, const char str[]) {
	int xchar = (wbutton - textwidth(str)) / 2;
	int ychar = (hbutton - textheight(str)) / 2;
	outtextxy(xtop_left + xchar, ytop_left + ychar, str);
}

class Grid {
private:
	int x, y;//位置
	bool grid_color=false;
	int wgrid = grid0.getwidth(),hgrid = grid0.getheight();
	int circle, position;
	int reversed_time = 0;
public:
	Grid(int x,int y,int circle,int position){
		this->x = x;
		this->y = y;
		this->circle = circle;
		this->position = position;
		put_png(x, y, &grid0);                                                                                   //<---待优化
	}

	void reverse_color() {
		if (reversed_time == 0) {
			grid_color = !grid_color; 
			if (grid_color == false) { put_png(x, y, &grid0); }                                                  //<---待优化
			else { putimage(x, y, &grid1, SRCAND); }
			reversed_time++;
		}
		else {
			display_you_lose();
		}
	}

	bool reverse_or_not () {
		if (msg.y - y > -1.732 * (msg.x - x - wgrid / 4) && msg.y > y && msg.y - y > 1.732 * (msg.x - x - 3 * wgrid / 4) && msg.y - y < 1.732 * (msg.x - x) && msg.y - y < 2 * 1.732 * wgrid / 4 && msg.y - y < -1.732 * (msg.x - x - 3 * wgrid / 4) + 2 * 1.732 * wgrid / 4) {
			return true;
		}
		else {
			return false;
		}
	}

	const int get_circle(){
		return circle;
	}

	const int get_position() {
		return position;
	}
};

void display_menu(){
	//主菜单背景图
	putimage(0, 0, &background);

	//定义不同按键左上角位置
	int xquit = 1260, yquit = 665;
	int xplay = 68, yplay = 306;
	int xload = xplay, yload = yplay + (h - yplay) / 4;
	int xhow_to_play = xplay, yhow_to_play = yplay + 2 * (h - yplay) / 4;
	int xsetting = xplay, ysetting = yplay + 3 * (h - yplay) / 4;

	//开始双缓冲绘图
	BeginBatchDraw();
	//根据第一个按键位置放置标题
	settextstyle(200, 0, "华文行楷", 0, 0, 200, false, false, false);
	loadimage(&title, "assets//title.png", textwidth("提笔乾坤"), textheight("提笔乾坤"));
	//put_png((w - textwidth("提笔乾坤")) / 2, (yplay - textheight("提笔乾坤")) / 2, &title);
	putimage((w - textwidth("提笔乾坤")) / 2, (yplay - textheight("提笔乾坤")) / 2, &title, SRCAND);
	put_png(xplay + wbutton + 200, yplay - 80, &pic);

	//放置按键贴图
	putimage(xquit, yquit, &quit);
	put_png( xplay, yplay, &play);//putimage(xplay, yplay, &play);
	put_png(xload, yload, &load);//putimage(xload, yload, &load);
	put_png( xhow_to_play, yhow_to_play, &how_to_play);// putimage(xhow_to_play, yhow_to_play, &how_to_play);
	put_png( xsetting, ysetting, &setting); //putimage(xsetting, ysetting, &setting);
	//缓冲完毕开始绘图
	EndBatchDraw();

    settextstyle(50, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	center_text(wbutton, hbutton, xplay, yplay, "新游戏");
	settextcolor(BLACK);
	center_text(wbutton, hbutton, xload, yload, "继续游戏");
	center_text(wbutton, hbutton, xhow_to_play, yhow_to_play, "游戏规则");
	center_text(wbutton, hbutton, xsetting, ysetting, "设置");

	//主菜单页面消息处理
	
	//持续捕捉鼠标信息
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if ((msg.x > xquit && msg.x < xquit + wquit) && (msg.y > yquit && msg.y < yquit + hquit)) {
					exit(0);//点击退出按钮将会结束进程        
				}
				if ((msg.x > xplay && msg.x < xplay + wbutton) && (msg.y > yplay && msg.y < yplay + hbutton)) {
					display_play();//点击开始游戏按钮将会跳转至游戏界面
				}
				if ((msg.x > xload && msg.x < xload + wbutton) && (msg.y > yload && msg.y < yload + hbutton)) {
					display_load();//点击读档按钮将会跳转至读档（主菜单）界面
				}
				if ((msg.x > xhow_to_play && msg.x < xhow_to_play + wbutton) && (msg.y > yhow_to_play && msg.y < yhow_to_play + hbutton)) {
					display_how_to_play();//点击游戏规则按钮将会跳转至游戏规则界面
				}
				if ((msg.x > xsetting && msg.x < xsetting + wbutton) && (msg.y > ysetting && msg.y < ysetting + hbutton)) {
					display_setting();//点击设置按钮将会跳转至设置界面
				}
			}
		}
	}
}

void display_how_to_play() {
	//换背景
	cleardevice();
	loadimage(&rule_background, "assets//rule_background.png", w, h);
	putimage(0, 0, &rule_background);

	//粘贴返回贴图
	button_png(&back, 40, 40, wback, hback);

	//写字
	settextstyle(120, 0, "华文行楷", 0, 0, 200, false, false, false);
	color = RGB(110, 180, 200);
	settextcolor(color);
	setbkmode(TRANSPARENT);
	center_text(w, h / 3, 0, 0, "提笔乾坤  游戏规则");
	settextstyle(29, 0, "华文行楷", 0, 0, 0, false, false, false);
	color = RGB(200, 120, 45);
	settextcolor(color);

	center_text(w, h / 12, 0, h / 3, "游戏在一个六边形盘面中进行，盘面上乱序分布着绿白两种颜色的宫格。");
	center_text(w, h / 12, 0, h / 3 + h / 12, "玩家需要通过连续的一笔触摸，连接这些宫格。");
	center_text(w, h / 12, 0, h / 3 + h / 6, "每个宫格被触碰后会变为反色, 且每个宫格最多只能反转一次。");
	center_text(w, h / 12, 0, h / 3 + h / 4, "当每圈所有宫格颜色相同时，答题成功。");

	//开始游戏按钮
	color = RGB(200, 0, 0);
	settextcolor(color);
	settextstyle(50, 0, "华文行楷", 0, 0, 100, false, false, false);
	//int wplay_from_rule = textwidth("我已了解 开始游戏"), hplay_from_rule = textheight("我已了解 开始游戏");
	center_text(w - 100, h / 4, 100, 3 * h / 4, "开始游戏");

	//持续捕捉鼠标信息
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if ((msg.x > 40 && msg.x < 40 + wback) && (msg.y > 40 && msg.y < 40 + hback)) {
					display_menu();//点击返回按钮将会返回主菜单
				}
				if ((msg.x > 100 + (w - textwidth("开始游戏") - 100) / 2 && msg.x < w - (w - 100 - textwidth("开始游戏")) / 2) && (msg.y > 3 * h / 4 + (h / 4 - textheight("开始游戏")) / 2 && msg.y < h - (h / 4 - textheight("开始游戏")) / 2)) {
					display_play();//点击开始游戏按钮将会跳转至游戏界面
				}
			}
		}
	}
}

void display_setting() {
	//粘贴返回贴图
	button_png(&back, 40, 40, wback, hback);

	//持续捕捉鼠标信息
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if ((msg.x > 40 && msg.x < 40 + wback) && (msg.y > 40 && msg.y < 40 + hback)) {
					display_menu();//点击返回按钮将会返回主菜单
				}
			}
		}
	}
}

void display_load() {
	//粘贴返回贴图
	button_png(&back, 40, 40, wback, hback);

	//持续捕捉鼠标信息
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if ((msg.x > 40 && msg.x < 40 + wback) && (msg.y > 40 && msg.y < 40 + hback)) {
					display_menu();//点击返回按钮将会返回主菜单
				}
			}
		}
	}
}

void display_you_lose() {
	cout << "you lose" << endl;
	display_menu();
}



void display_play() {
	//换背景
	cleardevice();
	loadimage(&play_background, "assets//play_background.png", w, h);
	putimage(0, 0, &play_background);

	//粘贴返回贴图
	button_png(&back, 40, 40, wback, hback);

	int deltax = 72, deltay = 38;//六边形紧密拼凑需要的横纵坐标差
	int x0 = (w - grid0.getwidth()) / 2, y0 = (h - grid0.getheight()) / 2;//最内圈六边形贴图的左上角坐标

	//开始双缓冲绘图
	BeginBatchDraw();
	//生成盘面贴图
	int xgrid, ygrid;
	std::vector<Grid> grids;
	grids.emplace_back(x0, y0, 0, 0);
	for (int i = 0; i < 5; i++) {
		ygrid = y0 - i * 2 * deltay;
		xgrid = x0;
		for (int j = 0; j < 6 * i; j++) {
			grids.emplace_back(xgrid, ygrid, i, j);
			if (j / i == 0) { xgrid += deltax; ygrid += deltay; }
			else if (j / i == 1) { ygrid += 2 * deltay; }
			else if (j / i == 2) { xgrid -= deltax; ygrid += deltay; }
			else if (j / i == 3) { xgrid -= deltax; ygrid -= deltay; }
			else if (j / i == 4) { ygrid -= 2 * deltay; }
			else if (j / i == 5) { xgrid += deltax; ygrid -= deltay; }
		}
	}
	//缓冲完毕开始绘图
	EndBatchDraw();
	
	//持续捕捉鼠标信息
	int judge_msg;
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN){
				if ((msg.x > 40 && msg.x < 40 + wback) && (msg.y > 40 && msg.y < 40 + hback)) {
					//点击返回按钮将会返回主菜单
					grids.clear();//析构所有宫格
					display_menu();
				}
				for (int i = 0; i <= 60; i++) {
					if (grids[i].reverse_or_not()) {
						grids[i].reverse_color();//点击时反转的宫格
						judge_msg = i;
						while (1) {
							if (peekmessage(&msg, EX_MOUSE)) {
								if (msg.message == WM_LBUTTONUP) {
									i = 61;//使for函数停止遍历
									break;
								}
								else { 
									for (int j = 0; j <= 60; j++) {
										if (judge_msg != j && grids[j].reverse_or_not()) {
											grids[j].reverse_color();//滑动时反转的宫格

											//如果不相邻将被视为作弊直接判负

                                            if(grids[judge_msg].get_circle() == 0) {
											    //如果前一个宫格是中心宫格
												if (grids[j].get_circle() != 1) {
													cout << 1 << endl;
													display_you_lose(); 
												}
										    }
										    else if (grids[judge_msg].get_position() % grids[judge_msg].get_circle() == 0) {
										        //如果前一个是角上的宫格
										        
										        if (grids[judge_msg].get_circle() == grids[j].get_circle() - 1) {
										        	//如果在外圈
													if (grids[j].get_position() != (grids[j].get_circle() * grids[judge_msg].get_position() / grids[judge_msg].get_circle() + 1) % (grids[j].get_circle() * 6) && grids[j].get_position() != (grids[j].get_circle() * grids[judge_msg].get_position() / grids[judge_msg].get_circle()) % (grids[j].get_circle() * 6) && grids[j].get_position() != (grids[j].get_circle() * grids[judge_msg].get_position() / grids[judge_msg].get_circle() - 1 + grids[j].get_circle() * 6) % (grids[j].get_circle() * 6)) {
														cout << 2 << endl;
														display_you_lose();
													}
										        }
										        else if (grids[judge_msg].get_circle() == grids[j].get_circle()) {
										        	//如果在同一圈
													if (grids[j].get_position() != (grids[judge_msg].get_position() + 1) % (grids[j].get_circle() * 6)  && grids[j].get_position() != (grids[judge_msg].get_position() - 1 + grids[j].get_circle() * 6) % (grids[j].get_circle() * 6)){
														cout << 3<<endl;
														 display_you_lose();
													}
										        }
										        else if (grids[judge_msg].get_circle() == grids[j].get_circle()+1) {
										        	//如果在内圈
													if (grids[judge_msg].get_circle() !=1&&grids[j].get_position() != (grids[j].get_circle() * grids[judge_msg].get_position() / grids[judge_msg].get_circle()) % (grids[j].get_circle() * 6)) {
														cout << 4 << endl;
														display_you_lose();
													}
										        }
										        else {
													cout << 5 << endl;
													display_you_lose();
										        }
										    }
										    else {
												//如果前一个是边上的宫格

												if (grids[judge_msg].get_circle() == grids[j].get_circle() - 1) {
													//如果在外圈
													if (grids[j].get_position() != (grids[judge_msg].get_position() / grids[judge_msg].get_circle() * grids[j].get_circle() + grids[judge_msg].get_position() % grids[judge_msg].get_circle()) % (grids[j].get_circle() * 6) && grids[j].get_position() != (grids[judge_msg].get_position() / grids[judge_msg].get_circle() * grids[j].get_circle() + grids[judge_msg].get_position() % grids[judge_msg].get_circle()+1) % (grids[j].get_circle() * 6)) {
														cout << 6 << endl;
														display_you_lose();
													}
												}
												else if (grids[judge_msg].get_circle() == grids[j].get_circle()) {
													//如果在同一圈
													if (grids[j].get_position() != (grids[judge_msg].get_position() + 1)%(grids[j].get_circle()*6) && grids[j].get_position() != (grids[judge_msg].get_position() - 1 + grids[j].get_circle() * 6) % (grids[j].get_circle() * 6)){
														cout << 7 << endl;
														display_you_lose();
													}
												}
												else if (grids[judge_msg].get_circle() == grids[j].get_circle() + 1) {
													//如果在内圈
													if (grids[j].get_position() != (grids[judge_msg].get_position() / grids[judge_msg].get_circle() * grids[j].get_circle() + grids[judge_msg].get_position() % grids[judge_msg].get_circle()) % (grids[j].get_circle() * 6) && grids[j].get_position() != (grids[judge_msg].get_position() / grids[judge_msg].get_circle() * grids[j].get_circle() + grids[judge_msg].get_position() % grids[judge_msg].get_circle()+ grids[j].get_circle() * 6 - 1) % (grids[j].get_circle() * 6)) {
														cout << 8 << endl;
														display_you_lose();
													}
												}
											}
											judge_msg = j;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

int main() {
	initgraph(w, h);
	//加载贴图
	loadimage(&background, "assets//background.png", w, h);
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
	display_menu();
}
