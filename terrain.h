#ifndef _VOX2_TERRAIN_H_
#define _VOX2_TERRAIN_H_

#include <iostream>
#include <SFML/System.hpp>

#include "types.h"
#include "chunk.h"
#include "grid.h"
#include "defines.h"

class Terrain
{
public:
	static void createLand(Chunk *chunk, Grid grid, int noiseHeight, int x, int y, int z);
	static void createCave(Chunk *chunk, Grid grid, int noiseHeight, int x, int y, int z);
	static void createSea(Chunk *chunk, Grid grid, int noiseHeight, int x, int y, int z);
	static void createTrees(Chunk *chunk, Grid grid, int noiseHeight, int x, int y, int z);
};

#endif /* _VOX2_TERRAIN_H_ */