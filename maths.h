#ifndef _VOX2_MATH_H_
#define _VOX2_MATH_H_

#include <ctime>
#include <cmath>
#include <random>

class Math
{
public:
	static constexpr float PI = 3.141518927;
	static float degtorad(float val);
	static float sqr(float val);
	static float min(float a, float b);
	static float max(float a, float b);
	static float clamp(float val, float a, float b);
	static void initRandom();
	static float random(float x);
	static float randomRange(float a, float b);
};

#endif /* _VOX2_MATH_H_ */