#include"Library.h"
#include"Song.h"
#include"PlayList.h"
#include"AuxLib.h"
#include<iomanip>
#include<string>
#include<fstream>
#include<map>
#include<filesystem>
namespace fs = std::filesystem;

Library::Library(std::map<std::string, Song>* library, std::map<std::string, PlayList*>* ListOfPlayLists,std::set<std::string>* regularPlaylistNames, int* numberOfRegularPlaylists) //(std::map<std::string, Song>* library, std::map<std::string, PlayList*>& ListOfPlayLists)
{
	// Initialization of Library objects
	m_bufferFlag = false;
	m_library = library;
	m_ListOfPlayLists = ListOfPlayLists;
	m_regularPlaylistNams = regularPlaylistNames;
	m_numberOfRegularPlaylists = numberOfRegularPlaylists;
	m_count = 0;
	m_singer = "Unknown";
	m_album = "Unknown";
	m_genre = "Unknown";
	m_duration = "Unknown";
	m_publishingYear = "Unknown";

	// Initialization of user options of Library menu
	m_options.insert(std::make_pair("More", '1'));
	m_options.insert(std::make_pair("List", '2'));
	m_options.insert(std::make_pair("Add", '3'));
	m_options.insert(std::make_pair("Update", '4'));
	m_options.insert(std::make_pair("Delete", '5'));
	m_options.insert(std::make_pair("PrintSong", '6'));
	m_options.insert(std::make_pair("Add2PL", '7'));
	m_options.insert(std::make_pair("RemoveFromPL", '8'));
	m_options.insert(std::make_pair("PrintPL", '9'));
	m_options.insert(std::make_pair("Play", 'A'));
	m_options.insert(std::make_pair("PlayAll", 'B'));
	m_options.insert(std::make_pair("PlayRandom", 'C'));
	m_options.insert(std::make_pair("Undelete", 'D'));
	m_options.insert(std::make_pair("Help", 'E'));
	m_options.insert(std::make_pair("Back", '0'));
}

std::map<std::string, Song>* Library::GetLibrary() const
{
	return m_library;
}

int Library::GetCount() const
{
	return m_count;
}

void Library::SetCount(int more)
{
	m_count = more;
}

void Library::PrintSongsByGivenCount(int count) const
{
	std::cout << "------------------------------ Library ------------------------------" << std::endl;
	if (!m_library->empty())
	{
		bool endFlag = false;
		auto itr = m_library->begin();
		(count > m_library->size()) ? count = m_library->size(), endFlag = true : count = count;
		while (count)
		{
			std::cout << "  Song name:" << std::setw(40) << itr->first << std::setw(15) << "song id:" << std::setw(5) << itr->second.GetSongId() << std::endl;
			itr++;
			count--;
		}
		if (endFlag)
		{
			std::cout << "No more songs..." << std::endl;
		}
	}
	else
	{
		std::cout << "The library is empty" << std::endl;
	}
	std::cout << std::endl;
}

void Library::AddSong(std::string songPath, std::string songName)
{
	if (m_library->find(songName) == m_library->end())  // check if this song already exsist in library
	{
		m_library->insert(std::make_pair(songName,Song(songName, songPath)));
	}
	else
	{
		std::cout << "This song already exist in the library" << std::endl;
	}
}

bool Library::ExtensionValidate(std::string path)
{
	std::string token;
	size_t pos = path.find_last_of('.'); // find the posion of last '.'
	token = path.substr(pos);
	if (token == ".mp3")
	{
		return true;
	}
	else
	{
		return false;
	}
}


void Library::DeleteSong(std::string songName, int songId)
{
	if (songId != 0) // If we want to delete song by given song id
	{
		std::string tempSongName = FindSongNameBySongId(songId); // Check if song id exsit, if exsit --> return the suitable song name , if not exsit --> return empty string
		if (tempSongName != "")  
		{
			songName = tempSongName;
		}
		else
		{
			std::cout << "This song id not found!" << std::endl;
			return;
		}
	}
	Song songObjToDel = m_library->at(songName);
	m_library->erase(songName);  // Remove the item of old song name from the library
	for (auto itr = m_ListOfPlayLists->begin(); itr != m_ListOfPlayLists->end(); ++itr)
	{
		if (itr->first != "Deleted")  // Delete this song from all the playlists except Deleted list
		{
			itr->second->DeleteSongBySongName(songName);
		}
	}
	m_ListOfPlayLists->at("Deleted")->AddSong(songName, songObjToDel);
	std::cout << "The song named " << songName<< " erased from the library and all playlist" << "\n\n";
}


