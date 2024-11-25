#include <math.h>
#include "Tool.h"

float Measurement(float pos1, float pos2)
{
	return pos2 - pos1;
};

float Length(float vectorX, float vectorY) {
	return sqrtf(powf(vectorX, 2.0f) + powf(vectorY, 2.0f));
};

float CrossProduct(float x1, float y1, float x2, float y2) {
	return x1 * y2 - y1 * x2;
};