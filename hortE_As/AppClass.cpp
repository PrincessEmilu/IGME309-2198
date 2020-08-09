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
	m_uTotalObstacles = 30;
	m_vObstacles = std::vector<MyEntity*>();

	// Init empty path
	m_vPathPositions = std::vector<vector3>();

	// Default "next" position for zombie
	m_iNextZombiePositionIndex = -1;

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 5.0f, 25.0f), //Position
		vector3(0.0f, 0.0f, 0.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

	m_pEntityMngr->AddEntity("Minecraft\\Steve.obj", "Steve");
	m_pEntityMngr->UsePhysicsSolver();
	m_pEntityMngr->AddEntity("Minecraft\\Zombie.obj", "Zombie");
	m_pEntityMngr->UsePhysicsSolver();

	m_pEntityMngr->SetModelMatrix(glm::translate(vector3(-1.0f, 0.0f, -1.0f)), "Zombie");
	m_pEntityMngr->SetModelMatrix(glm::scale(vector3(0.5f, 0.5f, 0.5f)));

	m_pBlockGrid->GenerateNewGrid(m_uGridSize);

	for (int i = 0; i < m_uTotalObstacles; i++)
	{
		m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", "Cube_" + std::to_string(i));
		vector3 v3Position = vector3(glm::sphericalRand(12.0f));
		v3Position.y = 0.0f;
		matrix4 m4Position = glm::translate(v3Position);
		m_pEntityMngr->SetModelMatrix(m4Position * glm::scale(vector3(0.75f)));
		m_pEntityMngr->UsePhysicsSolver();
		m_vObstacles.push_back(m_pEntityMngr->GetEntity(-1));
	}
}

void Application::AddObstacle()
{

}

void Application::RemoveObstacle()
{
	if (m_uTotalObstacles == 0)
		return;

	--m_uTotalObstacles;
	m_pEntityMngr->RemoveEntity(m_vObstacles.back()->GetUniqueID());
	m_vObstacles.pop_back();
}

void Application::MoveZombie()
{
	// If there are no positions or the path is complete, return
	if (m_iNextZombiePositionIndex == -1 || m_iNextZombiePositionIndex == m_vPathPositions.size())
		return;

	// TODO: LERP the zombie to the next block - but not toward the Y of that block
	vector3 start = m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Zombie"))->GetPosition();
	float endX = m_vPathPositions[m_iNextZombiePositionIndex].x;
	float endY = start.y;
	float endZ = m_vPathPositions[m_iNextZombiePositionIndex].z;
	vector3 end = vector3(endX, endY, endZ);
	vector3 position = glm::lerp(start, end, 0.2f);
	matrix4 mat4 = glm::translate(IDENTITY_M4, position);
	m_pEntityMngr->SetModelMatrix(mat4, "Zombie");

	// Go to next block if arived at the current target
	if (glm::distance(m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Zombie"))->GetPosition(), end) < 0.001f)
		++m_iNextZombiePositionIndex;
}

void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager
	m_pEntityMngr->Update();

	// Move the zombie to the next spot on the path
	MoveZombie();

	// Get a new end block (where the movable character is)
	UIntPair newEndBlockCoords = m_pBlockGrid->GetCollidingBlock(m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Steve")));
	if (newEndBlockCoords != m_uEndBlockCoords)
	{
		m_uEndBlockCoords = newEndBlockCoords;
		m_uStartBlockCoords = m_pBlockGrid->GetCollidingBlock(m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Zombie")));
	}

	// Calculate A* if necesary
	if (m_uEndBlockCoords != m_uPreviousEndCoords)
	{
		// Reset various values
		m_vPathPositions.clear();
		m_iNextZombiePositionIndex = 0;
		m_pBlockGrid->SetAllBlockWeights(m_vObstacles);
		m_uPreviousStartCoords = m_uStartBlockCoords;
		m_uPreviousEndCoords = m_uEndBlockCoords;

		// Get the path by running A* and get positions for the zombie to follow
		auto pathVector = m_pBlockGrid->CalculateAStarPath(m_uStartBlockCoords, m_uEndBlockCoords);
		for (uint i = 0; i < pathVector.size(); ++i)
			m_vPathPositions.push_back(m_pBlockGrid->GetBlockWorldPosition(pathVector[i]));
	}

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

	//Release MyEntityManager
	MyEntityManager::ReleaseInstance();

	// Release the BlockGrid
	BlockGrid::ReleaseInstance();

	//release GUI
	ShutdownGUI();
}