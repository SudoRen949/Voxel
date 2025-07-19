
#include "camera.h"

Camera::Camera()
{
	msec = sec = 0;
	mouseSensitivity = 6;
	fov = 90.0f;
	near = 2.0f;
	far = 400.0f;
	dimension = sf::Vector3f(6,18,6);
	speed = 10;
	cSpeed = speed;
	friction = 0.80;
	gravity = 15;
	camDistance = 1;
	texture.loadFromFile("assets/pointer.png");
	visionTexture[0].loadFromFile("assets/textures/vision/water.png");
	visionTexture[1].loadFromFile("assets/textures/vision/lava.png");
	cursor.setTexture(&texture);
	cursor.setSize(sf::Vector2f(texture.getSize().x,texture.getSize().y));
	cursor.setOrigin(cursor.getSize().x/2,cursor.getSize().y/2);
	for (int i = 0; i < NUM_VISION; i++)
	{
		vision[i].setTexture(&visionTexture[i]);
		vision[i].setPosition(0,0);
	}
}

void Camera::run(World *world, sf::RenderWindow *window, float dtime)
{
	if (!window->hasFocus()) return;
	blockPosition = sf::Vector3i(floor(position.x/BLOCK_SIZE),floor(position.y/BLOCK_SIZE),floor(position.z/BLOCK_SIZE));
	
	/** -- Keys -- */
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		// jump
		if (onGround && !onWater && !onLava)
		{
			if (!underWater && !underLava) 
				direction.y = 25;
		}
		// swim
		if (!onGround)
		{
			if (underWater) direction.y += 1;
			else if (underLava) direction.y += 0.85;
			else if (onWater) direction.y += 1;
			else if (onLava) direction.y += 0.85;
			else if (world->getMap().getValueAt(sf::Vector3i(blockPosition.x,blockPosition.y-1,blockPosition.z)) == Types::WATER) direction.y -= 5;
			else if (world->getMap().getValueAt(sf::Vector3i(blockPosition.x,blockPosition.y-1,blockPosition.z)) == Types::LAVA) direction.y -= 4;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		if (onGround && !onWater && !onLava)
		{
			if (!underWater && !underLava) speed = cSpeed / 3;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		direction.x = -sin(Math::degtorad(mouseAngle.x)) * speed;
		direction.z = -cos(Math::degtorad(mouseAngle.x)) * speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		direction.x =  sin(Math::degtorad(mouseAngle.x)) * speed;
		direction.z =  cos(Math::degtorad(mouseAngle.x)) * speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		direction.x =  sin(Math::degtorad(mouseAngle.x+90)) * speed;
		direction.z =  cos(Math::degtorad(mouseAngle.x+90)) * speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		direction.x =  sin(Math::degtorad(mouseAngle.x-90)) * speed;
		direction.z =  cos(Math::degtorad(mouseAngle.x-90)) * speed;
	}
	
	/** -- Physics -- */
	collideAt(world->getMap(),position,dtime);
	if (onWater || underWater) speed = cSpeed - 6;
	else if (onLava || underLava) speed = cSpeed - 7;
	else speed = cSpeed;
	position.x += direction.x * dtime;
	collide(world->getMap(),sf::Vector3f(direction.x,0,0));
	position.y += direction.y * dtime;
	collide(world->getMap(),sf::Vector3f(0,direction.y,0));
	position.z += direction.z * dtime;
	collide(world->getMap(),sf::Vector3f(0,0,direction.z));
	direction.x *= friction;
	direction.z *= friction;
	
	/** -- Camera View -- */
	window->setMouseCursorVisible(false);
	window->setMouseCursorGrabbed(true);
	mousePosition.x = window->getSize().x / 2;
	mousePosition.y = window->getSize().y / 2;
	mouseAngle.x += float(mousePosition.x - sf::Mouse::getPosition(*window).x) / mouseSensitivity;
	mouseAngle.y += float(mousePosition.y - sf::Mouse::getPosition(*window).y) / mouseSensitivity;
	mouseAngle.y = Math::clamp(mouseAngle.y,-89,89);
	sf::Mouse::setPosition(mousePosition,*window);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glDepthFunc(GL_LEQUAL);
	glAlphaFunc(GL_GEQUAL,0.5f);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
	viewPosition = sf::Vector3f(position.x,position.y+(dimension.y/2),position.z);
	viewEye = sf::Vector3f(
		viewPosition.x - sin(Math::degtorad(mouseAngle.x)),
		viewPosition.y + tan(Math::degtorad(mouseAngle.y)),
		viewPosition.z - cos(Math::degtorad(mouseAngle.x))
	);
	viewUp = sf::Vector3f(0,1,0);
	far = (float) renderDistance * 980.0f;
	aspect = (float)window->getSize().x / (float)window->getSize().y;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov,aspect,near,far);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,camDistance);
	gluLookAt(viewPosition.x,viewPosition.y,viewPosition.z,viewEye.x,viewEye.y,viewEye.z,viewUp.x,viewUp.y,viewUp.z);
	
	/** Raycast */
	raycast.setCamera(viewPosition,viewEye,viewUp);
	raycast.setPerspective(fov,aspect,near,far);
	raycast.calculateView();
	if (raycast.isHit(world->getMap()))
	{
		raycast.drawSelector();
		// player position (current)
		float xx = floor((position.x - dimension.x) / BLOCK_SIZE);
		float zz = floor((position.z - dimension.z) / BLOCK_SIZE);
		float yy = floor((position.y - (dimension.y/10)) / BLOCK_SIZE);
		// raycast position (previous)
		float rx = (float) raycast.getRayFrom().x;
		float rz = (float) raycast.getRayFrom().z;
		float ry = (float) raycast.getRayFrom().y;
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && 
			!sf::Mouse::isButtonPressed(sf::Mouse::Right) && mousePressed)
		{
			mousePressed = false;
		}
		if (buildOn(sf::Vector3f(xx,yy,zz),sf::Vector3f(rx,ry,rz)))
		{	
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !mousePressed)
			{
				world->getChunk()->addBlock(world->getMap(),raycast.getRayFrom(),Types::GLASS);
				mousePressed = true;
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mousePressed)
		{
			world->getChunk()->removeBlock(world->getMap(),raycast.getRayAt());
			mousePressed = true;
		}
	}
}

