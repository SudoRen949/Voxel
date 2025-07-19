
#include "chunk.h"

Chunk::Chunk(Grid grid)
{
	texClass.load("assets/textures/blocks.png");
	int cx = grid.getSize().x / CHUNK_SIZE;
	int cy = grid.getSize().y / CHUNK_SIZE;
	int cz = grid.getSize().z / CHUNK_SIZE;
	buffer.createGrid(sf::Vector3i(cx,cy,cz),12);
	update.createGrid(sf::Vector3i(cx,cy,cz));
}

Chunk::~Chunk()
{
	for (int y = 0; y < buffer.getSize().y; y++)
	for (int x = 0; x < buffer.getSize().x; x++)
	for (int z = 0; z < buffer.getSize().z; z++)
	{
		glDeleteLists(buffer.getValueAt(sf::Vector3i(x,y,z)),1);
	}
	buffer.destroyGrid();
	update.destroyGrid();
}

void Chunk::setupModel(Grid grid, sf::Vector3i position)
{
	glPushMatrix();
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	texClass.bind();
	glBegin(GL_QUADS);
	for (int y = position.y * CHUNK_SIZE; y < CHUNK_SIZE + (position.y * CHUNK_SIZE); y++)
	for (int x = position.x * CHUNK_SIZE; x < CHUNK_SIZE + (position.x * CHUNK_SIZE); x++)
	for (int z = position.z * CHUNK_SIZE; z < CHUNK_SIZE + (position.z * CHUNK_SIZE); z++)
	{
		if (x >= grid.getSize().x) break;
		if (y >= grid.getSize().y) break;
		if (z >= grid.getSize().z) break;
		x1 = x * BLOCK_SIZE;	x2 = (x+1) * BLOCK_SIZE;
		y1 = y * BLOCK_SIZE;	y2 = (y+1) * BLOCK_SIZE;
		z1 = z * BLOCK_SIZE;	z2 = (z+1) * BLOCK_SIZE;
		int block = grid.getValueAt(sf::Vector3i(x,y,z));
		if (block == Types::AIR) continue;
		texClass.calculateHMap(block);
		if (Types::checkOpaque(block)) drawOpaqueFaces(grid,position,x,y,z,block); 
		else if (Types::checkTransparent(block)) drawTransparentFaces(grid,position,x,y,z,block);
		else if (Types::checkBlend(block)) drawBlendedFaces(grid,position,x,y,z,block);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Chunk::renderModel(Grid grid, sf::Vector3i position, bool fast)
{
	if (fast)
	{
		/** Check block model */
		if (update.getValueAt(position) == 1)
		{
			buffer.setValueAt(position,(int)glGenLists(1));
			glNewList(buffer.getValueAt(position),GL_COMPILE);
			setupModel(grid,position);
			glEndList();
			update.setValueAt(position,0);
		}
		if (buffer.getValueAt(position) > 0)
			glCallList(buffer.getValueAt(position));
	}
	else if (!fast)
	{
		setupModel(grid,position);
	}
}

void Chunk::addBlock(Grid grid, sf::Vector3i position, int block)
{
	if (block == Types::AIR) return;
	grid.setValueAt(position,block);
	updateNearChunk(position);
}

void Chunk::removeBlock(Grid grid, sf::Vector3i position)
{
	if (grid.getValueAt(position) == Types::AIR) return;
	grid.setValueAt(position,Types::AIR);
	updateNearChunk(position);
}

/** -- PRIVATE FUNCTIONS -- */

void Chunk::updateChunk(sf::Vector3i position)
{
	if (position.x < 0 || position.y < 0 || position.z < 0) return;
	if (position.x >= update.getSize().x || position.y >= update.getSize().y || position.z >= update.getSize().z) return;
	if (update.getValueAt(position) == 0) update.setValueAt(position,1);
}

void Chunk::updateNearChunk(sf::Vector3i position)
{
	int cx, cy, cz;
	/** What chunk does the block at? */
	cx = position.x / CHUNK_SIZE;
	cy = position.y / CHUNK_SIZE;
	cz = position.z / CHUNK_SIZE;
	updateChunk(sf::Vector3i(cx,cy,cz));
	if (position.y == (((position.y+1)/CHUNK_SIZE)*CHUNK_SIZE)-1) updateChunk(sf::Vector3i(cx,cy+1,cz));
	if (position.x == (((position.x+1)/CHUNK_SIZE)*CHUNK_SIZE)-1) updateChunk(sf::Vector3i(cx+1,cy,cz));
	if (position.z == (((position.z+1)/CHUNK_SIZE)*CHUNK_SIZE)-1) updateChunk(sf::Vector3i(cx,cy,cz+1));
	if (position.y == (((position.y+1)/CHUNK_SIZE)*CHUNK_SIZE)) updateChunk(sf::Vector3i(cx,cy-1,cz));
	if (position.x == (((position.x+1)/CHUNK_SIZE)*CHUNK_SIZE)) updateChunk(sf::Vector3i(cx-1,cy,cz));
	if (position.z == (((position.z+1)/CHUNK_SIZE)*CHUNK_SIZE)) updateChunk(sf::Vector3i(cx,cy,cz-1));
}


/** -- ISOLATED FUNCTIONS */

void Chunk::drawOpaqueFaces(Grid grid, sf::Vector3i position, int x, int y, int z, int block)
{
	top = grid.getValueAt(sf::Vector3i(x,y+1,z));
	bottom = grid.getValueAt(sf::Vector3i(x,y-1,z));
	left = grid.getValueAt(sf::Vector3i(x-1,y,z));
	right = grid.getValueAt(sf::Vector3i(x+1,y,z));
	front = grid.getValueAt(sf::Vector3i(x,y,z+1));
	back = grid.getValueAt(sf::Vector3i(x,y,z-1));
	if (top == Types::AIR || Types::checkNotOpaque(top) || y >= grid.getSize().y-1)
	{
		glTexCoord2f(texClass.getMap(1,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y2,z2);
		glTexCoord2f(texClass.getMap(1,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y2,z2);
		glTexCoord2f(texClass.getMap(1,4),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y2,z1);
		glTexCoord2f(texClass.getMap(1,6),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y2,z1);
	}
	if (bottom == Types::AIR || Types::checkNotOpaque(bottom) || y == 0)
	{
		glTexCoord2f(texClass.getMap(3,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y1,z1);
		glTexCoord2f(texClass.getMap(3,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y1,z1);
		glTexCoord2f(texClass.getMap(3,4),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y1,z2);
		glTexCoord2f(texClass.getMap(3,6),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y1,z2);
	}
	if (right == Types::AIR || Types::checkNotOpaque(right) || x >= grid.getSize().x-1)
	{
		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x2,y1,z1);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y2,z1);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x2,y2,z2);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y1,z2);
	}
	if (left == Types::AIR || Types::checkNotOpaque(left) || x == 0)
	{
		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y1,z2);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x1,y2,z2);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y2,z1);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x1,y1,z1);
	}
	if (front == Types::AIR || Types::checkNotOpaque(front) || z >= grid.getSize().z-1)
	{
		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x2,y1,z2);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y2,z2);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y2,z2);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x1,y1,z2);
	}
	if (back == Types::AIR || Types::checkNotOpaque(back) || z == 0)
	{
		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y1,z1);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x1,y2,z1);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x2,y2,z1);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y1,z1);
	}
}

