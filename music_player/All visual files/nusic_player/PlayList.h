#ifndef PLAYLIST_H
#define PLAYLIST_H
#include<iostream>
#include<list>
#include<map>
#include<unordered_map>
#include<set>
#include"Player.h"  // Have not a pointer or referacne to this object
#include<time.h>
#include"Song.h" 

class PlayList		
{
public:
	virtual ~PlayList() {};
	static char PlayMidMenu();
	virtual void AddSong(std::string songName, Song songObj) = 0;
	virtual void DeleteSongBySongName(std::string songName) = 0;
	virtual void DeleteAllSongs() = 0;
	virtual void Menu() = 0;
	virtual void Play() = 0;
	virtual void PlayRandom() = 0;
	virtual void Print() = 0;
	virtual void UpdateSongName(std::string songName, std::string UpdateName) = 0;
	virtual bool CheckIfSongExist(std::string songName) = 0;
	virtual std::string GetPlaylistPath() const = 0;
	virtual Song GetSongObjectBySongName(std::string songName) = 0;
	 friend std::ostream& operator<<(std::ostream& os, PlayList& rhs)
	 {
		 rhs.Print();  
		 return os;
	 }
protected:
	Player m_player;
};

class Podcasts : public PlayList
{ 
public:
	Podcasts(std::map<std::string, Song>* library = nullptr, std::map<std::string, PlayList*>* listOfPalyLists = nullptr ,std::string podcastPath = "C:\\Users\\Owner\\Desktop\\temp\\Podcasts.txt");
	~Podcasts();
	enum PodcastActions { PODCAST_PLAY = '1', PODCAST_DELETE = '2', PODCAST_HELP = '3', PODCAST_BACK = '0' };
	void Menu(); 
	void AddSong(std::string podcastName, Song songObj);
	void DeleteSongBySongName(std::string podcastName);
	void DeleteAllSongs();
	void UpdateSongName(std::string podcastName, std::string UpdateName);
	bool CheckIfSongExist(std::string podcastName);
	void Play();
	void PlayRandom();
	void Print();
	std::string GetPlaylistPath() const;
	Song GetSongObjectBySongName(std::string podcastName) { return Song("", ""); };
	friend std::ostream& operator<<(std::ostream& os, Podcasts& rhs)
	{
		rhs.Print();  
		return os;
	}
private:
	// Private methodas:
	void PlaySpecificPodcast(std::string podcastName);
	bool BufferHandling(std::string userInput); // For handlig the empty string that remain in std::cin buffer after getline function
	void Help() const;
	char CheckUserParams(); 
	bool CheckPlayParams(); 
	bool CheckDeleteParams(); 

	// Private members:
	std::set<std::string> m_podcasts;
	std::map<std::string, Song>* m_library;
	std::map<std::string, PlayList*>* m_listOfPalyLists;
	std::string m_podcastsPath;
	std::unordered_map<std::string, char> m_options; 
	std::vector<std::string> m_selections; 
	bool m_bufferFlag; 
};

class Favorite : public PlayList
{
public:
	Favorite(std::map<std::string, Song>* library = nullptr, std::map<std::string, PlayList*>* listOfPalyLists = nullptr ,std::string favoritePath = "C:\\Users\\Owner\\Desktop\\temp\\Favorites.txt");
	~Favorite();
	void AddSong(std::string songName, Song songObj);
	void DeleteSongBySongName(std::string songName);
	void DeleteAllSongs();
	void UpdateSongName(std::string songName, std::string UpdateName);
	bool CheckIfSongExist(std::string songName);
	void Play();
	void Menu() {};
	void PlayRandom();
	void Print();
	std::string GetPlaylistPath() const;
	Song GetSongObjectBySongName(std::string songName) { return Song("", ""); };
	friend std::ostream& operator<<(std::ostream& os,Favorite& rhs)
	{
		rhs.Print();
		return os;
	}

private:
	std::map<std::string, Song>* m_library;
	std::map<std::string, PlayList*>* m_listOfPalyLists;
	std::set<std::string> m_favorite;
	std::string m_favoritePath;
};


class DailyMix : public PlayList
{
public:
	DailyMix(std::map<std::string, Song>* library = nullptr,std::map<std::string,PlayList*>* listOfPalyLists = nullptr ,std::string dailyMixPath = "C:\\Users\\Owner\\Desktop\\temp\\DailyMix.txt");
	~DailyMix();
	enum DailyMixActions { DAILY_PLAY = '1', DAILY_PLAY_RANDOM = '2',DAILY_HELP = '3', DAILY_BACK = '0' };
	void AddSong(std::string songName, Song songObj) {}; 
	void DeleteSongBySongName(std::string songName); 
	void DeleteAllSongs() {};
	void UpdateSongName(std::string songName, std::string UpdateName);
	bool CheckIfSongExist(std::string songName);
	void Menu();
	void Play();
	void Print();
	void PlayRandom();
	std::string GetPlaylistPath() const;
	Song GetSongObjectBySongName(std::string songName)  { return Song("", ""); };
	friend std::ostream& operator<<(std::ostream& os, DailyMix& rhs) // rhs can't be const because print function call to non const functions
	{
		rhs.Print(); 
		return os;
	}
private:
	// Private methods:
	bool CheckIfDayChanged(); // if true --> save the current date and return true , false --> return false
	void SelectRandomSongs();
	void Help() const; 
	char CheckUserParams(std::string input); 
	bool BufferHandling(std::string userInput);  