std::string Library::FindSongNameBySongId(int songId)
{
	for (auto itr = m_library->begin(); itr != m_library->end(); ++itr)
	{
		if (itr->second.GetSongId() == songId)
		{
			 return itr->second.GetSongName();
		}
	}
	return "";
}

void Library::UpdateSongParams(std::string songName,int songId, std::vector<std::string> updateVec)
{
	if (songId != 0) // If we want to update song by given song id
	{
		std::string tempSongName = FindSongNameBySongId(songId); // Check if song id exsit, if exsit --> return the suitable song name , if not exsit --> return empty string
		if (tempSongName != "")
		{
			songName = tempSongName;
		}
		else
		{
			std::cout << "This song id not found!" << "\n\n";
			return;
		}
	}
	bool SongNameUpdateFlag = false;
	std::string token;
	std::string update;
	std::vector<std::string> tokenVec;
	for (auto itr = updateVec.begin(); itr != updateVec.end(); ++itr)
	{
		tokenVec = AuxLib::SplitByDelim(*itr, '='); // split to token and update
		token = tokenVec.at(0); 
		update = AuxLib::ExtractWordBetween2Delim(tokenVec.at(1), '<', '>');
		if (update != "") // Check if the update input is valid
		{
			if (token == "name")
			{
				m_songName = update;
				SongNameUpdateFlag = true;
			}
			else if (token == "singer")
			{
				m_singer = update;
			}
			else if (token == "album")
			{
				m_album = update;
			}
			else if (token == "genre")
			{
				m_genre = update;
			}
			else if (token == "duration")
			{
				m_duration = update;
			}
			else if (token == "publishingYear")
			{
				try
				{
					int temp = std::stoi(update);  // Check if success to change to int
					// If changed 
					m_publishingYear = update;
				}
				catch (std::invalid_argument& ex)  // If we can not success to change to int
				{
					m_publishingYear = "Unknown";
				}
			}
			else
			{
				std::cout << "Invalid token input: " << token << "\n";
				std::cout << "The paramters: singer, album, genre and duration not updated" << "\n\n";
				ClearSongParams();
				return;
			}
		}
		else
		{
			std::cout << "Invalid input: " << tokenVec.at(1) << "\n\n";
			std::cout << "The paramters: singer, album, genre and duration not updated" << "\n\n";
			ClearSongParams();
			return;
		}
	}
	if (SongNameUpdateFlag)  // If we update song name in library we need to update this song name in all the playlists that it exists
	{
		if (m_library->find(m_songName) == m_library->end()) // Check if this song name already exist in library
		{
			m_library->insert(std::make_pair(m_songName, m_library->at(songName))); // Add new song with new song name
			m_library->at(m_songName).SetSongName(m_songName);  // Update the Song object with the new song name
			m_library->erase(songName);    // Remove the item of old song name from the library
			for (auto itr = m_ListOfPlayLists->begin(); itr != m_ListOfPlayLists->end(); ++itr)
			{
				itr->second->UpdateSongName(songName, m_songName);  // Update the song name in all playlists
			}
			songName = m_songName;
		}
	}
	m_library->at(songName).SetSingerName(m_singer);
	m_library->at(songName).SetAlbum(m_album);
	m_library->at(songName).SetGenre(m_genre);
	m_library->at(songName).SetDuration(m_duration);
	m_library->at(songName).SetPublishingYear(m_publishingYear);
	ClearSongParams();  // After each updae we have to "clear" the song parameters to next update.
	std::cout << "Update song named: " << songName << " Succeeded" << "\n\n";
}

