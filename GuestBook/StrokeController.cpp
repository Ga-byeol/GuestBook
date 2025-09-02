#include "StrokeController.h"

void StrokeController::Begin(int x, int y) {
	current.points.clear();
	current.points.push_back(Point{ x, y, 0 });
	recording = true;
}

void StrokeController::Add(int x, int y) {
	if (!recording) return;

	if (!current.points.empty()) {
		const Point& last = current.points.back();
		if (last.x == x && last.y == y) {
			return;
		}
	}
	current.points.push_back(Point{ x, y, 0 });
}

void StrokeController::End() {
	if (!recording) return;
	strokes.push_back(current);
	current.points.clear();
	recording = false;
}

void StrokeController::Clear() {
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