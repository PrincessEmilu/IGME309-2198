#include "Block.h"

using namespace Simplex;
Block::Block(UIntPair index, String a_sFileName, String a_sUniqueID) : MyEntity::MyEntity(a_sFileName, a_sUniqueID)
{
	m_uXZIndex = index;

	// The plane is to be drawn if this block is part of the calculated A* path
	m_BlockPlaneVisible = false;

	// The color the plane should be rendered in to indicated start, finish, or path.
	// Yellow is the devault for the path color
	m_vCurrentPlaneColor = m_vPathPlaneColor;

	// By default, all blocks are weighted the same.
	m_uWeight = m_uDefaultWeight;

	ResetAStar();
}

Block::~Block()
{
	// The previous block should have already been deleted. Make sure we don't have a dangling pointer.
	m_pPreviousBlock = nullptr;

	for (auto neighbor : m_vNeighborList)
	{
		neighbor = nullptr;
	}
}

void Block::ResetAStar()
{
	m_vCurrentPlaneColor = m_vPathPlaneColor;
	m_BlockPlaneVisible = false;
	m_bPermanent = false;
	m_pPreviousBlock = nullptr;
	m_fDistanceFromStart = FLT_MAX;
	m_fHeuristicCost = 0.0f;
}

void Block::AddToRenderList()
{
	// Render via the base method
	// Render the rigidbody if the block has been weighted
	MyEntity::AddToRenderList(m_uWeight == m_uObstacleWeight);

	// Render the top panel if visibility enabled
	if (m_BlockPlaneVisible)
	{
		matrix4 plane_matrix = GetModelMatrix();
		plane_matrix = glm::translate(plane_matrix, vector3(0.5f, 1.1f, 0.5f));
		plane_matrix = glm::rotate(plane_matrix, (float)(PI / -2), AXIS_X);
		plane_matrix = glm::scale(plane_matrix, vector3(0.5f));
		MeshManager::GetInstance()->AddPlaneToRenderList(plane_matrix, m_vCurrentPlaneColor);
	}
		
}

void Block::AddToNeighborList(Block* newNeigbor)
{
	// Check if this neighbor exists already
	for (auto neighbor : m_vNeighborList)
	{
		if (neighbor == newNeigbor)
			return;
	}

	// Add it to the list if it's new
	m_vNeighborList.push_back(newNeigbor);
}

std::vector<Block*> Simplex::Block::GetNeighborList()
{
	return m_vNeighborList;
}

void Simplex::Block::SetPreviousBlock(Block* previous_block)
{
	m_pPreviousBlock = previous_block;
}

Block* Simplex::Block::GetPreviousBlock()
{
	return m_pPreviousBlock;
}

void Simplex::Block::SetPermanent(bool isPermanent)
{
	m_bPermanent = isPermanent;
}

bool Simplex::Block::GetPermanent()
{
	return m_bPermanent;
}

void Simplex::Block::UpdateDistanceFromStart()
{
	m_fDistanceFromStart = m_uWeight;
	
	if (m_pPreviousBlock)
		m_fDistanceFromStart += m_pPreviousBlock->GetDistanceFromStart();
}

uint Simplex::Block::GetDistanceFromStart()
{
	return m_fDistanceFromStart;
}

void Simplex::Block::SetWeight(bool hasObstacle)
{
	if (hasObstacle)
		m_uWeight = m_uObstacleWeight;

	else
		m_uWeight = m_uDefaultWeight;
}

void Simplex::Block::SetHeuristicCost(float cost)
{
	if (cost >= 0.0f)
		m_fHeuristicCost = cost;
}

float Simplex::Block::GetDistancePlusHeuristic()
{
	return m_fDistanceFromStart + m_fHeuristicCost;
}
// Set visibility of the plane drawn on top of block for pathfinding info
void Simplex::Block::SetBlockPlaneVisible(bool isVisible)
{
	m_BlockPlaneVisible = isVisible;
}

// Set the color of the plane drawn on top of block for pathfinding info
void Simplex::Block::SetBlockPlaneColor(vector3 color)
{
	m_vCurrentPlaneColor = color;
}

// Get the XY index of the block as if it were in a 2D array
UIntPair Block::GetXZIndex()
{
	return m_uXZIndex;
}

// Recursively gets the XZ coordinates of each point along the path
std::vector<UIntPair> Block::GetCalculatedPath()
{
	auto returnPath = std::vector<UIntPair>();

	if (m_pPreviousBlock)
	{
		auto previousPath = m_pPreviousBlock->GetCalculatedPath();
		for (uint i = 0; i < previousPath.size(); i++)
		{
			returnPath.push_back(previousPath[i]);
		}
	}
	else
	{
		m_vCurrentPlaneColor = m_vStartPlaneColor;
	}

	returnPath.push_back(GetXZIndex());
	m_BlockPlaneVisible = true;
	return returnPath;
}