void Library::PrintSongDetails(std::string songName , int songId)
{
	if (songId != 0) // If we want to print song details by given song id
	{
		std::string tempSongName = FindSongNameBySongId(songId); // Check if song id exsit, if exsit --> return the suitable song name , if not exsit --> return empty string
		if (tempSongName != "")
		{
			songName = tempSongName;
		}
		else
		{
			std::cout << "This song id not found!" << std::endl;
			return;
		}
	}
	std::cout << std::endl;
	std::cout << "---------- Song Details: ----------" << std::endl;
	std::cout << "  " << "Song name:" << "          " << songName << std::endl;
	std::cout << "  " << "Singer name:" << "        " << m_library->at(songName).GetSingerName() << std::endl;
	std::cout << "  " << "Album name:" << "         " << m_library->at(songName).GetAlbum() << std::endl;
	std::cout << "  " << "Publishing year:" << "    " << m_library->at(songName).GetPublishingYear() << std::endl;
	std::cout << "  " << "Duration:" << "           " << m_library->at(songName).GetDuration() << std::endl;
	std::cout << "  " << "Genre:" << "              " << m_library->at(songName).GetGenre() << std::endl;
	std::cout << "  " << "Song Id:" << "            " << m_library->at(songName).GetSongId() << std::endl;
	std::cout << "  " << "Number of plays:" << "    " << m_library->at(songName).GetNumberOfPlays() << std::endl;
	std::cout << "  " << "Song path:" << "          " << m_library->at(songName).GetSongPath() << std::endl;
	std::cout << "  " << "Playlists:" << std::endl;
	int count = 1;
	for (auto itr = m_ListOfPlayLists->begin(); itr != m_ListOfPlayLists->end(); ++itr)
	{
		if (itr->second->CheckIfSongExist(songName))
		{
			std::cout << count << ". " << itr->first << std::endl;
			count++;
		}
	}
	std::cout << std::endl;
}


void Library::AddSongToPlaylist(int songId, std::string playlistName)
{
	std::string songName;
	std::string tempSongName = FindSongNameBySongId(songId); // Check if song id exsit, if exsit --> return the suitable song name , if not exsit --> return empty string
	if (tempSongName != "")
	{
		songName = tempSongName;
	}
	else
	{
		std::cout << "This song id not found!" << std::endl;
		return;
	}
	if (m_ListOfPlayLists->find(playlistName) == m_ListOfPlayLists->end()) // Check if the playlist exist in list of playlist
	{ // Not exist
		m_ListOfPlayLists->insert(std::make_pair(playlistName, new RegularPlayList(m_library, m_ListOfPlayLists, playlistName)));
		m_regularPlaylistNams->insert(playlistName);    // Add the new playlist name to list of regular playlist
		*m_numberOfRegularPlaylists += 1;    // Increase the number of regular playlist
		m_ListOfPlayLists->at(playlistName)->AddSong(songName, Song("", ""));
		std::cout << "Playlist named: " << playlistName << " created and the song named: " << songName << " transferred to it" << std::endl;
		std::cout << std::endl;
	}
	else // Exist
	{
		m_ListOfPlayLists->at(playlistName)->AddSong(songName, Song("", ""));
		std::cout << "The song named: " << songName << " transferred to playlist named: " << playlistName << std::endl;
		std::cout << std::endl;
	}
}

void Library::RemoveSongFromPlaylist(std::string songName, std::string playlistName)
{
	if (m_ListOfPlayLists->find(playlistName) != m_ListOfPlayLists->end()) // Check if playlist exist
	{
		if (playlistName == "Recent" || playlistName == "DailyMix" || playlistName == "MostPlayed") // We cannot deletee songs from these playlists 
		{
			std::cout << "Sorry, you cannot delete songs from one of the following playlists:" << std::endl;
			std::cout << "Recent, DailyMix and  MostPlayed." << std::endl;
			std::cout << std::endl;
		}
		else
		{
			if (m_ListOfPlayLists->at(playlistName)->CheckIfSongExist(songName)) // Check if the song exist in playlist
			{
				m_ListOfPlayLists->at(playlistName)->DeleteSongBySongName(songName);  // Delete song from playlist
				std::cout << "The song named: " << songName << " erase from playlist named: " << playlistName << std::endl;
				std::cout << std::endl;
			}
			else
			{
				std::cout << "The song named: " << songName << " not exist in playlist named: " << playlistName << std::endl;
				std::cout << std::endl;
			}
		}
	}
	else
	{
		std::cout << "The playlist named: " << playlistName << " not exist in list of playlist" << std::endl;
	}
}

