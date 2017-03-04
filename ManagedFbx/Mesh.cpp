#include "stdafx.h"
#include "Mesh.h"
#include "Manager.h"

using namespace ManagedFbx;

Mesh::Mesh(FbxMesh *nativeMesh)
{
	m_nativeMesh = nativeMesh;
}

Mesh ^Mesh::Triangulate()
{
	/*auto mesh = gcnew Mesh(Manager::GetGeomConverter()->TriangulateMesh(m_nativeMesh));
	mesh->UVLayer = UVLayer;
	return mesh;*/
	FbxNodeAttribute *attribute = Manager::GetGeomConverter()->Triangulate(m_nativeMesh, false);
	FbxMesh * mesh =(FbxMesh*) attribute;//FbxMesh::Create(attribute, m_nativeMesh->GetName());
	return gcnew Mesh(mesh);
}

bool Mesh::Triangulated::get()
{
	return m_nativeMesh->IsTriangleMesh();
}

array<ManagedFbx::Polygon> ^Mesh::Polygons::get()
{
	int count = m_nativeMesh->GetPolygonCount();
	auto list = gcnew array<Polygon>(count);

	for(int i = 0; i < count; i++)
	{
		auto poly = Polygon();

		int indexCount = m_nativeMesh->GetPolygonSize(i);
		poly.Indices = gcnew array<int>(indexCount);

		for(int j = 0; j < indexCount; j++)
			poly.Indices[j] = m_nativeMesh->GetPolygonVertex(i, j);

		list[i] = poly;
	}

	return list;
}

array<Vector3> ^Mesh::Vertices::get()
{
	int count = m_nativeMesh->GetControlPointsCount();
	auto list = gcnew array<Vector3>(count);

	for(int i = 0; i < count; i++)
	{
		auto point =  m_nativeMesh->GetControlPointAt(i);
		list[i] = Vector3(point);
	}

	return list;
}

void Mesh::Vertices::set(array<Vector3>^ controlPoints)
{
	int count = controlPoints->Length;
	m_nativeMesh->InitControlPoints(count);
	for (int i = 0; i < count; i++)
	{
		double x = controlPoints[i].X;
		double y = controlPoints[i].Y;
		double z = controlPoints[i].Z;
		FbxVector4 lNativeVector(x, y, z);
		m_nativeMesh->mControlPoints[i] = lNativeVector;
	}
}

bool Mesh::Valid::get()
{
	return m_nativeMesh != nullptr;
}

array<Vector3> ^Mesh::Normals::get()
{
	auto normals = m_nativeMesh->GetLayer(0)->GetNormals();
	int count = normals->GetDirectArray().GetCount();
	auto list = gcnew array<Vector3>(count);

	for(int i = 0; i < count; i++)
		list[i] = Vector3(normals->GetDirectArray().GetAt(i));

	return list;
}

void Mesh::Normals::set(array<Vector3>^ normals)
{
	FbxGeometryElementNormal* lGeometryElementNormal = m_nativeMesh->CreateElementNormal();
	lGeometryElementNormal->SetReferenceMode(FbxGeometryElement::eDirect);
	for each (Vector3 var in normals)
	{
		FbxVector4 normal(var.X, var.Y, var.Z);
		lGeometryElementNormal->GetDirectArray().Add(normal);
	}
}

array<Vector2> ^Mesh::TextureCoords::get()
{
	auto layer = m_nativeMesh->GetLayer(UVLayer);

	if(!layer)
		return gcnew array<Vector2>(0);

	auto coords = layer->GetUVs();
	int count = coords == nullptr ? 0 : coords->GetDirectArray().GetCount();
	auto list = gcnew array<Vector2>(count);

	for(int i = 0; i < count; i++)
		list[i] = Vector2(coords->GetDirectArray().GetAt(i));

	return list;
}

void Mesh::TextureCoords::set(array<Vector2>^ uvArray)
{
	int layer_count = m_nativeMesh->GetLayerCount();
	for (int i = 0; i < layer_count; i++)
	{
		auto t_layer = m_nativeMesh->GetLayer(i);
		if (t_layer)
		{
			auto element = t_layer->GetUVs();
			if (element)
			{
				element->Clear();
				FbxVector2 vector(uvArray[i].X, uvArray[i].Y);
				element->mDirectArray->Add(vector);
				return;
			}
		}
	}

	FbxLayerElementUV *lUVElement = FbxLayerElementUV::Create(m_nativeMesh, "");
	for (int i = 0; i < uvArray->Length; i++)
	{
		FbxVector2 vector(uvArray[i].X, uvArray[i].Y);
		lUVElement->mDirectArray->Add(vector);
	}

	m_nativeMesh->GetLayer(0)->SetUVs(lUVElement);
	
}

