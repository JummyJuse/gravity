#include<iostream>
#include<conio.h>
#include<thread>
#include<chrono>
#include<string>
#include<vector>
using namespace std;

void waitTick() {
	this_thread::sleep_for(20ms);
}

void main() {
start:
	bool index = false;
	while (true) {

		//Starting Screen
		system("cls");
		cout << "Press Space to Change Selected mode\n\n";
		if (index) {
			cout << "Basic Mode    <---------\nAdvanced Mode\n\n";
		}
		else {
			cout << "Basic Mode\nAdvanced Mode <---------\n\n";
		}
		cout << "Press Enter to Continue";
		//Package user selection
		int key = _getch();
		if (key == 32) {
			if (not(index)) {
				index = true;
			}
			else if (index) {
				index = false;
			}
		}
		waitTick();
		if (key == 13) {
			break;
		}
	}
	if (index) {
	basic:
		system("cls");
		//ASSIGNED-WORK-STARTS-HERE-------------------------------------------------
		double totalInches;
		cout << "Basic Mode\n\nPlease Enter Number of Inches: ";
		cin >> totalInches;
		int feet = totalInches / 12;
		cout << "\nThis is " << feet << " feet and " << totalInches - feet * 12 << " inches\n\n";
		//ASSIGNED-WORK-ENDS-HERE---------------------------------------------------
		cout << "Press Space to Restart or Enter to Change Mode";
		while (true) {
			if (_getch() == 32) {
				goto basic;
			}
			else if (_getch() == 13) {
				goto start;
			}
		}
	}
	else {
		int selectionCap = 5, arrow = 4;
		while (true) {
			system("cls");
			cout << "Enter to Change Mode\n\n";
			vector<string> units{
				"centimeters", "meters", "feet", "inches", "kilometers", "miles"
			};
			waitTick();

			for (int row = 0; row < (selectionCap + 1); ++row) {
				cout << units[row];
				if (arrow == row) {
					cout <<"   <-------\n";
				}
				else {
					cout << endl;
				}
			}

			int key = _getch();
			if (key == 72) {
				if (not(0 == arrow)) {
					arrow = arrow - 1;
				}
			}
			else if (key == 80) {
				if (not(selectionCap == arrow)) {
					arrow = arrow + 1;
				}
			}
			else if (key == 13) {
				goto start;
			}
		}
	}
}