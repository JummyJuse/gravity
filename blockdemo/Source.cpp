#include<iostream>
#include<sstream>
#include<conio.h>
#include<chrono>
#include<math.h>
#include<vector>
#include<thread>

using namespace std;
const int cubeLength = 20;
int distanceFromCamera = 50;
float stepSize = 0.6;
int FOV = 16;
int screen_x = 160, screen_y = 50;
float x, y, z;
float A = 0, B = 0, C = 0;
float VA = 0, VB = 0;
vector<float> zbuf;
vector<char> buf;
void clearSD() {
	buf.clear();
	zbuf.clear();
	for (int i = 0; i < screen_x * screen_y * 4; i++) {
		buf.push_back(' ');
		zbuf.push_back(0.0);
	}
}
void tick() {
	this_thread::sleep_for(10ms);
}
void testMovements() {
	A = A + 0.1 * VA;
	B = B + 0.1 * VB;
	VA = VA / 1.1;
	VB = VB / 1.1;
	if (_kbhit()) {
		char key = _getche();
		if (key == 'w') {
			VA = VA + 0.1;
		}
		else if (key == 's') {
			VA = VA - 0.1;
		}
		else if (key == 'a') {
			VB = VB + 0.1;
		}
		else if (key == 'd') {
			VB = VB - 0.1;
		}
	}
}
void calculateFace(float cubeX, float cubeY, float cubeZ, int ch) {
	x = (cubeY * sin(A) * sin(B) * cos(C)) 
		- (cubeZ * cos(A) * sin(B) * cos(C)) 
		+ (cubeY * cos(A) * sin(C)) 
		+ (cubeZ * sin(A) * sin(C)) 
		+ (cubeX * cos(B) * cos(C));
	y = (cubeY * cos(A) * cos(C)) 
		+ (cubeZ * sin(A) * cos(C)) 
		- (cubeY * sin(A) * sin(B) * sin(C)) 
		+ (cubeZ * cos(A) * sin(B) * sin(C)) 
		- (cubeX * cos(B) * sin(C));
	z = (cubeZ * cos(A) * cos(B)) 
		- (cubeY * sin(A) * cos(B)) 
		+ (cubeX * sin(B)) + distanceFromCamera;
	float depth = 1 / z;
	int px = ((screen_x / 2) - x * depth * FOV * 2),
		py = ((screen_y / 2) - y * depth * FOV);
	//cout << px << py;
	int pid = (px + (py * screen_x));
	if (pid > 0 && pid < screen_x * screen_y) {
		if (depth > zbuf[pid]) {
			zbuf[pid] = depth;
			buf[pid] = ch;
			//cout << "h";
		}
	}
}
void render() {
	system("cls");
	cout << "'wasd' to move\n";
	unsigned int k = 0;
	for (unsigned int i = 0; i < screen_y; i++) {
		ostringstream line;
		for (unsigned int j = 0; j < screen_x; j++) {
			line << buf[k];
			k++;
		}
		cout << line.str() << "\n";
	}

}
void main() {
	cout << "adjust the screen resolution to your liking, usually around (x=160, y=50)\nX res: ";
	cin >> screen_x;
	cout << "\nY res: ";
	cin >> screen_y;
	while (true) {
		clearSD();
		for (float Xcube = -cubeLength; Xcube < cubeLength; Xcube = Xcube + stepSize) {
			for (float Ycube = -cubeLength; Ycube < cubeLength; Ycube = Ycube + stepSize) {
				calculateFace(Xcube, Ycube, -cubeLength, '+');
				calculateFace(-Xcube, -Ycube, cubeLength, ':');
				calculateFace(Ycube, -cubeLength, Xcube, '#');
				calculateFace(Ycube, cubeLength, Xcube, '.');
				calculateFace(-cubeLength, Ycube, Xcube, '=');
				calculateFace(cubeLength, Ycube, Xcube, '-');


			}
		}
		render();
		A = A + 0.1;
		B = B + 0.1;

		//system("cls");for (int h = 0; h < buf.size(); h++) {cout << buf[h] << " ";}
		testMovements();
		tick();
	}
}