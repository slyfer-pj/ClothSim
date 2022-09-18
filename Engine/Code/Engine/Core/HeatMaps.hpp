#pragma once
#include "Engine/Math/IntVec2.hpp"
#include <vector>
#include <string>

class TileHeatMap
{
public:
	TileHeatMap() = default;
	TileHeatMap(const IntVec2& dimensions);
	float GetHeatValueAt(const IntVec2& tileCoordinate) const;
	float GetHighestHeatValue() const;
	float GetHighestHeatValueLessThan(float referenceHeatValue) const;
	IntVec2 GetLowestHeatNeighbouringTile(const IntVec2& referenceTileCoordinate) const;
	void SetHeatValueAt(const IntVec2& tileCoordinate, float heatValue);
	void AddHeatValueAt(const IntVec2& tileCoordinate, float heatToAdd);
	void SetAllValues(float newHeatValues);
	std::string ToString() const;

private:
	IntVec2 m_dimensions= IntVec2::ZERO;
	std::vector<float> m_values = {};

private:
	int GetTileIndexAtTileCoordinate(const IntVec2& tileCoordinate) const;
};