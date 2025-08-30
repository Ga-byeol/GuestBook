#include "StrokeStore.h"

void StrokeStore::Begin(int x, int y) {
	current.points.clear();
	current.points.push_back(Point{ x, y, 0 });
	recording = true;
}

void StrokeStore::Add(int x, int y) {
	if (!recording) return;

	if (!current.points.empty()) {
		const Point& last = current.points.back();
		if (last.x == x && last.y == y) {
			return;
		}
	}
	current.points.push_back(Point{ x, y, 0 });
}

void StrokeStore::End() {
	if (!recording) return;
	strokes.push_back(current);
	current.points.clear();
	recording = false;
}

void StrokeStore::Clear() {
	strokes.clear();
	current.points.clear();
	recording = false;
}

void StrokeStore::ReplayCopyStroke(Stroke& dest, const Stroke& src) {
	/// src(����) ���� dest(������)�� ���� ������
	/// dest.points = src.points
	/// dest.color = src.color
	/// dest.thickness = src.thickness
	/// ReplayStroke���� �� ������ �����ϰ� �ִ� k�� tempStroke�� ������
	dest = src;
}

void StrokeStore::ReplaySetCurrentStyle(COLORREF color, int thickness) {
	current.color = color;
	current.thickness = thickness;
}

void StrokeStore::ReplayCopyPointToCurrent(int x, int y) {
	/// pt(tempStorke.points ������ ������)�� current�� �ִ´�
	current.points.push_back(Point{ x, y });
}

void StrokeStore::ReplayCopyTempToStrokes(Stroke& s) {
	/// tempStroke ���� Strokes�� �ִ´� 
	strokes.push_back(s);
}

void StrokeStore::ReplayClearCurrent() {
	current.points.clear();
}
