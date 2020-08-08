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

	// Returns the XZ pair representing where this block is
	UIntPair GetXZIndex();

	// Reset all AStar values
	void ResetAStar();

	// Add this block to the render list
	void AddToRenderList();

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

	// Set the weigth of this block
	void SetWeight(bool hasObstacle);

	// Set the heuristic cost
	void SetHeuristicCost(float cost);

	// Returns the Distance from start + heuristic cost
	float GetDistancePlusHeuristic();

	// Set visibility of the grid panel
	void SetBlockPlaneVisible(bool isVisible);

	// Set the color of the grid panel
	void SetBlockPlaneColor(vector3 color);

	// Recursively print the xy pairs
	std::vector<UIntPair> GetCalculatedPath();

	// Constants
	// Weight values
	const uint m_uDefaultWeight = 1; // No obstacle
	const uint m_uObstacleWeight = 50; // Has obstacles

	// Plane colors
	const vector3 m_vStartPlaneColor = C_BLUE;
	const vector3 m_vEndPlaneColor = C_GREEN;
	const vector3 m_vPathPlaneColor = C_YELLOW;

private:
	// The array position for this block
	UIntPair m_uXZIndex;

	// Is this block marked permanent for the A* search?
	bool m_bPermanent;

	// The previous block on this one's path
	Block* m_pPreviousBlock;

	// Contains the block's neighbors
	std::vector<Block*> m_vNeighborList;

	// Total cost to get here
	float m_fDistanceFromStart;

	// Heuristic Cost
	float m_fHeuristicCost;

	// Weight
	uint m_uWeight;

	// Should the top plane be visible
	bool m_BlockPlaneVisible;

	// Color to draw the top plane in
	vector3 m_vCurrentPlaneColor;
};
}
#endif // BLOCK_H
