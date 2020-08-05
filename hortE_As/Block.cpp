#include "Block.h"

using namespace Simplex;
Block::Block(std::pair<uint, uint> index, String a_sFileName, String a_sUniqueID) : MyEntity::MyEntity(a_sFileName, a_sUniqueID)
{
	m_uXYIndex = index;
	m_vPlaneColor = C_RED;
}

void Block::AddToRenderList(bool rigidBodyVisible, bool gridPlaneVisible)
{
	// Render via the base method
	MyEntity::AddToRenderList(rigidBodyVisible);

	// Render the top panel if visibility enabled
	if (gridPlaneVisible)
		MeshManager::GetInstance()->AddPlaneToRenderList(GetModelMatrix(), C_RED);
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
std::pair<uint, uint> Block::GetXYIndex()
{
	return m_uXYIndex;
}