void Chunk::drawTransparentFaces(Grid grid, sf::Vector3i position, int x, int y, int z, int block)
{
	float l1 = 0, l2 = 0, l3 = 0, l4 = 0;
	float f1 = 0, f2 = 0, f3 = 0, f4 = 0;
	top = grid.getValueAt(sf::Vector3i(x,y+1,z));
	bottom = grid.getValueAt(sf::Vector3i(x,y-1,z));
	left = grid.getValueAt(sf::Vector3i(x-1,y,z));
	right = grid.getValueAt(sf::Vector3i(x+1,y,z));
	front = grid.getValueAt(sf::Vector3i(x,y,z+1));
	back = grid.getValueAt(sf::Vector3i(x,y,z-1));
	if (Types::checkBlend(top) || Types::checkOpaque(top) || y >= grid.getSize().y-1)
	{
		l1 = l2 = l3 = l4 = f1 = f2 = f3 = f4 = 0;
		if (Types::checkTransparent(left)) { l1 = 0.005; l4 = 0.005; }
		if (Types::checkTransparent(right)) { l2 = 0.005; l3 = 0.005; }
		if (Types::checkTransparent(back)) { f1 = 0.005; f4 = 0.005; }
		if (Types::checkTransparent(front)) { f2 = 0.005; f3 = 0.005; }
		glTexCoord2f(texClass.getMap(1,0)+l1,texClass.getMap(block%texClass.getNBlocks().y,1)-f3); glVertex3i(x1,y2,z2);
		glTexCoord2f(texClass.getMap(1,2)-l2,texClass.getMap(block%texClass.getNBlocks().y,3)-f2); glVertex3i(x2,y2,z2);
		glTexCoord2f(texClass.getMap(1,4)-l3,texClass.getMap(block%texClass.getNBlocks().y,5)+f1); glVertex3i(x2,y2,z1);
		glTexCoord2f(texClass.getMap(1,6)+l4,texClass.getMap(block%texClass.getNBlocks().y,7)+f4); glVertex3i(x1,y2,z1);
	}
	if (Types::checkBlend(bottom) || Types::checkOpaque(bottom) || y == 0)
	{
		l1 = l2 = l3 = l4 = f1 = f2 = f3 = f4 = 0;
		if (Types::checkTransparent(left)) { l1 = 0.005; l4 = 0.005; }
		if (Types::checkTransparent(right)) { l2 = 0.005; l3 = 0.005; }
		if (Types::checkTransparent(back)) { f1 = 0.005; f4 = 0.005; }
		if (Types::checkTransparent(front)) { f2 = 0.005; f3 = 0.005; }
		glTexCoord2f(texClass.getMap(3,0)+l1,texClass.getMap(block%texClass.getNBlocks().y,1)-f1); glVertex3i(x1,y1,z1);
		glTexCoord2f(texClass.getMap(3,2)-l2,texClass.getMap(block%texClass.getNBlocks().y,3)-f4); glVertex3i(x2,y1,z1);
		glTexCoord2f(texClass.getMap(3,4)-l3,texClass.getMap(block%texClass.getNBlocks().y,5)+f3); glVertex3i(x2,y1,z2);
		glTexCoord2f(texClass.getMap(3,6)+l4,texClass.getMap(block%texClass.getNBlocks().y,7)+f2); glVertex3i(x1,y1,z2);
	}
	if (Types::checkBlend(right) || Types::checkOpaque(right) || x >= grid.getSize().x-1)
	{
		l1 = l2 = l3 = l4 = f1 = f2 = f3 = f4 = 0;
		if (Types::checkTransparent(bottom)) { l1 = 0.005; l4 = 0.005; }
		if (Types::checkTransparent(top)) { l2 = 0.005; l3 = 0.005; }
		if (Types::checkTransparent(back)) { f1 = 0.005; f4 = 0.005; }
		if (Types::checkTransparent(front)) { f2 = 0.005; f3 = 0.005; }
		glTexCoord2f(texClass.getMap(2,0)+f1,texClass.getMap(block%texClass.getNBlocks().y,1)-l1); glVertex3i(x2,y1,z1);
		glTexCoord2f(texClass.getMap(2,6)+f4,texClass.getMap(block%texClass.getNBlocks().y,5)+l2); glVertex3i(x2,y2,z1);
		glTexCoord2f(texClass.getMap(2,4)-f2,texClass.getMap(block%texClass.getNBlocks().y,7)+l3); glVertex3i(x2,y2,z2);
		glTexCoord2f(texClass.getMap(2,2)-f3,texClass.getMap(block%texClass.getNBlocks().y,3)-l4); glVertex3i(x2,y1,z2);
	}
	if (Types::checkBlend(left) || Types::checkOpaque(left) || x == 0)
	{
		l1 = l2 = l3 = l4 = f1 = f2 = f3 = f4 = 0;
		if (Types::checkTransparent(bottom)) { l1 = 0.005; l4 = 0.005; }
		if (Types::checkTransparent(top)) { l2 = 0.005; l3 = 0.005; }
		if (Types::checkTransparent(back)) { f1 = 0.005; f4 = 0.005; }
		if (Types::checkTransparent(front)) { f2 = 0.005; f3 = 0.005; }
		glTexCoord2f(texClass.getMap(2,0)+f2,texClass.getMap(block%texClass.getNBlocks().y,1)-l1); glVertex3i(x1,y1,z2);
		glTexCoord2f(texClass.getMap(2,6)+f3,texClass.getMap(block%texClass.getNBlocks().y,5)+l2); glVertex3i(x1,y2,z2);
		glTexCoord2f(texClass.getMap(2,4)-f1,texClass.getMap(block%texClass.getNBlocks().y,7)+l3); glVertex3i(x1,y2,z1);
		glTexCoord2f(texClass.getMap(2,2)-f4,texClass.getMap(block%texClass.getNBlocks().y,3)-l4); glVertex3i(x1,y1,z1);
	}
	if (Types::checkBlend(front) || Types::checkOpaque(front) || z >= grid.getSize().z-1)
	{
		l1 = l2 = l3 = l4 = f1 = f2 = f3 = f4 = 0;
		if (Types::checkTransparent(bottom)) { l1 = 0.005; l4 = 0.005; }
		if (Types::checkTransparent(top)) { l2 = 0.005; l3 = 0.005; }
		if (Types::checkTransparent(left)) { f1 = 0.005; f4 = 0.005; }
		if (Types::checkTransparent(right)) { f2 = 0.005; f3 = 0.005; }
		glTexCoord2f(texClass.getMap(2,0)+f2,texClass.getMap(block%texClass.getNBlocks().y,1)-l1); glVertex3i(x2,y1,z2);
		glTexCoord2f(texClass.getMap(2,6)+f3,texClass.getMap(block%texClass.getNBlocks().y,5)+l2); glVertex3i(x2,y2,z2);
		glTexCoord2f(texClass.getMap(2,4)-f1,texClass.getMap(block%texClass.getNBlocks().y,7)+l3); glVertex3i(x1,y2,z2);
		glTexCoord2f(texClass.getMap(2,2)-f4,texClass.getMap(block%texClass.getNBlocks().y,3)-l4); glVertex3i(x1,y1,z2);
	}
	if (Types::checkBlend(back) || Types::checkOpaque(back) || z == 0)
	{
		l1 = l2 = l3 = l4 = f1 = f2 = f3 = f4 = 0;
		if (Types::checkTransparent(bottom)) { l1 = 0.005; l4 = 0.005; }
		if (Types::checkTransparent(top)) { l2 = 0.005; l3 = 0.005; }
		if (Types::checkTransparent(left)) { f1 = 0.005; f4 = 0.005; }
		if (Types::checkTransparent(right)) { f2 = 0.005; f3 = 0.005; }
		glTexCoord2f(texClass.getMap(2,0)+f1,texClass.getMap(block%texClass.getNBlocks().y,1)-l1); glVertex3i(x1,y1,z1);
		glTexCoord2f(texClass.getMap(2,6)+f4,texClass.getMap(block%texClass.getNBlocks().y,5)+l2); glVertex3i(x1,y2,z1);
		glTexCoord2f(texClass.getMap(2,4)-f2,texClass.getMap(block%texClass.getNBlocks().y,7)+l3); glVertex3i(x2,y2,z1);
		glTexCoord2f(texClass.getMap(2,2)-f3,texClass.getMap(block%texClass.getNBlocks().y,3)-l4); glVertex3i(x2,y1,z1);
	}
}

