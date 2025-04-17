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
#include <time.h>
#include <conio.h>
#include<fstream>
#include <sstream> 
using namespace std;

void put_png(int x, int y, IMAGE* img) {
	//贴图无白色
	putimage(x, y, img, SRCAND);
}

void put_png(int x, int y, IMAGE* img,IMAGE* img_mask) {
	//贴图有白色，需掩码图
	putimage(x, y, img_mask, NOTSRCERASE);
	putimage(x, y, img, SRCINVERT);
}

//设置画布参数
int w = 1360, h = 765;

//声明背景图和贴图
IMAGE title;
IMAGE background,rule_background,play_background;
IMAGE quit, play, load, how_to_play, setting, pic, pic_mask, grid0,grid0_mask, grid1;
IMAGE button_mask;
//IMAGE *back_mask=&back+1;
COLORREF color;

//定义消息结构体变量
ExMessage msg = { 0 };

//定义退出键,返回键和一般键大小
int wquit = 60, hquit = 60, wbutton = 315, hbutton = 70, wback = 90, hback = 60;

//定义关卡数
int level = 1;

//定义函数
void display_menu();
void display_how_to_play();
void display_setting();
void display_load();
void display_play();
void you_win(void next_stage());
void you_lose(bool *chessboard);
void retry(bool *chessboard);
void display_chooce_mode();

//按键中间输入文字
void center_text(int wbutton, int hbutton, int xtop_left, int ytop_left, const char str[]) {
	int xchar = (wbutton - textwidth(str)) / 2;
	int ychar = (hbutton - textheight(str)) / 2;
	outtextxy(xtop_left + xchar, ytop_left + ychar, str);
}

class button {
private:
	int x=0, y=0;//位置
	int w=0, h=0;//长宽
	IMAGE button_nor, button_mask, button_over;
	string word;
	bool state = false;//false代表鼠标不在按键上

	void set_str(const char* word) {
		center_text(w, h, x, y, word);
	}

	bool over_or_not() {
		if (msg.x > x && msg.x<x + w && msg.y>y && msg.y < y + h) {
			return true;
		}
		else {
			return false;
		}
	}
public:
	const int get_x() {
		return x;
	}

	const int get_y() {
		return y;
	}

	button(int x, int y, int w, int h, LPCTSTR img, LPCTSTR img_over) {
		this->x = x;
		this->y = y;
		loadimage(&button_nor,img, w, h);
		loadimage(&button_over, img_over, w, h);
		this->w = w;
		this->h = h;
		putimage(x, y, &button_nor);
	}

	button(int x, int y, int w, int h, LPCTSTR img, LPCTSTR img_over, LPCTSTR img_mask, const string str) {
		this->x = x;
		this->y = y;
		loadimage(&button_nor, img, w, h);
		loadimage(&button_mask, img_mask, w, h);
		loadimage(&button_over, img_over, w, h);
		this->w = w;
		this->h = h;
		put_png(x, y, &button_nor, &button_mask);
		word = str.c_str();
		set_str(word.c_str());
	}

	button(int x, int y, int w, int h, LPCTSTR img, LPCTSTR img_over, LPCTSTR img_mask) {
		this->x = x;
		this->y = y;
		loadimage(&button_nor, img, w, h);
		loadimage(&button_mask, img_mask, w, h);
		loadimage(&button_over, img_over, w, h);
		this->w = w;
		this->h = h;
		put_png(x, y, &button_nor, &button_mask);
	}

	bool state_change () {
		if (over_or_not() == !state)return true;
		else return false;
	}

	void act_over_mask() {
		if (over_or_not() == !state) {
			state = !(state);
			if (state) {
				put_png(x, y, &button_over, &button_mask);
				set_str(word.c_str());
			}
			else {
				put_png(x, y, &button_nor, &button_mask);
				set_str(word.c_str());
			}
		}
	}

	void act_over() {
		if (over_or_not() == !state) {
			state = !(state);
			if (state) {
				putimage(x, y, &button_over);
				set_str(word.c_str());
			}
			else {
				putimage(x, y, &button_nor);
				set_str(word.c_str());
			}
		}
	}

	void act_button(void f()) {
		//按钮跳转功能
		if (msg.x > x && msg.x<x + w && msg.y>y && msg.y < y + h) {
			f();//点击按钮将会跳转至指定函数
		}
	}

