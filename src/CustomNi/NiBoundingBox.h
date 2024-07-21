#pragma once
#include "../Logger/Logger.h"
#include <NiTriShape.h>
#include <NiTriStrips.h>
#include <NiWireframeProperty.h>
class NiBoundingBox 
{
private:

	static void UpdateVertices(NiNodePtr& BoundingBox, NiPoint3 MinPoint, NiPoint3 MaxPoint)
	{
		auto StripPtr = BoundingBox->GetAt(0);
		if (!NiIsKindOf(NiTriStrips, StripPtr))
		{
			LogError("Bounding Box Nif is broken!");
			return;
		}
		NiTriStrips* strips = (NiTriStrips*)StripPtr;
		strips->SetScale(1.f);
		if (strips->GetVertexCount() != 20)
		{
			LogError("Bounding Box Vertex Count Error!");
		}
		NiPoint3* Array = strips->GetVertices();
		NiPoint3 BFL = MinPoint;
		NiPoint3 BFR(MaxPoint.x, MinPoint.y, MinPoint.z);
		NiPoint3 BBR(MaxPoint.x, MaxPoint.y, MinPoint.z);
		NiPoint3 BBL(MinPoint.x, MaxPoint.y, MinPoint.z);

		NiPoint3 TBR = MaxPoint;
		NiPoint3 TBL(MinPoint.x, MaxPoint.y, MaxPoint.z);
		NiPoint3 TFL(MinPoint.x, MinPoint.y, MaxPoint.z);
		NiPoint3 TFR(MaxPoint.x, MinPoint.y, MaxPoint.z);
		Array[0] = BBL;
		Array[1] = BBR;
		Array[2] = BFL;
		Array[3] = BFR;
		Array[4] = BBR;
		Array[5] = TBR;
		Array[6] = TFR;
		Array[7] = TFR;
		Array[8] = TFL;
		Array[9] = TBL;
		Array[10] = TFL;
		Array[11] = BBL;
		Array[12] = BBL;
		Array[13] = TBL;
		Array[14] = BBR;
		Array[15] = TBR;
		Array[16] = BFR;
		Array[17] = TFR;
		Array[18] = BFL;
		Array[19] = TFL;
		//strips->GetModelData()->Replace(strips->GetVertexCount(), strips->GetVertices(), strips->GetNormals(), strips->GetColors(), strips->GetTextures(), strips->GetTextureSets(), strips->GetNormalBinormalTangentMethod());
		strips->GetModelData()->SetConsistency(NiGeometryData::MUTABLE);
		strips->GetModelData()->MarkAsChanged(NiGeometryData::VERTEX_MASK);
		strips->Update(0.0f);
	}
public:
	static void GetNewVerticesData(NiNode* Parent, NiPoint3& MinPoint, NiPoint3& MaxPoint,NiMatrix3& WorldRotate, NiPoint3 ParentTranslate = NiPoint3(0.f,0.f,0.f))
	{
		for (int i = 0; i < Parent->GetChildCount(); i++)
		{
			auto child = Parent->GetAt(i);
			if (child)
			{
				bool TriShape = NiIsKindOf(NiTriShape, child);
				bool TriStrips = NiIsKindOf(NiTriStrips, child);
				bool TriShapeData = NiIsKindOf(NiTriShapeData, child);
				bool TriStripsData = NiIsKindOf(NiTriStripsData, child);
				if (TriShape || TriShapeData || TriStrips || TriStripsData)
				{
					NiPoint3* VerticesArray = NULL;
					int VertexCount = 0;
					float scale = 1.f;
					NiPoint3 Translate = ParentTranslate;
					if (TriShape) {
						NiTriShape* shape = (NiTriShape*)child;
						VerticesArray = shape->GetVertices();
						WorldRotate = shape->GetRotate();
						VertexCount = shape->GetVertexCount();
						scale = shape->GetScale();
						Translate += shape->GetTranslate();
					}
					if (TriShapeData) {
						NiTriShapeData* shape = (NiTriShapeData*)child;
						VerticesArray = shape->GetVertices();
						VertexCount = shape->GetVertexCount();
					}
					if (TriStrips) {
						NiTriStrips* shape = (NiTriStrips*)child;
						VerticesArray = shape->GetVertices();
						WorldRotate = shape->GetRotate();
						VertexCount = shape->GetVertexCount();
						scale = shape->GetScale();
						Translate += shape->GetTranslate();

					}
					if (TriStripsData) {
						NiTriStripsData* shape = (NiTriStripsData*)child;
						VerticesArray = shape->GetVertices();
						VertexCount = shape->GetVertexCount();
					}
					for (int j = 0; j < VertexCount; j++)
					{
						NiPoint3 CurP = VerticesArray[j] * scale + Translate;
						if (CurP.x < MinPoint.x)
							MinPoint.x = CurP.x;
						if (CurP.y < MinPoint.y)
							MinPoint.y = CurP.y;
						if (CurP.z < MinPoint.z)
							MinPoint.z = CurP.z;
						if (CurP.x > MaxPoint.x)
							MaxPoint.x = CurP.x;
						if (CurP.y > MaxPoint.y)
							MaxPoint.y = CurP.y;
						if (CurP.z > MaxPoint.z)
							MaxPoint.z = CurP.z;
					}
				}

				if (NiIsKindOf(NiNode, child))
				{
					GetNewVerticesData((NiNode*)child, MinPoint, MaxPoint, WorldRotate, ParentTranslate + child->GetTranslate());
				}
			}
		}
	}
	static NiNodePtr CreateBoundingBox(NiNode* Parent,NiNodePtr BoundingBox)
	{
		NiPoint3 MinPoint(0.f, 0.f, 0.f);
		NiPoint3 MaxPoint(0.f, 0.f, 0.f);
		NiMatrix3 WorldRotate;
		GetNewVerticesData(Parent, MinPoint, MaxPoint, WorldRotate);
		UpdateVertices(BoundingBox, MinPoint, MaxPoint);

		BoundingBox->SetRotate(WorldRotate);
		return BoundingBox;
	}
};