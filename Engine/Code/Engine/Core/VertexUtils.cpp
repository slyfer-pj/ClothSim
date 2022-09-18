#include "VertexUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Core/EngineCommon.hpp"

void TransformVertexArrayXY3D(int numVerts, Vertex_PCU* verts, float uniformScaleXY, float rotationDegreesAboutZ, const Vec2& translationXY)
{
	for (int vertIndex = 0; vertIndex < numVerts; vertIndex++)
	{
		Vec3& vert = verts[vertIndex].m_position;
		TransformPositionXY3D(vert, uniformScaleXY, rotationDegreesAboutZ, translationXY);
	}
}

void TransformVertexArray3D(int numVerts, Vertex_PCU* verts, const Mat44& transform)
{
	for (int i = 0; i < numVerts; i++)
	{
		Vec3& vert = verts[i].m_position;
		vert = transform.TransformPosition3D(vert);
	}
}

void TransformVertexArray3D(int numVerts, Vertex_PNCU* verts, const Mat44& transform)
{
	for (int i = 0; i < numVerts; i++)
	{
		Vec3& vert = verts[i].m_position;
		vert = transform.TransformPosition3D(vert);
	}
}

void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, const AABB2& localBounds, Rgba8 color)
{
	Vertex_PCU vertices[6];
	vertices[0].m_position = Vec3(localBounds.m_mins.x, localBounds.m_mins.y, 0.f);
	vertices[1].m_position = Vec3(localBounds.m_maxs.x, localBounds.m_mins.y, 0.f);
	vertices[2].m_position = Vec3(localBounds.m_maxs.x, localBounds.m_maxs.y, 0.f);
	vertices[3].m_position = Vec3(localBounds.m_maxs.x, localBounds.m_maxs.y, 0.f);
	vertices[4].m_position = Vec3(localBounds.m_mins.x, localBounds.m_maxs.y, 0.f);
	vertices[5].m_position = Vec3(localBounds.m_mins.x, localBounds.m_mins.y, 0.f);

	vertices[0].m_uvTexCoords = Vec2(0.f, 0.f);
	vertices[1].m_uvTexCoords = Vec2(1.f, 0.f);
	vertices[2].m_uvTexCoords = Vec2(1.f, 1.f);
	vertices[3].m_uvTexCoords = Vec2(1.f, 1.f);
	vertices[4].m_uvTexCoords = Vec2(0.f, 1.f);
	vertices[5].m_uvTexCoords = Vec2(0.f, 0.f);

	vertices[0].m_color = color;
	vertices[1].m_color = color;
	vertices[2].m_color = color;
	vertices[3].m_color = color;
	vertices[4].m_color = color;
	vertices[5].m_color = color;

	for (int i = 0; i < 6; i++)
	{
		verts.push_back(vertices[i]);
	}
}

void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, const AABB2& localBounds, Rgba8 color, const Vec2& uvAtMins, const Vec2& uvAtMaxs)
{
	Vertex_PCU vertices[6];
	vertices[0].m_position = Vec3(localBounds.m_mins.x, localBounds.m_mins.y, 0.f);
	vertices[1].m_position = Vec3(localBounds.m_maxs.x, localBounds.m_mins.y, 0.f);
	vertices[2].m_position = Vec3(localBounds.m_maxs.x, localBounds.m_maxs.y, 0.f);
	vertices[3].m_position = Vec3(localBounds.m_maxs.x, localBounds.m_maxs.y, 0.f);
	vertices[4].m_position = Vec3(localBounds.m_mins.x, localBounds.m_maxs.y, 0.f);
	vertices[5].m_position = Vec3(localBounds.m_mins.x, localBounds.m_mins.y, 0.f);

	vertices[0].m_uvTexCoords = uvAtMins;
	vertices[1].m_uvTexCoords = Vec2(uvAtMaxs.x, uvAtMins.y);
	vertices[2].m_uvTexCoords = uvAtMaxs;
	vertices[3].m_uvTexCoords = uvAtMaxs;
	vertices[4].m_uvTexCoords = Vec2(uvAtMins.x, uvAtMaxs.y);
	vertices[5].m_uvTexCoords = uvAtMins;

	vertices[0].m_color = color;
	vertices[1].m_color = color;
	vertices[2].m_color = color;
	vertices[3].m_color = color;
	vertices[4].m_color = color;
	vertices[5].m_color = color;

	for (int i = 0; i < 6; i++)
	{
		verts.push_back(vertices[i]);
	}

}

void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, const Capsule2& capsule, const Rgba8& color)
{
	AddVertsForCapsule2D(verts, capsule.m_bone.m_start, capsule.m_bone.m_end, capsule.m_radius, color);
}