void Camera::draw(World *world, sf::RenderWindow *window)
{
	/** 2D Drawing */
	window->pushGLStates();
	cursor.setPosition(mousePosition.x,mousePosition.y);
	for (int i = 0; i < NUM_VISION; i++)
		vision[i].setSize(sf::Vector2f(window->getSize().x,window->getSize().y));
	if (underWater) window->draw(vision[0]);
	if (underLava) window->draw(vision[1]);
	window->draw(cursor);
	window->popGLStates();
}

void Camera::collide(Grid grid, sf::Vector3f cdirection)
{
	if (position.y <= -30000) { position.y = -30000; direction.y = 0; }
	if (position.x - dimension.x <= 0) position.x = dimension.x;
	if (position.z - dimension.z <= 0) position.z = dimension.z;
	if (position.x + dimension.x > grid.getSize().x * BLOCK_SIZE) position.x = (grid.getSize().x * BLOCK_SIZE) - dimension.x;
	if (position.z + dimension.z > grid.getSize().z * BLOCK_SIZE) position.z = (grid.getSize().z * BLOCK_SIZE) - dimension.z;
	// check if inside of block
	for (int y = (position.y - dimension.y) / BLOCK_SIZE; y < (position.y + dimension.y/1.5) / BLOCK_SIZE; y++)
	for (int x = (position.x - dimension.x) / BLOCK_SIZE; x < (position.x + dimension.x    ) / BLOCK_SIZE; x++)
	for (int z = (position.z - dimension.z) / BLOCK_SIZE; z < (position.z + dimension.z    ) / BLOCK_SIZE; z++)
	{
		if (Types::checkSolids(grid.getValueAt(sf::Vector3i(x,y,z))))
		{
			if (cdirection.y < 0)
			{
				position.y = y * BLOCK_SIZE + BLOCK_SIZE + dimension.y;
				direction.y = 0;
				onGround = true;
			}
			if (cdirection.y > 0)
			{
				position.y = y * BLOCK_SIZE - (dimension.y / 1.5);
				direction.y = 0;
				onGround = false;
			}
			if (cdirection.x < 0) { position.x = x * BLOCK_SIZE + BLOCK_SIZE + dimension.x; direction.x = 0; }
			if (cdirection.x > 0) { position.x = x * BLOCK_SIZE - dimension.x; direction.x = 0; }
			if (cdirection.z < 0) { position.z = z * BLOCK_SIZE + BLOCK_SIZE + dimension.z; direction.z = 0; }
			if (cdirection.z > 0) { position.z = z * BLOCK_SIZE - dimension.z; direction.z = 0; }
		}
	}
}

