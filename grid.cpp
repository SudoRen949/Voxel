
#include "grid.h"

void Grid::createGrid(sf::Vector3i size, int bit)
{
	/*
	*	Copyright
	*	Code from Maarten Baert (GameMaker Code)
	*	Fast Large Array
	*/
	if (size.x <= 0 || size.y <= 0 || size.z <= 0) return;
	gridSize = size;
	nBits = bit;
	byteSlice = 32 / bit;
	if (byteSlice == 1) dataSize = (size.x * size.y * size.z) * sizeof(int);
	else dataSize = (((size.x * size.y * size.z) / byteSlice) + 1) * sizeof(int);
	dataMap = (int*) malloc(dataSize);
	memset(dataMap,0,dataSize);
}

void Grid::destroyGrid()
{
	nBits = 0;
	byteSlice = 0;
	if (dataMap != NULL) free(dataMap);
}

void Grid::clear(int v)
{
	memset(dataMap,v,dataSize);
}

void Grid::setValueAt(sf::Vector3i position, int value)
{
	/*
	*	Copyright
	*	Code from Maarten Baert (GameMaker Code)
	*/
	if ( position.x < 0 || position.y < 0 || position.z < 0 ) return;
	if ( position.x >= gridSize.x || position.y >= gridSize.y || position.z >= gridSize.z ) return;
	int index = position.x * gridSize.x *  gridSize.y + position.y * gridSize.z + position.z;
	if (byteSlice == 1) dataMap[ index ] = value;
	else
	{
		int shift = (index % byteSlice) * nBits;
		int temp = ( (1 << nBits) - 1 ) << shift;
		dataMap[ index / byteSlice ] = (dataMap[ index / byteSlice ] & ~temp) | ( ( value << shift ) & temp );
	}
}

int Grid::getValueAt(sf::Vector3i position)
{
	/*
	*	Copyright
	*	Code from Maarten Baert (GameMaker Code)
	*/
	if ( position.x < 0 || position.y < 0 || position.z < 0 ) return 0;
	if ( position.x >= gridSize.x || position.y >= gridSize.y || position.z >= gridSize.z ) return 0;
	int index = position.x * gridSize.x *  gridSize.y + position.y * gridSize.z + position.z;
	if (byteSlice == 1) return dataMap[ index ];
	else 
	{
		int shift = (index % byteSlice) * nBits;
		return (dataMap[ index / byteSlice ] >> shift) & ( (1 << nBits) - 1 );
	}
}

sf::Vector3i Grid::getSize()
{
	return gridSize;
}