void Library::PrintAllPlaylist() const
{
	int count = 1;
	std::cout << "All existing playlist names:" << std::endl;
	for (auto itr = m_ListOfPlayLists->begin(); itr != m_ListOfPlayLists->end(); ++itr)
	{
		std::cout << count << ". " << itr->first << std::endl;
		count++;
	}
	std::cout << std::endl;
}

void Library::PlaySong(std::string songName, int songId)
{
	if (songId != 0) // If we want to play song by given song id
	{
		std::string tempSongName = FindSongNameBySongId(songId); // Check if song id exsit, if exsit --> return the suitable song name , if not exsit --> return empty string
		if (tempSongName != "")
		{
			songName = tempSongName;
		}
		else
		{
			std::cout << "This song id not found!" << std::endl;
			return;
		}
	}
	m_player.play(m_library->at(songName).GetSongPath(), true);
	m_library->at(songName).IncreaseNumberOfPlays();  // Increase the number of plays of this song
	m_ListOfPlayLists->at("Recent")->AddSong(songName, Song("", "")); // Add this podcast to recent playlist
	std::cout << "End to play " << songName << std::endl;
}

void Library::PlayAllSongs()
{
	if (!m_library->empty())
	{
		char ch;
		bool startFlag = true;
		for (auto itr = m_library->begin(); itr != m_library->end(); ++itr)
		{
			if (startFlag != true)
			{
				ch = PlayList::PlayMidMenu();  
				if(ch == '2')
				{
					break;
				}
			}
			startFlag = false;
			m_player.play(m_library->at(itr->first).GetSongPath(), true);
			m_library->at(itr->first).IncreaseNumberOfPlays();
			m_ListOfPlayLists->at("Recent")->AddSong(itr->first, Song("", "")); // Add this podcast to recent playlist
			std::cout << "End to play " << itr->first << std::endl;
		}
	}
	else
	{
		std::cout << "The library is empty" << std::endl;
		std::cout << std::endl;
	}
}

// Playing all the songs in Library randomly
void Library::PlayAllSongsRandom()
{
	if (!m_library->empty())
	{
		int i;
		bool startFlag = true;
		char ch;
		int count = m_library->size();
		std::vector<std::string> tempVec;
		std::string songName;
		for (auto itr = m_library->begin(); itr != m_library->end(); ++itr)
		{
			tempVec.push_back(itr->first);
		}
		while (count)
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
			i = rand() % tempVec.size();
			songName = tempVec.at(i);
			m_player.play(m_library->at(songName).GetSongPath(), true);
			m_library->at(songName).IncreaseNumberOfPlays();
			m_ListOfPlayLists->at("Recent")->AddSong(songName, Song("", "")); // Add this podcast to recent playlist
			tempVec.erase(tempVec.begin() + i);
			count--;
			std::cout << "End to play " << songName << std::endl;
		}
	}
	else
	{
		std::cout << "The library is empty" << std::endl;
		std::cout << std::endl;
	}
}

void Library::Undelete(std::string songName)
{
	if (m_ListOfPlayLists->at("Deleted")->CheckIfSongExist(songName))
	{ // Exist in Deleted list --> now we restore the song to library
		m_library->insert(std::make_pair(songName, m_ListOfPlayLists->at("Deleted")->GetSongObjectBySongName(songName)));  // Restore song name and song object from Deleted list
		m_ListOfPlayLists->at("Deleted")->DeleteSongBySongName(songName); // Delete the song from Deleted list
		std::cout << "The song name: " << songName << " Restored from Deleted list to library!" << std::endl;
	}
	else 
	{
		std::cout << "The song named: " << songName << " not exist in Deleted list" << std::endl;
	}
	std::cout << std::endl;
}

