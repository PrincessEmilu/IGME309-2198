#ifndef BLOCK_H
#define BLOCK_H

#include "MyEntity.h"

namespace Simplex
{
class Block : public MyEntity
{
public:
	Block(std::pair<uint, uint> index, String a_sFileName, String a_sUniqueID = "NA");

	std::pair<uint, uint> GetXYIndex();

	void AddToRenderList(bool rigidBodyVisible, bool gridPlaneVisible);

private:
	// The array position for this block
	std::pair<uint, uint> m_uXYIndex;

	// Heuristic Cost
	uint m_uHeuristicCost;

	// Weight
	uint m_uWeight;
};
}
#endif // BLOCK_H
