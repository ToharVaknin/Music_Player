#include"Song.h"


Song::Song(std::string songName, std::string songPath) : m_songName(songName) , m_songPath(songPath)
{
	m_duration = "Unknown"; 
	m_publishingYear = "Unknown"; 
	m_singerName = "Unknown";
	m_album = "Unknown";
	m_genre = "Unknown";
	NumberOfSongs++; // Each time we add Song the number of total songs increase by 1
	m_numberOfPlays = 0; 
	SerialNumber++; // Static variable
	m_songId = SerialNumber;
}

Song::Song(const Song& rhs)  
{							
	this->m_songName = rhs.m_songName;
	this->m_singerName = rhs.m_singerName;
	this->m_album = rhs.m_album;
	this->m_songPath = rhs.m_songPath;
	this->m_publishingYear = rhs.m_publishingYear;
	this->m_duration = rhs.m_duration;
	this->m_genre = rhs.m_genre;
	this->m_numberOfPlays = rhs.m_numberOfPlays;
	this->m_songId = rhs.m_songId;
}

Song& Song::operator=(const Song& rhs)  
{
	if (this == &rhs)
	{
		return *this;
	}
	this->m_songName = rhs.m_songName;
	this->m_singerName = rhs.m_singerName;
	this->m_album = rhs.m_album;
	this->m_songPath = rhs.m_songPath;
	this->m_publishingYear = rhs.m_publishingYear;
	this->m_duration = rhs.m_duration;
	this->m_genre = rhs.m_genre;
	this->m_numberOfPlays = rhs.m_numberOfPlays;
	this->m_songId = rhs.m_songId;
	return *this;
}

Song::~Song()
{
	NumberOfSongs--; 
}

std::string Song::GetSongName() const
{
	return m_songName;
}

std::string Song::GetSingerName() const
{
	return m_singerName;
}

std::string Song::GetAlbum() const
{
	return m_album;
}

std::string Song::GetPublishingYear() const
{
	return m_publishingYear;
}

std::string Song::GetDuration() const
{
	return m_duration;
}

std::string Song::GetSongPath() const
{
	return m_songPath;
}

int Song::GetSongId() const
{
	return m_songId;
}

int Song::GetNumberOfSongs() const
{
	return NumberOfSongs;
}

int Song::GetNumberOfPlays() const
{
	return m_numberOfPlays;
}

std::string Song::GetGenre() const
{
	return m_genre;
}

void Song::SetGenre(std::string genre)
{
	m_genre = genre;
}

void Song::SetSongName(std::string songName)
{
	m_songName = songName;
}

void Song::SetSingerName(std::string singerName)
{
	m_singerName = singerName;
}

void Song::SetAlbum(std::string album)
{
	m_album = album;
}
void Song::SetPublishingYear(std::string publishingYear)
{
	m_publishingYear = publishingYear;
}

void Song::SetDuration(std::string duration)
{
	m_duration = duration;
}

void Song::SetSongPath(std::string songPath)
{
	m_songPath = songPath;
}

void Song::SetNumberOfPlays(int numberOfPlays)
{
	m_numberOfPlays = numberOfPlays;
}

void Song::SetSongId(int songId)
{
	m_songId = songId;
}

void Song::IncreaseNumberOfPlays()
{
	m_numberOfPlays += 1;
}

int Song::SerialNumber(0); // Initialization of static variable
int Song::NumberOfSongs(0);