void Library::Help() const
{
	std::cout << "--------------- Library menu options ---------------" << "\n\n";
	std::cout << "1. For print more songs write:  " << "\n";
	std::cout << "   More" << "\n\n";
	std::cout << "2. For print 10 first songs write:" << "\n";
	std::cout << "   List" << "\n\n";
	std::cout << "3. For add a new song to library write: " << "\n";
	std::cout << "   Add filename_fullpath song_name singer=<singer> album=<album> genre=<genre> duration=<duration> publishingYear=<year>" << "\n";
	std::cout << "** You must enter filename_fullpath and song_name **" << "\n";
	std::cout << "** You must not include a space in the song name **" << "\n\n";
	std::cout << "4. For update song by song_name write:" << "\n";
	std::cout << "   Update song_name name=<name> singer=<singer> album=<album> genre=<genre> duration=<duration> publishingYear=<year>" << "\n";
	std::cout << "** You must enter song_name **" << "\n";
	std::cout << "** You must not include a space in the parameters names **" << "\n\n";
	std::cout << "5. For update song by song_id write: " << '\n';
	std::cout << "   Update song_id name=<name> singer=<singer> album=<album> genre=<genre> duration=<duration> publishingYear=<year>" << "\n";
	std::cout << "** You must enter song_id **" << "\n";
	std::cout << "** You must not include a space in the parameters names **" << "\n\n";
	std::cout << "6. For delete song by song_id write: " << "\n";
	std::cout << "   Delete <song_id>" << "\n\n";
	std::cout << "7. For delete song by song_name write: " << "\n";
	std::cout << "   Delete <song_name>" << "\n";
	std::cout << "** You must not include a space in the song name **" << "\n\n";
	std::cout << "8. For print song details by song_id write:" << "\n";
	std::cout << "   PrintSong <song_id>" << "\n\n";
	std::cout << "9. For print song details by song_name write:" << "\n";
	std::cout << "   PrintSong <song_name>" << "\n";
	std::cout << "** You must not include a space in the song name **" << "\n\n";
	std::cout << "10. For move song to playlist write: " << "\n";
	std::cout << "    Add2PL <song_id> <playlist_name>" << "\n";
	std::cout << "** You must not include a space in the playlist name **" << "\n\n";
	std::cout << "11. For remove song from playlist write: " << "\n";
	std::cout << "    RemoveFromPL <song_name> <playlist_name>" << "\n";
	std::cout << "** You must not include a space in the parameters names **" << "\n\n";
	std::cout << "12. For print all exist playlist write: " << "\n";
	std::cout << "    PrintPL" << "\n\n";
	std::cout << "13. For play song by song_name write: " << "\n";
	std::cout << "    Play <song_name>" << "\n";
	std::cout << "** You must not include a space in the song name **" << "\n\n";
	std::cout << "14. For play song by song_id write: " << "\n";
	std::cout << "    Play <song_id>" << "\n\n";
	std::cout << "15. For play all songs write: " << "\n";
	std::cout << "    PlayAll" << "\n\n";
	std::cout << "16. For play all songs randomly write: " << "\n";
	std::cout << "    PlayRandom" << "\n\n";
	std::cout << "17. For restore song from deleted list write: " << "\n";
	std::cout << "    Undelete <song_name>" << "\n";
	std::cout << "** You must not include a space in the song name **" << "\n\n";
	std::cout << "18.  For back to main menu write: " << "\n";
	std::cout << "     Back" << "\n\n";
	std::cout << "------------------------------------------------------" << "\n";
}

char Library::CheckUserParams()
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

bool Library::CheckAddParams()
{
	if (m_selections.size() < 3)  // For perform Add commad must be at least 3 parameters.
	{
		std::cout << "Invalid input params for Add command!" << "\n\n";
		return false;
	}
	if (!ExtensionValidate(m_selections.at(1)))  // Check if the path of the sond is valid
	{
		std::cout << "Invalid song path!" << "\n\n";
		return false;
	}
	// Check that the third paramer is a song name and not one of the tokens
	if (m_selections.at(2).find(' ') != std::string::npos)
	{// Exist token
		std::cout << "The third parameter have to be the song name without = " << "\n\n";
		return false;
	}
	return true;
}

void Library::ClearSongParams()
{
	m_singer = "Unknown";
	m_album = "Unknown";
	m_genre = "Unknown";
	m_duration = "Unknown";
	m_publishingYear = "Unknown";
}

bool Library::CheckIfSongExist(std::string songName)
{
	if (m_library->find(songName) != m_library->end())
	{
		return true;
	}
	return false;
}

bool Library::CheckUpdateParams()
{
	if (m_selections.size() < 3)
	{ // Example to valid input
		std::cout << "Invalid input params for Update command!" << "\n";
		std::cout << "You have to enter at least 3 params to update song:" << "\n";
		std::cout << "Example: Update song_name singer=<singer_name>" << "\n\n";
		return false;
	}
	if (m_selections.at(1).find("=") != std::string::npos)
	{// Exist token
		std::cout << "The second parameter have to be the song name or song id without = " << "\n\n";
		return false;
	}
	return true;
}

