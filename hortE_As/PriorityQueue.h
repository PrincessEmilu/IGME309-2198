#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <vector>
#include "MyEntity.h"

namespace Simplex
{
	typedef std::pair<uint, uint> UIntPair;
class PriorityQueue
{
public:
	// Constructor
	PriorityQueue();

	// Add a new "node"
	void AddNode(UIntPair);

private:
	// The member variable representing the actual data collection
	std::vector<UIntPair> m_vQueue;
};
}

#endif // PRIORITYQUEUE_H
