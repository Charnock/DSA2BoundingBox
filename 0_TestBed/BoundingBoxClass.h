/*
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2014/10
*/
#ifndef __BOUNDINGBOXCLASS_H_
#define __BOUNDINGBOXCLASS_H_

#include "ME\Mesh\shape\PrimitiveWireClass.h"
#include "ME\Mesh\shape\ModelManagerClass.h"
using namespace MyEngine;
//System Class
class BoundingBoxClass 
{
	String m_sInstance;//The name of the instance related to this Box
	bool m_bAAVisible;//Flag for rendering the AA Box
	bool m_bOVisible;//Flag for rendering the O Box
	float m_fAAWidth;//Width of the AA Box
	float m_fAAHeight;//Height of the AA Box
	float m_fAADepth;//Depth of the AA box
	float m_fOWidth;//Width of the O Box
	float m_fOHeight;//Height of the O Box
	float m_fODepth;//Depth of the O box
	vector3 m_v3AAMaximum;
	vector3 m_v3AAMinimum;
	vector3 m_v3OMaximum;
	vector3 m_v3OMinimum;
	vector3 m_v3AACentroid;//Centroid of the AA Box
	vector3 m_v3OCentroid;//Centroid of the O Box
	vector3 m_v3AAColor;//Color of the AA Box
	vector3 m_v3OColor;//Color of the O Box
	std::vector<vector3> m_vOVertices;
	std::vector<vector3> m_vOAxes;
	matrix4 m_mModelToWorld;//Model matrix 
	PrimitiveWireClass* m_pAAMesh;//AA Box Mesh
	PrimitiveWireClass* m_pOMesh;//O Box Mesh
	ModelManagerClass* m_pModelMngr;//ModelManager instance

public:
	/* Constructor 	*/
	BoundingBoxClass(String a_sInstanceName);
	/* Copy Constructor */
	BoundingBoxClass(BoundingBoxClass const& other);
	/* Copy Assignment Operator	*/
	BoundingBoxClass& operator=(BoundingBoxClass const& other);
	/*	Destructor	*/
	~BoundingBoxClass(void);

	/* Gets the visibility of the Box (whether or not it is going to be drawn in render) */
	bool GetAAVisible(void);
	bool GetOVisible(void);
	/*Sets the visibility of the Box (whether or not it is going to be drawn in render)
	Args:
		bool a_bVisible -> true for draw false for not draw*/
	void SetAAVisible(bool a_bVisible);
	void SetOVisible(bool a_bVisible);
	/* Property:
	GetVisible() or GetVisible()*/
	__declspec(property(get = GetAAVisible, put = SetAAVisible)) bool AAVisible;
	__declspec(property(get = GetOVisible, put = SetOVisible)) bool OVisible;



	/* Gets the name of the model associated with this bounding Box from model manager */
	String GetInstanceName(void);
	/* Property:
	GetInstanceName()*/
	__declspec(property(get = GetInstanceName)) String InstanceName;

	/* Gets the centroid the bounding Box */
	vector3 GetAACentroid(void);
	vector3 GetOCentroid(void);
	/* Property:
	GetCentroid()*/
	__declspec(property(get = GetAACentroid)) vector3 AACentroid;
	__declspec(property(get = GetOCentroid)) vector3 OCentroid;

	/* Get the radius of the bounding Box */
	float GetRadius(void);
	/* Property:
	GetRadius()	*/
	__declspec(property(get = GetRadius)) float Radius;

	vector3 GetAAMaximum(void);
	vector3 GetAAMinimum(void);
	vector3 GetOMaximum(void);
	vector3 GetOMinimum(void);
	std::vector<vector3> GetOAxes(void);
	std::vector<vector3> GetOVertices(void);

	__declspec(property(get = GetAAMaximum)) vector3 AAMaximum;
	__declspec(property(get = GetAAMinimum)) vector3 AAMinimum;
	__declspec(property(get = GetOMaximum)) vector3 OMaximum;
	__declspec(property(get = GetOMinimum)) vector3 OMinimum;
	__declspec(property(get = GetOAxes)) std::vector<vector3> OAxes;
	__declspec(property(get = GetOVertices)) std::vector<vector3> OVertices;
	

	/* Gets the "ModelToWorld" matrix associated with the bounding Box */
	matrix4 GetModelMatrix(void);
	/* Sets the "ModelToWorld" matrix associated with the bounding Box */
	void SetModelMatrix(matrix4 a_ModelMatrix);
	/* "ModelToWorld"
	GetModelMatrix() or SetModelMatrix() */
	__declspec(property(get = GetModelMatrix, put = SetModelMatrix)) matrix4 ModelMatrix;

	/* Gets the color vector of this bounding Box (the default color in which is going to be rendered) */
	vector3 GetAAColor(void);
	vector3 GetOColor(void);
	/* Sets the color vector of this bounding Box (the default color in which is going to be rendered) */
	void SetAAColor(vector3 a_v3Color);
	void SetOColor(vector3 a_v3Color);
	/* Property:
	GetColor() or SetColor() */
	__declspec(property(get = GetAAColor, put = SetAAColor)) vector3 AAColor;
	__declspec(property(get = GetOColor, put = SetOColor)) vector3 OColor;

	/* Renders the bounding Box
		Args:
			a_vColor -> determinate the color of the Box to be rendered, if MEDEFAULT
			it will render the shape in the constructed color (white) */
	void RenderAA( vector3 a_vColor = MEDEFAULT);
	void RenderO( vector3 a_vColor = MEDEFAULT);

private:
	/* Released Memory and objects allocated. */
	void Release(void);
	/* Calculates the Box from the instance
	Args:
		a_sInstance: The name of the instance for which the bounding Box is going to be calculated */
	void CalculateAABox(String a_sInstance);
	void CalculateOBox(String a_sInstance);

	// SAT helper methods
	void CalculateOVertices(void);
	void CalculateOAxes(void);
};


#endif //__BoundingBoxClass_H__
