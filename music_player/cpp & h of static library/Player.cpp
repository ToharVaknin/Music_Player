#include"Player.h"

HANDLE Player::m_pthread = 0;

LRESULT CALLBACK handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case MM_MCINOTIFY:
		cout << "Song ended" << endl;
		return 0;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

DWORD WINAPI threadedPlay(LPVOID lpParameter) {
	Player player;
	vector<string>* pl = (vector<string>*)lpParameter;
	vector<string>::const_iterator psong;
	for (psong = (*pl).begin(); psong != (*pl).end(); psong++)
		player.play(*psong, true);
	return 0;
}
