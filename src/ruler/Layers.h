#pragma once

#include "vector.h"
#include <vector>

#include "Tech.h"

using namespace std;

namespace ruler {

struct Rect {
	Rect();
	Rect(int net, vec2i ll, vec2i ur);
	~Rect();

	int net;
	// locations in db units of lower left corner and upper right corner
	// respectively
	vec2i ll;
	vec2i ur;

	int operator[](int i) {
		return (i&1) ? ur[i>>1] : ll[i>>1];
	}
};


// is this bound compared along the x or y boundary?
struct Bound {
	Bound();
	Bound(int pos, int idx);
	~Bound();
	
	// index into Layer::geo
	int idx;
	
	int pos;
};

bool operator<(const Bound &b0, const Bound &b1);
bool operator<(const Bound &b, int p);

struct Interval {
	// DESIGN(edward.bingham) this interval is inclusive [from,to] because it
	// represents geometry positions

	// location in db units of left and right bound
	int from;
	int to;
};

struct Layer {
	// this is the source of truth
	// index into layer stack defined in Tech
	int id;
	vector<Rect> geo;
	
	// these are here for performance
	bool dirty;
	
	enum {
		FROM_H = 0,
		TO_H = 1,
		FROM_V = 2,
		TO_V = 3
	};

	vector<Bound> bound[4];

	void sync();

	void push(Rect rect, bool doSync=false);
	void push(vector<Rect> rects, bool doSync=false);
	void erase(int index, bool doSync=false);
};

struct Layers {
	vector<Layer> layer;
};

bool minOffset(int *offset, const Tech &tech, int axis, Layer &l0, Layer &l1);
int minOffset(int *offset, const Tech &tech, int axis, Layers &l0, Layers &l1);

}