void Chunk::drawBlendedFaces(Grid grid, sf::Vector3i position, int x, int y, int z, int block)
{	
	top 	= grid.getValueAt(sf::Vector3i(x,y+1,z));
	bottom 	= grid.getValueAt(sf::Vector3i(x,y-1,z));
	left 	= grid.getValueAt(sf::Vector3i(x-1,y,z));
	right 	= grid.getValueAt(sf::Vector3i(x+1,y,z));
	front 	= grid.getValueAt(sf::Vector3i(x,y,z+1));
	back 	= grid.getValueAt(sf::Vector3i(x,y,z-1));
	if (Types::checkLiquids(block) && (Types::checkOpaque(top) || Types::checkTransparent(top))) y2 -= 1;
	int l1 = 0, l2= 0, l3 = 0, l4 = 0;
	if (top == Types::AIR || Types::checkOpaque(top) || y >= grid.getSize().y-1)
	{
		if (Types::checkNotOpaque(grid.getValueAt(sf::Vector3i(x,y+1,z+1)))) { l1 = 1; l2 = 1; } // front
		if (Types::checkNotOpaque(grid.getValueAt(sf::Vector3i(x,y+1,z-1)))) { l3 = 1; l4 = 1; } // back
		if (Types::checkNotOpaque(grid.getValueAt(sf::Vector3i(x+1,y+1,z)))) { l2 = 1; l3 = 1; } // right
		if (Types::checkNotOpaque(grid.getValueAt(sf::Vector3i(x-1,y+1,z)))) { l1 = 1; l4 = 1; } // left
		if (Types::checkNotOpaque(grid.getValueAt(sf::Vector3i(x-1,y+1,z+1)))) l1 = 1; // front-left
		if (Types::checkNotOpaque(grid.getValueAt(sf::Vector3i(x+1,y+1,z-1)))) l3 = 1; // back-right
		if (Types::checkNotOpaque(grid.getValueAt(sf::Vector3i(x-1,y+1,z-1)))) l4 = 1; // back-left
		if (Types::checkNotOpaque(grid.getValueAt(sf::Vector3i(x+1,y+1,z+1)))) l2 = 1; // front-right
		glTexCoord2f(texClass.getMap(1,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y2+l1,z2);
		glTexCoord2f(texClass.getMap(1,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y2+l2,z2);
		glTexCoord2f(texClass.getMap(1,4),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y2+l3,z1);
		glTexCoord2f(texClass.getMap(1,6),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y2+l4,z1);
		// inverted face
		glTexCoord2f(texClass.getMap(1,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y2+l4,z1);
		glTexCoord2f(texClass.getMap(1,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y2+l3,z1);
		glTexCoord2f(texClass.getMap(1,4),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y2+l2,z2);
		glTexCoord2f(texClass.getMap(1,6),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y2+l1,z2);
	}
	if (bottom == Types::AIR || y == 0)
	{
		glTexCoord2f(texClass.getMap(3,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y1,z1);
		glTexCoord2f(texClass.getMap(3,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y1,z1);
		glTexCoord2f(texClass.getMap(3,4),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y1,z2);
		glTexCoord2f(texClass.getMap(3,6),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y1,z2);
	}
	if (right == Types::AIR || x >= grid.getSize().x-1)
	{
		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x2,y1,z1);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y2+l3,z1);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x2,y2+l2,z2);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y1,z2);
	}
	if (left == Types::AIR || x == 0)
	{
		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y1,z2);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x1,y2+l1,z2);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y2+l4,z1);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x1,y1,z1);
	}
	if (front == Types::AIR || z >= grid.getSize().z-1)
	{
		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x2,y1,z2);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y2+l2,z2);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y2+l1,z2);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x1,y1,z2);
	}
	if (back == Types::AIR || z == 0)
	{
		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y1,z1);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x1,y2+l4,z1);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x2,y2+l3,z1);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y1,z1);
	}
}
