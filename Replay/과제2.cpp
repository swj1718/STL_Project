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
// 惟績 発井 痕呪
#define MAX_DDONG       100 // 極税 置企 鯵呪.

#define LEVEL_COUNT     3   // 傾婚原陥 潅嬢劾 極鯵呪
#define LEVEL_SPEED     15  // 傾婚原陥 察虞霜 什杷球 (1000 = 1段, 15 = 0.015段)
// 極 姥繕端.

random_device rnd;
mt19937_64 dre(rnd());
uniform_int_distribution<> di(1, 24);

int check = 1;
int maxNum = 0; //置企 極姐呪
int ddongnum = 0; //杷廃 極 姐呪
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
// 鉢檎税 働舛 是帖稽 戚疑背 爽澗 敗呪.

struct User user;

void gotoxy(int x, int y)
{
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// 徹脊径 敗呪
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
		 // 極戚 置馬舘拭 亀鐸 梅聖 凶 坦軒.

		 // 極 窒径
		 gotoxy(x, y);
		 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf5);
		 cout << "��";
		 // 中宜 端滴
		 if (abs(x - user.x) < 2 && y == user.y)
		 {
			 // 搾覗製 窒径..
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

// 惟績 敗呪.
vector<Obstacle> ddong;
vector<Keyboard> keyboard;
int game(int level)
{
	system("Color F0");
	
	int i, key;
	int speed = 75 - level * LEVEL_SPEED;
	int count = 5 + level * LEVEL_COUNT;
	// 傾婚穣拭 琶推廃 極鯵呪
	int levelup = 20 + level * 20;
	// 極鯵呪 端滴. (置企葵聖 角走 省亀系 繕舛)
	if (count >= MAX_DDONG) count = MAX_DDONG - 1;
	// 什杷球 端滴. (置社葵戚 10 左陥 拙走 省亀系 煽舛)
	if (speed < 10) speed = 10;
	// 政煽 獣拙 是帖.
	user.x = 12;
	user.y = 19;

	chrono::system_clock::time_point start;

	chrono::duration<double> toutosec;

	start = chrono::system_clock::now();

	chrono::system_clock::time_point shoot; //極 1段拭 馬蟹梢 幻級嬢走惟

	chrono::duration<double> shootsec;

	shoot = chrono::system_clock::now();
	

	// 惟績 欠覗
	while (check)
	{
		Sleep(50);
		toutosec = chrono::system_clock::now() - start;
		shootsec = chrono::system_clock::now() - shoot;
		if (maxNum < MAX_DDONG && toutosec.count() > 2 && shootsec.count() >= 0.5)//極 100鯵幻 幻級奄
		{
			shoot = chrono::system_clock::now();
			maxNum++; 
			ddong.push_back(Obstacle(di(dre),toutosec.count()));
			auto iter = ddong.end()-1;
			(*iter).save();
		}

		// 鉢檎 走酔奄.
		system("cls");
		// 惟績 発井 窒径.
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
		cout << "[極杷馬奄惟績]" << "		" << level << endl << "繊呪 :" << ddongnum;
		gotoxy(0, user.y + 3);	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x00);
		cout << "けけけけけけけけけけけけけ" << "けけけけけけけけけけけけけ";
		toutosec = chrono::system_clock::now() - start;
		// 紫寓 崇送績 坦軒
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
		// 紫寓 窒径
		gotoxy(user.x, user.y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
		cout << "O ";
		gotoxy(user.x, user.y + 1);
		cout << "さ";
		gotoxy(user.x, user.y + 2);
		cout << "さ";
		// 極 崇送績 坦軒
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
		if (ddongnum >= MAX_DDONG)// 杷廃 極鯵呪亜 100 鯵檎 惟績 適軒嬢
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
		//level精 馬蟹稽幻 軒巴傾戚 幻級奄
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
	// 傾婚穣拭 琶推廃 極鯵呪
	int levelup = 20 + level * 20;
	// 極鯵呪 端滴. (置企葵聖 角走 省亀系 繕舛)
	if (count >= MAX_DDONG) count = MAX_DDONG - 1;
	// 什杷球 端滴. (置社葵戚 10 左陥 拙走 省亀系 煽舛)
	if (speed < 10) speed = 10;
	// 政煽 獣拙 是帖.
	user.x = 12;
	user.y = 19;

	chrono::system_clock::time_point start;

	chrono::duration<double> toutosec;

	start = chrono::system_clock::now();

	// 惟績 欠覗
	while (check)
	{
		Sleep(50);

		// 鉢檎 走酔奄.
		system("cls");
		// 惟績 発井 窒径.
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
		cout << "[極杷馬奄惟績]" << "		" << level << endl << "繊呪 :" << ddongnum;
		gotoxy(0, user.y + 3);	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x00);
		cout << "けけけけけけけけけけけけけ" << "けけけけけけけけけけけけけ";

		toutosec = chrono::system_clock::now() - start;
		// 紫寓 崇送績 坦軒
		for(auto userController = keyboard.begin(); userController != keyboard.end();++userController)
		{
			if ((*userController).getTime() <= toutosec.count())
			{
				user.x = (*userController).getX();

			}

		}
		// 紫寓 窒径
		gotoxy(user.x, user.y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
		cout << "O ";
		gotoxy(user.x, user.y + 1);
		cout << "さ";
		gotoxy(user.x, user.y + 2);
		cout << "さ";
		// 極 崇送績 坦軒
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


		if (ddongnum >= MAX_DDONG)// 杷廃 極鯵呪亜 100 鯵檎 惟績 適軒嬢
			return 3;
		//level精 馬蟹稽幻 軒巴傾戚 幻級奄
	}
	return 2;
}
//惟績 段奄鉢
void ResetGame() {

	keyboard.clear(); ddong.clear(); check = 1; ddongnum = 0; maxNum = 0;
}
// 五昔 敗呪
void main()
{
	system("mode con: cols=26 lines=25");
	int level = 1;

	system("Color 0A");
	gotoxy(6, 1);
	cout << "[極杷馬奄惟績]";
	gotoxy(6, 11);
	cout << "1:  惟績獣拙";
	gotoxy(6, 12);
	cout << "2:  亜戚球";
	gotoxy(6, 13);
	cout << "3:  軒巴傾戚";
	gotoxy(6, 14);
	cout << "4:  魁鎧奄";
	gotoxy(0, 24);
	cout << " ";

	choose = _getch();

	if (choose == '1')
	{
		
		cout << " 煽舌拝 督析 戚硯 : ";
		cin >> strFile;
		choose == '0';
		while (1)
		{
			// 鉢檎 走酔奄.
			system("cls");
			system("Color 0A");
			// 傾婚 妊獣.
			gotoxy(6, 1); cout << "[極杷馬奄惟績]";
			gotoxy(9, 11); cout << "GAMESTART";
			// 獣娃 走尻 (1段 = 1000)
			Sleep(1500);
			// 惟績 獣拙.
			if (game(level) == 2)
			{
				system("cls");
				gotoxy(8, 5);
				system("Color 0F");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0D);
				cout << "惟績 神獄" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0A);
				gotoxy(8, 12);
				cout << "傾婚: " << level;
				gotoxy(8, 13);
				cout << "繊呪: " << ddongnum;
				gotoxy(8, 22);
				cout << "1: 五昔五敢";
				gotoxy(8, 23);
				cout << "2: 魁鎧奄";
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
			else if (game(level) == 3) //惟績 適軒嬢
			{
				system("cls");
				gotoxy(8, 5);
				system("Color 0F");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0D);
				cout << "惟績 適軒嬢" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0A);
				gotoxy(8, 12);
				cout << "傾婚: " << level;
				gotoxy(8, 13);
				cout << "繊呪: " << ddongnum;
				gotoxy(8, 22);
				cout << "1: 五昔五敢";
				gotoxy(8, 23);
				cout << "2: 魁鎧奄";
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
		cout << "[極杷馬奄惟績]";
		gotoxy(10, 8);
		cout << "繕拙狛";
		gotoxy(3, 10);
		cout << "戚疑   : 疎,酔 鉢詞妊";
		gotoxy(8, 23);
		cout << "1: 宜焼亜奄";
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
		cout << " 災君臣 督析 戚硯 : ";
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
			// 鉢檎 走酔奄.
			system("cls");
			system("Color 0A");
			// 傾婚 妊獣.
			gotoxy(6, 1); cout << "[極杷馬奄惟績]";
			gotoxy(9, 11); cout << "GAMESTART";
			// 獣娃 走尻 (1段 = 1000)
			Sleep(1500);
			// 惟績 獣拙.
			if (LoadGame(level) == 2)
			{
				system("cls");
				gotoxy(8, 5);
				system("Color 0F");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0D);
				cout << "惟績 神獄" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0A);
				gotoxy(8, 12);
				cout << "傾婚: " << level;
				gotoxy(8, 13);
				cout << "繊呪: " << ddongnum;
				gotoxy(8, 22);
				cout << "1: 五昔五敢";
				gotoxy(8, 23);
				cout << "2: 魁鎧奄";
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
			else if (LoadGame(level) == 3) //惟績 適軒嬢
			{
				system("cls");
				gotoxy(8, 5);
				system("Color 0F");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0D);
				cout << "惟績 適軒嬢" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0A);
				gotoxy(8, 12);
				cout << "傾婚: " << level;
				gotoxy(8, 13);
				cout << "繊呪: " << ddongnum;
				gotoxy(8, 22);
				cout << "1: 五昔五敢";
				gotoxy(8, 23);
				cout << "2: 魁鎧奄";
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