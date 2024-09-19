#include<iostream>
#include<sstream>
#include<vector>
#include<thread>
#include<chrono>
#include<conio.h>

using namespace std;
int res_x = 89, res_y = 50; //caps at around 89, 50 (depending on the termanal window size

int calculationResolution =100; //amount of calculations made per frame

vector<double> mass, object_x, object_y, velocity_x, velocity_y;
vector<bool> screen((res_x* res_y), false);

double G = 0.01; //gravitational constant (4 is pretty good)

void createObject(double mass_value, double x, double y, double x_vel, double y_vel) {
	mass.push_back(mass_value);
	object_x.push_back(x);
	object_y.push_back(y);
	velocity_x.push_back(x_vel);
	velocity_y.push_back(y_vel);
}

void drawPoint(double x, double y) {
	int yr = round(y), xr = round(x);
		if ((xr > 0 and yr > 0) and (xr < res_x and yr < res_y)) {
			screen[yr * res_x + xr] = true;
		}
}

//changing this is inverse of framerate ie: 100ms = 0.1 fps
void tick() {
	this_thread::sleep_for(100ms);
}

void updatePositions() {
	for (int f = 0; f < mass.size(); f++) {
		object_x[f] = object_x[f] + (velocity_x[f]/calculationResolution);
		object_y[f] = object_y[f] + (velocity_y[f]/calculationResolution);
		drawPoint(object_x[f], object_y[f]);
	}

}

void updateVelocities() {
	if (mass.size() > 1) {
		for (int i = 0; i < mass.size(); i++) {
			for (int p = 0; p < mass.size(); p++) {
				
				if (not(p == i)) {
					double relx = (object_x[p] - object_x[i]), rely = (object_y[p] - object_y[i]),
					distance = sqrt((relx * relx) + (rely * rely)),
					velocity = ((G * mass[p]) / distance*distance),
					angle = atan2(rely, relx),
					addedvel_x = (velocity * cos(angle)),
					addedvel_y = (velocity * sin(angle));
					velocity_x[i] = velocity_x[i] + (addedvel_x/calculationResolution);
					velocity_y[i] = velocity_y[i] + (addedvel_y/calculationResolution);
					
				}
			}
		}
	}
}

void clearScreen() {
	fill(screen.begin(), screen.end(), false);
}

void renderScreen() {

	system("cls");
	for (int i = 0; i < res_y; ++i) {
		ostringstream line;
		for (int p = 0; p < res_x; ++p) {

			if (screen[i * res_x + p]) {
				line << "[]";
			}
			else {
				line << "  ";
			}
		}
		cout << line.str() << "\n";
	}
}

void main() {
	createObject(10, 20, 30, 0, 0);
	createObject(1, 50, 30, 0, -0.4);
	createObject(0.1, 55, 30, 0.0, -0.44);

	while (true) {
		for (int j = 0; j < calculationResolution; j++) {
			clearScreen();
			updateVelocities();
			updatePositions();
		}
		renderScreen();


		tick();
	}
}