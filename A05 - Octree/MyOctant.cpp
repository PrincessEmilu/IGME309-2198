#include "MyOctant.h"

MyOctant::MyOctant(MyOctant* parent, int totalDivisionLevels, int divisionLevel, int optimalObjects)
{
	// Get manager instances
	m_meshManager = Simplex::MeshManager::GetInstance();
	m_entityManager = Simplex::MyEntityManager::GetInstance();

	// Init collections
	m_entityVector = std::vector<Simplex::uint>();

	// Setup a brand-new OctTree
	if (parent == nullptr)
	{
		CalculateCuboidDimensions();
		PopulateEntityVector();
	}
	// Else, this is a child octant from another octant and must be constructed a little differently
	else
	{

	}
}

MyOctant::MyOctant(MyOctant const& other)
{
}

/*
Octant& Octant::operator=(Octant const& other)
{
	// TODO: insert return statement here
	return nullptr;
}
*/

MyOctant::~MyOctant()
{
	// Managers become nullpointers; don't delete because the appclass takes care of that
	m_entityManager = nullptr;
	m_meshManager = nullptr;
}

int MyOctant::GetDivisionLevel()
{
	return 0;
}

bool MyOctant::HasChildren()
{
	return false;
}

void MyOctant::DisplayOctant()
{
	int genCube = m_meshManager->GenerateCuboid(m_cuboidDimensions, Simplex::vector3(1.0f, 0.1f, 0.0f));
	m_meshManager->AddMeshToRenderList(m_meshManager->GetMesh(genCube), Simplex::IDENTITY_M4, Simplex::RENDER_WIRE);
}

void MyOctant::CalculateCuboidDimensions()
{
	// TODO: This should probably not be hardcoded to be the entity manager count
	Simplex::uint entityCount = m_entityManager->GetEntityCount();

	for (Simplex::uint i = 0; i < entityCount; i++)
	{
		// Get the current entity and its center point
		Simplex::MyEntity* entity = m_entityManager->GetEntity(i);
		Simplex::vector3 entityGCenter = entity->GetRigidBody()->GetCenterGlobal();

		// Check X
		if (entityGCenter.x < m_minimumCoordinates.x)
			m_minimumCoordinates.x = entityGCenter.x;
		else if (entityGCenter.x > m_maximumCoordinates.x)
			m_maximumCoordinates.x = entityGCenter.x;

		// Check Y
		if (entityGCenter.y < m_minimumCoordinates.y)
			m_minimumCoordinates.y = entityGCenter.y;
		else if (entityGCenter.y > m_maximumCoordinates.y)
			m_maximumCoordinates.y = entityGCenter.y;

		// Check Z
		if (entityGCenter.z < m_minimumCoordinates.z)
			m_minimumCoordinates.z = entityGCenter.z;
		else if (entityGCenter.z > m_maximumCoordinates.z)
			m_maximumCoordinates.z = entityGCenter.z;
	}

	// Set this octant's dimensions based on the min/max coordinates
	m_cuboidDimensions.x = m_maximumCoordinates.x + std::abs(m_minimumCoordinates.x);
	m_cuboidDimensions.y = m_maximumCoordinates.y + std::abs(m_minimumCoordinates.y);
	m_cuboidDimensions.z = m_maximumCoordinates.z + std::abs(m_minimumCoordinates.z);
}

void MyOctant::PopulateEntityVector()
{
	// TODO: This probably shouldn't be hardcoded here...
	Simplex::uint entityCount = m_entityManager->GetEntityCount();

	for (Simplex::uint i = 0; i < entityCount; i++)
	{
		Simplex::MyEntity* entity = m_entityManager->GetEntity(i);
		Simplex::vector3 entityGCenter = entity->GetRigidBody()->GetCenterGlobal();

		// If the center is within the min/max of this cuboid, add to this octant
		// TODO: This doesn't consider literal edge cases, which will be a bit annoying...
		if (entityGCenter.x >= m_minimumCoordinates.x && entityGCenter.x <= m_maximumCoordinates.x
			&& entityGCenter.y >= m_minimumCoordinates.y && entityGCenter.y <= m_maximumCoordinates.y
			&& entityGCenter.z >= m_minimumCoordinates.z && entityGCenter.z <= m_maximumCoordinates.z)
		{
			m_entityVector.push_back(i);
		}
	}

	std::cout << "\nEntity Count of octant: " << m_entityVector.size();
}

void MyOctant::Subdivide()
{

}