int Mesh::GetMaterialId(int polygon)
{
	FbxLayerElementArrayTemplate<int> *materials = nullptr;
	m_nativeMesh->GetMaterialIndices(&materials);
	return materials->GetAt(polygon);
}

array<int> ^Mesh::MaterialIDs::get()
{
	auto materials = m_nativeMesh->GetLayer(0)->GetMaterials();
	int count = materials == nullptr ? 0 : materials->GetIndexArray().GetCount();
	auto list = gcnew array<int>(count);

	for(int i = 0; i < count; i++)
		list[i] = materials->GetIndexArray().GetAt(i);

	return list;
}

int Mesh::GetUVIndex(int polygon, int index)
{
	return m_nativeMesh->GetTextureUVIndex(polygon, index);
}

Vector3 Mesh::GetVertexNormal(int polygon, int index)
{
	FbxVector4 normal;
	m_nativeMesh->GetPolygonVertexNormal(polygon, index, normal);
	return Vector3(normal);
}

array<Colour> ^Mesh::VertexColours::get()
{
	auto colours = m_nativeMesh->GetLayer(0)->GetVertexColors();
	int count = colours == nullptr ? 0 : colours->GetDirectArray().GetCount();
	auto list = gcnew array<Colour>(count);

	for(int i = 0; i < count; i++)
		list[i] = Colour(colours->GetDirectArray().GetAt(i));

	return list;
}

int Mesh::NormalMappingMode::get(){
	FbxGeometryElementNormal* leNormals = m_nativeMesh->GetElementNormal(0);
	return leNormals->GetMappingMode();
}

void Mesh::NormalMappingMode::set(int mode)
{
	FbxGeometryElementNormal* leNormal = m_nativeMesh->GetElementNormal(0);
	leNormal->SetMappingMode((FbxLayerElement::EMappingMode) mode);
}

int Mesh::UVMappingMode::get()
{
	auto layer = m_nativeMesh->GetLayer(UVLayer);
	if (!layer)
		return -1;
	auto element = layer->GetUVs();

	return (int)element->GetMappingMode();
}

void Mesh::UVMappingMode::set(int mode)
{
	auto layer = m_nativeMesh->GetLayer(UVLayer);
	if (!layer)
		return;
	auto element = layer->GetUVs();

	element->SetMappingMode((FbxLayerElement::EMappingMode) mode);
}

void ManagedFbx::Mesh::Test(){
	FbxVector4* controlpoints = m_nativeMesh->GetControlPoints();
	while (controlpoints!=nullptr)
	{

	}
}

FbxMesh* Mesh::NativePtr::get(){
	return m_nativeMesh;
}

void Mesh::AddPolygons(List<int>^ polygonIndex)
{
	for (int i = 0; i < polygonIndex->Count; i += 3)
	{
		m_nativeMesh->BeginPolygon();
		{
			m_nativeMesh->AddPolygon(polygonIndex[i]);
			m_nativeMesh->AddPolygon(polygonIndex[i + 1]);
			m_nativeMesh->AddPolygon(polygonIndex[i + 2]);
		}
		m_nativeMesh->EndPolygon();
	}
}

void Mesh::AddPolygon(List<int>^ polygonIndex, int materialId)
{
	m_nativeMesh->BeginPolygon(materialId);
	{
		for (int i = 0; i < polygonIndex->Count; i++)
		{
			int index = polygonIndex[i];
			m_nativeMesh->AddPolygon(index);
		}
	}
	m_nativeMesh->EndPolygon();
}

void Mesh::AddPolygons(List<int>^ pPolygonIndex, int pMaterialId)
{
	for (int i = 0; i < pPolygonIndex->Count; i += 3)
	{
		m_nativeMesh->BeginPolygon(pMaterialId);
		{
			m_nativeMesh->AddPolygon(pPolygonIndex[i]);
			m_nativeMesh->AddPolygon(pPolygonIndex[i + 1]);
			m_nativeMesh->AddPolygon(pPolygonIndex[i + 2]);
		}
		m_nativeMesh->EndPolygon();
	}
}