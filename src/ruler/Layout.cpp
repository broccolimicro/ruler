#include "Layout.h"
#include <algorithm>

using namespace std;

namespace ruler {

Rect::Rect() {
	net = -1;
	ll = vec2i(0,0);
	ur = vec2i(0,0);	
}

Rect::Rect(int net, vec2i ll, vec2i ur) {
	this->net = net;
	this->ll = ll;
	this->ur = ur;

	if (this->ur[1] < this->ll[1]) {
		int tmp = this->ur[1];
		this->ur[1] = this->ll[1];
		this->ll[1] = tmp;
	}

	if (this->ur[0] < this->ll[0]) {
		int tmp = this->ur[0];
		this->ur[0] = this->ll[0];
		this->ll[0] = tmp;
	}
}

Rect::~Rect() {
}

bool Rect::merge(Rect r) {
	if (net == r.net and ll[0] == r.ll[0] and ur[0] == r.ur[0] and ll[1] <= r.ur[1] and ur[1] >= r.ll[1]) {
		if (r.ll[1] < ll[1]) {
			ll[1] = r.ll[1];
		}
		if (r.ur[1] > ur[1]) {
			ur[1] = r.ur[1];
		}
		return true;
	} else if (net == r.net and ll[1] == r.ll[1] and ur[1] == r.ur[1] and ll[0] <= r.ur[0] and ur[0] >= r.ll[0]) {
		if (r.ll[0] < ll[0]) {
			ll[0] = r.ll[0];
		}
		if (r.ur[0] > ur[0]) {
			ur[0] = r.ur[0];
		}
		return true;
	} else if (net == r.net and ll[1] <= r.ll[1] and ur[1] >= r.ur[1] and ll[0] <= r.ll[0] and ur[0] >= r.ur[0]) {
		return true;
	} else if (net == r.net and ll[1] >= r.ll[1] and ur[1] <= r.ur[1] and ll[0] >= r.ll[0] and ur[0] <= r.ur[0]) {
		ll[0] = r.ll[0];
		ur[0] = r.ur[0];
		ll[1] = r.ll[1];
		ur[1] = r.ur[1];
		return true;
	}
	return false;
}

bool Rect::hasLabel() const {
	return net >= 0;
}

gdstk::Polygon *Rect::emit(const Tech &tech, int layer) const {
	return new gdstk::Polygon(gdstk::rectangle(gdstk::Vec2{(double)ll[0], (double)ll[1]}, gdstk::Vec2{(double)ur[0], (double)ur[1]}, gdstk::make_tag(tech.mats[layer].major, tech.mats[layer].minor)));
}

gdstk::Label *Rect::emitLabel(const Tech &tech, const Layout &layout, int layer) const {
	if (net < 0) {
		return nullptr;
	}
	return new gdstk::Label{
		.tag = gdstk::make_tag(tech.mats[layer].major, tech.mats[layer].minor),
		.text = strdup(layout.nets[net].c_str()),
		.origin = gdstk::Vec2{(double)((ll[0] + ur[0])/2), (double)((ll[1]+ur[1])/2)},
		.magnification = 1,
	};
}

bool operator<(const Bound &b0, const Bound &b1) {
	return (b0.pos < b1.pos);
}

bool operator<(const Bound &b, int p) {
	return (b.pos < p);
}

Bound::Bound() {
	idx = -1;
	pos = 0;
}

Bound::Bound(int pos, int idx) {
	this->idx = idx;
	this->pos = pos;
}

Bound::~Bound() {
}

Layer::Layer() {
	draw = -1;
	label = -1;
	pin = -1;
	dirty = false;
}

Layer::Layer(int draw, int label, int pin) {
	this->draw = draw;
	this->label = label;
	this->pin = pin;
	this->dirty = false;
}

Layer::~Layer() {
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
	int index = (int)geo.size();
	geo.reserve(geo.size()+rects.size());
	geo.insert(geo.end(), rects.begin(), rects.end());
	dirty = dirty or not doSync;
	if (not dirty) {
		for (int i = 0; i < 4; i++) {
			int sz = (int)bound[i].size();
			bound[i].reserve(bound[i].size()+rects.size());
			for (int j = 0; j < (int)rects.size(); j++) {
				bound[i].push_back(Bound(rects[j][i], index+j));
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

void Layer::merge(bool doSync) {
	// TODO use the bounds array to improve performance
	for (int i = (int)geo.size()-1; i >= 0; i--) {
		for (int j = (int)geo.size()-1; j > i; j--) {
			if (geo[i].merge(geo[j])) {
				erase(j, doSync);
			}
		}
	}
}

void Layer::emit(const Tech &tech, const Layout &layout, gdstk::Cell *cell) const {
	for (auto r = geo.begin(); r != geo.end(); r++) {
		cell->polygon_array.append(r->emit(tech, draw));
		if (r->hasLabel()) {
			cell->label_array.append(r->emitLabel(tech, layout, label));
		}
	}
}

bool operator<(const Layer &l0, const Layer &l1) {
	return l0.draw < l1.draw;
}

bool operator<(const Layer &l0, int id) {
	return l0.draw < id;
}

Layout::Layout() {
}

Layout::~Layout() {
}

void Layout::updateBox(vec2i ll, vec2i ur) {
	if (ll[0] < box.ll[0]) {
		box.ll[0] = ll[0];
	}

	if (ll[1] < box.ll[1]) {
		box.ll[1] = ll[1];
	}

	if (ll[0] > box.ur[0]) {
		box.ur[0] = ll[0];
	}

	if (ll[1] > box.ur[1]) {
		box.ur[1] = ll[1];
	}

	if (ur[0] < box.ll[0]) {
		box.ll[0] = ur[0];
	}

	if (ur[1] < box.ll[1]) {
		box.ll[1] = ur[1];
	}

	if (ur[0] > box.ur[0]) {
		box.ur[0] = ur[0];
	}

	if (ur[1] > box.ur[1]) {
		box.ur[1] = ur[1];
	}
}

void Layout::merge(bool doSync) {
	for (auto layer = layers.begin(); layer != layers.end(); layer++) {
		layer->merge(doSync);
	}
}

void Layout::push(int layerID, Rect rect, bool doSync) {
	auto layer = lower_bound(layers.begin(), layers.end(), layerID);
	if (layer == layers.end() or layer->draw != layerID) {
		layer = layers.insert(layer, Layer(layerID));
	}

	layer->push(rect, doSync);
}

void Layout::emit(const Tech &tech, gdstk::Library &lib) const {
	gdstk::Cell *cell = new gdstk::Cell();
	cell->init(name.c_str());
	for (auto layer = layers.begin(); layer != layers.end(); layer++) {
		layer->emit(tech, *this, cell);
	}

	lib.cell_array.append(cell);
}

// Compute the offset from (0,0) of the l0 geometry to (0,0) of the l1 geometry
// along axis at which l0 and l1 abut and save into offset. Require spacing on
// the opposite axis for non-intersection (default is 0). Return false if the two geometries
// will never intersect.
bool minOffset(int *offset, const Tech &tech, int axis, Layer &l0, Layer &l1, int spacing) {
	if (l0.dirty) {
		l0.sync();
	}
	if (l1.dirty) {
		l1.sync();
	}

	int result = 0;
	bool conflict = false;

	vector<int> stack[2] = {vector<int>(), vector<int>()};
	// four indices:
	// l0 from for axis
	// l0 to for axis
	// l1 from for axis
	// l1 to for axis
	vector<int> idx(4, 0);
	while (true) {
		int minValue = -1;
		int minIdx = -1;
		for (int i = 0; i < (int)idx.size(); i++) {
			if (idx[i] < (int)((i>>1) ?
				l1.bound[(axis<<1)+(i&1)].size() :
				l0.bound[(axis<<1)+(i&1)].size())) {
				int value = ((i>>1) ?
					l1.bound[(axis<<1)+(i&1)][idx[i]].pos :
					l0.bound[(axis<<1)+(i&1)][idx[i]].pos) + 2*spacing*(i&1) - spacing;
				if (i == 0 or value < minValue) {
					minValue = value;
					minIdx = i;
				}
			}
		}

		if (minIdx < 0) {
			break;
		}

		int layer = minIdx>>1;
		int isEnd = minIdx&1;
		int off = layer ?
			l1.geo[l1.bound[(axis<<1)+isEnd][idx[minIdx]].idx][((1-axis)<<1)+0] :
			l0.geo[l0.bound[(axis<<1)+isEnd][idx[minIdx]].idx][((1-axis)<<1)+1];
		auto loc = lower_bound(stack[layer].begin(), stack[layer].end(), off);
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

int minOffset(int *offset, const Tech &tech, int axis, vector<Layer> &l0, vector<Layer> &l1) {
	int result = -1;
	for (int i = 0; i < (int)l0.size();	i++) {
		// TODO(edward.bingham) get spacing rule for l0.draw to l1.draw for cross-layer spacing
		int spacing = tech.mats[l0[i].draw].minSpacing;

		for (int j = 0; j < (int)l1.size(); j++) {
			int offset;
			if (l0[i].draw == l1[j].draw and minOffset(&offset, tech, axis, l0[i], l1[j], spacing)) {
				if (result < 0 or offset < result) {
					result = offset;
				}
			}
		}
	}
	return result;
}

}
