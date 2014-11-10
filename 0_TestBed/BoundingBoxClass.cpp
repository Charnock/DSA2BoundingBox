#include "BoundingBoxClass.h"
//The big 3
BoundingBoxClass::BoundingBoxClass(String a_sInstanceName)
{
	//Initialize variables
	m_pAAMesh = nullptr;
	m_pOMesh = nullptr;
	m_v3AACentroid = vector3(0.0f,0.0f,0.0f);
	m_v3OCentroid = vector3(0.0f,0.0f,0.0f);
	m_v3AAMaximum = vector3(0.0f, 0.0f, 0.0f);
	m_v3AAMinimum = vector3(0.0f, 0.0f, 0.0f);
	m_v3OMaximum = vector3(0.0f, 0.0f, 0.0f);
	m_v3OMinimum = vector3(0.0f, 0.0f, 0.0f);
	m_vOVertices = std::vector<vector3>();
	m_vOAxes = std::vector<vector3>();
	m_v3AAColor = MEWHITE;
	m_v3OColor = MEBLUE;
	m_mModelToWorld = matrix4(1.0f);
	m_bAAVisible = false;
	m_bOVisible = false;

	//Get the singleton instance of the Model Manager
	m_pModelMngr = ModelManagerClass::GetInstance();
	m_sInstance = a_sInstanceName;
	//Identify the instance from the list inside of the Model Manager
	int nInstance = m_pModelMngr->IdentifyInstance(m_sInstance);
	//If there is no instance with that name the Identify Instance will return -1
	//In which case there is nothing to do here so we just return without allocating memory
	if(nInstance == -1)
		return;

	//Construct a Box with the dimensions of the instance, they will be allocated in the
	//corresponding member variables inside the method
	CalculateAABox();
	CalculateOBox(m_sInstance);
	//Get the Model to World matrix associated with the Instance
	m_mModelToWorld = m_pModelMngr->GetModelMatrix(m_sInstance);
	//If the size of the radius is 0 it means that there are no points or all of them are allocated
	//right at the origin, which will cause an issue, so we just return with no allocations
	if(m_fAAWidth == 0.0f)
		return;
	//Crete a new Box and initialize it using the member variables
	m_pAAMesh = new PrimitiveWireClass();
	m_pAAMesh->GenerateCube(1, MEWHITE);
	matrix4 AAModelMatrix = glm::translate(m_mModelToWorld, m_v3AACentroid);
	m_pAAMesh->SetModelMatrix(glm::scale(AAModelMatrix, m_fAAWidth, m_fAAHeight, m_fAADepth));

	m_pOMesh = new PrimitiveWireClass();
	m_pOMesh->GenerateCube(1, MEWHITE);
	matrix4 OModelMatrix = glm::translate(m_mModelToWorld, m_v3OCentroid);
	m_pOMesh->SetModelMatrix(glm::scale(OModelMatrix, m_fOWidth, m_fOHeight, m_fODepth));
}
BoundingBoxClass::BoundingBoxClass(BoundingBoxClass const& other)
{
	//Initialize the Box using other instance of it
	m_sInstance = other.m_sInstance;
	m_bAAVisible = other.m_bAAVisible;
	m_bOVisible = other.m_bOVisible;
	m_fAAWidth = other.m_fAAWidth;
	m_fAAHeight = other.m_fAAHeight;
	m_fAADepth = other.m_fAADepth;
	m_fOWidth = other.m_fOWidth;
	m_fOHeight = other.m_fOHeight;
	m_fODepth = other.m_fODepth;
	m_v3AACentroid = other.m_v3AACentroid;
	m_v3OCentroid = other.m_v3OCentroid;
	m_vOVertices = other.m_vOVertices;
	m_vOAxes = other.m_vOAxes;
	m_mModelToWorld = other.m_mModelToWorld;
	m_pModelMngr = other.m_pModelMngr;

	m_pAAMesh = new PrimitiveWireClass();
	m_pAAMesh->GenerateCube(1, MEWHITE);
	matrix4 AAModelMatrix = glm::translate(m_mModelToWorld, m_v3AACentroid);
	m_pAAMesh->SetModelMatrix(glm::scale(AAModelMatrix, m_fAAWidth, m_fAAHeight, m_fAADepth));

	m_pOMesh = new PrimitiveWireClass();
	m_pOMesh->GenerateCube(1, MEWHITE);
	matrix4 OModelMatrix = glm::translate(m_mModelToWorld, m_v3OCentroid);
	m_pOMesh->SetModelMatrix(glm::scale(OModelMatrix, m_fOWidth, m_fOHeight, m_fODepth));
}
BoundingBoxClass& BoundingBoxClass::operator=(BoundingBoxClass const& other)
{
	//If the incoming instance is the same as the current there is nothing to do here
	if(this != &other)
	{
		//Release the existing object
		Release();
		//Construct the object as in the copy constructor
		m_sInstance = other.m_sInstance;
		m_bAAVisible = other.m_bAAVisible;
		m_bOVisible = other.m_bOVisible;
		m_fAAWidth = other.m_fAAWidth;
		m_fAAHeight = other.m_fAAHeight;
		m_fAADepth = other.m_fAADepth;
		m_fOWidth = other.m_fOWidth;
		m_fOHeight = other.m_fOHeight;
		m_fODepth = other.m_fODepth;
		m_v3AACentroid = other.m_v3AACentroid;
		m_v3OCentroid = other.m_v3OCentroid;
		m_vOVertices = other.m_vOVertices;
		m_vOAxes = other.m_vOAxes;
		m_mModelToWorld = other.m_mModelToWorld;
		m_pModelMngr = other.m_pModelMngr;

	m_pAAMesh = new PrimitiveWireClass();
	m_pAAMesh->GenerateCube(1, MEWHITE);
	matrix4 AAModelMatrix = glm::translate(m_mModelToWorld, m_v3AACentroid);
	m_pAAMesh->SetModelMatrix(glm::scale(AAModelMatrix, m_fAAWidth, m_fAAHeight, m_fAADepth));

	m_pOMesh = new PrimitiveWireClass();
	m_pOMesh->GenerateCube(1, MEWHITE);
	matrix4 OModelMatrix = glm::translate(m_mModelToWorld, m_v3OCentroid);
	m_pOMesh->SetModelMatrix(glm::scale(OModelMatrix, m_fOWidth, m_fOHeight, m_fODepth));
	}
	return *this;
}
BoundingBoxClass::~BoundingBoxClass()
{
	//Destroying the object requires releasing all the allocated memory first
	Release();
}
void BoundingBoxClass::Release(void)
{
	//If the mesh exists release it
	if(m_pAAMesh != nullptr)
	{
		delete m_pAAMesh;
		m_pAAMesh = nullptr;
	
	}
	if(m_pOMesh != nullptr)
	{
		delete m_pOMesh;
		m_pOMesh = nullptr;
	}

	//The job of releasing the Model Manager Instance is going to be the work
	//of another method, we can't assume that this class is the last class
	//that uses this singleton, so we do not release it, we just make the pointer
	//point at nothing.
	m_pModelMngr = nullptr;
}
//Accessors
vector3 BoundingBoxClass::GetAACentroid(void){ return m_v3AACentroid; }
vector3 BoundingBoxClass::GetOCentroid(void){ return m_v3OCentroid; };
vector3 BoundingBoxClass::GetAAColor(void){ return m_v3AAColor; }
vector3 BoundingBoxClass::GetOColor(void){ return m_v3OColor; }
vector3 BoundingBoxClass::GetAAMaximum(void){ return m_v3AAMaximum; }
vector3 BoundingBoxClass::GetAAMinimum(void){ return m_v3AAMinimum; }
vector3 BoundingBoxClass::GetOMaximum(void){ return m_v3OMaximum; }
vector3 BoundingBoxClass::GetOMinimum(void){ return m_v3OMinimum; }
void BoundingBoxClass::SetAAColor(vector3 a_v3Color){ m_v3AAColor = a_v3Color; }
void BoundingBoxClass::SetOColor(vector3 a_v3Color){ m_v3OColor = a_v3Color; }
matrix4 BoundingBoxClass::GetModelMatrix(void){ return m_mModelToWorld; }
void BoundingBoxClass::SetModelMatrix(matrix4 a_mModelMatrix)
{
	//Sets the model matrix of the Box
	m_mModelToWorld = a_mModelMatrix;
	//Sets the Model Matrix of the actual Box shape
	//(which is translated m_v3Centrod away from the origin of our Box)
	CalculateOAxes();
	CalculateAABox();
	matrix4 modelMatrix = glm::translate(matrix4(1.0f), m_v3AACentroid);
	m_pAAMesh->SetModelMatrix(glm::scale(modelMatrix, m_fAAWidth, m_fAAHeight, m_fAADepth));
	modelMatrix = glm::translate(a_mModelMatrix, m_v3OCentroid);
	m_pOMesh->SetModelMatrix(glm::scale(modelMatrix, m_fOWidth, m_fOHeight, m_fODepth));
}

