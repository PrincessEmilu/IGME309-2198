#include "BlockGrid.h"
#include "Definitions.h"

using namespace Simplex;

// init instance
BlockGrid* BlockGrid::m_instance = nullptr;

BlockGrid::BlockGrid()
{
	m_bShowRigidBody = false;
	m_bShowGridPlane = false;

	std::cout << "BlockGrid()" << std::endl;
	m_uGridSize = 0;
	m_pBlockArray = std::vector<Block*>();
}

BlockGrid::~BlockGrid()
{
	std::cout << "~BlockGrid()" << std::endl;
	// Clear the block vector
	for (auto block : m_instance->m_pBlockArray)
	{
		delete block;
		block = nullptr;
	}
}

void BlockGrid::Render()
{
	for (uint i = 0; i < m_pBlockArray.size(); i++)
	{
		m_pBlockArray[i]->AddToRenderList();
	}
}

BlockGrid* BlockGrid::GetInstance()
{
	if (m_instance)
	{
		return m_instance;
	}
	else
	{
		m_instance = new BlockGrid();
		return m_instance;
	}
}

void BlockGrid::ReleaseInstance()
{
	if (m_instance)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

uint BlockGrid::GetIndexFromXYPair(UIntPair xyPair)
{
	// Calculates the 1D index from a 2D value if it's within range
	uint index = -1;
	if (xyPair.first < m_uGridSize && xyPair.second < m_uGridSize)
		index = xyPair.first + (xyPair.second * m_uGridSize);

	return index;
}

UIntPair BlockGrid::GetXYPairFromIndex(uint index)
{
	// Calculate the 2D index from a 1D value if it's within range.
	auto xYPair = UIntPair(-1, -1);
	if (index < m_pBlockArray.size())
	{
		xYPair.first = index % m_uGridSize;
		xYPair.second = (index - xYPair.first) / m_uGridSize;
	}

	return xYPair;
}

std::vector<Block*> BlockGrid::CalculateAStarPath(UIntPair a_UStartBlock, UIntPair a_uEndBlock)
{
	// The blocks that are the path from start to finish
	auto pathVector = std::vector<Block*>();
	auto queue = std::deque<Block*>();

	// Get the blocks for start and end
	Block* startBlock = m_pBlockArray[GetIndexFromXYPair(a_UStartBlock)];
	Block* endBlock = m_pBlockArray[GetIndexFromXYPair(a_uEndBlock)];

	// Keep track of the current block we are on
	Block* currentBlock = startBlock;

	// Reset all blocks' A* values
	ResetAllBlocks();

	// TODO: Calculate a weight for the blocks, somehow?
	//SetBlockWeight();

	//Assign all blocks' Heuristic Costs
	SetHeuristicCost(endBlock);

	// Prepare the start block
	startBlock->SetHeuristicCost(0.0f);
	startBlock->SetPermanent(true);
	startBlock->UpdateDistanceFromStart();

	// The starting block is the first block to check
	queue.push_back(startBlock);

	// Loop until we have reached the end... I think that's how we do it here?
	while (currentBlock != endBlock)
	{
		//std::cout << "Current Block Index: " << currentBlock->GetXYIndex().first << ", " << currentBlock->GetXYIndex().second << std::endl;
		// For each of the current block's neighbors
		for (auto neighbor : currentBlock->GetNeighborList())
		{
			// Only work on non-permanent blocks
			if (false == neighbor->GetPermanent())
			{
				// Set the neighbor's previous block to the current block if the current one yields a shorter path.
				if (currentBlock->GetDistanceFromStart() < neighbor->GetDistanceFromStart())
				{
					neighbor->SetPreviousBlock(currentBlock);
					neighbor->UpdateDistanceFromStart();
				}
			}
		}

		// Every neighbor has been checked, mark the current block as permanent
		currentBlock->SetPermanent(true);

		// Add the front element of the queue to the path vector
		//pathVector.push_back(queue.front());

		// Remove the current block from the queue
		queue.pop_front();

		// Add neighbors to the list
		auto neighborList = currentBlock->GetNeighborList();
		for (auto neighbor : neighborList)
		{
			if (false == neighbor->GetPermanent())
				queue.push_back(neighbor);
		}

		// Get the least-cost neighbor and put it in the front
		// We will just assume that the front is the least-cost and test against that
		while (queue.front()->GetPermanent())
		{
			for (uint i = 1; i < queue.size(); i++)
			{
				if ((queue[i]->GetDistanceFromStart() + queue[i]->GetHeuristicCost()) < (queue.front()->GetDistanceFromStart() + queue.front()->GetHeuristicCost()))
				{
					std::swap(queue[0], queue[i]);
				}
			}

			if (queue.front()->GetPermanent())
				queue.pop_front();
		}

		// Set the next current block to the cheapest (first) block in the list
		currentBlock = queue.front();
	}

	endBlock->PrintPath();
	endBlock->SetGridPanelColor(C_GREEN_LIME);

	return pathVector;
}

void BlockGrid::PutCheapestBlockInFront()
{
	int cheapestIndex = 0; // Assume it's the first one
}

void BlockGrid::GenerateNewGrid(uint size)
{
	m_uGridSize = size;

	// Nested for loop fakes creating a real 2D array... don't tell anyone we are cheating!
	for (uint i = 0; i < m_uGridSize; ++i)
	{
		for (uint j = 0; j < m_uGridSize; ++j)
		{
			m_pBlockArray.push_back(new Block(UIntPair(j, i), m_sModelFile, "Block"));

			// Calculate the X and Z such that the grid will be centered at the origin
			float x = (-1 * ((float)(m_uGridSize) / 2.0f)) + j;
			float z = (-1 * ((float)(m_uGridSize) / 2.0f)) + i;
			m_pBlockArray.back()->SetModelMatrix(glm::translate(vector3(x, 0.0f, z)));
		}
	}

	// Assign all valid neighbor blocks to this block
	for (auto block : m_pBlockArray)
	{
		AssignNeighbors(block);
	}

	// TODO: EMILY, remove this!
	CalculateAStarPath(UIntPair(0, 0), UIntPair(2, 20));
}

void BlockGrid::SetHeuristicCost(Block* goalBlock)
{
	for (auto block : m_pBlockArray)
	{
		block->SetHeuristicCost(glm::distance(goalBlock->GetPosition(), block->GetPosition()));
	}
}

void BlockGrid::AssignNeighbors(Block* block)
{
	// "Coordinates" for this block
	uint x = block->GetXYIndex().first;
	uint z = block->GetXYIndex().second;

	// EMILY TODO: Remove before submission!
	uint total = 0;

	// Add Neighbors- four possibilities
	// Left
	if (x > 0)
	{
		block->AddToNeighborList(m_pBlockArray[GetIndexFromXYPair(UIntPair(x - 1, z))]);
		++total;
	}

	// Right
	if (x < m_uGridSize - 1)
	{
		block->AddToNeighborList(m_pBlockArray[GetIndexFromXYPair(UIntPair(x + 1, z))]);
		++total;
	}

	// Above
	if (z > 0)
	{
		block->AddToNeighborList(m_pBlockArray[GetIndexFromXYPair(UIntPair(x, z - 1))]);
		++total;
	}

	// Below
	if (z < m_uGridSize - 1)
	{
		block->AddToNeighborList(m_pBlockArray[GetIndexFromXYPair(UIntPair(x, z + 1))]);
		++total;
	}

	//std::cout << "Neighbors Added: " << total << std::endl;
}

void BlockGrid::ResetAllBlocks()
{
	// Tell each block to reset itself for a new A* calculation
	for (auto block : m_pBlockArray)
		block->ResetAStar();
}
