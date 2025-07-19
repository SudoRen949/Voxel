#ifndef _VOX2_GRID_H_
#define _VOX2_GRID_H_

#include <cstring>
#include <cstdlib>
#include <SFML/System.hpp>

#include "maths.h"

class Grid
{
public:
	void createGrid(sf::Vector3i size, int bit = 1);
	void destroyGrid();
	void clear(int v = 0);
	void setValueAt(sf::Vector3i position, int value);
	int getValueAt(sf::Vector3i position);
	sf::Vector3i getSize();
private:
	int byteSlice, nBits;
	sf::Vector3i gridSize;
	int *dataMap, dataSize;
};

#endif /* _VOX2_GRID_H_ */