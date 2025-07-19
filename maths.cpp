
#include "maths.h"

float Math::degtorad(float val)
{
	return (val) / 180 * PI;
}

float Math::sqr(float val)
{
	return (val) * (val);
}

float Math::min(float a, float b)
{
	return (a < b) ? a : b;
}

float Math::max(float a, float b)
{
	return (a > b) ? a : b;
}

float Math::clamp(float val, float a, float b)
{
	return min(max(val,a),b);
}

void Math::initRandom()
{
	srand(static_cast<unsigned>(time(NULL)));
}

float Math::random(float x)
{
	return ((float) rand() / ((float) RAND_MAX / x));
}

float Math::randomRange(float a, float b)
{
	return ((float) rand() / ((float) RAND_MAX / (a - b))) + b;
}