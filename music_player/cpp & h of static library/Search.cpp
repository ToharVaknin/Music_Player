#include"Search.h"
#include"AuxLib.h"
#include<iomanip>
#include<fstream>
#include<filesystem>
namespace fs = std::filesystem;

Search::Search(std::map<std::string, Song>* library,std::map<std::string,PlayList*>* listOfPlaylists, std::string searchPath) 
{
	// Initialization of search options container:
	m_searchOptions.insert(std::make_pair("Search by name", '1'));
	m_searchOptions.insert(std::make_pair("Search by singer", '2'));
	m_searchOptions.insert(std::make_pair("Search by album", '3'));
	m_searchOptions.insert(std::make_pair("Search by genre", '4'));
	m_searchOptions.insert(std::make_pair("PrintHistory", '5'));
	m_searchOptions.insert(std::make_pair("MoreHistory", '6'));
	m_searchOptions.insert(std::make_pair("DeleteHistory", '7'));
	m_searchOptions.insert(std::make_pair("Help", '8'));
	m_searchOptions.insert(std::make_pair("Back", '0'));

	// Initialization of delete options container:
	m_deleteOptions.insert(std::make_pair("DeleteName", '1'));
	m_deleteOptions.insert(std::make_pair("DeleteSinger", '2'));
	m_deleteOptions.insert(std::make_pair("DeleteAlbum", '3'));
	m_deleteOptions.insert(std::make_pair("DeleteGenre", '4'));
	m_deleteOptions.insert(std::make_pair("Help", '5'));
	m_deleteOptions.insert(std::make_pair("Back", '0'));

	// Initialization of play options container:
	m_playOptions.insert(std::make_pair("Play", '1'));
	m_playOptions.insert(std::make_pair("PlayAll", '2'));
	m_playOptions.insert(std::make_pair("Back", '0'));

	// Initialization of search objects
	m_library = library;
	m_listOfPlaylists = listOfPlaylists;
	m_searchPath = searchPath;
	m_bufferFlag = false;
	m_count = 0;
	std::ifstream file(m_searchPath);
	if (!file)
	{
		std::ofstream file(m_searchPath);
	}
	// Deserialization process
	if (!fs::is_empty(m_searchPath))
	{
		std::string history;
		while (std::getline(file,history))
		{
			if (history == "@@@")
			{
				break;
			}
			m_history.push_back(history);
		}
	}
	file.close();
}

Search::~Search()
{
	std::ofstream file(m_searchPath);
	if (!file)
	{
		std::cout << m_searchPath << " not found!" << std::endl;
	}
	else // Serialization process
	{
		for (auto itr = m_history.begin(); itr != m_history.end(); ++itr)
		{
			file << *itr << std::endl;
		}
		file << "@@@";
	}
	file.close();
}


void Search::SearchBySongName(std::string songName)
{ // Check if this song name exist in Library
	if (m_library->find(songName) != m_library->end())  
	{ // Exist
		std::cout << "The song named: " << songName << " found!" << std::endl;
		m_search.insert(songName);
		PlaySearches();
	}
	else  // Not exist
	{
		std::cout << "The song named: " << songName << " not exist in library" << std::endl;
		std::cout << std::endl;
	}
}

void Search::SearchBySingerName(std::string singerName)
{
	bool Flag = false;
	for (auto itr = m_library->begin(); itr != m_library->end(); ++itr)
	{
		if (itr->second.GetSingerName() == singerName)
		{
			m_search.insert(itr->first);  // Insert the song name to container
			Flag = true;  // If at least one song of this singer found , the flag will be true
		}
	}
	if (Flag) 
	{ // At least one song of this singer found
		int count = 1;
		std::cout << "Songs list of " << singerName << ":" << std::endl;
		for (auto itr = m_search.begin(); itr != m_search.end(); ++itr)
		{
			std::cout<<count<<". " << *itr << std::endl;
			count++;
		}
		std::cout << std::endl;
		PlaySearches();
	}
	else
	{
		std::cout << "No songs found in the library for this singer" << std::endl;
		std::cout << std::endl;
	}
}

