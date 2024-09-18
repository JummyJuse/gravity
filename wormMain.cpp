#include<iostream>
#include<vector>
#include <thread>
#include<chrono>
#include<conio.h>
#include<string>
#include<random>

using namespace std;
int res_x = 15, res_y = 15; //caps at around 89, 50 (depending on the termanal window size)
int head_x = 5, head_y = 5, length = 1;
float pkey;
bool alive = true;
vector<int> screen((res_x* res_y), false);
int food_x = 8, food_y = 8;

int random(int min, int max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<>distr(min, max);
	return distr(gen);
}

/*
* clears the termanal and fills it with "  " and "[]" as a make-shift screen using (res_x, res_y) values. Uses a int vector called screen and plots values greater than 0.
*/
void renderScreen() {
	system("cls");
	cout << "  'wasd' to move";
	for (int i = 0; i < res_y; ++i) {
		if (i > 0) {
			for (int p = 0; p < res_x; ++p) {
				int pixelValue = screen[i * res_x + p];
				if ((food_x == p) and (food_y == i)) {
					cout << "<>";
				}
				else if (pixelValue == length) {
					if ((pkey == 'w') or (pkey == 's')) {
						cout << "oo";
					}
					else {
						cout << "8 ";
					}
				}
				else if ((pixelValue > 0) or (p == 0)) {
					cout << "  ";
				}
				else if (screen[i * res_x + p] == 0) {
					cout << "[]";
				}
			}
		}
		cout << endl;
	}
	cout << "  things: " << length - 1;
}

/*
detects what key is pressed (if any) and carries out moving the head coordinates according to what direction its in
*/
void testMovements() {
	if (_kbhit()) {
		char key = _getche();
		if ((key == 'w') and not(pkey == 's')) {
			pkey = key;
		}
		else if ((key == 's') and not(pkey == 'w')) {
			pkey = key;
		}
		else if ((key == 'a') and not(pkey == 'd')) {
			pkey = key;
		}
		else if ((key == 'd') and not(pkey == 'a')) {
			pkey = key;
		}
	}
	if (pkey == 'w') {
		head_y--;
	}
	else if (pkey == 's') {
		head_y++;
	}
	else if (pkey == 'd') {
		head_x++;
	}
	else if (pkey == 'a') {
		head_x--;
	}
	else {
		return;
	}
}

/*
*Updates values on the screen vector by subtracting values greater than zero by 1, and then plotting the position of the head with the value of the length
*/
void graphPoints() {
	for (int k = 0; k < screen.size(); k++) { //iterate through whole screen, simulate movements
		if (screen[k] > 0) {
			screen[k] = screen[k] - 1;
		}
	}
	int yr = round(head_y), xr = round(head_x);
	if ((yr < 1) or (xr < 1) or (yr > res_y - 1) or (xr > res_x - 1)) {
		alive = false;
	}
	else if ((screen[(yr)*res_x + xr] > 0) and (pkey)) {
		alive = false;
	}
	else {
		screen[(yr)*res_x + xr] = length;
	}
	if ((head_x == food_x) and (head_y == food_y)) { //eating procedure
		length++; //add to worm size
		food_x = random(1, res_x - 1); //set new position of food
		food_y = random(1, res_y - 1);
	}
}

void main() {
	while (alive) {
		testMovements();
		graphPoints();
		renderScreen();
		this_thread::sleep_for(100ms);
	}
}