#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;
// 게임 환경 변수
#define MAX_DDONG       100 // 똥의 최대 개수.

#define LEVEL_COUNT     3   // 레벨마다 늘어날 똥개수
#define LEVEL_SPEED     15  // 레벨마다 빨라질 스피드 (1000 = 1초, 15 = 0.015초)
// 똥 구조체.

random_device rnd;
mt19937_64 dre(rnd());
uniform_int_distribution<> di(1, 24);

int check = 1;
int maxNum = 0; //최대 똥갯수
int ddongnum = 0; //피한 똥 갯수
string strFile;
char choose;
struct User{
	int x, y;
};

class Keyboard {
private:
	int x;
	double time;
public:

	Keyboard(int x, double time):x(x),time(time){}

	int getX() {
		return x;
	}

	double getTime() {
		return time;
	}

};
// 화면의 특정 위치로 이동해 주는 함수.

struct User user;

void gotoxy(int x, int y)
{
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// 키입력 함수
class Obstacle {
private:
	int x;
	int y;
	double  time;
public:
	Obstacle(int x, double time) :x(x),time(time) {
		y = 3;
	}

	void save() {
		ofstream out(strFile + ".txt", ios::app);
		out << x << " ";
		out << time << endl;
		out.close();
	}

	 void update()  {
		 y++;
		 // 똥이 최하단에 도착 했을 때 처리.

		 // 똥 출력
		 gotoxy(x, y);
		 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf5);
		 cout << "＠";
		 // 충돌 체크
		 if (abs(x - user.x) < 2 && y == user.y)
		 {
			 // 비프음 출력..
			 printf("\a");
			 check = 0;
		 }
		 
	};

	 bool eraseDDong() {
		 if (y >= 21)
			 return true;
		 else
			 return false;
	 }

	 int getX() const{
		 return x;
	 }

	 double getTime() const {
		 return time;
	 }

	 int setX(int n) {
		 x = n;
		 return x;
	 }

	 double setTime(double n) {
		 time = n;
		 return time;
	 }
	 ~Obstacle() {};
};

BOOL IsKeyDown(int Key)
{
	return ((GetAsyncKeyState(Key) & 0x8000) != 0);
}

// 게임 함수.
vector<Obstacle> ddong;
vector<Keyboard> keyboard;
int game(int level)
{
	system("Color F0");
	
	int i, key;
	int speed = 75 - level * LEVEL_SPEED;
	int count = 5 + level * LEVEL_COUNT;
	// 레벨업에 필요한 똥개수
	int levelup = 20 + level * 20;
	// 똥개수 체크. (최대값을 넘지 않도록 조정)
	if (count >= MAX_DDONG) count = MAX_DDONG - 1;
	// 스피드 체크. (최소값이 10 보다 작지 않도록 저정)
	if (speed < 10) speed = 10;
	// 유저 시작 위치.
	user.x = 12;
	user.y = 19;

	chrono::system_clock::time_point start;

	chrono::duration<double> toutosec;

	start = chrono::system_clock::now();

	chrono::system_clock::time_point shoot; //똥 1초에 하나씩 만들어지게

	chrono::duration<double> shootsec;

	shoot = chrono::system_clock::now();
	

	// 게임 루프
	while (check)
	{
		Sleep(50);
		toutosec = chrono::system_clock::now() - start;
		shootsec = chrono::system_clock::now() - shoot;
		if (maxNum < MAX_DDONG && toutosec.count() > 2 && shootsec.count() >= 0.5)//똥 100개만 만들기
		{
			shoot = chrono::system_clock::now();
			maxNum++; 
			ddong.push_back(Obstacle(di(dre),toutosec.count()));
			auto iter = ddong.end()-1;
			(*iter).save();
		}

		// 화면 지우기.
		system("cls");
		// 게임 환경 출력.
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
		cout << "[똥피하기게임]" << "		" << level << endl << "점수 :" << ddongnum;
		gotoxy(0, user.y + 3);	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x00);
		cout << "ㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁ" << "ㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁ";
		toutosec = chrono::system_clock::now() - start;
		// 사람 움직임 처리
		if (IsKeyDown(VK_LEFT))
		{
			if (user.x > 1)
			{
				user.x--;	
				keyboard.push_back(Keyboard(user.x,toutosec.count()));
			}
		}
		if (IsKeyDown(VK_RIGHT))
		{
			if (user.x <= 23)
			{
				user.x++;	
				keyboard.push_back(Keyboard(user.x, toutosec.count()));
			}
		}
		// 사람 출력
		gotoxy(user.x, user.y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
		cout << "O ";
		gotoxy(user.x, user.y + 1);
		cout << "ㅅ";
		gotoxy(user.x, user.y + 2);
		cout << "ㅅ";
		// 똥 움직임 처리
		for (auto iter = ddong.begin(); iter != ddong.end();)
		{
			(*iter).update();
			if ((*iter).eraseDDong())
			{
				ddongnum++;
				iter = ddong.erase(iter);
			}
			else
				iter++;
			
		}
		if (ddongnum >= MAX_DDONG)// 피한 똥개수가 100 개면 게임 클리어
		{
			ofstream out(strFile + "_user"  +".txt", ios::app);
			for (int i = 0; i < keyboard.size(); ++i)
			{
				out << keyboard[i].getX() << " ";
				out << keyboard[i].getTime() << endl;
			}
			out.close();
			return 3;
		}
		//level은 하나로만 리플레이 만들기
	}

	{
		ofstream out(strFile+"_user" + ".txt", ios::app);
		for (int i = 0; i < keyboard.size(); ++i)
		{
			out << keyboard[i].getX() << " ";
			out << keyboard[i].getTime() << endl;
		}
		out.close();
		return 2;
	}
}

