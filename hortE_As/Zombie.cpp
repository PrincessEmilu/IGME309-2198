#include "Zombie.h"


using namespace Simplex;

// Just use the base constructor
Simplex::Zombie::Zombie(String a_sFileName, String a_sUniqueID) : MyEntity(a_sFileName, a_sUniqueID)
{
	m_iNextPathPositionIndex = -1; // Set to invalid so that it doesn't try moving while there's no path
	m_vPathPositions = std::vector<vector3>();
}

void Simplex::Zombie::MoveZombie()
{
	// If there are no positions or the path is complete, return
	if (m_iNextPathPositionIndex == -1 || m_iNextPathPositionIndex == m_vPathPositions.size())
		return;

	// LERP the zombie
	// The step (percentage) the zombie should move per lerp
	float step;
	if (m_iNextPathPositionIndex == 0)
		step = 0.5f;
	else
		step = 0.2f;

	// Define the values we want to LERP by
	vector3 start = GetPosition();
	float endX = m_vPathPositions[m_iNextPathPositionIndex].x;
	float endY = start.y;
	float endZ = m_vPathPositions[m_iNextPathPositionIndex].z;
	vector3 end = vector3(endX, endY, endZ);

	// Transform the zombie
	vector3 position = glm::lerp(start, end, step);
	matrix4 mat4 = glm::translate(IDENTITY_M4, position);
	SetModelMatrix(mat4);

	// Go to next block if arived at the current target
	if (glm::distance(GetPosition(), end) < 0.001f)
		++m_iNextPathPositionIndex;
}

void Simplex::Zombie::SetPositionsVector(std::vector<vector3> positionsVector)
{
	m_vPathPositions = positionsVector;
	m_iNextPathPositionIndex = 1;
}