void Search::SearchByAlbum(std::string albumName)
{
	bool Flag = false;
	for (auto itr = m_library->begin(); itr != m_library->end(); ++itr)
	{
		if (itr->second.GetAlbum() == albumName)
		{
			m_search.insert(itr->first);  // Insert the song name to container
			Flag = true;  // If at least one song of this singer found , the flag will be true
		}
	}
	if (Flag)
	{ // At least one song of this singer found
		int count = 1;
		std::cout << "Songs list of " << albumName << ":" << std::endl;
		for (auto itr = m_search.begin(); itr != m_search.end(); ++itr)
		{
			std::cout << count << ". " << *itr << std::endl;
			count++;
		}
		std::cout << std::endl;
		PlaySearches();
	}
	else
	{
		std::cout << "No songs found in the library for this album" << std::endl;
		std::cout << std::endl;
	}
}

void Search::SearchByGenre(std::string genreName)
{
	bool Flag = false;
	for (auto itr = m_library->begin(); itr != m_library->end(); ++itr)
	{
		if (itr->second.GetGenre() == genreName)
		{
			m_search.insert(itr->first);  // Insert the song name to container
			Flag = true;  // If at least one song of this singer found , the flag will be true
		}
	}
	if (Flag)
	{  // At least one song of this singer found
		int count = 1;
		std::cout << "Songs list of " << genreName << ":" << std::endl;
		for (auto itr = m_search.begin(); itr != m_search.end(); ++itr)
		{
			std::cout << count << ". " << *itr << std::endl;
			count++;
		}
		std::cout << std::endl;
		PlaySearches();
	}
	else
	{
		std::cout << "No songs found in the library for this genre" << std::endl;
		std::cout << std::endl;
	}
}

void Search::PlayBySongName(std::string songName)
{ // Check if this song name exist in m_search object
	if (m_search.find(songName) != m_search.end())
	{ // Exist
		m_player.play(m_library->at(songName).GetSongPath(), true);
		m_library->at(songName).IncreaseNumberOfPlays();
		m_listOfPlaylists->at("Recent")->AddSong(songName, Song("", ""));
		std::cout << "End to play " << songName << std::endl;
	}
	else // Not exist
	{
		std::cout << "The song named: " << songName << " not found!" << std::endl;
	}
}

void Search::InsertToHistorySearch(std::string search)
{
	if (std::find(m_history.begin(), m_history.end(), search) == m_history.end())  
	{ // Not exist in history
		m_history.push_front(search);
	}
	else // Exist in history
	{
		m_history.remove(search);
		m_history.push_front(search);
	}
}

void Search::PrintHistoryByGivenCount(int count)
{
	if (!m_history.empty())
	{
		bool endFlag = false;
		auto itr = m_history.begin();
		(count > m_history.size()) ? count = m_history.size(), endFlag = true : count = count;
		std::cout <<"------------------------------ Search History ------------------------------" << std::endl;
		while (count)
		{
			std::cout << "  " << *itr << std::endl;
			itr++;
			count--;
		}
		if (endFlag)
		{
			std::cout << "No more history..." << std::endl;
		}
	}
	else
	{
		std::cout << "No search history" << std::endl;
	}
	std::cout << std::endl;
}

bool Search::ChekIfExsitInHistory(std::string search)
{
	if (std::find(m_history.begin(), m_history.end(), search) != m_history.end())
	{
		return true;
	}
	return false;
}

char Search::CheckUserParamsDeleteMenu()
{ // Check if the token that the user entered found in delete options
	if (m_deleteOptions.find(m_selections.at(0)) != m_deleteOptions.end())
	{ // Exist in delete options
		return m_deleteOptions.at(m_selections.at(0));
	}
	return 'x';
}

bool Search::CheckDeleteParams()
{
	if (m_selections.size() == 2)
	{
		return true;
	}
	std::cout << "Invalid input params for Delete command!" << "\n";
	std::cout << "You have to enter 2 params:" << "\n";
	std::cout << "Example: DeleteSinger <singer_name>" << "\n\n";
	return false;
}

