#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

float A, B, C;

const float cubeWidth = 18.0f;
int width = 80, height = 24;
float zBuffer[160 * 44];
char buffer[160 * 44];
const int backgroundASCIICode = ' ';
const int distanceFromCam = 100;
const float K1 = 40.0f;

const float incrementSpeed = 0.6f;


float calculateX(int i, int j, int k) {
  return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
         j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}


float calculateY(int i, int j, int k) {
  return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
         j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
         i * cos(B) * sin(C);
}


float calculateZ(int i, int j, int k) {
  return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}


void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch) {
  float x = calculateX(cubeX, cubeY, cubeZ);
  float y = calculateY(cubeX, cubeY, cubeZ);
  float z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

  float ooz = 1 / z;

  int xp = (int)(width / 2 + K1 * ooz * x * 2);
  int yp = (int)(height / 2 + K1 * ooz * y);

  int idx = xp + yp * width;
  if (idx >= 0 && idx < width * height) {
    if (ooz > zBuffer[idx]) {
      zBuffer[idx] = ooz;
      buffer[idx] = ch;
    }
  }
}


int main() {
  printf("\x1b[2J");
  while (1) {
    memset(buffer, backgroundASCIICode, width * height);
    memset(zBuffer, 0.0f, width * height * 4);

    for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {
      for (float cubeY = -cubeWidth; cubeY < cubeWidth;
           cubeY += incrementSpeed) {
        calculateForSurface(cubeX, cubeY, -cubeWidth, '@');
        calculateForSurface(cubeWidth, cubeY, cubeX, '$');
        calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');
        calculateForSurface(-cubeX, cubeY, cubeWidth, '#');
        calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
        calculateForSurface(cubeX, cubeWidth, cubeY, '+');
      }
    }
    printf("\x1b[H");
    for (int k = 0; k < width * height; k++) {
      putchar(k % width ? buffer[k] : 10);
    }

    A += 0.05f;
    B += 0.05f;
    C += 0.01f;
    usleep(8000 * 2);
  }

  return 0;
}
