#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "MyEntity.h"

namespace Simplex
{
class Zombie : public MyEntity
{
public:
	// Constructor, will just use the default
	Zombie(String a_sFileName, String a_sUniqueID = "NA");

	// Move the zombie, should be called every updated
	void MoveZombie();

	// Set the positions vector for the zombie to follow
	void SetPositionsVector(std::vector<vector3>);

private:
	// The next index in the path the zombie should move to
	uint m_iNextPathPositionIndex;

	// The vector containing the positions the zombie should move to
	std::vector<vector3> m_vPathPositions;
};
}
#endif // !ZOMBIE_H
