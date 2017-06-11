#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <string>

#define MAX_STEPS 500

using namespace std;


void clear(bool **mat, int H, int W) //Sets matrix to all dead
{
	for (int m = 0; m < H; m++)
	{
		for (int n = 0; n < W; n++)
			mat[m][n] = 0;
	}
}

void print(bool **mat, int H, int W) //Prints matrix to screen
{
	cout << setw(3) << " ";
	for (int p = 0; 5 * p < W; p++) cout << setw(5) << 5 * p + 1;
	cout << endl;
	for (int m = 0; m < H; m++)
	{
		cout << setw(3) << m + 1;
		for (int n = 0; n < W; n++)
		{
			if (mat[m][n]) cout << "X";
			else cout << /*"\xB1"*/"-";
		}
		cout << endl;
	}
}

void calculate(bool **mata, bool **matb, int H, int W)
{
	unsigned int neighbors;
	for (int m = 1; m < H; m++)
	{
		for (int n = 1; n < W; n++)
		{
			neighbors = 0;
			//Begin counting number of neighbors:
			//left-top
			if ((m > 0 && n>0) && mata[m - 1][n - 1] == 1) neighbors += 1;

			//left
			if ((m > 0) && mata[m - 1][n] == 1) neighbors += 1;

			//left-bottom
			if ((m > 0 && n < W) && mata[m - 1][n + 1] == 1) neighbors += 1;

			//top
			if ((n > 0) && mata[m][n - 1] == 1) neighbors += 1;

			//bottom
			if ((n < W) && mata[m][n + 1] == 1) neighbors += 1;

			//right-top
			if ((n > 0 && m < H - 1) && mata[m + 1][n - 1] == 1) neighbors += 1;

			//right
			if ((m < H - 1) && mata[m + 1][n] == 1) neighbors += 1;

			//right-bottom
			if ((m < H - 1 && n < W) && mata[m + 1][n + 1] == 1) neighbors += 1;

			if (m == H - 1) {
				if (n > 0 && mata[0][n - 1] == 1) neighbors += 1;
				if (mata[0][n] == 1) neighbors += 1;
				if ((n < W) && mata[0][n + 1] == 1) neighbors += 1;
			}

			if (n == W - 1) {
				if (m > 0 && mata[m - 1][0] == 1) neighbors += 1;
				if (mata[m][0] == 1) neighbors += 1;
				if (m < H - 1 && mata[m + 1][0] == 1) neighbors += 1;
			}

			//Apply rules to the cell:
			if (mata[m][n] == 1 && neighbors < 2)
				matb[m][n] = 0;
			else if (mata[m][n] == 1 && neighbors > 3)
				matb[m][n] = 0;
			else if (mata[m][n] == 1 && (neighbors == 2 || neighbors == 3))
				matb[m][n] = 1;
			else if (mata[m][n] == 0 && neighbors == 3)
				matb[m][n] = 1;
		}
	}
}

void swap(bool **mata, bool **matb, int H, int W) //Replaces first matrix with second
{
	for (int m = 0; m < H; m++)
	{
		for (int n = 0; n < W; n++)
			mata[m][n] = matb[m][n];
	}
}

int main()
{
	setlocale(LC_ALL, "");

	int x, y, cont; //Used for user input

	string iterationsCodes[MAX_STEPS];

	cout << "ѕравила игры:" << endl;
	cout << "1. Ѕела€ €чейка, у которой есть меньше чем две белые €чейки среди соседей, становитс€ чЄрной." << endl;
	cout << "2. Ѕела€ €чейка, у которой более чем три белые €чейки среди соседей, также становитс€ чЄрной." << endl;
	cout << "3. Ѕела€ €чейка, у которой две или три белые €чейки среди соседей, остаЄтс€ белой." << endl;
	cout << "4. „Єрна€ €чейка, у которой ровно три белые €чейки среди соседей, становитс€ белой." << endl << endl;
	cout << "Ќажмите любую клавишу дл€ старта. ¬ведите координаты €чейки(через пробел) дл€ оживлени€";
	cout << " огда формирование пол€ закончитс€ - введите \"-1\", его-же дл€ завершени€" << endl;
	cin.get();

	int height, width;
	cout << "¬ведите высоту пол€";
	cin >> height;
	cout << "¬ведите ширину пол€";
	cin >> width;

	//Creates now and then matrixes
	bool **now = new bool*[height];
	for (int i = 0; i < height; ++i) {
		now[i] = new bool[width];
	}

	bool **next = new bool*[height];
	for (int i = 0; i < height; ++i) {
		next[i] = new bool[width];
	}

	//clear
	clear(now, height, width);
	//print
	print(now, height, width);

	do //Get initial state
	{
		cin >> x;
		if (x == -1) break; //User is done inputting
		cin >> y;
		now[y - 1][x - 1] = 1; //Sets cell to alive
							   //print
		print(now, height, width);
	} while (x != -1);

	int iterationsCount = 0;
	do //Keep updating new generations
	{
		//clear
		clear(next, height, width);
		calculate(now, next, height, width);
		//swap
		swap(now, next, height, width);
		//print
		print(now, height, width);
		iterationsCount++;

		string currentIterationCode = "";
		bool isLifeExistOnIteration = false;

		for (int m = 0; m < height; m++)
		{
			for (int n = 0; n < width; n++) {
				currentIterationCode += now[m][n] ? "1" : "0";
				isLifeExistOnIteration = isLifeExistOnIteration || now[m][n];
			}
		}

		string *sameCode = std::find(std::begin(iterationsCodes), std::end(iterationsCodes), currentIterationCode);

		if (sameCode != std::end(iterationsCodes)) {
			cout << "ќбнаружен цикл на " << std::distance(iterationsCodes, sameCode) << " итерации" << endl;
			cont = -1;
		}
		else if (!isLifeExistOnIteration) {
			cout << "∆ивых клеток не осталось" << endl;
			cont = -1;
		}
		else {
			iterationsCodes[iterationsCount] = currentIterationCode;
			cin >> cont;
		}
	} while (cont != -1);

	cout << "»гра окончена за " << iterationsCount << " итераций" << endl;
	system("pause");
	return 0;
}