bool BoundingBoxClass::GetAAVisible(void) { return m_bAAVisible; }
bool BoundingBoxClass::GetOVisible(void) { return m_bOVisible; }
std::vector<vector3> BoundingBoxClass::GetOAxes(void){ return m_vOAxes; }
std::vector<vector3> BoundingBoxClass::GetOVertices(void) { return m_vOVertices; }
void BoundingBoxClass::SetAAVisible(bool a_bVisible) { m_bAAVisible = a_bVisible; }
void BoundingBoxClass::SetOVisible(bool a_bVisible) { m_bOVisible = a_bVisible; }
String BoundingBoxClass::GetInstanceName(void){ return m_sInstance; }
//Methods
void BoundingBoxClass::CalculateAABox(void)
{
	//Get the vertices List to calculate the maximum and minimum
	std::vector<vector3> vVertices = m_vOVertices;
	int nVertices = static_cast<int>(vVertices.size());
	
	//If the size of the List is 0 it means we dont have any vertices in this Instance
	//which means there is an error somewhere
	if(nVertices == 0)
		return;

	// Rotate the vertices to recalculate the AA bounding Box
	for(int nVertex = 0; nVertex < nVertices; nVertex++)
	{	
		//vVertices[nVertex] =  static_cast<glm::mat3>(m_mModelToWorld) * vVertices[nVertex];
	}
	//Go one by one on each component and realize which one is the smallest one
	vector3 v3Minimum;
	if(nVertices > 0)
	{
		//We assume the first vertex is the smallest one
		v3Minimum = vVertices[0];
		//And iterate one by one
		for(int nVertex = 1; nVertex < nVertices; nVertex++)
		{
			if(vVertices[nVertex].x < v3Minimum.x)
				v3Minimum.x = vVertices[nVertex].x;

			if(vVertices[nVertex].y < v3Minimum.y)
				v3Minimum.y = vVertices[nVertex].y;

			if(vVertices[nVertex].z < v3Minimum.z)
				v3Minimum.z = vVertices[nVertex].z;
		}
	}
	
	//Go one by one on each component and realize which one is the largest one
	vector3 v3Maximum;
	if(nVertices > 0)
	{
		//We assume the first vertex is the largest one
		v3Maximum = vVertices[0];
		//And iterate one by one
		for(int nVertex = 1; nVertex < nVertices; nVertex++)
		{
			if(vVertices[nVertex].x > v3Maximum.x)
				v3Maximum.x = vVertices[nVertex].x;

			if(vVertices[nVertex].y > v3Maximum.y)
				v3Maximum.y = vVertices[nVertex].y;

			if(vVertices[nVertex].z > v3Maximum.z)
				v3Maximum.z = vVertices[nVertex].z;
		}
	}

	//The centroid is going to be the point that is halfway of the min to the max
	m_v3AAMaximum = v3Maximum; //+ static_cast<vector3>(m_pModelMngr->GetModelMatrix(a_sInstance)[3]);
	m_v3AAMinimum = v3Minimum; // + static_cast<vector3>(m_pModelMngr->GetModelMatrix(a_sInstance)[3]);

	m_v3AACentroid = v3Minimum + v3Maximum;
	m_v3AACentroid /= 2.0f;
	
	m_fAAWidth = v3Maximum.x - v3Minimum.x;
	m_fAAHeight = v3Maximum.y - v3Minimum.y;
	m_fAADepth = v3Maximum.z - v3Minimum.z;
	
	return;
}

