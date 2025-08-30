#pragma once
#include <windows.h>
#include <unordered_map>
#include <functional>
using namespace std;
class ButtonController
{
public:
	void Create(HWND parents, LPCWSTR text, int id, int x, int y, int w, int h);
	HWND GetHwnd() const { return hwnd; }
	void HandleCommand(WPARAM wParam); /// wParam���� ��ư ID�� �����ؼ� ID�� ������ ��ϵ� ���� ����
	void MessageOption();///��ư Ŭ�� �� ����� ���� �Լ� ��� 
	void Init(HWND parent, HINSTANCE instance) {
		hParent = parent;
		hInstance = instance;
	}
	
private:

	unordered_map<int, function<void()>>ButtonMessage;	/// [ Ű ] int = ��ư ID , [ �� ] function<void()> = Ŭ�� �� ����Ǵ� �Լ� 
	HWND hwnd = nullptr;  /// ��ư �ڵ� 
	HWND hParent = nullptr;
	HINSTANCE hInstance = nullptr;
};


