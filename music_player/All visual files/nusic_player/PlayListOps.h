#ifndef PLAY_LIST_OPS_H
#define PLAY_LIST_OPS_H
#include<map>
#include<unordered_map>
#include<set>
#include<vector>
#include<iostream>
class Song;
class PlayList;


class PlayListOps
{

public:
	enum PlayListActions {
		PLAY_LIST_ADD = '1', PLAY_LIST_DELETE = '2', PLAY_LIST_PLAY = '3',
		PLAY_LIST_PLAY_RANDOM = '4', PLAY_LIST_PRINT = '5',PLAY_LIST_HELP = '6', PLAY_LIST_BACK = '0'
	};
	PlayListOps(std::map<std::string, Song>* library, std::map<std::string, PlayList*>* listOfPlaylist, std::set<std::string>* regularPlayListNames, std::set<std::string>* constPlayLists,int* numberOfRegularPlaylists);
	void PrintAllExistPlayLists() const;
	void PlayListMenu();

private:
	// Private methods:
	void AddPlayList(std::string playListName);
	void DeletePlayList(std::string playListName);
	void PlayPlaylist(std::string playListName);
	void PlayPlaylistRandom(std::string playListName);
	void PrintPlaylist(std::string playListName);
	void Help() const; 
	bool BufferHandling(std::string userInput); 
	char CheckUserParams(); 
	bool CheckPlaylistParams(); 

	// Private members:
	std::map<std::string, Song>* m_library;  // key -> name of the song , value -> Song object
	std::map<std::string, PlayList*>* m_ListOfPlayLists;  // key -> folder name , value -> pointer to Folder object
	std::set<std::string>* m_regularPlayListNames;
	std::set<std::string>* m_constPlayLists;
	int* m_numberOfRegularPlaylists;
	std::unordered_map<std::string, char> m_options; 
	std::vector<std::string> m_selections; 
	bool m_bufferFlag; 
};




#endif // !PLAY_LIST_OPS_H
