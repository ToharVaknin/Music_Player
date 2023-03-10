#include"PlayList.h"
#include<fstream>
#include"Song.h"
#include"AuxLib.h"
#include<iomanip>
#include <filesystem>
#include<time.h>
#include <iterator>
#include <cstdlib>
#include<vector>
#include<algorithm>
namespace fs = std::filesystem;

// After each song that play the user need to decide if to continue playing the list or stop the playing of the list
char PlayList::PlayMidMenu()  // Static method
{
	std::string userInput;
	std::cout << "\n";
	while (true)
	{
		std::cout << "Please select one of the following options : " << "\n";
		std::cout << "1. For play the next song write: " << "\n";
		std::cout << "   Next" << "\n\n";
		std::cout << "2. For stop palying write: " << "\n";
		std::cout << "   Stop" << "\n\n";
		std::cin >> userInput;
		if (userInput == "Next")
		{
			return '1';
		}
		else if(userInput == "Stop")
		{
			return '2';
		}
		else
		{
			std::cout << "Invalid input!" << "\n\n";
		}
	}
}

Favorite::Favorite(std::map<std::string, Song>* library, std::map<std::string, PlayList*>* listOfPalyLists, std::string favoritePath)
{
	m_library = library;
	m_listOfPalyLists = listOfPalyLists;
	m_favoritePath = favoritePath;
	std::ifstream file(m_favoritePath);
	if (!file)
	{
		std::ofstream file(m_favoritePath);  // If the file not exist , open new file
	}
	// Deserialization process
	if (!fs::is_empty(m_favoritePath))
	{
		std::string songName;
		while (!file.eof())
		{
			file >> songName;
			if (songName == "@@@")   // To prevent the last line of the file from being read twice
			{
				break;
			}
			m_favorite.insert(songName);
		}
	}
	file.close();
}

Favorite::~Favorite()
{
	std::ofstream file(m_favoritePath);
	if (!file)
	{
		std::cout << m_favoritePath << " not found!" << std::endl;
	}
	else  // Serialization process
	{
		for (auto itr = m_favorite.begin(); itr != m_favorite.end(); ++itr)
		{
			file << *itr << std::endl;
		}
		file << "@@@";   // To prevent the last line of the file from being read twice
	}
	file.close();
}

std::string Favorite::GetPlaylistPath() const
{
	return m_favoritePath;
}

void Favorite::AddSong(std::string songName, Song songObj)
{
	if (m_favorite.find(songName) == m_favorite.end())
	{
		m_favorite.insert(songName);
	}
	else
	{
		std::cout << "The song " << songName << " already exists on the Favorite list" << std::endl;
	}
} 

void Favorite::DeleteSongBySongName(std::string songName)
{
	if (m_favorite.find(songName) != m_favorite.end())
	{
		m_favorite.erase(songName);
	}
}

void Favorite::DeleteAllSongs()
{
	if (!m_favorite.empty())
	{
		m_favorite.clear();
		std::cout << "All the songs in Favorite list erased" << std::endl;
	}
	else
	{
		std::cout << "Favorite list is empty" << std::endl;
	}
}

void Favorite::UpdateSongName(std::string songName, std::string UpdateName)
{
	if (m_favorite.find(songName) != m_favorite.end()) // If this song name exist in podcast we update his name
	{
		m_favorite.erase(songName);
		m_favorite.insert(UpdateName);
	}
}

bool Favorite::CheckIfSongExist(std::string songName)
{
	if (m_favorite.find(songName) == m_favorite.end())
	{
		return false;
	}
	return true;
}

void Favorite::Play()
{
	if (!m_favorite.empty())
	{
		for (auto itr = m_favorite.begin(); itr != m_favorite.end(); ++itr)
		{
			m_player.play(m_library->at(*itr).GetSongPath(),true);
			m_library->at(*itr).IncreaseNumberOfPlays();  // Increase the number of plays of this song
			m_listOfPalyLists->at("Recent")->AddSong(*itr, Song("", ""));
			std::cout << "End to play " << *itr << std::endl;
			if (PlayMidMenu() == '2')
			{
				break;
			}
		}
	}
	else
	{
		std::cout << "The Favorite list is empty" << std::endl;
	}
}

void Favorite::PlayRandom()
{
	if (!m_favorite.empty())
	{
		int i;
		char ch;
		bool startFlag = true;
		int count = m_favorite.size();
		std::vector<std::string> tempVec;
		std::string songName;
		for (auto itr = m_favorite.begin(); itr != m_favorite.end(); ++itr)
		{
			tempVec.push_back(*itr);
		}
		while (count)
		{
			if (startFlag != true )  // We call to playMenu only after the first song played
			{
				ch = PlayMidMenu();
				if (ch == '2')
				{
					break;
				}
			}
			startFlag = false;
			i = rand() % tempVec.size();
			songName = tempVec.at(i);
			m_player.play(m_library->at(songName).GetSongPath(), true);
			m_library->at(songName).IncreaseNumberOfPlays();  // Increase the number of plays of this song
			m_listOfPalyLists->at("Recent")->AddSong(songName, Song("", "")); // Add this podcast to recent playlist
			tempVec.erase(tempVec.begin() + i);
			count--;
			std::cout << "End to play " << songName << std::endl;
		}
	}
	else
	{
		std::cout << "The Favorite list is empty" << std::endl;
	}
}

void Favorite::Print() 
{
	std::cout << "------------------------------ Favorites ------------------------------" << std::endl;
	if (!m_favorite.empty())
	{
		for (auto itr = m_favorite.begin(); itr != m_favorite.end(); ++itr)
		{
			std::cout << "  Song name:" << std::setw(40) << *itr << std::setw(15) << "song id:" << std::setw(5) << m_library->at(*itr).GetSongId() << std::endl;
		}
	}
	else
	{
		std::cout << "The Favorite list is empty" << std::endl;
	}
}

