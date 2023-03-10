#ifndef SEARCH_H
#define SEARCH_H
#include"Song.h" // Have not a pointer or referacne to this object
#include<map>
#include<unordered_map>
#include<set>
#include<iostream>
#include<list>
#include"Player.h"
#include"PlayList.h"


class Search
{
public:
	enum SearchMenue { NAME = '1', SINGER = '2', ALBUM = '3', GENRE = '4',PRINT_HISTORY = '5', PRINT_MORE_HISTORY = '6',DELETE_HISTORY = '7',HELP = '8', SEARCH_BACK_1 = '0' };
	enum playMenu { SEARCH_PLAY_SONG = '1', SEARCH_PLAY_ALL = '2', SEARCH_BACK_2 = '0' };
	enum DeleteMenu {SEARCH_DELETE_NAME = '1', SEARCH_DELETE_SINGER = '2', SEARCH_DELETE_ALBUM = '3', SEARCH_DELETE_GENRE = '4' ,SEARCH_HELP_2 = '5', SEARCH_BACK_3 = '0' };
	Search(std::map<std::string, Song>* library = nullptr,std::map<std::string,PlayList*>* listOfPlaylists = nullptr, std::string searchPath = "C:\\Users\\Owner\\Desktop\\temp\\Search.txt");
	~Search();
	friend std::ostream& operator<<(std::ostream& os, Search& rhs)
	{
		rhs.SearchMenu();
		return os;
	}

private:
	// Private methods:
	void SearchMenu(); 
	void PlaySearches(); 
	void PlayBySongName(std::string songName);
	void PlayAll();
	void SearchBySongName(std::string songName);
	void SearchBySingerName(std::string singerName);
	void SearchByAlbum(std::string albumName);
	void SearchByGenre(std::string genreName);
	void InsertToHistorySearch(std::string search);
	void PrintHistoryByGivenCount(int count);
	void DeleteHistory();
	bool ChekIfExsitInHistory(std::string search);
	void Help() const; 
	void DeleteHelp() const; 
	void PlayHelp() const; 
	char CheckUserParamsSearchMenue(); 
	char CheckUserParamsDeleteMenu(); 
	char CheckUserParamsPlayMenu(); 
	bool CheckSearchParams(); 
	bool CheckDeleteParams(); 
	bool BufferHandling(std::string userInput); 

	// Private members:
	std::map<std::string, Song>* m_library;
	std::map<std::string, PlayList*>* m_listOfPlaylists;
	std::set<std::string> m_search;
	std::list<std::string> m_history;
	std::string m_searchPath;
	Player m_player;
	int m_count;
	bool m_bufferFlag;
	std::unordered_map<std::string, char> m_searchOptions; 
	std::unordered_map<std::string, char> m_deleteOptions; 
	std::unordered_map<std::string, char> m_playOptions; 
	std::vector<std::string> m_selections; 

};

#endif // !SEARCH_H

