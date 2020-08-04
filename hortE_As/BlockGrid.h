#ifndef BLOCKGRID_H
#define BLOCKGRID_H

#include <vector>

#include "Definitions.h"

#include "Block.h"

namespace Simplex
{
	class BlockGrid
	{
	public:
		// Returns an instance of the singleton
		static BlockGrid* GetInstance();

		// Cleans up the instance
		static void ReleaseInstance();

		// Adds entities to the render list
		void Render();

		// Returns an ordered path of block IDs
		std::vector<uint> CalculateAStarPath(std::pair<uint, uint> startBlock, std::pair<uint, uint> endBlock);

		// Creates and initiates a new array of blocks of a given size
		MyEntity* GenerateNewGrid(uint size);

		// Get the 2D array coordinages of a block from a given vector index
		std::pair<uint, uint> GetXYPairFromIndex(uint index);

		// Get the vector index for a block at a given coordinate
		uint GetIndexFromXYPair(std::pair<uint, uint>);

		// Destructor
		~BlockGrid();

	private:
		// String for cube model file
		const std::string m_sModelFile = "Minecraft\\Cube.obj";

		// Instance of the Singleton
		static BlockGrid* m_instance;

		// Vector containing our grid blocks
		std::vector<Block*> m_pBlockArray;

		//Dimension of the grid
		uint m_uGridSize;

		// Display rigid body?
		bool m_bShowRigidBody;

		// Display grid plane?
		bool m_bShowGridPlane;

		// Private constructor - Needs the size of the grid
		BlockGrid();
	};
}

#endif // BLOCKGRID_H
