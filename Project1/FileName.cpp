#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include "Domino.h"


using namespace std;

// Проверка на ввод данных
void input(int& x, const char* msg = "");

// Проверка на ввод пункта меню
void checkpoint(int& x, int maxim, int minim = 0, const char* msg = "");

// Проверка на ввод имени файла
string readfilename();

// Проверка на существование файла с данным именем
void checkfilename(string& file_name);


istream& operator>>(istream& stream, Domino& dom) {
	string str;
	stream >> str;
	dom.x = str[0] - '0';
	dom.y = str[2] - '0';
	return stream;
}

ostream& operator <<(ostream& stream, const Domino& dom) {
	return stream << dom.x << ';' << dom.y;
}


istream& operator>>(istream& stream, list<Domino>& p) {
	p.clear();
	string str;
	stringstream ss;
	getline(stream, str);
	ss << str;
	while (ss >> str) {
		p.push_back(Domino(str));
	}
	return stream;
}

ostream& operator<<(ostream& stream, list<Domino>& p) {
	for (const Domino d : p)
		stream << d << ' ';
	return stream;
}


bool task(list<Domino>& p1, list<Domino>& p2, int left, int right, string& seq, bool skipped = false, bool p1_is_playing = true)
{
	bool ans= false;
	if (p1.empty() || p2.empty()) // если игра закончилась (игра заканчивается, когда кто-то из игроков выложил все костяшки)
		ans = false;
	else
	{

		if (p1_is_playing) // если ходит первый игрок
		{
			bool skip = true; // пропуск хода первым игроком
			for (list<Domino>::iterator i = p1.begin(); i != p1.end() && ans == false; i ++)
			{
				Domino d = *i;
				list<Domino> newp1(p1);
				newp1.remove(d);

				int newleft = put(d, left); // игрок кладет i-ю доминошку к левому концу цепочки
				if (newleft != -1) // если она подошла, то при новых условиях спускаемся в рекурсию
				{
					skip = false;
					string newseq = to_string(newleft) + ';' + to_string(left) + ' ' + seq;
					ans = task(newp1, p2, newleft, right, newseq, skip, false);
				}

				int newright = put(d, right);  // игрок кладет i-ю доминошку к правому концу цепочки
				if (newright != -1) // если она подошла, то при новых условиях спускаемся в рекурсию
				{
					skip = false;
					string newseq =  seq + ' ' + to_string(right) + ';' + to_string(newright);
					if (!ans)
						ans = task(newp1, p2, left, newright, newseq, skip, false);
				}
			}
			if (skip)
			{
				ans = task(p1, p2, left, right, seq, skip, false);

			}

		}
		else // если ходит второй игрок
		{
			bool skip = true; // пропуск хода вторым игроком
			for (list<Domino>::iterator i = p2.begin(); i != p2.end() && ans == false; i++)
			{
				Domino d = *i;
				list<Domino> newp2(p2);
				newp2.remove(d);
				int newleft = put(d, left);
				if (newleft != -1)
				{
					skip = false;
					string newseq = to_string(newleft) + ';' + to_string(left) + ' ' + seq;
					ans = task(p1, newp2, newleft, right,  newseq, skip);
				}

				int newright = put(d, right);
				if (newright != -1) {
					skip = false;
					string newseq = seq + ' ' + to_string(right) + ';' + to_string(newright);
					if (!ans)
						ans = task(p1, newp2, left, newright, newseq,  skip);
				}

			}
			if (skip) // если второй должен пропустить ход, то мы получили искомую последовательность ходов
			{
				if (!skipped) // если это не тот случай, когда оба игрока не имеют подходящих костей и игра закончилась ничьей
				{
					ans = true;
					cout << "Есть:\n";
					cout << seq << endl;
				}

			}
		}
	}
	return ans;
}
void main_menu()
{
	int x = 1;
	int left{}, right{};
	bool ans = false;
	list<Domino> p1, p2;
	while (x)
	{
		cout << "1.  Ввод из файла\n";
		cout << "2.  Ввод границ цепочки домино\n";
		cout << "3.  Ввод набора костяшек у игроков\n";
		cout << "4.  Печать\n";
		cout << "5.  Task\n";
		cout << "0.  Выход\n\n";
		checkpoint(x, 5);
		switch (x)
		{
		case 1: // Ввод из файла
		{
			string filename;
			checkfilename(filename);
			ifstream file(filename);
			file >> left >> right;
			file.get();
			file >> p1 >> p2;

			
			cout << "Границы: left = " << left << ", right =" << right << endl;
			cout << "Player1: " << p1 << endl;
			cout << "Player2: " << p2 << endl;

			break;
		}
		case 2: // Ввод границ цепочки домино
		{
			cout << "Введите левую границу: ";
			checkpoint(left, 6);
			cout << "Введите правую границу: ";
			checkpoint(right, 6);
			break;
		}

		case 3: // Ввод набора костяшек у игроков
		{
			cout << "Введите набор для первого игрока через пробел (например, 0;1 6;6 и тд):\n";
			
			cin >> p1;
			cout << "Введите набор для второго игрока через пробел (например, 0;1 6;6 и тд):\n";
			cin >> p2;
			break;
		}
		case 4: // Печать
		{
			if (p1.empty() && p2.empty())
			{
				cout << "У Игроков нет доминошек!\n";
			}
			else
			{
				cout << "Границы: left = " << left << ", right =" << right << endl;
				cout << "Player1: " << p1 << endl;
				cout << "Player2: " << p2 << endl;
			}

			break;
		}
		case 5: // Task
		{
			if (p1.empty() && p2.empty())
			{
				cout << "У Игроков нет доминошек!\n";
			}
			else
			{
				string str = to_string(left) + "..." + to_string(right);
				ans = task(p1, p2, left, right, str);
				if (!ans)
					cout << "Нет\n";
			}
			break;
		}

		}
		cout << endl;
	}
}

