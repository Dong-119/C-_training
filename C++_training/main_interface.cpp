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
#include <windows.h>
#include <stdio.h>
#include <graphics.h>
#include <vector>
#include <set>
#include <time.h>
#include <conio.h>
#include<fstream>
#include <sstream>
#pragma comment(lib, "winmm.lib")

using namespace std;

void put_png(int x, int y, IMAGE* img) {
	BeginBatchDraw();
	//贴图无白色
	putimage(x, y, img, SRCAND);
	EndBatchDraw();
}

void put_png(int x, int y, IMAGE* img,IMAGE* img_mask) {
	BeginBatchDraw();
	//贴图有白色，需掩码图
	putimage(x, y, img_mask, NOTSRCERASE);
	putimage(x, y, img, SRCINVERT);
	EndBatchDraw();
}

//设置画布参数
int w = 1360, h = 765;

//声明背景图和贴图
IMAGE title;
IMAGE background,rule_background,play_background;
IMAGE quit, play, load, how_to_play, setting, pic, pic_mask, grid0, grid0_mask, grid1;
IMAGE button_mask;
IMAGE hint_window, hint_window_mask;
//IMAGE *back_mask=&back+1;
COLORREF color;

//定义消息结构体变量
ExMessage msg = { 0 };

//定义退出键,返回键和一般键大小
int wquit = 60, hquit = 60, wbutton = 315, hbutton = 70, wback = 90, hback = 60;

