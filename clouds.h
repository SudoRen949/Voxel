#ifndef _VOX2_CLOUDS_H_
#define _VOX2_CLOUDS_H_

#include <GL/glu.h>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "texture.h"
#include "defines.h"
#include "maths.h"

class Clouds
{
public:
	Clouds();
	void render(sf::Vector3i position);
private:
	int cloudsSize;
	sf::Vector3i position;
	Texture texture;
};

#endif /* _VOX2_CLOUDS_H_ */