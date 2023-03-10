#include"PlayListOps.h"
#include"Player.h"
#include"PlayList.h"
#include"AuxLib.h"
#include<filesystem>
namespace fs = std::filesystem;


PlayListOps::PlayListOps(std::map<std::string, Song>* library, std::map<std::string, PlayList*>* listOfPlaylist, std::set<std::string>* regularPlayListNames, std::set<std::string>* constPlayLists, int* numberOfRegularPlaylists)
{
	// Initialization of playlist options:
	m_options.insert(std::make_pair("Add", '1'));
	m_options.insert(std::make_pair("Delete", '2'));
	m_options.insert(std::make_pair("Play", '3'));
	m_options.insert(std::make_pair("PlayRandom", '4'));
	m_options.insert(std::make_pair("Print", '5'));
	m_options.insert(std::make_pair("Help", '6'));
	m_options.insert(std::make_pair("Back", '0'));

	// Initialization of PlayListOps objects:
	m_bufferFlag = false;  
	m_library = library;
	m_ListOfPlayLists = listOfPlaylist;
	m_regularPlayListNames = regularPlayListNames;
	m_numberOfRegularPlaylists = numberOfRegularPlaylists;
	m_constPlayLists = constPlayLists;
}

// Add new playlist that user created
void PlayListOps::AddPlayList(std::string playListName)
{ // Check if playlist name already exist
	if (m_ListOfPlayLists->find(playListName) == m_ListOfPlayLists->end())
	{ // Exist
		m_ListOfPlayLists->insert(std::make_pair(playListName, new RegularPlayList(m_library, m_ListOfPlayLists, playListName)));
		*m_numberOfRegularPlaylists += 1;
		m_regularPlayListNames->insert(playListName);
		std::cout << "Playlist named " << playListName << " added to list of playlist" << std::endl;
		std::cout << std::endl;
	}
	else // Not exist
	{
		std::cout << "The playlist called " << playListName << " already exist on the list of playlists" << std::endl;
		std::cout << std::endl;

	}
}

// Delete playlist --> only not constant playlist
void PlayListOps::DeletePlayList(std::string playListName)
{
	//bool deleteFlag = false;
	if (m_ListOfPlayLists->find(playListName) != m_ListOfPlayLists->end())
	{
		if (playListName == "Deleted" || playListName == "Podcasts" || playListName == "Favorites") // Delete only the songs in the playlist but not the folder (file)
		{
			m_ListOfPlayLists->at(playListName)->DeleteAllSongs();
			std::cout << "All the songs in " << playListName << " playlist deleted" << std::endl;
			std::cout << std::endl;
		}
		else if (m_constPlayLists->find(playListName) != m_constPlayLists->end()) // Doesn't delete the songs or the folder
		{
			std::cout << "This folder cannot be deleted" << std::endl;
			std::cout << std::endl;
		}
		else  // Delete the file
		{
			try
			{
				if (fs::remove(m_ListOfPlayLists->at(playListName)->GetPlaylistPath()))
				{
					delete m_ListOfPlayLists->at(playListName);  
					m_ListOfPlayLists->erase(playListName);
					*m_numberOfRegularPlaylists -= 1;
					m_regularPlayListNames->erase(playListName);
					std::cout << "The playlist named: " << playListName << " erased" << std::endl;
					std::cout << std::endl;
				}
				else
				{
					std::cout << "The file not found" << std::endl;
					std::cout << std::endl;
				}
			}
			catch (const std::filesystem::filesystem_error& err)
			{
				std::cout << "filesystem error: " << err.what() << std::endl;
			}
		}
	}
	else
	{
		std::cout << "The playlist named: " << playListName << " not found in list of playlist" << std::endl;
		std::cout << std::endl;
	}
}

void PlayListOps::PlayPlaylist(std::string playListName)
{
	if (m_ListOfPlayLists->find(playListName) != m_ListOfPlayLists->end())
	{ // This playlist exist in list of playlists
		m_ListOfPlayLists->at(playListName)->Play();
	}
	else
	{
		std::cout << "The playlist named: " << playListName << " not exist in list of playlists" << std::endl;
		std::cout << std::endl;
	}
}

void PlayListOps::PlayPlaylistRandom(std::string playListName)
{
	if (m_ListOfPlayLists->find(playListName) != m_ListOfPlayLists->end())
	{ // This playlist exist in list of playlists
		m_ListOfPlayLists->at(playListName)->PlayRandom();
	}
	else
	{
		std::cout << "The playlist named: " << playListName << " not exist in list of playlists" << std::endl;
		std::cout << std::endl;
	}
}

void PlayListOps::PrintPlaylist(std::string playListName)
{
	if (m_ListOfPlayLists->find(playListName) != m_ListOfPlayLists->end())
	{ // This playlist exist in list of playlists
		std::cout << *m_ListOfPlayLists->at(playListName);   // Print all the song in this playlist
		std::cout << std::endl;
	}
	else
	{
		std::cout << "The playlist named: " << playListName << " not exist in list of playlists" << std::endl;
		std::cout << std::endl;
	}
}

