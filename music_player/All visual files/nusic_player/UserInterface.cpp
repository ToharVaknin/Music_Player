#include"UserInterface.h"
#include<fstream>
#include"Song.h"      // Have not a pointer or referacne to this object
#include"PlayList.h"  // Have not a pointer or referacne to this object
#include<iomanip>
#include<iostream>
#include <string>
#include<filesystem>
namespace fs = std::filesystem;

UserInterface::UserInterface(std::map<std::string, Song>* library,std::map<std::string,PlayList*>* ListOfPlayLists, std::string libraryPath) : m_searchObj(&m_library, &m_ListOfPlayLists), 
                                                                                                                                               m_libraryObj(&m_library,&m_ListOfPlayLists,&m_regularPlayListNames,&m_numberOfRegularPlayLists), 
	                                                                                                                                           m_playListOps(&m_library, &m_ListOfPlayLists,&m_regularPlayListNames,&m_constPlayLists,&m_numberOfRegularPlayLists)

{  
	// Initialization of user options in main menu
	m_options.insert(std::make_pair("Daily Mix", '1'));
	m_options.insert(std::make_pair("Search", '2'));
	m_options.insert(std::make_pair("Podcasts", '3'));
	m_options.insert(std::make_pair("Library", '4'));
	m_options.insert(std::make_pair("Play List", '5'));
	m_options.insert(std::make_pair("Exit", '6'));
	m_options.insert(std::make_pair("Help", '7'));
	m_libraryPath = libraryPath;
	m_bufferFlag = false;     // Flag to handling the empty string in the std::cin in the second itration of getline function
	std::ifstream file(m_libraryPath);  
	if (!file)
	{
		std::cout << "The Library file not found" << std::endl;
		return;
	}
	// Deserialization process
	std::string songName, path, singerName, album, publishingYear, duration, genre, regularPlayListName;
	int numberOfPlays, songId, numberOfRegularPlayLists;
	bool startLoop = true;
	while (!file.eof())  
	{
		if (startLoop == true)
		{
			file >> numberOfRegularPlayLists;
			m_numberOfRegularPlayLists = numberOfRegularPlayLists;
			if (m_numberOfRegularPlayLists != 0)
			{
				for (int i = 0; i < m_numberOfRegularPlayLists; ++i)
				{
					file >> regularPlayListName;
					m_regularPlayListNames.insert(regularPlayListName);
				}
			}
			startLoop = false;
			continue;
		}
			file >> songName >> path >> singerName >> album >> publishingYear
				>> duration >> genre >> numberOfPlays >> songId;
			if (songName == "@@@")
			{
				break;
			}
			m_library.insert(std::make_pair(songName, Song(songName, path)));
			m_library[songName].SetSongName(songName);
			m_library[songName].SetSongPath(path);
			m_library[songName].SetSingerName(singerName);
			m_library[songName].SetAlbum(album);
			m_library[songName].SetPublishingYear(publishingYear);
			m_library[songName].SetDuration(duration);
			m_library[songName].SetGenre(genre);
			m_library[songName].SetNumberOfPlays(numberOfPlays);
			m_library[songName].SetSongId(songId);
	}
	file.close();
	// Every constructor has own desirealization method:
	m_ListOfPlayLists.insert(std::make_pair("Favorite", new Favorite(&m_library,&m_ListOfPlayLists)));
	m_ListOfPlayLists.insert(std::make_pair("Recent", new Recent(&m_library,&m_ListOfPlayLists)));
	m_ListOfPlayLists.insert(std::make_pair("DailyMix", new DailyMix(&m_library,&m_ListOfPlayLists)));
	m_ListOfPlayLists.insert(std::make_pair("MostPlayed", new MostPlayed(&m_library,&m_ListOfPlayLists)));
	m_ListOfPlayLists.insert(std::make_pair("Deleted", new Deleted(&m_library)));
	m_ListOfPlayLists.insert(std::make_pair("Podcasts", new Podcasts(&m_library,&m_ListOfPlayLists)));
	// Deserialization of playlist that user created
	if (m_numberOfRegularPlayLists != 0)
	{
		for (auto itr = m_regularPlayListNames.begin(); itr != m_regularPlayListNames.end(); ++itr)
		{
			m_ListOfPlayLists.insert(std::make_pair(*itr, new RegularPlayList(&m_library, &m_ListOfPlayLists, *itr)));
		}
	} 
	m_constPlayLists.insert("DailyMix");
	m_constPlayLists.insert("MostPlayed");
	m_constPlayLists.insert("Recent");
}

