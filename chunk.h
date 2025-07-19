#ifndef _VOX2_BLOCK_H_
#define _VOX2_BLOCK_H_

#include <SFML/OpenGL.hpp>

#include "grid.h"
#include "texture.h"
#include "types.h"
#include "defines.h"

class Chunk
{
public:
	Chunk(Grid grid);
	~Chunk();
	void setupModel(Grid grid, sf::Vector3i position);
	void renderModel(Grid grid, sf::Vector3i position, bool fast = true);
	void addBlock(Grid grid, sf::Vector3i position, int block);
	void removeBlock(Grid grid, sf::Vector3i position);
private:
	int x1,y1,z1,x2,y2,z2;
	int top, bottom, left, right, back, front;
	void updateChunk(sf::Vector3i position);
	void updateNearChunk(sf::Vector3i position);
	Texture texClass;
	Grid buffer, update;
protected:
	void drawOpaqueFaces(Grid grid, sf::Vector3i position, int x, int y, int z, int block);
	void drawTransparentFaces(Grid grid, sf::Vector3i position, int x, int y, int z, int block);
	void drawBlendedFaces(Grid grid, sf::Vector3i position, int x, int y, int z, int block);
};

#endif /* _VOX2_CHUNK_H_ */