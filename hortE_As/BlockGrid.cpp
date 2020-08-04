#include "BlockGrid.h"
#include "Definitions.h"

using namespace Simplex;

// init instance
BlockGrid* BlockGrid::m_instance = nullptr;

BlockGrid::BlockGrid()
{
	std::cout << "BlockGrid()" << std::endl;
	m_gridSize = 0;
}


BlockGrid* BlockGrid::GetInstance()
{
	if (m_instance)
	{
		std::cout << "Instance exists, return it" << std::endl;
		return m_instance;
	}
	else
	{
		std::cout << "Create a new instance of BlockGrid" << std::endl;
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

std::vector<uint> BlockGrid::CalculateAStarPath(std::pair<uint, uint> startBlock, std::pair<uint, uint> endBlock)
{
	return std::vector<uint>();
}

BlockGrid::~BlockGrid()
{
	std::cout << "~BlockGrid()" << std::endl;
}

MyEntity* BlockGrid::GenerateNewGrid(uint size)
{
	m_gridSize = size;
	std::cout << "Generate a new grid size: " << m_gridSize << std::endl;
	return nullptr;
}
