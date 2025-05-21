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
#include <set>
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
//����ģʽ״̬:��0����ͨ1������2
int mode = 1;
//��������ģʽ���޾�0������1��������Զ���2
int play_mode = 0;
//���嵱ǰBGM״̬
int music = -1;
//��������
int volume=20;
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

//������������
DWORD CalculateVolume(int leftPercent, int rightPercent) {
	return (DWORD)((leftPercent * 0xFFFF / 100) | ((rightPercent * 0xFFFF / 100) << 16));
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
void display_set_question();

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

	void act_button(void retry(bool *chessboard),bool * chessboard) {
		//��ť��ת����
		if (msg.x > x && msg.x<x + w && msg.y>y && msg.y < y + h) {
			retry(chessboard);//�����ť������ת��ָ������
		}
	}

	void act_button(void save_in(int save_position,IMAGE img, bool* chessboard), int save_position, IMAGE img, bool* chessboard) {
		//��ť��ת����
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

//���j��judge_msg���ڷ���ture,Ϊ��ͬ������ڸ񷵻�false
bool adjacent_or_not(int judge_msg,int j) {
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

//���ĳ��ͬɫ�Ƿ�����һƬ�����򷵻�true�����򷵻�false
bool one_piece_or_not(vector<int> color) {
	// ��ɫ��������ȡ��һ����Ϊ��ǰ���򣬱�����������Ѱ������ͬɫ����
	vector<int>now; vector<int>next;
	now.push_back(color.at(0));
	color.erase(color.begin());
	while (1) {
		next.clear();
		// ��������ԭ����ɫ��������ɾ����ǰ�񲢼�¼������ͬɫ�����ŵ���ǰ�����飬�ٱ�����ǰ����ÿ��Ԫ���ҳ��뵱ǰ�������ڵĸ�Ϊ�µĵ�ǰ����
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
		// �������������ɫ��Ϊ��ǰ����˵������һƬ
		if (now == color)return true;
		else if (next.empty())return false;
	}
}

//��ͷ��⣬���ر���ɫ����ͷ��
int ends_num(vector<int> color) {
	//��¼��ǰ������ͬɫ��������ʼ��Ϊ��
	int counter = 0;
	//��¼��ͷ������ʼ��Ϊ��
	int ends = 0;
	for (const auto& this_grid : color) {
		//��������λ
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

//��������Ϊȫ��
void reset_grids() {
	for (int i = 0; i < 61; i++) {
		if (grids[i].get_color()) {
			grids[i].reverse_set_question();
		}
		grids[i].set_reversed_time(0);
	}
}

void set_question_auto(int num) {
	//��������
	reset_grids();
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
	// ���ÿȦ��ɫ����ͬ����������ÿȦ��ɫ
	if (check[0] == check[1] &&check[1] == check[2] &&check[2] == check[3] &&check[3] ==check[4] ) {
		goto change_circle_color;
	}
	
	//��ת��ɫ����ɳ���
	for (int k = 0; k < num; k++) {
		grids[reverse[k]].reverse_set_question();
	}
	// ���ͬɫ����һ����ɣ����ڼ����³��⣬���巽ʽ���£�
	// ����������ɫ���ɫ�����ŷֱ𴢴�������������
	vector<int>green;
	vector<int>white;
	//���Ĺ���ɾ����ɲ��������ڿ��Ƿ�Χ��
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
	// ��ͬɫ����һƬ�����о�ͷͳ��
	// ����ͷ��ָ��Χֻ��һ��ͬɫ����Ĺ�����������ͼһ������������ɫ�Ĺ��񣬾߱��������Ĺ���ֻ��������ͷ���β
	// ����ͷ��<=2����Ŀ���ڼ����³��⣬�����������³���

	if (one_piece_or_not(green)) {
		if (ends_num(green) <= 2) {
			set_question_auto(num);
			EndBatchDraw();
			return;
		}
	}
	// ��ɫͬ��
	if (one_piece_or_not(white)) {
		if (ends_num(white) <= 2) {
			set_question_auto(num);
			EndBatchDraw();
			return;
		}
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

void you_win_last_stage() {
	grids.clear();//�������й���
	level++;
	loadimage(&pop_up, "assets//pop_up.png", 0, 0);
	loadimage(&pop_up_mask, "assets//pop_up_mask.png", 0, 0);
	int ximg = (w - pop_up.getwidth()) / 2; int yimg = (h - pop_up.getheight()) / 2;
	put_png(ximg, yimg, &pop_up, &pop_up_mask);
	settextstyle(50, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg, "GAME CLEAR");
	int wbt = 200; int hbt = 50;
	settextstyle(20, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	button back(ximg + pop_up.getwidth() / 3 + (2 * pop_up.getwidth() / 3 - wbt) / 2, yimg + 2 * pop_up.getheight() / 3 + (pop_up.getheight() / 3 - hbt) / 2, wbt, hbt, "assets\\UItemplate3_nor.png", "assets\\UItemplate3_over.png", "assets\\UItemplate3_mask.png", "���˵�");
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
	grids.clear();//�������й���
	loadimage(&pop_up, "assets//pop_up.png", 0, 0);
	loadimage(&pop_up_mask, "assets//pop_up_mask.png", 0, 0);
	int ximg = (w - pop_up.getwidth()) / 2; int yimg = (h - pop_up.getheight()) / 2;
	put_png(ximg, yimg, &pop_up, &pop_up_mask);
	settextstyle(50, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg, "YOU LOSE");
	if (level != 0) {
		center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg + 120, "��ǰ�ؿ�");
		std::ostringstream oss;
		oss << "level " << level; // ƴ���ַ���
		std::string levelnum = oss.str();
		center_text(2 * pop_up.getwidth() / 3, pop_up.getheight() / 3, ximg + pop_up.getwidth() / 3, yimg + 210, levelnum.c_str());
	}
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

//�������������������ĸĶ�
void exit() {
	exit(0);
}

void display_menu(){
	//���ùؿ���
	level = 1;
	//�������й���
	grids.clear();
	//��������
	waveOutSetVolume(NULL, CalculateVolume(volume, volume));
	//��BGM
	music_change_to(0);
	//������ͼ
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

	//�Զ��尴ť
	int wsq=241, hsq = 55;
	settextstyle(25, 0, "��Բ", 0, 0, 1000, false, false, false);
	button set_question(w-wsq-180, h-hsq-180, wsq, hsq, "assets//UItemplate3_nor.png", "assets//UItemplate3_over.png", "assets//UItemplate3_mask.png","�����Զ���ؿ�");

	//������׽�����Ϣ
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

void save_in(int save_position,IMAGE img, bool* chessboard) {
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
	chessboard_save_stream << level;
}

//����saveload��������ͼ����
int wimg = 229*2/3, himg = 128*2/3;

void display_save(bool *chessboard,IMAGE img) {
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
					saves[i].act_button(save_in, i+1, img, chessboard);
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
	for (int i = 61; i < 122; i++) {
		answer[i - 61] = numbers[i];
	}

	// �ر��ļ�
	inputFile.close();

	retry(chessboard);
}

void load_to_input() {
	// ��������ؿ����Ա�����
	level = 0;

	//��¼��ǰĿ¼����ֹ·�������µ���Դ�޷���������
	char currentDir[256];
	GetCurrentDirectory(256, currentDir);

	// �����ļ�ѡ��Ի���Ĳ���
	OPENFILENAME ofn;
	char str[100] = ""; // �洢�ļ�·�����ַ���

	// ��ʼ���ļ�ѡ��Ի���ṹ
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL; // ����и����ڣ���������Ϊ�����ڵľ��
	ofn.lpstrFile = str;
	ofn.nMaxFile = sizeof(str);
	ofn.lpstrFilter = "All Files\0*.*\0"; // �ļ�������
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL; // ��ʼĿ¼
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // ����ѡ����ڵ��ļ�

	// �����ļ�ѡ��Ի���
	if (GetOpenFileName(&ofn)) {
		// ����û��ɹ�ѡ�����ļ���str �н������ļ�·��
		SetCurrentDirectory(currentDir);// ���õ�ǰĿ¼
		std::ifstream file(str);
		if (!file.is_open()) {
			MessageBox(NULL, "δѡ���ļ���ѡ���ļ�ʧ��", "����", MB_OK | MB_ICONERROR);
			return;
		}

		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// ������ݳ���
		if (content.length() != 122) {
			MessageBox(NULL, "�ļ����ʹ���", "����", MB_OK | MB_ICONERROR);
			return;
		}

		// ���ÿ���ַ��Ƿ�Ϊ0��1
		for (char c : content) {
			if (c != '0' && c != '1') {
				MessageBox(NULL, "�ļ����ʹ���", "����", MB_OK | MB_ICONERROR);
				return;
			}
		}

		load_to(str); // ���� load_to ���������ļ�
	}
	else {
		// ����û�ȡ����ѡ�񣬻��߷�������
		MessageBox(NULL, "δѡ���ļ���ѡ���ļ�ʧ��", "����", MB_OK | MB_ICONERROR);
	}
}

void display_load() {
	// ��������ؿ����Ա�����
	level = 0;
	//��������ģʽΪ������Զ���
	play_mode = 2;
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
	//��������ģʽΪ����
	play_mode = 1;
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
					level = i + 1;
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

//�������淴���������������ģʽ���չؿ����Զ���ؿ�����void next_level()��
void last_level() {};

void play_interact(button back,bool* chessboard, void next_level()) {
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
									display_save(chessboard, img);
								}
								else if (msg.vkcode == 0x48) {// �ж��Ƿ��� h �� H ��
									hint(xhint, yhint, whint, hhint);
								}
								else if (msg.vkcode == 0x52) {// �ж��Ƿ��� r �� R ��
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

//����������ɹؿ�
void display_play() {
	grids.clear();//�������й���
	//��������ģʽ
	play_mode = 0;
	//��BGM
	music_change_to(1);
	//������
	cleardevice();
	loadimage(&play_background, "assets//play_background.png", w, h);
	putimage(0, 0, &play_background);

	//ճ��������ͼ
	button back(40, 40, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");
	settextstyle(60, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	std::ostringstream oss;
	oss << "level " << level; // ƴ���ַ���
	std::string levelnum = oss.str();
	center_text(180, 80, (w - 180) / 4, 60, levelnum.c_str());

	set_chessboard();

	set_question_auto(rand()%6+36);

	//��¼�������棬�����Թ���ʹ��
	bool chessboard[61];
	for (int i = 0; i <= 60; i++) {
		chessboard[i] = grids[i].get_color();
	}

	play_interact(back, chessboard, display_play);
}

void next_specific_level() {
	std::ostringstream oss;
	oss << "level " << level; // ƴ���ַ���
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
										you_win_last_stage();
									}
									else {
										you_lose(chessboard);
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
									display_save(chessboard, img);
								}
								else if (msg.vkcode == 0x48) {// �ж��Ƿ��� h �� H ��
									hint(xhint, yhint, whint, hhint);
								}
								else if (msg.vkcode == 0x52) {// �ж��Ƿ��� r �� R ��
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
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	if (level != 0) {
		settextstyle(60, 0, "��Բ", 0, 0, 1000, false, false, false);
		std::ostringstream oss;
		oss << "level " << level; // ƴ���ַ���
		std::string levelnum = oss.str();
		center_text(180, 80, (w - 180) / 4, 60, levelnum.c_str());
	}
	else {
		settextstyle(32, 0, "��Բ", 0, 0, 1000, false, false, false);
		center_text(180, 80, (w - 180) / 4, 60, "������Զ���ؿ�");
	}
	
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
	//��¼��ǰĿ¼����ֹ·�������µ���Դ�޷���������
	char currentDir[256];
	GetCurrentDirectory(256, currentDir);

	OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFile[260];       // buffer for file name

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn)); // ��ofn�ṹ������г�Ա��ʼ��Ϊ0
	ofn.lStructSize = sizeof(ofn); // ���ýṹ��Ĵ�С
	ofn.hwndOwner = NULL; // ���öԻ���ĸ����ھ����NULL��ʾû�и�����
	ofn.lpstrFile = szFile; // �����ļ����ַ����Ļ�����
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0'; // ���ļ����������ĵ�һ���ַ�����Ϊ'\0'����ʾ���ַ���
	ofn.nMaxFile = sizeof(szFile); // �����ļ�������������󳤶�
	ofn.lpstrFilter = TEXT("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0"); // �����ļ����͹�����
	ofn.nFilterIndex = 1; // ����Ĭ�ϵ��ļ����͹�����������1��ʾ�ڶ�����������All Files (*.*)��
	ofn.lpstrFileTitle = NULL; // ��ʹ���ļ�����
	ofn.nMaxFileTitle = 0; // �ļ��������󳤶�
	ofn.lpstrInitialDir = NULL; // �����ó�ʼĿ¼
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // ���ñ�־��Ҫ��·�����ļ��������

	// Display the Save As dialog box.
	if (GetSaveFileName(&ofn) == TRUE) { // ���������ļ��Ի���
		std::ofstream file(ofn.lpstrFile, std::ios::out); // ���������ģʽ���ļ�
		if (!file.is_open()) { // ����ļ���ʧ��
			MessageBox(NULL, "�ļ���ʧ��", "����", MB_OK | MB_ICONERROR); // ���������Ϣ
		}

		for (int i = 0; i < 61; i++) {
			file << grids[i].get_color();
		}
		for (int i = 0; i < 61; i++) {
			file << grids[i].get_reversed_time();
		}
		file.close(); // �ر��ļ�

		MessageBox(NULL, "�ļ�����ɹ�", "����ɹ�", MB_OK | MB_ICONERROR); // ����ɹ������ļ�����Ϣ
	}
	else {
		MessageBox(NULL, "ȡ������", "����ʧ��", MB_OK | MB_ICONERROR); // ����û�ȡ���˲����������ʾ��Ϣ
	}
	SetCurrentDirectory(currentDir);// ���õ�ǰĿ¼
}

void display_set_question() {
	grids.clear();//�������й���
	//��BGM
	music_change_to(1);
	//������
	cleardevice();
	loadimage(&play_background, "assets//play_background.png", w, h);
	putimage(0, 0, &play_background);

	//ճ��������ͼ
	button back(40, 40, wback, hback, "assets//back.png", "assets//back_over.png", "assets//back_mask.png");
	settextstyle(60, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	std::ostringstream oss;
	oss << "level " << level; // ƴ���ַ���
	std::string levelnum = oss.str();
	center_text(180, 80, (w - 180) / 4, 60, levelnum.c_str());

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
	put_png(w - 2 * (text_length + length), h - y_to_btm - length, &save_image, &save_image_mask);
	settextstyle(20, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	center_text(length, length, w - 2 * (text_length + length), h - y_to_btm - length, "R");
	center_text(text_length, length, w - 2 * text_length - length, h - y_to_btm - length, "����");

	loadimage(&hint_window, "assets\\sys_base@helpbase%layer.png", whint, hhint);
	loadimage(&hint_window_mask, "assets\\sys_base@helpbase%layer_mask.png", whint, hhint);

	put_png(xhint, yhint, &hint_window, &hint_window_mask);
	settextstyle(16, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(BLACK);
	center_text(whint, hhint, xhint, yhint-30, "����һ�ʻ������");
	center_text(whint, hhint, xhint, yhint+30, "������ÿȦ��ɫ�Ƿ�ת");
	settextstyle(24, 0, "��Բ", 0, 0, 1000, false, false, false);
	center_text(50, 50, 100, 120, "����Է�ת��Ȧ��ɫ");

	settextstyle(28, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	button OK(w - 240, h - 140, 160, 50, "assets//UItemplate3_nor.png", "assets//UItemplate3_over.png", "assets//UItemplate3_mask.png","���");
	vector<button> circle_color_change;
	for (int i = 0; i < 5; i++) {
		circle_color_change.emplace_back(100, 200 + 80 * i, 50, 50, "assets//stage.png", "assets//stage_over.png", "assets//stage_mask.png", to_string(i+1).c_str());
	}
	//������Ͽ�ʼ��ͼ
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
							MessageBox(NULL, "�����ظ�����ͬһ����", "������", MB_OK | MB_ICONERROR);
							reset_grids();
							goto restart;
						};//���ʱ��ת�Ĺ���
						judge_msg = i;
						while (1) {
							if (peekmessage(&msg, EX_MOUSE | EX_KEY)) {
								if (msg.message == WM_LBUTTONUP) {
									if (complete_or_not()) {
										MessageBox(NULL, "�����þ��п����ԵĹؿ�", "������", MB_OK | MB_ICONERROR);
										reset_grids();
										goto restart;
									}
									i = 61;//ֹͣ����
									break;
								}
								else {
									for (int j = 0; j <= 60; j++) {
										if (grids[j].reverse_or_not() && adjacent_or_not(judge_msg, j)) {
											if (!grids[j].reverse_color(1)) {
												MessageBox(NULL, "�����ظ�����ͬһ����", "������", MB_OK | MB_ICONERROR);
												reset_grids();
												goto restart;
											};
											judge_msg = j;
											break;
										}
									}
								}
							}
							else if (msg.message == WM_KEYDOWN) // �ж��Ƿ��ǰ���������Ϣ
							{
								if (msg.vkcode == 0x52) {// �ж��Ƿ��� r �� R ��
									reset_grids();
								}
							}
						}
					}
				}
			}
			else if (msg.message == WM_KEYDOWN) // �ж��Ƿ��ǰ���������Ϣ
			{
				if (msg.vkcode == 0x52) {// �ж��Ƿ��� r �� R ��
					reset_grids();
				}
			}
		}
	}
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