bool Library::checkPrintSongParams()
{
	if (m_selections.size() == 2)
	{
		return true;
	} 
	// Example to valid input
	std::cout << "Invalid input params for PrintSong command!" << "\n";
	std::cout << "You have to enter 2 params:" << "\n";
	std::cout << "Example: PrintSong <song_id>" << "\n\n";
	return false;
}

bool Library::CheckDeleteParams()
{
	if (m_selections.size() == 2)
	{
		return true;
	}
	// Example to valid input
	std::cout << "Invalid input params for Delete command!" << "\n";
	std::cout << "You have to enter 2 params:" << "\n";
	std::cout << "Example: Delete <song_id>" << "\n\n";
	return false;
}

bool Library::CheckAdd2PlParams()
{
	if (m_selections.size() == 3)
	{
		return true;
	}
	// Example to valid input
	std::cout << "Invalid input params for Add2PL command!" << "\n";
	std::cout << "You have to enter 3 params:" << "\n";
	std::cout << "Example: Add2PL <song_id> <playlist_name>" << "\n\n";
	return false;
}

bool Library::CheckRemoveFromPlParams()
{
	if (m_selections.size() == 3)
	{
		return true;
	}
	// Example to valid input
	std::cout << "Invalid input params for RemoveFromPL command!" << "\n";
	std::cout << "You have to enter 3 params:" << "\n";
	std::cout << "Example: RemoveFromPL <song_name> <playlist_name>" << "\n\n";
	return false;
}

bool Library::CheckPlayParams()
{
	if (m_selections.size() == 2)
	{
		return true;
	}
	// Example to valid input
	std::cout << "Invalid input params for Play command!" << "\n";
	std::cout << "You have to enter 2 params:" << "\n";
	std::cout << "Example: Play <song_name> " << "\n\n";
	return false;
}
bool Library::CheckUndeleteParams()
{
	if (m_selections.size() == 2)
	{
		return true;
	}
	// Example to valid input
	std::cout << "Invalid input params for Undelete command!" << "\n";
	std::cout << "You have to enter 2 params:" << "\n";
	std::cout << "Example: Undelete <song_name> " << "\n\n";
	return false;
}

// In the second itration of while loop std::cin remain empty string that enter to userInput variable in getline function,
// this function handling this phenomenon.
bool Library::BufferHandling(std::string userInput)
{
	if (userInput == "")
	{
		m_bufferFlag = true;
		return true;
	}
	m_bufferFlag = false;
	return false;
}

