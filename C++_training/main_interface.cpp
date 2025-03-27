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
#include <stdio.h>
#include <graphics.h>
#include <vector>
#include <time.h>
#include <conio.h>
using namespace std;

void put_png(int x, int y, IMAGE* img) {
	//��ͼ�ް�ɫ
	putimage(x, y, img, SRCAND);
}

void put_png(int x, int y, IMAGE* img,IMAGE* img_mask) {
	//��ͼ�а�ɫ��������ͼ
	putimage(x, y, img_mask, NOTSRCERASE);
	putimage(x, y, img, SRCINVERT);
}

//���û�������
int w = 1360, h = 765;

//��������ͼ����ͼ
IMAGE title;
IMAGE background,rule_background,play_background;
IMAGE back,back_mask;
IMAGE quit, play, load, how_to_play, setting, pic, pic_mask, grid0,grid0_mask, grid1;
IMAGE button_mask;
//IMAGE *back_mask=&back+1;
COLORREF color;

//������Ϣ�ṹ�����
ExMessage msg = { 0 };

//�����˳���,���ؼ���һ�����С
int wquit = 60, hquit = 60, wbutton = 315, hbutton = 70, wback = 90, hback = 60;

//���庯��
void display_menu();
void display_how_to_play();
void display_setting();
void display_load();
void display_play();
void you_win();
void you_lose();

//�����м���������
void center_text(int wbutton, int hbutton, int xtop_left, int ytop_left, const char str[]) {
	int xchar = (wbutton - textwidth(str)) / 2;
	int ychar = (hbutton - textheight(str)) / 2;
	outtextxy(xtop_left + xchar, ytop_left + ychar, str);
}

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
		putimage(x, y, &grid0_mask, NOTSRCERASE);
		putimage(x, y, &grid0, SRCINVERT);//<---���Ż�
	}

	void reverse_color() {
		if (reversed_time == 0) {
			grid_color = !grid_color; 
			if (grid_color == false) {
				putimage(x, y, &grid0_mask, NOTSRCERASE);
				putimage(x, y, &grid0, SRCINVERT);
			}                                                  //<---���Ż�
			else { putimage(x, y, &grid1, SRCAND); }
			reversed_time++;
		}
		else {
			you_lose();
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

IMAGE pop_up, pop_up_mask, pop_up_frame, pop_up_frame_mask;
void you_win() {
	loadimage(&pop_up, "assets//pop_up.png", 0, 0);
	loadimage(&pop_up_mask, "assets//pop_up_mask.png", 0, 0);
	put_png((w - pop_up.getwidth()) / 2, (h - pop_up.getheight()) / 2, &pop_up, &pop_up_mask);
}

void you_lose() {
	grids.clear();//�������й���
	cout << "you lose" << endl;
	display_menu();
}


void display_menu(){
	//���˵�����ͼ
	putimage(0, 0, &background);

	//���岻ͬ�������Ͻ�λ��
	int xquit = 1260, yquit = 665;
	int xplay = 68, yplay = 306;
	int xload = xplay, yload = yplay + (h - yplay) / 4;
	int xhow_to_play = xplay, yhow_to_play = yplay + 2 * (h - yplay) / 4;
	int xsetting = xplay, ysetting = yplay + 3 * (h - yplay) / 4;

	//���ݵ�һ������λ�÷��ñ���
	settextstyle(200, 0, "�����п�", 0, 0, 200, false, false, false);
	loadimage(&title, "assets//title.png", textwidth("���Ǭ��"), textheight("���Ǭ��"));
	putimage((w - textwidth("���Ǭ��")) / 2, (yplay - textheight("���Ǭ��")) / 2, &title, SRCAND);
	put_png(xplay + wbutton + 200, yplay - 80, &pic, &pic_mask);

	//���ð�����ͼ
	putimage(xquit, yquit, &quit);
	put_png( xplay, yplay, &play, &button_mask);
	put_png(xload, yload, &load, &button_mask);
	put_png( xhow_to_play, yhow_to_play, &how_to_play, &button_mask);
	put_png( xsetting, ysetting, &setting, &button_mask);

    settextstyle(50, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	center_text(wbutton, hbutton, xplay, yplay, "����Ϸ");
	settextcolor(BLACK);
	center_text(wbutton, hbutton, xload, yload, "������Ϸ");
	center_text(wbutton, hbutton, xhow_to_play, yhow_to_play, "��Ϸ����");
	center_text(wbutton, hbutton, xsetting, ysetting, "����");

	//���˵�ҳ����Ϣ����
	
	//������׽�����Ϣ
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if ((msg.x > xquit && msg.x < xquit + wquit) && (msg.y > yquit && msg.y < yquit + hquit)) {
					exit(0);//����˳���ť�����������        
				}
				if ((msg.x > xplay && msg.x < xplay + wbutton) && (msg.y > yplay && msg.y < yplay + hbutton)) {
					display_play();//�����ʼ��Ϸ��ť������ת����Ϸ����
				}
				if ((msg.x > xload && msg.x < xload + wbutton) && (msg.y > yload && msg.y < yload + hbutton)) {
					display_load();//���������ť������ת�����������˵�������
				}
				if ((msg.x > xhow_to_play && msg.x < xhow_to_play + wbutton) && (msg.y > yhow_to_play && msg.y < yhow_to_play + hbutton)) {
					display_how_to_play();//�����Ϸ����ť������ת����Ϸ�������
				}
				if ((msg.x > xsetting && msg.x < xsetting + wbutton) && (msg.y > ysetting && msg.y < ysetting + hbutton)) {
					display_setting();//������ð�ť������ת�����ý���
				}
			}
		}
	}
}