void Camera::setPosition(sf::Vector3f position)
{
	this->position = sf::Vector3f(position.x * BLOCK_SIZE, position.y * BLOCK_SIZE, position.z * BLOCK_SIZE);
}

void Camera::setRenderDistance(int distance)
{
	renderDistance = distance;
}

Raycast Camera::getRaycaster()
{
	return raycast;
}

sf::Vector3f Camera::getPosition()
{
	return position;
}



/** PRIVATE FUNCTIONS */


void Camera::collideAt(Grid grid, sf::Vector3f pos, float dtime)
{
	int X = pos.x / BLOCK_SIZE;
	int Y = pos.y / BLOCK_SIZE;
	int Z = pos.z / BLOCK_SIZE;
	onLava = onWater = false;
	underLava = underWater = false;
	if ((grid.getValueAt(sf::Vector3i(X,Y+1,Z)) == Types::WATER &&		// Check if on lava....
		 grid.getValueAt(sf::Vector3i(X,Y-1,Z)) == Types::WATER) ||
		 grid.getValueAt(sf::Vector3i(X,Y+1,Z)) == Types::WATER ||
		 grid.getValueAt(sf::Vector3i(X,Y,Z)) == Types::WATER)
	{
		underWater = true;
	}
	else if ((grid.getValueAt(sf::Vector3i(X,Y+1,Z)) == Types::LAVA &&		// Check if under lava....
			  grid.getValueAt(sf::Vector3i(X,Y-1,Z)) == Types::LAVA) ||
			  grid.getValueAt(sf::Vector3i(X,Y+1,Z)) == Types::LAVA ||
			  grid.getValueAt(sf::Vector3i(X,Y,Z)) == Types::LAVA)
	{
		underLava = true;
	}
	else if (grid.getValueAt(sf::Vector3i(X,Y-1,Z)) == Types::WATER) onWater = true;
	else if (grid.getValueAt(sf::Vector3i(X,Y-1,Z)) == Types::LAVA) onLava = true;
	
	/** Update Gravity */
	if (onWater || underWater)
	{
		gravity = 1;
		direction.y *= 0.95;
	}
	else if (onLava || underLava)
	{
		gravity = 0.85;
		direction.y *= 0.95;
	}
	else gravity = 15;
	if (!onGround) direction.y -= gravity * dtime;
	onGround = false;
}

bool Camera::buildOn(sf::Vector3f pos1, sf::Vector3f pos2)
{
	int v = sqrt( Math::sqr(pos2.x - pos1.x) + Math::sqr(pos2.y - pos1.y) + Math::sqr(pos2.z - pos1.z) );
	if (v > 1) return true;
	return false;
}