Podcasts::Podcasts(std::map<std::string, Song>* library , std::map<std::string, PlayList*>* listOfPalyLists, std::string podcastPath)
{
	// Initialization of user options in podcast menu
	m_options.insert(std::make_pair("Play", '1'));
	m_options.insert(std::make_pair("Delete", '2'));
	m_options.insert(std::make_pair("Help", '3'));
	m_options.insert(std::make_pair("Back", '0'));

	// Initialization of podcast objects
	m_bufferFlag = false;
	m_podcastsPath = podcastPath;
	m_listOfPalyLists = listOfPalyLists;
	m_library = library;
	std::ifstream file(m_podcastsPath);
	if (!file)
	{
		std::ofstream file(m_podcastsPath);
	}
	// Deserialization process
	if (!fs::is_empty(m_podcastsPath))
	{
		if (!m_podcasts.empty())
		{
			m_podcasts.clear();
		}
		std::string podcastName;
		while (!file.eof())
		{
			file >> podcastName;
			if (podcastName == "@@@")  // To prevent the last line of the file from being read twice
			{
				break;
			}
			m_podcasts.insert(podcastName);
		}
	}
	file.close();
}

Podcasts::~Podcasts()
{
	std::ofstream file(m_podcastsPath);
	if (!file)
	{
		std::cout << m_podcastsPath << " not found!" << std::endl;												
	}
	else // Serialization process
	{
		for (auto itr = m_podcasts.begin(); itr != m_podcasts.end(); ++itr)
		{
			file << *itr << std::endl;
		}
		file << "@@@";   // To prevent the last line of the file from being read twice
	}
	file.close();
}

bool Podcasts::BufferHandling(std::string userInput) 
{ // In order to skip one itaration when cin buffer has empty string
	if (userInput == "")
	{
		m_bufferFlag = true;
		return true;
	}
	m_bufferFlag = false;
	return false;
}

char Podcasts::CheckUserParams()
{
	if (m_selections.empty())
	{
		return 'x';
	}
	// Check if the first word of the command that the user entered exist in the options
	if (m_options.find(m_selections.at(0)) == m_options.end())
	{ // Not exist
		return 'x';
	}
	else  // Exist, then we return the proper token of this command
	{
		return m_options.at(m_selections.at(0));
	}
}

bool Podcasts::CheckPlayParams()
{
	if (m_selections.size() == 2)
	{
		return true;
	}
	// Example to valid input
	std::cout << "Invalid input params for Play command!" << "\n";
	std::cout << "You have to enter 2 params:" << "\n";
	std::cout << "Example: Play <podcast_name> " << "\n\n";
	return false;
}

bool Podcasts::CheckDeleteParams() 
{
	if (m_selections.size() == 2)
	{
		return true;
	} 
	// Example to valid input
	std::cout << "Invalid input params for Delete command!" << "\n";
	std::cout << "You have to enter 2 params:" << "\n";
	std::cout << "Example: Delete <podcast_name> " << "\n\n";
	return false;
}

