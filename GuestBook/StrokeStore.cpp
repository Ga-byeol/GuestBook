#include "StrokeStore.h"

void StrokeStore::Begin(int x, int y) {
	current.points.clear();
	current.points.push_back(POINT{ x, y });
	recording = true;
}

void StrokeStore::Add(int x, int y) {
	if (!recording) return;
	current.points.push_back(POINT{ x, y });
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
