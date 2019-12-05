#include "AppClass.h"
#include "MyOctant.h"
using namespace Simplex;

int laneCount = 6;//Number of lanes of creepers
int mobCount = 0; //Total number of mobs spawned
float creeperSpeed = 0.15f; //How fast the creepers move
float cowSpeed = .30f; //How fast the cows move
int creeperInterval = 3; //How often to spawn creepers
int cowInterval = 3; //How often to spawn cows
float deltaTime = 0.0f;

time_t currentTime;
time_t cowCurrentTime;
int creeperTimeSpawned; //Time the creepers were last spawned
int cowTimeSpawned; //Time the cows were last spawned

void Application::InitVariables(void)
{
	m_sProgrammer = " Crosby, Grayson \n Ge, Annie \n Ratliff, Thomas \n Saha, Ankita";

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 25.0f, 25.0f), //Position
		vector3(0.0f, 0.0f, 0.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

															 
	m_pEntityMngr = MyEntityManager::GetInstance(); //Initialize the entity manager

	m_pEntityMngr->AddEntity("Minecraft\\Steve.obj", "Steve");
	vector3 v3Position = vector3(0.0f, 0.0f, 10.0f);
	matrix4 m4Position = glm::translate(v3Position);
	m4Position = glm::rotate(m4Position, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_pEntityMngr->SetModelMatrix(m4Position);
	m_pEntityMngr->UsePhysicsSolver(true);
	m_pEntityMngr->SetMass(1.5);
	m_pEntityMngr->SetPosition(vector3(0, 0, 10.0f));

	//Creating floor/walls
	m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", "LeftWall");
	vector3 v3PositionLeftWall = vector3(-75.0f, -5.0f, -40.0f);
	matrix4 m4PositionLeftWall = glm::translate(v3PositionLeftWall) * glm::scale(vector3(50.0f));
	m_pEntityMngr->SetModelMatrix(m4PositionLeftWall);
	m_pEntityMngr->SetMass(1.5);

	m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", "RightWall");
	vector3 v3PositionRightWall = vector3(25.0f, -5.0f, -40.0f);
	matrix4 m4PositionRightWall = glm::translate(v3PositionRightWall) * glm::scale(vector3(50.0f));
	m_pEntityMngr->SetModelMatrix(m4PositionRightWall);
	m_pEntityMngr->SetMass(1.5);

	m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", "Floor");
	vector3 v3PositionFloor = vector3(-25.0f, -51.0f, -40.0f);
	matrix4 m4PositionFloor = glm::translate(v3PositionFloor) * glm::scale(vector3(50.0f));
	m_pEntityMngr->SetModelMatrix(m4PositionFloor);
	m_pEntityMngr->SetMass(1.5);

	m_pEntityMngr->AddEntity("Minecraft\\Pig.obj", "Finish");
	vector3 v3PositionFinish = vector3(0.0f, 0.0f, -38.0f);
	matrix4 m4PositionFinish = glm::translate(v3PositionFinish) * glm::scale(vector3(3.0f));;
	m_pEntityMngr->SetModelMatrix(m4PositionFinish);
	m_pEntityMngr->SetMass(1.5);

	m_uOctantLevels = 1;
	m_pEntityMngr->Update();
	m_pRoot = new MyOctant(m_uOctantLevels, 5);
}
void Application::Update(void)
{
	deltaTime = ImGui::GetIO().Framerate / 1000;

	cowCurrentTime = time(NULL);
	currentTime = time(NULL);

	//Tried out a differnt way to implement time
	//float a = m_pSystem->GetTimeSinceStart(18);

	//if ((int)a % creeperInterval == 0.0f)
	//{

	//	for (size_t i = 0; i < laneCount; i++)
	//	{
	//		if (i % 2 == 0)
	//		{
	//			//Create creeper
	//			m_pEntityMngr->AddEntity("Minecraft\\Creeper.obj", ("Creeper" + std::to_string(mobCount)));

	//			//Assign its row
	//			m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Creeper" + std::to_string(mobCount)))->row = i;
	//			//Assign position and rotation
	//			vector3 v3PositionCreeper;
	//			float lanePosition = 5.0f - (i * 6); //z pos of where creeper is in current lane

	//			v3PositionCreeper = vector3(22.0f, 0.0f, lanePosition);

	//			//Apply position
	//			m_pEntityMngr->SetPosition(v3PositionCreeper);
	//			mobCount++;
	//			m_pEntityMngr->UsePhysicsSolver(true);
	//			//creeperTimeSpawned = currentTime;
	//		}
	//	}
	//}

	float playerZPos = m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Steve"))->GetPosition().z;
	if (currentTime >= creeperTimeSpawned + creeperInterval)
	{
		for (size_t i = 0; i < laneCount; i++)
		{
			if (i % 2 == 0)
			{
				//Create creeper
				m_pEntityMngr->AddEntity("Minecraft\\Creeper.obj", ("Creeper" + std::to_string(mobCount)));

				//Assign its row
				m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Creeper" + std::to_string(mobCount)))->row = i;
				//Assign position and rotation
				vector3 v3PositionCreeper;
				float lanePosition = 5.0f - (i * 6); //z pos of where creeper is in current lane

				v3PositionCreeper = vector3(22.0f, 0.0f, lanePosition);

				//Apply position
				m_pEntityMngr->SetPosition(v3PositionCreeper);
				mobCount++;
				m_pEntityMngr->UsePhysicsSolver(true);
				creeperTimeSpawned = currentTime;
			}
			else
			{
				//Create cow
				m_pEntityMngr->AddEntity("Minecraft\\Cow.obj", ("Cow" + std::to_string(mobCount)));

				//Assign its row
				m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Cow" + std::to_string(mobCount)))->row = i;

				//Assign position and rotation
				vector3 v3PositionCow;
				float lanePosition = 5.0f - (i * 6); //z pos of where cow is in current lane

				v3PositionCow = vector3(-22.0f, 0.0f, lanePosition);

				//Trying to make the cows rotate
				/*matrix4 m4PositionCow = glm::translate(v3PositionCow);
				m4PositionCow = glm::rotate(m4PositionCow, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 0.0f));*/
				cowTimeSpawned = cowCurrentTime;

				//Apply position
				m_pEntityMngr->SetPosition(v3PositionCow);
				mobCount++;
				m_pEntityMngr->UsePhysicsSolver(true);
			}
		}
	}

	//Update player's row
	for (size_t i = 0; i < laneCount; i++)
	{
		float lanePosition = 5.0f - (i * 6); //z pos of where creeper is in current lane
		if (playerZPos > (lanePosition - 2) && playerZPos < (lanePosition + 2))
		{
			m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Steve"))->row = i;
		}
	}

	////Move each creeper forward
	//for (size_t i = 0; i < creeperCount; i++)
	//{
		//Move each creeper forward
		for (size_t i = 0; i < mobCount; i++)
		{
			//Bounce mobs back into world bounds if knocked out
			if (m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Creeper" + std::to_string(i)))->GetPosition().z < -36.0f)
			{
				m_pEntityMngr->ApplyForce(vector3(0, 0, 2.0f), "Creeper" + std::to_string(i));
			}

			if (m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Cow" + std::to_string(i)))->GetPosition().z < -36.0f)
			{
				m_pEntityMngr->ApplyForce(vector3(0, 0, 2.0f), "Cow" + std::to_string(i));
			}

			if (m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Creeper" + std::to_string(i)))->GetPosition().z > 11.0f)
			{
				m_pEntityMngr->ApplyForce(vector3(0, 0, -2.0f), "Creeper" + std::to_string(i));
			}

			if (m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Cow" + std::to_string(i)))->GetPosition().z > 11.0f)
			{
				m_pEntityMngr->ApplyForce(vector3(0, 0, -2.0f), "Cow" + std::to_string(i));
			}

			if (i % 2 == 0)
			{
				//Move it left
				m_pEntityMngr->ApplyForce(vector3(-creeperSpeed * deltaTime, 0.0f, 0.0f), "Creeper" + std::to_string(i));
				//Delete it if it goes too far
				if (m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Creeper" + std::to_string(i)))->GetPosition().x < -24.0f)
				{
					m_pEntityMngr->RemoveEntity(m_pEntityMngr->GetEntityIndex("Creeper" + std::to_string(i)));
				}

			}
			else
			{
				//Move it right
				m_pEntityMngr->ApplyForce(vector3(cowSpeed * deltaTime, 0.0f, 0.0f), "Cow" + std::to_string(i));
				//Delete it if it goes too far
				if (m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Cow" + std::to_string(i)))->GetPosition().x > 24.0f)
				{
					m_pEntityMngr->RemoveEntity(m_pEntityMngr->GetEntityIndex("Cow" + std::to_string(i)));
				}
			}
		}
	//}

	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Rebuild octree
	fDelta = m_pSystem->GetDeltaTime(0);
	if (fDelta % 2 == 0)
	{
		m_pEntityMngr->ClearDimensionSetAll();
		SafeDelete(m_pRoot);
		m_pRoot = new MyOctant(m_uOctantLevels, 5);
	}

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

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
	
	//draw gui
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