int main() {
	setlocale(LC_ALL, "rus");
	main_menu();
	return 0;
}


void input(int& x, const char* msg)
{
	bool good = 0;
	do {
		cout << msg;
		cin >> x;
		if (!cin.fail()) {
			good = 1;
			cin.clear();
			cin.ignore(cin.rdbuf()->in_avail());
		}
		else {
			cout << "Ошибка ввода!\n";
			cin.clear();
			cin.ignore(cin.rdbuf()->in_avail());
		}
	} while (!good);
}

void checkpoint(int& x, int maxim, int minim, const char* msg)
{
	input(x);
	while (x > maxim || x < minim) {
		cout << "Ошибка ввода!\n";
		input(x, msg);
	}
}



string readfilename()
{
	string file_name;
	cout << "Введите имя файла:\n";
	cin >> file_name;
	return file_name;
}

void checkfilename(string& file_name)
{
	bool good = true;
	do {
		file_name = readfilename();
		ifstream file(file_name);
		if (file.fail())
		{
			cout << "Файла не существует!\n";
			good = false;
		}
		else
			good = true;
	} while (!good);
}



//
//struct Player {
//	Domino* dominos = nullptr;
//	int size = 0;
//	Player() {}
//	Player(const Player &p) {
//		this->dominos = new Domino[p.size];
//		int i = 0;
//		while (i < size)
//		{
//			
//			this->dominos[i] = dominos[i];
//			i++;
//		}
//		this->size = p.size;
//	}
//	Player(const Domino* dominos, int size, int index_of_used_domino) {
//		
//		this->dominos = new Domino[size-1];
//		int i = 0;
//		while (i < size)
//		{
//			if (i != index_of_used_domino)
//				this->dominos[i] = dominos[i];
//			i++;
//		}
//		this->size = size - 1;
//	}
//	~Player(){
//		delete[] dominos;
//	}
//	bool empty() {
//		return size == 0;
//	}
//
//};
//
//void task(Player &p1, Player &p2, int left, int right,  bool &ans, bool p1_is_playing = true)
//{
//	if (!(ans || p1.empty() || p2.empty())) // если мы не нашли еще последовательность ходов или если игра не закончилась
//		// (игра заканчивается, когда кто-то из игроков выложил все костяшки)
//	{
//
//		if (p1_is_playing) // если ходит первый игрок
//		{
//			bool skip = true; // пропуск хода первым игроком
//			for (int i = 0; i < p1.size && !ans; i++) 
//			{
//				int newleft = put(p1.dominos[i], left); // игрок кладет i-ю доминошку к левому концу цепочки
//				if (newleft != -1) // если она подошла, то при новых условиях спускаемся в рекурсию
//				{
//					skip = false;
//					Player newp1(p1.dominos, p1.size, i);
//					task(newp1, p2, newleft, right, ans, false);
//				}
//
//				int newright = put(p1.dominos[i], right);  // игрок кладет i-ю доминошку к правому концу цепочки
//				if (newright != -1) // если она подошла, то при новых условиях спускаемся в рекурсию
//				{
//					skip = false;
//					Player newp1(p1.dominos, p1.size, i);
//					task(newp1, p2, left, newright,  ans, false);
//				}
//			}
//			if (skip) 
//			{
//				task(p1, p2, left, right, ans, false);
//			}
//
//		}
//		else // если ходит второй игрок
//		{
//			bool skip = true; // пропуск хода вторым игроком
//			for (int i = 0; i < p2.size && !ans; i++)
//			{
//				int newleft = put(p2.dominos[i], left);
//				if (newleft != -1)
//				{
//					skip = false;
//					Player newp2(p2.dominos, p2.size, i);
//					task(p1, newp2, newleft, right,  ans);
//				}
//
//				int newright = put(p2.dominos[i], right);
//				if (newright != -1) {
//					skip = false;
//					Player newp2(p2.dominos, p2.size, i);
//					task(p1, newp2, left, newright,  ans);
//				}
//
//			}
//			if (skip) // если второй должен пропустить ход, то мы получили искомую последовательность ходов
//			{
//				ans = true;
//				cout << "YES\n";
//			}
//		}
//	}
//		
//}