void PlayListOps::PrintAllExistPlayLists() const
{
	int count = 4;
	std::cout << "All existing playlist names:" << std::endl;
	std::cout << "1. " << "Favorite" << std::endl;
	std::cout << "2. " << "MostPlayed" << std::endl;
	std::cout << "3. " << "Recent" << std::endl;
	for (auto itr = m_ListOfPlayLists->begin(); itr != m_ListOfPlayLists->end(); ++itr)
	{
		if (itr->first != "Favorite" && itr->first != "MostPlayed" && itr->first != "Recent" && itr->first != "Deleted")
		{
			std::cout << count << ". " << itr->first << std::endl;
			count++;
		}
	}
	std::cout << count << ". " << "Deleted" << std::endl;
	std::cout << std::endl;
}

bool PlayListOps::BufferHandling(std::string userInput)
{
	if (userInput == "")
	{
		m_bufferFlag = true;
		return true;
	}
	m_bufferFlag = false;
	return false;
}

char PlayListOps::CheckUserParams()
{ // Check the size input of the user
	if (m_selections.size() == 1 || m_selections.size() == 2)
	{  // Check if the token that the user enter found in playlist options
		if (m_options.find(m_selections.at(0)) != m_options.end())
		{ // Exist
			return m_options.at(m_selections.at(0));
		}
	}
	return 'x';
}

bool PlayListOps::CheckPlaylistParams()
{
	if (m_selections.size() == 2)
	{
		return true;
	}
	std::cout << "Invalid input params for playlist commands!" << "\n";
	std::cout << "You have to enter 2 params:" << "\n";
	std::cout << "Example: Add <playlist_name>" << "\n\n";
	return false;
}

// Playlist menu
void PlayListOps::PlayListMenu()
{
	std::string playlistName,userInput;
	char select;
	m_bufferFlag = false;
	while (true)
	{
		if (m_bufferFlag == false)
			Help();
		std::getline(std::cin, userInput);
		if (BufferHandling(userInput))
			continue;
		m_selections = AuxLib::SplitByDelim(userInput, ' ');  // Split the input string by spaces.
		select = CheckUserParams();
		switch (select)
		{
		case PLAY_LIST_ADD:
			// Check the size of the input of user for know if we can extract the playlist name
			if (CheckPlaylistParams())
			{
				playlistName = AuxLib::ExtractWordBetween2Delim(m_selections.at(1), '<', '>');
				if (playlistName != "")
				{
					AddPlayList(playlistName);
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case PLAY_LIST_DELETE:
			if (CheckPlaylistParams())
			{
				playlistName = AuxLib::ExtractWordBetween2Delim(m_selections.at(1), '<', '>');
				if (playlistName != "")
				{
					DeletePlayList(playlistName);
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case PLAY_LIST_PLAY:
			if (CheckPlaylistParams())
			{
				playlistName = AuxLib::ExtractWordBetween2Delim(m_selections.at(1), '<', '>');
				if (playlistName != "")
				{
					PlayPlaylist(playlistName);
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case PLAY_LIST_PLAY_RANDOM:
			if (CheckPlaylistParams())
			{
				playlistName = AuxLib::ExtractWordBetween2Delim(m_selections.at(1), '<', '>');
				if (playlistName != "")
				{
					PlayPlaylistRandom(playlistName);
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case PLAY_LIST_PRINT:
			if (CheckPlaylistParams())
			{
				playlistName = AuxLib::ExtractWordBetween2Delim(m_selections.at(1), '<', '>');
				if (playlistName != "")
				{
					PrintPlaylist(playlistName);
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case PLAY_LIST_HELP:
			Help();
			m_bufferFlag = true; // For not to print the help function twice
			break;
		case PLAY_LIST_BACK:
			return;
		default:
			std::cout << "Invalid input" << "\n\n";
			break;
		}
	}
}

void PlayListOps::Help() const
{
	std::cout << "--------------- Playlist Menu ---------------" << "\n\n";
	std::cout << "1. For add playlist write: " << "\n";
	std::cout << "   Add <playlist_name>" << "\n";
	std::cout << "** You must not include a space in the playlist name **" << "\n\n";
	std::cout << "2. For delete playlist write: " << "\n";
	std::cout << "   Delete <playlist_name>" << "\n";
	std::cout << "** You must not include a space in the playlist name **" << "\n\n";
	std::cout << "3. For play playlist write: " << "\n";
	std::cout << "   Play <playlist_name>" << "\n";
	std::cout << "** You must not include a space in the playlist name **" << "\n\n";
	std::cout << "4. For play playlist randomly write: " << "\n";
	std::cout << "   PlayRandom <playlist_name>" << "\n";
	std::cout << "** You must not include a space in the playlist name **" << "\n\n";
	std::cout << "5. For print playlist write: " << "\n";
	std::cout << "   Print <playlist_name>" << "\n";
	std::cout << "** You must not include a space in the playlist name **" << "\n\n";
	std::cout << "6. For help write: " << "\n";
	std::cout << "   Help" << "\n\n";
	std::cout << "7. For back to main menu write: " << "\n";
	std::cout << "   Back" << "\n\n";
	std::cout << "------------------------------------------------------" << "\n";
}