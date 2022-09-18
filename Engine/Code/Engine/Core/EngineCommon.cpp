#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "ThirdParty/TinyXML2/tinyxml2.h"

NamedStrings g_gameConfigBlackboard;

void LoadGameConfigBlackboard()
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError status = doc.LoadFile("Data/GameConfig.xml");
	GUARANTEE_OR_DIE(status == tinyxml2::XML_SUCCESS, "Failed to load xml file");
	//doc.Parse(doc.FirstChildElement()->Name());
	tinyxml2::XMLElement* rootElement = doc.FirstChildElement();
	g_gameConfigBlackboard.PopulateFromXmlElementAttributes(*rootElement);
}
