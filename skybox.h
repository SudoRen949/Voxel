#ifndef _VOX2_SKYBOX_H_
#define _VOX2_SKYBOX_H_

#include <SFML/Graphics.hpp>

#include "texture.h"
#include "chunk.h"

class Skybox
{
public:
	Skybox();
	void render(sf::Vector3i position);
	void setSize(int size);
	int getSize();
	int getDefaultSize();
private:
	int boxSize;
	Texture texture[3];
};

#endif /* _VOX2_SKYBOX_H_ */