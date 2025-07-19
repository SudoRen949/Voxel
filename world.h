#ifndef _VOX2_WORLD_H_
#define _VOX2_WORLD_H_

#include "terrain.h"
#include "noise.h"
#include "raycast.h"
#include "defines.h"
#include "skybox.h"
#include "clouds.h"

class World
{
public:
	World(int seed = 0);
	~World();
	void render(sf::Vector3f position, Raycast raycast);
	void setRenderDistance(int distance);
	int getRenderDistance();
	Grid getMap();
	Chunk* getChunk();
private:
	int renderDistance;
	Grid worldMap;
	Chunk* chunk;
	Noise* perlinNoise;
	Skybox skybox;
	Clouds clouds;
};

#endif /* _VOX2_WORLD_H_ */