int LoadGame(int level)
{
	

	system("Color F0");

	int key;
	int speed = 75 - level * LEVEL_SPEED;
	int count = 5 + level * LEVEL_COUNT;
	// 레벨업에 필요한 똥개수
	int levelup = 20 + level * 20;
	// 똥개수 체크. (최대값을 넘지 않도록 조정)
	if (count >= MAX_DDONG) count = MAX_DDONG - 1;
	// 스피드 체크. (최소값이 10 보다 작지 않도록 저정)
	if (speed < 10) speed = 10;
	// 유저 시작 위치.
	user.x = 12;
	user.y = 19;

	chrono::system_clock::time_point start;

	chrono::duration<double> toutosec;

	start = chrono::system_clock::now();

	// 게임 루프
	while (check)
	{
		Sleep(50);

		// 화면 지우기.
		system("cls");
		// 게임 환경 출력.
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
		cout << "[똥피하기게임]" << "		" << level << endl << "점수 :" << ddongnum;
		gotoxy(0, user.y + 3);	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x00);
		cout << "ㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁ" << "ㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁ";

		toutosec = chrono::system_clock::now() - start;
		// 사람 움직임 처리
		for(auto userController = keyboard.begin(); userController != keyboard.end();++userController)
		{
			if ((*userController).getTime() <= toutosec.count())
			{
				user.x = (*userController).getX();

			}

		}
		// 사람 출력
		gotoxy(user.x, user.y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
		cout << "O ";
		gotoxy(user.x, user.y + 1);
		cout << "ㅅ";
		gotoxy(user.x, user.y + 2);
		cout << "ㅅ";
		// 똥 움직임 처리
		toutosec = chrono::system_clock::now() - start;
		for (auto iter = ddong.begin(); iter != ddong.end();)
		{
			if ((*iter).getTime() <= toutosec.count())
				(*iter).update();
			if ((*iter).eraseDDong())
			{
				ddongnum++;
				iter = ddong.erase(iter);
			}
			else
				iter++;

		}


		if (ddongnum >= MAX_DDONG)// 피한 똥개수가 100 개면 게임 클리어
			return 3;
		//level은 하나로만 리플레이 만들기
	}
	return 2;
}
//게임 초기화
void ResetGame() {

	keyboard.clear(); ddong.clear(); check = 1; ddongnum = 0; maxNum = 0;
}
// 메인 함수
void main()
{
	system("mode con: cols=26 lines=25");
	int level = 1;

	system("Color 0A");
	gotoxy(6, 1);
	cout << "[똥피하기게임]";
	gotoxy(6, 11);
	cout << "1:  게임시작";
	gotoxy(6, 12);
	cout << "2:  가이드";
	gotoxy(6, 13);
	cout << "3:  리플레이";
	gotoxy(6, 14);
	cout << "4:  끝내기";
	gotoxy(0, 24);
	cout << " ";

	choose = _getch();

	if (choose == '1')
	{
		
		cout << " 저장할 파일 이름 : ";
		cin >> strFile;
		choose == '0';
		while (1)
		{
			// 화면 지우기.
			system("cls");
			system("Color 0A");
			// 레벨 표시.
			gotoxy(6, 1); cout << "[똥피하기게임]";
			gotoxy(9, 11); cout << "GAMESTART";
			// 시간 지연 (1초 = 1000)
			Sleep(1500);
			// 게임 시작.
			if (game(level) == 2)
			{
				system("cls");
				gotoxy(8, 5);
				system("Color 0F");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0D);
				cout << "게임 오버" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0A);
				gotoxy(8, 12);
				cout << "레벨: " << level;
				gotoxy(8, 13);
				cout << "점수: " << ddongnum;
				gotoxy(8, 22);
				cout << "1: 메인메뉴";
				gotoxy(8, 23);
				cout << "2: 끝내기";
				gotoxy(0, 24);
				cout << " ";
				
				ResetGame();
				do
				{
					choose = _getch();
					if (choose == '1')
						main();
					if (choose == '2')
						break;
				} while (1);
				break;
			}
			else if (game(level) == 3) //게임 클리어
			{
				system("cls");
				gotoxy(8, 5);
				system("Color 0F");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0D);
				cout << "게임 클리어" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0A);
				gotoxy(8, 12);
				cout << "레벨: " << level;
				gotoxy(8, 13);
				cout << "점수: " << ddongnum;
				gotoxy(8, 22);
				cout << "1: 메인메뉴";
				gotoxy(8, 23);
				cout << "2: 끝내기";
				gotoxy(0, 24);
				cout << " ";
				ResetGame();

				do
				{
					choose = _getch();
					if (choose == '1')
						main();
					if (choose == '2')
						break;
				} while (1);
				break;
			}
			else
				level++;
		}
	}
	else if (choose == '2')
	{
		choose = '0';
		system("cls");
		system("Color 0A");
		gotoxy(6, 1);
		cout << "[똥피하기게임]";
		gotoxy(10, 8);
		cout << "조작법";
		gotoxy(3, 10);
		cout << "이동   : 좌,우 화살표";
		gotoxy(8, 23);
		cout << "1: 돌아가기";
		gotoxy(0, 24);
		cout << " ";

		while (choose != '1')
		{
			choose = _getch();
		}
		main();
	}
	else if (choose == '3')
	{
		cout << endl;
		cout << " 불러올 파일 이름 : ";
		cin >> strFile;
		ifstream in(strFile + ".txt");

		int a = 0;


		while (!in.eof()) {
			int y;
			double t;
			in >> y >> t;
			if (in.eof())
				break;
			ddong.push_back(Obstacle(y, t));
			++a;
		}
		in.close();

		ifstream inUser(strFile + "_user.txt");

		a = 0;

	
		while (!inUser.eof()) {
			int y;
			double t;
			inUser >> y >> t;
			if (inUser.eof())
				break;
			keyboard.push_back(Keyboard(y, t));
			++a;
		}
		inUser.close();

		choose == '0';
		while (1)
		{
			// 화면 지우기.
			system("cls");
			system("Color 0A");
			// 레벨 표시.
			gotoxy(6, 1); cout << "[똥피하기게임]";
			gotoxy(9, 11); cout << "GAMESTART";
			// 시간 지연 (1초 = 1000)
			Sleep(1500);
			// 게임 시작.
			if (LoadGame(level) == 2)
			{
				system("cls");
				gotoxy(8, 5);
				system("Color 0F");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0D);
				cout << "게임 오버" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0A);
				gotoxy(8, 12);
				cout << "레벨: " << level;
				gotoxy(8, 13);
				cout << "점수: " << ddongnum;
				gotoxy(8, 22);
				cout << "1: 메인메뉴";
				gotoxy(8, 23);
				cout << "2: 끝내기";
				gotoxy(0, 24);
				cout << " ";

				ResetGame();
				do
				{
					choose = _getch();
					if (choose == '1')
						main();
					if (choose == '2')
						break;
				} while (1);
				break;
			}
			else if (LoadGame(level) == 3) //게임 클리어
			{
				system("cls");
				gotoxy(8, 5);
				system("Color 0F");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0D);
				cout << "게임 클리어" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0A);
				gotoxy(8, 12);
				cout << "레벨: " << level;
				gotoxy(8, 13);
				cout << "점수: " << ddongnum;
				gotoxy(8, 22);
				cout << "1: 메인메뉴";
				gotoxy(8, 23);
				cout << "2: 끝내기";
				gotoxy(0, 24);
				cout << " ";
				ResetGame();

				do
				{
					choose = _getch();
					if (choose == '1')
						main();
					if (choose == '2')
						break;
				} while (1);
				break;
			}
			else
				level++;
		}
	}
	else
		return;
}