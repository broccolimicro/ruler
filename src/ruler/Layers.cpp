#include "Layers.h"
#include <algorithm>

using namespace std;

namespace ruler {

Rect::Rect() {
	net = -1;
	ll = vec2i(0,0);
	ur = vec2i(0,0);	
}

Rect::Rect(int draw, vec2i ll, vec2i ur) {
	this->net = -1;
	this->ll = ll;
	this->ur = ur;

	if (this->top < this->bottom) {
		int tmp = this->top;
		this->top = this->bottom;
		this->bottom = tmp;
	}

	if (this->right < this->left) {
		int tmp = this->right;
		this->right = this->left;
		this->left = tmp;
	}
}

Rect::Rect(const Routing &layer, int net, vec2i ll, vec2i ur) {
	this->draw = layer.drawingLayer;
	this->text = layer.labelLayer;
	this->net = net;

	this->left = ll[0];
	this->bottom = ll[1];
	this->right = ur[0];
	this->top = ur[1];

	if (this->top < this->bottom) {
		int tmp = this->top;
		this->top = this->bottom;
		this->bottom = tmp;
	}

	if (this->right < this->left) {
		int tmp = this->right;
		this->right = this->left;
		this->left = tmp;
	}
}


Rect::~Rect() {
}


bool operator<(const Bound &b0, const Bound &b1) {
	return (b0.pos < b1.pos);
}

bool operator<(const Bound &b, int p) {
	return (b.pos < p);
}

void Layer::sync() {
	for (int i = 0; i < 4; i++) {
		bound[i].clear();
		bound[i].reserve(geo.size());
		for (int j = 0; j < (int)geo.size(); j++) {
			bound[i].push_back(Bound(geo[j][i], j));
		}
		sort(bound[i].begin(), bound[i].end());
	}
	dirty = false;
}

void Layer::push(Rect rect, bool doSync) {
	int index = (int)geo.size();
	geo.push_back(rect);
	dirty = dirty or not doSync;
	if (not dirty) {
		for (int i = 0; i < 4; i++) {
			auto loc = lower_bound(bound[i].begin(), bound[i].end(), rect[i]); 
			bound[i].insert(loc, Bound(rect[i], index));
		}
	}
}

void Layer::push(vector<Rect> rects, bool doSync) {
	geo.reserve(geo.size()+rects.size());
	geo.insert(geo.end(), rects.begin(), rects.end());
	dirty = dirty or not doSync;
	if (not dirty) {
		for (int i = 0; i < 4; i++) {
			int sz = (int)bound[i].size();
			bound[i].reserve(bound[i].size()+rects.size());
			for (auto rect = rects.begin(); rect != rects.end(); rect++) {
				bound[i].push_back(Bound((*rect)[i], index));
			}
			sort(bound[i].begin() + sz, bound[i].end());
			inplace_merge(bound[i].begin(), bound[i].begin()+sz, bound[i].end());
		}
	}
}

void Layer::erase(int idx, bool doSync) {
	geo.erase(geo.begin()+idx);
	dirty = dirty or not doSync;
	if (not dirty) {
		for (int i = 0; i < 4; i++) {
			for (int j = (int)bound[i].size()-1; j >= 0; j--) {
				if (bound[i][j].idx == idx) {
					bound[i].erase(bound[i].begin() + j);
				} else if (bound[i][j].idx > idx) {
					bound[i][j].idx--;
				}
			}
		}
	}
}

// Compute the offset from (0,0) of the l0 geometry to (0,0) of the l1 geometry
// along axis at which l0 and l1 abut and save into offset. Require spacing on
// the opposite axis for non-intersection (default is 0). Return false if the two geometries
// will never intersect.
bool minOffset(int *offset, const Tech &tech, int axis, const Layer &l0, const Layer &l1) {
	if (l0.dirty) {
		l0.sync();
	}
	if (l1.dirty) {
		l1.sync();
	}

	int result = 0;
	bool conflict = false;

	// TODO(edward.bingham) get spacing rule for l0.id to l1.id
	int spacing = tech.mats[l0.id].minSpacing;

	vector<int> stack[2] = {vector<int>(), vector<int>()};
	// four indices:
	// l0 from for axis
	// l0 to for axis
	// l1 from for axis
	// l1 to for axis
	vector<int> idx(4, 0);
	int minValue;
	int minIdx;
	while (i < (int)layer.from[axis].size() and j < (int)layer.from[axis].size()) {
		for (int i = 0; i < (int)idx.size(); i++) {
			int value = ((i>>1) ?
				l1.bound[(axis<<1)+(i&1)][idx[i]].pos :
				l0.bound[(axis<<1)+(i&1)][idx[i]].pos) + 2*spacing*(i&1) - spacing;
			if (i == 0 or value < minValue) {
				minValue = value;
				minIdx = i;
			}
		}

		int layer = minIdx>>1;
		int isEnd = minIdx&1;
		int off = layer ?
			l1.geo[l1.bound[(axis<<1)+isEnd][idx[minIdx]].idx][((1-axis)<<1)+0] :
			l0.geo[l0.bound[(axis<<1)+isEnd][idx[minIdx]].idx][((1-axis)<<1)+1];
		auto loc = lower_bound(off, stack[layer].begin(), stack[layer].end());
		if (isEnd) {
			if (loc != stack[layer].end() and *loc == off) {
				stack[layer].erase(loc);
			}
		} else {
			stack[layer].insert(loc, off);
			int diff = abs(stack[1-layer].back() - off) + spacing;
			if (not conflict or diff > result) {
				result = diff;
				conflict = true;
			}
		}
		idx[minIdx]++;
	}

	*offset = result;
	return conflict;
}

int minOffset(int *offset, const Tech &tech, int axis, const Layers &l0, const Layers &l1) {
	int result = -1;
	for (int i = 0; i < (int)l0.layer.size();	i++) {
		for (int j = 0; j < (int)l1.layer.size(); j++) {
			int offset;
			// TODO(edward.bingham) handle cross-layer spacing
			if (l0.layer[i].id == l1.layer[j].id and minOffset(&offset, tech, axis, l0.layer[i], l1.layer[j])) {
				if (result < 0 or offset < result) {
					result = offset;
				}
			}
		}
	}
	return result;
}

}
