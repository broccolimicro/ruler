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

Rect Rect::shift(vec2i pos, vec2i dir) const {
	return Rect(net, pos+ll*dir, pos+ur*dir);
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
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			bound[i][j] = vector<Bound>();
		}
	}
}

Layer::Layer(int draw, int label, int pin) {
	this->draw = draw;
	this->label = label;
	this->pin = pin;
	this->dirty = false;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			bound[i][j] = vector<Bound>();
		}
	}
}

Layer::~Layer() {
}

void Layer::sync() {
	for (int axis = 0; axis < 2; axis++) {
		for (int fromTo = 0; fromTo < 2; fromTo++) {
			vector<Bound> &bounds = bound[axis][fromTo];

			bounds.clear();
			bounds.reserve(geo.size());
			for (int j = 0; j < (int)geo.size(); j++) {
				bounds.push_back(Bound(geo[j][fromTo][axis], j));
			}
			sort(bounds.begin(), bounds.end());
		}
	}
	dirty = false;
}

void Layer::push(Rect rect, bool doSync) {
	int index = (int)geo.size();
	geo.push_back(rect);
	dirty = dirty or not doSync;
	if (not dirty) {
		for (int axis = 0; axis < 2; axis++) {
			for (int fromTo = 0; fromTo < 2; fromTo++) {
				vector<Bound> &bounds = bound[axis][fromTo];

				auto loc = lower_bound(bounds.begin(), bounds.end(), rect[fromTo][axis]); 
				bounds.insert(loc, Bound(rect[fromTo][axis], index));
			}
		}
	}
}

void Layer::push(vector<Rect> rects, bool doSync) {
	int index = (int)geo.size();
	geo.reserve(geo.size()+rects.size());
	geo.insert(geo.end(), rects.begin(), rects.end());
	dirty = dirty or not doSync;
	if (not dirty) {
		for (int axis = 0; axis < 2; axis++) {
			for (int fromTo = 0; fromTo < 2; fromTo++) {
				vector<Bound> &bounds = bound[axis][fromTo];

				int sz = (int)bounds.size();
				bounds.reserve(bounds.size()+rects.size());
				for (int j = 0; j < (int)rects.size(); j++) {
					bounds.push_back(Bound(rects[j][fromTo][axis], index+j));
				}
				sort(bounds.begin() + sz, bounds.end());
				inplace_merge(bounds.begin(), bounds.begin()+sz, bounds.end());
			}
		}
	}
}