	// Private members:
	std::string m_dailyMixPath;
	std::set<std::string> m_dailyMix;
	std::map<std::string, Song>* m_library;
	std::map<std::string, PlayList*>* m_listOfPalyLists;
	std::unordered_map<std::string, char> m_options; 
	bool m_bufferFlag; 
	int m_day;
	int m_month;
	int m_year;
};

class MostPlayed : public PlayList
{
public:
	MostPlayed(std::map<std::string, Song>* library = nullptr , std::map<std::string, PlayList*>* listOfPalyLists = nullptr , std::string mostPlayedPath = "C:\\Users\\Owner\\Desktop\\temp\\MostPlayed.txt");
	~MostPlayed();
	void AddSong(std::string songName, Song songObj) {};  // We can not to add song to MostPlayed list
	void DeleteSongBySongName(std::string songName);
	void DeleteAllSongs() {};  // We can not delete all the songs of MostPlayed list
	void UpdateSongName(std::string songName, std::string UpdateName); 
	bool CheckIfSongExist(std::string songName);
	void Play();
	void Menu() {};
	void Print(); 
	void PlayRandom();
	std::string GetPlaylistPath() const;
	Song GetSongObjectBySongName(std::string songName)  { return Song("", ""); }
	friend std::ostream& operator<<(std::ostream& os, MostPlayed& rhs)
	{
		rhs.Print();
		return os;
	}

private:
	// Private methods:
	void UpdateMostPlayed();

	// Private members
	std::map<std::string, Song>* m_library;
	std::map<std::string, PlayList*>* m_listOfPalyLists;
	std::multimap<int,std::string, std::greater<int>> m_mostPlayed;
	std::string m_mostPlayedPath;

};

class Recent : public PlayList
{
public:
	Recent(std::map<std::string, Song>* library = nullptr, std::map<std::string, PlayList*>* listOfPalyLists = nullptr,std::string recentPath = "C:\\Users\\Owner\\Desktop\\temp\\Recent.txt");
	~Recent();
	void AddSong(std::string songName, Song songObj);
	void DeleteSongBySongName(std::string songName);
	void DeleteAllSongs() {};
	void UpdateSongName(std::string songName, std::string UpdateName);
	bool CheckIfSongExist(std::string songName);
	void Play();
	void Menu() {};
	void Print();
	void PlayRandom();
	std::string GetPlaylistPath() const;
	Song GetSongObjectBySongName(std::string songName)  { return Song("", ""); };
	friend std::ostream& operator<<(std::ostream& os,Recent& rhs)
	{
		rhs.Print();
		return os;
	}

private:
	std::list<std::string> m_recentList;
	std::map<std::string, Song>* m_library;
	std::map<std::string, PlayList*>* m_listOfPalyLists;
	std::string m_recentPath;
};

class Deleted : public PlayList
{
public:
	Deleted(std::map<std::string, Song>* library = nullptr, std::string deletedPath = "C:\\Users\\Owner\\Desktop\\temp\\Deleted.txt");
	~Deleted();
	void Play() {};
	void Menu() {};
	void Print();
	void PlayRandom() {};
	std::string GetPlaylistPath() const;
	Song GetSongObjectBySongName(std::string songName);
	friend std::ostream& operator<<(std::ostream& os, Deleted& rhs)
	{
		rhs.Print();
		return os;
	}
	void AddSong(std::string songName, Song songObj);
	void DeleteSongBySongName(std::string songName);
	void DeleteAllSongs();
	void UpdateSongName(std::string songName, std::string UpdateName) {};  
	bool CheckIfSongExist(std::string songName);


private:
	std::map<std::string, Song>* m_library; 
	std::map<std::string, Song> m_deleted;
	std::string m_deletedPath;
};

class RegularPlayList : public PlayList
{
public:
	RegularPlayList(std::map<std::string, Song>* library = nullptr , std::map<std::string,PlayList*>* listOfPalyLists = nullptr , std::string regularPlayListName = "", std::string regularPlayListPath = "C:\\Users\\Owner\\Desktop\\temp\\");
	~RegularPlayList();
	void AddSong(std::string songName, Song songObj);
	void DeleteSongBySongName(std::string songName);
	void DeleteAllSongs();
	void UpdateSongName(std::string songName, std::string UpdateName);
	bool CheckIfSongExist(std::string songName);
	void Play();
	void Menu() {};
	void Print();
	void PlayRandom();
	std::string GetPlaylistPath() const;
	Song GetSongObjectBySongName(std::string songName)  { return Song("", ""); };
	friend std::ostream& operator<<(std::ostream& os, RegularPlayList& rhs)
	{
		rhs.Print();
		return os;
	}

private:
	std::map<std::string, Song>* m_library;
	std::map<std::string, PlayList*>* m_listOfPalyLists;
	std::set<std::string> m_regularPlayList;
	std::string m_regularPlayListPath;
	std::string m_regularPlayListName;
};






#endif // PLAYLIST_H