void Search::DeleteHistory()
{
	std::string userInput,deleteName;
	char select;
	bool ExistFlag = false; // For check if the erase succeeded
	m_bufferFlag = false;
	while (true)
	{
		if (m_bufferFlag == false)
			DeleteHelp();
		std::getline(std::cin, userInput);
		if (BufferHandling(userInput))
			continue;
		m_selections = AuxLib::SplitByDelim(userInput, ' ');  // Split the input string by spaces.
		select = CheckUserParamsDeleteMenu();  // Check the token input of the user
		switch (select)
		{
		case SEARCH_DELETE_NAME:
			if (CheckDeleteParams())
			{
				deleteName = AuxLib::ExtractWordBetween2Delim(m_selections.at(1), '<', '>');
				if (deleteName != "")
				{
					// Check if this song name exist in history search --> if yes, remove this search from history search container
					(ChekIfExsitInHistory("Song name: " + deleteName)) ? m_history.remove("Song name: " + deleteName), ExistFlag = true : ExistFlag = false;
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case SEARCH_DELETE_SINGER:
			if (CheckDeleteParams())
			{
				deleteName = AuxLib::ExtractWordBetween2Delim(m_selections.at(1), '<', '>');
				if (deleteName != "")
				{
					// Check if this singer name exist in history search --> if yes, remove this search from history search container
					(ChekIfExsitInHistory("Singer name: " + deleteName)) ? m_history.remove("Singer name: " + deleteName), ExistFlag = true : ExistFlag = false;
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case SEARCH_DELETE_ALBUM:
			if (CheckDeleteParams())
			{
				deleteName = AuxLib::ExtractWordBetween2Delim(m_selections.at(1), '<', '>');
				if (deleteName != "")
				{
					// Check if this album name exsit in history search --> if yes, remove this search from history search container
					(ChekIfExsitInHistory("Album name: " + deleteName)) ? m_history.remove("Album name: " + deleteName), ExistFlag = true : ExistFlag = false;
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case SEARCH_DELETE_GENRE:
			if (CheckDeleteParams())
			{
				deleteName = AuxLib::ExtractWordBetween2Delim(m_selections.at(1), '<', '>');
				if (deleteName != "")
				{
					// Check if this genre name exsit in history search --> if yes, remove this search from history search container
					(ChekIfExsitInHistory("Genre name: " + deleteName)) ? m_history.remove("Genre name: " + deleteName), ExistFlag = true : ExistFlag = false;
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case SEARCH_HELP_2:
			DeleteHelp();
			m_bufferFlag = true;
			break;
		case SEARCH_BACK_3:
			return;
		default:
			std::cout << "Illegal action!" << std::endl;
			std::cout << std::endl;
			continue;
		}
		if (ExistFlag)
		{
			std::cout << "The deletion succeeded" << std::endl;
			std::cout << std::endl;
		}
		else
		{
			std::cout << "Deletion was unsuccessful" << std::endl;
			std::cout << std::endl;
		}
	}
}

void Search::DeleteHelp() const
{
	std::cout << "--------------- Delete Menu ---------------" << "\n\n";
	std::cout << "1. For delete history by song name write: " << "\n";
	std::cout << "   DeleteName <song_name>" << "\n";
	std::cout << "** You must not include a space in the song name **" << "\n\n";
	std::cout << "2. For delete history by singer name write: " << "\n";
	std::cout << "   DeleteSinger <singer_name>" << "\n";
	std::cout << "** You must not include a space in the singer name **" << "\n\n";
	std::cout << "3. For delete history by album name write: " << "\n";
	std::cout << "   DeleteAlbum <album_name>" << "\n";
	std::cout << "** You must not include a space in the album name **" << "\n\n";
	std::cout << "4. For delete history by genre name write: " << "\n";
	std::cout << "   DeleteGenre <genre_name>" << "\n";
	std::cout << "** You must not include a space in the genre name **" << "\n\n";
	std::cout << "5. For help write: " << "\n";
	std::cout << "   Help" << "\n\n";
	std::cout << "6. For back to search menu write: " << "\n";
	std::cout << "   Back" << "\n\n";
	std::cout << "------------------------------------------------------" << "\n";

}

// Play all the songs that exist in m_search 
void Search::PlayAll()
{
	bool startFlag = true;
	char ch;
	for (auto itr = m_search.begin(); itr != m_search.end(); ++itr)
	{
		if (startFlag != true)
		{
			ch = PlayList::PlayMidMenu();
			if (ch == '2')
			{
				break;
			}
		}
		startFlag = false;
		m_player.play(m_library->at(*itr).GetSongPath(), true);
		m_library->at(*itr).IncreaseNumberOfPlays();
		m_listOfPlaylists->at("Recent")->AddSong(*itr, Song("", ""));
		std::cout << "End to play " << *itr << std::endl;
	}
}

// Play menu
void Search::PlaySearches()
{
	m_bufferFlag = false;
	std::string userInput, songName;
	char select;
	while (true)
	{
		if (m_bufferFlag == false)
			PlayHelp();
		std::getline(std::cin, userInput);
		if (BufferHandling(userInput))
			continue;
		m_selections = AuxLib::SplitByDelim(userInput, ' ');  // Split the input string by spaces.
		select = CheckUserParamsPlayMenu();
		switch (select)
		{
		case SEARCH_PLAY_SONG:
			if (m_selections.size() == 2)
			{
				songName = AuxLib::ExtractWordBetween2Delim(m_selections.at(1), '<', '>');
				if (songName != "")
				{
					PlayBySongName(songName);
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case SEARCH_PLAY_ALL:
			PlayAll();
			break;
		case SEARCH_BACK_2:
			return;
		default:
			std::cout << "Illegal action!" << "\n\n";
			break;
		}
	}
}

char Search::CheckUserParamsPlayMenu()
{
	if (m_selections.size() == 1 || m_selections.size() == 2)
	{ // Check if the token that the user entered found in play options
		if (m_playOptions.find(m_selections.at(0)) != m_playOptions.end())
		{
			return m_playOptions.at(m_selections.at(0));
		}
	}
	return 'x';
}

void Search::PlayHelp() const
{
	std::cout << "--------------- Play Menu ---------------" << "\n\n";
	std::cout << "1. For play by song name write: " << "\n";
	std::cout << "   Play <song_name>" << "\n";
	std::cout << "** You must not include a space in the song name **" << "\n\n";
	std::cout << "2. For play all write: " << "\n";
	std::cout << "   PlayAll" << "\n\n";
	std::cout << "3. For back to search menu write: " << "\n";
	std::cout << "   Back" << "\n\n";
	std::cout << "------------------------------------------------------" << "\n";

}

bool Search::BufferHandling(std::string userInput)
{ // In order to skip one itaration when cin buffer has empty string
	if (userInput == "")
	{
		m_bufferFlag = true;
		return true;
	}
	m_bufferFlag = false;
	return false;
}

char Search::CheckUserParamsSearchMenue()
{  
	// The input of the user can be contain 1, 2 or 4 params:
	if (m_selections.size() == 2 || m_selections.size() == 1)
	{
		if (m_searchOptions.find(m_selections.at(0)) != m_searchOptions.end()) // Check if the input of user found in the search options
		{
			return m_searchOptions.at(m_selections.at(0));
		}
	}
	if (m_selections.size() == 4)  // Example: Search by name <song_name> 
	{
		std::string userInput = m_selections.at(0) + " " + m_selections.at(1) + " " + m_selections.at(2);
		if (m_searchOptions.find(userInput) != m_searchOptions.end()) // Check if the input of user found in the search options
		{
			return m_searchOptions.at(userInput);
		}
	}
	 return 'x';

}

bool Search::CheckSearchParams()
{
	if (m_selections.size() == 4)
	{
		return true;
	}
	std::cout << "Invalid input params for Search command!" << "\n";
	std::cout << "You have to enter 4 params:" << "\n";
	std::cout << "Example: Search by name <song_name>" << "\n\n";
	return false;
}

// Search menu
void Search::SearchMenu()
{
	std::string userInput, searchName;
	char select;
	while (true)
	{
		if (m_bufferFlag == false)
			Help();
		std::getline(std::cin, userInput);
		if (BufferHandling(userInput))
			continue;
		m_selections = AuxLib::SplitByDelim(userInput, ' ');  // Split the input string by spaces.
		select = CheckUserParamsSearchMenue();
		switch (select)
		{
		case NAME:
			m_count = 0;
			if (CheckSearchParams())
			{
				searchName = AuxLib::ExtractWordBetween2Delim(m_selections.at(3), '<', '>');
				if (searchName != "")
				{
					InsertToHistorySearch("Song name: " + searchName);  // Enter to history search
					SearchBySongName(searchName);
					m_search.clear();  // Clear the search container to next operations
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case SINGER:
			m_count = 0;
			if (CheckSearchParams())
			{
				searchName = AuxLib::ExtractWordBetween2Delim(m_selections.at(3), '<', '>');
				if (searchName != "")
				{
					InsertToHistorySearch("Singer name: " + searchName);
					SearchBySingerName(searchName);
					m_search.clear();
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case ALBUM:
			m_count = 0;
			if (CheckSearchParams())
			{
				searchName = AuxLib::ExtractWordBetween2Delim(m_selections.at(3), '<', '>');
				if (searchName != "")
				{
					InsertToHistorySearch("Album name: " + searchName);
					SearchByAlbum(searchName);
					m_search.clear();
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case GENRE:
			m_count = 0;
			if (CheckSearchParams())
			{
				searchName = AuxLib::ExtractWordBetween2Delim(m_selections.at(3), '<', '>');
				if (searchName != "")
				{
					InsertToHistorySearch("Genre name: " + searchName);
					SearchByGenre(searchName);
					m_search.clear();
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case PRINT_HISTORY:
			m_count = 10;
			PrintHistoryByGivenCount(m_count);
			break;
		case PRINT_MORE_HISTORY:
			m_count += 10;
			PrintHistoryByGivenCount(m_count);
			break;
		case DELETE_HISTORY:
			m_count = 0;
			DeleteHistory();
			break;
		case HELP:
			m_count = 0;
			Help();
			m_bufferFlag = true;  // Increase the buffer flag to true in order to not print help function twice
			break;
		case SEARCH_BACK_1:
			return;
		default:
			m_count = 0;
			std::cout << "Illegal action!" << std::endl;
			std::cout << std::endl;
			break;


		}
	}
}

void Search::Help() const
{
	std::cout << "--------------- Search Menu ---------------" << "\n\n";
	std::cout << "1. For search by song name write: " << "\n";
	std::cout << "   Search by name <song_name>" << "\n";
	std::cout << "** You must not include a space in the song name **" << "\n\n";
	std::cout << "2. For search by singer name write: " << "\n";
	std::cout << "   Search by singer <singer_name>" << "\n";
	std::cout << "** You must not include a space in the singer name **" << "\n\n";
	std::cout << "3. For search by album write: " << "\n";
	std::cout << "   Search by album <albun_name>" << "\n";
	std::cout << "** You must not include a space in the album name **" << "\n\n";
	std::cout << "4. For search by genre write: " << "\n";
	std::cout << "   Search by genre <genre_name>" << "\n";
	std::cout << "** You must not include a space in the genre name **" << "\n\n";
	std::cout << "5. For print search history write: " << "\n";
	std::cout << "   PrintHistory" << "\n\n";
	std::cout << "6. For print more search history write: " << "\n";
	std::cout << "   MoreHistory" << "\n\n";
	std::cout << "7. For print delete search history write: " << "\n";
	std::cout << "   DeleteHistory" << "\n\n";
	std::cout << "8. For help write: " << "\n";
	std::cout << "   Help" << "\n\n";
	std::cout << "9. For back to main menu write: " << "\n";
	std::cout << "   Back" << "\n\n";
	std::cout << "------------------------------------------------------" << "\n";
}