#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H
#include<map>
#include <unordered_map>
#include<iostream>
#include<set>
#include<string>
#include"Search.h"    // Has not a pointer or referacne to this object
#include"Library.h"  // Has not a pointer or referacne to this object
#include"PlayListOps.h"
class PlayList;


class UserInterface
{
public:
	enum MainMenu {DAILY_MIX = '1', SEARCH = '2', PODCAST = '3', LIBRARY = '4', PLAY_LIST = '5',EXIT = '6',PLAY_HELP = '7' };
	enum PlayListActions {PLAY_LIST_ADD = '1', PLAY_LIST_DELETE = '2' , PLAY_LIST_PLAY = '3',
	                      PLAY_LIST_PLAY_RANDOM = '4' , PLAY_LIST_PRINT = '5' , PLAY_LIST_BACK = '0'};
	UserInterface(std::map<std::string, Song>* library = nullptr, std::map<std::string,PlayList*>* ListOfPlayLists = nullptr,std::string libraryPath = "C:\\Users\\Owner\\Desktop\\temp\\Library.txt");
	~UserInterface();
	void Play(); 
private:
	void Help() const;
	char CheckParams(std::string input); 
	bool BufferHandling(std::string userInput); // For handlig the empty string that remain in std::cin buffer after getline function
	std::string m_libraryPath;
	Search m_searchObj;
	Library m_libraryObj;
	PlayListOps m_playListOps;
	std::map<std::string, Song> m_library;  // key -> name of the song , value -> Song object
	std::map<std::string, PlayList*> m_ListOfPlayLists;  // key -> folder name , value -> pointer to Folder object
	std::set<std::string> m_constPlayLists;
	std::set<std::string> m_regularPlayListNames;
	int m_numberOfRegularPlayLists;
	std::unordered_map<std::string, char> m_options; 
	bool m_bufferFlag; 
};


#endif // !USER_INTERFACE_H

