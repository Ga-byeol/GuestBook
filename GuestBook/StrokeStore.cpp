#include "StrokeStore.h"

void StrokeStore::Begin(int x, int y) {
	current.points.clear();
	lastTime = GetTickCount64();
	current.points.push_back(Point{ x, y });
	recording = true;
	// 색상 가져와야함
}

void StrokeStore::Add(int x, int y) {
	if (!recording) return;
	DWORD now = GetTickCount64();

	/*if (!current.points.empty()) {
		const Point& last = current.points.back();
		if (last.x == x && last.y == y) {
			return;
		}
	}*/
	current.points.push_back(Point{ x, y, now - lastTime });
	lastTime = now;

}

void StrokeStore::End() {
	if (!recording) return;
	strokes.push_back(current);
	current.points.clear();
	recording = false;
}

void StrokeStore::Clear() {
	std::lock_guard<std::mutex> lock(mtx);

	strokes.clear();
	current.points.clear();
	recording = false;
}

void StrokeStore::ReplayCopyStroke(Stroke& dest, const Stroke& src) {
	std::lock_guard<std::mutex> lock(mtx);
	/// src(원본) 값을 dest(목적지)에 복사 대입함
	/// dest.points = src.points
	/// dest.color = src.color
	/// dest.thickness = src.thickness
	/// ReplayStroke안의 선 정보를 참조하고 있는 k를 tempStroke에 복사함
	dest = src;
}

void StrokeStore::ReplaySetCurrentStyle(COLORREF color, int thickness) {
	std::lock_guard<std::mutex> lock(mtx);
	current.color = color;
	current.thickness = thickness;
}

void StrokeStore::ReplayCopyPointToCurrent(Point pt) {
	std::lock_guard<std::mutex> lock(mtx);
	/// pt(tempStorke.points 정보를 복사함)를 current에 넣는다
	current.points.push_back(pt);
}

void StrokeStore::ReplayCopyTempToStrokes() {
	std::lock_guard<std::mutex> lock(mtx);
	/// tempStroke 선을 Strokes에 넣는다 
	strokes.push_back(current);
}

void StrokeStore::ReplayClearCurrent() {
	std::lock_guard<std::mutex> lock(mtx);
	current.points.clear();
}