//定义关卡数
int level = 1;
//定义模式状态:简单0，普通1，困难2
int mode = 1;
//定义游玩模式：无尽0，闯关1，保存或自定义2
int play_mode = 0;
//定义当前BGM状态
int music = -1;
//定义音量
int volume=20;
//切换BGM函数
void music_change_to(int i) {
	if (i == music)return;
	if (i == 0) {
		music = 0;
		PlaySound(TEXT("assets//title.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	else if (i == 1) {
		music = 1;
		PlaySound(TEXT("assets//play.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
}

//计算音量函数
DWORD CalculateVolume(int leftPercent, int rightPercent) {
	return (DWORD)((leftPercent * 0xFFFF / 100) | ((rightPercent * 0xFFFF / 100) << 16));
}

//记录本局答案
int answer[61] = { 0 };

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
void display_set_question();

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

	void act_button(void retry(bool *chessboard),bool * chessboard) {
		//按钮跳转功能
		if (msg.x > x && msg.x<x + w && msg.y>y && msg.y < y + h) {
			retry(chessboard);//点击按钮将会跳转至指定函数
		}
	}

	void act_button(void save_in(int save_position,IMAGE img, bool* chessboard), int save_position, IMAGE img, bool* chessboard) {
		//按钮跳转功能
		if (msg.x > x && msg.x<x + w && msg.y>y && msg.y < y + h) {
			save_in(save_position, img, chessboard);
		}
	}

	void act_button(void load_to(const char* chessboard_file_name), const char* chessboard_file_name) {
		if (msg.x > x && msg.x<x + w && msg.y>y && msg.y < y + h) {
			load_to(chessboard_file_name);
		}
	}

	bool act_button(int chooce_mode) {
		if (msg.x > x && msg.x<x + w && msg.y>y && msg.y < y + h) {
			mode = chooce_mode;
			loadimage(&button_nor, "assets//sys_page1@use__speech%button;normal;on.png", w, h);
			loadimage(&button_over, "assets//sys_page1@use__speech%button;over;on.png", w, h);
			loadimage(&button_mask, "assets//sys_page1@use__speech%button;normal;off_mask.png", w, h);
			put_png(x, y, &button_nor, &button_mask);
			set_str(word.c_str());
			return true;
		}
		return false;
	}

	void act_button(void change_circle_color(int n),int n) {
		if (msg.x > x && msg.x<x + w && msg.y>y && msg.y < y + h) {
			change_circle_color(n);
		}
	}

	void change_image(LPCTSTR img, LPCTSTR img_over, LPCTSTR img_mask) {
		loadimage(&button_nor, img, w, h);
		loadimage(&button_mask, img_mask, w, h);
		loadimage(&button_over, img_over, w, h);
		put_png(x, y, &button_nor, &button_mask);
		set_str(word.c_str());
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
		put_png(x, y, &grid0, &grid0_mask);
	}

	void set_reversed_time(int n){
		reversed_time = n;
	}

	bool reverse_color(int mode) {
		if (mode == 0) {
			IMAGE grid_cover, grid_cover_mask;
			loadimage(&grid_cover, "assets//grid_cover.png", wgrid, hgrid);
			loadimage(&grid_cover_mask, "assets//grid_cover_mask.png", wgrid, hgrid);
			if (reversed_time == 0) {
				grid_color = !grid_color;
				if (grid_color == false) {
					put_png(x, y, &grid0, &grid0_mask);
					put_png(x, y, &grid_cover, &grid_cover_mask);
				}
				else {
					put_png(x, y, &grid1, &grid0_mask);
					put_png(x, y, &grid_cover, &grid_cover_mask);
				}
				reversed_time++;
				return true;
			}
			else {
				return false;
			}
		}
		else if (mode == 1) {
			if (reversed_time == 0) {
				grid_color = !grid_color;
				if (grid_color == false) {
					put_png(x, y, &grid0, &grid0_mask);
				}
				else {put_png(x, y, &grid1, &grid0_mask); }
				reversed_time++;
				return true;
			}
			else {
				return false;
			}
		}
		else if (mode == 2) {
			if (reversed_time == 0) {
				grid_color = !grid_color;
				reversed_time++;
				return true;
			}
			else {
				return false;
			}
		}
	}

	void reverse_set_question() {
		grid_color = !grid_color;
		if (grid_color == false) {
			put_png(x, y, &grid0, &grid0_mask);
		}
		else { put_png(x, y, &grid1, &grid0_mask); }
	}

	bool reverse_or_not () {
		if (msg.y-y>-2*hgrid*(msg.x-x)/wgrid+hgrid/2 && msg.y-y>0 && msg.y-y>2*hgrid*(msg.x-x)/wgrid-3*hgrid/2 && msg.y-y<2*hgrid*(msg.x-x)/wgrid+hgrid/2 && msg.y-y<hgrid && msg.y-y<-2*hgrid*(msg.x-x)/wgrid+5*hgrid/2) {
			return true;
		}
		else {
			return false;
		}
	}

	void hint_cover() {
		IMAGE grid_hint_cover, grid_hint_cover_mask;
		loadimage(&grid_hint_cover, "assets//grid_hint_cover.png", grid0.getwidth(), grid0.getheight());
		loadimage(&grid_hint_cover_mask, "assets//grid_cover_mask.png", grid0.getwidth(), grid0.getheight());
		put_png(x, y, &grid_hint_cover, &grid_hint_cover_mask);
	}

	void hint_wrong_cover() {
		IMAGE grid_wrong_cover, grid_wrong_cover_mask;
		loadimage(&grid_wrong_cover, "assets//grid_wrong_cover.png", grid0.getwidth(), grid0.getheight());
		loadimage(&grid_wrong_cover_mask, "assets//grid_wrong_cover_mask.png", grid0.getwidth(), grid0.getheight());
		put_png(x, y, &grid_wrong_cover, &grid_wrong_cover_mask);
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

	const int get_reversed_time() {
		return reversed_time;
	}
};

vector<Grid> grids;

//如果j与judge_msg相邻返回ture,为相同格或不相邻格返回false
bool adjacent_or_not(int judge_msg,int j) {
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

//检查某相同色是否连成一片，是则返回true，否则返回false
bool one_piece_or_not(vector<int> color) {
	// 绿色宫格数组取第一个作为当前格域，遍历整个数组寻找相邻同色宫格
	vector<int>now; vector<int>next;
	now.push_back(color.at(0));
	color.erase(color.begin());
	while (1) {
		next.clear();
		// 如有则在原本绿色格数组中删除当前格并记录其相邻同色宫格编号到当前格数组，再遍历当前格中每个元素找出与当前格域相邻的格为新的当前格域
		for (const auto& same_color : color) {
			for (const auto& now_grid : now) {
				if (adjacent_or_not(now_grid, same_color)) {
					next.push_back(same_color);
					break;
				}
			}
		}
		set<int> setNow(now.begin(), now.end());
		color.erase(std::remove_if(color.begin(), color.end(), [&setNow](int value) {
			return setNow.find(value) != setNow.end();
		}), color.end());
		now.clear();
		now = next;
		// 如果最终所有绿色格都为当前格，则说明连成一片
		if (now == color)return true;
		else if (next.empty())return false;
	}
}

//尽头检测，返回本颜色宫格尽头数
int ends_num(vector<int> color) {
	//记录当前格相邻同色格数，初始化为零
	int counter = 0;
	//记录尽头数，初始化为零
	int ends = 0;
	for (const auto& this_grid : color) {
		//计数器复位
		counter = 0;
		for (const auto& grid : color) {
			if (adjacent_or_not(this_grid, grid))counter++;
			if (counter > 1)break;
		}
		if (counter == 1){
			ends++;
		}
	}
	return ends;
}

//重置盘面为全白
void reset_grids() {
	for (int i = 0; i < 61; i++) {
		if (grids[i].get_color()) {
			grids[i].reverse_set_question();
		}
		grids[i].set_reversed_time(0);
	}
}

void set_question_auto(int num) {
	//重置盘面
	reset_grids();
	//记录需反转宫格编号
	int* reverse = new int[num];
	//记录当前格周围可反转的宫格，记录整个棋盘颜色情况
	int container[6] = {0}, recorder[61] = { 0 };
	//记录当前格周围可反转的宫格数量
	int counter = 0;
	//随机生成起点
	int i = rand() % 61;
	for (int k=0;k<num;k++) {
		if (i == 0) {//如果路径经过中心格，重新出题，减少题目过于简单概率
			set_question_auto(num);
			return;
		}
		//反转当前宫格颜色
		recorder[i] = 1;
		//记录当前格编号
		reverse[k] = i;
		//重置当前格周围可反转的宫格数量
		counter = 0;
		//将当前格周围可翻转格记录在容器中
		for (int j = 0; j < 61; j++) {
			if (!recorder[j]) {
				if (adjacent_or_not(i, j)) {
					container[counter] = j;
					counter++;
				}
			}
		}
		//如果出题到死路，重新出题
		if (counter == 0&&k!=num-1) {
			set_question_auto(num);
			return;
		}//如果出题可以正常进行，更新当前格
		else if (k != num - 1) {
			i = container[rand() % counter];
		}
	}
	//随机生成每圈颜色
	BeginBatchDraw();
change_circle_color:
	int check[5];
	for (int k = 0; k < 5; k++) {
		if (check[k]=rand() % 2) {
			for (int l = 0; l < 61; l++) {
				if (grids[l].get_circle() == k) {
					grids[l].reverse_set_question();
				}
			}
		}
	}
	// 如果每圈颜色都相同，重新设置每圈颜色
	if (check[0] == check[1] &&check[1] == check[2] &&check[2] == check[3] &&check[3] ==check[4] ) {
		goto change_circle_color;
	}
	
	//反转颜色，完成出题
	for (int k = 0; k < num; k++) {
		grids[reverse[k]].reverse_set_question();
	}
	// 如果同色可以一笔完成，过于简单重新出题，具体方式如下：
	// 遍历宫格将绿色与白色宫格编号分别储存在两个数组中
	vector<int>green;
	vector<int>white;
	//中心宫格可经过可不经过不在考虑范围内
	green.push_back(0);
	white.push_back(0);
	for (int i = 1; i < 61; i++) {
		if (grids[i].get_color()) {
			green.push_back(i);
		}
		else {
			white.push_back(i);
		}
	}
	// 当同色连成一片，进行尽头统计
	// “尽头”指周围只有一个同色宫格的宫格，如果玩家试图一笔连接这种颜色的宫格，具备该特征的宫格只能用作开头或结尾
	// 若尽头数<=2，题目过于简单重新出题，否则无需重新出题

	if (one_piece_or_not(green)) {
		if (ends_num(green) <= 2) {
			set_question_auto(num);
			EndBatchDraw();
			return;
		}
	}
	// 白色同理
	if (one_piece_or_not(white)) {
		if (ends_num(white) <= 2) {
			set_question_auto(num);
			EndBatchDraw();
			return;
		}
	}
	//记录答案盘面
	for (int k = 0; k < 61; k++) {
		answer[k] = recorder[k];
	}
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

void you_win_last_stage() {
	grids.clear();//析构所有宫格
	level++;
	loadimage(&pop_up, "assets//pop_up.png", 0, 0);
	loadimage(&pop_up_mask, "assets//pop_up_mask.png", 0, 0);
	int ximg = (w - pop_up.getwidth()) / 2; int yimg = (h - pop_up.getheight()) / 2;
	put_png(ximg, yimg, &pop_up, &pop_up_mask);
	settextstyle(50, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg, "GAME CLEAR");
	int wbt = 200; int hbt = 50;
	settextstyle(20, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	button back(ximg + pop_up.getwidth() / 3 + (2 * pop_up.getwidth() / 3 - wbt) / 2, yimg + 2 * pop_up.getheight() / 3 + (pop_up.getheight() / 3 - hbt) / 2, wbt, hbt, "assets\\UItemplate3_nor.png", "assets\\UItemplate3_over.png", "assets\\UItemplate3_mask.png", "主菜单");
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			back.act_over_mask();
			if (msg.message == WM_LBUTTONDOWN) {
				back.act_button(display_menu);
			}
		}
	}
}

void you_lose(bool *chessboard) {
	grids.clear();//析构所有宫格
	loadimage(&pop_up, "assets//pop_up.png", 0, 0);
	loadimage(&pop_up_mask, "assets//pop_up_mask.png", 0, 0);
	int ximg = (w - pop_up.getwidth()) / 2; int yimg = (h - pop_up.getheight()) / 2;
	put_png(ximg, yimg, &pop_up, &pop_up_mask);
	settextstyle(50, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg, "YOU LOSE");
	if (level != 0) {
		center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg + 120, "当前关卡");
		std::ostringstream oss;
		oss << "level " << level; // 拼接字符串
		std::string levelnum = oss.str();
		center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg + 210, levelnum.c_str());
	}
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

//适配其他函数所做出的改动
void exit() {
	exit(0);
}

void display_menu(){
	//重置关卡数
	level = 1;
	//析构所有宫格
	grids.clear();
	//设置音量
	waveOutSetVolume(NULL, CalculateVolume(volume, volume));
	//切BGM
	music_change_to(0);
	//加载贴图
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
	int wtitle = 800, htitle = 360;
	loadimage(&title, "assets//title.png", wtitle, htitle);
	putimage((w - wtitle) / 2, (yplay - htitle) / 2, &title, SRCAND);
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
	grids.clear();//析构所有宫格
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
	int wbt = 200, hbt = 50;
	settextcolor(WHITE);
	settextstyle(30, 0, "华文行楷", 0, 0, 100, false, false, false);
	button start(w/2, 3 * h / 4+80 , wbt, hbt, "assets//UItemplate3_nor.png", "assets//UItemplate3_over.png", "assets//UItemplate3_mask.png", "开始游戏");

	//持续捕捉鼠标信息
	while (1) {
		back.act_over_mask();
		start.act_over_mask();
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				back.act_button(display_menu);
				start.act_button(display_chooce_mode);
			}
		}
	}
}

void display_setting() {
	grids.clear();//析构所有宫格
	IMAGE setting_background;
	loadimage(&setting_background, "assets//sys_base@base%base.png", w, h);

	IMAGE backgroundpopup, backgroundpopup_mask;
	int wbackgroundpopup = w - 10, hbackgroundpopup = h - 10;
	loadimage(&backgroundpopup, "assets//sys_base@framebase%layer.png", wbackgroundpopup, hbackgroundpopup);
	loadimage(&backgroundpopup_mask, "assets//sys_base@framebase%layer_mask.png", wbackgroundpopup, hbackgroundpopup);
	putimage(0, 0, &setting_background);
	int xbackgroundpopup = (w - backgroundpopup.getwidth()) / 2, ybackgroundpopup = (h - backgroundpopup.getheight()) / 2;
	put_png(xbackgroundpopup, ybackgroundpopup, &backgroundpopup, &backgroundpopup_mask);

	//粘贴返回贴图
	button back(40, 40, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");

	settextstyle(40, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	int xmode = xbackgroundpopup + 40, ymode = hback + 40 + 20 , wmode = 200, hmode = 360, htext = 40;
	center_text(4*wmode, htext, xmode, ymode, "难度选择");

	int wbox = 20, hbox = 20;
	vector<button> mode_chooce;
	mode_chooce.emplace_back(xmode + wmode - wbox, ymode + htext + ((hmode - htext) / 3 - hbox) / 2 , wbox, hbox, "assets//sys_page1@use__speech%button;normal;off.png", "assets//sys_page1@use__speech%button;over;off.png", "assets//sys_page1@use__speech%button;normal;off_mask.png");
	mode_chooce.emplace_back(xmode + wmode - wbox, ymode + htext + ((hmode - htext) / 3 - hbox) / 2 + (hmode - htext) / 3, wbox, hbox, "assets//sys_page1@use__speech%button;normal;off.png", "assets//sys_page1@use__speech%button;over;off.png", "assets//sys_page1@use__speech%button;normal;off_mask.png");
	mode_chooce.emplace_back(xmode + wmode - wbox, ymode + htext + ((hmode - htext) / 3 - hbox) / 2 + 2*(hmode - htext) / 3, wbox, hbox, "assets//sys_page1@use__speech%button;normal;off.png", "assets//sys_page1@use__speech%button;over;off.png", "assets//sys_page1@use__speech%button;normal;off_mask.png");
	mode_chooce[mode].change_image("assets//sys_page1@use__speech%button;normal;on.png", "assets//sys_page1@use__speech%button;over;on.png", "assets//sys_page1@use__speech%button;normal;off_mask.png");

	settextstyle(30, 0, "幼圆", 0, 0, 1000, false, false, false);
	center_text(wmode - wbox, (hmode - htext) / 3, xmode, ymode + htext, "简单模式");
	center_text(wmode - wbox, (hmode - htext) / 3, xmode, ymode + htext+ (hmode - htext) / 3, "普通模式");
	center_text(wmode - wbox, (hmode - htext) / 3, xmode, ymode + htext+ 2 * (hmode - htext) / 3, "困难模式");
	settextstyle(18, 0, "幼圆", 0, 0, 1000, false, false, false);
	center_text(3*wmode , (hmode - htext) / 3, xmode+ wmode, ymode + htext, "被反转过的宫格将被标记并反转颜色");
	center_text(3*wmode , (hmode - htext) / 3, xmode+ wmode, ymode + htext + (hmode - htext) / 3, "被反转过的宫格将被反转颜色但不会被标记");
	center_text(3*wmode , (hmode - htext) / 3, xmode+ wmode, ymode + htext + 2 * (hmode - htext) / 3, "被反转过的宫格将不会有任何变化，需玩家记忆");

	int xvolume = xmode + 4 * wmode, yvolume = ymode, wvolume = 2*wmode, hvolume = htext;
	settextstyle(40, 0, "幼圆", 0, 0, 1000, false, false, false);
	center_text(wvolume, hvolume, xvolume, yvolume, "音量调节");

	//定义音量显示框贴图
	IMAGE box,box_mask;
	//定义音量显示框长宽参数
	wbox = 80, hbox = 40;
	//定义加减符长宽
	int block = 40;

	loadimage(&box, "assets//file_qload@edit%sysbutton;normal.png", wbox, hbox);
	loadimage(&box_mask, "assets//file_qload@edit%sysbutton;normal_mask.png", wbox, hbox);

	//定义音量显示框位置参数
	int xbox = xvolume + block + (wvolume - 2 * block - wbox) / 2, ybox = yvolume + 3 * hvolume;
	put_png(xbox, ybox, &box, &box_mask);
	settextstyle(20, 0, "幼圆", 0, 0, 1000, false, false, false);
	center_text(wbox, hbox, xbox, ybox, to_string(volume).c_str());

	//定义加减符位置参数
	int xmi = xvolume, xpl = xvolume + wvolume - block, ymi = yvolume + 3 * hvolume, ypl = yvolume + 3 * hvolume;
	settextstyle(block, 0, "幼圆", 0, 0, 1000, false, false, false);
	center_text(block, block, xmi, ymi, "-");
	center_text(block, block, xpl, ypl, "+");

	//自定义按钮
	int wsq=241, hsq = 55;
	settextstyle(25, 0, "幼圆", 0, 0, 1000, false, false, false);
	button set_question(w-wsq-180, h-hsq-180, wsq, hsq, "assets//UItemplate3_nor.png", "assets//UItemplate3_over.png", "assets//UItemplate3_mask.png","设置自定义关卡");

	//持续捕捉鼠标信息
	while (1) {
		back.act_over_mask();
		set_question.act_over_mask();
		for (int i = 0; i < 3; i++) {
			mode_chooce[i].act_over_mask();
		}
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				back.act_button(display_menu);
				set_question.act_button(display_set_question);
				for (int i = 0; i < 3; i++) {
					if (mode_chooce[i].act_button(i)) {
						for (int j = 0; j < 3; j++) {
							if (j != i) {
								mode_chooce[j].change_image("assets//sys_page1@use__speech%button;normal;off.png", "assets//sys_page1@use__speech%button;over;off.png", "assets//sys_page1@use__speech%button;normal;off_mask.png");
							}
						}
					}
				}
				if (msg.x > xmi && msg.x<xmi + block && msg.y>ymi && msg.y < ymi + block) {
					if (volume == 0)continue;
					volume -= 10;
					waveOutSetVolume(NULL, CalculateVolume(volume,volume));
					put_png(xbox, ybox, &box, &box_mask);
					settextstyle(20, 0, "幼圆", 0, 0, 1000, false, false, false);
					center_text(wbox, hbox, xbox, ybox, to_string(volume).c_str());
				}
				else if (msg.x > xpl && msg.x<xpl + block && msg.y>ypl && msg.y < ypl + block) {
					if (volume == 100)continue;
					volume += 10;
					waveOutSetVolume(NULL, CalculateVolume(volume, volume));
					put_png(xbox, ybox, &box, &box_mask);
					settextstyle(20, 0, "幼圆", 0, 0, 1000, false, false, false);
					center_text(wbox, hbox, xbox, ybox, to_string(volume).c_str());
				}
			}
		}
	}
}

void save_in(int save_position,IMAGE img, bool* chessboard) {
	std::ostringstream oss;
	oss << "save\\save" << save_position << "\\chessboard_data"; // 拼接三段字符串
	std::string chessboard_file = oss.str();

	std::ostringstream ossimg;
	ossimg << "save\\save" << save_position << "\\image_data.png"; // 拼接三段字符串
	std::string image_file = ossimg.str();

	saveimage(_T(image_file.c_str()), &img);

	ofstream chessboard_save_stream(chessboard_file);

	for (int i = 0; i < 61; i++) {
		chessboard_save_stream << chessboard[i];
	}
	for (int i = 0; i < 61; i++) {
		chessboard_save_stream << answer[i];
	}
	chessboard_save_stream << level;
}

//定义saveload界面缩略图长宽
int wimg = 229*2/3, himg = 128*2/3;

void display_save(bool *chessboard,IMAGE img) {
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
					saves[i].act_button(save_in, i+1, img, chessboard);
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
	for (int i = 61; i < 122; i++) {
		answer[i - 61] = numbers[i];
	}

	// 关闭文件
	inputFile.close();

	retry(chessboard);
}

void load_to_input() {
	// 设置特殊关卡数以便区分
	level = 0;

	//记录当前目录，防止路径错误导致的资源无法正常加载
	char currentDir[256];
	GetCurrentDirectory(256, currentDir);

	// 定义文件选择对话框的参数
	OPENFILENAME ofn;
	char str[100] = ""; // 存储文件路径的字符串

	// 初始化文件选择对话框结构
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL; // 如果有父窗口，可以设置为父窗口的句柄
	ofn.lpstrFile = str;
	ofn.nMaxFile = sizeof(str);
	ofn.lpstrFilter = "All Files\0*.*\0"; // 文件过滤器
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL; // 初始目录
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // 必须选择存在的文件

	// 弹出文件选择对话框
	if (GetOpenFileName(&ofn)) {
		// 如果用户成功选择了文件，str 中将包含文件路径
		SetCurrentDirectory(currentDir);// 重置当前目录
		std::ifstream file(str);
		if (!file.is_open()) {
			MessageBox(NULL, "未选择文件或选择文件失败", "错误", MB_OK | MB_ICONERROR);
			return;
		}

		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// 检查内容长度
		if (content.length() != 122) {
			MessageBox(NULL, "文件类型错误", "错误", MB_OK | MB_ICONERROR);
			return;
		}

		// 检查每个字符是否为0或1
		for (char c : content) {
			if (c != '0' && c != '1') {
				MessageBox(NULL, "文件类型错误", "错误", MB_OK | MB_ICONERROR);
				return;
			}
		}

		load_to(str); // 调用 load_to 函数加载文件
	}
	else {
		// 如果用户取消了选择，或者发生错误
		MessageBox(NULL, "未选择文件或选择文件失败", "错误", MB_OK | MB_ICONERROR);
	}
}

void display_load() {
	// 设置特殊关卡数以便区分
	level = 0;
	//定义游玩模式为保存或自定义
	play_mode = 2;
	grids.clear();//析构所有宫格
	IMAGE save_background, save_caption;
	loadimage(&save_background, "assets\\file_load@base%base.png", w, h);
	int hbar = 76;
	loadimage(&save_caption, "assets\\file_load@caption%layer.png", w, hbar);
	putimage(0, 0, &save_background);
	putimage(0, 0, &save_caption);

	//粘贴返回贴图
	button back(0, h - hback, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");

	vector<button> loads;

	int wsave_img = 412, hsave_img = 132;
	IMAGE save_img;
	for (int i = 0; i < 8; i++) {
		loads.emplace_back((w / 2 - wsave_img) / 2 + (w / 2) * (i / 4), hbar + (h / 4 - hsave_img) / 2 + (h - hbar) / 4 * (i % 4), wsave_img, hsave_img, "assets\\file_save@item%button;normal.png", "assets\\file_save@item%button;over.png", "assets\\file@item%button;mask.png");
		std::ostringstream oss;
		oss << "save\\save" << i + 1 << "\\image_data.png"; // 拼接三段字符串
		std::string image_file = oss.str();
		loadimage(&save_img, image_file.c_str(), wimg, himg, 0);
		putimage(loads[i].get_x() + 12, loads[i].get_y() + 12, &save_img);
	}

	int wbt = 200, hbt = 50;
	settextstyle(20, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	button user_input(w - wbt - 40, (hbar - hbt) / 2 , wbt, hbt, "assets\\UItemplate3_nor.png", "assets\\UItemplate3_over.png", "assets\\UItemplate3_mask.png", "载入自定义关卡");
    
	//持续捕捉鼠标信息
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			back.act_over_mask();
			user_input.act_over_mask();
			for (int i = 0; i < 8; i++) {
				if (loads[i].state_change()) {

					//开始双缓冲绘图
					BeginBatchDraw();

					loads[i].act_over_mask();
					std::ostringstream oss;
					oss << "save\\save" << i + 1 << "\\image_data.png"; // 拼接三段字符串
					std::string image_file = oss.str();
					loadimage(&save_img, image_file.c_str(), wimg, himg);
					putimage(loads[i].get_x() + 12, loads[i].get_y() + 12, &save_img);

					//缓冲完毕开始绘图
					EndBatchDraw();
				}
			}
			if (msg.message == WM_LBUTTONDOWN) {
				back.act_button(display_menu);
				user_input.act_button(load_to_input);
				for (int i = 0; i < 8; i++) {
					std::ostringstream oss;
					oss << "save\\save" << i + 1 << "\\chessboard_data"; // 拼接三段字符串
					loads[i].act_button(load_to, oss.str().c_str());
				}
			}
		}
	}
}

void display_chooce_stage() {
	grids.clear();//析构所有宫格
	//设置游玩模式为闯关
	play_mode = 1;
	//设置背景
	IMAGE chooce_stage_background;
	loadimage(&chooce_stage_background, "assets\\extra@base%base.png", w, h);
	putimage(0, 0, &chooce_stage_background);

	//粘贴返回贴图
	button back(40, 40, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");

	//设置选关按键
	vector<button> chooce_stage;

	//设置按键大小
	int wbt = 80, hbt = 80;

	for (int j = 1; j < 6; j++) {
		settextstyle(60, 0, "幼圆", 0, 0, 1000, false, false, false);
		settextcolor(WHITE);
		chooce_stage.emplace_back((j - 1) * (w / 5) + (w / 5 - wbt) / 2, (h - hbt) / 2, wbt, hbt, "assets//stage.png", "assets//stage_over.png", "assets//stage_mask.png",to_string(j));
	}

	while (1) {
		back.act_over_mask();
		for (int i = 0; i < 5; i++) {
			chooce_stage[i].act_over_mask();
		}
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				back.act_button(display_menu);
				for (int i = 0; i < 5; i++) {
					std::ostringstream oss;
					oss << "level " << i + 1; // 拼接三段字符串
					level = i + 1;
					chooce_stage[i].act_button(load_to,oss.str().c_str());
				}
			}
		}
	}
}

void display_chooce_mode() {
	grids.clear();//析构所有宫格
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

void hint(int xhint,int yhint ,int whint,int hhint) {
	for (int i = 0; i < 61; i++) {
		if (answer[i]&&grids[i].get_reversed_time()==0) {
			//高亮提示答案路线
			grids[i].hint_cover();
		}
		else if (!answer[i] && grids[i].get_reversed_time() == 1) {
			//高亮提示错误步骤
			grids[i].hint_wrong_cover();
		}
	}
	put_png(xhint, yhint, &hint_window, &hint_window_mask);
	settextstyle(20, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(BLACK);
	center_text(whint, hhint, xhint, yhint, "提示信息已高亮显示");
}

const static int whint = 1.2 * 214, hhint = 1.2 * 246, xhint = w - whint, yhint = 240;

void set_chessboard() {
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

	IMAGE save_image, save_image_mask;
	int length = 30;
	int text_length = 100;
	int y_to_btm = 20;
	loadimage(&save_image, "assets\\stage.png", length, length);
	loadimage(&save_image_mask, "assets\\stage_mask.png", length, length);
	put_png(w - text_length - length, h - y_to_btm - length, &save_image, &save_image_mask);
	put_png(w - 2 * (text_length + length), h - y_to_btm - length, &save_image, &save_image_mask);
	settextstyle(20, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	center_text(length, length, w - text_length - length, h - y_to_btm - length, "S");
	center_text(text_length, length, w - text_length, h - y_to_btm - length, "存档");
	center_text(length, length, w - 2 * (text_length + length), h - y_to_btm - length, "R");
	center_text(text_length, length, w - 2 * text_length - length, h - y_to_btm - length, "重试");

	loadimage(&hint_window, "assets\\sys_base@helpbase%layer.png", whint, hhint);
	loadimage(&hint_window_mask, "assets\\sys_base@helpbase%layer_mask.png", whint, hhint);

	put_png(xhint, yhint, &hint_window, &hint_window_mask);
	settextstyle(30, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(BLACK);
	center_text(whint, hhint, xhint, yhint, "按H获得提示");
	//缓冲完毕开始绘图
	EndBatchDraw();
}

//适配游玩反馈函数，如果闯关模式最终关卡或自定义关卡代替void next_level()用
void last_level() {};

void play_interact(button back,bool* chessboard, void next_level()) {
	//持续捕捉鼠标信息
	int judge_msg;
	while (1) {
		back.act_over_mask();
		if (peekmessage(&msg, EX_MOUSE | EX_KEY)) {
			if (msg.message == WM_LBUTTONDOWN) {
				back.act_button(display_menu);
				for (int i = 0; i <= 60; i++) {
					if (grids[i].reverse_or_not()) {
						if (!grids[i].reverse_color(mode)) {
							you_lose(chessboard);
						};//点击时反转的宫格
						judge_msg = i;
						while (1) {
							if (peekmessage(&msg, EX_MOUSE | EX_KEY)) {
								if (msg.message == WM_LBUTTONUP) {
									if (complete_or_not()) {
										if (next_level == last_level) {
											you_win_last_stage();
										}
										else {
											you_win(next_level);
										}
									}
									else {
										you_lose(chessboard);
									}
									i = 61;//停止遍历
									break;
								}
								else {
									for (int j = 0; j <= 60; j++) {
										if (grids[j].reverse_or_not() && adjacent_or_not(judge_msg, j)) {
											if (!grids[j].reverse_color(mode)) {
												you_lose(chessboard);
											};
											judge_msg = j;
											break;
										}
									}
								}
							}
							else if (msg.message == WM_KEYDOWN) // 判断是否是按键按下消息
							{
								if (msg.vkcode == 0x53) // 判断是否按下 s 或 S 键
								{
									IMAGE img;
									int wget_img = 229 * 5.7, hget_img = 128 * 5.7;
									getimage(&img, (w - wget_img) / 2, (h - hget_img) / 2, wget_img, hget_img);
									display_save(chessboard, img);
								}
								else if (msg.vkcode == 0x48) {// 判断是否按下 h 或 H 键
									hint(xhint, yhint, whint, hhint);
								}
								else if (msg.vkcode == 0x52) {// 判断是否按下 r 或 R 键
									retry(chessboard);
								}
							}
						}
					}
				}
			}
			else if (msg.message == WM_KEYDOWN) // 判断是否是按键按下消息
			{
				if (msg.vkcode == 0x53) // 判断是否按下 s 或 S 键
				{
					IMAGE img;
					int wget_img = 229 * 5.7, hget_img = 128 * 5.7;
					getimage(&img, (w - wget_img) / 2, (h - hget_img) / 2, wget_img, hget_img);
					display_save(chessboard,img);
				}
				else if (msg.vkcode == 0x48) {
					hint(xhint, yhint, whint, hhint);
				}
				else if (msg.vkcode == 0x52) {
					retry(chessboard);
				}
			}
		}
	}
}

//游玩随机生成关卡
void display_play() {
	grids.clear();//析构所有宫格
	//重置游玩模式
	play_mode = 0;
	//切BGM
	music_change_to(1);
	//换背景
	cleardevice();
	loadimage(&play_background, "assets//play_background.png", w, h);
	putimage(0, 0, &play_background);

	//粘贴返回贴图
	button back(40, 40, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");
	settextstyle(60, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	std::ostringstream oss;
	oss << "level " << level; // 拼接字符串
	std::string levelnum = oss.str();
	center_text(180, 80, (w - 180) / 4, 60, levelnum.c_str());

	set_chessboard();

	set_question_auto(rand()%6+36);

	//记录本关盘面，供重试功能使用
	bool chessboard[61];
	for (int i = 0; i <= 60; i++) {
		chessboard[i] = grids[i].get_color();
	}

	play_interact(back, chessboard, display_play);
}

void next_specific_level() {
	std::ostringstream oss;
	oss << "level " << level; // 拼接字符串
	load_to(oss.str().c_str());
}

void interact(button back, bool* chessboard) {
	if (play_mode == 0) {
		play_interact(back, chessboard, display_play);
		return;
	}
	else if (play_mode == 1 && level != 5) {
		play_interact(back, chessboard, next_specific_level);
		return;
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
						if (!grids[i].reverse_color(mode)) {
							you_lose(chessboard);
						};//点击时反转的宫格
						judge_msg = i;
						while (1) {
							if (peekmessage(&msg, EX_MOUSE | EX_KEY)) {
								if (msg.message == WM_LBUTTONUP) {
									if (complete_or_not()) {
										you_win_last_stage();
									}
									else {
										you_lose(chessboard);
									}
									i = 61;//停止遍历
									break;
								}
								else {
									for (int j = 0; j <= 60; j++) {
										if (grids[j].reverse_or_not() && adjacent_or_not(judge_msg, j)) {
											if (!grids[j].reverse_color(mode)) {
												you_lose(chessboard);
											};
											judge_msg = j;
											break;
										}
									}
								}
							}
							else if (msg.message == WM_KEYDOWN) // 判断是否是按键按下消息
							{
								if (msg.vkcode == 0x53) // 判断是否按下 s 或 S 键
								{
									IMAGE img;
									int wget_img = 229 * 5.7, hget_img = 128 * 5.7;
									getimage(&img, (w - wget_img) / 2, (h - hget_img) / 2, wget_img, hget_img);
									display_save(chessboard, img);
								}
								else if (msg.vkcode == 0x48) {// 判断是否按下 h 或 H 键
									hint(xhint, yhint, whint, hhint);
								}
								else if (msg.vkcode == 0x52) {// 判断是否按下 r 或 R 键
									retry(chessboard);
								}
							}
						}
					}
				}
			}
			else if (msg.message == WM_KEYDOWN) // 判断是否是按键按下消息
			{
				if (msg.vkcode == 0x53) // 判断是否按下 s 或 S 键
				{
					IMAGE img;
					int wget_img = 229 * 5.7, hget_img = 128 * 5.7;
					getimage(&img, (w - wget_img) / 2, (h - hget_img) / 2, wget_img, hget_img);
					display_save(chessboard, img);
				}
				else if (msg.vkcode == 0x48) {
					hint(xhint, yhint, whint, hhint);
				}
				else if (msg.vkcode == 0x52) {
					retry(chessboard);
				}
			}
		}
	}
}

//游玩某指定关卡
void retry(bool* chessboard) {
	grids.clear();//析构所有宫格
	//切BGM
	music_change_to(1);
	//换背景
	cleardevice();
	loadimage(&play_background, "assets//play_background.png", w, h);
	putimage(0, 0, &play_background);

	set_chessboard();

	//粘贴返回贴图
	button back(40, 40, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	if (level != 0) {
		settextstyle(60, 0, "幼圆", 0, 0, 1000, false, false, false);
		std::ostringstream oss;
		oss << "level " << level; // 拼接字符串
		std::string levelnum = oss.str();
		center_text(180, 80, (w - 180) / 4, 60, levelnum.c_str());
	}
	else {
		settextstyle(32, 0, "幼圆", 0, 0, 1000, false, false, false);
		center_text(180, 80, (w - 180) / 4, 60, "保存或自定义关卡");
	}
	
	//加载盘面
	for (int i = 0; i < 61; i++) {
		if (chessboard[i]) {
			grids[i].reverse_set_question();
		}
	}

	//记录本关盘面，供重试功能使用
	for (int i = 0; i <= 60; i++) {
		chessboard[i] = grids[i].get_color();
	}

	interact(back, chessboard);
}

void change_circle_color(int n) {
	for (int i = 0; i < 61; i++) {
		if (grids[i].get_circle() == n) {
			grids[i].reverse_set_question();
		}
	}
}

void save_custom_levels() {
	//记录当前目录，防止路径错误导致的资源无法正常加载
	char currentDir[256];
	GetCurrentDirectory(256, currentDir);

	OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFile[260];       // buffer for file name

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn)); // 将ofn结构体的所有成员初始化为0
	ofn.lStructSize = sizeof(ofn); // 设置结构体的大小
	ofn.hwndOwner = NULL; // 设置对话框的父窗口句柄，NULL表示没有父窗口
	ofn.lpstrFile = szFile; // 设置文件名字符串的缓冲区
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0'; // 将文件名缓冲区的第一个字符设置为'\0'，表示空字符串
	ofn.nMaxFile = sizeof(szFile); // 设置文件名缓冲区的最大长度
	ofn.lpstrFilter = TEXT("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0"); // 设置文件类型过滤器
	ofn.nFilterIndex = 1; // 设置默认的文件类型过滤器索引，1表示第二个过滤器（All Files (*.*)）
	ofn.lpstrFileTitle = NULL; // 不使用文件标题
	ofn.nMaxFileTitle = 0; // 文件标题的最大长度
	ofn.lpstrInitialDir = NULL; // 不设置初始目录
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // 设置标志，要求路径和文件必须存在

	// Display the Save As dialog box.
	if (GetSaveFileName(&ofn) == TRUE) { // 弹出保存文件对话框
		std::ofstream file(ofn.lpstrFile, std::ios::out); // 尝试以输出模式打开文件
		if (!file.is_open()) { // 如果文件打开失败
			MessageBox(NULL, "文件打开失败", "错误", MB_OK | MB_ICONERROR); // 输出错误信息
		}

		for (int i = 0; i < 61; i++) {
			file << grids[i].get_color();
		}
		for (int i = 0; i < 61; i++) {
			file << grids[i].get_reversed_time();
		}
		file.close(); // 关闭文件

		MessageBox(NULL, "文件保存成功", "保存成功", MB_OK | MB_ICONERROR); // 输出成功创建文件的信息
	}
	else {
		MessageBox(NULL, "取消操作", "保存失败", MB_OK | MB_ICONERROR); // 如果用户取消了操作，输出提示信息
	}
	SetCurrentDirectory(currentDir);// 重置当前目录
}

void display_set_question() {
	grids.clear();//析构所有宫格
	//切BGM
	music_change_to(1);
	//换背景
	cleardevice();
	loadimage(&play_background, "assets//play_background.png", w, h);
	putimage(0, 0, &play_background);

	//粘贴返回贴图
	button back(40, 40, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");
	settextstyle(60, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	std::ostringstream oss;
	oss << "level " << level; // 拼接字符串
	std::string levelnum = oss.str();
	center_text(180, 80, (w - 180) / 4, 60, levelnum.c_str());

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

	IMAGE save_image, save_image_mask;
	int length = 30;
	int text_length = 100;
	int y_to_btm = 20;
	loadimage(&save_image, "assets\\stage.png", length, length);
	loadimage(&save_image_mask, "assets\\stage_mask.png", length, length);
	put_png(w - 2 * (text_length + length), h - y_to_btm - length, &save_image, &save_image_mask);
	settextstyle(20, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	center_text(length, length, w - 2 * (text_length + length), h - y_to_btm - length, "R");
	center_text(text_length, length, w - 2 * text_length - length, h - y_to_btm - length, "重置");

	loadimage(&hint_window, "assets\\sys_base@helpbase%layer.png", whint, hhint);
	loadimage(&hint_window_mask, "assets\\sys_base@helpbase%layer_mask.png", whint, hhint);

	put_png(xhint, yhint, &hint_window, &hint_window_mask);
	settextstyle(16, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(BLACK);
	center_text(whint, hhint, xhint, yhint-30, "请先一笔画出题解");
	center_text(whint, hhint, xhint, yhint+30, "再设置每圈颜色是否反转");
	settextstyle(24, 0, "幼圆", 0, 0, 1000, false, false, false);
	center_text(50, 50, 100, 120, "点击以反转各圈颜色");

	settextstyle(28, 0, "幼圆", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	button OK(w - 240, h - 140, 160, 50, "assets//UItemplate3_nor.png", "assets//UItemplate3_over.png", "assets//UItemplate3_mask.png","完成");
	vector<button> circle_color_change;
	for (int i = 0; i < 5; i++) {
		circle_color_change.emplace_back(100, 200 + 80 * i, 50, 50, "assets//stage.png", "assets//stage_over.png", "assets//stage_mask.png", to_string(i+1).c_str());
	}
	//缓冲完毕开始绘图
	EndBatchDraw();

	int judge_msg;

restart:
	while (1) {
		back.act_over_mask();
		OK.act_over_mask();
		for (int i = 0; i < 5; i++) {
			circle_color_change[i].act_over_mask();
		}
		if (peekmessage(&msg, EX_MOUSE | EX_KEY)) {
			if (msg.message == WM_LBUTTONDOWN) {
				back.act_button(display_menu);
				OK.act_button(save_custom_levels);
				for (int i = 0; i < 5; i++) {
					circle_color_change[i].act_button(change_circle_color, i);
				}
				for (int i = 0; i <= 60; i++) {
					if (grids[i].reverse_or_not()) {
						reset_grids();
						if (!grids[i].reverse_color(1)) {
							MessageBox(NULL, "请勿重复经过同一宫格", "请重试", MB_OK | MB_ICONERROR);
							reset_grids();
							goto restart;
						};//点击时反转的宫格
						judge_msg = i;
						while (1) {
							if (peekmessage(&msg, EX_MOUSE | EX_KEY)) {
								if (msg.message == WM_LBUTTONUP) {
									if (complete_or_not()) {
										MessageBox(NULL, "请设置具有可玩性的关卡", "请重试", MB_OK | MB_ICONERROR);
										reset_grids();
										goto restart;
									}
									i = 61;//停止遍历
									break;
								}
								else {
									for (int j = 0; j <= 60; j++) {
										if (grids[j].reverse_or_not() && adjacent_or_not(judge_msg, j)) {
											if (!grids[j].reverse_color(1)) {
												MessageBox(NULL, "请勿重复经过同一宫格", "请重试", MB_OK | MB_ICONERROR);
												reset_grids();
												goto restart;
											};
											judge_msg = j;
											break;
										}
									}
								}
							}
							else if (msg.message == WM_KEYDOWN) // 判断是否是按键按下消息
							{
								if (msg.vkcode == 0x52) {// 判断是否按下 r 或 R 键
									reset_grids();
								}
							}
						}
					}
				}
			}
			else if (msg.message == WM_KEYDOWN) // 判断是否是按键按下消息
			{
				if (msg.vkcode == 0x52) {// 判断是否按下 r 或 R 键
					reset_grids();
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