void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, const Vec2& boneStart, const Vec2& boneEnd, float radius, const Rgba8& color)
{
	Vertex_PCU rectOfCapsuleVertices[6];
	Vec2 vectorFromStartToEnd = boneEnd - boneStart;
	vectorFromStartToEnd.SetLength(radius);

	//get corner points of the rect part of the capsule and add those verts
	Vec2 leftTop = boneStart - vectorFromStartToEnd.GetRotated90Degrees();
	Vec2 rightTop = boneStart + vectorFromStartToEnd.GetRotated90Degrees();
	Vec2 leftBot = boneEnd - vectorFromStartToEnd.GetRotated90Degrees();
	Vec2 rightBot = boneEnd + vectorFromStartToEnd.GetRotated90Degrees();

	rectOfCapsuleVertices[0].m_position = Vec3(leftTop.x, leftTop.y, 0.f);
	rectOfCapsuleVertices[1].m_position = Vec3(rightTop.x, rightTop.y, 0.f);
	rectOfCapsuleVertices[2].m_position = Vec3(rightBot.x, rightBot.y, 0.f);
	rectOfCapsuleVertices[3].m_position = Vec3(leftTop.x, leftTop.y, 0.f);
	rectOfCapsuleVertices[4].m_position = Vec3(rightBot.x, rightBot.y, 0.f);
	rectOfCapsuleVertices[5].m_position = Vec3(leftBot.x, leftBot.y, 0.f);

	for (int i = 0; i < 6; i++)
	{
		rectOfCapsuleVertices[i].m_color = color;
		verts.push_back(rectOfCapsuleVertices[i]);
	}

	Vertex_PCU semiCircleOfCapsuleVertices[27];
	float thetaDeg = vectorFromStartToEnd.GetRotated90Degrees().GetOrientationDegrees();

	for (int i = 0; i < 27; i++)
	{
		if (i % 3 == 2)
			thetaDeg += 180.f / 9.f;

		if (i % 3 == 0)
		{
			semiCircleOfCapsuleVertices[i].m_position = Vec3(boneStart.x, boneStart.y, 0.f);
		}
		else
		{
			semiCircleOfCapsuleVertices[i].m_position.x = radius * CosDegrees(thetaDeg) + boneStart.x;
			semiCircleOfCapsuleVertices[i].m_position.y = radius * SinDegrees(thetaDeg) + boneStart.y;
		}
	}

	for (int i = 0; i < 27; i++)
	{
		semiCircleOfCapsuleVertices[i].m_color = color;
		verts.push_back(semiCircleOfCapsuleVertices[i]);
	}

	thetaDeg = (-vectorFromStartToEnd.GetRotated90Degrees()).GetOrientationDegrees();

	for (int i = 0; i < 27; i++)
	{
		if (i % 3 == 2)
			thetaDeg += 180.f / 9.f;

		if (i % 3 == 0)
		{
			semiCircleOfCapsuleVertices[i].m_position = Vec3(boneEnd.x, boneEnd.y, 0.f);
		}
		else
		{
			semiCircleOfCapsuleVertices[i].m_position.x = radius * CosDegrees(thetaDeg) + boneEnd.x;
			semiCircleOfCapsuleVertices[i].m_position.y = radius * SinDegrees(thetaDeg) + boneEnd.y;
		}
	}

	for (int i = 0; i < 27; i++)
	{
		semiCircleOfCapsuleVertices[i].m_color = color;
		verts.push_back(semiCircleOfCapsuleVertices[i]);
	}
	
}

void AddVertsForDisc2D(std::vector<Vertex_PCU>& verts, const Vec2& center, float radius, const Rgba8& color)
{
	Vertex_PCU vertices[48];
	float thetaDeg = 0.f;

	for (int vertIndex = 0; vertIndex < 48; vertIndex++)
	{
		//increase angle for every 3rd vertex of a triangle.
		if (vertIndex % 3 == 2)
			thetaDeg += 360.f / 16.f;

		//the first vertex will always be center
		if (vertIndex % 3 == 0)
		{
			vertices[vertIndex].m_position = Vec3(center.x, center.y, 0.f);
		}
		else
		{
			vertices[vertIndex].m_position.x = radius * CosDegrees(thetaDeg) + center.x;
			vertices[vertIndex].m_position.y = radius * SinDegrees(thetaDeg) + center.y;
		}

		vertices[vertIndex].m_color = color;

		//add vertex to vector
		verts.push_back(vertices[vertIndex]);
	}
}

void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, const OBB2& box, const Rgba8& color)
{
	Vec2 rightDisp = box.m_iBasisNormal * box.m_halfDimensions.x;
	Vec2 upDisp = box.m_iBasisNormal.GetRotated90Degrees() * box.m_halfDimensions.y;

	Vec2 topRightPoint = box.m_center + rightDisp + upDisp;
	Vec2 topLeftPoint = box.m_center - rightDisp + upDisp;
	Vec2 botRightPoint = box.m_center + rightDisp - upDisp;
	Vec2 botLeftPoint = box.m_center - rightDisp - upDisp;

	Vertex_PCU vertices[6];
	vertices[0].m_position = Vec3(topRightPoint.x, topRightPoint.y, 0.f);
	vertices[1].m_position = Vec3(botRightPoint.x, botRightPoint.y, 0.f);
	vertices[2].m_position = Vec3(botLeftPoint.x, botLeftPoint.y, 0.f);
	vertices[3].m_position = Vec3(topRightPoint.x, topRightPoint.y, 0.f);
	vertices[4].m_position = Vec3(botLeftPoint.x, botLeftPoint.y, 0.f);
	vertices[5].m_position = Vec3(topLeftPoint.x, topLeftPoint.y, 0.f);

	for (int i = 0; i < 6; i++)
	{
		vertices[i].m_color = color;
		verts.push_back(vertices[i]);
	}
}

