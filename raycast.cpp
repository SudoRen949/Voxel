#include "raycast.h"

Raycast::Raycast()
{
	angle 	= 0.0f;
	aspect 	= 0.0f;
	near 	= 0.0f;
	far 	= 0.0f;
	from 	= sf::Vector3f(0,0,0);
	to 		= sf::Vector3f(0,0,0);
	up 		= sf::Vector3f(0,0,0);
	reachRange = 80;
}

void Raycast::setCamera(sf::Vector3f from, sf::Vector3f to, sf::Vector3f up)
{
	this->from = from;
	this->to = to - this->from;
	this->up = up;
}

void Raycast::setPerspective(float angle, float aspect, float near, float far)
{
	this->angle = angle;
	this->aspect = aspect;
	this->near = near;
	this->far = far;
}

void Raycast::setReachRange(int range)
{
	reachRange = range;
}

void Raycast::calculateView()
{
	float distance = sqrt( Math::sqr(this->to.x) + Math::sqr(this->to.y) + Math::sqr(this->to.z) );
	this->to /= distance;
	distance = (this->up.x * this->to.x) + (this->up.y * this->to.y) + (this->up.z * this->to.z);
	this->up.x -= distance * this->to.x;
	this->up.y -= distance * this->to.y;
	this->up.z -= distance * this->to.z;
	distance = sqrt( Math::sqr(this->up.x) + Math::sqr(this->up.y) + Math::sqr(this->up.z) );
	this->up /= distance;
	cross.x = (this->up.y * this->to.z) - (this->up.z * this->to.y);
	cross.y = (this->up.z * this->to.x) - (this->up.x * this->to.z);
	cross.z = (this->up.x * this->to.y) - (this->up.y * this->to.x);
	fcullTan.y = tan( Math::degtorad(this->angle) / 2 );
	fcullTan.x = fcullTan.y * this->aspect;
	fcullSec.x = sqrt( 1 + Math::sqr(fcullTan.x) );
	fcullSec.y = sqrt( 1 + Math::sqr(fcullTan.y) );
}

bool Raycast::isHit(Grid grid)
{
	for (int i = 0; i < reachRange; i++)
	{
		float x = floor( (from.x + to.x * i) / BLOCK_SIZE );
		float y = floor( (from.y + to.y * i) / BLOCK_SIZE );
		float z = floor( (from.z + to.z * i) / BLOCK_SIZE );
		if (y < 0) y = -1;
		int block = grid.getValueAt(sf::Vector3i(x,y,z));
		if ( x >= 0 && x <= grid.getSize().x-1 && z >= 0 && z <= grid.getSize().z-1)
		{
			if ( block != Types::AIR && !Types::checkLiquids(block) && y >= 0 && y <= grid.getSize().y-1 )
			{
				positionTo.x = floor( (from.x + to.x * i) / BLOCK_SIZE );
				positionTo.y = floor( (from.y + to.y * i) / BLOCK_SIZE );
				positionTo.z = floor( (from.z + to.z * i) / BLOCK_SIZE );
				i -= 1;
				positionFrom.x = floor( (from.x + to.x * i) / BLOCK_SIZE );
				positionFrom.y = floor( (from.y + to.y * i) / BLOCK_SIZE );
				positionFrom.z = floor( (from.z + to.z * i) / BLOCK_SIZE );
				return true;
			}
		}
	}
	return false;
}

void Raycast::drawSelector()
{
	glDisable(GL_CULL_FACE);
	glColor3f(0,0,0);
	glPushMatrix();
	glTranslatef(floor(positionTo.x)*BLOCK_SIZE,floor(positionTo.y)*BLOCK_SIZE,floor(positionTo.z)*BLOCK_SIZE);
    glBegin(GL_LINE_LOOP);
		glVertex3i(-1,BLOCK_SIZE+1,-1); //
		glVertex3i( BLOCK_SIZE+1,BLOCK_SIZE+1,-1);
		glVertex3i( BLOCK_SIZE+1,BLOCK_SIZE+1, BLOCK_SIZE+1);
		glVertex3i(-1,BLOCK_SIZE+1, BLOCK_SIZE+1);
		glVertex3i(-1,-1, BLOCK_SIZE+1); //
		glVertex3i( BLOCK_SIZE+1,-1, BLOCK_SIZE+1);
		glVertex3i( BLOCK_SIZE+1,-1,-1);
		glVertex3i(-1,-1,-1);
		glVertex3i(-1,-1, BLOCK_SIZE+1); //
		glVertex3i(-1,-1,-1);
		glVertex3i(-1, BLOCK_SIZE+1,-1);
		glVertex3i(-1, BLOCK_SIZE+1, BLOCK_SIZE+1);
		glVertex3i( BLOCK_SIZE+1,-1,-1); //
		glVertex3i( BLOCK_SIZE+1, BLOCK_SIZE+1,-1);
		glVertex3i( BLOCK_SIZE+1, BLOCK_SIZE+1, BLOCK_SIZE+1);
		glVertex3i( BLOCK_SIZE+1,-1, BLOCK_SIZE+1);
		glVertex3i( BLOCK_SIZE+1,-1, BLOCK_SIZE+1); //
		glVertex3i(-1,-1, BLOCK_SIZE+1);
		glVertex3i(-1, BLOCK_SIZE+1, BLOCK_SIZE+1);
		glVertex3i( BLOCK_SIZE+1, BLOCK_SIZE+1, BLOCK_SIZE+1);
		glVertex3i(-1,-1,-1); //
		glVertex3i( BLOCK_SIZE+1,-1,-1);
		glVertex3i( BLOCK_SIZE+1, BLOCK_SIZE+1,-1);
		glVertex3i(-1, BLOCK_SIZE+1,-1);
    glEnd();
	glPopMatrix();
	glEnable(GL_CULL_FACE);
}

bool Raycast::surfaceHidden(sf::Vector3f position, float radius)
{
	float x1 = position.x - from.x;
	float y1 = position.y - from.y;
	float z1 = position.z - from.z;
	float z2 = to.x * x1 + to.y * y1 + to.z * z1;
	if ( z2 < near - radius || z2 > (far/2) + radius) return false;
	float x2 = cross.x * x1 + cross.y * y1 + cross.z * z1;
	if ( abs(x2) > z2 * fcullTan.x + radius * fcullSec.x ) return false;
	float y2 = up.x * x1 + up.y * y1 + up.z * z1;
	if ( abs(y2) > z2 * fcullTan.y + radius * fcullSec.y ) return false;
	return true;
}

sf::Vector3i Raycast::getRayAt()
{
	return sf::Vector3i(positionTo.x,positionTo.y,positionTo.z);
}

sf::Vector3i Raycast::getRayFrom()
{
	return sf::Vector3i(positionFrom.x,positionFrom.y,positionFrom.z);
}
