#ifndef LIBRARY_H
#define LIBRARY_H
#include<map>
#include<unordered_map>
#include<string>
#include<set>
#include"Player.h" 
class Song;
class PlayList;

class Library
{
public:
	enum libraryOps
	{
		LIB_MORE = '1',LIB_LIST = '2' , LIB_ADD = '3' , LIB_UPDATE = '4', LIB_DELETE = '5',
		LIB_PRINT_SONG = '6' , LIB_ADD2PL = '7' , LIB_REMOVE_FROM_PL = '8' , LIB_PRINT_PL = '9',
		LIB_PLAY = 'A' , LIB_PLAY_ALL = 'B', LIB_PLAY_RANDOM = 'C' , LIB_UNDELETE = 'D',LIB_HELP = 'E', LIB_BACK = '0'
	};
	Library(std::map<std::string, Song>* library = nullptr, std::map<std::string, PlayList*>* ListOfPlayLists = nullptr , std::set<std::string>* regularPlaylistNames = nullptr , int* numberOfRegularPlaylists = nullptr); 
	std::map<std::string, Song>* GetLibrary() const;
	int GetCount() const;
	void SetCount(int count);
	void Menu(); 
	friend std::ostream& operator<<(std::ostream& os, Library& rhs)  // rhs can not be const because we cange the member of this object within print function
	{
		rhs.SetCount(10);
		rhs.PrintSongsByGivenCount(rhs.GetCount());
		return os;
	}
private:
	// Private methods:
	void UpdateSongParams(std::string songName, int songId, std::vector<std::string> updateVec); 
	void PrintSongsByGivenCount(int count) const;
	bool ExtensionValidate(std::string path);
	std::string FindSongNameBySongId(int songId);
	void DeleteSong(std::string songName, int songId);
	void AddSong(std::string songPath, std::string songName);
	void PrintSongDetails(std::string songName, int songId);
	void AddSongToPlaylist(int songId, std::string playlistName);
	void RemoveSongFromPlaylist(std::string songName, std::string playlistName);
	void PrintAllPlaylist() const;
	void PlaySong(std::string songName, int songId); 
	void PlayAllSongs();
	void PlayAllSongsRandom(); 
	void Undelete(std::string songName); 
	bool CheckIfSongExist(std::string songName); 
	void Help() const; 
	char CheckUserParams(); // check the input of user and return proper token depending on the user input
	bool CheckAddParams();  
	void ClearSongParams();  
	bool CheckUpdateParams(); 
	bool checkPrintSongParams(); 
	bool CheckDeleteParams(); 
	bool CheckAdd2PlParams(); 
	bool CheckRemoveFromPlParams(); 
	bool CheckPlayParams();  
	bool CheckUndeleteParams(); 
	bool BufferHandling(std::string userInput); // For handlig the empty string that remain in std::cin buffer after getline function

	// Private data members:
	std::map<std::string, Song>* m_library;
	std::map<std::string, PlayList*>* m_ListOfPlayLists;
	std::set<std::string>* m_regularPlaylistNams;
	int* m_numberOfRegularPlaylists;
	int m_count;
	Player m_player;
	std::unordered_map<std::string, char> m_options; 
	std::vector<std::string> m_selections; 
	std::string m_singer; 
	std::string m_genre;
	std::string m_duration;
	std::string m_album;
	std::string m_songName; 
	std::string m_publishingYear;
	bool m_bufferFlag;

}; 















#endif