//void task(list<Domino> &p1, list<Domino>& p2, int left, int right, bool& ans,  bool skipped = false, bool p1_is_playing = true )
//{
//	if (!(ans || p1.empty() || p2.empty())) // если мы не нашли еще последовательность ходов или если игра не закончилась
//		// (игра заканчивается, когда кто-то из игроков выложил все костяшки)
//	{
//
//		if (p1_is_playing) // если ходит первый игрок
//		{
//			bool skip = true; // пропуск хода первым игроком
//			for (const Domino& d: p1)
//			{
//				list<Domino> newp1(p1);
//				newp1.remove(d);
//
//				int newleft = put(d, left); // игрок кладет i-ю доминошку к левому концу цепочки
//				if (newleft != -1) // если она подошла, то при новых условиях спускаемся в рекурсию
//				{
//					skip = false;
//					task(newp1, p2, newleft, right, ans,  skip, false);
//				}
//
//				int newright = put(d, right);  // игрок кладет i-ю доминошку к правому концу цепочки
//				if (newright != -1) // если она подошла, то при новых условиях спускаемся в рекурсию
//				{
//					skip = false;
//					task(newp1, p2, left, newright, ans,  skip, false);
//				}
//			}
//			if (skip)
//			{
//				task(p1, p2, left, right, ans,  skip, false);
//				
//			}
//
//		}
//		else // если ходит второй игрок
//		{
//			bool skip = true; // пропуск хода вторым игроком
//			for (const Domino& d: p2)
//			{
//				list<Domino> newp2(p2);
//				newp2.remove(d);
//				int newleft = put(d, left);
//				if (newleft != -1)
//				{
//					skip = false;					
//					task(p1, newp2, newleft, right, ans, skip);
//				}
//
//				int newright = put(d, right);
//				if (newright != -1) {
//					skip = false;					
//					task(p1, newp2, left, newright, ans, skip);
//				}
//
//			}
//			if (skip) // если второй должен пропустить ход, то мы получили искомую последовательность ходов
//			{
//				if (!skipped) // если это не тот случай, когда оба игрока не имеют подходящих костей и игра закончилась ничьей
//					ans = true;
//			}
//		}
//	}
//
//}
//