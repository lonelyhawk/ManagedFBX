#include "stdafx.h"
#include "SceneNode.h"

using namespace ManagedFbx;

SceneNode::SceneNode(FbxNode *node)
{
	m_nativeNode = node;

	m_children = gcnew List<SceneNode^>();
	m_attributes = gcnew List<NodeAttribute^>();

	for(int i = 0; i < m_nativeNode->GetChildCount(); i++)
	{
		auto sub = m_nativeNode->GetChild(i);
		m_children->Add(gcnew SceneNode(sub));
	}

	for(int i = 0; i < m_nativeNode->GetNodeAttributeCount(); i++)
	{
		auto attr = m_nativeNode->GetNodeAttributeByIndex(i);
		m_attributes->Add(gcnew NodeAttribute(attr));
	}
}

IEnumerable<SceneNode^>^ SceneNode::ChildNodes::get()
{
	return m_children->AsReadOnly();
}

IEnumerable<NodeAttribute^>^ SceneNode::Attributes::get()
{
	return m_attributes->AsReadOnly();
}

string ^SceneNode::Name::get()
{
	return gcnew string(m_nativeNode->GetName());
}

void SceneNode::Name::set(string ^value)
{
	m_nativeNode->SetName(StringHelper::ToNative(value));
}

void SceneNode::AddChild(SceneNode ^node)
{
	m_nativeNode->AddChild(node->m_nativeNode);
	m_children->Add(node);
}

void SceneNode::Position::set(Vector3 value)
{
	m_nativeNode->LclTranslation.Set(value);
}

void SceneNode::Rotation::set(Vector3 value)
{
	m_nativeNode->LclRotation.Set(value);
}

void SceneNode::Scale::set(Vector3 value)
{
	m_nativeNode->LclScaling.Set(value);
}

Vector3 SceneNode::Position::get()
{
	return Vector3(m_nativeNode->LclTranslation.Get());
}

Vector3 SceneNode::GeometricTranslate::get(){
	return Vector3(m_nativeNode->PostRotation);
	//return Vector3(m_nativeNode->GetGeometricTranslation(FbxNode::eDestinationPivot));
}

Vector3 SceneNode::GeometricRotation::get(){
	return Vector3(m_nativeNode->GetGeometricRotation(FbxNode::eDestinationPivot));
}

Vector3 SceneNode::GeometricScale::get(){
	return Vector3(m_nativeNode->GetGeometricScaling(FbxNode::eDestinationPivot));
}

Vector3 SceneNode::Rotation::get()
{
	return Vector3(m_nativeNode->LclRotation.Get());
}

Vector3 SceneNode::Scale::get()
{
	return Vector3(m_nativeNode->LclScaling.Get());
}

Mesh ^SceneNode::Mesh::get()
{
	return gcnew ManagedFbx::Mesh(m_nativeNode->GetMesh());
}

Light ^SceneNode::Light::get()
{
	return gcnew ManagedFbx::Light(m_nativeNode->GetLight());
}

array<double>^ SceneNode::Transform::get(){
	//return safe_cast<double>( m_nativeNode->EvaluateGlobalTransform());


	FbxAMatrix matrix = m_nativeNode->EvaluateLocalTransform();
	//matrix.Inverse();
	//double* ptr = (double*)(m_nativeNode->EvaluateGlobalTransform());
	double* ptr = (double*)(matrix);
		array<double>^ list = gcnew array<double>(16);
	for (int i = 0; i < 16; i++)
	{
		//double val = *(ptr + i / 4 + i % 4 * 4);
		double val = *(ptr + i);
		list[i] = val;
	}
	return list;
}

Vector3 SceneNode::MultT(Vector3 vector){

	FbxAMatrix matrixGeo;
	matrixGeo.SetIdentity();
	if (m_nativeNode->GetNodeAttribute())
	{
		const FbxVector4 lT = m_nativeNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		const FbxVector4 lR = m_nativeNode->GetGeometricRotation(FbxNode::eSourcePivot);
		const FbxVector4 lS = m_nativeNode->GetGeometricScaling(FbxNode::eSourcePivot);
		matrixGeo.SetT(lT);
		matrixGeo.SetR(lR);
		matrixGeo.SetS(lS);
	}
	FbxAMatrix matrix = m_nativeNode->EvaluateLocalTransform();

	matrix = matrix*matrixGeo;
	FbxVector4 native_vector(vector.X, vector.Y, vector.Z);
	FbxVector4 result = matrix.MultT(native_vector);
	Vector3^ rt = gcnew Vector3(result[0], result[1], result[2]);
	return *rt;
}

List<Vector3>^ SceneNode::MultT(List<Vector3>^ vectorList){
	List<Vector3>^ results = gcnew List < Vector3 > ;
	FbxAMatrix matrixGeo;
	matrixGeo.SetIdentity();
	if (m_nativeNode->GetNodeAttribute())
	{
		const FbxVector4 lT = m_nativeNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		const FbxVector4 lR = m_nativeNode->GetGeometricRotation(FbxNode::eSourcePivot);
		const FbxVector4 lS = m_nativeNode->GetGeometricScaling(FbxNode::eSourcePivot);
		matrixGeo.SetT(lT);
		matrixGeo.SetR(lR);
		matrixGeo.SetS(lS);
	}
	FbxAMatrix matrix = m_nativeNode->EvaluateLocalTransform();

	matrix = matrix*matrixGeo;

	for each (Vector3 vector in vectorList)
	{
		FbxVector4 native_vector(vector.X, vector.Y, vector.Z);
		FbxVector4 transformVector = matrix.MultT(native_vector);
		Vector3^ rt = gcnew Vector3(transformVector[0], transformVector[1], transformVector[2]);
		results->Add(*rt);
	}
	return results;
}

List<SceneNode^>^ SceneNode::FindAllHierarchyChildren(){
	List<SceneNode^>^ nodeList = gcnew List<SceneNode^>;
	FindChildNodes(this, nodeList);
	return nodeList;
}

void SceneNode::FindChildNodes(SceneNode^ node, List<SceneNode^>^ nodeList)
{
	IEnumerable<SceneNode^>^ childList = node->ChildNodes;
	for each (SceneNode^ child in childList)
	{
		nodeList->Add(child);
		FindChildNodes(child, nodeList);
	}
}