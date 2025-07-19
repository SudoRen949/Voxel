#ifndef _VOX2_RAYCAST_H_
#define _VOX2_RAYCAST_H_

#include <GL/gl.h>
#include <iostream>
#include <SFML/System.hpp>

#include "maths.h"
#include "grid.h"
#include "defines.h"
#include "types.h"

class Raycast
{
public:
	Raycast();
	void setCamera(sf::Vector3f from, sf::Vector3f to, sf::Vector3f up);
	void setPerspective(float angle, float aspect, float near, float far);
	void setReachRange(int range);
	void calculateView();
	void drawSelector();
	bool isHit(Grid grid);
	bool surfaceHidden(sf::Vector3f position, float radius);
	sf::Vector3i getRayAt();
	sf::Vector3i getRayFrom();
private:
	int reachRange;
	float angle, aspect, near, far;
	sf::Vector3f from, to, up;
	sf::Vector3f cross;
	sf::Vector2f fcullTan, fcullSec;
	sf::Vector3f positionFrom, positionTo;
};

#endif /* _VOX2_RAYCAST_H_ */