	void act_button(void f(bool *b),bool *b) {
		//按钮跳转功能
		if (msg.x > x && msg.x<x + w && msg.y>y && msg.y < y + h) {
			f(b);//点击按钮将会跳转至指定函数
		}
	}

	void act_button(void save_in(int save_position,IMAGE img, bool* chessboard, int level), int save_position, IMAGE img, bool* chessboard, int level) {
		//按钮跳转功能
		if (msg.x > x && msg.x<x + w && msg.y>y && msg.y < y + h) {
			save_in(save_position, img, chessboard, level);
		}
	}
};

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
		putimage(x, y, &grid0_mask, NOTSRCERASE);
		putimage(x, y, &grid0, SRCINVERT);
	}

	void set_reversed_time(int n){
		reversed_time = n;
	}

	bool reverse_color() {
		if (reversed_time == 0) {
			grid_color = !grid_color; 
			if (grid_color == false) {
				putimage(x, y, &grid0_mask, NOTSRCERASE);
				putimage(x, y, &grid0, SRCINVERT);
			}
			else { putimage(x, y, &grid1, SRCAND); }
			reversed_time++;
			return true;
		}
		else {
			return false;
		}
	}

	void reverse_set_question() {
		grid_color = !grid_color;
		if (grid_color == false) {
			putimage(x, y, &grid0_mask, NOTSRCERASE);
			putimage(x, y, &grid0, SRCINVERT);
		}
		else { putimage(x, y, &grid1, SRCAND); }
	}

	bool reverse_or_not () {
		if (msg.y-y>-2*hgrid*(msg.x-x)/wgrid+hgrid/2 && msg.y-y>0 && msg.y-y>2*hgrid*(msg.x-x)/wgrid-3*hgrid/2 && msg.y-y<2*hgrid*(msg.x-x)/wgrid+hgrid/2 && msg.y-y<hgrid && msg.y-y<-2*hgrid*(msg.x-x)/wgrid+5*hgrid/2) {
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

	const bool get_color() {
		return grid_color;
	}
};

vector<Grid> grids;

bool adjacent_or_not(int judge_msg,int j) {
	//如果j与judge_msg相邻返回ture

	if (judge_msg != j) {
		if (grids[judge_msg].get_circle() == 0) {
			//如果前一个宫格是中心宫格
			if (grids[j].get_circle() == 1) {
				return true;
			}
			else {
				return false;
			}
		}
		else if (grids[judge_msg].get_position() % grids[judge_msg].get_circle() == 0) {
			//如果前一个是角上的宫格

			if (grids[judge_msg].get_circle() == grids[j].get_circle() - 1) {
				//如果在外圈
				if (grids[j].get_position() == (grids[j].get_circle() * grids[judge_msg].get_position() / grids[judge_msg].get_circle() + 1) % (grids[j].get_circle() * 6) || grids[j].get_position() == (grids[j].get_circle() * grids[judge_msg].get_position() / grids[judge_msg].get_circle()) % (grids[j].get_circle() * 6) || grids[j].get_position() == (grids[j].get_circle() * grids[judge_msg].get_position() / grids[judge_msg].get_circle() - 1 + grids[j].get_circle() * 6) % (grids[j].get_circle() * 6)) {
					return true;
				}
				else {
					return false;
				}
			}
			else if (grids[judge_msg].get_circle() == grids[j].get_circle()) {
				//如果在同一圈
				if (grids[j].get_position() == (grids[judge_msg].get_position() + 1) % (grids[j].get_circle() * 6) || grids[j].get_position() == (grids[judge_msg].get_position() - 1 + grids[j].get_circle() * 6) % (grids[j].get_circle() * 6)) {
					return true;
				}
				else {
					return false;
				}
			}
			else if (grids[judge_msg].get_circle() == grids[j].get_circle() + 1) {
				//如果在内圈
				if (grids[judge_msg].get_circle() == 1 || grids[j].get_position() == (grids[j].get_circle() * grids[judge_msg].get_position() / grids[judge_msg].get_circle()) % (grids[j].get_circle() * 6)) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			//如果前一个是边上的宫格

			if (grids[judge_msg].get_circle() == grids[j].get_circle() - 1) {
				//如果在外圈
				if (grids[j].get_position() == (grids[judge_msg].get_position() / grids[judge_msg].get_circle() * grids[j].get_circle() + grids[judge_msg].get_position() % grids[judge_msg].get_circle()) % (grids[j].get_circle() * 6) || grids[j].get_position() == (grids[judge_msg].get_position() / grids[judge_msg].get_circle() * grids[j].get_circle() + grids[judge_msg].get_position() % grids[judge_msg].get_circle() + 1) % (grids[j].get_circle() * 6)) {
					return true;
				}
				else {
					return false;
				}
			}
			else if (grids[judge_msg].get_circle() == grids[j].get_circle()) {
				//如果在同一圈
				if (grids[j].get_position() == (grids[judge_msg].get_position() + 1) % (grids[j].get_circle() * 6) || grids[j].get_position() == (grids[judge_msg].get_position() - 1 + grids[j].get_circle() * 6) % (grids[j].get_circle() * 6)) {
					return true;
				}
				else {
					return false;
				}
			}
			else if (grids[judge_msg].get_circle() == grids[j].get_circle() + 1) {
				//如果在内圈
				if (grids[j].get_position() == (grids[judge_msg].get_position() / grids[judge_msg].get_circle() * grids[j].get_circle() + grids[judge_msg].get_position() % grids[judge_msg].get_circle()) % (grids[j].get_circle() * 6) || grids[j].get_position() == (grids[judge_msg].get_position() / grids[judge_msg].get_circle() * grids[j].get_circle() + grids[judge_msg].get_position() % grids[judge_msg].get_circle() + grids[j].get_circle() * 6 - 1) % (grids[j].get_circle() * 6)) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
	}
	else {
		return false;
	}
}

void set_question_auto(int num) {
	int* reverse = new int[num];
	int container[6] = {0}, recorder[61] = { 0 };
	int counter = 0;
	int i = rand() % 61;
	for (int k=0;k<num;k++) {
		recorder[i] = 1;
		reverse[k] = i;
		counter = 0;
		for (int j = 0; j < 61; j++) {
			if (!recorder[j]) {
				if (adjacent_or_not(i, j)) {
					container[counter] = j;
					counter++;
				}
			}
		}
		if (counter == 0&&k!=num-1) {
			set_question_auto(num);
			goto end;
		}
		else if (k != num - 1) {
			i = container[rand() % counter];
		}
	}
	for (int k = 0; k < 5; k++) {
		if (rand() % 2) {
			for (int l = 0; l < 61; l++) {
				if (grids[l].get_circle() == k) {
					grids[l].reverse_set_question();
				}
			}
		}
	}
	for (int k = 0; k < num; k++) {
		grids[reverse[k]].reverse_set_question();
	}
end:
	;
}

bool complete_or_not() {
	for (int j = 1; j < 60; j++) {
		if (grids[j].get_circle() == grids[j+1].get_circle()) {
			if (grids[j].get_color() != grids[j + 1].get_color()) {
				return false;
			}
		}
	}
	return true;
}

IMAGE pop_up, pop_up_mask;
void you_win(void next_stage()) {
	grids.clear();//析构所有宫格
	level++;
	loadimage(&pop_up, "assets//pop_up.png", 0, 0);
	loadimage(&pop_up_mask, "assets//pop_up_mask.png", 0, 0);
	int ximg = (w - pop_up.getwidth()) / 2; int yimg = (h - pop_up.getheight()) / 2;
	put_png(ximg, yimg, &pop_up, &pop_up_mask);
	settextstyle(50, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg, "YOU WIN");
	int wbt = 200; int hbt = 50;
	settextstyle(20, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	button next(ximg + pop_up.getwidth() / 3 + (pop_up.getwidth() / 3 - wbt) / 2, yimg + 2 * pop_up.getheight() / 3 + (pop_up.getheight() / 3 - hbt) / 2, wbt, hbt, "assets\\UItemplate3_nor.png", "assets\\UItemplate3_over.png", "assets\\UItemplate3_mask.png","下一关");
	button back(ximg + 2* pop_up.getwidth() / 3 + (pop_up.getwidth() / 3 - wbt) / 2, yimg + 2 * pop_up.getheight() / 3 + (pop_up.getheight() / 3 - hbt) / 2, wbt, hbt , "assets\\UItemplate3_nor.png", "assets\\UItemplate3_over.png", "assets\\UItemplate3_mask.png","主菜单");
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			next.act_over_mask();
			back.act_over_mask();
			if (msg.message == WM_LBUTTONDOWN) {
				next.act_button(next_stage);
				back.act_button(display_menu);
			}
		}
	}
}

void you_lose(bool *chessboard) {
	grids.clear();//析构所有宫格
	level = 1;
	loadimage(&pop_up, "assets//pop_up.png", 0, 0);
	loadimage(&pop_up_mask, "assets//pop_up_mask.png", 0, 0);
	int ximg = (w - pop_up.getwidth()) / 2; int yimg = (h - pop_up.getheight()) / 2;
	put_png(ximg, yimg, &pop_up, &pop_up_mask);
	settextstyle(50, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg, "YOU LOSE");
	int wbt = 200; int hbt = 50;
	settextstyle(20, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	button next(ximg + pop_up.getwidth() / 3 + (pop_up.getwidth() / 3 - wbt) / 2, yimg + 2 * pop_up.getheight() / 3 + (pop_up.getheight() / 3 - hbt) / 2, wbt, hbt, "assets\\UItemplate3_nor.png", "assets\\UItemplate3_over.png", "assets\\UItemplate3_mask.png", "重试");
	button back(ximg + 2 * pop_up.getwidth() / 3 + (pop_up.getwidth() / 3 - wbt) / 2, yimg + 2 * pop_up.getheight() / 3 + (pop_up.getheight() / 3 - hbt) / 2, wbt, hbt, "assets\\UItemplate3_nor.png", "assets\\UItemplate3_over.png", "assets\\UItemplate3_mask.png", "主菜单");
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			next.act_over_mask();
			back.act_over_mask();
			if (msg.message == WM_LBUTTONDOWN) {
				next.act_button(retry,chessboard);
				back.act_button(display_menu);
			}
		}
	}
}

void exit() {
	exit(0);
}

void display_menu(){
	loadimage(&pic, "assets//menu_pic.png", 500, 500);
	loadimage(&pic_mask, "assets//menu_pic_mask.png", 500, 500);
	loadimage(&background, "assets//background.png", w, h);
	//主菜单背景图
	putimage(0, 0, &background);

	//定义不同按键左上角位置
	int xquit = 1260, yquit = 665;
	int xplay = 68, yplay = 306;
	int xload = xplay, yload = yplay + (h - yplay) / 4;
	int xhow_to_play = xplay, yhow_to_play = yplay + 2 * (h - yplay) / 4;
	int xsetting = xplay, ysetting = yplay + 3 * (h - yplay) / 4;

	//根据第一个按键位置放置标题
	settextstyle(200, 0, "华文行楷", 0, 0, 200, false, false, false);
	loadimage(&title, "assets//title.png", textwidth("提笔乾坤"), textheight("提笔乾坤"));
	putimage((w - textwidth("提笔乾坤")) / 2, (yplay - textheight("提笔乾坤")) / 2, &title, SRCAND);
	put_png(xplay + wbutton + 200, yplay - 80, &pic, &pic_mask);

	//放置按键贴图
	button quit(xquit, yquit,wquit,hquit, "assets//quit.jpg", "assets//quit_over.png");

	settextstyle(50, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	button play(xplay, yplay, wbutton, hbutton, "assets//UItemplate2.png", "assets//UItemplate2_over.png", "assets//UItemplate_mask.png","新游戏");
	
	settextcolor(BLACK);
	button load(xload, yload, wbutton, hbutton, "assets//UItemplate1.png", "assets//UItemplate1_over.png", "assets//UItemplate_mask.png", "继续游戏");
	button how_to_play(xhow_to_play, yhow_to_play, wbutton, hbutton, "assets//UItemplate1.png", "assets//UItemplate1_over.png", "assets//UItemplate_mask.png", "游戏规则");
	button setting(xsetting, ysetting, wbutton, hbutton, "assets//UItemplate1.png", "assets//UItemplate1_over.png", "assets//UItemplate_mask.png", "设置");

	//主菜单页面消息处理
	
	//持续捕捉鼠标信息
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			settextcolor(WHITE);
			play.act_over_mask();

			settextcolor(BLACK);
			load.act_over_mask();
			how_to_play.act_over_mask();
			setting.act_over_mask();

			quit.act_over();
			if (msg.message == WM_LBUTTONDOWN) {
				play.act_button(display_chooce_mode);
				load.act_button(display_load);
				how_to_play.act_button(display_how_to_play);
				setting.act_button(display_setting);
				quit.act_button(exit);
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
	button back(40, 40, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");

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
	center_text(w - 100, h / 4, 100, 3 * h / 4, "开始游戏");

	//持续捕捉鼠标信息
	while (1) {
		back.act_over_mask();
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				back.act_button(display_menu);
				if ((msg.x > 100 + (w - textwidth("开始游戏") - 100) / 2 && msg.x < w - (w - 100 - textwidth("开始游戏")) / 2) && (msg.y > 3 * h / 4 + (h / 4 - textheight("开始游戏")) / 2 && msg.y < h - (h / 4 - textheight("开始游戏")) / 2)) {
					display_play();//点击开始游戏按钮将会跳转至游戏界面
				}
			}
		}
	}
}

void display_setting() {
	//粘贴返回贴图
	button back(40, 40, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");

	//持续捕捉鼠标信息
	while (1) {
		back.act_over_mask();
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				back.act_button(display_menu);
			}
		}
	}
}

void save_in(int save_position,IMAGE img, bool* chessboard, int level) {
	std::ostringstream oss;
	oss << "save\\save" << save_position << "\\chessboard_data"; // 拼接三段字符串
	std::string chessboard_file = oss.str();

	std::ostringstream ossimg;
	ossimg << "save\\save" << save_position << "\\image_data.png"; // 拼接三段字符串
	std::string image_file = ossimg.str();

	saveimage(_T(image_file.c_str()), &img);

	ofstream chessboard_save_stream(chessboard_file);
	 
	//if (!chessboard_save_stream.is_open()) {
	//	display_checkmsg();
	//}

	for (int i = 0; i < 61; i++) {
		chessboard_save_stream << chessboard[i];
	}
}

//定义saveload界面缩略图长宽
int wimg = 229*2/3, himg = 128*2/3;

void display_save(bool *chessboard,int level,IMAGE img) {
	grids.clear();//析构所有宫格
	IMAGE save_background,save_caption;
	loadimage(&save_background, "assets\\file_save@base%base.png", w, h );
	int hbar = 76;
	loadimage(&save_caption, "assets\\file_save@caption%layer.png", w, hbar);
	putimage(0, 0, &save_background);
	putimage(0, 0, &save_caption);

	//粘贴返回贴图
	button back(0, h-hback, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");

	vector<button> saves;

	int wsave_img = 412, hsave_img = 132;
	IMAGE save_img;
	for (int i = 0; i < 8; i++) {
		saves.emplace_back((w / 2 - wsave_img)/2 + (w / 2) * (i / 4), hbar + (h / 4 - hsave_img) / 2 + (h - hbar) / 4 * (i % 4), wsave_img, hsave_img, "assets\\file_save@item%button;normal.png", "assets\\file_save@item%button;over.png", "assets\\file@item%button;mask.png");
		std::ostringstream oss;
		oss << "save\\save" << i+1 << "\\image_data.png"; // 拼接三段字符串
		std::string image_file = oss.str();
		loadimage(&save_img, image_file.c_str(), wimg, himg,0);
		putimage(saves[i].get_x() + 12, saves[i].get_y() + 12, &save_img);
	}

	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			back.act_over_mask();
			for (int i = 0; i < 8; i++) {
				if (saves[i].state_change()) {

					//开始双缓冲绘图
					BeginBatchDraw();

					saves[i].act_over_mask();
					std::ostringstream oss;
					oss << "save\\save" << i + 1 << "\\image_data.png"; // 拼接三段字符串
					std::string image_file = oss.str();
					loadimage(&save_img, image_file.c_str(), wimg, himg);
					putimage(saves[i].get_x() + 12, saves[i].get_y() + 12, &save_img);

					//缓冲完毕开始绘图
					EndBatchDraw();
				}
			}
			if (msg.message == WM_LBUTTONDOWN) {
				back.act_button(retry,chessboard);
				for (int i = 0; i < 8; i++) {
					saves[i].act_button(save_in, i+1, img, chessboard ,level);
					std::ostringstream oss;
					oss << "save\\save" << i + 1 << "\\image_data.png"; // 拼接三段字符串
					std::string image_file = oss.str();
					loadimage(&save_img, image_file.c_str(), wimg, himg);
					putimage(saves[i].get_x() + 12, saves[i].get_y() + 12, &save_img);
				}
			}
		}
	}
}

void load_to (const char* chessboard_file_name) {
	vector<int> numbers; // 使用动态数组存储数字

	// 打开文件
	ifstream inputFile(chessboard_file_name);
	if (!inputFile.is_open()) {

		//cerr << "无法打开文件！" << endl;
		loadimage(&pop_up, "assets//pop_up.png", 0, 0);
		loadimage(&pop_up_mask, "assets//pop_up_mask.png", 0, 0);
		int ximg = (w - pop_up.getwidth()) / 2; int yimg = (h - pop_up.getheight()) / 2;
		put_png(ximg, yimg, &pop_up, &pop_up_mask);
		settextstyle(50, 0, "幼圆", 0, 0, 1000, false, false, false);
		settextcolor(BLACK);
		setbkmode(TRANSPARENT);
		center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg, "无法打开文件");
		center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg+ pop_up.getheight() / 3, "请重新输入文件地址\n文件地址不要加引号，并确认是否用\\\\作地址分隔符");
		center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg+2* pop_up.getheight() / 3, "按任意键返回");

		//持续捕捉鼠标信息
		while (1) {
			if (peekmessage(&msg, EX_KEY)) {
				if (msg.message == WM_KEYDOWN) {
					display_menu();
				}
			}
		}
	}

	char ch;
	// 逐字符读取文件内容
	while (inputFile.get(ch)) {
		if (isdigit(ch)) { // 检查是否为数字字符
			numbers.push_back(ch - '0'); // 将字符转换为数字并存储
		}
	}

	bool chessboard[61];
	for (int i = 0; i < 61; i++) {
		chessboard[i] = numbers[i];
	}

	// 关闭文件
	inputFile.close();

	retry(chessboard);
}

