#include "BoundingBoxManager.h"
//  BoundingBoxManager
BoundingBoxManager* BoundingBoxManager::m_pInstance = nullptr;

BoundingBoxManager* BoundingBoxManager::GetInstance()
{
	if(m_pInstance == nullptr)
	{
		m_pInstance = new BoundingBoxManager();
	}
	return m_pInstance;
}
void BoundingBoxManager::ReleaseInstance()
{
	if(m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
void BoundingBoxManager::Init(void)
{
	m_pModelMngr = ModelManagerClass::GetInstance();
	m_vAACollidingNames.clear();
	m_nBoxes = 0;
}
void BoundingBoxManager::Release(void)
{
	RemoveBox("ALL");
	return;
}
//The big 3
BoundingBoxManager::BoundingBoxManager(){Init();}
BoundingBoxManager::BoundingBoxManager(BoundingBoxManager const& other){ }
BoundingBoxManager& BoundingBoxManager::operator=(BoundingBoxManager const& other) { return *this; }
BoundingBoxManager::~BoundingBoxManager(){Release();};
//Accessors
int BoundingBoxManager::GetNumberOfBoxes(void){ return m_nBoxes; }
//--- Non Standard Singleton Methods
void BoundingBoxManager::SetAAVisible(bool a_bVisible, String a_sInstance)
{
	if(a_sInstance == "ALL")
	{
		int nBoxes = GetNumberOfBoxes();
		for(int nBox = 0; nBox < nBoxes; nBox++)
		{
			m_vBoundingBox[nBox]->SetAAVisible(a_bVisible);
		}
	}
	else
	{
		int nBox = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nBox < 0 || nBox < m_nBoxes)
			m_vBoundingBox[nBox]->SetAAVisible(a_bVisible);
	}
}

void BoundingBoxManager::SetOVisible(bool a_bVisible, String a_sInstance)
{
	if(a_sInstance == "ALL")
	{
		int nBoxes = GetNumberOfBoxes();
		for(int nBox = 0; nBox < nBoxes; nBox++)
		{
			m_vBoundingBox[nBox]->SetOVisible(a_bVisible);
		}
	}
	else
	{
		int nBox = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nBox < 0 || nBox < m_nBoxes)
			m_vBoundingBox[nBox]->SetOVisible(a_bVisible);
	}
}
void BoundingBoxManager::SetAAColor(vector3 a_v3Color, String a_sInstance)
{
	if(a_sInstance == "ALL")
	{
		int nBoxes = GetNumberOfBoxes();
		for(int nBox = 0; nBox < nBoxes; nBox++)
		{
			m_vBoundingBox[nBox]->SetAAColor(a_v3Color);
		}
	}
	else
	{
		int nBox = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nBox < 0 || nBox < m_nBoxes)
			m_vBoundingBox[nBox]->SetAAColor(a_v3Color);
	}
}

void BoundingBoxManager::SetOColor(vector3 a_v3Color, String a_sInstance)
{
	if(a_sInstance == "ALL")
	{
		int nBoxes = GetNumberOfBoxes();
		for(int nBox = 0; nBox < nBoxes; nBox++)
		{
			m_vBoundingBox[nBox]->SetOColor(a_v3Color);
		}
	}
	else
	{
		int nBox = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nBox < 0 || nBox < m_nBoxes)
			m_vBoundingBox[nBox]->SetOColor(a_v3Color);
	}
}
void BoundingBoxManager::SetModelMatrix(matrix4 a_mModelMatrix, String a_sInstance)
{
	if(a_sInstance == "ALL")
	{
		int nBoxes = GetNumberOfBoxes();
		for(int nBox = 0; nBox < nBoxes; nBox++)
		{
			m_vBoundingBox[nBox]->SetModelMatrix(a_mModelMatrix);
		}
	}
	else
	{
		int nBox = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nBox < 0 || nBox < m_nBoxes)
			m_vBoundingBox[nBox]->SetModelMatrix(a_mModelMatrix);
	}
}

