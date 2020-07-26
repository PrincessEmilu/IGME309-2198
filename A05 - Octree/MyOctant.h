#ifndef _MYOCTANT_H_
#define _MYOCTANT_H_

#include "MyEntity.h"
#include "MyEntityManager.h"

class MyOctant
{
#define TOP_LEFT_REAR_VECTOR Simplex::vector3(-1.0f, 1.0f, -1.0f)
#define BOTTOM_LEFT_REAR_VECTOR Simplex::vector3(-1.0f, -1.0f, -1.0f)

#define BOTTOM_RIGHT_REAR_VECTOR Simplex::vector3(1.0f, -1.0f, -1.0f)
#define TOP_RIGHT_REAR_VECTOR Simplex::vector3(1.0f, 1.0f, -1.0f)

#define TOP_LEFT_FRONT_VECTOR Simplex::vector3(-1.0f, 1.0f, 1.0f)
#define BOTTOM_LEFT_FRONT_VECTOR Simplex::vector3(-1.0f, -1.0f, 1.0f)

#define TOP_RIGHT_FRONT_VECTOR Simplex::vector3(1.0f, 1.0f, 1.0f)
#define BOTTOM_RIGHT_FRONT_VECTOR Simplex::vector3(1.0f, -1.0f, 1.0f)

public:

	enum OctantSegments
	{
		TOP_LEFT_FRONT = 0,
		BOTTOM_LEFT_FRONT,
		TOP_RIGHT_FRONT,
		BOTTOM_RIGHT_FRONT,
		TOP_LEFT_REAR,
		BOTTOM_LEFT_REAR,
		TOP_RIGHT_REAR,
		BOTTOM_RIGHT_REAR
	};

	// The big 3 (why is it not called 4?)
	// Constructor
	MyOctant(MyOctant* parent, std::vector<Simplex::vector3> centerPointCloud, int subDivisionIndex, int divisionLevel, int totalDivisionLevels);

	// Copy Constructor
	MyOctant(MyOctant const& other);

	// Assignment Operator
	/*
	Octant& operator=(Octant const& other);
	*/

	// Destructor
	~MyOctant();

	// Accessors (I miss C# Properties) //

	// Get the dimension of this octant
	Simplex::uint GetDimension();

	// Get the dimenions of this octant's cuboid
	Simplex::vector3 GetCuboidDimensions();

	// Get the rigidbody
	Simplex::MyRigidBody* GetRigidBody();

	// Get this octant's list of entity indecies
	const std::vector<Simplex::uint>& GetEntityVector();

	// Are there any children?
	bool HasChildren();

	// Tells this octant to subdivide
	//void Subdivide();

	// Displays the Octants
	void DisplayOctant();

	// Sets isVisible
	void Set_Visible(bool visible);

private:

	//The rigidbody of this octant
	Simplex::MyRigidBody* m_rigidBody = nullptr;

	//Child Octants (nodes)
	std::vector<MyOctant*> m_childrenVector;

	// References to managers
	Simplex::MeshManager* m_meshManager = nullptr;
	Simplex::MyEntityManager* m_entityManager = nullptr;

	// Cloud of centerpoints for the entites
	std::vector<Simplex::vector3> m_centerpointCloud;

	// Dimensions of the cuboid representing this Octant
	Simplex::vector3 m_cuboidDimensions;

	// Centerpoint for this octant
	Simplex::vector3 m_CenterPoint;

	// Indecies of entities contained in this octant
	std::vector<Simplex::uint> m_entityVector;

	// The parent of this Octant
	MyOctant* m_parentOctant;

	// The number of children that an octant should have, if it has children
	const int m_MaxSubdivisions = 8;

	// Index of the octant's position vector
	int m_subDivisionIndex;

	// The current count of subdivisions; might just use this in a loop, or use the count of a list.
	int m_dimension;

	// How many divisions down from the "top" we are
	int m_divisionLevel;

	// The total divisions that this Octant (and the others in the structure) should account for
	int m_totalDivisionLevels;

	// Are octancs visible?
	bool m_isVisible;

	// Given an index, returns a vector representing the position of the octant relative to the parent
	Simplex::vector3 GetOctantPositionVector(int index);

	// Gets an octanct color for the given index
	Simplex::vector3 GetNewOctantColor(int index);

	// Set the center point
	void SetCenterPoint(Simplex::vector3 center);

	// Calculates the appropriate size for the first octant
	void CalculateFirstCuboidDimensions();

	// Calculate the appropriate size of a child octant
	void CalculateChildCuboidDimensions(int octantSegment);

	// Add all entites within this subdivision's space to the vector
	void PopulateEntityVector();

	// Setup subdivision and take the steps to subdivide
	void Subdivide();
};

#endif // !_OCTANT_H_
