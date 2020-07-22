#ifndef __OCTANT_H_
#define __OCTANT_H_

#include "MyEntity.h"

class Octant
{
public:

	// The big 3 (why is it not called 4?)
	// Constructor
	Octant(int totalDivisionLevels, int divisionLevel, int optimalObjects);

	// Copy Constructor
	Octant(Octant const& other);

	// Assignment Operator
	/*
	Octant& operator=(Octant const& other);
	*/

	// Destructor
	~Octant();

	// Accessors (I miss C# Properties)
	int GetDivisionLevel();

	// Are there any children?
	bool HasChildren();

	// Tells this octant to subdivide
	//void Subdivide();

	// Displays the Octants
	void DisplayOctant();

private:

	Simplex::MeshManager* m_meshManager;

	// Min/Max Coordinates of the octant
	Simplex::vector3 m_minimumCoordinates;
	Simplex::vector3 m_maximumCoordinates;

	// Center Point of the octant
	Simplex::vector3 m_centerPoint;

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

	// The entities that are in this octant
	//std::vector<Simplex::MyEntity*> m_entityVector;

	// The child octants: Might Consider making this an array instead of a vector because it will always be constant size?
	//std::vector<Octant*> m_octantVector;

	// Setup subdivision and take the steps to subdivide
	void Subdivide();

	// Add all entities within this subdivision's space to the vector
	void PopulateEntityVector();
};

#endif //__OCTANT_H_