void display_how_to_play() {
	//������
	cleardevice();
	loadimage(&rule_background, "assets//rule_background.png", w, h);
	putimage(0, 0, &rule_background);

	//ճ��������ͼ
	put_png(40, 40, &back,&back_mask);

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
	color = RGB(200, 0, 0);
	settextcolor(color);
	settextstyle(50, 0, "�����п�", 0, 0, 100, false, false, false);
	//int wplay_from_rule = textwidth("�����˽� ��ʼ��Ϸ"), hplay_from_rule = textheight("�����˽� ��ʼ��Ϸ");
	center_text(w - 100, h / 4, 100, 3 * h / 4, "��ʼ��Ϸ");

	//������׽�����Ϣ
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if ((msg.x > 40 && msg.x < 40 + wback) && (msg.y > 40 && msg.y < 40 + hback)) {
					display_menu();//������ذ�ť���᷵�����˵�
				}
				if ((msg.x > 100 + (w - textwidth("��ʼ��Ϸ") - 100) / 2 && msg.x < w - (w - 100 - textwidth("��ʼ��Ϸ")) / 2) && (msg.y > 3 * h / 4 + (h / 4 - textheight("��ʼ��Ϸ")) / 2 && msg.y < h - (h / 4 - textheight("��ʼ��Ϸ")) / 2)) {
					display_play();//�����ʼ��Ϸ��ť������ת����Ϸ����
				}
			}
		}
	}
}

void display_setting() {
	//ճ��������ͼ
	put_png(40, 40, &back, &back_mask);

	//������׽�����Ϣ
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if ((msg.x > 40 && msg.x < 40 + wback) && (msg.y > 40 && msg.y < 40 + hback)) {
					display_menu();//������ذ�ť���᷵�����˵�
				}
			}
		}
	}
}

void display_load() {
	//ճ��������ͼ
	put_png(40, 40, &back, &back_mask);

	//������׽�����Ϣ
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if ((msg.x > 40 && msg.x < 40 + wback) && (msg.y > 40 && msg.y < 40 + hback)) {
					display_menu();//������ذ�ť���᷵�����˵�
				}
			}
		}
	}
}


void display_play() {
	//������
	cleardevice();
	loadimage(&play_background, "assets//play_background.png", w, h);
	putimage(0, 0, &play_background);

	//ճ��������ͼ
	put_png(40, 40, &back, &back_mask);

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
	//������Ͽ�ʼ��ͼ
	EndBatchDraw();
	
	set_question_auto(rand()%11+20);

	//������׽�����Ϣ
	int judge_msg;
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN){
				if ((msg.x > 40 && msg.x < 40 + wback) && (msg.y > 40 && msg.y < 40 + hback)) {
					//������ذ�ť���᷵�����˵�
					grids.clear();//�������й���
					display_menu();
				}
				for (int i = 0; i <= 60; i++) {
					if (grids[i].reverse_or_not()) {
						grids[i].reverse_color();//���ʱ��ת�Ĺ���
						judge_msg = i;
						while (1) {
							if (peekmessage(&msg, EX_MOUSE)) {
								if (msg.message == WM_LBUTTONUP) {
									if (complete_or_not()) {
										you_win();
									}
									i = 61;//ʹfor����ֹͣ����
									break;
								}
								else { 
									for (int j = 0; j <= 60; j++) {
										if (grids[j].reverse_or_not() && adjacent_or_not(judge_msg,j)) {
											grids[j].reverse_color();
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
		}
	}
}

int main() {
	srand(time(NULL));
	initgraph(w, h);
	//������ͼ
	loadimage(&background, "assets//background.png", w, h);
	loadimage(&quit, "assets//quit.jpg", wquit, hquit);
	loadimage(&play, "assets//UItemplate2.png", wbutton, hbutton);
	loadimage(&load, "assets//UItemplate1.png", wbutton, hbutton);
	loadimage(&how_to_play, "assets//UItemplate1.png", wbutton, hbutton);
	loadimage(&setting, "assets//UItemplate1.png", wbutton, hbutton);
	loadimage(&back, "assets//back.png", wback, hback);//���뷵����ͼ
	loadimage(&back_mask, "assets//back_mask.png", wback, hback);
	loadimage(&pic, "assets//menu_pic.png", 500, 500);
	loadimage(&pic_mask, "assets//menu_pic_mask.png", 500, 500);
	loadimage(&grid0, "assets//grid0.png", 0, 0);
	loadimage(&grid0_mask, "assets//grid0_mask.png", 0, 0);
	loadimage(&grid1, "assets//grid1.png", 0, 0);
	loadimage(&button_mask, "assets//UItemplate_mask.png", wbutton, hbutton);

	//���˵�
	display_menu();
}
