#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
Simplex::MyEntityManager* Simplex::MyEntityManager::m_pInstance = nullptr;
void Simplex::MyEntityManager::Init(void)
{
	m_uEntityCount = 0;
	m_mEntityArray = nullptr;
}
void Simplex::MyEntityManager::Release(void)
{
	for (uint uEntity = 0; uEntity < m_uEntityCount; ++uEntity)
	{
		MyEntity* pEntity = m_mEntityArray[uEntity];
		SafeDelete(pEntity);
	}
	m_uEntityCount = 0;
	m_mEntityArray = nullptr;
}
Simplex::MyEntityManager* Simplex::MyEntityManager::GetInstance()
{
	if(m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void Simplex::MyEntityManager::ReleaseInstance()
{
	if(m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	//look one by one for the specified unique id
	for (uint uIndex = 0; uIndex < m_uEntityCount; ++uIndex)
	{
		if (a_sUniqueID == m_mEntityArray[uIndex]->GetUniqueID())
			return uIndex;
	}
	//if not found return -1
	return -1;
}
//Accessors
Simplex::uint Simplex::MyEntityManager::GetEntityCount(void) {	return m_uEntityCount; }
Simplex::Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return nullptr;

	// if out of bounds
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->GetModel();
}
Simplex::Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		return pTemp->GetModel();
	}
	return nullptr;
}
Simplex::MyRigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return nullptr;

	// if out of bounds
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->GetRigidBody();
}
Simplex::MyRigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		return pTemp->GetRigidBody();
	}
	return nullptr;
}
Simplex::matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return matrix4();

	// if out of bounds
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->GetModelMatrix();
}
Simplex::matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		return pTemp->GetModelMatrix();
	}
	return IDENTITY_M4;
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->SetModelMatrix(a_m4ToWorld);
	}
}
void Simplex::MyEntityManager::SetAxisVisibility(bool a_bVisibility, uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->SetAxisVisible(a_bVisibility);
}
void Simplex::MyEntityManager::SetAxisVisibility(bool a_bVisibility, String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->SetAxisVisible(a_bVisibility);
	}
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	m_mEntityArray[a_uIndex]->SetModelMatrix(a_m4ToWorld);
}
//The big 3
Simplex::MyEntityManager::MyEntityManager(){Init();}
Simplex::MyEntityManager::MyEntityManager(MyEntityManager const& a_pOther){ }
Simplex::MyEntityManager& Simplex::MyEntityManager::operator=(MyEntityManager const& a_pOther) { return *this; }
Simplex::MyEntityManager::~MyEntityManager(){Release();};
// other methods
void Simplex::MyEntityManager::Update(void)
{
	//Clear all collisions
	for (uint i = 0; i < m_uEntityCount; i++)
	{
		m_mEntityArray[i]->ClearCollisionList();
	}
	
	//check collisions
	for (uint i = 0; i < m_uEntityCount; i++)
	{
		for (uint j = i + 1; j < m_uEntityCount; j++)
		{
			//if objects are colliding resolve the collision
			bool colliding = m_mEntityArray[i]->IsColliding(m_mEntityArray[j]);

			//Checking collisions on steve
			if (m_mEntityArray[i]->GetUniqueID() == "Steve")
			{
				//If other collider is a creeper, and it has the same row as steve
				if ((m_mEntityArray[j]->GetUniqueID().find("Creeper") != std::string::npos || m_mEntityArray[j]->GetUniqueID().find("Cow") != std::string::npos) && m_mEntityArray[i]->row == m_mEntityArray[j]->row)
				{
					m_mEntityArray[j]->GetRigidBody()->m_v3DisplayColor = C_BLUE;
					if (colliding == true && gameWin != true)
					{
						//Set game state
						gameLose = true;
						gameWin = false;

						//Rest easy young soldier (rotates steve)
						matrix4 m4Position = glm::translate(m_mEntityArray[i]->GetPosition());
						m4Position = glm::rotate(m4Position, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
						m_mEntityArray[i]->SetModelMatrix(m4Position);

						//Collision
						m_mEntityArray[i]->ResolveCollision(m_mEntityArray[j]);
						m_mEntityArray[i]->ApplyForce(100.0f * glm::normalize(m_mEntityArray[i]->GetPosition() - m_mEntityArray[j]->GetPosition()));
						m_mEntityArray[i]->ApplyForce(vector3(0, 30.0f, 0));
						m_mEntityArray[j]->ApplyForce(-20.0f * glm::normalize(m_mEntityArray[i]->GetPosition() - m_mEntityArray[j]->GetPosition()));
					}
				}
				else
				{
					m_mEntityArray[j]->GetRigidBody()->m_v3DisplayColor = C_YELLOW;
				}

				//Check for victory and set game state
				if (m_mEntityArray[j]->GetUniqueID() == "Finish")
				{
					if (colliding == true && gameLose != true)
					{
						gameWin = true;
						gameLose = false;
					}
				}
			}

			//Check when mobs collide with walls
			if (m_mEntityArray[i]->GetUniqueID() == "LeftWall")
			{
				if (m_mEntityArray[j]->GetUniqueID().find("Creeper") != std::string::npos)
				{
					if (colliding == true)
					{
						PEntity temp = m_mEntityArray[j];
						RemoveEntity(temp->GetUniqueID());
					}
				}
			}
			if (m_mEntityArray[i]->GetUniqueID() == "RightWall")
			{
				if (m_mEntityArray[j]->GetUniqueID().find("Cow") != std::string::npos)
				{
					if (colliding == true)
					{
						PEntity temp = m_mEntityArray[j];
						RemoveEntity(temp->GetUniqueID());
					}
				}
			}
			if (m_mEntityArray[i]->GetUniqueID() == "BackWall")
			{
				if (m_mEntityArray[j]->GetUniqueID().find("Cow") != std::string::npos || m_mEntityArray[j]->GetUniqueID().find("Creeper") != std::string::npos)
				{
					if (colliding == true)
					{
						m_mEntityArray[i]->ResolveCollision(m_mEntityArray[j]);
						m_mEntityArray[j]->ApplyForce(vector3(0, 0, 2.0f));
					}
				}
			}
			if (m_mEntityArray[i]->GetUniqueID() == "FrontWall")
			{
				if (m_mEntityArray[j]->GetUniqueID().find("Cow") != std::string::npos || m_mEntityArray[j]->GetUniqueID().find("Creeper") != std::string::npos)
				{
					if (colliding == true)
					{
						m_mEntityArray[i]->ResolveCollision(m_mEntityArray[j]);
						m_mEntityArray[j]->ApplyForce(vector3(0, 0, -2.0f));
					}
				}
			}

			//Check when mobs collide with each other
			if (m_mEntityArray[i]->GetUniqueID().find("Creeper") != std::string::npos)
			{
				if (m_mEntityArray[j]->GetUniqueID().find("Cow") != std::string::npos)
				{
					//Chain reaction
					if (colliding == true)
					{
						std::cout << "BOOM" << std::endl;
						m_mEntityArray[i]->ApplyForce(100.0f * glm::normalize(m_mEntityArray[i]->GetPosition() - m_mEntityArray[j]->GetPosition()));
						m_mEntityArray[i]->ApplyForce(vector3(0, 30.0f, 0));
						m_mEntityArray[j]->ApplyForce(-100.0f * glm::normalize(m_mEntityArray[i]->GetPosition() - m_mEntityArray[j]->GetPosition()));
						m_mEntityArray[j]->ApplyForce(vector3(0, 30.0f, 0));
					}
				}
				else if (m_mEntityArray[j]->GetUniqueID().find("Creeper") != std::string::npos)
				{
					//Chain reaction
					if (colliding == true)
					{
						std::cout << "BOOM" << std::endl;
						m_mEntityArray[i]->ApplyForce(100.0f * glm::normalize(m_mEntityArray[i]->GetPosition() - m_mEntityArray[j]->GetPosition()));
						m_mEntityArray[i]->ApplyForce(vector3(0, 30.0f, 0));
						m_mEntityArray[j]->ApplyForce(-100.0f * glm::normalize(m_mEntityArray[i]->GetPosition() - m_mEntityArray[j]->GetPosition()));
						m_mEntityArray[j]->ApplyForce(vector3(0, 30.0f, 0));
					}
				}
			}
			if (m_mEntityArray[i]->GetUniqueID().find("Cow") != std::string::npos)
			{
				if (m_mEntityArray[j]->GetUniqueID().find("Cow") != std::string::npos)
				{
					//Chain reaction
					if (colliding == true)
					{
						std::cout << "BOOM" << std::endl;
						m_mEntityArray[i]->ApplyForce(100.0f * glm::normalize(m_mEntityArray[i]->GetPosition() - m_mEntityArray[j]->GetPosition()));
						m_mEntityArray[i]->ApplyForce(vector3(0, 30.0f, 0));
						m_mEntityArray[j]->ApplyForce(-100.0f * glm::normalize(m_mEntityArray[i]->GetPosition() - m_mEntityArray[j]->GetPosition()));
						m_mEntityArray[j]->ApplyForce(vector3(0, 30.0f, 0));
					}
				}
			}
		}
		//Update each entity
		m_mEntityArray[i]->Update();
	}
}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	//Create a temporal entity to store the object
	MyEntity* pTemp = new MyEntity(a_sFileName, a_sUniqueID);
	//if I was able to generate it add it to the list
	if (pTemp->IsInitialized())
	{
		//create a new temp array with one extra entry
		PEntity* tempArray = new PEntity[m_uEntityCount + 1];
		//start from 0 to the current count
		uint uCount = 0;
		for (uint i = 0; i < m_uEntityCount; ++i)
		{
			tempArray[uCount] = m_mEntityArray[i];
			++uCount;
		}
		tempArray[uCount] = pTemp;
		//if there was an older array delete
		if (m_mEntityArray)
		{
			delete[] m_mEntityArray;
		}
		//make the member pointer the temp pointer
		m_mEntityArray = tempArray;
		//add one entity to the count
		++m_uEntityCount;
	}
}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return;

	// if out of bounds choose the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	// if the entity is not the very last we swap it for the last one
	if (a_uIndex != m_uEntityCount - 1)
	{
		std::swap(m_mEntityArray[a_uIndex], m_mEntityArray[m_uEntityCount - 1]);
	}
	
	//and then pop the last one
	//create a new temp array with one less entry
	PEntity* tempArray = new PEntity[m_uEntityCount - 1];
	//start from 0 to the current count
	for (uint i = 0; i < m_uEntityCount - 1; ++i)
	{
		tempArray[i] = m_mEntityArray[i];
	}
	//if there was an older array delete
	if (m_mEntityArray)
	{
		delete[] m_mEntityArray;
	}
	//make the member pointer the temp pointer
	m_mEntityArray = tempArray;
	//add one entity to the count
	--m_uEntityCount;
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	int nIndex = GetEntityIndex(a_sUniqueID);
	RemoveEntity((uint)nIndex);
}
Simplex::String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return "";

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->GetUniqueID();
}
Simplex::MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return nullptr;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex];
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	//if out of bounds will do it for all
	if (a_uIndex >= m_uEntityCount)
	{
		//add for each one in the entity list
		for (a_uIndex = 0; a_uIndex < m_uEntityCount; ++a_uIndex)
		{
			m_mEntityArray[a_uIndex]->AddToRenderList(a_bRigidBody);
		}
	}
	else //do it for the specified one
	{
		m_mEntityArray[a_uIndex]->AddToRenderList(a_bRigidBody);
	}
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->AddToRenderList(a_bRigidBody);
	}
}
void Simplex::MyEntityManager::AddDimension(uint a_uIndex, uint a_uDimension)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->AddDimension(a_uDimension);
}
void Simplex::MyEntityManager::AddDimension(String a_sUniqueID, uint a_uDimension)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->AddDimension(a_uDimension);
	}
}
void Simplex::MyEntityManager::RemoveDimension(uint a_uIndex, uint a_uDimension)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->RemoveDimension(a_uDimension);
}
void Simplex::MyEntityManager::RemoveDimension(String a_sUniqueID, uint a_uDimension)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->RemoveDimension(a_uDimension);
	}
}
void Simplex::MyEntityManager::ClearDimensionSetAll(void)
{
	for (uint i = 0; i < m_uEntityCount; ++i)
	{
		ClearDimensionSet(i);
	}
}
void Simplex::MyEntityManager::ClearDimensionSet(uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->ClearDimensionSet();
}
void Simplex::MyEntityManager::ClearDimensionSet(String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->ClearDimensionSet();
	}
}
bool Simplex::MyEntityManager::IsInDimension(uint a_uIndex, uint a_uDimension)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return false;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->IsInDimension(a_uDimension);
}
bool Simplex::MyEntityManager::IsInDimension(String a_sUniqueID, uint a_uDimension)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		return pTemp->IsInDimension(a_uDimension);
	}
	return false;
}
bool Simplex::MyEntityManager::SharesDimension(uint a_uIndex, MyEntity* const a_pOther)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return false;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->SharesDimension(a_pOther);
}
bool Simplex::MyEntityManager::SharesDimension(String a_sUniqueID, MyEntity* const a_pOther)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		return pTemp->SharesDimension(a_pOther);
	}
	return false;
}
void Simplex::MyEntityManager::ApplyForce(vector3 a_v3Force, String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity does not exists return
	if(pTemp)
		pTemp->ApplyForce(a_v3Force);
	return;
}
void Simplex::MyEntityManager::ApplyForce(vector3 a_v3Force, uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->ApplyForce(a_v3Force);
}
void Simplex::MyEntityManager::SetPosition(vector3 a_v3Position, String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity does not exists return
	if (pTemp)
	{
		pTemp->SetPosition(a_v3Position);
	}
	return;
}
void Simplex::MyEntityManager::SetPosition(vector3 a_v3Position, uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	m_mEntityArray[a_uIndex]->SetPosition(a_v3Position);

	return;
}
void Simplex::MyEntityManager::SetMass(float a_fMass, String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity does not exists return
	if (pTemp)
	{
		pTemp->SetMass(a_fMass);
	}
	return;
}
void Simplex::MyEntityManager::SetMass(float a_fMass, uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	m_mEntityArray[a_uIndex]->SetMass(a_fMass);

	return;
}
void Simplex::MyEntityManager::UsePhysicsSolver(bool a_bUse, String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);

	//if the entity does not exists return
	if (pTemp)
		pTemp->UsePhysicsSolver(a_bUse);
	return;
}
void Simplex::MyEntityManager::UsePhysicsSolver(bool a_bUse, uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->UsePhysicsSolver(a_bUse);
}