void BoundingBoxClass::CalculateOBox(String a_sInstance)
{
	//Get the vertices List to calculate the maximum and minimum
	std::vector<vector3> vVertices = m_pModelMngr->GetVertices(a_sInstance);
	int nVertices = static_cast<int>(vVertices.size());
	
	//If the size of the List is 0 it means we dont have any vertices in this Instance
	//which means there is an error somewhere
	if(nVertices == 0)
		return;

	//Go one by one on each component and realize which one is the smallest one
	vector3 v3Minimum;
	if(nVertices > 0)
	{
		//We assume the first vertex is the smallest one
		v3Minimum = vVertices[0];
		//And iterate one by one
		for(int nVertex = 1; nVertex < nVertices; nVertex++)
		{
			if(vVertices[nVertex].x < v3Minimum.x)
				v3Minimum.x = vVertices[nVertex].x;

			if(vVertices[nVertex].y < v3Minimum.y)
				v3Minimum.y = vVertices[nVertex].y;

			if(vVertices[nVertex].z < v3Minimum.z)
				v3Minimum.z = vVertices[nVertex].z;
		}
	}
	
	//Go one by one on each component and realize which one is the largest one
	vector3 v3Maximum;
	if(nVertices > 0)
	{
		//We assume the first vertex is the largest one
		v3Maximum = vVertices[0];
		//And iterate one by one
		for(int nVertex = 1; nVertex < nVertices; nVertex++)
		{
			if(vVertices[nVertex].x > v3Maximum.x)
				v3Maximum.x = vVertices[nVertex].x;

			if(vVertices[nVertex].y > v3Maximum.y)
				v3Maximum.y = vVertices[nVertex].y;

			if(vVertices[nVertex].z > v3Maximum.z)
				v3Maximum.z = vVertices[nVertex].z;
		}
	}

	//The centroid is going to be the point that is halfway of the min to the max
	m_v3OMaximum = v3Maximum;
	m_v3OMinimum = v3Minimum;

	m_v3OCentroid = v3Minimum + v3Maximum;
	m_v3OCentroid /= 2.0f;

	m_fOWidth = v3Maximum.x - v3Minimum.x;
	m_fOHeight = v3Maximum.y - v3Minimum.y;
	m_fODepth = v3Maximum.z - v3Minimum.z;

	return;
}

