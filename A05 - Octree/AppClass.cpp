#include "AppClass.h"
#include "MyOctant.h"

using namespace Simplex;

int creeperCount = 7;
float creeperSpeed = 0.1f;

void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 25.0f, 25.0f), //Position
		vector3(0.0f, 0.0f, 0.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

//#ifdef DEBUG
//	uint uInstances = 900;
//#else
//	uint uInstances = 1849;
//#endif
//	int nSquare = static_cast<int>(std::sqrt(uInstances));
//	m_uObjects = nSquare * nSquare;
//	uint uIndex = -1;
//	for (int i = 0; i < nSquare; i++)
//	{
//		for (int j = 0; j < nSquare; j++)
//		{
//			uIndex++;
//			m_pEntityMngr->AddEntity("Minecraft\\Cube.obj");
//			vector3 v3Position = vector3(glm::sphericalRand(34.0f));
//			matrix4 m4Position = glm::translate(v3Position);
//			m_pEntityMngr->SetModelMatrix(m4Position);
//		}
//	}
	m_pEntityMngr->AddEntity("Minecraft\\Steve.obj", "Steve");
	vector3 v3Position = vector3(0.0f, 0.0f, 10.0f);
	matrix4 m4Position = glm::translate(v3Position);
	m4Position = glm::rotate(m4Position, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_pEntityMngr->SetModelMatrix(m4Position);

	m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", "Floor");
	vector3 v3PositionCube = vector3(-25.0f, -51.0f, -40.0f);
	matrix4 m4PositionCube = glm::translate(v3PositionCube) * glm::scale(vector3(50.0f));
	m_pEntityMngr->SetModelMatrix(m4PositionCube);

	//String finishID = "Finish";
	m_pEntityMngr->AddEntity("Minecraft\\Steve.obj", "Finish");
	vector3 finishCoords = vector3(0, 0, -20);
	m4Position = glm::translate(finishCoords);
	m4Position = glm::rotate(m4Position, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_pEntityMngr->SetModelMatrix(m4Position);


	for (size_t i = 0; i < creeperCount; i++)
	{
		//Create creeper
		m_pEntityMngr->AddEntity("Minecraft\\Creeper.obj", ("Creeper" + std::to_string(i)));

		//Assign position and rotation
		vector3 v3PositionCreeper;
		matrix4 m4PositionCreeper;
		if (i % 2 == 0)
		{
			v3PositionCreeper = vector3(10.0f, 0.0f, 5.0f - (i * 6));
			m4PositionCreeper = glm::translate(v3PositionCreeper);
			m4PositionCreeper = glm::rotate(m4PositionCreeper, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		}
		else
		{
			v3PositionCreeper = vector3(-10.0f, 0.0f, 5.0f - (i * 6));
			m4PositionCreeper = glm::translate(v3PositionCreeper);
			m4PositionCreeper = glm::rotate(m4PositionCreeper, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		
		//Apply position and rotation
		m_pEntityMngr->SetModelMatrix(m4PositionCreeper);
	}

	/*m_pEntityMngr->AddEntity("Minecraft\\Creeper.obj", "Creeper1");
	vector3 v3PositionCreeper = vector3();
	matrix4 m4PositionCreeper = glm::translate(v3Position);
	m4PositionCreeper = glm::rotate(m4PositionCreeper, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_pEntityMngr->SetModelMatrix(m4PositionCreeper);*/

	m_uOctantLevels = 1;
	m_pEntityMngr->Update();
	m_pRoot = new MyOctant(m_uOctantLevels, 5);
}
void Application::Update(void)
{
	//Move each creeper forward
	for (size_t i = 0; i < creeperCount; i++)
	{
		matrix4 model = m_pEntityMngr->GetModelMatrix("Creeper" + std::to_string(i)) * glm::translate(vector3(0.0f, 0.0f, creeperSpeed));
		m_pEntityMngr->SetModelMatrix(model, "Creeper" + std::to_string(i));
	}

	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Rebuild octree
	fDelta = m_pSystem->GetDeltaTime(0);
	if (fDelta%2 == 0)
	{
		m_pEntityMngr->ClearDimensionSetAll();
		SafeDelete(m_pRoot);
		m_pRoot = new MyOctant(m_uOctantLevels, 5);
	}

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	//CameraRotation();
	
	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//display octree
	m_pRoot->Display();
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
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
	//release GUI
	ShutdownGUI();

	SafeDelete(m_pRoot);
}