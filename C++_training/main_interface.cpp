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
#include <conio.h>
#pragma comment(lib,"MSIMG32.LIB")
using namespace std;

inline void put_png(int x, int y, IMAGE* img)
{//����͸��������png
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

//���û�������
int w = 1360, h = 765;

//��������ͼ����ͼ
IMAGE title;
IMAGE background,rule_background,play_background;
IMAGE back;
IMAGE quit, play, load, how_to_play, setting, pic, grid0, grid1;
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
void display_you_lose();

void button_png(IMAGE* png,int x,int y,int width,int height) {
	//��pngͼ�Կ�weight��height����backgroundΪ������λ���ϣ��۵����һ��������ɫ��ͬ����
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
		put_png(x, y, &grid0);                                                                                   //<---���Ż�
	}

	void reverse_color() {
		if (reversed_time == 0) {
			grid_color = !grid_color; 
			if (grid_color == false) { put_png(x, y, &grid0); }                                                  //<---���Ż�
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
	//���˵�����ͼ
	putimage(0, 0, &background);

	//���岻ͬ�������Ͻ�λ��
	int xquit = 1260, yquit = 665;
	int xplay = 68, yplay = 306;
	int xload = xplay, yload = yplay + (h - yplay) / 4;
	int xhow_to_play = xplay, yhow_to_play = yplay + 2 * (h - yplay) / 4;
	int xsetting = xplay, ysetting = yplay + 3 * (h - yplay) / 4;

	//��ʼ˫�����ͼ
	BeginBatchDraw();
	//���ݵ�һ������λ�÷��ñ���
	settextstyle(200, 0, "�����п�", 0, 0, 200, false, false, false);
	loadimage(&title, "assets//title.png", textwidth("���Ǭ��"), textheight("���Ǭ��"));
	//put_png((w - textwidth("���Ǭ��")) / 2, (yplay - textheight("���Ǭ��")) / 2, &title);
	putimage((w - textwidth("���Ǭ��")) / 2, (yplay - textheight("���Ǭ��")) / 2, &title, SRCAND);
	put_png(xplay + wbutton + 200, yplay - 80, &pic);

	//���ð�����ͼ
	putimage(xquit, yquit, &quit);
	put_png( xplay, yplay, &play);//putimage(xplay, yplay, &play);
	put_png(xload, yload, &load);//putimage(xload, yload, &load);
	put_png( xhow_to_play, yhow_to_play, &how_to_play);// putimage(xhow_to_play, yhow_to_play, &how_to_play);
	put_png( xsetting, ysetting, &setting); //putimage(xsetting, ysetting, &setting);
	//������Ͽ�ʼ��ͼ
	EndBatchDraw();

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
	button_png(&back, 40, 40, wback, hback);

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
	button_png(&back, 40, 40, wback, hback);

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
	button_png(&back, 40, 40, wback, hback);

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

void display_you_lose() {
	cout << "you lose" << endl;
	display_menu();
}



void display_play() {
	//������
	cleardevice();
	loadimage(&play_background, "assets//play_background.png", w, h);
	putimage(0, 0, &play_background);

	//ճ��������ͼ
	button_png(&back, 40, 40, wback, hback);

	int deltax = 72, deltay = 38;//�����ν���ƴ����Ҫ�ĺ��������
	int x0 = (w - grid0.getwidth()) / 2, y0 = (h - grid0.getheight()) / 2;//����Ȧ��������ͼ�����Ͻ�����

	//��ʼ˫�����ͼ
	BeginBatchDraw();
	//����������ͼ
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
	//������Ͽ�ʼ��ͼ
	EndBatchDraw();
	
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
									i = 61;//ʹfor����ֹͣ����
									break;
								}
								else { 
									for (int j = 0; j <= 60; j++) {
										if (judge_msg != j && grids[j].reverse_or_not()) {
											grids[j].reverse_color();//����ʱ��ת�Ĺ���

											//��������ڽ�����Ϊ����ֱ���и�

                                            if(grids[judge_msg].get_circle() == 0) {
											    //���ǰһ�����������Ĺ���
												if (grids[j].get_circle() != 1) {
													cout << 1 << endl;
													display_you_lose(); 
												}
										    }
										    else if (grids[judge_msg].get_position() % grids[judge_msg].get_circle() == 0) {
										        //���ǰһ���ǽ��ϵĹ���
										        
										        if (grids[judge_msg].get_circle() == grids[j].get_circle() - 1) {
										        	//�������Ȧ
													if (grids[j].get_position() != (grids[j].get_circle() * grids[judge_msg].get_position() / grids[judge_msg].get_circle() + 1) % (grids[j].get_circle() * 6) && grids[j].get_position() != (grids[j].get_circle() * grids[judge_msg].get_position() / grids[judge_msg].get_circle()) % (grids[j].get_circle() * 6) && grids[j].get_position() != (grids[j].get_circle() * grids[judge_msg].get_position() / grids[judge_msg].get_circle() - 1 + grids[j].get_circle() * 6) % (grids[j].get_circle() * 6)) {
														cout << 2 << endl;
														display_you_lose();
													}
										        }
										        else if (grids[judge_msg].get_circle() == grids[j].get_circle()) {
										        	//�����ͬһȦ
													if (grids[j].get_position() != (grids[judge_msg].get_position() + 1) % (grids[j].get_circle() * 6)  && grids[j].get_position() != (grids[judge_msg].get_position() - 1 + grids[j].get_circle() * 6) % (grids[j].get_circle() * 6)){
														cout << 3<<endl;
														 display_you_lose();
													}
										        }
										        else if (grids[judge_msg].get_circle() == grids[j].get_circle()+1) {
										        	//�������Ȧ
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
												//���ǰһ���Ǳ��ϵĹ���

												if (grids[judge_msg].get_circle() == grids[j].get_circle() - 1) {
													//�������Ȧ
													if (grids[j].get_position() != (grids[judge_msg].get_position() / grids[judge_msg].get_circle() * grids[j].get_circle() + grids[judge_msg].get_position() % grids[judge_msg].get_circle()) % (grids[j].get_circle() * 6) && grids[j].get_position() != (grids[judge_msg].get_position() / grids[judge_msg].get_circle() * grids[j].get_circle() + grids[judge_msg].get_position() % grids[judge_msg].get_circle()+1) % (grids[j].get_circle() * 6)) {
														cout << 6 << endl;
														display_you_lose();
													}
												}
												else if (grids[judge_msg].get_circle() == grids[j].get_circle()) {
													//�����ͬһȦ
													if (grids[j].get_position() != (grids[judge_msg].get_position() + 1)%(grids[j].get_circle()*6) && grids[j].get_position() != (grids[judge_msg].get_position() - 1 + grids[j].get_circle() * 6) % (grids[j].get_circle() * 6)){
														cout << 7 << endl;
														display_you_lose();
													}
												}
												else if (grids[judge_msg].get_circle() == grids[j].get_circle() + 1) {
													//�������Ȧ
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
	//������ͼ
	loadimage(&background, "assets//background.png", w, h);
	loadimage(&quit, "assets//quit.jpg", wquit, hquit);
	loadimage(&play, "assets//UItemplate2.png", wbutton, hbutton);
	loadimage(&load, "assets//UItemplate1.png", wbutton, hbutton);
	loadimage(&how_to_play, "assets//UItemplate1.png", wbutton, hbutton);
	loadimage(&setting, "assets//UItemplate1.png", wbutton, hbutton);
	loadimage(&back, "assets//back.png", wback, hback);//���뷵����ͼ
	loadimage(&pic, "assets//menu_pic.png", 500, 500);
	loadimage(&grid0, "assets//grid0.png", 0, 0);
	loadimage(&grid1, "assets//grid1.png", 0, 0);

	//���˵�
	display_menu();
}
