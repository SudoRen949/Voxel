#include "clouds.h"

Clouds::Clouds()
{
	cloudsSize = 5000;
	texture.load("assets/textures/sky/clouds.png");
}

void Clouds::render(sf::Vector3i position)
{
	float yheight = Math::clamp(yheight,position.y,180*BLOCK_SIZE);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glPushMatrix();
	glTranslatef(position.x-(cloudsSize/2),yheight,position.z-(cloudsSize/2));
	texture.bind();
	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex3f(			0,cloudsSize/5,		 0);
		glTexCoord2f(1,0); glVertex3f(cloudsSize,cloudsSize/5,		 0);
		glTexCoord2f(1,1); glVertex3f(cloudsSize,cloudsSize/5,cloudsSize);
		glTexCoord2f(0,1); glVertex3f(			0,cloudsSize/5,cloudsSize);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
}

