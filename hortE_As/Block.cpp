#include "Block.h"

using namespace Simplex;
Block::Block(UIntPair index, String a_sFileName, String a_sUniqueID) : MyEntity::MyEntity(a_sFileName, a_sUniqueID)
{
	m_uXYIndex = index;
	m_vPlaneColor = C_YELLOW;

	m_bGridPlaneVisible = false;

	// By default, all blocks are weighted the same.
	m_uWeight = 1;

	ResetAStar();
}

Block::~Block()
{
	SafeDelete(m_pPreviousBlock);

	// Call base desctructor
	MyEntity::~MyEntity();
}

void Block::ResetAStar()
{
	m_vPlaneColor = C_YELLOW;
	m_bGridPlaneVisible = false;
	m_bPermanent = false;
	m_pPreviousBlock = nullptr;
	m_fDistanceFromStart = FLT_MAX;
	m_fHeuristicCost = 0.0f;
}

void Block::AddToRenderList()
{
	// Render via the base method
	MyEntity::AddToRenderList(false);

	// Render the top panel if visibility enabled
	if (m_bGridPlaneVisible)
	{
		matrix4 plane_matrix = GetModelMatrix();
		plane_matrix = glm::translate(plane_matrix, vector3(0.5f, 1.1f, 0.5f));
		plane_matrix = glm::rotate(plane_matrix, (float)(PI / -2), AXIS_X);
		plane_matrix = glm::scale(plane_matrix, vector3(0.5f));
		MeshManager::GetInstance()->AddPlaneToRenderList(plane_matrix, m_vPlaneColor);
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

void Simplex::Block::SetHeuristicCost(float cost)
{
	if (cost >= 0.0f)
		m_fHeuristicCost = cost;
}
float Simplex::Block::GetHeuristicCost()
{
	return m_fHeuristicCost;
}
// Set visibility of the plane drawn on top of block for pathfinding info
void Simplex::Block::SetGridPanelVisible(bool isVisible)
{
	m_bGridPlaneVisible = isVisible;
}

// Set the color of the plane drawn on top of block for pathfinding info
void Simplex::Block::SetGridPanelColor(vector3 color)
{
	m_vPlaneColor = color;
}

// Get the XY index of the block as if it were in a 2D array
UIntPair Block::GetXYIndex()
{
	return m_uXYIndex;
}

// Print 
void Block::PrintPath()
{
	if (m_pPreviousBlock)
		m_pPreviousBlock->PrintPath();
	else
		m_vPlaneColor = C_BLUE;

	m_bGridPlaneVisible = true;
}


