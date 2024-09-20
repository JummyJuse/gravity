#include<iostream>
#include<sstream>
#include<conio.h>
#include<chrono>
#include<math.h>
#include<vector>
#include<thread>

using namespace std;
int cubeLength = 20;
int distanceFromCamera = 50;
float stepSize = 0.6;
int FOV = 16;
int screen_x = 100, screen_y = 50;
float x, y, z, xp, yp, zp;
float cx = 0, cy = 0, cz = -50;
float A = 0, B = 0, C = 0;
float VA = 0, VB = 0;
vector<float> zbuf;
vector<char> buf;
vector<float> cubes{ 0, 0, 0, 1, 3, 0 };
static void clearSD() {
	buf.clear();
	zbuf.clear();
	for (int i = 0; i < screen_x * screen_y * 4; i++) {
		buf.push_back(' ');
		zbuf.push_back(0.0);
	}
}
void tick() {
	this_thread::sleep_for(1ms);
}
void testMovements() {
	if (_kbhit()) {
		char key = _getche();
		if (key == 72) {
			C = C + 0.1;
		}
		else if (key == 80) {
			C = C - 0.1;
		}
		else if (key == 75) {
			B = B + 0.1;
		}
		else if (key == 77) {
			B = B - 0.1;
		}		
		else if (key == 'w') {
			cx = cx + 1 * sin(B);
			cz = cz + 1 * cos(B);
		}
		else if (key == 's') {
			cx = cx - 1 * sin(B);
			cz = cz - 1 * cos(B);
		}
		else if (key == 'a') {
			cx = cx + 1 * cos(B);
			cz = cz + 1 * sin(B);
		}
		else if (key == 'd') {
			cx = cx - 1 * cos(B);
			cz = cz - 1 * sin(B);
		}
		else if (key == 'b') {
			cy = cy + 1;
		}
		else if (key == ' ') {
			cy = cy - 1;
		}
	}
}
void calculateFace(float cubeX, float cubeY, float cubeZ, int ch) {
	int i = cubeX - cx,
		j = cubeY - cy,
		k = cubeZ - cz;
	x = (j * sin(A) * sin(B) * cos(C)) - (k * cos(A) * sin(B) * cos(C)) + (j * cos(A) * sin(C)) + (k * sin(A) * sin(C)) + (i * cos(B) * cos(C));
	y = (j * cos(A) * cos(C)) + (k * sin(A) * cos(C)) - (j * sin(A) * sin(B) * sin(C)) + (k * cos(A) * sin(B) * sin(C)) - (i * cos(B) * sin(C));
	z = (k * cos(A) * cos(B)) - (j * sin(A) * cos(B)) + (i * sin(B));
	float depth = 1 / z;
	int px = ((screen_x / 2) - x * depth * FOV * 2),
		py = ((screen_y / 2) - y * depth * FOV);
	int pid = (px + (py * screen_x));
	if (pid > 0 && pid < screen_x * screen_y && z > 0 && screen_x > px && px > 0) {
		if (depth > zbuf[pid]) {
			zbuf[pid] = depth;
			buf[pid] = ch;
		}
	}
}
void render() {
	system("cls");
	cout << "'wasd' to move\n";
	int k = 0;
	for (int i = 0; i < screen_y; i++) {
		ostringstream line;
		for (int j = 0; j < screen_x; j++) {
			line << buf[k];
			k++;
		}
		cout << line.str() << "\n";
	}

}
void calculateCube(float x, float y, float z) {
	for (float Xcube = -cubeLength; Xcube < cubeLength; Xcube = Xcube + stepSize) {
		for (float Ycube = -cubeLength; Ycube < cubeLength; Ycube = Ycube + stepSize) {
			xp = Xcube;
			yp = Ycube;
			zp = cubeLength;
			calculateFace(x + xp, y + yp, z - zp, '.');
			calculateFace(x + xp, y + yp, z + zp, '!');
			calculateFace(x + xp, y - zp, z + yp, '"');
			calculateFace(x + xp, y + zp, z + yp, 'c');
			calculateFace(x - zp, y + xp, z + yp, '#');
			calculateFace(x + zp, y + xp, z + yp, '-');
		}
	}
}
void main() {
	//cout << "adjust the screen resolution to your liking, usually around (x=160, y=50)\nX res: ";
	//cin >> screen_x;
	//cout << "\nY res: ";
	//cin >> screen_y;
	while (true) {
		clearSD();
		//for (int l = 0; l < cubes.size()/3 + 1; l = l + 3) {
			//calculateCube(cubes[l] * cubeLength * 2, cubes[l + 1] * cubeLength * 2, cubes[l + 2] * cubeLength * 2);
		//}
		calculateCube(0, 0, 0);
		calculateCube(0, 70, 0);
		render();
		//system("cls");for (int h = 0; h < buf.size(); h++) {cout << buf[h] << " ";}
		testMovements();
		tick();
	}
}