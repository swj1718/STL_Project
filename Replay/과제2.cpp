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
// ���� ȯ�� ����
#define MAX_DDONG       100 // ���� �ִ� ����.

#define LEVEL_COUNT     3   // �������� �þ �˰���
#define LEVEL_SPEED     15  // �������� ������ ���ǵ� (1000 = 1��, 15 = 0.015��)
// �� ����ü.

random_device rnd;
mt19937_64 dre(rnd());
uniform_int_distribution<> di(1, 24);

int check = 1;
int maxNum = 0; //�ִ� �˰���
int ddongnum = 0; //���� �� ����
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
// ȭ���� Ư�� ��ġ�� �̵��� �ִ� �Լ�.

struct User user;

void gotoxy(int x, int y)
{
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// Ű�Է� �Լ�
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
		 // ���� ���ϴܿ� ���� ���� �� ó��.

		 // �� ���
		 gotoxy(x, y);
		 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf5);
		 cout << "��";
		 // �浹 üũ
		 if (abs(x - user.x) < 2 && y == user.y)
		 {
			 // ������ ���..
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

// ���� �Լ�.
vector<Obstacle> ddong;
vector<Keyboard> keyboard;
int game(int level)
{
	system("Color F0");
	
	int i, key;
	int speed = 75 - level * LEVEL_SPEED;
	int count = 5 + level * LEVEL_COUNT;
	// �������� �ʿ��� �˰���
	int levelup = 20 + level * 20;
	// �˰��� üũ. (�ִ밪�� ���� �ʵ��� ����)
	if (count >= MAX_DDONG) count = MAX_DDONG - 1;
	// ���ǵ� üũ. (�ּҰ��� 10 ���� ���� �ʵ��� ����)
	if (speed < 10) speed = 10;
	// ���� ���� ��ġ.
	user.x = 12;
	user.y = 19;

	chrono::system_clock::time_point start;

	chrono::duration<double> toutosec;

	start = chrono::system_clock::now();

	chrono::system_clock::time_point shoot; //�� 1�ʿ� �ϳ��� ���������

	chrono::duration<double> shootsec;

	shoot = chrono::system_clock::now();
	

	// ���� ����
	while (check)
	{
		Sleep(50);
		toutosec = chrono::system_clock::now() - start;
		shootsec = chrono::system_clock::now() - shoot;
		if (maxNum < MAX_DDONG && toutosec.count() > 2 && shootsec.count() >= 0.5)//�� 100���� �����
		{
			shoot = chrono::system_clock::now();
			maxNum++; 
			ddong.push_back(Obstacle(di(dre),toutosec.count()));
			auto iter = ddong.end()-1;
			(*iter).save();
		}

		// ȭ�� �����.
		system("cls");
		// ���� ȯ�� ���.
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
		cout << "[�����ϱ����]" << "		" << level << endl << "���� :" << ddongnum;
		gotoxy(0, user.y + 3);	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x00);
		cout << "��������������������������" << "��������������������������";
		toutosec = chrono::system_clock::now() - start;
		// ��� ������ ó��
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
		// ��� ���
		gotoxy(user.x, user.y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
		cout << "O ";
		gotoxy(user.x, user.y + 1);
		cout << "��";
		gotoxy(user.x, user.y + 2);
		cout << "��";
		// �� ������ ó��
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
		if (ddongnum >= MAX_DDONG)// ���� �˰����� 100 ���� ���� Ŭ����
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
		//level�� �ϳ��θ� ���÷��� �����
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
	// �������� �ʿ��� �˰���
	int levelup = 20 + level * 20;
	// �˰��� üũ. (�ִ밪�� ���� �ʵ��� ����)
	if (count >= MAX_DDONG) count = MAX_DDONG - 1;
	// ���ǵ� üũ. (�ּҰ��� 10 ���� ���� �ʵ��� ����)
	if (speed < 10) speed = 10;
	// ���� ���� ��ġ.
	user.x = 12;
	user.y = 19;

	chrono::system_clock::time_point start;

	chrono::duration<double> toutosec;

	start = chrono::system_clock::now();

	// ���� ����
	while (check)
	{
		Sleep(50);

		// ȭ�� �����.
		system("cls");
		// ���� ȯ�� ���.
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
		cout << "[�����ϱ����]" << "		" << level << endl << "���� :" << ddongnum;
		gotoxy(0, user.y + 3);	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x00);
		cout << "��������������������������" << "��������������������������";

		toutosec = chrono::system_clock::now() - start;
		// ��� ������ ó��
		for(auto userController = keyboard.begin(); userController != keyboard.end();++userController)
		{
			if ((*userController).getTime() <= toutosec.count())
			{
				user.x = (*userController).getX();

			}

		}
		// ��� ���
		gotoxy(user.x, user.y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
		cout << "O ";
		gotoxy(user.x, user.y + 1);
		cout << "��";
		gotoxy(user.x, user.y + 2);
		cout << "��";
		// �� ������ ó��
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


		if (ddongnum >= MAX_DDONG)// ���� �˰����� 100 ���� ���� Ŭ����
			return 3;
		//level�� �ϳ��θ� ���÷��� �����
	}
	return 2;
}
//���� �ʱ�ȭ
void ResetGame() {

	keyboard.clear(); ddong.clear(); check = 1; ddongnum = 0; maxNum = 0;
}
// ���� �Լ�
void main()
{
	system("mode con: cols=26 lines=25");
	int level = 1;

	system("Color 0A");
	gotoxy(6, 1);
	cout << "[�����ϱ����]";
	gotoxy(6, 11);
	cout << "1:  ���ӽ���";
	gotoxy(6, 12);
	cout << "2:  ���̵�";
	gotoxy(6, 13);
	cout << "3:  ���÷���";
	gotoxy(6, 14);
	cout << "4:  ������";
	gotoxy(0, 24);
	cout << " ";

	choose = _getch();

	if (choose == '1')
	{
		
		cout << " ������ ���� �̸� : ";
		cin >> strFile;
		choose == '0';
		while (1)
		{
			// ȭ�� �����.
			system("cls");
			system("Color 0A");
			// ���� ǥ��.
			gotoxy(6, 1); cout << "[�����ϱ����]";
			gotoxy(9, 11); cout << "GAMESTART";
			// �ð� ���� (1�� = 1000)
			Sleep(1500);
			// ���� ����.
			if (game(level) == 2)
			{
				system("cls");
				gotoxy(8, 5);
				system("Color 0F");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0D);
				cout << "���� ����" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0A);
				gotoxy(8, 12);
				cout << "����: " << level;
				gotoxy(8, 13);
				cout << "����: " << ddongnum;
				gotoxy(8, 22);
				cout << "1: ���θ޴�";
				gotoxy(8, 23);
				cout << "2: ������";
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
			else if (game(level) == 3) //���� Ŭ����
			{
				system("cls");
				gotoxy(8, 5);
				system("Color 0F");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0D);
				cout << "���� Ŭ����" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0A);
				gotoxy(8, 12);
				cout << "����: " << level;
				gotoxy(8, 13);
				cout << "����: " << ddongnum;
				gotoxy(8, 22);
				cout << "1: ���θ޴�";
				gotoxy(8, 23);
				cout << "2: ������";
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
		cout << "[�����ϱ����]";
		gotoxy(10, 8);
		cout << "���۹�";
		gotoxy(3, 10);
		cout << "�̵�   : ��,�� ȭ��ǥ";
		gotoxy(8, 23);
		cout << "1: ���ư���";
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
		cout << " �ҷ��� ���� �̸� : ";
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
			// ȭ�� �����.
			system("cls");
			system("Color 0A");
			// ���� ǥ��.
			gotoxy(6, 1); cout << "[�����ϱ����]";
			gotoxy(9, 11); cout << "GAMESTART";
			// �ð� ���� (1�� = 1000)
			Sleep(1500);
			// ���� ����.
			if (LoadGame(level) == 2)
			{
				system("cls");
				gotoxy(8, 5);
				system("Color 0F");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0D);
				cout << "���� ����" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0A);
				gotoxy(8, 12);
				cout << "����: " << level;
				gotoxy(8, 13);
				cout << "����: " << ddongnum;
				gotoxy(8, 22);
				cout << "1: ���θ޴�";
				gotoxy(8, 23);
				cout << "2: ������";
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
			else if (LoadGame(level) == 3) //���� Ŭ����
			{
				system("cls");
				gotoxy(8, 5);
				system("Color 0F");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0D);
				cout << "���� Ŭ����" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0A);
				gotoxy(8, 12);
				cout << "����: " << level;
				gotoxy(8, 13);
				cout << "����: " << ddongnum;
				gotoxy(8, 22);
				cout << "1: ���θ޴�";
				gotoxy(8, 23);
				cout << "2: ������";
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