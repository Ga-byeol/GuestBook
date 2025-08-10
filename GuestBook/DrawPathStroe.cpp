#include "DrawPathStore.h"

void DrawPathStore::Begin(int x, int y) {
	current.points.clear();
	current.points.push_back(POINT{ x, y });
	recording = true;
}

void DrawPathStore::Add(int x, int y) {
	if (!recording) return;
	current.points.push_back(POINT{ x, y });
}

void DrawPathStore::End() {
	if (!recording) return;
	paths.push_back(current);
	current.points.clear();
	recording = false;
}

void DrawPathStore::Clear() {
	paths.clear();
	current.points.clear();
	recording = false;
}
