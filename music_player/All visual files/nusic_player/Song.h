#ifndef SONG_H
#define SONG_H
#include<iostream>


class Song
{
public:
	Song(std::string songName ="", std::string songPath ="");
	Song(const Song& rhs);
	~Song();
	static int SerialNumber;
	static int NumberOfSongs;
	int GetNumberOfSongs() const;
	std::string GetSongName() const;
	std::string GetSingerName() const;
	std::string GetAlbum() const;
	std::string GetPublishingYear() const;
	std::string GetDuration() const;
	std::string GetSongPath() const;
	std::string GetGenre() const;
	int GetNumberOfPlays() const;
	int GetSongId() const;
	void SetGenre(std::string genre);
	void SetSongName(std::string songName);
	void SetSingerName(std::string singerName);
	void SetAlbum(std::string album);
	void SetPublishingYear(std::string publishingYear);
	void SetDuration(std::string duration);
	void SetSongPath(std::string songPath);
	void SetNumberOfPlays(int numberOfPlays);
	void SetSongId(int songId);
	void IncreaseNumberOfPlays();
	friend std::ostream& operator<<(std::ostream& os, const Song& rhs)
	{
		os << rhs.GetSongName();
		return os;
	}
	Song& operator=(const Song& rhs);
private:
	std::string m_songName;
	std::string m_singerName;
	std::string m_album;
	std::string m_songPath;
	std::string m_publishingYear;
	std::string m_duration;
	std::string m_genre;
	int m_numberOfPlays;
	int m_songId;
};


#endif // !SONG_H

