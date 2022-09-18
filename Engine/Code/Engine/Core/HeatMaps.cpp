#include "Engine/Core/HeatMaps.hpp"

TileHeatMap::TileHeatMap(const IntVec2& dimensions)
	:m_dimensions(dimensions)
{
	m_values.reserve(dimensions.x * dimensions.y);
	for (int y = 0; y < dimensions.y; y++)
	{
		for (int x = 0; x < dimensions.x; x++)
		{
			m_values.push_back(0.f);
		}
	}
}

float TileHeatMap::GetHeatValueAt(const IntVec2& tileCoordinate) const
{
	return m_values[GetTileIndexAtTileCoordinate(tileCoordinate)];
}

float TileHeatMap::GetHighestHeatValue() const
{
	float max = 0.f;
	for (int i = 0; i < m_values.size(); i++)
	{
		if (m_values[i] > max)
			max = m_values[i];
	}

	return max;
}

float TileHeatMap::GetHighestHeatValueLessThan(float referenceHeatValue) const
{
	float max = 0.f;
	for (int i = 0; i < m_values.size(); i++)
	{
		if (m_values[i] > max && m_values[i] < referenceHeatValue)
			max = m_values[i];
	}

	return max;
}

IntVec2 TileHeatMap::GetLowestHeatNeighbouringTile(const IntVec2& referenceTileCoordinate) const
{
	IntVec2 lowestHeatNeighbour = IntVec2::ZERO;
	float minHeat = 99999.f;
	float neighbourHeatValue = 0.f;
	neighbourHeatValue = GetHeatValueAt(referenceTileCoordinate + IntVec2(0, 1));
	if (neighbourHeatValue < minHeat)
	{
		minHeat = neighbourHeatValue;
		lowestHeatNeighbour = referenceTileCoordinate + IntVec2(0, 1);
	}

	neighbourHeatValue = GetHeatValueAt(referenceTileCoordinate + IntVec2(0, -1));
	if (neighbourHeatValue < minHeat)
	{
		minHeat = neighbourHeatValue;
		lowestHeatNeighbour = referenceTileCoordinate + IntVec2(0, -1);
	}

	neighbourHeatValue = GetHeatValueAt(referenceTileCoordinate + IntVec2(1, 0));
	if (neighbourHeatValue < minHeat)
	{
		minHeat = neighbourHeatValue;
		lowestHeatNeighbour = referenceTileCoordinate + IntVec2(1, 0);
	}

	neighbourHeatValue = GetHeatValueAt(referenceTileCoordinate + IntVec2(-1, 0));
	if (neighbourHeatValue < minHeat)
	{
		minHeat = neighbourHeatValue;
		lowestHeatNeighbour = referenceTileCoordinate + IntVec2(-1, 0);
	}

	return lowestHeatNeighbour;
}

void TileHeatMap::SetHeatValueAt(const IntVec2& tileCoordinate, float heatValue)
{
	m_values[GetTileIndexAtTileCoordinate(tileCoordinate)] = heatValue;
}

void TileHeatMap::AddHeatValueAt(const IntVec2& tileCoordinate, float heatToAdd)
{
	m_values[GetTileIndexAtTileCoordinate(tileCoordinate)] += heatToAdd;
}

void TileHeatMap::SetAllValues(float newHeatValues)
{
	for (int i = 0; i < m_values.size(); i++)
	{
		m_values[i] = newHeatValues;
	}
}

std::string TileHeatMap::ToString() const
{
	std::string returnVal = "";
	for (int i = 0; i < m_values.size(); i++)
	{
		returnVal.append(std::to_string(m_values[i]));
		returnVal.append(", ");
	}
	return returnVal;
}

int TileHeatMap::GetTileIndexAtTileCoordinate(const IntVec2& tileCoordinate) const
{
	return tileCoordinate.x + (tileCoordinate.y * m_dimensions.x);
}