UserInterface::~UserInterface()
{
	for (auto itr = m_ListOfPlayLists.begin(); itr != m_ListOfPlayLists.end(); ++itr)
	{
		delete itr->second;
	}
	std::ofstream file(m_libraryPath);
	file.clear();
	if (!file)
	{
		std::cout << m_libraryPath << " not found!" << std::endl;
	}
	else  // Serialization process
	{  
		file << m_numberOfRegularPlayLists << std::endl;
		if (m_numberOfRegularPlayLists != 0)
		{
			for (auto itr = m_regularPlayListNames.begin(); itr != m_regularPlayListNames.end(); ++itr)
			{
				file << *itr << std::endl;
			}
		}
		for (auto itr = m_library.begin(); itr != m_library.end(); ++itr)
		{
			file << itr->second.GetSongName() << " " << itr->second.GetSongPath() << " " 
				<< itr->second.GetSingerName() << " " << itr->second.GetAlbum() << " "
				<< itr->second.GetPublishingYear() << " " << itr->second.GetDuration() << " "
				<< itr->second.GetGenre() << " " << itr->second.GetNumberOfPlays() << " "
				<< itr->second.GetSongId() << std::endl;
		}
		file << "@@@";
	}
	file.close();
}

void UserInterface::Help() const
{
	std::cout << "--------------- Main Menu ---------------" << "\n\n";
	std::cout << "1. For Daily Mix write: " << "\n";
	std::cout << "   Daily Mix" << "\n\n";
	std::cout << "2. For Search write: " << "\n";
	std::cout << "   Search" << "\n\n";
	std::cout << "3. For Podcasts write: " << "\n";
	std::cout << "   Podcasts" << "\n\n";
	std::cout << "4. For Library write: " << "\n";
	std::cout << "   Library" << "\n\n";
	std::cout << "5. For Play List write: " << "\n";
	std::cout << "   Play List" << "\n\n";
	std::cout << "5. For help write: " << "\n";
	std::cout << "   Help" << "\n\n";
	std::cout << "7. For Exit write: " << "\n";
	std::cout << "   Exit" << "\n\n";
	std::cout << "------------------------------------------------------" << "\n";
}

// In the second itration of while loop std::cin remain empty string that enter to userInput variable in getline function,
// this function handling this phenomenon.
bool UserInterface::BufferHandling(std::string userInput)
{
	if (userInput == "")
	{
		m_bufferFlag = true;
		return true;
	}
	m_bufferFlag = false;
	return false;
}

// Chech if the input of user found in the options of main menu
char UserInterface::CheckParams(std::string input)
{
	if (m_options.find(input) == m_options.end())
	{
		return 'x';
	}
	return m_options.at(input);
}


void UserInterface::Play()
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
		select = CheckParams(userInput);
		switch (select)
		{
		case DAILY_MIX:
			std::cout << *m_ListOfPlayLists["DailyMix"];
			m_ListOfPlayLists["DailyMix"]->Menu();
			break;
		case SEARCH:
			std::cout << m_searchObj;
			break;
		case PODCAST:
			std::cout << *m_ListOfPlayLists["Podcasts"];
			m_ListOfPlayLists["Podcasts"]->Menu();
			break;
		case LIBRARY:
			std::cout << m_libraryObj;   // Print the 10 first songs in library
			m_libraryObj.Menu();
			break;
		case PLAY_LIST:
			m_playListOps.PrintAllExistPlayLists();
			m_playListOps.PlayListMenu();
			break;
		case PLAY_HELP:
			Help();
			m_bufferFlag = true;
			break;
		case EXIT:
			return;
		default:
			std::cout << "Invalid input" << "\n\n";
			break;

		}
	}
}