void BoundingBoxClass::CalculateOVertices(void)
{
	m_vOVertices.clear();
	m_vOVertices.push_back(m_v3OMaximum);
	m_vOVertices.push_back(vector3(m_v3OMaximum.x, m_v3OMaximum.y, m_v3OMinimum.z));
	m_vOVertices.push_back(vector3(m_v3OMinimum.x, m_v3OMaximum.y, m_v3OMinimum.z));
	m_vOVertices.push_back(vector3(m_v3OMinimum.x, m_v3OMaximum.y, m_v3OMaximum.z));
	m_vOVertices.push_back(vector3(m_v3OMaximum.x, m_v3OMinimum.y, m_v3OMaximum.z));
	m_vOVertices.push_back(vector3(m_v3OMaximum.x, m_v3OMinimum.y, m_v3OMinimum.z));
	m_vOVertices.push_back(m_v3OMinimum);
	m_vOVertices.push_back(vector3(m_v3OMinimum.x, m_v3OMinimum.y, m_v3OMaximum.z));

	for(int nVertex = 0;nVertex < m_vOVertices.size(); nVertex++)
	{
		m_vOVertices[nVertex] = static_cast<glm::mat3>(m_mModelToWorld) * m_vOVertices[nVertex];
		m_vOVertices[nVertex] = static_cast<vector3>(m_pModelMngr->GetModelMatrix(m_sInstance)[3]) + m_vOVertices[nVertex];
	}
}

void BoundingBoxClass::CalculateOAxes(void)
{
	CalculateOVertices();

	m_vOAxes.clear();
	m_vOAxes.push_back(glm::normalize(glm::cross(m_vOVertices[0] - m_vOVertices[1], m_vOVertices[1] - m_vOVertices[2])));
	m_vOAxes.push_back(glm::normalize(glm::cross(m_vOVertices[4] - m_vOVertices[0], m_vOVertices[0] - m_vOVertices[1])));
	m_vOAxes.push_back(glm::normalize(glm::cross(m_vOVertices[5] - m_vOVertices[1], m_vOVertices[1] - m_vOVertices[2])));
	m_vOAxes.push_back(glm::normalize(glm::cross(m_vOVertices[6] - m_vOVertices[2], m_vOVertices[2] - m_vOVertices[3])));
	m_vOAxes.push_back(glm::normalize(glm::cross(m_vOVertices[7] - m_vOVertices[3], m_vOVertices[3] - m_vOVertices[0])));
	m_vOAxes.push_back(glm::normalize(glm::cross(m_vOVertices[6] - m_vOVertices[5], m_vOVertices[5] - m_vOVertices[4])));	
}

void BoundingBoxClass::RenderAA( vector3 a_vColor )
{
	//If the shape is visible render it
	//otherwise just return
	if(!m_bAAVisible)
		return;
	//Calculate the color we want the shape to be
	vector3 vColor;
	//if the argument was MEDEFAULT just use the color variable in our class
	if(a_vColor == MEDEFAULT)
		vColor = m_v3AAColor;
	else //Otherwise use the argument
		vColor = a_vColor;

	//render the shape using a special case of the Shape Render method which uses the Color Shader.
	m_pAAMesh->Render( matrix4(1.0f), vColor );
	
}

void BoundingBoxClass::RenderO( vector3 a_vColor )
{
	//If the shape is visible render it
	//otherwise just return
	if(!m_bOVisible)
		return;
	//Calculate the color we want the shape to be
	vector3 vColor;
	//if the argument was MEDEFAULT just use the color variable in our class
	if(a_vColor == MEDEFAULT)
		vColor = m_v3OColor;
	else //Otherwise use the argument
		vColor = a_vColor;

	//render the shape using a special case of the Shape Render method which uses the Color Shader.
	m_pOMesh->Render( matrix4(1.0f), vColor );
	
}

