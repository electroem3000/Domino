#pragma once
#include <string>
#include <iostream>
using namespace std;
struct Domino {
	int x{}, y{};
	Domino() {}
	Domino(string str) {
		x = str[0] - '0';
		y = str[2] - '0';
	}
	Domino(int x, int y) : x(x), y(y) {}

	bool operator==(const Domino& d)const {
		return x == d.x && y == d.y;
	}
};

int put(Domino dom, int end) // проверка можно ли доминошку присоединить к концу цепочки
{
	if (dom.x == end)
		return dom.y;
	if (dom.y == end)
		return dom.x;
	return -1;
}


