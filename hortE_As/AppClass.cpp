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

	m_pBlockGrid->GenerateNewGrid(m_uGridSize);

	/*
	for (int i = 0; i < 100; i++)
	{
		m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", "Cube_" + std::to_string(i));
		vector3 v3Position = vector3(glm::sphericalRand(12.0f));
		v3Position.y = 0.0f;
		matrix4 m4Position = glm::translate(v3Position);
		m_pEntityMngr->SetModelMatrix(m4Position * glm::scale(vector3(2.0f)));
		m_pEntityMngr->UsePhysicsSolver();
		//m_pEntityMngr->SetMass(2);

		//m_pEntityMngr->SetMass(i+1);
	}
	*/
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
		m_pBlockGrid->CalculateAStarPath(m_uStartBlockCoords, m_uEndBlockCoords);
		m_uPreviousStartCoords = m_uStartBlockCoords;
		m_uPreviousEndCoords = m_uEndBlockCoords;
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
	//Release MyEntityManager
	MyEntityManager::ReleaseInstance();

	// Release the BlockGrid
	BlockGrid::ReleaseInstance();

	//release GUI
	ShutdownGUI();
}