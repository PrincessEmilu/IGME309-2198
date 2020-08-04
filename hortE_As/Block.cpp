#include "Block.h"

using namespace Simplex;
Block::Block(std::pair<uint, uint> index, String a_sFileName, String a_sUniqueID) : MyEntity::MyEntity(a_sFileName, a_sUniqueID)
{
	m_uXYIndex = index;
}

void Block::AddToRenderList(bool rigidBodyVisible, bool gridPlaneVisible)
{
	// Render via the base method
	MyEntity::AddToRenderList(rigidBodyVisible);

	// TODO: Render the top plane here!
}

std::pair<uint, uint> Block::GetXYIndex()
{
	return m_uXYIndex;
}


