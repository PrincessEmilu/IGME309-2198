#ifndef BLOCK_H
#define BLOCK_H

#include "MyEntity.h"

namespace Simplex
{
	typedef std::pair<uint, uint> UIntPair ;
class Block : public MyEntity
{
public:
	// Constructor, similar to base but inits some other variables
	Block(UIntPair index, String a_sFileName, String a_sUniqueID = "NA");

	// Returns the XY pair representing where this block is
	UIntPair GetXYIndex();

	// Add this block to the render list
	void AddToRenderList(bool rigidBodyVisible, bool gridPlaneVisible);

	// Add a new neighbor to the neighbor list
	void AddToNeighborList(UIntPair newNeighbor);

	// Set the heuristics cost
	void SetHeuristicCost(float cost);

	// Set visibility of the grid panel
	void SetGridPanelVisible(bool isVisible);

	// Set the color of the grid panel
	void SetGridPanelColor(vector3 color);

private:
	// The array position for this block
	UIntPair m_uXYIndex;

	// The list of neighbots this block has, by XY index
	std::vector<UIntPair> m_vNeighborList;

	// Heuristic Cost
	float m_fHeuristicCost;

	// Weight
	uint m_uWeight;

	// Should the top plane be visible
	bool m_bGridPlaneVisible;

	// Color to draw the top plane in
	vector3 m_vPlaneColor;
};
}
#endif // BLOCK_H
