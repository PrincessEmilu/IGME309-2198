#include "Block.h"

using namespace Simplex;
Block::Block(UIntPair index, String a_sFileName, String a_sUniqueID) : MyEntity::MyEntity(a_sFileName, a_sUniqueID)
{
	m_uXYIndex = index;
	m_vPlaneColor = C_RED;

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
	m_bPermanent = false;
	m_pPreviousBlock = nullptr;
	m_uDistanceFromStart = 100000000000; // Yes, I could use INT_MAX but a calculation I do can sometimes result in underflow... maybe it's a sign I should do it differently, but this works fine for this scale
	m_fHeuristicCost = 0.0f;
	m_vNeighborList = std::vector<Block*>();
}

void Block::AddToRenderList(bool rigidBodyVisible, bool gridPlaneVisible)
{
	// Render via the base method
	MyEntity::AddToRenderList(rigidBodyVisible);

	// Render the top panel if visibility enabled
	if (gridPlaneVisible)
		MeshManager::GetInstance()->AddPlaneToRenderList(GetModelMatrix(), C_RED);
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
	m_uDistanceFromStart = m_uWeight + m_fHeuristicCost;
	
	if (m_pPreviousBlock)
		m_uDistanceFromStart += m_pPreviousBlock->GetDistanceFromStart();
}

uint Simplex::Block::GetDistanceFromStart()
{
	return m_uDistanceFromStart;
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


