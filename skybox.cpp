
#include "skybox.h"

Skybox::Skybox()
{
	boxSize = 550;
	texture[0].load("assets/textures/sky/sky_top.jpg");
	texture[1].load("assets/textures/sky/sky_side.jpg");
	texture[2].load("assets/textures/sky/sky_bottom.jpg");
}

void Skybox::render(sf::Vector3i position)
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTranslatef(position.x,position.y,position.z);
	glColor3f(1,1,1); // CYAN
	texture[0].bind();
	glBegin(GL_QUADS); // CREATE LARGE CUBE
	// TOP FACE
	glTexCoord2f(0,0); glVertex3f(-boxSize,boxSize,-boxSize);
	glTexCoord2f(1,0); glVertex3f( boxSize,boxSize,-boxSize);
	glTexCoord2f(1,1); glVertex3f( boxSize,boxSize, boxSize);
	glTexCoord2f(0,1); glVertex3f(-boxSize,boxSize, boxSize);
	glEnd();
	texture[2].bind();
	glBegin(GL_QUADS); // CREATE LARGE CUBE
	// BOTTOM FACE
	glTexCoord2f(0,1); glVertex3f(-boxSize,-boxSize, boxSize);
	glTexCoord2f(1,1); glVertex3f( boxSize,-boxSize, boxSize);
	glTexCoord2f(1,0); glVertex3f( boxSize,-boxSize,-boxSize);
	glTexCoord2f(0,0); glVertex3f(-boxSize,-boxSize,-boxSize);
	glEnd();
	texture[1].bind();
	glBegin(GL_QUADS); // CREATE LARGE CUBE
	// LEFT FACE
	glTexCoord2f(1,0); glVertex3f(-boxSize, boxSize, boxSize);
	glTexCoord2f(0,1); glVertex3f(-boxSize,-boxSize, boxSize);
	glTexCoord2f(1,1); glVertex3f(-boxSize,-boxSize,-boxSize);
	glTexCoord2f(0,0); glVertex3f(-boxSize, boxSize,-boxSize);
	// RIGHT 
	glTexCoord2f(1,0); glVertex3f(boxSize, boxSize,-boxSize);
	glTexCoord2f(0,1); glVertex3f(boxSize,-boxSize,-boxSize);
	glTexCoord2f(1,1); glVertex3f(boxSize,-boxSize, boxSize);
	glTexCoord2f(0,0); glVertex3f(boxSize, boxSize, boxSize);
	// FRONT
	glTexCoord2f(1,1); glVertex3f( boxSize,-boxSize,boxSize);
	glTexCoord2f(0,1); glVertex3f(-boxSize,-boxSize,boxSize);
	glTexCoord2f(1,0); glVertex3f(-boxSize, boxSize,boxSize);
	glTexCoord2f(0,0); glVertex3f( boxSize, boxSize,boxSize);
	// BACK
	glTexCoord2f(1,1); glVertex3f(-boxSize,-boxSize,-boxSize);
	glTexCoord2f(0,1); glVertex3f( boxSize,-boxSize,-boxSize);
	glTexCoord2f(1,0); glVertex3f( boxSize, boxSize,-boxSize);
	glTexCoord2f(0,0); glVertex3f(-boxSize, boxSize,-boxSize);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Skybox::setSize(int size)
{
	if (size <= 0) return;
	boxSize = size;
}

int Skybox::getSize()
{
	return boxSize;
}

int Skybox::getDefaultSize()
{
	return 550;
}
