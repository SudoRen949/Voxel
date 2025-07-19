
#include "terrain.h"

void Terrain::createLand(Chunk *chunk, Grid grid, int noiseHeight, int x, int y, int z)
{
	int dirtHeight = noiseHeight-Math::randomRange(2,4);
	int stoneHeight = Math::randomRange(3,7);
	for (int yy = 0; yy < noiseHeight; yy++)
	{
		if (yy == noiseHeight-1) chunk->addBlock(grid,sf::Vector3i(x,yy,z),Types::GRASS);
		if (yy < noiseHeight-1 && yy >= dirtHeight) chunk->addBlock(grid,sf::Vector3i(x,yy,z),Types::DIRT);
		if (yy < dirtHeight && yy >= stoneHeight) chunk->addBlock(grid,sf::Vector3i(x,yy,z),Types::STONE);
		if (yy < stoneHeight && yy >= 0) chunk->addBlock(grid,sf::Vector3i(x,yy,z),Types::BEDROCK);
	}
}

void Terrain::createCave(Chunk *chunk, Grid grid, int noiseHeight, int x, int y, int z)
{
	int seeds = Math::random(50);
	if (seeds == 5)
	{
		int X = Math::random(x), Z = Math::random(z);
		int rPY = Math::randomRange(15,noiseHeight+3);
		int wX = Math::randomRange(2,7), wZ = Math::randomRange(2,7);
		int deep = Math::randomRange(4,14);
		for (int xx = -wX; xx <= wX; xx++)
		for (int zz = -wZ; zz <= wZ; zz++)
		{
			for (int yy = rPY; yy > rPY-deep; yy--)
			{
				if ((xx == -wX && zz ==  wZ) ||
					(xx ==  wX && zz ==  wZ) ||
					(xx ==  wX && zz == -wZ) ||
					(xx == -wX && zz == -wZ)) continue;
				chunk->removeBlock(grid,sf::Vector3i(X+xx,yy,Z+zz));
			}
		}
	}
}

void Terrain::createSea(Chunk *chunk, Grid grid, int noiseHeight, int x, int y, int z)
{
	for (int yy = 50; yy < MAX_NOISE_HEIGHT - 5; yy++)
	{
		if (grid.getValueAt(sf::Vector3i(x,yy,z)) != Types::AIR) continue;
		chunk->addBlock(grid,sf::Vector3i(x,yy,z),Types::WATER);
	}
	for (int yy = 0; yy < MAX_NOISE_HEIGHT - 48; yy++)
	{
		if (grid.getValueAt(sf::Vector3i(x,yy,z)) != Types::AIR) continue;
		chunk->addBlock(grid,sf::Vector3i(x,yy,z),Types::LAVA);
	}
}

void Terrain::createTrees(Chunk *chunk, Grid grid, int noiseHeight, int x, int y, int z)
{
	if (grid.getValueAt(sf::Vector3i(x,noiseHeight-1,z)) != Types::GRASS ||
		grid.getValueAt(sf::Vector3i(x,noiseHeight,z)) != Types::AIR) return;
	int seeds = Math::random(50);
	if (seeds == 20)
	{
		int treeHeight = Math::randomRange(4,8);
		int leafShape = Math::random(3);
		for (int y1 = noiseHeight; y1 < noiseHeight+treeHeight; y1++) // STEM
			chunk->addBlock(grid,sf::Vector3i(x,y1,z),Types::LOG);
		switch (leafShape)
		{
			case 0:
			{
				for (int xx = -2; xx <= 2; xx++)
				for (int zz = -2; zz <= 2; zz++)
				{
					chunk->addBlock(grid,sf::Vector3i(x+xx,noiseHeight+treeHeight,z+zz),Types::LEAF);
					if (xx == -2 || zz == -2 ||
						xx ==  2 || zz ==  2) continue;
					chunk->addBlock(grid,sf::Vector3i(x+xx,(noiseHeight+treeHeight)+1,z+zz),Types::LEAF);
					if (xx == -1 || zz == -1 ||
						xx ==  1 || zz ==  1) continue;
					chunk->addBlock(grid,sf::Vector3i(x+xx,(noiseHeight+treeHeight)+2,z+zz),Types::LEAF);
				}
				break;
			}
			case 1:
			{
				for (int xx = -2; xx <= 2; xx++)
				for (int zz = -2; zz <= 2; zz++)
				{
					if ((xx == -2 && zz == -2) ||
						(xx == -2 && zz ==  2) ||
						(xx ==  2 && zz == -2) ||
						(xx ==  2 && zz ==  2)) continue;
					chunk->addBlock(grid,sf::Vector3i(x+xx,noiseHeight+treeHeight,z+zz),Types::LEAF);
					chunk->addBlock(grid,sf::Vector3i(x+xx,(noiseHeight+treeHeight)+1,z+zz),Types::LEAF);
					chunk->addBlock(grid,sf::Vector3i(x+xx,(noiseHeight+treeHeight)+2,z+zz),Types::LEAF);
				}
				break;
			}
			case 2:
			case 3:
			{
				for (int xx = -1; xx <= 1; xx++)
				for (int zz = -1; zz <= 1; zz++)
				{
					if ((xx == -1 && zz == -1) ||
						(xx == -1 && zz ==  1) ||
						(xx ==  1 && zz == -1) ||
						(xx ==  1 && zz ==  1)) continue;
					for (int yy = noiseHeight+treeHeight; yy <= (noiseHeight+treeHeight)+5; yy++)
						chunk->addBlock(grid,sf::Vector3i(x+xx,yy,z+zz),Types::LEAF);
					if (xx == 0 && zz == 0)
					{
						chunk->addBlock(grid,sf::Vector3i(x+xx,(noiseHeight+treeHeight)+6,z+zz),Types::LEAF);
						chunk->addBlock(grid,sf::Vector3i(x+xx,(noiseHeight+treeHeight)+7,z+zz),Types::LEAF);
						chunk->addBlock(grid,sf::Vector3i(x+xx,(noiseHeight+treeHeight)+8,z+zz),Types::LEAF);
					}
				}
				break;
			}
		}
	}
}


