//���Ǭ��
//��ģʽ������ת���Ĺ��񽫱���ǲ���ת��ɫ
//��ͨģʽ������ת���Ĺ��񽫱���ת��ɫ�����ᱻ���
//����ģʽ������ת���Ĺ��񽫲������κα仯������Ҽ���


/*����ǿ���ԡ��еġ����Ǭ�����������£�

1.��Ϸ���棺��Ϸ��һ�������������н��У�����������ֲ��źڰ�������ɫ�Ĺ���
2.������ʽ�������Ҫͨ��������һ�ʴ�����������Щ����
3.����仯��ÿ�����񱻴�������Ϊ��ɫ,��ÿ���������ֻ�ܷ�תһ�Ρ�
4.ʤ����������ÿȦ���й�����ɫ��ͬʱ������ɹ���������5Ȧ��

����Ϸ������ҵ��߼�˼ά�����Ϳռ�����������Ҫ�����ϸ�۲�͹滮����·�����ԴﵽĿ�ꡣ*/


//���õ�������·����Ϊ�𰸣�Ȼ��ÿһȦ����·�����·�������ɫ

#include<iostream>
#include <windows.h>
#include <stdio.h>
#include <graphics.h>
#include <vector>
#include <time.h>
#include <conio.h>
#include<fstream>
#include <sstream>
#pragma comment(lib, "winmm.lib")

using namespace std;

void put_png(int x, int y, IMAGE* img) {
	BeginBatchDraw();
	//��ͼ�ް�ɫ
	putimage(x, y, img, SRCAND);
	EndBatchDraw();
}

void put_png(int x, int y, IMAGE* img,IMAGE* img_mask) {
	BeginBatchDraw();
	//��ͼ�а�ɫ��������ͼ
	putimage(x, y, img_mask, NOTSRCERASE);
	putimage(x, y, img, SRCINVERT);
	EndBatchDraw();
}

//���û�������
int w = 1360, h = 765;

//��������ͼ����ͼ
IMAGE title;
IMAGE background,rule_background,play_background;
IMAGE quit, play, load, how_to_play, setting, pic, pic_mask, grid0, grid0_mask, grid1;
IMAGE button_mask;
IMAGE hint_window, hint_window_mask;
//IMAGE *back_mask=&back+1;
COLORREF color;

//������Ϣ�ṹ�����
ExMessage msg = { 0 };

//�����˳���,���ؼ���һ�����С
int wquit = 60, hquit = 60, wbutton = 315, hbutton = 70, wback = 90, hback = 60;

//����ؿ���
int level = 1;
//����ģʽ״̬
int mode = 1;
//���嵱ǰBGM״̬
int music = -1;
//��������
int volume=100;
//�л�BGM����
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

//��¼���ִ�
int answer[61] = { 0 };

//���庯��
void display_menu();
void display_how_to_play();
void display_setting();
void display_load();
void display_play();
void you_win(void next_stage());
void you_lose(bool *chessboard);
void retry(bool *chessboard);
void display_chooce_mode();

//�����м���������
void center_text(int wbutton, int hbutton, int xtop_left, int ytop_left, const char str[]) {
	int xchar = (wbutton - textwidth(str)) / 2;
	int ychar = (hbutton - textheight(str)) / 2;
	outtextxy(xtop_left + xchar, ytop_left + ychar, str);
}

