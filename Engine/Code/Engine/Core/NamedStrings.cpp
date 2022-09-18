#include "Engine/Core/NamedStrings.hpp"
#include <locale>

void NamedStrings::PopulateFromXmlElementAttributes(const XmlElement& element)
{
	const tinyxml2::XMLAttribute* attributeInElement = element.FirstAttribute();
	while (attributeInElement != nullptr)
	{
		std::string attributeName = attributeInElement->Name();
		std::string attributeValue = attributeInElement->Value();
		SetValue(attributeName, attributeValue);
		attributeInElement = attributeInElement->Next();
	}
}

void NamedStrings::SetValue(const std::string& keyName, const std::string& newValue)
{
	m_keyValuePairs[keyName] = newValue;
}

std::string NamedStrings::GetValue(const std::string& keyName, const std::string& defaultValue) const
{
	std::map<std::string, std::string>::const_iterator iter;
	iter = m_keyValuePairs.find(keyName);
	if (iter != m_keyValuePairs.end())
		return iter->second;
	else
		return defaultValue;
}

bool NamedStrings::GetValue(const std::string& keyName, bool defaultValue) const
{
	bool value = defaultValue;
	std::map<std::string, std::string>::const_iterator iter;
	iter = m_keyValuePairs.find(keyName);
	if (iter != m_keyValuePairs.end())
	{
		std::string stringValue = iter->second;
		if (stringValue[0] == 'T' || stringValue[0] == 't')
			value = true;
		else
			value = false;
	}

	return value;
}

int NamedStrings::GetValue(const std::string& keyName, int defaultValue) const
{
	int value = defaultValue;
	std::map<std::string, std::string>::const_iterator iter;
	iter = m_keyValuePairs.find(keyName);
	if (iter != m_keyValuePairs.end())
	{
		value = atoi(iter->second.c_str());
	}

	return value;
}

float NamedStrings::GetValue(const std::string& keyName, float defaultValue) const
{
	float value = defaultValue;
	std::map<std::string, std::string>::const_iterator iter;
	iter = m_keyValuePairs.find(keyName);
	if (iter != m_keyValuePairs.end())
	{
		value = static_cast<float>(atof(iter->second.c_str()));
	}

	return value;
}

std::string NamedStrings::GetValue(const std::string& keyName, const char* defaultValue) const
{
	std::string value = defaultValue;
	std::map<std::string, std::string>::const_iterator iter;
	iter = m_keyValuePairs.find(keyName);
	if (iter != m_keyValuePairs.end())
	{
		value = iter->second;
	}

	return value;
}

Rgba8 NamedStrings::GetValue(const std::string& keyName, const Rgba8& defaultValue) const
{
	Rgba8 value = defaultValue;
	std::map<std::string, std::string>::const_iterator iter;
	iter = m_keyValuePairs.find(keyName);
	if (iter != m_keyValuePairs.end())
	{
		value.SetFromText(iter->second.c_str());
	}

	return value;
}

Vec2 NamedStrings::GetValue(const std::string& keyName, const Vec2& defaultValue) const
{
	Vec2 value = defaultValue;
	std::map<std::string, std::string>::const_iterator iter;
	iter = m_keyValuePairs.find(keyName);
	if (iter != m_keyValuePairs.end())
	{
		value.SetFromText(iter->second.c_str());
	}

	return value;
}

IntVec2 NamedStrings::GetValue(const std::string& keyName, const IntVec2& defaultValue) const
{
	IntVec2 value = defaultValue;
	std::map<std::string, std::string>::const_iterator iter;
	iter = m_keyValuePairs.find(keyName);
	if (iter != m_keyValuePairs.end())
	{
		value.SetFromText(iter->second.c_str());
	}

	return value;
}

Mat44 NamedStrings::GetValue(const std::string& keyName, const Mat44& defaultValue) const
{
	Mat44 value = defaultValue;
	std::map<std::string, std::string>::const_iterator iter;
	iter = m_keyValuePairs.find(keyName);
	if (iter != m_keyValuePairs.end())
	{
		value.SetFromText(iter->second.c_str());
	}

	return value;
}

Strings NamedStrings::GetAllKeys() const
{
	Strings keys;
	std::map<std::string, std::string>::const_iterator iter;
	for (iter = m_keyValuePairs.begin(); iter != m_keyValuePairs.end(); ++iter)
	{
		keys.push_back(iter->first);
	}

	return keys;
}

