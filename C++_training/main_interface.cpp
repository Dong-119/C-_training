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



//���û���
int w = 1360, h = 765;

//��������ͼ�ͷ�����ͼ
IMAGE background,rule_background,play_background;
IMAGE back;

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

void menu_background() {
	//������ñ���ͼ
	loadimage(&background, "assets//background.png", w, h);
	putimage(0, 0, &background);
}


//�����м���������
void center_text(int wbutton, int hbutton, int xtop_left, int ytop_left, const char str[]) {
	int xchar = (wbutton - textwidth(str)) / 2;
	int ychar = (hbutton - textheight(str)) / 2;
	outtextxy(xtop_left + xchar, ytop_left + ychar, str);
}

int main() {
	
	initgraph(w, h);
	//������ͼ
	IMAGE quit, play, load, how_to_play, setting, pic,grid0,grid1;
	COLORREF color;

	int wquit = 60, hquit = 60, wbutton = 315, hbutton = 70, wback = 90, hback = 60;;//�����˳���,���ؼ���һ�����С

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


menu:
	//���˵�����ͼ
	menu_background();


	//�������½Ƿ����˳�ͼ�꣬�����ÿ�ʼ��Ϸ��������Ϸ���淨���ܣ�����
	

	//���岻ͬ�������Ͻ�λ��
	int xquit = 1260,yquit = 665;
	int xplay = 68, yplay = 306;
	int xload = xplay, yload = yplay + (h - yplay) / 4;
	int xhow_to_play = xplay, yhow_to_play = yplay + 2*(h - yplay) / 4;
	int xsetting = xplay, ysetting = yplay + 3*(h - yplay) / 4;


	//���ݵ�һ������λ�÷��ñ���
	settextstyle(200, 0, "�����п�", 0, 0, 200, false, false, false);
	IMAGE title;
	loadimage(&title, "assets//title.png", textwidth("���Ǭ��"), textheight("���Ǭ��"));
	put_png((w - textwidth("���Ǭ��")) / 2, (yplay - textheight("���Ǭ��")) / 2, &title);
	put_png(xplay + wbutton+200, yplay-80, &pic);

	//���ð�����ͼ
	putimage(xquit, yquit, &quit);
	button_png(&play,  xplay, yplay, wbutton, hbutton);//putimage(xplay, yplay, &play);
	button_png(&load, xload, yload, wbutton, hbutton);//putimage(xload, yload, &load);
	button_png(&how_to_play, xhow_to_play, yhow_to_play, wbutton, hbutton);// putimage(xhow_to_play, yhow_to_play, &how_to_play);
	button_png(&setting,  xsetting, ysetting, wbutton, hbutton); //putimage(xsetting, ysetting, &setting);

	settextstyle(50, 0, "��Բ", 0, 0, 1000, false, false, false);
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);


	center_text(wbutton, hbutton, xplay, yplay, "����Ϸ");

	settextcolor(BLACK);

	center_text(wbutton, hbutton, xload, yload, "������Ϸ");

	center_text(wbutton, hbutton, xhow_to_play, yhow_to_play, "��Ϸ����");

	center_text(wbutton, hbutton, xsetting, ysetting, "����");

	//���˵�ҳ����Ϣ����
	//������Ϣ�ṹ�����
	ExMessage msg = { 0 };
	//������׽�����Ϣ
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if ((msg.x > xquit && msg.x < xquit+wquit) && (msg.y > yquit && msg.y < yquit+hquit)) {
					return 0;//����˳���ť�����������        
				}
				if ((msg.x > xplay && msg.x < xplay + wbutton) && (msg.y > yplay && msg.y < yplay + hbutton)) {
					goto play;//�����ʼ��Ϸ��ť������ת����Ϸ����
				}
				if ((msg.x > xload && msg.x < xload + wbutton) && (msg.y > yload && msg.y < yload + hbutton)) {
					goto load;//���������ť������ת�����������˵�������
				}
				if ((msg.x > xhow_to_play && msg.x < xhow_to_play + wbutton) && (msg.y > yhow_to_play && msg.y < yhow_to_play + hbutton)) {
					goto how_to_play;//�����Ϸ����ť������ת����Ϸ�������
				}
				if ((msg.x > xsetting && msg.x < xsetting + wbutton) && (msg.y > ysetting && msg.y < ysetting + hbutton)) {
					goto setting;//���x���ð�ť������ת�����ý���
				}
			}
		}
	}


	//��Ϸ�������


how_to_play:
	//������
	cleardevice();
	loadimage(&rule_background, "assets//rule_background.png", w, h);
	putimage(0, 0, &rule_background);

	//ճ��������ͼ
	button_png(&back,  40, 40, wback, hback);

	//д��
	settextstyle(120, 0, "�����п�", 0, 0, 200, false, false, false);
	color = RGB(110, 180, 200);
	settextcolor(color);
	setbkmode(TRANSPARENT);
	center_text(w, h/3, 0, 0, "���Ǭ��  ��Ϸ����");
	settextstyle(29, 0, "�����п�", 0, 0, 0, false, false, false);
	color = RGB(200, 120, 45);
	settextcolor(color);

	center_text(w, h / 12, 0, h/3, "��Ϸ��һ�������������н��У�����������ֲ����̰�������ɫ�Ĺ���");
	center_text(w, h / 12, 0, h / 3 + h / 12, "�����Ҫͨ��������һ�ʴ�����������Щ����");
	center_text(w, h / 12, 0, h / 3 + h / 6, "ÿ�����񱻴�������Ϊ��ɫ, ��ÿ���������ֻ�ܷ�תһ�Ρ�");
	center_text(w, h / 12, 0, h / 3 + h / 4, "��ÿȦ���й�����ɫ��ͬʱ������ɹ���");

	//��ʼ��Ϸ��ť
	color = RGB(200, 0, 0);
	settextcolor(color);
	settextstyle(50, 0, "�����п�", 0, 0, 100, false, false, false);
	//int wplay_from_rule = textwidth("�����˽� ��ʼ��Ϸ"), hplay_from_rule = textheight("�����˽� ��ʼ��Ϸ");
	center_text(w-100, h / 4, 100, 3*h/4, "��ʼ��Ϸ");

	//������׽�����Ϣ
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if ((msg.x > 40 && msg.x < 40+wback) && (msg.y > 40 && msg.y < 40+hback)) {
					goto menu;//������ذ�ť���᷵�����˵�
				}
				if ((msg.x > 100+(w-textwidth("��ʼ��Ϸ")-100)/2 && msg.x < w- (w-100 - textwidth("��ʼ��Ϸ")) / 2) && (msg.y > 3 * h / 4+ (h/4 - textheight("��ʼ��Ϸ")) / 2 && msg.y < h- (h / 4 - textheight("��ʼ��Ϸ")) / 2)) {
					goto play;//������ذ�ť���᷵�����˵�
				}
			}
		}
	}


	//���ý���


setting:
	//ճ��������ͼ
	button_png(&back,  40, 40, wback, hback);

	//������׽�����Ϣ
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if ((msg.x > 40 && msg.x < 40 + wback) && (msg.y > 40 && msg.y < 40 + hback)) {
					goto menu;//������ذ�ť���᷵�����˵�
				}
			}
		}
	}


	//�������棨���˵���


load:
	//ճ��������ͼ
	button_png(&back,  40, 40, wback, hback);

	//������׽�����Ϣ
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if ((msg.x > 40 && msg.x < 40 + wback) && (msg.y > 40 && msg.y < 40 + hback)) {
					goto menu;//������ذ�ť���᷵�����˵�
				}
			}
		}
	}


	//��Ϸ����


play:
	//������
	cleardevice();
	loadimage(&play_background, "assets//play_background.png", w, h);
	putimage(0, 0, &play_background);

	//ճ��������ͼ
	button_png(&back,  40, 40, wback, hback);

	int deltax = 72, deltay = 77;//�����ν���ƴ����Ҫ�ĺ��������
	int x0 = (w - grid0.getwidth()) / 2, y0 = (h - grid0.getheight()) / 2;//����Ȧ��������ͼ�����Ͻ�����

	//����������ͼ
	for (int i = -4; i <= 4; i++) {
		for (int j = 1; j <= (i<0 ? (9+i):(9-i)) ; j++) {
			put_png(x0+i*deltax, y0-(i < 0 ? (9 + i) : (9 - i))*deltay/2+j*deltay-deltay/2, &grid0);
		}
	}
	

	//������׽�����Ϣ
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if ((msg.x > 40 && msg.x < 40 + wback) && (msg.y > 40 && msg.y < 40 + hback)) {
					goto menu;//������ذ�ť���᷵�����˵�
				}
			}
		}
	}
}