void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, const Vec2& start, const Vec2& end, float thickness, const Rgba8& color)
{
	float halfWidth = thickness * 0.5f;
	Vec2 unitForwardVector = (end - start) / (end - start).GetLength();
	Vec2 unitLeftVector = unitForwardVector.GetRotated90Degrees();
	Vec2 leftTopCorner = start + (halfWidth * unitLeftVector) - (halfWidth * unitForwardVector);
	Vec2 leftBottomCorner = start - (halfWidth * unitLeftVector) - (halfWidth * unitForwardVector);
	Vec2 rightTopCorner = end + (halfWidth * unitLeftVector) + (halfWidth * unitForwardVector);
	Vec2 rightBottomCorner = end - (halfWidth * unitLeftVector) + (halfWidth * unitForwardVector);

	Vertex_PCU drawVerticesForLine[6];
	drawVerticesForLine[0].m_position = Vec3(leftTopCorner.x, leftTopCorner.y, 0.f);
	drawVerticesForLine[1].m_position = Vec3(leftBottomCorner.x, leftBottomCorner.y, 0.f);
	drawVerticesForLine[2].m_position = Vec3(rightTopCorner.x, rightTopCorner.y, 0.f);
	drawVerticesForLine[3].m_position = Vec3(leftBottomCorner.x, leftBottomCorner.y, 0.f);
	drawVerticesForLine[4].m_position = Vec3(rightBottomCorner.x, rightBottomCorner.y, 0.f);
	drawVerticesForLine[5].m_position = Vec3(rightTopCorner.x, rightTopCorner.y, 0.f);

	drawVerticesForLine[0].m_color = color;
	drawVerticesForLine[1].m_color = color;
	drawVerticesForLine[2].m_color = color;
	drawVerticesForLine[3].m_color = color;
	drawVerticesForLine[4].m_color = color;
	drawVerticesForLine[5].m_color = color;

	for (int i = 0; i < 6; i++)
		verts.push_back(drawVerticesForLine[i]);
}

void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, const LineSegment2& lineSegment, float thickness, const Rgba8& color)
{
	AddVertsForLineSegment2D(verts, lineSegment.m_start, lineSegment.m_end, thickness, color);
}

void AddVertsForRing2D(std::vector<Vertex_PCU>& verts, const Vec2& ringCenter, float ringRadius, Rgba8 color, float ringThickness)
{
	float radiusOuter = ringRadius;
	float radiusInner = ringRadius - ringThickness;

	float thetaDeg = 0;
	float deltaThethaDeg = 360.f / 16.f;

	int vertIndex = 0;
	Vec2 pointsOnCircles[4];

	Vertex_PCU drawVerticesForRing[96];
	int drawVertIndex = 0;
	int vertexOrder[6] = { 0, 3, 1, 0, 2, 3 };

	while (thetaDeg <= 360.f)
	{
		if (vertIndex != 0 && vertIndex % 4 == 0)
		{
			for (int i = 0; i < 6; i++)
			{
				drawVerticesForRing[drawVertIndex].m_position = Vec3(pointsOnCircles[vertexOrder[i]].x + ringCenter.x
					, pointsOnCircles[vertexOrder[i]].y + ringCenter.y, 0);
				drawVerticesForRing[drawVertIndex].m_color = color;
				drawVertIndex++;
			}

			vertIndex = 0;
		}


		if (vertIndex != 0 && vertIndex % 2 == 0)
			thetaDeg += deltaThethaDeg;

		pointsOnCircles[vertIndex % 4].x = radiusOuter * CosDegrees(thetaDeg);
		pointsOnCircles[vertIndex % 4].y = radiusOuter * SinDegrees(thetaDeg);

		vertIndex++;

		pointsOnCircles[vertIndex % 4].x = radiusInner * CosDegrees(thetaDeg);
		pointsOnCircles[vertIndex % 4].y = radiusInner * SinDegrees(thetaDeg);

		vertIndex++;
	}

	verts.reserve(96);
	for (int i = 0; i < 96; i++)
	{
		verts.push_back(drawVerticesForRing[i]);
	}
}

void AddVertsForArrow2D(std::vector<Vertex_PCU>& verts, const Vec2& tailPos, const Vec2& tipPos, float lineThickness, const Rgba8& color)
{
	constexpr float arrowSize = 2.f;
	constexpr float arrowHeadLength = 0.8f;
	Vec2 tailToTipDirection = (tipPos - tailPos).GetNormalized();
	Vec2 arrowBeginPoint = tipPos + (-tailToTipDirection) * arrowHeadLength;
	Vec2 arrowLeftPoint = arrowBeginPoint + (tailToTipDirection.GetRotated90Degrees() * arrowSize * 0.5f);
	Vec2 arrowRightPoint = arrowBeginPoint + (tailToTipDirection.GetRotatedMinus90Degrees() * arrowSize * 0.5f);

	AddVertsForLineSegment2D(verts, arrowLeftPoint, tipPos, lineThickness, color);
	AddVertsForLineSegment2D(verts, arrowRightPoint, tipPos, lineThickness, color);
	AddVertsForLineSegment2D(verts, tailPos, tipPos, lineThickness, color);
}