class button {
private:
	int x=0, y=0;//λ��
	int w=0, h=0;//����
	IMAGE button_nor, button_mask, button_over;
	string word;
	bool state = false;//false������겻�ڰ�����

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
		//��ť��ת����
		if (msg.x > x && msg.x<x + w && msg.y>y && msg.y < y + h) {
			f();//�����ť������ת��ָ������
		}
	}

	void act_button(void f(bool *b),bool *b) {
		//��ť��ת����
		if (msg.x > x && msg.x<x + w && msg.y>y && msg.y < y + h) {
			f(b);//�����ť������ת��ָ������
		}
	}

	void act_button(void save_in(int save_position,IMAGE img, bool* chessboard, int level), int save_position, IMAGE img, bool* chessboard, int level) {
		//��ť��ת����
		if (msg.x > x && msg.x<x + w && msg.y>y && msg.y < y + h) {
			save_in(save_position, img, chessboard, level);
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
	int x, y;//λ��
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

bool adjacent_or_not(int judge_msg,int j) {
	//���j��judge_msg���ڷ���ture

	if (judge_msg != j) {
		if (grids[judge_msg].get_circle() == 0) {
			//���ǰһ�����������Ĺ���
			if (grids[j].get_circle() == 1) {
				return true;
			}
			else {
				return false;
			}
		}
		else if (grids[judge_msg].get_position() % grids[judge_msg].get_circle() == 0) {
			//���ǰһ���ǽ��ϵĹ���

			if (grids[judge_msg].get_circle() == grids[j].get_circle() - 1) {
				//�������Ȧ
				if (grids[j].get_position() == (grids[j].get_circle() * grids[judge_msg].get_position() / grids[judge_msg].get_circle() + 1) % (grids[j].get_circle() * 6) || grids[j].get_position() == (grids[j].get_circle() * grids[judge_msg].get_position() / grids[judge_msg].get_circle()) % (grids[j].get_circle() * 6) || grids[j].get_position() == (grids[j].get_circle() * grids[judge_msg].get_position() / grids[judge_msg].get_circle() - 1 + grids[j].get_circle() * 6) % (grids[j].get_circle() * 6)) {
					return true;
				}
				else {
					return false;
				}
			}
			else if (grids[judge_msg].get_circle() == grids[j].get_circle()) {
				//�����ͬһȦ
				if (grids[j].get_position() == (grids[judge_msg].get_position() + 1) % (grids[j].get_circle() * 6) || grids[j].get_position() == (grids[judge_msg].get_position() - 1 + grids[j].get_circle() * 6) % (grids[j].get_circle() * 6)) {
					return true;
				}
				else {
					return false;
				}
			}
			else if (grids[judge_msg].get_circle() == grids[j].get_circle() + 1) {
				//�������Ȧ
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
			//���ǰһ���Ǳ��ϵĹ���

			if (grids[judge_msg].get_circle() == grids[j].get_circle() - 1) {
				//�������Ȧ
				if (grids[j].get_position() == (grids[judge_msg].get_position() / grids[judge_msg].get_circle() * grids[j].get_circle() + grids[judge_msg].get_position() % grids[judge_msg].get_circle()) % (grids[j].get_circle() * 6) || grids[j].get_position() == (grids[judge_msg].get_position() / grids[judge_msg].get_circle() * grids[j].get_circle() + grids[judge_msg].get_position() % grids[judge_msg].get_circle() + 1) % (grids[j].get_circle() * 6)) {
					return true;
				}
				else {
					return false;
				}
			}
			else if (grids[judge_msg].get_circle() == grids[j].get_circle()) {
				//�����ͬһȦ
				if (grids[j].get_position() == (grids[judge_msg].get_position() + 1) % (grids[j].get_circle() * 6) || grids[j].get_position() == (grids[judge_msg].get_position() - 1 + grids[j].get_circle() * 6) % (grids[j].get_circle() * 6)) {
					return true;
				}
				else {
					return false;
				}
			}
			else if (grids[judge_msg].get_circle() == grids[j].get_circle() + 1) {
				//�������Ȧ
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
	//��¼�跴ת������
	int* reverse = new int[num];
	//��¼��ǰ����Χ�ɷ�ת�Ĺ��񣬼�¼����������ɫ���
	int container[6] = {0}, recorder[61] = { 0 };
	//��¼��ǰ����Χ�ɷ�ת�Ĺ�������
	int counter = 0;
	//����������
	int i = rand() % 61;
	for (int k=0;k<num;k++) {
		if (i == 0) {//���·���������ĸ����³��⣬������Ŀ���ڼ򵥸���
			set_question_auto(num);
			return;
		}
		//��ת��ǰ������ɫ
		recorder[i] = 1;
		//��¼��ǰ����
		reverse[k] = i;
		//���õ�ǰ����Χ�ɷ�ת�Ĺ�������
		counter = 0;
		//����ǰ����Χ�ɷ�ת���¼��������
		for (int j = 0; j < 61; j++) {
			if (!recorder[j]) {
				if (adjacent_or_not(i, j)) {
					container[counter] = j;
					counter++;
				}
			}
		}
		//������⵽��·�����³���
		if (counter == 0&&k!=num-1) {
			set_question_auto(num);
			return;
		}//�����������������У����µ�ǰ��
		else if (k != num - 1) {
			i = container[rand() % counter];
		}
	}
	//�������ÿȦ��ɫ
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
	//���ÿȦ��ɫ����ͬ����������ÿȦ��ɫ
	if (check[0] == check[1] &&check[1] == check[2] &&check[2] == check[3] &&check[3] ==check[4] ) {
		goto change_circle_color;
	}
	//���ͬɫ����һ����ɣ����ڼ����³���
	

	//����


	//��ת��ɫ����ɳ���
	for (int k = 0; k < num; k++) {
		grids[reverse[k]].reverse_set_question();
	}
	//��¼������
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
	grids.clear();//�������й���
	level++;
	loadimage(&pop_up, "assets//pop_up.png", 0, 0);
	loadimage(&pop_up_mask, "assets//pop_up_mask.png", 0, 0);
	int ximg = (w - pop_up.getwidth()) / 2; int yimg = (h - pop_up.getheight()) / 2;
	put_png(ximg, yimg, &pop_up, &pop_up_mask);
	settextstyle(50, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg, "YOU WIN");
	int wbt = 200; int hbt = 50;
	settextstyle(20, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	button next(ximg + pop_up.getwidth() / 3 + (pop_up.getwidth() / 3 - wbt) / 2, yimg + 2 * pop_up.getheight() / 3 + (pop_up.getheight() / 3 - hbt) / 2, wbt, hbt, "assets\\UItemplate3_nor.png", "assets\\UItemplate3_over.png", "assets\\UItemplate3_mask.png","��һ��");
	button back(ximg + 2* pop_up.getwidth() / 3 + (pop_up.getwidth() / 3 - wbt) / 2, yimg + 2 * pop_up.getheight() / 3 + (pop_up.getheight() / 3 - hbt) / 2, wbt, hbt , "assets\\UItemplate3_nor.png", "assets\\UItemplate3_over.png", "assets\\UItemplate3_mask.png","���˵�");
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
	grids.clear();//�������й���
	level = 1;
	loadimage(&pop_up, "assets//pop_up.png", 0, 0);
	loadimage(&pop_up_mask, "assets//pop_up_mask.png", 0, 0);
	int ximg = (w - pop_up.getwidth()) / 2; int yimg = (h - pop_up.getheight()) / 2;
	put_png(ximg, yimg, &pop_up, &pop_up_mask);
	settextstyle(50, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg, "YOU LOSE");
	int wbt = 200; int hbt = 50;
	settextstyle(20, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	button next(ximg + pop_up.getwidth() / 3 + (pop_up.getwidth() / 3 - wbt) / 2, yimg + 2 * pop_up.getheight() / 3 + (pop_up.getheight() / 3 - hbt) / 2, wbt, hbt, "assets\\UItemplate3_nor.png", "assets\\UItemplate3_over.png", "assets\\UItemplate3_mask.png", "����");
	button back(ximg + 2 * pop_up.getwidth() / 3 + (pop_up.getwidth() / 3 - wbt) / 2, yimg + 2 * pop_up.getheight() / 3 + (pop_up.getheight() / 3 - hbt) / 2, wbt, hbt, "assets\\UItemplate3_nor.png", "assets\\UItemplate3_over.png", "assets\\UItemplate3_mask.png", "���˵�");
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
	grids.clear();//�������й���
	//��BGM
	music_change_to(0);
	loadimage(&pic, "assets//menu_pic.png", 500, 500);
	loadimage(&pic_mask, "assets//menu_pic_mask.png", 500, 500);
	loadimage(&background, "assets//background.png", w, h);
	//���˵�����ͼ
	putimage(0, 0, &background);

	//���岻ͬ�������Ͻ�λ��
	int xquit = 1260, yquit = 665;
	int xplay = 68, yplay = 306;
	int xload = xplay, yload = yplay + (h - yplay) / 4;
	int xhow_to_play = xplay, yhow_to_play = yplay + 2 * (h - yplay) / 4;
	int xsetting = xplay, ysetting = yplay + 3 * (h - yplay) / 4;

	//���ݵ�һ������λ�÷��ñ���
	int wtitle = 800, htitle = 360;
	loadimage(&title, "assets//title.png", wtitle, htitle);
	putimage((w - wtitle) / 2, (yplay - htitle) / 2, &title, SRCAND);
	put_png(xplay + wbutton + 200, yplay - 80, &pic, &pic_mask);

	//���ð�����ͼ
	button quit(xquit, yquit,wquit,hquit, "assets//quit.jpg", "assets//quit_over.png");

	settextstyle(50, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	button play(xplay, yplay, wbutton, hbutton, "assets//UItemplate2.png", "assets//UItemplate2_over.png", "assets//UItemplate_mask.png","����Ϸ");
	
	settextcolor(BLACK);
	button load(xload, yload, wbutton, hbutton, "assets//UItemplate1.png", "assets//UItemplate1_over.png", "assets//UItemplate_mask.png", "������Ϸ");
	button how_to_play(xhow_to_play, yhow_to_play, wbutton, hbutton, "assets//UItemplate1.png", "assets//UItemplate1_over.png", "assets//UItemplate_mask.png", "��Ϸ����");
	button setting(xsetting, ysetting, wbutton, hbutton, "assets//UItemplate1.png", "assets//UItemplate1_over.png", "assets//UItemplate_mask.png", "����");

	//���˵�ҳ����Ϣ����
	
	//������׽�����Ϣ
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
	grids.clear();//�������й���
	//������
	cleardevice();
	loadimage(&rule_background, "assets//rule_background.png", w, h);
	putimage(0, 0, &rule_background);

	//ճ��������ͼ
	button back(40, 40, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");

	//д��
	settextstyle(120, 0, "�����п�", 0, 0, 200, false, false, false);
	color = RGB(110, 180, 200);
	settextcolor(color);
	setbkmode(TRANSPARENT);
	center_text(w, h / 3, 0, 0, "���Ǭ��  ��Ϸ����");
	settextstyle(29, 0, "�����п�", 0, 0, 0, false, false, false);
	color = RGB(200, 120, 45);
	settextcolor(color);

	center_text(w, h / 12, 0, h / 3, "��Ϸ��һ�������������н��У�����������ֲ����̰�������ɫ�Ĺ���");
	center_text(w, h / 12, 0, h / 3 + h / 12, "�����Ҫͨ��������һ�ʴ�����������Щ����");
	center_text(w, h / 12, 0, h / 3 + h / 6, "ÿ�����񱻴�������Ϊ��ɫ, ��ÿ���������ֻ�ܷ�תһ�Ρ�");
	center_text(w, h / 12, 0, h / 3 + h / 4, "��ÿȦ���й�����ɫ��ͬʱ������ɹ���");

	//��ʼ��Ϸ��ť
	int wbt = 200, hbt = 50;
	settextcolor(WHITE);
	settextstyle(30, 0, "�����п�", 0, 0, 100, false, false, false);
	button start(w/2, 3 * h / 4+80 , wbt, hbt, "assets//UItemplate3_nor.png", "assets//UItemplate3_over.png", "assets//UItemplate3_mask.png", "��ʼ��Ϸ");

	//������׽�����Ϣ
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

DWORD CalculateVolume(int leftPercent, int rightPercent) {
	return (DWORD)((leftPercent * 0xFFFF / 100) | ((rightPercent * 0xFFFF / 100) << 16));
}

void display_setting() {
	grids.clear();//�������й���
	IMAGE setting_background;
	loadimage(&setting_background, "assets//sys_base@base%base.png", w, h);

	IMAGE backgroundpopup, backgroundpopup_mask;
	int wbackgroundpopup = w - 10, hbackgroundpopup = h - 10;
	loadimage(&backgroundpopup, "assets//sys_base@framebase%layer.png", wbackgroundpopup, hbackgroundpopup);
	loadimage(&backgroundpopup_mask, "assets//sys_base@framebase%layer_mask.png", wbackgroundpopup, hbackgroundpopup);
	putimage(0, 0, &setting_background);
	int xbackgroundpopup = (w - backgroundpopup.getwidth()) / 2, ybackgroundpopup = (h - backgroundpopup.getheight()) / 2;
	put_png(xbackgroundpopup, ybackgroundpopup, &backgroundpopup, &backgroundpopup_mask);

	//ճ��������ͼ
	button back(40, 40, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");

	settextstyle(40, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	int xmode = xbackgroundpopup + 40, ymode = hback + 40 + 20 , wmode = 200, hmode = 360, htext = 40;
	center_text(4*wmode, htext, xmode, ymode, "�Ѷ�ѡ��");

	int wbox = 20, hbox = 20;
	vector<button> mode_chooce;
	mode_chooce.emplace_back(xmode + wmode - wbox, ymode + htext + ((hmode - htext) / 3 - hbox) / 2 , wbox, hbox, "assets//sys_page1@use__speech%button;normal;off.png", "assets//sys_page1@use__speech%button;over;off.png", "assets//sys_page1@use__speech%button;normal;off_mask.png");
	mode_chooce.emplace_back(xmode + wmode - wbox, ymode + htext + ((hmode - htext) / 3 - hbox) / 2 + (hmode - htext) / 3, wbox, hbox, "assets//sys_page1@use__speech%button;normal;off.png", "assets//sys_page1@use__speech%button;over;off.png", "assets//sys_page1@use__speech%button;normal;off_mask.png");
	mode_chooce.emplace_back(xmode + wmode - wbox, ymode + htext + ((hmode - htext) / 3 - hbox) / 2 + 2*(hmode - htext) / 3, wbox, hbox, "assets//sys_page1@use__speech%button;normal;off.png", "assets//sys_page1@use__speech%button;over;off.png", "assets//sys_page1@use__speech%button;normal;off_mask.png");
	mode_chooce[mode].change_image("assets//sys_page1@use__speech%button;normal;on.png", "assets//sys_page1@use__speech%button;over;on.png", "assets//sys_page1@use__speech%button;normal;off_mask.png");

	settextstyle(30, 0, "��Բ", 0, 0, 1000, false, false, false);
	center_text(wmode - wbox, (hmode - htext) / 3, xmode, ymode + htext, "��ģʽ");
	center_text(wmode - wbox, (hmode - htext) / 3, xmode, ymode + htext+ (hmode - htext) / 3, "��ͨģʽ");
	center_text(wmode - wbox, (hmode - htext) / 3, xmode, ymode + htext+ 2 * (hmode - htext) / 3, "����ģʽ");
	settextstyle(18, 0, "��Բ", 0, 0, 1000, false, false, false);
	center_text(3*wmode , (hmode - htext) / 3, xmode+ wmode, ymode + htext, "����ת���Ĺ��񽫱���ǲ���ת��ɫ");
	center_text(3*wmode , (hmode - htext) / 3, xmode+ wmode, ymode + htext + (hmode - htext) / 3, "����ת���Ĺ��񽫱���ת��ɫ�����ᱻ���");
	center_text(3*wmode , (hmode - htext) / 3, xmode+ wmode, ymode + htext + 2 * (hmode - htext) / 3, "����ת���Ĺ��񽫲������κα仯������Ҽ���");

	int xvolume = xmode + 4 * wmode, yvolume = ymode, wvolume = 2*wmode, hvolume = htext;
	settextstyle(40, 0, "��Բ", 0, 0, 1000, false, false, false);
	center_text(wvolume, hvolume, xvolume, yvolume, "��������");

	//����������ʾ����ͼ
	IMAGE box,box_mask;
	//����������ʾ�򳤿����
	wbox = 80, hbox = 40;
	//����Ӽ�������
	int block = 40;

	loadimage(&box, "assets//file_qload@edit%sysbutton;normal.png", wbox, hbox);
	loadimage(&box_mask, "assets//file_qload@edit%sysbutton;normal_mask.png", wbox, hbox);

	//����������ʾ��λ�ò���
	int xbox = xvolume + block + (wvolume - 2 * block - wbox) / 2, ybox = yvolume + 3 * hvolume;
	put_png(xbox, ybox, &box, &box_mask);
	settextstyle(20, 0, "��Բ", 0, 0, 1000, false, false, false);
	center_text(wbox, hbox, xbox, ybox, to_string(volume).c_str());

	//����Ӽ���λ�ò���
	int xmi = xvolume, xpl = xvolume + wvolume - block, ymi = yvolume + 3 * hvolume, ypl = yvolume + 3 * hvolume;
	settextstyle(block, 0, "��Բ", 0, 0, 1000, false, false, false);
	center_text(block, block, xmi, ymi, "-");
	center_text(block, block, xpl, ypl, "+");

	//������׽�����Ϣ
	while (1) {
		back.act_over_mask();
		for (int i = 0; i < 3; i++) {
			mode_chooce[i].act_over_mask();
		}
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				back.act_button(display_menu);
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
					settextstyle(20, 0, "��Բ", 0, 0, 1000, false, false, false);
					center_text(wbox, hbox, xbox, ybox, to_string(volume).c_str());
				}
				else if (msg.x > xpl && msg.x<xpl + block && msg.y>ypl && msg.y < ypl + block) {
					if (volume == 100)continue;
					volume += 10;
					waveOutSetVolume(NULL, CalculateVolume(volume, volume));
					put_png(xbox, ybox, &box, &box_mask);
					settextstyle(20, 0, "��Բ", 0, 0, 1000, false, false, false);
					center_text(wbox, hbox, xbox, ybox, to_string(volume).c_str());
				}
			}
		}
	}
}

void save_in(int save_position,IMAGE img, bool* chessboard, int level) {
	std::ostringstream oss;
	oss << "save\\save" << save_position << "\\chessboard_data"; // ƴ�������ַ���
	std::string chessboard_file = oss.str();

	std::ostringstream ossimg;
	ossimg << "save\\save" << save_position << "\\image_data.png"; // ƴ�������ַ���
	std::string image_file = ossimg.str();

	saveimage(_T(image_file.c_str()), &img);

	ofstream chessboard_save_stream(chessboard_file);

	for (int i = 0; i < 61; i++) {
		chessboard_save_stream << chessboard[i];
	}
	for (int i = 0; i < 61; i++) {
		chessboard_save_stream << answer[i];
	}
}

//����saveload��������ͼ����
int wimg = 229*2/3, himg = 128*2/3;

void display_save(bool *chessboard,int level,IMAGE img) {
	grids.clear();//�������й���
	IMAGE save_background,save_caption;
	loadimage(&save_background, "assets\\file_save@base%base.png", w, h );
	int hbar = 76;
	loadimage(&save_caption, "assets\\file_save@caption%layer.png", w, hbar);
	putimage(0, 0, &save_background);
	putimage(0, 0, &save_caption);

	//ճ��������ͼ
	button back(0, h-hback, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");

	vector<button> saves;

	int wsave_img = 412, hsave_img = 132;
	IMAGE save_img;
	for (int i = 0; i < 8; i++) {
		saves.emplace_back((w / 2 - wsave_img)/2 + (w / 2) * (i / 4), hbar + (h / 4 - hsave_img) / 2 + (h - hbar) / 4 * (i % 4), wsave_img, hsave_img, "assets\\file_save@item%button;normal.png", "assets\\file_save@item%button;over.png", "assets\\file@item%button;mask.png");
		std::ostringstream oss;
		oss << "save\\save" << i+1 << "\\image_data.png"; // ƴ�������ַ���
		std::string image_file = oss.str();
		loadimage(&save_img, image_file.c_str(), wimg, himg,0);
		putimage(saves[i].get_x() + 12, saves[i].get_y() + 12, &save_img);
	}

	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			back.act_over_mask();
			for (int i = 0; i < 8; i++) {
				if (saves[i].state_change()) {

					//��ʼ˫�����ͼ
					BeginBatchDraw();

					saves[i].act_over_mask();
					std::ostringstream oss;
					oss << "save\\save" << i + 1 << "\\image_data.png"; // ƴ�������ַ���
					std::string image_file = oss.str();
					loadimage(&save_img, image_file.c_str(), wimg, himg);
					putimage(saves[i].get_x() + 12, saves[i].get_y() + 12, &save_img);

					//������Ͽ�ʼ��ͼ
					EndBatchDraw();
				}
			}
			if (msg.message == WM_LBUTTONDOWN) {
				back.act_button(retry,chessboard);
				for (int i = 0; i < 8; i++) {
					saves[i].act_button(save_in, i+1, img, chessboard ,level);
					std::ostringstream oss;
					oss << "save\\save" << i + 1 << "\\image_data.png"; // ƴ�������ַ���
					std::string image_file = oss.str();
					loadimage(&save_img, image_file.c_str(), wimg, himg);
					putimage(saves[i].get_x() + 12, saves[i].get_y() + 12, &save_img);
				}
			}
		}
	}
}

void load_to (const char* chessboard_file_name) {
	vector<int> numbers; // ʹ�ö�̬����洢����

	// ���ļ�
	ifstream inputFile(chessboard_file_name);
	if (!inputFile.is_open()) {

		//cerr << "�޷����ļ���" << endl;
		loadimage(&pop_up, "assets//pop_up.png", 0, 0);
		loadimage(&pop_up_mask, "assets//pop_up_mask.png", 0, 0);
		int ximg = (w - pop_up.getwidth()) / 2; int yimg = (h - pop_up.getheight()) / 2;
		put_png(ximg, yimg, &pop_up, &pop_up_mask);
		settextstyle(30, 0, "��Բ", 0, 0, 1000, false, false, false);
		settextcolor(BLACK);
		setbkmode(TRANSPARENT);
		center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg, "�޷����ļ�");
		center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg+ pop_up.getheight() / 3-40, "�����������ļ���ַ");
		center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg + pop_up.getheight() / 3, "�ļ���ַ��Ҫ������");
		center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg + pop_up.getheight() / 3+40, "��ȷ���Ƿ���\\\\����ַ�ָ���");
		center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg+2* pop_up.getheight() / 3, "�����������");

		//������׽�����Ϣ
		while (1) {
			if (peekmessage(&msg, EX_KEY)) {
				if (msg.message == WM_KEYDOWN) {
					display_menu();
				}
			}
		}
	}

	char ch;
	// ���ַ���ȡ�ļ�����
	while (inputFile.get(ch)) {
		if (isdigit(ch)) { // ����Ƿ�Ϊ�����ַ�
			numbers.push_back(ch - '0'); // ���ַ�ת��Ϊ���ֲ��洢
		}
	}

	bool chessboard[61];
	for (int i = 0; i < 61; i++) {
		chessboard[i] = numbers[i];
	}
	/////////////////////////////////////////////////for (int i = 61; i < 122; i++) {
	/////////////////////////////////////////////////	answer[i - 61] = numbers[i];
	/////////////////////////////////////////////////}

	// �ر��ļ�
	inputFile.close();

	retry(chessboard);
}

void load_to_input() {
	char str[100];
	InputBox(str, 100, "�����������ļ���ַ"); // �����Ի������û������ַ���

	outtextxy(100, 100, str); // �ڴ�������ʾ�û�������ַ���

	load_to(str);
}

void display_load() {
	grids.clear();//�������й���
	IMAGE save_background, save_caption;
	loadimage(&save_background, "assets\\file_load@base%base.png", w, h);
	int hbar = 76;
	loadimage(&save_caption, "assets\\file_load@caption%layer.png", w, hbar);
	putimage(0, 0, &save_background);
	putimage(0, 0, &save_caption);

	//ճ��������ͼ
	button back(0, h - hback, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");

	vector<button> loads;

	int wsave_img = 412, hsave_img = 132;
	IMAGE save_img;
	for (int i = 0; i < 8; i++) {
		loads.emplace_back((w / 2 - wsave_img) / 2 + (w / 2) * (i / 4), hbar + (h / 4 - hsave_img) / 2 + (h - hbar) / 4 * (i % 4), wsave_img, hsave_img, "assets\\file_save@item%button;normal.png", "assets\\file_save@item%button;over.png", "assets\\file@item%button;mask.png");
		std::ostringstream oss;
		oss << "save\\save" << i + 1 << "\\image_data.png"; // ƴ�������ַ���
		std::string image_file = oss.str();
		loadimage(&save_img, image_file.c_str(), wimg, himg, 0);
		putimage(loads[i].get_x() + 12, loads[i].get_y() + 12, &save_img);
	}

	int wbt = 200, hbt = 50;
	settextstyle(20, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	button user_input(w - wbt - 40, (hbar - hbt) / 2 , wbt, hbt, "assets\\UItemplate3_nor.png", "assets\\UItemplate3_over.png", "assets\\UItemplate3_mask.png", "�����Զ���ؿ�");
    
	//������׽�����Ϣ
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			back.act_over_mask();
			user_input.act_over_mask();
			for (int i = 0; i < 8; i++) {
				if (loads[i].state_change()) {

					//��ʼ˫�����ͼ
					BeginBatchDraw();

					loads[i].act_over_mask();
					std::ostringstream oss;
					oss << "save\\save" << i + 1 << "\\image_data.png"; // ƴ�������ַ���
					std::string image_file = oss.str();
					loadimage(&save_img, image_file.c_str(), wimg, himg);
					putimage(loads[i].get_x() + 12, loads[i].get_y() + 12, &save_img);

					//������Ͽ�ʼ��ͼ
					EndBatchDraw();
				}
			}
			if (msg.message == WM_LBUTTONDOWN) {
				back.act_button(display_menu);
				user_input.act_button(load_to_input);
				for (int i = 0; i < 8; i++) {
					std::ostringstream oss;
					oss << "save\\save" << i + 1 << "\\chessboard_data"; // ƴ�������ַ���
					loads[i].act_button(load_to, oss.str().c_str());
				}
			}
		}
	}
}

void display_chooce_stage() {
	grids.clear();//�������й���
	//���ñ���
	IMAGE chooce_stage_background;
	loadimage(&chooce_stage_background, "assets\\extra@base%base.png", w, h);
	putimage(0, 0, &chooce_stage_background);

	//ճ��������ͼ
	button back(40, 40, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");

	//����ѡ�ذ���
	vector<button> chooce_stage;

	//���ð�����С
	int wbt = 80, hbt = 80;

	for (int j = 1; j < 6; j++) {
		settextstyle(60, 0, "��Բ", 0, 0, 1000, false, false, false);
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
					oss << "level " << i + 1; // ƴ�������ַ���
					chooce_stage[i].act_button(load_to,oss.str().c_str());
				}
			}
		}
	}
}

void display_chooce_mode() {
	grids.clear();//�������й���
	//ճ��������ͼ
	button back(40, 40, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");

	IMAGE chooce_UI,chooce_UI_mask;
	loadimage(&chooce_UI, "assets//chooce_mode.png", 0, h);
	loadimage(&chooce_UI_mask, "assets//chooce_mode_mask.png", 0, h);
	put_png(w - chooce_UI.getwidth(), 0, &chooce_UI, &chooce_UI_mask);

	int wbt = 200, hbt = 50;
	settextstyle(25, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	int deltax = (chooce_UI.getwidth() - wbt) / 2, deltay = (h / 2 - chooce_UI.getheight()) / 2;
	button stage(w - chooce_UI.getwidth() + deltax, h / 2 + deltay, wbt, hbt, "assets\\UItemplate3_nor.png", "assets\\UItemplate3_over.png", "assets\\UItemplate3_mask.png", "����ģʽ");
	button endless(w - chooce_UI.getwidth() + deltax, h + deltay, wbt, hbt, "assets\\UItemplate3_nor.png", "assets\\UItemplate3_over.png", "assets\\UItemplate3_mask.png", "�޾�ģʽ");

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

//��ǰλ��grids[i]ʱ��������ʾ
void hint(int xhint,int yhint ,int whint,int hhint) {
	for (int i = 0; i < 61; i++) {
		if (answer[i]&&grids[i].get_reversed_time()==0) {
			//������ʾ��·��
			grids[i].hint_cover();
		}
		else if (!answer[i] && grids[i].get_reversed_time() == 1) {
			//������ʾ������
			grids[i].hint_wrong_cover();
		}
	}
	put_png(xhint, yhint, &hint_window, &hint_window_mask);
	settextstyle(20, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(BLACK);
	center_text(whint, hhint, xhint, yhint, "��ʾ��Ϣ�Ѹ�����ʾ");
}

const static int whint = 1.2 * 214, hhint = 1.2 * 246, xhint = w - whint, yhint = 240;

void set_chessboard() {
	int deltax = 72, deltay = 38;//�����ν���ƴ����Ҫ�ĺ��������
	int x0 = (w - grid0.getwidth()) / 2, y0 = (h - grid0.getheight()) / 2;//����Ȧ��������ͼ�����Ͻ�����

	//��ʼ˫�����ͼ
	BeginBatchDraw();
	//����������ͼ
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
	settextstyle(20, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	center_text(length, length, w - text_length - length, h - y_to_btm - length, "S");
	center_text(text_length, length, w - text_length, h - y_to_btm - length, "�浵");
	center_text(length, length, w - 2 * (text_length + length), h - y_to_btm - length, "R");
	center_text(text_length, length, w - 2 * text_length - length, h - y_to_btm - length, "����");

	loadimage(&hint_window, "assets\\sys_base@helpbase%layer.png", whint, hhint);
	loadimage(&hint_window_mask, "assets\\sys_base@helpbase%layer_mask.png", whint, hhint);

	put_png(xhint, yhint, &hint_window, &hint_window_mask);
	settextstyle(30, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(BLACK);
	center_text(whint, hhint, xhint, yhint, "��H�����ʾ");
	//������Ͽ�ʼ��ͼ
	EndBatchDraw();

}

void play_interact(button back,bool* chessboard) {
	//������׽�����Ϣ
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
						};//���ʱ��ת�Ĺ���
						judge_msg = i;
						while (1) {
							if (peekmessage(&msg, EX_MOUSE | EX_KEY)) {
								if (msg.message == WM_LBUTTONUP) {
									if (complete_or_not()) {
										you_win(display_play);
									}
									i = 61;//ֹͣ����
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
							else if (msg.message == WM_KEYDOWN) // �ж��Ƿ��ǰ���������Ϣ
							{
								if (msg.vkcode == 0x53) // �ж��Ƿ��� s �� S ��
								{
									IMAGE img;
									int wget_img = 229 * 5.7, hget_img = 128 * 5.7;
									getimage(&img, (w - wget_img) / 2, (h - hget_img) / 2, wget_img, hget_img);
									display_save(chessboard, level, img);
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
			}
			else if (msg.message == WM_KEYDOWN) // �ж��Ƿ��ǰ���������Ϣ
			{
				if (msg.vkcode == 0x53) // �ж��Ƿ��� s �� S ��
				{
					IMAGE img;
					int wget_img = 229 * 5.7, hget_img = 128 * 5.7;
					getimage(&img, (w - wget_img) / 2, (h - hget_img) / 2, wget_img, hget_img);
					display_save(chessboard, level, img);
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

//����������ɹؿ�
void display_play() {
	grids.clear();//�������й���
	//��BGM
	music_change_to(1);
	//������
	cleardevice();
	loadimage(&play_background, "assets//play_background.png", w, h);
	putimage(0, 0, &play_background);

	//ճ��������ͼ
	button back(40, 40, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");

	set_chessboard();

	set_question_auto(rand()%6+36);

	//��¼�������棬�����Թ���ʹ��
	bool chessboard[61];
	for (int i = 0; i <= 60; i++) {
		chessboard[i] = grids[i].get_color();
	}

	play_interact(back,chessboard);
}

//����ĳָ���ؿ�
void retry(bool* chessboard) {
	grids.clear();//�������й���
	//��BGM
	music_change_to(1);
	//������
	cleardevice();
	loadimage(&play_background, "assets//play_background.png", w, h);
	putimage(0, 0, &play_background);

	set_chessboard();

	//ճ��������ͼ
	button back(40, 40, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");

	
	//��������
	for (int i = 0; i < 61; i++) {
		if (chessboard[i]) {
			grids[i].reverse_set_question();
		}
	}

	//��¼�������棬�����Թ���ʹ��
	for (int i = 0; i <= 60; i++) {
		chessboard[i] = grids[i].get_color();
	}

	play_interact(back, chessboard);
}

int main() {
	srand(time(NULL));
	initgraph(w, h);
	//������ͼ
	loadimage(&grid0, "assets//grid0.png", 0, 0);
	loadimage(&grid0_mask, "assets//grid0_mask.png", 0, 0);
	loadimage(&grid1, "assets//grid1.png", 0, 0);

	//���˵�
	display_menu();
}