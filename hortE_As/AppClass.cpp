#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	// Seed the random number generator
	srand(time(0));

	// Set default start and end block coordinates
	m_uStartBlockCoords = UIntPair(0, 0);
	m_uEndBlockCoords = UIntPair(m_uGridSize - 1, m_uGridSize - 1);
	m_uPreviousStartCoords = m_uStartBlockCoords;
	m_uPreviousEndCoords = m_uEndBlockCoords;

	// Init obstacle collection
	m_uTotalObstacles = 0;
	m_vObstacles = std::vector<MyEntity*>();

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 5.0f, 25.0f), //Position
		vector3(0.0f, 0.0f, 0.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

	// Create steve
	m_pEntityMngr->AddEntity("Minecraft\\Steve.obj", "Steve");
	m_pEntityMngr->UsePhysicsSolver();

	// Create the zombie
	m_pZombie = new Zombie("Minecraft\\Zombie.obj", "Zombie");
	m_pEntityMngr->AddExistingEntity(m_pZombie);
	m_pEntityMngr->UsePhysicsSolver();

	// Generates a new grid
	m_pBlockGrid->GenerateNewGrid(m_uGridSize);

	// Generate obstacles onto the grid
	for (int i = 0; i < m_uMaxObstacles / 2; i++)
		GenerateObstacle();
}

void Application::GenerateObstacle()
{
	m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", "Cube_" + std::to_string(m_uTotalObstacles));
	vector3 v3Position = vector3(glm::sphericalRand(12.0f));
	v3Position.y = 0.0f;
	matrix4 m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position * glm::scale(vector3(0.75f)));
	m_pEntityMngr->UsePhysicsSolver();
	m_vObstacles.push_back(m_pEntityMngr->GetEntity(-1));
	m_uTotalObstacles++;
}
void Application::AddObstacle()
{
	// Can't have too many
	if (m_uTotalObstacles == 30)
		return;

	//Create a new obstacle
	GenerateObstacle();
}

void Application::RemoveObstacle()
{
	// Can't have negative obstacles
	if (m_uTotalObstacles == 0)
		return;

	--m_uTotalObstacles;
	m_pEntityMngr->RemoveEntity(m_vObstacles.back()->GetUniqueID());
	m_vObstacles.pop_back();
}

void Application::HandleAStar()
{
	// Get a new end block(where the movable character is)
		UIntPair newEndBlockCoords = m_pBlockGrid->GetCollidingBlock(m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Steve")));
	if (newEndBlockCoords != m_uEndBlockCoords)
	{
		m_uEndBlockCoords = newEndBlockCoords;
		m_uStartBlockCoords = m_pBlockGrid->GetCollidingBlock(m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Zombie")));
	}

	// Stores the actual positions of each block in the path
	auto positionsVector = std::vector<vector3>();

	// Calculate A* if necesary
	if (m_uEndBlockCoords != m_uPreviousEndCoords)
	{
		// Reset various values
		m_pBlockGrid->SetAllBlockWeights(m_vObstacles);
		m_uPreviousStartCoords = m_uStartBlockCoords;
		m_uPreviousEndCoords = m_uEndBlockCoords;

		// Get the path by running A* and get positions for the zombie to follow
		auto pathVector = m_pBlockGrid->CalculateAStarPath(m_uStartBlockCoords, m_uEndBlockCoords);
		for (uint i = 0; i < pathVector.size(); ++i)
			positionsVector.push_back(m_pBlockGrid->GetBlockWorldPosition(pathVector[i]));

		// Give the zombie the new path to follow
		m_pZombie->SetPositionsVector(positionsVector);
	}

}

void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	// Move the zombie
	m_pZombie->MoveZombie();

	//Update Entity Manager
	m_pEntityMngr->Update();

	HandleAStar();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	// Render BlockGrid
	m_pBlockGrid->Render();

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//draw gui,
	DrawGUI();

	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	// Nullify the obstacle vector
	for (auto obstacle : m_vObstacles)
	{
		obstacle = nullptr;
	}

	// Null the zombie
	m_pZombie = nullptr;

	//Release MyEntityManager
	MyEntityManager::ReleaseInstance();

	// Release the BlockGrid
	BlockGrid::ReleaseInstance();

	//release GUI
	ShutdownGUI();
}