#ifndef BLOCKGRID_H
#define BLOCKGRID_H

#include <vector>

#include "Definitions.h"
#include "MyEntityManager.h"

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
		std::vector<UIntPair> CalculateAStarPath(UIntPair startBlock, UIntPair endBlock);

		// Get the world position for a given block given the XZ pair
		vector3 GetBlockWorldPosition(UIntPair index);

		// Creates and initiates a new array of blocks of a given size
		void GenerateNewGrid(uint size);

		// Get the 2D array coordinages of a block from a given vector index
		UIntPair GetXZPairFromIndex(uint index);

		// Get the vector index for a block at a given coordinate
		uint GetIndexFromXZPair(UIntPair);

		// Returns the XZ coords for the plack the entity is on. Used for getting the start/end points
		UIntPair GetCollidingBlock(MyEntity* targetEntity);

		// Given a vector of obstacles, sets new weights to each block if it has obstacles on it
		void SetAllBlockWeights(std::vector<MyEntity*> obstacleVector);

		// Assigns all valid neighbors to this block
		void AssignAllNeighborsToBlock(Block* block);

		// Sets the heuristic cost for each block based on distance to finish
		void SetHeuristicCost(Block* goalBlock);

		// Reset each block for a new A* search
		void ResetAllBlocks();

		// Destructor
		~BlockGrid();

	private:
		// String for cube model file
		const std::string m_sModelFile = "Minecraft\\Cube.obj";

		// Instance of the Singleton
		static BlockGrid* m_pInstance;

		// Vector containing our grid blocks
		std::vector<Block*> m_vBlockVector;

		//Dimension of the grid - it is always going to be square
		uint m_uGridSize;

		// Private constructor - Needs the size of the grid
		BlockGrid();
	};
}

#endif // BLOCKGRID_H
