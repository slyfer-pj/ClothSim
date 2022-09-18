#pragma once
#include "ThirdParty/TinyXML2/tinyxml2.h"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Math/IntVec3.hpp"

typedef tinyxml2::XMLElement XmlElement;

int ParseXmlAttribute(const XmlElement& element, const char* attributeName, int defaultValue);
char ParseXmlAttribute(const XmlElement& element, const char* attributeName, char defaultValue);
bool ParseXmlAttribute(const XmlElement& element, const char* attributeName, bool defaultValue);
float ParseXmlAttribute(const XmlElement& element, const char* attributeName, float defaultValue);
Rgba8 ParseXmlAttribute(const XmlElement& element, const char* attributeName, const Rgba8& defaultValue);
Vec2 ParseXmlAttribute(const XmlElement& element, const char* attributeName, const Vec2& defaultValue);
Vec3 ParseXmlAttribute(const XmlElement& element, const char* attributeName, const Vec3& defaultValue);
IntVec2 ParseXmlAttribute(const XmlElement& element, const char* attributeName, const IntVec2& defaultValue);
IntVec3 ParseXmlAttribute(const XmlElement& element, const char* attributeName, const IntVec3& defaultValue);
std::string ParseXmlAttribute(const XmlElement& element, const char* attributeName, const std::string& defaultValue);
Strings ParseXmlAttribute(const XmlElement& element, const char* attributeName, const Strings& defaultValue);
std::string ParseXmlAttribute(const XmlElement& element, const char* attributeName, const char* defaultValue);
EulerAngles ParseXmlAttribute(const XmlElement& element, const char* attributeName, const EulerAngles& defaultValue);
FloatRange ParseXmlAttribute(const XmlElement& element, const char* attributeName, const FloatRange& defaultValue);