void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color, const AABB2& UVs)
{
	Vertex_PCU vertices[6];
	vertices[0].m_position = topLeft;
	vertices[0].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	vertices[1].m_position = bottomLeft;
	vertices[1].m_uvTexCoords = UVs.m_mins;
	vertices[2].m_position = bottomRight;
	vertices[2].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	vertices[3].m_position = topLeft;
	vertices[3].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	vertices[4].m_position = bottomRight;
	vertices[4].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	vertices[5].m_position = topRight;
	vertices[5].m_uvTexCoords = UVs.m_maxs;

	for (int i = 0; i < 6; i++)
	{
		vertices[i].m_color = color;
		verts.push_back(vertices[i]);
	}
}

void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indices, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color /*= Rgba8::WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	int baseIndex = int(verts.size());
	Vertex_PCU vertices[4];
	vertices[0].m_position = topLeft;
	vertices[0].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	vertices[0].m_color = color;
	verts.push_back(vertices[0]);
	vertices[1].m_position = bottomLeft;
	vertices[1].m_uvTexCoords = UVs.m_mins;
	vertices[1].m_color = color;
	verts.push_back(vertices[1]);
	vertices[2].m_position = bottomRight;
	vertices[2].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	vertices[2].m_color = color;
	verts.push_back(vertices[2]);
	vertices[3].m_position = topRight;
	vertices[3].m_uvTexCoords = UVs.m_maxs;
	vertices[3].m_color = color;
	verts.push_back(vertices[3]);
	
	indices.reserve(6);
	indices.push_back(0 + baseIndex);
	indices.push_back(1 + baseIndex);
	indices.push_back(2 + baseIndex);
	indices.push_back(0 + baseIndex);
	indices.push_back(2 + baseIndex);
	indices.push_back(3 + baseIndex);
}

void AddVertsForQuad3D(std::vector<Vertex_PNCU>& verts, std::vector<unsigned int>& indices, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color /*= Rgba8::WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	int baseIndex = int(verts.size());
	Vertex_PNCU vertices[4];
	vertices[0].m_position = topLeft;
	vertices[0].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	vertices[0].m_color = color;
	vertices[0].m_normal = CrossProduct3D((bottomLeft - topLeft).GetNormalized(), (topRight - topLeft).GetNormalized()).GetNormalized();
	verts.push_back(vertices[0]);
	vertices[1].m_position = bottomLeft;
	vertices[1].m_uvTexCoords = UVs.m_mins;
	vertices[1].m_color = color;
	vertices[1].m_normal = vertices[0].m_normal;
	verts.push_back(vertices[1]);
	vertices[2].m_position = bottomRight;
	vertices[2].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	vertices[2].m_color = color;
	vertices[2].m_normal = vertices[0].m_normal;
	verts.push_back(vertices[2]);
	vertices[3].m_position = topRight;
	vertices[3].m_uvTexCoords = UVs.m_maxs;
	vertices[3].m_color = color;
	vertices[3].m_normal = vertices[0].m_normal;
	verts.push_back(vertices[3]);

	indices.reserve(6);
	indices.push_back(0 + baseIndex);
	indices.push_back(1 + baseIndex);
	indices.push_back(2 + baseIndex);
	indices.push_back(0 + baseIndex);
	indices.push_back(2 + baseIndex);
	indices.push_back(3 + baseIndex);
}

void AddVertsForQuad3D(std::vector<Vertex_PNCU>& verts, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color /*= Rgba8::WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	Vertex_PNCU vertices[6];
	vertices[0].m_position = topLeft;
	vertices[0].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	vertices[0].m_normal = CrossProduct3D((bottomLeft - topLeft).GetNormalized(), (topRight - topLeft).GetNormalized()).GetNormalized();
	vertices[1].m_position = bottomLeft;
	vertices[1].m_uvTexCoords = UVs.m_mins;
	vertices[1].m_normal = vertices[0].m_normal;
	vertices[2].m_position = bottomRight;
	vertices[2].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	vertices[2].m_normal = vertices[0].m_normal;
	vertices[3].m_position = topLeft;
	vertices[3].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	vertices[3].m_normal = vertices[0].m_normal;
	vertices[4].m_position = bottomRight;
	vertices[4].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	vertices[4].m_normal = vertices[0].m_normal;
	vertices[5].m_position = topRight;
	vertices[5].m_uvTexCoords = UVs.m_maxs;
	vertices[5].m_normal = vertices[0].m_normal;

	for (int i = 0; i < 6; i++)
	{
		vertices[i].m_color = color;
		verts.push_back(vertices[i]);
	}


}
//void AddVertsForRoundedQuad3D(std::vector<Vertex_PNCU>& vertexes, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color /*= Rgba8::WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/)
//{
//	float quadWidth = GetDistance3D(topLeft, topRight);
//	Vec3 topMid = topLeft + (topRight - topLeft).GetNormalized() * quadWidth * 0.5f;
//	Vec3 bottomMid = bottomLeft + (bottomRight - bottomLeft).GetNormalized() * quadWidth * 0.5f;
//
//	float midbottomxUV = (UVs.m_mins.x + UVs.m_maxs.x) * 0.5f;
//	float midbottomyUV = UVs.m_mins.y;
//	float midtopxUV = (UVs.m_mins.x + UVs.m_maxs.x) * 0.5f;
//	float midtopyUV = UVs.m_maxs.y;
//	Vec2 bottomMidUVS = Vec2(midbottomxUV, midbottomyUV);
//	Vec2 topMidUVS = Vec2(midtopxUV, midtopyUV);
//	Vec3 leftNormal = -(bottomLeft - bottomRight).GetNormalized();
//	Vec3 rightNormal = -(bottomRight - bottomLeft).GetNormalized();
//	Vec3 centerNormal = (topLeft - bottomLeft).GetNormalized();
//	Vec3 topCenterNormal = CrossProduct3D(leftNormal, centerNormal);
//	Vec3 bottomCenterNormal = CrossProduct3D(leftNormal, -centerNormal);
//	Vertex_PNCU tempVertexArrays[12];
//	//-----------------POSITIONS----------------------
//	tempVertexArrays[0].m_position = bottomLeft;
//	tempVertexArrays[1].m_position = bottomMid;
//	tempVertexArrays[2].m_position = topLeft;
//	tempVertexArrays[3].m_position = topLeft;
//	tempVertexArrays[4].m_position = bottomMid;
//	tempVertexArrays[5].m_position = topMid;
//	tempVertexArrays[6].m_position = bottomMid;
//	tempVertexArrays[7].m_position = bottomRight;
//	tempVertexArrays[8].m_position = topMid;
//	tempVertexArrays[9].m_position = topMid;
//	tempVertexArrays[10].m_position = bottomRight;
//	tempVertexArrays[11].m_position = topRight;
//	//-----------------NORMALS------------------------
//	tempVertexArrays[0].m_normal = leftNormal;
//	tempVertexArrays[1].m_normal = bottomCenterNormal;
//	tempVertexArrays[2].m_normal = leftNormal;
//	tempVertexArrays[3].m_normal = leftNormal;
//	tempVertexArrays[4].m_normal = bottomCenterNormal;
//	tempVertexArrays[5].m_normal = topCenterNormal;
//	tempVertexArrays[6].m_normal = bottomCenterNormal;
//	tempVertexArrays[7].m_normal = rightNormal;
//	tempVertexArrays[8].m_normal = topCenterNormal;
//	tempVertexArrays[9].m_normal = topCenterNormal;
//	tempVertexArrays[10].m_normal = rightNormal;
//	tempVertexArrays[11].m_normal = rightNormal;
//	//---------------UVS-------------------
//	tempVertexArrays[0].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_mins.y);
//	tempVertexArrays[1].m_uvTexCoords = bottomMidUVS;
//	tempVertexArrays[2].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
//	tempVertexArrays[3].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
//	tempVertexArrays[4].m_uvTexCoords = bottomMidUVS;
//	tempVertexArrays[5].m_uvTexCoords = topMidUVS;
//	tempVertexArrays[6].m_uvTexCoords = bottomMidUVS;
//	tempVertexArrays[7].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
//	tempVertexArrays[8].m_uvTexCoords = topMidUVS;
//	tempVertexArrays[9].m_uvTexCoords = topMidUVS;
//	tempVertexArrays[10].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
//	tempVertexArrays[11].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_maxs.y);
//	for (int i = 0; i < 12; i++)
//	{
//		tempVertexArrays[i].m_color = color;
//		vertexes.push_back(tempVertexArrays[i]);
//	}
//}

void AddVertsForRoundedQuad3D(std::vector<Vertex_PNCU>& vertexes, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color /*= Rgba8::WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	//split the regular quad into by having 2 vertices in the middle
	float quadWidth = GetDistance3D(topLeft, topRight);
	Vec3 topCenter = topLeft + (topRight - topLeft).GetNormalized() * quadWidth * 0.5f;
	Vec3 bottomCenter = bottomLeft + (bottomRight - bottomLeft).GetNormalized() * quadWidth * 0.5f;
	Vec3 uNormal = (bottomRight - bottomLeft).GetNormalized();
	Vec3 vNormal = (topLeft - bottomLeft).GetNormalized();
	float uvWidth = UVs.m_maxs.x - UVs.m_mins.x;

	Vertex_PNCU vertices[12];
	//quad 1
	vertices[0].m_position = topLeft;
	vertices[0].m_color = color;
	vertices[0].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	vertices[0].m_normal = -uNormal;
	vertices[1].m_position = bottomLeft;
	vertices[1].m_color = color;
	vertices[1].m_uvTexCoords = UVs.m_mins;
	vertices[1].m_normal = -uNormal;
	vertices[2].m_position = bottomCenter;
	vertices[2].m_color = color;
	vertices[2].m_uvTexCoords = Vec2(UVs.m_mins.x + (uvWidth * 0.5f), UVs.m_mins.y);
	vertices[2].m_normal = CrossProduct3D(uNormal, vNormal).GetNormalized();
	vertices[3].m_position = topLeft;
	vertices[3].m_color = color;
	vertices[3].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	vertices[3].m_normal = -uNormal;
	vertices[4].m_position = bottomCenter;
	vertices[4].m_color = color;
	vertices[4].m_uvTexCoords = Vec2(UVs.m_mins.x + (uvWidth * 0.5f), UVs.m_mins.y);
	vertices[4].m_normal = vertices[2].m_normal;
	vertices[5].m_position = topCenter;
	vertices[5].m_color = color;
	vertices[5].m_uvTexCoords = Vec2(UVs.m_mins.x + (uvWidth * 0.5f), UVs.m_maxs.y);
	vertices[5].m_normal = vertices[2].m_normal;

	//quad 2
	vertices[6].m_position = topCenter;
	vertices[6].m_color = color;
	vertices[6].m_uvTexCoords = Vec2(UVs.m_mins.x + (uvWidth * 0.5f), UVs.m_maxs.y);
	vertices[6].m_normal = vertices[2].m_normal;
	vertices[7].m_position = bottomCenter;
	vertices[7].m_color = color;
	vertices[7].m_uvTexCoords = Vec2(UVs.m_mins.x + (uvWidth * 0.5f), UVs.m_mins.y);
	vertices[7].m_normal = vertices[2].m_normal;
	vertices[8].m_position = bottomRight;
	vertices[8].m_color = color;
	vertices[8].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	vertices[8].m_normal = uNormal;
	vertices[9].m_position = topCenter;
	vertices[9].m_color = color;
	vertices[9].m_uvTexCoords = Vec2(UVs.m_mins.x + (uvWidth * 0.5f), UVs.m_maxs.y);
	vertices[9].m_normal = vertices[2].m_normal;
	vertices[10].m_position = bottomRight;
	vertices[10].m_color = color;
	vertices[10].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	vertices[10].m_normal = uNormal;
	vertices[11].m_position = topRight;
	vertices[11].m_color = color;
	vertices[11].m_uvTexCoords = UVs.m_maxs;
	vertices[11].m_normal = uNormal;

	for (int i = 0; i < 12; i++)
	{
		vertexes.push_back(vertices[i]);
	}
}

void AddVertsForAABB3D(std::vector<Vertex_PCU>& verts, const AABB3& bounds, const Rgba8& color, const AABB2& UVs)
{
	//x forward, y left, z up
	Vec3 near_topLeft(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_maxs.z);
	Vec3 near_bottomLeft(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_mins.z);
	Vec3 near_bottomRight(bounds.m_mins.x, bounds.m_mins.y, bounds.m_mins.z);
	Vec3 near_topRight(bounds.m_mins.x, bounds.m_mins.y, bounds.m_maxs.z);
	Vec3 far_topLeft(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_maxs.z);
	Vec3 far_bottomLeft(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_mins.z);
	Vec3 far_bottomRight(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_mins.z);
	Vec3 far_topRight(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_maxs.z);

	AddVertsForQuad3D(verts, near_topLeft, near_bottomLeft, near_bottomRight, near_topRight, color, UVs);	//-x face 
	AddVertsForQuad3D(verts, near_topRight, near_bottomRight, far_bottomRight, far_topRight, color, UVs);	//-y face
	AddVertsForQuad3D(verts, far_topRight, far_bottomRight, far_bottomLeft, far_topLeft, color, UVs);		//+x face
	AddVertsForQuad3D(verts, far_topLeft, far_bottomLeft, near_bottomLeft, near_topLeft, color, UVs);		//+y face
	AddVertsForQuad3D(verts, far_topLeft, near_topLeft, near_topRight, far_topRight, color, UVs);			//+z face
	AddVertsForQuad3D(verts, near_bottomLeft, far_bottomLeft, far_bottomRight, near_bottomRight, color, UVs);	//-z face
}

void AddVertsForSphere(std::vector<Vertex_PCU>& verts, int longitudinalSlices, int latitudinalStacks, float radius, const Vec3& center, const Rgba8& color)
{
	std::vector<Vertex_PCU> sphereVerts;
	float currentYaw = 0.f;
	float currentPitch = -90.f;
	float deltaYaw = 360.f / longitudinalSlices;
	float deltaPitch = 180.f / latitudinalStacks;

	for(int slice = 0; slice < longitudinalSlices; slice++)
	{
		float cy = CosDegrees(currentYaw);
		float sy = SinDegrees(currentYaw);
		float cyNext = CosDegrees(currentYaw + deltaYaw);
		float syNext = SinDegrees(currentYaw + deltaYaw);

		currentPitch = -90.f;
		for (int stack = 0; stack < latitudinalStacks; stack++)
		{
			float cp = CosDegrees(currentPitch);
			float sp = SinDegrees(currentPitch);
			float cpNext = CosDegrees(currentPitch + deltaPitch);
			float spNext = SinDegrees(currentPitch + deltaPitch);

			Vertex_PCU topLeft, botLeft, topRight, botRight;
			topLeft.m_position  = Vec3(cy * cp, sy * cp, -sp) * radius;
			topLeft.m_uvTexCoords.x = currentYaw / 360.f;
			topLeft.m_uvTexCoords.y = RangeMap(currentPitch, 90.f, -90.f, 0.f, 1.f);
			topLeft.m_color = color;

			botLeft.m_position = Vec3(cy * cpNext, sy * cpNext, -spNext) * radius;
			botLeft.m_uvTexCoords.x = currentYaw / 360.f;
			botLeft.m_uvTexCoords.y = RangeMap(currentPitch + deltaPitch, 90.f, -90.f, 0.f, 1.f);
			botLeft.m_color = color;

			topRight.m_position = Vec3(cyNext * cp, syNext * cp, -sp) * radius;
			topRight.m_uvTexCoords.x = (currentYaw + deltaYaw) / 360.f;
			topRight.m_uvTexCoords.y = RangeMap(currentPitch, 90.f, -90.f, 0.f, 1.f);
			topRight.m_color = color;

			botRight.m_position = Vec3(cyNext * cpNext, syNext * cpNext, -spNext) * radius;
			botRight.m_uvTexCoords.x = (currentYaw + deltaYaw) / 360.f;
			botRight.m_uvTexCoords.y = RangeMap(currentPitch + deltaPitch, 90.f, -90.f, 0.f, 1.f);
			botRight.m_color = color;

			sphereVerts.push_back(topLeft);
			sphereVerts.push_back(botLeft);
			sphereVerts.push_back(botRight);
			sphereVerts.push_back(topLeft);
			sphereVerts.push_back(botRight);
			sphereVerts.push_back(topRight);

			currentPitch += deltaPitch;
		}

		currentYaw += deltaYaw;
	}

	if (center != Vec3())
	{
		Mat44 translation = Mat44::CreateTranslation3D(center);
		TransformVertexArray3D((int)sphereVerts.size(), sphereVerts.data(), translation);
	}

	for (int i = 0; i < sphereVerts.size(); i++)
	{
		verts.push_back(sphereVerts[i]);
	}
}

void AddVertsForCylinder3D(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, const Rgba8& color)
{
	std::vector<Vertex_PCU> cylinderVerts;
	constexpr int slices = 10;
	float height = (end - start).GetLength();
	float currentYaw = 0.f;
	float deltaYaw = 360.f / (float)slices;
	float cylinderZ = height * 0.5f;
	Vec3 cylinderTopCenter(0.f, 0.f, cylinderZ);
	Vec3 cylinderBottomCenter(0.f, 0.f, -cylinderZ);

	for (int i = 0; i < slices; i++)
	{
		float cy = CosDegrees(currentYaw);
		float sy = SinDegrees(currentYaw);
		float cyNext = CosDegrees(currentYaw + deltaYaw);
		float syNext = SinDegrees(currentYaw + deltaYaw);

		//top surface points
		Vertex_PCU topCenter, topPointA, topPointB;
		topCenter.m_position = cylinderTopCenter;
		topCenter.m_uvTexCoords = Vec2(0.5f, 0.5f);
		topCenter.m_color = color;

		topPointA.m_position = Vec3(cy * radius, sy * radius, cylinderZ);
		topPointA.m_color = color;
		topPointB.m_position = Vec3(cyNext * radius, syNext * radius, cylinderZ);
		topPointB.m_color = color;

		//bottom surface points
		Vertex_PCU botCenter, botPointA, botPointB;
		botCenter.m_position = cylinderBottomCenter;
		botCenter.m_uvTexCoords = Vec2(0.5f, 0.5f);
		botCenter.m_color = color;

		botPointA.m_position = Vec3(cy * radius, sy * radius, -cylinderZ);
		botPointA.m_color = color;
		botPointB.m_position = Vec3(cyNext * radius, syNext * radius, -cylinderZ);
		botPointB.m_color = color;

		//top circle pie
		topPointA.m_uvTexCoords.x = (cy + 1) * 0.5f;
		topPointA.m_uvTexCoords.y = (sy + 1) * 0.5f;
		topPointB.m_uvTexCoords.x = (cyNext + 1) * 0.5f;
		topPointB.m_uvTexCoords.y = (syNext + 1) * 0.5f;
		cylinderVerts.push_back(topCenter);
		cylinderVerts.push_back(topPointA);
		cylinderVerts.push_back(topPointB);

		//bottom circle pie
		botPointA.m_uvTexCoords.x = topPointA.m_uvTexCoords.x;
		botPointA.m_uvTexCoords.y = 1.f - topPointA.m_uvTexCoords.y;
		botPointB.m_uvTexCoords.x = topPointB.m_uvTexCoords.x;
		botPointB.m_uvTexCoords.y = 1.f - topPointB.m_uvTexCoords.y;
		cylinderVerts.push_back(botPointB);
		cylinderVerts.push_back(botPointA);
		cylinderVerts.push_back(botCenter);

		//cylinder surface rect
		topPointA.m_uvTexCoords = Vec2(currentYaw / 360.f, 1.f);
		topPointB.m_uvTexCoords = Vec2((currentYaw + deltaYaw) / 360.f, 1.f);
		botPointA.m_uvTexCoords = Vec2(currentYaw / 360.f, 0.f);
		botPointB.m_uvTexCoords = Vec2((currentYaw + deltaYaw) / 360.f, 0.f);
		cylinderVerts.push_back(topPointA);
		cylinderVerts.push_back(botPointA);
		cylinderVerts.push_back(botPointB);
		cylinderVerts.push_back(topPointA);
		cylinderVerts.push_back(botPointB);
		cylinderVerts.push_back(topPointB);

		currentYaw += deltaYaw;
	}

	Vec3 jBasis, iBasis;
	Vec3 worldZ = Vec3(0.f, 0.f, 1.f);
	Vec3 worldX = Vec3(1.f, 0.f, 0.f);
	Vec3 kBasis = (end - start).GetNormalized();
	if (abs(DotProduct3D(kBasis, worldZ)) < 1.f)
	{
		jBasis = CrossProduct3D(worldZ, kBasis).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	else
	{
		jBasis = CrossProduct3D(kBasis, worldX).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}

	Vec3 translation = start + ((end - start).GetNormalized() * height * 0.5f);
	Mat44 transform(iBasis, jBasis, kBasis, translation);
	TransformVertexArray3D((int)cylinderVerts.size(), cylinderVerts.data(), transform);

	for (int i = 0; i < cylinderVerts.size(); i++)
	{
		verts.push_back(cylinderVerts[i]);
	}
}

void AddVertsForArrow3D(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, const Rgba8& color)
{
	constexpr float arrowHeadLengthFraction = 0.2f;
	constexpr float arrowShaftRadiusFraction = 0.5f;
	float totalArrowLength = (end - start).GetLength();
	Vec3 direction = (end - start).GetNormalized();
	float arrowShaftLength = totalArrowLength * (1.f - arrowHeadLengthFraction);
	AddVertsForCylinder3D(verts, start, start + (direction * arrowShaftLength), radius * arrowShaftRadiusFraction, color);

	//draw arrow cone
	std::vector<Vertex_PCU> coneVerts;
	Vec3 coneBase = start + (direction * arrowShaftLength);
	AddVertsForCone3D(coneVerts, coneBase, end, radius, color);

	for (int i = 0; i < coneVerts.size(); i++)
	{
		verts.push_back(coneVerts[i]);
	}
}

void AddVertsForCone3D(std::vector<Vertex_PCU>& verts, const Vec3& base, const Vec3& tip, float baseRadius, const Rgba8& color)
{
	std::vector<Vertex_PCU> coneVerts;
	constexpr int slices = 10;
	float height = (tip - base).GetLength();
	float deltaYaw = 360.f / (float)slices;
	float currentYaw = 0.f;
	Vec3 coneBase(0.f, 0.f, -height * 0.5f);
	Vec3 coneTip(0.f, 0.f, height * 0.5f);

	for (int i = 0; i < slices; i++)
	{
		float cy = CosDegrees(currentYaw) * baseRadius;
		float sy = SinDegrees(currentYaw) * baseRadius;
		float cyNext = CosDegrees(currentYaw + deltaYaw) * baseRadius;
		float syNext = SinDegrees(currentYaw + deltaYaw) * baseRadius;

		//surface triangles
		Vertex_PCU coneTop, coneBot, basePointA, basePointB;
		coneTop.m_position = coneTip;
		coneTop.m_uvTexCoords = Vec2(currentYaw / 360.f, 1.f);
		coneTop.m_color = color;
		basePointA.m_position = Vec3(cy, sy, -height * 0.5f);
		basePointA.m_uvTexCoords = Vec2(currentYaw / 360.f, 0.f);
		basePointA.m_color = color;
		basePointB.m_position = Vec3(cyNext, syNext, -height * 0.5f);
		basePointB.m_uvTexCoords = Vec2((currentYaw + deltaYaw) / 360.f, 0.f);
		basePointB.m_color = color;

		coneVerts.push_back(coneTop);
		coneVerts.push_back(basePointA);
		coneVerts.push_back(basePointB);

		//base circle triangles
		coneBot.m_position = coneBase;
		coneBot.m_uvTexCoords = Vec2(0.5f, 0.5f);
		coneBot.m_color = color;
		basePointA.m_uvTexCoords.x = 1.f - ((cy + 1) * 0.5f);
		basePointA.m_uvTexCoords.y = 1.f - ((sy + 1) * 0.5f);
		basePointA.m_uvTexCoords.x = 1.f - ((cyNext + 1) * 0.5f);
		basePointA.m_uvTexCoords.y = 1.f - ((syNext + 1) * 0.5f);
		
		coneVerts.push_back(coneBot);
		coneVerts.push_back(basePointB);
		coneVerts.push_back(basePointA);

		currentYaw += deltaYaw;
	}

	Vec3 jBasis, iBasis;
	Vec3 worldZ = Vec3(0.f, 0.f, 1.f);
	Vec3 worldX = Vec3(1.f, 0.f, 0.f);
	Vec3 kBasis = (tip - base).GetNormalized();
	if ( abs(DotProduct3D(kBasis, worldZ)) < 1.f)
	{
		jBasis = CrossProduct3D(worldZ, kBasis).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	else
	{
		jBasis = CrossProduct3D(kBasis, worldX).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}

	Vec3 translation = base + ((tip - base).GetNormalized() * height * 0.5f);
	Mat44 transform(iBasis, jBasis, kBasis, translation);
	TransformVertexArray3D((int)coneVerts.size(), coneVerts.data(), transform);

	for (int i = 0; i < coneVerts.size(); i++)
	{
		verts.push_back(coneVerts[i]);
	}
}