void display_load() {
	grids.clear();//析构所有宫格
	IMAGE save_background, save_caption;
	loadimage(&save_background, "assets\\file_save@base%base.png", w, h);
	int hbar = 76;
	loadimage(&save_caption, "assets\\file_save@caption%layer.png", w, hbar);
	putimage(0, 0, &save_background);
	putimage(0, 0, &save_caption);

	//粘贴返回贴图
	button back(0, h - hback, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");

	char str[100];
	InputBox( str, 100 , "请输入盘面文件地址"); // 弹出对话框让用户输入字符串

	outtextxy(100, 100, str); // 在窗口中显示用户输入的字符串

	load_to(str);
}

void display_chooce_stage() {

}

void display_chooce_mode() {
	//粘贴返回贴图
	button back(40, 40, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");

	IMAGE chooce_UI,chooce_UI_mask;
	loadimage(&chooce_UI, "assets//chooce_mode.png", 0, h);
	loadimage(&chooce_UI_mask, "assets//chooce_mode_mask.png", 0, h);
	put_png(w - chooce_UI.getwidth(), 0, &chooce_UI, &chooce_UI_mask);

	int wbt = 200, hbt = 50;
	settextstyle(25, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	int deltax = (chooce_UI.getwidth() - wbt) / 2, deltay = (h / 2 - chooce_UI.getheight()) / 2;
	button stage(w - chooce_UI.getwidth() + deltax, h / 2 + deltay, wbt, hbt, "assets\\UItemplate3_nor.png", "assets\\UItemplate3_over.png", "assets\\UItemplate3_mask.png", "闯关模式");
	button endless(w - chooce_UI.getwidth() + deltax, h + deltay, wbt, hbt, "assets\\UItemplate3_nor.png", "assets\\UItemplate3_over.png", "assets\\UItemplate3_mask.png", "无尽模式");

	while (1) {
		back.act_over_mask();
		stage.act_over_mask();
		endless.act_over_mask();
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				back.act_button(display_menu);
				stage.act_button(display_chooce_stage);
				endless.act_button(display_play);
			}
		}
	}
}

void display_play() {
	//换背景
	cleardevice();
	loadimage(&play_background, "assets//play_background.png", w, h);
	putimage(0, 0, &play_background);

	//粘贴返回贴图
	button back(40, 40, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");

	int deltax = 72, deltay = 38;//六边形紧密拼凑需要的横纵坐标差
	int x0 = (w - grid0.getwidth()) / 2, y0 = (h - grid0.getheight()) / 2;//最内圈六边形贴图的左上角坐标

	//开始双缓冲绘图
	BeginBatchDraw();
	//生成盘面贴图
	int xgrid, ygrid;
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
	
	set_question_auto(rand()%11+20);

	//记录本关盘面，供重试功能使用
	bool chessboard[61];
	for (int i = 0; i <= 60; i++) {
		chessboard[i] = grids[i].get_color();
	}

	//持续捕捉鼠标信息
	int judge_msg;
	while (1) {
		back.act_over_mask();
		if (peekmessage(&msg, EX_MOUSE | EX_KEY)) {
			if (msg.message == WM_LBUTTONDOWN) {
				back.act_button(display_menu);
				for (int i = 0; i <= 60; i++) {
					if (grids[i].reverse_or_not()) {
						if (!grids[i].reverse_color()) {
							you_lose(chessboard);
						};//点击时反转的宫格
						judge_msg = i;
						while (1) {
							if (peekmessage(&msg, EX_MOUSE)) {
								if (msg.message == WM_LBUTTONUP) {
									if (complete_or_not()) {
										you_win(display_play);
									}
									i = 61;//使for函数停止遍历
									break;
								}
								else {
									for (int j = 0; j <= 60; j++) {
										if (grids[j].reverse_or_not() && adjacent_or_not(judge_msg, j)) {
											if (!grids[j].reverse_color()) {
												you_lose(chessboard);
											};
											judge_msg = j;
											break;
										}
									}
								}
							}
						}
					}
				}
			}
			else if (msg.message == WM_KEYDOWN) {
				if (msg.vkcode = 'S') {
					IMAGE img;
					int wget_img = 229 * 5.7, hget_img = 128 * 5.7;
					getimage(&img, (w - wget_img) / 2, (h - hget_img) / 2, wget_img, hget_img);
					display_save(chessboard, level, img);
				}
			}
		}
	}
}

void retry(bool* chessboard) {
	//换背景
	cleardevice();
	loadimage(&play_background, "assets//play_background.png", w, h);
	putimage(0, 0, &play_background);

	//粘贴返回贴图
	button back(40, 40, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");

	int deltax = 72, deltay = 38;//六边形紧密拼凑需要的横纵坐标差
	int x0 = (w - grid0.getwidth()) / 2, y0 = (h - grid0.getheight()) / 2;//最内圈六边形贴图的左上角坐标

	//开始双缓冲绘图
	BeginBatchDraw();

	//生成盘面贴图
	int xgrid, ygrid;
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

	for (int i = 0; i < 61; i++) {
		if (chessboard[i]) {
			grids[i].reverse_set_question();
		}
	}

	//记录本关盘面，供重试功能使用
	for (int i = 0; i <= 60; i++) {
		chessboard[i] = grids[i].get_color();
	}

	//持续捕捉鼠标信息
	int judge_msg;
	while (1) {
		back.act_over_mask();
		if (peekmessage(&msg, EX_MOUSE | EX_KEY)) {
			if (msg.message == WM_LBUTTONDOWN) {
				back.act_button(display_menu);
				for (int i = 0; i <= 60; i++) {
					if (grids[i].reverse_or_not()) {
						if (!grids[i].reverse_color()) {
							you_lose(chessboard);
						};//点击时反转的宫格
						judge_msg = i;
						while (1) {
							if (peekmessage(&msg, EX_MOUSE)) {
								if (msg.message == WM_LBUTTONUP) {
									if (complete_or_not()) {
										you_win(display_play);
									}
									i = 61;//使for函数停止遍历
									break;
								}
								else {
									for (int j = 0; j <= 60; j++) {
										if (grids[j].reverse_or_not() && adjacent_or_not(judge_msg, j)) {
											if (!grids[j].reverse_color()) {
												you_lose(chessboard);
											};
											judge_msg = j;
											break;
										}
									}
								}
							}
						}
					}
				}
			}
			else if (msg.message == WM_KEYDOWN) {
				if (msg.vkcode = 'S') {
					IMAGE img;
					int wget_img = 229 * 5.7, hget_img = 128 * 5.7;
					getimage(&img, (w - wget_img) / 2, (h - hget_img) / 2, wget_img, hget_img);
					display_save(chessboard, level, img);
				}
			}
		}
	}
}

int main() {
	srand(time(NULL));
	initgraph(w, h);
	//加载贴图
	loadimage(&grid0, "assets//grid0.png", 0, 0);
	loadimage(&grid0_mask, "assets//grid0_mask.png", 0, 0);
	loadimage(&grid1, "assets//grid1.png", 0, 0);

	//主菜单
	display_menu();
}
