#ifndef _VOX2_TYPES_H_
#define _VOX2_TYPES_H_

class Types
{
public:
	enum Blocks
	{
		AIR = 0,
		GRASS,
		DIRT,
		STONE,
		COBBLE,
		LOG,
		GLASS,
		WATER,
		LAVA,
		BEDROCK,
		LEAF,
		
		BlockCount
	};
	enum Chunks
	{
		OPAQUE = 1,
		TRANSPARENT,
		BLEND,
		
		ChunkCount
	};
	/** Chunk */
	static bool checkOpaque(int block)
	{
		if (checkTransparent(block) ||
			checkBlend(block)) return false;
		return true;
	}
	static bool checkTransparent(int block)
	{
		if (block == GLASS) return true;
		return false;
	}
	static bool checkBlend(int block)
	{
		if (block == WATER ||
			block == LAVA) return true;
		return false;
	}
	static bool checkNotOpaque(int block)
	{
		if (checkTransparent(block) || checkBlend(block)) return true;
		return false;
	}
	/** Block */
	static bool checkSolids(int block)
	{
		if (block == AIR ||
			checkLiquids(block)) return false;
		return true;
	}
	static bool checkLiquids(int block)
	{
		if (block == WATER ||
			block == LAVA) return true;
		return false;
	}
};

#endif /* _VOX2_TYPES_H_ */