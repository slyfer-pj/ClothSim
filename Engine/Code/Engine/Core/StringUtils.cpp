#include "Engine/Core/StringUtils.hpp"
#include <stdarg.h>


//-----------------------------------------------------------------------------------------------
constexpr int STRINGF_STACK_LOCAL_TEMP_LENGTH = 2048;


//-----------------------------------------------------------------------------------------------
const std::string Stringf( char const* format, ... )
{
	char textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, STRINGF_STACK_LOCAL_TEMP_LENGTH, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	return std::string( textLiteral );
}


//-----------------------------------------------------------------------------------------------
const std::string Stringf( int maxLength, char const* format, ... )
{
	char textLiteralSmall[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	char* textLiteral = textLiteralSmall;
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		textLiteral = new char[ maxLength ];

	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, maxLength, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ maxLength - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	std::string returnValue( textLiteral );
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		delete[] textLiteral;

	return returnValue;
}

Strings SplitStringOnDelimiter(const std::string& originalString, char delimiterToSplitOn)
{
	Strings splitStrings;
	int currentDelimiterIndex = -1;
	int index = -1;
	do
	{
		index = currentDelimiterIndex + 1;
		currentDelimiterIndex = (int)(originalString.find(delimiterToSplitOn, currentDelimiterIndex + 1));
		std::string subString = originalString.substr(index, currentDelimiterIndex - index);
		splitStrings.push_back(subString);
		if (currentDelimiterIndex == std::string::npos)
			break;
	} while (index < originalString.size());

	return splitStrings;
}

void RemoveLeadingAndTrailingWhiteSpaces(std::string& stringToClean)
{
	std::string copy = stringToClean;
	int firstNonSpaceCharacterPos = static_cast<int>(copy.find_first_not_of(' '));
	int lastNonSpaceCharacterPos = static_cast<int>(copy.find_last_not_of(' '));
	stringToClean = copy.substr(firstNonSpaceCharacterPos, lastNonSpaceCharacterPos - firstNonSpaceCharacterPos + 1);
}

std::string GetLongestStringAmongstStrings(Strings stringsToCheck)
{
	std::string longestString = "";
	for (int i = 0; i < stringsToCheck.size(); i++)
	{
		if (stringsToCheck[i].length() > longestString.length())
			longestString = stringsToCheck[i];
	}

	return longestString;
}


