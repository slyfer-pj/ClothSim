#include <string>
#include "Engine/Core/XmlUtils.hpp"

int ParseXmlAttribute(const XmlElement& element, const char* attributeName, int defaultValue)
{
	int returnValue = defaultValue;
	element.QueryIntAttribute(attributeName, &returnValue);
	return returnValue;
}

char ParseXmlAttribute(const XmlElement& element, const char* attributeName, char defaultValue)
{
	char returnValue = defaultValue;
	const char* attributeQuery = "";
	element.QueryStringAttribute(attributeName, &attributeQuery);
	if (attributeQuery[0] == '\0')
		returnValue = attributeQuery[0];

	return returnValue;
}

bool ParseXmlAttribute(const XmlElement& element, const char* attributeName, bool defaultValue)
{
	bool returnValue = defaultValue;
	element.QueryBoolAttribute(attributeName, &returnValue);
	return returnValue;
}

float ParseXmlAttribute(const XmlElement& element, const char* attributeName, float defaultValue)
{
	float returnValue = defaultValue;
	element.QueryFloatAttribute(attributeName, &returnValue);
	return returnValue;
}

Rgba8 ParseXmlAttribute(const XmlElement& element, const char* attributeName, const Rgba8& defaultValue)
{
	Rgba8 returnValue;
	const char* attributeQuery = "";
	element.QueryStringAttribute(attributeName, &attributeQuery);
	if (attributeQuery[0] == '\0')
		returnValue = defaultValue;
	else
		returnValue.SetFromText(attributeQuery);

	return returnValue;
}

Vec2 ParseXmlAttribute(const XmlElement& element, const char* attributeName, const Vec2& defaultValue)
{
	Vec2 returnValue;
	const char* attributeQuery = "";
	element.QueryStringAttribute(attributeName, &attributeQuery);
	if (attributeQuery[0] == '\0')
		returnValue = defaultValue;
	else
		returnValue.SetFromText(attributeQuery);

	return returnValue;
}

IntVec2 ParseXmlAttribute(const XmlElement& element, const char* attributeName, const IntVec2& defaultValue)
{
	IntVec2 returnValue;
	const char* attributeQuery = "";
	element.QueryStringAttribute(attributeName, &attributeQuery);
	if (attributeQuery[0] == '\0')
		returnValue = defaultValue;
	else
		returnValue.SetFromText(attributeQuery);

	return returnValue;
}

std::string ParseXmlAttribute(const XmlElement& element, const char* attributeName, const std::string& defaultValue)
{
	std::string returnValue;
	const char* attributeQuery = "";
	element.QueryStringAttribute(attributeName, &attributeQuery);
	if (attributeQuery[0] == '\0')
		returnValue = defaultValue;
	else
		returnValue = attributeQuery;

	return returnValue;
}

Strings ParseXmlAttribute(const XmlElement& element, const char* attributeName, const Strings& defaultValue)
{
	Strings returnValue;
	const char* attributeQuery = "";
	element.QueryStringAttribute(attributeName, &attributeQuery);
	if (attributeQuery[0] == '\0')
		returnValue = defaultValue;
	else
		returnValue = SplitStringOnDelimiter(attributeQuery, ',');

	return returnValue;
}

std::string ParseXmlAttribute(const XmlElement& element, const char* attributeName, const char* defaultValue)
{
	return ParseXmlAttribute(element, attributeName, static_cast<std::string>(defaultValue));
}

Vec3 ParseXmlAttribute(const XmlElement& element, const char* attributeName, const Vec3& defaultValue)
{
	Vec3 returnValue;
	const char* attributeQuery = "";
	element.QueryStringAttribute(attributeName, &attributeQuery);
	if (attributeQuery[0] == '\0')
		returnValue = defaultValue;
	else
		returnValue.SetFromText(attributeQuery);

	return returnValue;
}

EulerAngles ParseXmlAttribute(const XmlElement& element, const char* attributeName, const EulerAngles& defaultValue)
{
	EulerAngles returnValue;
	const char* attributeQuery = "";
	element.QueryStringAttribute(attributeName, &attributeQuery);
	if (attributeQuery[0] == '\0')
		returnValue = defaultValue;
	else
		returnValue.SetFromText(attributeQuery);

	return returnValue;
}

FloatRange ParseXmlAttribute(const XmlElement& element, const char* attributeName, const FloatRange& defaultValue)
{
	FloatRange returnValue;
	const char* attributeQuery = "";
	element.QueryStringAttribute(attributeName, &attributeQuery);
	if (attributeQuery[0] == '\0')
		returnValue = defaultValue;
	else
		returnValue.SetFromText(attributeQuery);

	return returnValue;
}

IntVec3 ParseXmlAttribute(const XmlElement& element, const char* attributeName, const IntVec3& defaultValue)
{
	IntVec3 returnValue;
	const char* attributeQuery = "";
	element.QueryStringAttribute(attributeName, &attributeQuery);
	if (attributeQuery[0] == '\0')
		returnValue = defaultValue;
	else
		returnValue.SetFromText(attributeQuery);

	return returnValue;
}
