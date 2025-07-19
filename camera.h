#ifndef _VOX2_CAMERA_H_
#define _VOX2_CAMERA_H_

#define NUM_VISION 2

#include <GL/glu.h>
#include <SFML/Graphics.hpp>

#include "grid.h"
#include "maths.h"
#include "defines.h"
#include "raycast.h"
#include "world.h"

class Camera
{
public:
	Camera();
	void run(World *world, sf::RenderWindow *window, float dtime);
	void draw(World *world, sf::RenderWindow *window);
	void collide(Grid grid, sf::Vector3f cdirection);
	void setPosition(sf::Vector3f position);
	void setRenderDistance(int distance);
	Raycast getRaycaster();
	sf::Vector3f getPosition();
private:
	int camDistance, renderDistance;
	int msec, sec;
	bool onGround, mousePressed;
	bool onWater, onLava, underWater, underLava;
	float aspect;
	float near, far, speed, cSpeed, gravity;
	float friction, fov, mouseSensitivity;
	Raycast raycast;
	sf::Vector3f position, direction, dimension;
	sf::Vector2f mouseAngle;
	sf::Vector3f viewPosition,viewEye,viewUp;
	sf::Vector2i mousePosition;
	sf::Vector3i blockPosition;
	sf::RectangleShape cursor, vision[NUM_VISION];
	sf::Texture texture, visionTexture[NUM_VISION];
	void collideAt(Grid grid, sf::Vector3f pos, float dtime);
	bool buildOn(sf::Vector3f pos1, sf::Vector3f pos2);
};

#endif /* _VOX2_CAMERA_H_ */
