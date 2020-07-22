#ifndef _MYOCTANT_H_
#define _MYOCTANT_H_

#include "MyEntity.h"
#include "MyEntityManager.h"

class MyOctant
{
public:

	// The big 3 (why is it not called 4?)
	// Constructor
	MyOctant(MyOctant* parent, int totalDivisionLevels, int divisionLevel, int optimalObjects);

	// Copy Constructor
	MyOctant(MyOctant const& other);

	// Assignment Operator
	/*
	Octant& operator=(Octant const& other);
	*/

	// Destructor
	~MyOctant();

	// Accessors (I miss C# Properties)
	int GetDivisionLevel();

	// Are there any children?
	bool HasChildren();

	// Tells this octant to subdivide
	//void Subdivide();

	// Displays the Octants
	void DisplayOctant();

private:

	// References to managers
	Simplex::MeshManager* m_meshManager = nullptr;
	Simplex::MyEntityManager* m_entityManager = nullptr;

	// Min/Max Coordinates of the octant
	Simplex::vector3 m_minimumCoordinates;
	Simplex::vector3 m_maximumCoordinates;

	// Dimensions of the cuboid representing this Octant
	Simplex::vector3 m_cuboidDimensions;

	// Centerpoint for this octant
	Simplex::vector3 m_CenterPoint;

	// Indecies of entities contained in this octant
	std::vector<Simplex::uint> m_entityVector;

	// The number of children that an octant should have, if it has children
	const int m_MaxSubdivisions = 8;

	// The current count of subdivisions; might just use this in a loop, or use the count of a list.
	int m_currentSubdivisions;

	// The total divisions that this Octant (and the others in the structure) should account for
	int m_totalDivisionLevels;

	// How many divisions down from the "top" we are
	int m_divisionLevel;

	// Optimal number of objects per octant; might not be used?
	int m_OptimalObjects;

	// The child octants: Might Consider making this an array instead of a vector because it will always be constant size?
	//std::vector<Octant*> m_octantVector;

	// Calculates the appropriate size for this octant
	void CalculateCuboidDimensions();

	// Add all entities within this subdivision's space to the vector
	void PopulateEntityVector();

	// Setup subdivision and take the steps to subdivide
	void Subdivide();
};

#endif // !_OCTANT_H_
