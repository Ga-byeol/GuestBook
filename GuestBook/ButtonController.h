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
	void HandleCommand(WPARAM wParam); /// wParam에서 버튼 ID를 추출해서 ID가 있으면 등록된 람다 실행
	void MessageOption();///버튼 클릭 시 실행될 람다 함수 등록 
	void Init(HWND parent, HINSTANCE instance) {
		hParent = parent;
		hInstance = instance;
	}
	
private:

	unordered_map<int, function<void()>>ButtonMessage;	/// [ 키 ] int = 버튼 ID , [ 값 ] function<void()> = 클릭 시 실행되는 함수 
	HWND hwnd = nullptr;  /// 버튼 핸들 
	HWND hParent = nullptr;
	HINSTANCE hInstance = nullptr;
};


