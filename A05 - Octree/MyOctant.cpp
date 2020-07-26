#include "MyOctant.h"

static Simplex::uint totalDimensions = 0;

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

Simplex::vector3 MyOctant::GetNewOctantColor(int index)
{
	switch (index)
	{
	case 1:
		return Simplex::C_RED;

	case 2:
		return Simplex::C_ORANGE;

	case 3:
		return Simplex::C_YELLOW;

	case 4:
		return Simplex::C_GREEN;

	case 5:
		return Simplex::C_BLUE;

	case 6:
		return Simplex::C_BLUE_CORNFLOWER;

	case 7:
		return Simplex::C_VIOLET;

	case 8:
		return Simplex::C_BLACK;

	default:
		return Simplex::C_WHITE;
	}
}

void MyOctant::SetCenterPoint(Simplex::vector3 center)
{
	m_CenterPoint = center;
}

MyOctant::MyOctant(MyOctant* parent, std::vector<Simplex::vector3> centerPointCloud, int subDivisionIndex, int divisionLevel, int totalDivisionLevels)
{
	// Incremense the static total dimensions variable
	totalDimensions++;

	m_isVisible = true;

	// Assign member variables
	m_parentOctant = parent;
	m_dimension = totalDimensions;
	m_divisionLevel = divisionLevel;
	m_totalDivisionLevels = totalDivisionLevels;
	m_subDivisionIndex = subDivisionIndex;
	m_centerpointCloud = centerPointCloud;

	// Get manager instances
	m_meshManager = Simplex::MeshManager::GetInstance();
	m_entityManager = Simplex::MyEntityManager::GetInstance();

	// Init collections
	m_childrenVector = std::vector<MyOctant*>();
	m_entityVector = std::vector<Simplex::uint>();

	// Make a new rigidbody using the entity centerpoints
	m_rigidBody = new Simplex::MyRigidBody(m_centerpointCloud);

	// Will this object subdivide?
	bool isLeafNode = !(m_divisionLevel < m_totalDivisionLevels);

	// If this is the first octree, calculate its dimensions
	if (m_parentOctant == nullptr)
		CalculateFirstCuboidDimensions();

	// Else, this is a child octant from another octant and must be constructed a little differently
	else
	{
		CalculateChildCuboidDimensions(subDivisionIndex);

		// If this octant is a leaf node, populate the entity vector
		if (isLeafNode)
			PopulateEntityVector();
	}

	// Create more octants if we need to go deeper!
	if (!isLeafNode)
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
	SafeDelete(m_rigidBody);

	// Managers become nullpointers; don't delete because the appclass takes care of that
	m_entityManager = nullptr;
	m_meshManager = nullptr;

	// Delete the children
	if (HasChildren())
	{
		for (auto octant : m_childrenVector)
			SafeDelete(octant);
	}

	// Reset total dimensions
	totalDimensions = 0;
}

Simplex::uint MyOctant::GetDimension()
{
	return m_dimension;
}

Simplex::vector3 MyOctant::GetCuboidDimensions()
{
	return m_cuboidDimensions;
}

Simplex::MyRigidBody* MyOctant::GetRigidBody()
{
	return m_rigidBody;
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
	if (!m_isVisible)
		return;

	// Add this octant to the render list
	if (m_rigidBody)
		m_rigidBody->AddToRenderList();

	// If there are children, add the children
	if (HasChildren())
	{
		for (auto octant : m_childrenVector)
			octant->DisplayOctant();
	}
}

void MyOctant::Set_Visible(bool visible)
{
	m_isVisible = visible;

	if (HasChildren())
	{
		for (auto child : m_childrenVector)
			child->Set_Visible(false);
	}
}

void MyOctant::CalculateFirstCuboidDimensions()
{
	// Set this octant's dimensions based on the min/max coordinates of the rigidbody
	m_cuboidDimensions.x = m_rigidBody->GetMaxGlobal().x + std::abs(m_rigidBody->GetMinGlobal().x);
	m_cuboidDimensions.y = m_rigidBody->GetMaxGlobal().y + std::abs(m_rigidBody->GetMinGlobal().y);
	m_cuboidDimensions.z = m_rigidBody->GetMaxGlobal().z + std::abs(m_rigidBody->GetMinGlobal().z);

	m_rigidBody->GetMaxGlobal() + m_rigidBody->GetMinGlobal();
}

void MyOctant::CalculateChildCuboidDimensions(int octantSegment)
{
	// Cube should be a fraction of the parent
	auto parentDimensions = m_parentOctant->GetCuboidDimensions();

	m_cuboidDimensions.x = parentDimensions.x / 2;
	m_cuboidDimensions.y = parentDimensions.y / 2;
	m_cuboidDimensions.z = parentDimensions.z / 2;

	// Get the correct offset for this octant
	Simplex::vector3 octantOffset = GetOctantPositionVector(m_subDivisionIndex);

	// Set the start center of model matrix to parent center
	m_rigidBody->SetModelMatrix(glm::translate(m_parentOctant->GetRigidBody()->GetCenterGlobal()));

	// Translate the rigidbody to the correct position
	m_rigidBody->SetModelMatrix(glm::translate(m_rigidBody->GetModelMatrix(), Simplex::vector3((m_cuboidDimensions.x / 2) * octantOffset.x, (m_cuboidDimensions.y / 2) * octantOffset.y, (m_cuboidDimensions.z / 2) * octantOffset.z)));
	
	// Scale it down based on the current dimension level
	for (Simplex::uint i = 0; i < m_divisionLevel - 1; i++)
		m_rigidBody->SetModelMatrix(glm::scale(m_rigidBody->GetModelMatrix(), Simplex::vector3(0.5f, 0.5f, 0.5f)));

	// Set the octant's known centerpoint
	SetCenterPoint(m_rigidBody->GetCenterGlobal());
}

void MyOctant::PopulateEntityVector()
{
	Simplex::uint entityCount = m_entityManager->GetEntityCount();

	for (Simplex::uint i = 0; i < entityCount; i++)
	{
		// If the entity's rigid body is colliding with this one, add it to the entity list
		if (m_rigidBody->IsColliding(m_entityManager->GetEntity(i)->GetRigidBody()))
		{
			m_entityVector.push_back(i);
			m_entityManager->GetEntity(i)->AddDimension(totalDimensions);
		}
	}
	std::cout << "Entity Count of octant " << m_dimension << " : " << m_entityVector.size() << "\n";
}

void MyOctant::Subdivide()
{
	for (Simplex::uint i = 0; i < m_MaxSubdivisions; i++)
	{
		m_childrenVector.push_back(new MyOctant(this, m_centerpointCloud, i, m_divisionLevel + 1, m_totalDivisionLevels));

		// Change color for each octant, for debugging (and it looks neat)
		m_childrenVector.back()->GetRigidBody()->SetColorColliding(GetNewOctantColor(i + 1));
		m_childrenVector.back()->GetRigidBody()->SetColorNotColliding(GetNewOctantColor(i + 1));
	}
}
