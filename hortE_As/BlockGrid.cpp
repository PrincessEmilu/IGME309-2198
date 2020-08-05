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
		m_pBlockArray[i]->AddToRenderList(m_bShowRigidBody, m_bShowGridPlane);
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

uint BlockGrid::GetIndexFromXYPair(std::pair<uint, uint> xyPair)
{
	// Calculates the 1D index from a 2D value if it's within range
	uint index = -1;
	if (xyPair.first < m_uGridSize || xyPair.second < m_uGridSize)
		index = xyPair.first + (xyPair.second * m_uGridSize);

	return index;
}

std::pair<uint, uint> BlockGrid::GetXYPairFromIndex(uint index)
{
	// Calculate the 2D index from a 1D value if it's within range.
	auto xYPair = std::pair<uint, uint>(-1, -1);
	if (index < m_pBlockArray.size())
	{
		xYPair.first = index % m_uGridSize;
		xYPair.second = (index - xYPair.first) / m_uGridSize;
	}

	return xYPair;
}

std::vector<uint> BlockGrid::CalculateAStarPath(std::pair<uint, uint> startBlock, std::pair<uint, uint> endBlock)
{
	return std::vector<uint>();
}

void BlockGrid::GenerateNewGrid(uint size)
{
	m_uGridSize = size;

	// Nested for loop fakes creating a real 2D array... don't tell anyone we are cheating!
	for (uint i = 0; i < m_uGridSize; ++i)
	{
		for (uint j = 0; j < m_uGridSize; ++j)
		{
			m_pBlockArray.push_back(new Block(std::pair<uint, uint>(j, i), m_sModelFile, "Block"));

			// Calculate the X and Z such that the grid will be centered at the origin
			float x = (-1 * ((float)(m_uGridSize) / 2.0f)) + j;
			float z = (-1 * ((float)(m_uGridSize) / 2.0f)) + i;
			m_pBlockArray.back()->SetModelMatrix(glm::translate(vector3(x, 0.0f, z)));
		}
	}
}