void BoundingBoxManager::RenderAA(String a_sInstance)
{
	
	if(a_sInstance == "ALL")
	{
		int nBoxes = GetNumberOfBoxes();
		for(int nBox = 0; nBox < nBoxes; nBox++)
		{
			m_vBoundingBox[nBox]->RenderAA(MEDEFAULT);
		}
	}
	else
	{
		int nBox = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nBox < 0 || nBox < m_nBoxes)
			m_vBoundingBox[nBox]->RenderAA(MEDEFAULT);
	}
}

void BoundingBoxManager::RenderO(String a_sInstance)
{
	
	if(a_sInstance == "ALL")
	{
		int nBoxes = GetNumberOfBoxes();
		for(int nBox = 0; nBox < nBoxes; nBox++)
		{
			m_vBoundingBox[nBox]->RenderO(MEDEFAULT);
		}
	}
	else
	{
		int nBox = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nBox < 0 || nBox < m_nBoxes)
			m_vBoundingBox[nBox]->RenderO(MEDEFAULT);
	}
}
void BoundingBoxManager::AddBox(String a_sInstanceName)
{
	BoundingBoxClass* oBox = new BoundingBoxClass(a_sInstanceName);
	m_vBoundingBox.push_back(oBox);
	oBox->SetModelMatrix(m_pModelMngr->GetModelMatrix(a_sInstanceName));
	m_nBoxes ++;
}
void BoundingBoxManager::RemoveBox(String a_sInstanceName)
{
	if(a_sInstanceName == "ALL")
	{
		//Clear the vector's element first otherwise there is a memory leak
		for(int nBox = 0; nBox < m_nBoxes; nBox++)
		{
			BoundingBoxClass* pBS = m_vBoundingBox[nBox];
			delete pBS;
		}
		m_vBoundingBox.clear();
		m_nBoxes = 0;
		return;
	}
	std::vector<BoundingBoxClass*> vTemp;
	int nBox = m_pModelMngr->IdentifyInstance(a_sInstanceName);
	if(nBox < 0 || nBox < m_nBoxes)
	{
		for(int nBox = 0; nBox < m_nBoxes; nBox++)
		{
			if(nBox != nBox)
			{
				vTemp.push_back(m_vBoundingBox[nBox]);
			}
			else
			{
				BoundingBoxClass* pBS = m_vBoundingBox[nBox];
				delete pBS;
			}
		}
	}
	m_vBoundingBox = vTemp;
	m_nBoxes--;
}
void BoundingBoxManager::Update(void)
{
	m_vAACollidingNames.clear();
	m_vOCollidingNames.clear();
	for(int nBox = 0; nBox < m_nBoxes; nBox++)
	{
		m_vBoundingBox[nBox]->SetAAColor(MEYELLOW);
		m_vBoundingBox[nBox]->SetOColor(MEBLUE);
	}
	CollisionCheck();
	SATCollisionCheck();
	CollisionResponse();
}
void BoundingBoxManager::CollisionCheck(void)
{
	for(int nBox2 = 0; nBox2 < m_nBoxes; nBox2++)
	{
		for(int nBox1 = 0; nBox1 < m_nBoxes; nBox1++)
		{
			if(nBox1 != nBox2)
			{
				vector3 maximum1 = m_vBoundingBox[nBox1]->GetAAMaximum();
				vector3 minimum1 = m_vBoundingBox[nBox1]->GetAAMinimum();
				vector3 maximum2 = m_vBoundingBox[nBox2]->GetAAMaximum();
				vector3 minimum2 = m_vBoundingBox[nBox2]->GetAAMinimum();

				if(!(maximum1.x < minimum2.x || minimum1.x > maximum2.x) &&
					!(maximum1.y < minimum2.y || minimum1.y > maximum2.y) &&
					!(maximum1.z < minimum2.z || minimum1.z > maximum2.z))
				{
					m_vAACollidingNames.push_back(m_vBoundingBox[nBox1]->GetInstanceName());
					m_vAACollidingNames.push_back(m_vBoundingBox[nBox2]->GetInstanceName());
				}
			}
		}
	}
}
bool BoundingBoxManager::CheckForNameInAAList(String a_sName)
{
	int nNames = static_cast<int>(m_vAACollidingNames.size());
	for(int nName = 0; nName < nNames; nName++)
	{
		if(m_vAACollidingNames[nName] == a_sName)
			return true;
	}
	return false;
}

