
#include "world.h"

World::World(int seed)
{
	int genSeed = 0;
	renderDistance = 3;
	worldMap.createGrid(sf::Vector3i(WORLD_SIZE_H,WORLD_SIZE_V,WORLD_SIZE_H),8); // we only store 0 - 255 blocks
	chunk = new Chunk(worldMap);
	Math::initRandom();
	genSeed = Math::randomRange(100000,9999999);
	perlinNoise = new Noise(genSeed);
	/** Generate World */
	int height = 0;
	for (int x = 0; x < worldMap.getSize().x; x++)
	for (int z = 0; z < worldMap.getSize().z; z++)
	{
		height = static_cast<int>(perlinNoise->getHeight(x,z) * 32) + MAX_NOISE_HEIGHT;
		Terrain::createLand(chunk,worldMap,height,x,0,z);
	}
	for (int x = 0; x < worldMap.getSize().x; x++)
	for (int z = 0; z < worldMap.getSize().z; z++)
	{
		height = static_cast<int>(perlinNoise->getHeight(x,z) * 32) + MAX_NOISE_HEIGHT;
		Terrain::createCave(chunk,worldMap,height,x,0,z);
	}
	for (int x = 0; x < worldMap.getSize().x; x++)
	for (int z = 0; z < worldMap.getSize().z; z++)
	{
		height = static_cast<int>(perlinNoise->getHeight(x,z) * 32) + MAX_NOISE_HEIGHT;
		Terrain::createSea(chunk,worldMap,height,x,0,z);
	}
	for (int x = 0; x < worldMap.getSize().x; x++)
	for (int z = 0; z < worldMap.getSize().z; z++)
	{
		height = static_cast<int>(perlinNoise->getHeight(x,z) * 32) + MAX_NOISE_HEIGHT;
		Terrain::createTrees(chunk,worldMap,height,x,0,z);
	}
}

World::~World()
{
	delete chunk;
	delete perlinNoise;
	worldMap.destroyGrid();
}

void World::render(sf::Vector3f position, Raycast raycast)
{
	skybox.setSize(skybox.getDefaultSize()*renderDistance);
	skybox.render(sf::Vector3i(position.x,position.y,position.z));
	clouds.render(sf::Vector3i(position.x,position.y,position.z));
	float size = (float) CHUNK_SIZE * (float) BLOCK_SIZE;
	float radius = sqrt(Math::sqr(size/2)*3);
	for (int y = (ceil(position.y / CHUNK_SIZE) / BLOCK_SIZE) - renderDistance; y <= (ceil(position.y / CHUNK_SIZE) / BLOCK_SIZE) + (renderDistance+2); y++)
	for (int x = (ceil(position.x / CHUNK_SIZE) / BLOCK_SIZE) - renderDistance; x <= (ceil(position.x / CHUNK_SIZE) / BLOCK_SIZE) + renderDistance; x++)
	for (int z = (ceil(position.z / CHUNK_SIZE) / BLOCK_SIZE) - renderDistance; z <= (ceil(position.z / CHUNK_SIZE) / BLOCK_SIZE) + renderDistance; z++)
	{
		if (!raycast.surfaceHidden(sf::Vector3f(x*size+size/2,y*size+size/2,z*size+size/2),radius)) continue;
		chunk->renderModel(worldMap,sf::Vector3i(x,y,z),true);
	}
}

int World::getRenderDistance()
{
	return renderDistance;
}

Grid World::getMap()
{
	return worldMap;
}

Chunk* World::getChunk()
{
	return chunk;
}
