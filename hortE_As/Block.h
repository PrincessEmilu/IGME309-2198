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

	// Destructor
	~Block();

	// Returns the XY pair representing where this block is
	UIntPair GetXYIndex();

	// Reset all AStar values
	void ResetAStar();

	// Add this block to the render list
	void AddToRenderList(bool rigidBodyVisible, bool gridPlaneVisible);

	// Add a new neighbor to the neighbor list
	void AddToNeighborList(Block* newNeighbor);

	// Gets the neighbor list
	std::vector<Block*> GetNeighborList();

	// Set the previous block
	void SetPreviousBlock(Block* previous_block);

	//Get the previous block
	Block* GetPreviousBlock();

	//Set Permanent
	void SetPermanent(bool isPermanent);

	// Get Permanent
	bool GetPermanent();

	// Set the total path cost
	void UpdateDistanceFromStart();

	// Gets the total path cost
	uint GetDistanceFromStart();

	// Set the heuristics cost
	void SetHeuristicCost(float cost);

	// Returns the heuristic cost
	float GetHeuristicCost();

	// Set visibility of the grid panel
	void SetGridPanelVisible(bool isVisible);

	// Set the color of the grid panel
	void SetGridPanelColor(vector3 color);

private:
	// The array position for this block
	UIntPair m_uXYIndex;

	// Is this block marked permanent for the A* search?
	bool m_bPermanent;

	// The previous block on this one's path
	Block* m_pPreviousBlock;

	// Containts the block's neighbors
	std::vector<Block*> m_vNeighborList;

	// Total cost to get here
	uint m_uDistanceFromStart;

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
