#include "MyOctant.h"

MyOctant::MyOctant(MyOctant* parent, int dimension, int divisionLevel, int totalDivisionLevels)
{
	// Assign member variables
	m_parentOctant = parent;
	m_matrix = Simplex::IDENTITY_M4;
	m_dimension = dimension;
	m_divisionLevel = divisionLevel;
	m_totalDivisionLevels = totalDivisionLevels;

	// Get manager instances
	m_meshManager = Simplex::MeshManager::GetInstance();
	m_entityManager = Simplex::MyEntityManager::GetInstance();

	// Init collections
	m_childrenVector = std::vector<MyOctant*>();
	m_entityVector = std::vector<Simplex::uint>();

	// Starting from scratch- this is the very first octant in the octree
	if (m_parentOctant == nullptr)
	{
		CalculateFirstCuboidDimensions();
		PopulateEntityVector();
	}
	// Else, this is a child octant from another octant and must be constructed a little differently
	else
	{
		CalculateChildCuboidDimensions(m_dimension / m_divisionLevel);
		PopulateEntityVector(m_parentOctant->GetEntityVector());
	}

	// Create more octants if we need to go deeper!
	if (m_divisionLevel < m_totalDivisionLevels)
		Subdivide();
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
	std::cout << "\nOctant Destructor Called";

	// Managers become nullpointers; don't delete because the appclass takes care of that
	m_entityManager = nullptr;
	m_meshManager = nullptr;

	// Delete the children
	if (HasChildren())
	{
		for (auto octant : m_childrenVector)
			SafeDelete(octant);
	}
}

Simplex::uint MyOctant::GetDimension()
{
	return m_dimension;
}

Simplex::vector3 MyOctant::GetCuboidDimensions()
{
	return m_cuboidDimensions;
}

const std::vector<Simplex::uint>& MyOctant::GetEntityVector()
{
	return m_entityVector;
}

bool MyOctant::HasChildren()
{
	return m_childrenVector.size() > 0;
}

void MyOctant::DisplayOctant()
{
	int genCube = m_meshManager->GenerateCuboid(m_cuboidDimensions, Simplex::vector3(1.0f, 0.1f, 0.0f));
	// TODO: Will have to handle adding a transform?
	// glm::translate(m_matrix, Simplex::vector3(20.0, 1.0, 1.0));
	m_meshManager->AddMeshToRenderList(m_meshManager->GetMesh(genCube), m_matrix, Simplex::RENDER_WIRE);

	if (HasChildren())
	{
		for (auto octant : m_childrenVector)
			octant->DisplayOctant();
	}
}

// Return a vector for the given Octant number
Simplex::vector3 MyOctant::GetOctantPositionVector(int index)
{
	switch (index)
	{
	case OctantSegments::BOTTOM_LEFT_FRONT:
		return BOTTOM_LEFT_FRONT_VECTOR;

	case OctantSegments::BOTTOM_LEFT_REAR:
		return BOTTOM_LEFT_REAR_VECTOR;

	case OctantSegments::BOTTOM_RIGHT_FRONT:
		return BOTTOM_RIGHT_FRONT_VECTOR;

	case OctantSegments::BOTTOM_RIGHT_REAR:
		return BOTTOM_RIGHT_REAR_VECTOR;

	case OctantSegments::TOP_LEFT_FRONT:
		return TOP_LEFT_FRONT_VECTOR;

	case OctantSegments::TOP_LEFT_REAR:
		return TOP_LEFT_REAR_VECTOR;

	case OctantSegments::TOP_RIGHT_FRONT:
		return TOP_RIGHT_FRONT_VECTOR;

	case OctantSegments::TOP_RIGHT_REAR:
		return TOP_RIGHT_REAR_VECTOR;

	default:
		return Simplex::vector3(0.0f);
	}
}

void MyOctant::CalculateFirstCuboidDimensions()
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

void MyOctant::CalculateChildCuboidDimensions(int octantSegment)
{
	// Cube should be a fraction of the parent
	auto parentDimensions = m_parentOctant->GetCuboidDimensions();

	auto quarterX = parentDimensions.x / 4.0f;
	auto quarterY = parentDimensions.y / 4.0f;
	auto quarterZ = parentDimensions.z / 4.0f;

	m_cuboidDimensions.x = parentDimensions.x / 2;
	m_cuboidDimensions.y = parentDimensions.y / 2;
	m_cuboidDimensions.z = parentDimensions.z / 2;

	// Get the correct offset for this octant
	Simplex::vector3 octantOffset = GetOctantPositionVector(m_dimension);

	// Translate this matrix from the identity
	m_matrix = glm::translate(m_matrix, Simplex::vector3(quarterX * octantOffset.x, quarterY * octantOffset.y, quarterZ * octantOffset.z));
}

void MyOctant::PopulateEntityVector()
{
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

	std::cout << "\nEntity Count of octant " << m_dimension << " : " << m_entityVector.size() << "\n";
}

void MyOctant::PopulateEntityVector(const std::vector<Simplex::uint>& entityVector)
{
	Simplex::uint entityCount = entityVector.size();

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

	std::cout << "\nEntity Count of octant " << m_dimension << " : " << m_entityVector.size() << "\n";
}

void MyOctant::Subdivide()
{
	std::cout << "\n\nSubdivide()\n\n";

	for (Simplex::uint i = 0; i < m_MaxSubdivisions; i++)
	{
		int childDimension = (m_dimension + 1) + i;
		m_childrenVector.push_back(new MyOctant(this, childDimension, m_divisionLevel + 1, m_totalDivisionLevels));
	}
}
