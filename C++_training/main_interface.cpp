#include<iostream>
#include <ws2tcpip.h>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <windows.h>
#include <stdio.h>
#include <graphics.h>
#include <vector>
#include <set>
#include <time.h>
#include <conio.h>
#include<fstream>
#include <sstream>
#include"Resources.h"
#pragma comment(lib, "winmm.lib")

using namespace std;
int main() {
	srand(time(NULL));
	initgraph(w, h);
	//¼ÓÔØÌùÍ¼
	loadimage(&grid0, "assets//grid0.png", 0, 0);
	loadimage(&grid0_mask, "assets//grid0_mask.png", 0, 0);
	loadimage(&grid1, "assets//grid1.png", 0, 0);

	//Ö÷²Ëµ¥
	display_menu();
}