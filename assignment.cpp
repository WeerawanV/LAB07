#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>
#include<thread>

void gotoxy(int x, int y) {
	COORD c = { x,y };
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void setcursor(bool visible) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}

void setcolor(int fg, int bg) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}

char cursor(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))
		return'\0';
	else
		return buf[0];
}

void draw_ship(int x, int y) {
	gotoxy(x, y);
	printf(" <-0-> ");
}

void draw_bullet(int x, int y) {
	gotoxy(x, y);
	printf("^");
}

void draw_star(int x, int y) {
	gotoxy(x, y);
	printf("*");
}

void set_star(int x,int y){
	srand(time(NULL));
	x = 10 + rand() % 61;
	y = 2 + rand() % 4;
	draw_star(x, y);
}

void erase_ship(int x, int y) {
	gotoxy(x, y);
	printf("       ");
}

void erase_bullet(int x, int y) {
	gotoxy(x, y);
	printf(" ");
}

int main() {
	int x = 38, y = 20;
	int s, sx, sy;
	char ch = ' ';
	int bx[5], by[5], i;
	int bulletStatus[5] = { 0,0,0,0,0 };
	int direction = 0;
	int status = 0; // 1 = on , 0 = off
	int score = 0;
	gotoxy(85, 0);
	printf("Score : %d" ,score);
	srand(time(NULL));
	for (s = 0; s <= 20; s++) {
		sx = 10 + rand() % 61;
		sy = 2 + rand() % 4;
		draw_star(sx, sy);
	}
	setcursor(0);
	draw_ship(x, y);
	do {
		if (_kbhit()) {
			ch = _getch();
			if (ch == 'a') {
				direction = 1; //left
			}
			if (ch == 'd') {
				direction = 2; //right
			}
			if (ch == 's') {
				direction = 0; //stop
			}
			if (ch == ' ' && bulletStatus[status] == 0) {
				bulletStatus[status] = 1;
				bx[status] = x + 3;
				by[status] = y - 1;
				status++;
				status %= 5;
			}
			fflush(stdin);
		}
		Sleep(100);
		if (direction == 1 && x >= 1) {
			erase_ship(x, y);
			draw_ship(--x, y);
			Sleep(250);
		}
		if (direction == 2 && x <= 80) {
			erase_ship(x, y);
			draw_ship(++x, y);
			Sleep(250);
		}
		for (i = 0; i < 5; i++) { 
			if (bulletStatus[i] == 1) {
				erase_bullet(bx[i], by[i]);
				if (by[i] < 3) {
					bulletStatus[i] = 0; 
				}
				else {
					if (cursor(bx[i], by[i]-1) == '*') {
						gotoxy(bx[i], by[i]-1);
						printf(" ");
						erase_bullet(bx[i], by[i]);
						bulletStatus[i] = 0;
						score++;
						gotoxy(85, 0);
						printf("Score : %d", score);
						std::thread q(Beep,700, 100);
						q.detach();
						set_star(x, y);
					}
					else {
						draw_bullet(bx[i], --by[i]);
						std::thread q(Beep,300, 100);
						q.detach();
					}
				} 
			}
		}
	} while (ch != 'x');
	return 0;
}