void Layer::erase(int idx, bool doSync) {
	geo.erase(geo.begin()+idx);
	dirty = dirty or not doSync;
	if (not dirty) {
		for (int axis = 0; axis < 2; axis++) {
			for (int fromTo = 0; fromTo < 2; fromTo++) {
				vector<Bound> &bounds = bound[axis][fromTo];

				for (int j = (int)bounds.size()-1; j >= 0; j--) {
					if (bounds[j].idx == idx) {
						bounds.erase(bounds.begin() + j);
					} else if (bounds[j].idx > idx) {
						bounds[j].idx--;
					}
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

vector<Layer>::iterator Layout::findLayer(int draw, int label, int pin) {
	auto layer = lower_bound(layers.begin(), layers.end(), draw);
	if (layer == layers.end() or layer->draw != draw) {
		layer = layers.insert(layer, Layer(draw, label, pin));
	}
	return layer;
}

void Layout::merge(bool doSync) {
	for (auto layer = layers.begin(); layer != layers.end(); layer++) {
		layer->merge(doSync);
	}
}

void Layout::push(int layerID, Rect rect, bool doSync) {
	auto layer = findLayer(layerID, layerID);
	layer->push(rect, doSync);
}

void Layout::push(int layerID, vector<Rect> rects, bool doSync) {
	findLayer(layerID, layerID)->push(rects, doSync);
}

void Layout::emit(const Tech &tech, gdstk::Library &lib) const {
	gdstk::Cell *cell = new gdstk::Cell();
	cell->init(name.c_str());
	for (auto layer = layers.begin(); layer != layers.end(); layer++) {
		layer->emit(tech, *this, cell);
	}

	lib.cell_array.append(cell);
}

struct StackElem {
	StackElem() {
		net = -1;
		pos = 0;
	}
	StackElem(int net, int pos) {
		this->net = net;
		this->pos = pos;
	}
	~StackElem() {}

	int net;
	int pos;
};

bool operator==(const StackElem &e0, const StackElem &e1) {
	return e0.pos == e1.pos and e0.net == e1.net;
}

bool operator<(const StackElem &e0, const StackElem &e1) {
	return e0.pos < e1.pos or (e0.pos == e1.pos and e0.net < e1.net);
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

	// indexed as [layer]
	vector<StackElem> stack[2] = {vector<StackElem>(), vector<StackElem>()};
	// indexed as [layer][fromTo]
	int idx[2][2] = {{0, 0}, {0, 0}};
	while (true) {
		// We're simultaneously iterating over four arrays in sorted order. So we
		// need to find the index that points to the minimal element in the sort
		// order and then increment it.
		int minValue = -1;
		int minLayer = -1;
		int minFromTo = -1;
		for (int layer = 0; layer < 2; layer++) {
			for (int fromTo = 0; fromTo < 2; fromTo++) {
				int boundIdx = idx[layer][fromTo];
				const vector<Bound> &bounds = layer ? l1.bound[1-axis][fromTo] : l0.bound[1-axis][fromTo];

				if (boundIdx < (int)bounds.size()) {
					int value = bounds[boundIdx].pos + 2*spacing*fromTo - spacing;
					if (minLayer < 0 or value < minValue) {
						minValue = value;
						minLayer = layer;
						minFromTo = fromTo;
					}
				}
			}
		}

		if (minLayer < 0) {
			break;
		}

		int boundIdx = idx[minLayer][minFromTo];
		const Bound &bound = minLayer ? l1.bound[1-axis][minFromTo][boundIdx] : l0.bound[1-axis][minFromTo][boundIdx];
		Rect rect = minLayer ? l1.geo[bound.idx] : l0.geo[bound.idx];

		// Since we're measuring distance from layer 0 to layer 1, then we need to
		// look at layer 0's to boundary and layer 1's from boundary. From is index
		// 0 and to is index 1, so we need 1-layer.
		StackElem elem(rect.net, rect[1-minLayer][axis]);

		auto loc = lower_bound(stack[minLayer].begin(), stack[minLayer].end(), elem);
		if (minFromTo) {
			// When the index found is the end of a rectangle, we remove that bound
			// from the stack.
			if (loc != stack[minLayer].end() and *loc == elem) {
				stack[minLayer].erase(loc);
			}
		} else {
			// When the index found is the start of a rectangle, then we add that
			// rectangle to the associated stack.
			stack[minLayer].insert(loc, elem);
			// Then we need to check the distances to the opposite layer along the
			// opposite axis. We need to compute the distances from left to right or
			// top to bottom
			if (minLayer == 0) {
				// from layer 0 to layer 1
				for (int i = 0; i < (int)stack[1].size(); i++) {
					if (l0.draw != l1.draw or stack[1][i].net != elem.net) {
						int diff = elem.pos + spacing - stack[1][i].pos;
						if (not conflict or diff > result) {
							result = diff;
							conflict = true;
						}
						break;
					}
				}
			} else if (minLayer == 1) {
				// from layer 1 to layer 0
				for (int i = (int)stack[0].size()-1; i >= 0; i--) {
					if (l0.draw != l1.draw or stack[0][i].net != elem.net) {
						int diff = stack[0][i].pos + spacing - elem.pos;
						if (not conflict or diff > result) {
							result = diff;
							conflict = true;
						}
						break;
					}
				}
			}
		}

		idx[minLayer][minFromTo]++;
	}

	if (conflict) {
		*offset = result;
	}
	return conflict;
}

bool minOffset(int *offset, const Tech &tech, int axis, vector<Layer> &l0, vector<Layer> &l1) {
	bool conflict = false;
	int result = 0;
	for (int i = 0; i < (int)l0.size();	i++) {
		for (int j = 0; j < (int)l1.size(); j++) {
			int spacing = tech.findSpacing(l0[i].draw, l1[j].draw);
			printf("%d->%d: %d\n", l0[i].draw, l1[i].draw, spacing);
			int layerResult = 0;
			if (spacing >= 0 and minOffset(&layerResult, tech, axis, l0[i], l1[j], spacing)) {
				if (not conflict or layerResult > result) {
					result = layerResult;
					conflict = true;
				}
			}
		}
	}
	if (conflict) {
		*offset = result;
	}
	return conflict;
}

}
