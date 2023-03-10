#include"AuxLib.h"
//#include<string>

std::vector<std::string> AuxLib::SplitByDelim(std::string str, char delim)
{
	std::vector<std::string> myVec;
	std::istringstream line(str);
	std::string row;
	while (std::getline(line,row,delim))
	{
		myVec.push_back(row);
	}
	return myVec;
}

std::string AuxLib::ExtractWordBetween2Delim(std::string word, char startDelim, char endDelim)
{
	if (word.find(startDelim) == std::string::npos || word.find(endDelim) == std::string::npos)
	{
		return "";
	}
	size_t first = word.find(startDelim);
	size_t last = word.find(endDelim);
	return word.substr(first+1, last - first-1);
}

 