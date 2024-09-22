#include<iostream>
#include<sstream>
#include<conio.h>
#include<chrono>
#include<math.h>
#include<vector>
#include<thread>

using namespace std;
const float pi = 3.14159;
const int torusRadius = 40,
torusThickness = 15;
const float subdA = 0.01,
			subdB = 0.05;
const int distanceFromCamera = 275,
		  vFactor = 90,
		  scr_x = 160, 
		  scr_y = 45;
float A = 1 * pi / 2, B = 0, C = 0;
float VA, VB, VC;
vector<float> glv{ 10, 1 ,0 };
vector<float> zbuffer;
vector<float> buffer;
vector<char> gradTable{ '█', '$', '&', '@', '%', '#', '*', '=', '+', '~', ':', '-', '`', '.', ' '};

void wait() {
	this_thread::sleep_for(10ms);
}
float transform_x(float i, float j, float k) {
	return (j * sin(A) * sin(B) * cos(C))
		- (k * cos(A) * sin(B) * cos(C)) 
		+ (j * cos(A) * sin(C)) 
		+ (k * sin(A) * sin(C)) 
		+ (i * cos(B) * cos(C));
}
float transform_y(float i, float j, float k) {
	return (j * cos(A) * cos(C)) 
		+ (k * sin(A) * cos(C)) 
		- (j * sin(A) * sin(B) * sin(C)) 
		+ (k * cos(A) * sin(B) * sin(C)) 
		- (i * cos(B) * sin(C));
}
float transform_z(float i, float j, float k) {
	return (k * cos(A) * cos(B)) 
		- (j * sin(A) * cos(B)) 
		+ (i * sin(B) + distanceFromCamera);
}
void normalize(float x, float y, float z, float &xn, float &yn, float &zn) {
	float magnitude = sqrt(x * x + y * y + z * z);
	if (magnitude == 0) {
		xn = 0;
		yn = 1;
		zn = 0;
		return;
	}
	xn = x / magnitude;
	yn = y / magnitude;
	zn = y / magnitude;
}
float distanceBetween(float x0, float y0, float z0, float x1, float y1, float z1) {
	float dx = x1 - x0,
		dy = y1 - y0,
		dz = z1 - z0;
	return sqrt(dx * dx + dy * dy + dz * dz);
}
void storePointVector(float x_fin, float y_fin, float z_fin, float x_cen, float y_cen, float z_cen) {
	float x = transform_x(x_fin, y_fin, z_fin),
		y = transform_y(x_fin, y_fin, z_fin),
		z = transform_z(x_fin, y_fin, z_fin),
		x_Tcen = transform_x(x_cen, y_cen, z_cen),
		y_Tcen = transform_y(x_cen, y_cen, z_cen),
		z_Tcen = transform_z(x_cen, y_cen, z_cen);
	float xn, yn, zn;
	float depth = 1 / z;
	normalize(x - x_Tcen, y - y_Tcen, z - z_Tcen, xn, yn, zn);
	int screenPos_x = round((scr_x / 2) - x * depth * vFactor * 2),
		screenPos_y = round((scr_y / 2) - y * depth * vFactor);
	int screenPos = screenPos_x + screenPos_y * scr_x;
	if (screenPos > 0 && screenPos < scr_x * scr_y && screenPos_x > 0 && screenPos_x < scr_x) {
		if (zbuffer[screenPos] < depth) {
			zbuffer[screenPos] = depth;
			buffer[screenPos] = distanceBetween(xn, yn, zn, glv[0], glv[1], glv[2]);
		}
	}
}
void overflowAngle(float& angle) {
	if (angle > 2 * pi) {
		angle = 0;
	}
}
void testMovements() {
	A = A + 0.1 * VA;
	B = B + 0.1 * VB;
	VA = VA / 1.001;
	VB = VB / 1.001;
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
void main() {
	normalize(glv[0], glv[1], glv[2], glv[0], glv[1], glv[2]);
	while (true) {
		testMovements();
		zbuffer.clear();
		buffer.clear();
		for (int i = 0; i < scr_x * scr_y * 2; i++) {
			zbuffer.push_back(0.0);
			buffer.push_back(3.0);
		}
		wait();
		for (float slice = 0; slice < 2 * pi; slice = slice + subdA) {
			storePointVector(torusRadius * cos(slice), 0, torusRadius * sin(slice), 0, 0, 0);
			float x_cen = torusRadius * cos(slice),
				z_cen = torusRadius * sin(slice),
				y_cen = 0;
			for (float splice = 0; splice < 2 * pi; splice = splice + subdB) {
				float x_fin = (torusRadius + torusThickness * cos(splice)) * cos(slice),
					y_fin = torusThickness * sin(splice),
					z_fin = (torusRadius + torusThickness * cos(splice)) * sin(slice);
				//if ((slice == 0) or ((slice > pi-subdA) and (slice < pi+subdA)) or ((slice > pi / 2 - subdA) and (slice < pi / 2 + subdA))) {
					storePointVector(x_fin, y_fin, z_fin, x_cen, y_cen, z_cen);
				//}
			}
		}
		system("cls");
		cout << "I made a donut cus im hungry\nu can use 'wasd' to make the donut dance\n";
		unsigned int k = 0;
		for (unsigned int i = 0; i < scr_y; i++) {
			ostringstream line;
			for (unsigned int j = 0; j < scr_x; j++) {
				if (ceil(buffer[k] / 2) > 1) {
					line << ' ';
				}
				else {
					//line << gradTable[round((buffer[k] / 2) * 8)];
					line << gradTable[ceil((buffer[k] / 2) * 15)];
				}
				k++;
			}
			cout << line.str() << "\n";
		}
		//A = A + 0.1;
		//B = B + 0.05;
		//C = C + 0.05;
		overflowAngle(A);
		overflowAngle(B);
		overflowAngle(C);
	}
}