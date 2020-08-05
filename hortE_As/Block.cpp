#include "Block.h"

using namespace Simplex;
Block::Block(UIntPair index, String a_sFileName, String a_sUniqueID) : MyEntity::MyEntity(a_sFileName, a_sUniqueID)
{
	m_uXYIndex = index;
	m_vPlaneColor = C_RED;

	m_fHeuristicCost = 0.0f;
	m_vNeighborList = std::vector<UIntPair>();
}

void Block::AddToRenderList(bool rigidBodyVisible, bool gridPlaneVisible)
{
	// Render via the base method
	MyEntity::AddToRenderList(rigidBodyVisible);

	// Render the top panel if visibility enabled
	if (gridPlaneVisible)
		MeshManager::GetInstance()->AddPlaneToRenderList(GetModelMatrix(), C_RED);
}

void Block::AddToNeighborList(UIntPair newNeigbor)
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

void Simplex::Block::SetHeuristicCost(float cost)
{
	if (cost >= 0.0f)
		m_fHeuristicCost = cost;
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