bool BoundingBoxManager::CheckForNameInOList(String a_sName)
{
	int nNames = static_cast<int>(m_vOCollidingNames.size());
	for(int nName = 0; nName < nNames; nName++)
	{
		if(m_vOCollidingNames[nName] == a_sName)
			return true;
	}
	return false;
}

void BoundingBoxManager::CollisionResponse(void)
{
	for(int nBox = 0; nBox < m_nBoxes; nBox++)
	{
		if(CheckForNameInAAList(m_vBoundingBox[nBox]->GetInstanceName()))
		{
			m_vBoundingBox[nBox]->SetAAColor(MEMAGENTA);
		}
		if(CheckForNameInOList(m_vBoundingBox[nBox]->GetInstanceName()))
		{
			m_vBoundingBox[nBox]->SetOColor(MERED);
		}
	}
}

void BoundingBoxManager::SATCollisionCheck(void)
{
	for(int nBox2 = 0; nBox2 < m_nBoxes; nBox2++)
	{
		for(int nBox1 = 0; nBox1 < m_nBoxes; nBox1++)
		{
			if(nBox1 != nBox2)
			{
				if(SATCalculations(nBox1, nBox2))
				{
					m_vOCollidingNames.push_back(m_vBoundingBox[nBox1]->GetInstanceName());
					m_vOCollidingNames.push_back(m_vBoundingBox[nBox2]->GetInstanceName());
				}
			}			
		}		
	}
}

bool BoundingBoxManager::SATCalculations(int nBox1, int nBox2)
{
	std::vector<vector3> vAxes1 = m_vBoundingBox[nBox1]->GetOAxes();
	std::vector<vector3> vAxes2 = m_vBoundingBox[nBox2]->GetOAxes();

	std::vector<vector3> vVertices1 = m_vBoundingBox[nBox1]->GetOVertices();
	std::vector<vector3> vVertices2 = m_vBoundingBox[nBox2]->GetOVertices();


	for(int nAxis = 0; nAxis < vAxes1.size();nAxis++)
	{
		vector2 v2Proj1 = vector2(INT_MAX, -INT_MAX);
		vector2 v2Proj2 = vector2(INT_MAX, -INT_MAX);

		vector3 v3Axis = vAxes1[nAxis];
		for(int nVertex = 0; nVertex < vVertices1.size();nVertex++)
		{
			float fDot = glm::dot(v3Axis, vVertices1[nVertex]);

			if(fDot < v2Proj1.x)
				v2Proj1.x = fDot;
			

			if(fDot > v2Proj1.y)
				v2Proj1.y = fDot;
		}

		for(int nVertex = 0; nVertex < vVertices2.size();nVertex++)
		{
			float fDot = glm::dot(v3Axis, vVertices2[nVertex]);

			if(fDot < v2Proj2.x)
				v2Proj2.x = fDot;

			if(fDot > v2Proj2.y)
				v2Proj2.y = fDot;
		}

		if(v2Proj1.x > v2Proj2.y)
			return false;

	}

	for(int nAxis = 0; nAxis < vAxes2.size();nAxis++)
	{

		vector2 v2Proj1 = vector2(INT_MAX, -INT_MAX);
		vector2 v2Proj2 = vector2(INT_MAX, -INT_MAX);

		vector3 v3Axis = vAxes2[nAxis];

		for(int nVertex = 0; nVertex < vVertices1.size();nVertex++)
		{
			float fDot = glm::dot(v3Axis,vVertices1[nVertex]);
			if(fDot < v2Proj1.x)
				v2Proj1.x = fDot;

			if(fDot > v2Proj1.y)
				v2Proj1.y = fDot;
		}
		for(int nVertex = 0; nVertex < vVertices2.size();nVertex++)
		{
			float fDot = glm::dot(v3Axis, vVertices2[nVertex]);
			if(fDot < v2Proj2.x)
				v2Proj2.x = fDot;

			if(fDot > v2Proj2.y)
				v2Proj2.y = fDot;
		}

		if(v2Proj1.x > v2Proj2.y)
			return false;
	}

	return true;
}
