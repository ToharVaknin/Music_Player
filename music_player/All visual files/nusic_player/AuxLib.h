#ifndef AUX_LIB_H
#define AUX_LIB_H
#include<iostream>
#include<string>
#include<sstream>
#include<iostream>
#include<vector>


class AuxLib
{
public:
	static std::vector<std::string> SplitByDelim(std::string str, char delim);
	static std::string ExtractWordBetween2Delim(std::string word, char startDelim, char endDelim);
private:

};


#endif // !AUX_LIB_H