// Podcast menu
void Podcasts::Menu()
{
	std::string userInput;
	std::string input;
	char select;
	while (true)
	{
		if (m_bufferFlag == false)
			Help();
		std::getline(std::cin, userInput);
		if (BufferHandling(userInput))
			continue;
		m_selections = AuxLib::SplitByDelim(userInput, ' ');  // Split the input string by spaces.
		select = CheckUserParams(); // Check if the input of the user is valid - chaeck the first word - token
		switch (select)
		{
		case PODCAST_PLAY:
			if (CheckPlayParams())
			{
				input = AuxLib::ExtractWordBetween2Delim(m_selections.at(1), '<', '>'); // Podcast name
				if (input != "")
				{
					PlaySpecificPodcast(input);
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case PODCAST_DELETE:
			if (CheckDeleteParams())
			{
				input = AuxLib::ExtractWordBetween2Delim(m_selections.at(1), '<', '>'); // Podcast name
				if (input != "")
				{
					if (m_podcasts.find(input) != m_podcasts.end())
					{
						DeleteSongBySongName(input);
						std::cout << "The podcast named: " << input << " erased from podcasts list" << "\n\n";
					}
					else
					{
						std::cout << "The podcast named: " << input << " not found on the podcasts list" << "\n\n";
					}
					
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case PODCAST_HELP:
			Help();
			m_bufferFlag = true;  // For help Won't show up twice
			break;
		case PODCAST_BACK:
			return;
		default:
			std::cout << "Illegal action!" << std::endl;
			std::cout << std::endl;
			break;
		}
	}
}

void Podcasts::Help() const
{
	std::cout << "--------------- Podcasts Menu ---------------" << "\n\n";
	std::cout << "1. For delete podcast by podcast name write: " << "\n";
	std::cout << "   Delete <podcast_name>" << "\n";
	std::cout << "** You must not include a space in the podcast name **" << "\n\n";
	std::cout << "2. For play podcast by name write: " << "\n";
	std::cout << "   Play <podcast_name>" << "\n";
	std::cout << "** You must not include a space in the podcast name **" << "\n\n";
	std::cout << "3. For help write: " << "\n";
	std::cout << "   Help" << "\n\n";
	std::cout << "4. For back to main menu write: " << "\n";
	std::cout << "   Back" << "\n\n";
	std::cout << "------------------------------------------------------" << "\n";

}

std::string Podcasts::GetPlaylistPath() const
{
	return m_podcastsPath;
}

void Podcasts::AddSong(std::string podcastName, Song songObj)
{ // Check if the song already exist in podcasts list
	if (m_podcasts.find(podcastName) == m_podcasts.end())
	{
		m_podcasts.insert(podcastName);
	}
	else
	{
		std::cout << "The song called " << podcastName << "is already exist on the podcasts list" << std::endl;
	}
}

void Podcasts::DeleteSongBySongName(std::string podcastName)
{
	if (m_podcasts.find(podcastName) != m_podcasts.end())
	{
		m_podcasts.erase(podcastName);
	}
}

void Podcasts::DeleteAllSongs()
{
	if (!m_podcasts.empty())
	{
		m_podcasts.clear();
		std::cout << "All the songs in Podcast list erased" << std::endl;
	}
	else
	{
		std::cout << "The podcast list is empty" << std::endl;
	}
}

void Podcasts::UpdateSongName(std::string podcastName, std::string UpdateName)
{
	if (m_podcasts.find(podcastName) != m_podcasts.end()) // If this song name exist in podcast we update his name
	{
		m_podcasts.erase(podcastName);
		m_podcasts.insert(UpdateName);
	}
}

void Podcasts::PlaySpecificPodcast(std::string podcastName)
{
	if (m_podcasts.find(podcastName) != m_podcasts.end())
	{
		m_player.play(m_library->at(podcastName).GetSongPath(), true);
		m_library->at(podcastName).IncreaseNumberOfPlays();  // Increase the number of plays of this song
		m_listOfPalyLists->at("Recent")->AddSong(podcastName, Song("", "")); // Add this podcast to recent playlist
		std::cout << "End to play " << podcastName << std::endl;
	}
	else
	{
		std::cout << "The podcast called " << podcastName << " not found on the podcasts list" << std::endl;
		std::cout << std::endl;
	}
}

bool Podcasts::CheckIfSongExist(std::string podcastName)
{
	if (m_podcasts.find(podcastName) == m_podcasts.end())
	{
		return false;
	}
	return true;
}


void Podcasts::Play()  // Play all the podcasts alphabetically
{
	if (!m_podcasts.empty())
	{
		bool startFlag = true;
		char ch;
		for (auto itr = m_podcasts.begin(); itr != m_podcasts.end(); ++itr)
		{
			if (startFlag != true)
			{
				ch = PlayMidMenu();
				if (ch == '2')
				{
					break;
				}
			}
			startFlag = false;
			m_player.play(m_library->at(*itr).GetSongPath(),true);
			m_library->at(*itr).IncreaseNumberOfPlays();  // Increase the number of plays of this song
			m_listOfPalyLists->at("Recent")->AddSong(*itr, Song("", "")); // Add this podcast to recent playlist
			std::cout << "End to play " << *itr << std::endl;
		}
	}
	else
	{
		std::cout << "The podcasts list is empty" << std::endl;
		std::cout << std::endl;
	}
}

void Podcasts::PlayRandom() // play all the podcasts randomly
{
	if (!m_podcasts.empty())
	{
		int i;
		char ch;
		bool startFlag = true;
		int count = m_podcasts.size();
		std::vector<std::string> tempVec;
		std::string podcastName;
		for (auto itr = m_podcasts.begin(); itr != m_podcasts.end(); ++itr)
		{
			tempVec.push_back(*itr);
		}		
		while (count)
		{
			if (startFlag != true) // We call to PlayMenu only after the first song played
			{
				ch = PlayMidMenu();
				if (ch == '2')
				{
					break;
				}
			}
			startFlag = false;
			i = rand() % tempVec.size();
			podcastName = tempVec.at(i);
			m_player.play(m_library->at(podcastName).GetSongPath(), true);
			m_library->at(podcastName).IncreaseNumberOfPlays();  // Increase the number of plays of this song
			m_listOfPalyLists->at("Recent")->AddSong(podcastName, Song("", "")); // Add this podcast to recent playlist
			tempVec.erase(tempVec.begin() + i);
			count--;
			std::cout << "End to play " << podcastName << std::endl;
		}
	}
	else
	{
		std::cout << "The podcasts list is empty" << std::endl;
		std::cout << std::endl;
	}
}

void Podcasts::Print() 
{
	std::cout << "------------------------------ Podcasts ------------------------------" << std::endl;
	if (!m_podcasts.empty())
	{
		for (auto itr = m_podcasts.begin(); itr != m_podcasts.end(); ++itr)
		{
			std::cout << "  Song name:" << std::setw(40) << *itr << std::setw(15) << "song id:" << std::setw(5) << m_library->at(*itr).GetSongId() << std::endl;
		}
	}
	else
	{
		std::cout << "The podcasts list is empty" << std::endl;
	}
	std::cout << std::endl;
}

MostPlayed::MostPlayed(std::map<std::string, Song>* library, std::map<std::string, PlayList*>* listOfPalyLists, std::string mostPlayedPath)
{
	m_library = library;
	m_listOfPalyLists = listOfPalyLists;
	m_mostPlayedPath = mostPlayedPath;
	std::ifstream file(m_mostPlayedPath);
	if (!file)
	{
		std::ofstream file(m_mostPlayedPath);  // If the file not exist , open new file
	}
	// Deserialization process
	if (!fs::is_empty(m_mostPlayedPath))
	{
		if (!m_mostPlayed.empty())
		{
			m_mostPlayed.clear();
		}
		std::string songName;
		int numberOfPlays;
		while (!file.eof())
		{
			file >> songName >> numberOfPlays;
			if (songName == "@@@")
			{
				break;
			}
			m_mostPlayed.insert(std::make_pair(numberOfPlays, songName));
		}
	}
}

MostPlayed::~MostPlayed()
{
	UpdateMostPlayed();
	std::ofstream file(m_mostPlayedPath);
	if (!file)
	{
		std::cout << m_mostPlayedPath << " not found!" << std::endl;
	}
	else // Serialization process
	{
		for (auto itr = m_mostPlayed.begin(); itr != m_mostPlayed.end(); ++itr)
		{
			file << itr->second << " " << itr->first << std::endl;
		}
		file << "@@@";
	}
	file.close();
}

std::string MostPlayed::GetPlaylistPath() const
{
	return m_mostPlayedPath;
}

// Any time we need to update the most played list because the number of song playing always changed!
void MostPlayed::UpdateMostPlayed()
{
	m_mostPlayed.clear();
	for (auto itr = m_library->begin(); itr != m_library->end(); ++itr)
	{
		m_mostPlayed.insert(std::make_pair(itr->second.GetNumberOfPlays(), itr->first));
	}
}

void MostPlayed::UpdateSongName(std::string songName, std::string UpdateName)
{
	for (auto itr = m_mostPlayed.begin(); itr != m_mostPlayed.end(); ++itr)
	{
		if (itr->second == songName)
		{
			itr->second = UpdateName;   // Update the name of the song
			return;
		}
	}
}

void MostPlayed::DeleteSongBySongName(std::string songName)
{
	for (auto itr = m_mostPlayed.begin(); itr != m_mostPlayed.end(); ++itr)
	{
		if (itr->second == songName)
		{
			m_mostPlayed.erase(itr);  // Delete by iterator
			return;
		}
	}
}

bool MostPlayed::CheckIfSongExist(std::string songName)
{
	UpdateMostPlayed();
	int count;
	(m_mostPlayed.size() < 10) ? count = m_mostPlayed.size() : count = 10;
	auto itr = m_mostPlayed.begin();
	while (count)
	{
		if (itr->second == songName)
		{
			return true;
		}
		itr++;
		count--;
	}
	return false;
}

void MostPlayed::Play()
{
	UpdateMostPlayed();
	if (!m_mostPlayed.empty())
	{
		int count;
		auto itr = m_mostPlayed.begin();
		bool startFlag = true;
		char ch;
		(m_mostPlayed.size() < 10) ? count = m_mostPlayed.size() : count = 10;
		while (count)
		{
			if (startFlag != true)
			{
				ch = PlayMidMenu();
				if (ch == '2')
				{
					break;
				}
			}
			startFlag = false;
			m_player.play(m_library->at(itr->second).GetSongPath(), true);
			m_library->at(itr->second).IncreaseNumberOfPlays();  // Increase the number of plays of this song
			m_listOfPalyLists->at("Recent")->AddSong(itr->second, Song("", "")); // Add this podcast to recent playlist
			std::cout << "End to play " << itr->first << std::endl;
			count--;
			itr++;
		}
	}
	else
	{
		std::cout << "The Most Played list is empty" << std::endl;
	}
}

void MostPlayed::Print()
{
	std::cout << "------------------------------ MostPlayed ------------------------------" << std::endl;
	UpdateMostPlayed();
	int count;
	(m_mostPlayed.size() < 10) ? count = m_mostPlayed.size() : count = 10;
	auto itr = m_mostPlayed.begin();
	while (count)
	{
		std::cout << "  Song name:" << std::setw(40) << itr->second << std::setw(15) << "song id:" << std::setw(5) << m_library->at(itr->second).GetSongId() << std::endl;
		itr++;
		count--;
	}
}

// Playin of most played list randomly
void MostPlayed::PlayRandom()
{
	UpdateMostPlayed();
	if (!m_mostPlayed.empty())
	{
		int i;
		bool startFlag = true;
		char ch;
		int count;
		std::vector<std::string> tempVec;
		std::string songName;
		auto itr = m_mostPlayed.begin();
		(m_mostPlayed.size() < 10) ? count = m_mostPlayed.size() : count = 10;
		while (count)
		{
			tempVec.push_back(itr->second);
			itr++;
			count--;
		}
		(m_mostPlayed.size() < 10) ? count = m_mostPlayed.size() : count = 10;
		while (count)
		{
			if (startFlag != true)
			{
				ch = PlayMidMenu();
				if (ch == '2')
				{
					break;
				}
			}
			startFlag = false;
			i = rand() % tempVec.size();
			songName = tempVec.at(i);
			m_player.play(m_library->at(songName).GetSongPath(), true);
			m_library->at(songName).IncreaseNumberOfPlays();  // Increase the number of plays of this song
			m_listOfPalyLists->at("Recent")->AddSong(songName, Song("", "")); // Add this podcast to recent playlist
			tempVec.erase(tempVec.begin() + i);
			count--;
			std::cout << "End to play " << songName << std::endl;
		}
	}
	else
	{
		std::cout << "The Most Played list is empty" << std::endl;
	}
}

Recent::Recent(std::map<std::string, Song>* library, std::map<std::string, PlayList*>* listOfPalyLists ,std::string recentPath)
{
	m_library = library;
	m_recentPath = recentPath; 
	m_listOfPalyLists = listOfPalyLists;
	std::ifstream file(m_recentPath);
	if (!file)
	{
		std::ofstream file(m_recentPath);  // If the file not exist , open new file
	}
	// Deserialization process
	if (!fs::is_empty(m_recentPath))
	{
		if (!m_recentList.empty())
		{
			m_recentList.clear();
		}
		std::string songName;
		while (!file.eof())
		{
			file >> songName;
			if (songName == "@@@")
			{
				break;
			}
			m_recentList.push_back(songName);         
		}
	}
	file.close();
}

Recent::~Recent()
{
	std::ofstream file(m_recentPath);
	if (!file)
	{
		std::cout << m_recentPath << " not found!" << std::endl;
	}
	else // Serialization process
	{
		for (auto itr = m_recentList.begin(); itr != m_recentList.end(); ++itr)
		{
			file << *itr << std::endl;
		}
		file << "@@@";
	}
	file.close();
}

std::string Recent::GetPlaylistPath() const
{
	return m_recentPath;
}

void Recent::AddSong(std::string songName, Song songObj)
{
	if (std::find(m_recentList.begin(),m_recentList.end(), songName) == m_recentList.end())
	{  // This song name not exist in the list
		m_recentList.push_front(songName); // Add song name to head of the list    
	}
	else // This song name exist in the list so we move it to head of the list
	{
		m_recentList.remove(songName);                                             
		m_recentList.push_front(songName); // Moving the song to head of the list     
	}
}

void Recent::Print()
{
	std::cout << "------------------------------ Recents ------------------------------" << std::endl;
	if (!m_recentList.empty())
	{
		int count;
		auto itr = m_recentList.begin();
		(m_recentList.size() < 10) ? count = m_recentList.size() : count = 10;
		while (count)
		{
			std::cout << "  Song name:" << std::setw(40) << *itr << std::setw(15) << "song id:" << std::setw(5) << m_library->at(*itr).GetSongId() << std::endl;
			count--;
			itr++;
		}
	}
	else
	{
		std::cout << "The recent list is empty" << std::endl;
	}
}

// Playing of all the songs in recent list
void Recent::Play()
{
	if (!m_recentList.empty())
	{
		bool startFlag = true;
		char ch;
		int count;
		auto itr = m_recentList.begin();
		(m_recentList.size() < 10) ? count = m_recentList.size() : count = 10;
		while (count)
		{
			if (startFlag != true)
			{
				ch = PlayMidMenu();
				if (ch == '2')
				{
					break;
				}
			}
			startFlag = false;
			m_player.play(m_library->at(*itr).GetSongPath(), true);
			m_library->at(*itr).IncreaseNumberOfPlays();
			std::cout << "End to play " << *itr << std::endl;
			count--;
			itr++;
		}
	}
	else
	{
		std::cout << "Recent list is empty" << std::endl;
	}
}

// Playing of all the songs in recent list randomly
void Recent::PlayRandom()     
{
	if (!m_recentList.empty())
	{
		int i;
		bool startFlag = true;
		char ch;
		int count;
		std::vector<std::string> tempVec;
		std::string songName;
		auto itr = m_recentList.begin();
		(m_recentList.size() < 10) ? count = m_recentList.size() : count = 10;
		while (count)
		{
			tempVec.push_back(*itr);
			itr++;
			count--;
		}
		(m_recentList.size() < 10) ? count = m_recentList.size() : count = 10;
		while (count)
		{
			if (startFlag != true)
			{
				ch = PlayMidMenu();
				if (ch == '2')
				{
					break;
				}
			}
			startFlag = false;
			i = rand() % tempVec.size();
			songName = tempVec.at(i);
			m_player.play(m_library->at(songName).GetSongPath(), true);
			m_library->at(songName).IncreaseNumberOfPlays();
			tempVec.erase(tempVec.begin() + i);
			count--;
			std::cout << "End to play " << songName << std::endl;
		}
	}
	else
	{
		std::cout << "Recent list is empty" << std::endl;
	}
}

void Recent::DeleteSongBySongName(std::string songName)
{
	if (std::find(m_recentList.begin(), m_recentList.end(), songName) != m_recentList.end())
	{
		m_recentList.remove(songName);              
	}
}

void Recent::UpdateSongName(std::string songName, std::string UpdateName)
{ // In order to save the position of the song that we want update his name, we go through the whole list and find the song name we want to update , if we find it we update it and save it position in the recent list.
	for (auto itr = m_recentList.begin(); itr != m_recentList.end(); ++itr)
	{
		if (*itr == songName)
		{
			*itr = UpdateName;                 
			break;
		}
	}
}

bool Recent::CheckIfSongExist(std::string songName)  // Check if the song exist in top 10 recent list
{
	int count = 10;
	(m_recentList.size() < 10) ? count = m_recentList.size() : count = 10;
	auto itr = m_recentList.begin();
	while (count)
	{
		if (*itr == songName)
		{
			return true;
		}
		count--;
		itr++;
	}
	return false;
}

Deleted::Deleted(std::map<std::string, Song>* library, std::string deletedPath)
{
	m_library = library;
	m_deletedPath = deletedPath;
	std::ifstream file(m_deletedPath);
	if (!file)
	{
		std::ofstream file(m_deletedPath);  // If the file not exist , open new file
	}
	// Deserialization process
	if (!fs::is_empty(m_deletedPath))
	{
		if (!m_deleted.empty())
		{
			m_deleted.clear();
		}
		std::string songName, path, singerName, album, publishingYear, duration, genre;
		int numberOfPlays, songId;
		while (!file.eof())
		{
				file >> songName >> path >> singerName >> album >> publishingYear
					>> duration >> genre >> numberOfPlays >> songId;
				if (songName == "@@@")   // To prevent the last line of the file from being read twice
				{
					break;
				}
				m_deleted.insert(std::make_pair(songName, Song(songName, path)));
				m_deleted[songName].SetSongName(songName);
				m_deleted[songName].SetSongPath(path);
				m_deleted[songName].SetSingerName(singerName);
				m_deleted[songName].SetAlbum(album);
				m_deleted[songName].SetPublishingYear(publishingYear);
				m_deleted[songName].SetDuration(duration);
				m_deleted[songName].SetGenre(genre);
				m_deleted[songName].SetNumberOfPlays(numberOfPlays);
				m_deleted[songName].SetSongId(songId);
		}
	}
	file.close();
}

Deleted::~Deleted()
{
	std::ofstream file(m_deletedPath);
	if (!file)
	{
		std::cout << m_deletedPath << " not found!" << std::endl;
		exit;
	}
	else  // Serialization process
	{
		for (auto itr = m_deleted.begin(); itr != m_deleted.end(); ++itr)
		{
			file << itr->second.GetSongName() << " " << itr->second.GetSongPath() << " "  
				<< itr->second.GetSingerName() << " " << itr->second.GetAlbum() << " "
				<< itr->second.GetPublishingYear() << " " << itr->second.GetDuration() << " "
				<< itr->second.GetGenre() << " " << itr->second.GetNumberOfPlays() << " "
				<< itr->second.GetSongId() << std::endl;
		}
		file << "@@@";   // To prevent the last line of the file from being read twice
	}
	file.close();
}

std::string Deleted::GetPlaylistPath() const
{
	return m_deletedPath;
}

Song Deleted::GetSongObjectBySongName(std::string songName) 
{
	if (m_deleted.find(songName) != m_deleted.end())
	{
		return m_deleted[songName];
	}
}

void Deleted::AddSong(std::string songName, Song songObj)
{
	if (m_deleted.find(songName) == m_deleted.end())
	{
		m_deleted.insert(std::make_pair(songName, songObj));
	}
	else
	{
		std::cout << "The song called " << songName << "is already exist on the Deleted list" << std::endl;
	}
}

void Deleted::DeleteSongBySongName(std::string songName)
{
	if (m_deleted.find(songName) != m_deleted.end())
	{
		m_deleted.erase(songName);
	}
}

void Deleted::DeleteAllSongs()
{
	if (!m_deleted.empty())
	{
		m_deleted.clear();
		std::cout << "All the songs in Deleted list erased" << std::endl;
	}
	else
	{
		std::cout << "Deleted list is empty" << std::endl;
	}
}

bool Deleted::CheckIfSongExist(std::string songName)
{
	if (m_deleted.find(songName) == m_deleted.end())
	{
		return false;
	}
	return true;
}

void Deleted::Print()
{
	std::cout << "------------------------------ Deleted ------------------------------" << std::endl;
	if (!m_deleted.empty())
	{
		for (auto itr = m_deleted.begin(); itr != m_deleted.end(); ++itr)
		{
			std::cout << "  Song name:" << std::setw(40) << itr->first << std::setw(15) << "song id:" << std::setw(5) << itr->second.GetSongId()<< std::endl;
		}
	}
	else
	{
		std::cout << "Deleted list is empty" << std::endl;
	}
}

DailyMix::DailyMix(std::map<std::string, Song>* library, std::map<std::string, PlayList*>* listOfPalyLists ,std::string dailyMixPath)
{
	srand(time(NULL));  // For randomaly operations

	// Intialization of user options in Daily Mix menu
	m_options.insert(std::make_pair("Play", '1'));
	m_options.insert(std::make_pair("Random", '2'));
	m_options.insert(std::make_pair("Help", '3'));
	m_options.insert(std::make_pair("Back", '0'));
	m_bufferFlag = false;  

	// Initialization of time struct to know if day changed
	struct tm m_newtime;
	time_t m_now;
	m_now = time(0);
	localtime_s(&m_newtime, &m_now);

	// Initialization Daily Mix objects
	m_dailyMixPath = dailyMixPath;
	m_library = library;
	m_listOfPalyLists = listOfPalyLists;
	std::ifstream file(m_dailyMixPath);
	if (!file)
	{
		std::ofstream file(m_dailyMixPath);  // If the file not exist , open new file
	}
	if (fs::is_empty(m_dailyMixPath)) // First enter to player
	{
		if (!m_dailyMix.empty())
		{
			m_dailyMix.clear();
		}
		int count = 10;   // Because we want 10 randomal songs
		std::vector<std::string> tempVec;
		std::string songName;
		for (auto itr = m_library->begin(); itr != m_library->end(); ++itr)  // Move all the songs name (keys) from library to vector for randomal searching
		{
			tempVec.push_back(itr->first);
		}
		while (count)
		{	
			int i = rand() % tempVec.size();
			songName = tempVec.at(i);
			m_dailyMix.insert(songName);  
			tempVec.erase(tempVec.begin() + i);
			count--;
		}
		m_day = m_newtime.tm_mday;
		m_month = 1 + m_newtime.tm_mon;
		m_year = 1900 + m_newtime.tm_year;
	}
	// Deserialization process
	else  // If the DailyMix file is not empty we get the data from the file
	{
		std::string songName;
		bool firstLoop = true;   
		while (!file.eof())
		{
			if (firstLoop == true)  
			{
				file >> m_day >> m_month >> m_year;   
				firstLoop = false;
				continue;
			}
			file >> songName; 
			if (songName == "@@@")  // To prevent the last line of the file from being read twice
			{
				break;
			}
			m_dailyMix.insert(songName);   
		}
		// Checking if day changed:
		if (m_day != m_newtime.tm_mday || m_month != (1 + m_newtime.tm_mon) || m_year != (1900 + m_newtime.tm_year))
		{ // day changed then we update the date and create new random DailyMix
			m_day = m_newtime.tm_mday;
			m_month = 1 + m_newtime.tm_mon;
			m_year = 1900 + m_newtime.tm_year;
			if (!m_dailyMix.empty())
			{
				m_dailyMix.clear();
			}
			int i;
			int count = 10;   // Because we want 10 randomal songs
			std::vector<std::string> tempVec;
			std::string songName;
			for (auto itr = m_library->begin(); itr != m_library->end(); ++itr)  // Move all the songs name (keys) from library to vector for randomal searching
			{
				tempVec.push_back(itr->first);
			}
			while (count)
			{
				i = rand() % tempVec.size();
				songName = tempVec.at(i);
				m_dailyMix.insert(songName);
				tempVec.erase(tempVec.begin() + i);
				count--;
			}
		}
	}
	file.close();
}

DailyMix::~DailyMix()
{
	std::ofstream file(m_dailyMixPath);
	if (!file)
	{
		std::cout << m_dailyMixPath << " not found!" << std::endl;
	}
	else  // Serialization process
	{
		file << m_day << " " << m_month << " " << m_year << " " << std::endl;
		for (auto itr = m_dailyMix.begin(); itr != m_dailyMix.end(); ++itr)
		{
			file << *itr << std::endl;
		}
		file << "@@@";  // To prevent the last line of the file from being read twice
	}
	file.close();
}

std::string DailyMix::GetPlaylistPath() const
{
	return m_dailyMixPath;
}

void DailyMix::DeleteSongBySongName(std::string songName)
{
	if (m_dailyMix.find(songName) != m_dailyMix.end())
	{
		m_dailyMix.erase(songName);
	}
} 

void DailyMix::UpdateSongName(std::string songName, std::string UpdateName)
{
	if (m_dailyMix.find(songName) != m_dailyMix.end()) // If this song name exist in podcast we update his name
	{
		m_dailyMix.erase(songName);
		m_dailyMix.insert(UpdateName);
	}
}

bool DailyMix::CheckIfSongExist(std::string songName)
{
	if (m_dailyMix.find(songName) == m_dailyMix.end())
	{
		return false;
	}
	return true;
}

bool DailyMix::CheckIfDayChanged()
{
	// Each time we check if the day changed we need to knoow the current day
	struct tm m_newtime;
	time_t m_now;
	m_now = time(0); 
	localtime_s(&m_newtime, &m_now);
	if (m_day == m_newtime.tm_mday && m_month == (1 + m_newtime.tm_mon) && m_year == (1900 + m_newtime.tm_year))
	{
		return false;
	}
	m_day = m_newtime.tm_mday;
	m_month = 1 + m_newtime.tm_mon;
	m_year = 1900 + m_newtime.tm_year;
	return true;
}

// Selection of 10 randomal songs from library
void DailyMix::SelectRandomSongs()
{
	if (!m_dailyMix.empty())
	{
		m_dailyMix.clear();
	}
	int i;
	int count;   
	std::vector<std::string> tempVec;
	std::string songName;
	auto itr = m_dailyMix.begin();
	for (auto itr = m_library->begin(); itr != m_library->end(); ++itr)  // Move all the songs name (keys) from library to vector for randomal searching
	{
		tempVec.push_back(itr->first);
	}
	(tempVec.size() < 10) ? count = tempVec.size() : count = 10;
	while (count)
	{
		i = rand() % tempVec.size();
		songName = tempVec.at(i);
		m_dailyMix.insert(songName);  
		tempVec.erase(tempVec.begin() + i);
		count--;
	}
}

void DailyMix::Print() 
{
	std::cout << "------------------------------ DailyMix ------------------------------" << std::endl;
	if (CheckIfDayChanged())
	{
		SelectRandomSongs();
	}
	for (auto itr = m_dailyMix.begin(); itr != m_dailyMix.end(); ++itr)
	{
		std::cout << "  Song name:" << std::setw(40) << *itr << std::setw(15) << "song id:" << std::setw(5) << m_library->at(*itr).GetSongId() << std::endl; 
	}
	std::cout << std::endl;
}

// Play all the songs in DailyMix 
void DailyMix::Play()
{
	if (CheckIfDayChanged())
	{
		SelectRandomSongs();
	}
	if (!m_dailyMix.empty())
	{
		char ch;
		bool startFlag = true;
		int count;
		auto itr = m_dailyMix.begin();
		(m_dailyMix.size() < 10) ? count = m_dailyMix.size() : count = 10;
		while (count)
		{
			if (startFlag != true)
			{
				ch = PlayMidMenu();
				if (ch == '2')
				{
					break;
				}
			}
			startFlag = false;
			m_player.play(m_library->at(*itr).GetSongPath(), true);
			m_library->at(*itr).IncreaseNumberOfPlays();
			m_listOfPalyLists->at("Recent")->AddSong(*itr, Song("", ""));
			std::cout << "End to play " << *itr << std::endl;
			itr++;
			count--;
		}
	}
	else
	{
		std::cout << "Daily Mix is empty" << std::endl;
	}
}

// Play all the songs in DailyMix randomly
void DailyMix::PlayRandom()
{
	if (CheckIfDayChanged())
	{
		SelectRandomSongs();
	}
	if (!m_dailyMix.empty())
	{
		int i;
		char ch;
		bool startFlag = true;
		int count;
		std::vector<std::string> tempVec;
		std::string songName;
		auto itr = m_dailyMix.begin();
		(m_dailyMix.size() < 10) ? count = m_dailyMix.size() : count = 10;
		while (count)
		{
			tempVec.push_back(*itr);
			itr++;
			count--;
		}
		(m_dailyMix.size() < 10) ? count = m_dailyMix.size() : count = 10;
		while (count)
		{
			if (startFlag != true)
			{
				ch = PlayMidMenu();
				if (ch == '2')
				{
					break;
				}
			}
			startFlag = false;
			i = rand() % tempVec.size();
			songName = tempVec.at(i);
			m_player.play(m_library->at(songName).GetSongPath(), true);
			m_library->at(songName).IncreaseNumberOfPlays();
			m_listOfPalyLists->at("Recent")->AddSong(songName, Song("", ""));
			tempVec.erase(tempVec.begin() + i);
			count--;
			std::cout << "End to play " << songName << std::endl;
		}
	}
	else
	{
		std::cout << "Daily Mix is empty..." << std::endl;
	}
}

void DailyMix::Help() const
{
	std::cout << "--------------- Daily Mix Menu ---------------" << "\n\n";
	std::cout << "1. For play all the song in Daily Mix write: " << "\n";
	std::cout << "   Play" << "\n\n";
	std::cout << "2. For play all the song in Daily Mix randomly write: " << "\n";
	std::cout << "   Random" << "\n\n";
	std::cout << "3. For help write: " << "\n";
	std::cout << "   Help" << "\n\n";
	std::cout << "4. For back to main menu: " << "\n";
	std::cout << "   Back" << "\n\n";
	std::cout << "------------------------------------------------------" << "\n";
}

// Check if the user token exist in DailyMix options
char DailyMix::CheckUserParams(std::string input)
{
	if (m_options.find(input) == m_options.end())
	{
		return 'x';
	}
	return m_options.at(input);
}

// In order to skip one itaration when cin buffer has empty string
bool DailyMix::BufferHandling(std::string userInput)
{
	if (userInput == "")
	{
		m_bufferFlag = true;
		return true;
	}
	m_bufferFlag = false;
	return false;
}
 
// DailyMix menu
void DailyMix::Menu()
{
	std::string userInput;
	char select;
	while (true)
	{
		if (m_bufferFlag == false)
			Help();
		std::getline(std::cin, userInput);
		if (BufferHandling(userInput))
			continue;
		select = CheckUserParams(userInput);
		switch (select)
		{
		case DAILY_PLAY:
			Play();
			break;
		case DAILY_PLAY_RANDOM:
			PlayRandom();
			break;
		case DAILY_HELP:
			Help();
			m_bufferFlag = true;  // For help Won't show up twice
			break;
		case DAILY_BACK:
			return;
		default:
			std::cout << "Illegal action!" << std::endl;
			std::cout << std::endl;
			break;
		}
	}
}

RegularPlayList::RegularPlayList(std::map<std::string, Song>* library, std::map<std::string, PlayList*>* listOfPalyLists, std::string regularPlayListName, std::string regularPlayListPath)
{
	// Initialization of RegularPlayList objects
	m_library = library;
	m_listOfPalyLists = listOfPalyLists;
	m_regularPlayListName = regularPlayListName;
	m_regularPlayListPath = regularPlayListPath;
	m_regularPlayListPath += m_regularPlayListName;
	m_regularPlayListPath += ".txt";
	std::ifstream file(m_regularPlayListPath);
	if (!file) // If the file not exist --> create new file
	{ 
		std::ofstream file(m_regularPlayListPath);   
	}
	else // The file already exist then we can read it
	{    // Deserialization process
		if (!fs::is_empty(m_regularPlayListPath))
		{ 
			std::string songName;
			while (!file.eof())
			{
				file >> songName;
				if (songName == "@@@")
				{
					break;
				}
				m_regularPlayList.insert(songName);
			}
		}
	}
	file.close();
}

RegularPlayList::~RegularPlayList()
{
	std::ifstream file(m_regularPlayListPath); // Try to open the file 
	if (!file)
	{
		
	}
	else // Serialization process
	{
		std::ofstream file(m_regularPlayListPath); // Open file to write
		for (auto itr = m_regularPlayList.begin(); itr != m_regularPlayList.end(); ++itr)
		{
			file << *itr << std::endl;
		}
		file << "@@@";
	}
	file.close();
}

std::string RegularPlayList::GetPlaylistPath() const
{
	return m_regularPlayListPath;
}

void RegularPlayList::AddSong(std::string songName, Song songObj)
{
	if (m_regularPlayList.find(songName) == m_regularPlayList.end())
	{
		m_regularPlayList.insert(songName);
	}
	else
	{
		std::cout << "The song called " << songName << "is already exist on this playlist" << std::endl;
	}
}

void RegularPlayList::DeleteSongBySongName(std::string songName)
{
	if (m_regularPlayList.find(songName) != m_regularPlayList.end())
	{
		m_regularPlayList.erase(songName);
	}
}

void RegularPlayList::DeleteAllSongs()
{
	if (!m_regularPlayList.empty())
	{
		m_regularPlayList.clear();
		std::cout << "All the songs in " << m_regularPlayListName << " erased" << std::endl;
	}
	else
	{
		std::cout << m_regularPlayListName << " list is empty" << std::endl;
	}
}

void RegularPlayList::UpdateSongName(std::string songName, std::string UpdateName)
{
	if (m_regularPlayList.find(songName) != m_regularPlayList.end())
	{
		m_regularPlayList.erase(songName);
		m_regularPlayList.insert(UpdateName);
	}
}

bool RegularPlayList::CheckIfSongExist(std::string songName)
{
	if (m_regularPlayList.find(songName) == m_regularPlayList.end())
	{
		return false;
	}
	return true;
}

void RegularPlayList::Play()  // Play all the podcasts alphabetically
{
	if (!m_regularPlayList.empty())
	{
		for (auto itr = m_regularPlayList.begin(); itr != m_regularPlayList.end(); ++itr)
		{
			m_player.play(m_library->at(*itr).GetSongPath(),true);
			m_library->at(*itr).IncreaseNumberOfPlays();
			m_listOfPalyLists->at("Recent")->AddSong(*itr, Song("", "")); // Add this podcast to recent playlist
			std::cout << "End to play " << *itr << std::endl;
			if (PlayMidMenu() == '2')
			{
				break;
			}
		}
	}
	else
	{
		std::cout << "This regular list is empty" << std::endl;
		std::cout << std::endl;
	}
}

void RegularPlayList::PlayRandom()  // play all the podcasts randomly
{
	if (!m_regularPlayList.empty())
	{
		int i;
		int count = m_regularPlayList.size();
		std::vector<std::string> tempVec;
		std::string songName;
		for (auto itr = m_regularPlayList.begin();itr != m_regularPlayList.end(); ++itr)
		{
			tempVec.push_back(*itr);
		}
		while (count)
		{
			i = rand() % tempVec.size();
			songName = tempVec.at(i);
			m_player.play(m_library->at(songName).GetSongPath(),true);
			m_library->at(songName).IncreaseNumberOfPlays();
			m_listOfPalyLists->at("Recent")->AddSong(songName, Song("", "")); // Add this podcast to recent playlist
			tempVec.erase(tempVec.begin() + i);
			count--;
			std::cout << "End to play " << songName << std::endl;
			if (PlayMidMenu() == '2')
			{
				break;
			}
		}
	}
	else
	{
		std::cout << "This regular playlist is empty" << std::endl;
	}
}

void RegularPlayList::Print()
{
	std::cout << "------------------------------ "<< m_regularPlayListName <<" ------------------------------" << std::endl;
	if (!m_regularPlayList.empty())
	{
		for (auto itr = m_regularPlayList.begin(); itr != m_regularPlayList.end(); ++itr)
		{
			std::cout << "  Song name:" << std::setw(40) << *itr << std::setw(15) << "song id:" << std::setw(5) << m_library->at(*itr).GetSongId() << std::endl;
		}
	}
	else
	{
		std::cout << "The playlist named: "<< m_regularPlayListName<< " is empty" << std::endl;
	}
}

