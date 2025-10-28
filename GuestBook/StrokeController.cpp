#include "StrokeController.h"

void StrokeController::Begin(int x, int y, COLORREF color) {
	current.points.clear();
	lastTime = GetTickCount64();
	current.points.push_back(Point{ x, y});
	current.color = color;
	recording = true;
}

void StrokeController::Add(int x, int y) {
	if (!recording) return;
	DWORD now = GetTickCount64();

	if (!current.points.empty()) {
		const Point& last = current.points.back();
		if (last.x == x && last.y == y) {
			return;
		}
	}
	current.points.push_back(Point{ x, y, now - lastTime });
	lastTime = now;
}

void StrokeController::End() {
	if (!recording) return;
	strokes.push_back(current);
	current.points.clear();
	recording = false;
}

void StrokeController::Clear() {
	std::lock_guard<std::mutex> lock(mtx);
	strokes.clear();
	current.points.clear();
	recording = false;
}

void StrokeController::AddPoint(Point p) {
	current.points.push_back(p);
}

void StrokeController::AddStroke() {
	strokes.push_back(current);
	current.points.clear();
}