#ifndef BLOCKGRID_H
#define BLOCKGRID_H

#include <vector>

#include "Definitions.h"

namespace Simplex
{
	class BlockGrid
	{
	public:
		// Returns an instance of the singleton
		static BlockGrid* GetInstance();

		// Cleans up the instance
		static void ReleaseInstance();

		// Returns an ordered path of block IDs
		std::vector<uint> CalculateAStarPath(std::pair<uint, uint> startBlock, std::pair<uint, uint> endBlock);

		// Creates and initiates a new array of blocks of a given size
		MyEntity* GenerateNewGrid(uint size);

		// Destructor
		~BlockGrid();

	private:
		// Instance of the Singleton
		static BlockGrid* m_instance;

		//Dimension of the grid
		uint m_gridSize;

		// Private constructor - Needs the size of the grid
		BlockGrid();
	};
}

#endif // BLOCKGRID_H