// Library menu
void Library::Menu()
{
	std::string userInput;
	std::string input1, input2;
	char select;
	while (true)
	{
		if(m_bufferFlag == false)
			Help();
		std::getline(std::cin, userInput);
		if (BufferHandling(userInput))
			continue;
		m_selections = AuxLib::SplitByDelim(userInput, ' ');  // Split the input string by spaces.
		select = CheckUserParams();  // Check the user input.
		switch (select)
		{
		case LIB_MORE:
			m_count += 10;
			PrintSongsByGivenCount(m_count);
			break;
		case LIB_LIST:
			m_count = 10;
			PrintSongsByGivenCount(m_count);
			break;
		case LIB_ADD:
			m_count = 10;
			if (CheckAddParams())  // Check the input of the user
			{
				AddSong(m_selections.at(1), m_selections.at(2));
				if ((m_selections.size() - 3))  // Check if the user enter optional option
				{
					std::vector<std::string> tempVec;
					tempVec.assign(m_selections.begin() + 3, m_selections.end()); // Move all the update params
					UpdateSongParams(m_selections.at(2), 0, tempVec);
				}
			}
			break;
		case LIB_UPDATE:
			m_count = 0;
			if (CheckUpdateParams())
			{
				std::vector<std::string> tempVec;
				tempVec.assign(m_selections.begin() + 2, m_selections.end()); // Move all the update params
				try
				{ // If the user enter song id:
					UpdateSongParams("", std::stod(m_selections.at(1)), tempVec);
				}
				catch (std::invalid_argument& ex)
				{ // If the user enter song name
					if (m_library->find(m_selections.at(1)) != m_library->end())
					{
						UpdateSongParams(m_selections.at(1), 0, tempVec);
					}
					else
					{
						std::cout << "The song named: " << m_selections.at(1) << " not exist in Library!" << "\n\n";
					}
				}
			}
			break;
		case LIB_PRINT_SONG:
			m_count = 0;
			if (checkPrintSongParams())
			{
				input1 = AuxLib::ExtractWordBetween2Delim(m_selections.at(1), '<', '>');
				if (input1 != "")
				{
					try
					{ // Print song details by sond id
						PrintSongDetails("", std::stoi(input1));
					}
					catch (std::invalid_argument& ex)
					{ // Print song details by song name
						if (CheckIfSongExist(input1))  
						{
							PrintSongDetails(input1, 0);
						}
						else
						{
							std::cout << "The song named: " << input1 << " not exist in Library" << "\n\n";
						}
					}
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case LIB_DELETE:
			m_count = 0;
			if (CheckDeleteParams())
			{
				input1 = AuxLib::ExtractWordBetween2Delim(m_selections.at(1), '<', '>');
				if (input1 != "") // Check if the input is valid
				{
					try
					{ // Delete song by song id
						DeleteSong("", std::stoi(input1));
					}
					catch (std::invalid_argument& ex)
					{ // Delete song by song name
						if (CheckIfSongExist(input1))
						{
							DeleteSong(input1, 0);
						}
						else
						{
							std::cout << "The song named: " << input1 << " not exist in Library" << "\n\n";
						}
					}
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case LIB_ADD2PL:
			m_count = 0;
			if (CheckAdd2PlParams())
			{
				input1 = AuxLib::ExtractWordBetween2Delim(m_selections.at(1), '<', '>');  // sond id
				input2 = AuxLib::ExtractWordBetween2Delim(m_selections.at(2), '<', '>');  // playlist name
				if (input1 != "" && input2 != "")
				{
					try
					{
						AddSongToPlaylist(std::stoi(input1), input2);
					}
					catch (std::invalid_argument& ex)
					{
						std::cout << ex.what() << "\n\n";
					}
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case LIB_REMOVE_FROM_PL:
			m_count = 0;
			if (CheckRemoveFromPlParams())
			{
				input1 = AuxLib::ExtractWordBetween2Delim(m_selections.at(1), '<', '>');  // sond name
				input2 = AuxLib::ExtractWordBetween2Delim(m_selections.at(2), '<', '>');  // playlist name
				if (CheckIfSongExist(input1)) // Check if the song exist in Library
				{
					RemoveSongFromPlaylist(input1, input2);
				}
				else
				{
					std::cout << "The song named: " << input1 << " not exist in Library" << "\n\n";
				}
			}
			break;
		case LIB_PRINT_PL:
			m_count = 0;
			PrintAllPlaylist();
			break;
		case LIB_PLAY:
			m_count = 0;
			if (CheckPlayParams())
			{
				input1 = AuxLib::ExtractWordBetween2Delim(m_selections.at(1), '<', '>');  // sond name
				if (input1 != "")
				{
					try
					{ // Play song by song id
						 PlaySong("",std::stoi(input1));
					}
					catch (std::invalid_argument& ex)
					{ // Play song by song name
						if (CheckIfSongExist(input1))
						{
							PlaySong(input1, 0);
						}
						else
						{
							std::cout << "The song named: " << input1 << " not exist in Library" << "\n\n";
						}
					}
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case LIB_PLAY_ALL:
			m_count = 0;
			PlayAllSongs();
			break;
		case LIB_PLAY_RANDOM:
			m_count = 0;
			PlayAllSongsRandom();
			break;
		case LIB_UNDELETE:
			m_count = 0;
			if (CheckUndeleteParams())
			{
				input1 = AuxLib::ExtractWordBetween2Delim(m_selections.at(1), '<', '>');  // sond name
				if (input1 != "")
				{
					Undelete(input1);
				}
				else
				{
					std::cout << "Invalid input!" << "\n\n";
				}
			}
			break;
		case LIB_HELP:
			m_count = 0;
			Help();
			m_bufferFlag = true;  // For help Won't show up twice
			break;
		case LIB_BACK:
			m_count = 0;
			return;
		default:
			m_count = 0;
			std::cout << "Illegal action!" << std::endl;
			std::cout << std::endl;
			break;
		}
	}
}