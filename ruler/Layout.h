#pragma once

#include <vector>
#include <array>
#include <gdstk/gdstk.hpp>

#include "vector.h"
#include "Tech.h"

using namespace std;

namespace ruler {

struct Layout;

struct Rect {
	Rect();
	Rect(int net, vec2i ll, vec2i ur);
	~Rect();

	int net;
	// locations in db units of lower left corner and upper right corner
	// respectively
	vec2i ll;
	vec2i ur;

	vec2i &operator[](int corner) {
		return corner ? ur : ll;
	}

	Rect shift(vec2i pos, vec2i dir=vec2i(1,1)) const;
	bool merge(Rect r);
	bool overlaps(Rect r);
	bool hasLabel() const;
	Rect &bound(vec2i rll, vec2i rur);
	Rect &bound(Rect r);

	gdstk::Polygon *emit(const Tech &tech, int layer) const;
	gdstk::Label *emitLabel(const Tech &tech, const Layout &layout, int layer) const;
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

struct Layer {
	Layer();
	Layer(const Tech &tech, int draw, int label = -1, int pin = -1);
	~Layer();

	enum {
		UNKNOWN = -1,
	};

	// this is the source of truth
	// index into layer stack defined in Tech
	int draw;
	int label;
	int pin;
	vector<Rect> geo;

	// flags to help pull apart spacing rules for
	// cell construction
	bool isRouting;
	bool isSubstrate;
	
	/////////////////////////////////////////////
	// these optimize performance in the minOffset
	// computation
	bool dirty;
	
	// indexed as [axis][fromTo]
	array<array<vector<Bound>, 2>, 2> bound;

	////////////////////////////////////////////

	bool isFill(const Tech &tech);

	void clear();
	void sync();

	void push(Rect rect, bool doSync=false);
	void push(vector<Rect> rects, bool doSync=false);
	void erase(int index, bool doSync=false);

	Rect bbox();
	void merge(bool doSync=false);
	
	void emit(const Layout &layout, gdstk::Cell *cell) const;
};

bool operator<(const Layer &l0, const Layer &l1);
bool operator<(const Layer &l, int id);

Layer operator&(const Layer &l0, const Layer &l1);
Layer interact(const Layer &l0, const Layer &l1);
Layer not_interact(const Layer &l0, const Layer &l1);
Layer operator|(const Layer &l0, const Layer &l1);
Layer operator~(const Layer &l);

struct Evaluation {
	Evaluation();
	Evaluation(Layout &layout);
	~Evaluation();

	Layout *layout;
	// negative index into Tech::rules -> geometry
	map<int, Layer> layers;

	// negative index into Tech::rules -> count of ready operands in layers
	map<int, int> incomplete;

	void init();
	bool has(int idx);
	Layer &at(int idx);
	void evaluate();
};

struct Layout {
	//Layout();
	Layout(const Tech &tech);
	~Layout();

	enum {
		DEFAULT = 0,
		MERGENET = 1,
		IGNORE = 2,
	};

	const Tech *tech;

	string name;
	Rect box;
	vector<string> nets;
	vector<Layer> layers;
	
	vector<Layer>::iterator findLayer(int draw, int layer=-1, int pin=-1);
	void push(int layer, Rect rect, bool doSync=false);
	void push(int layer, vector<Rect> rects, bool doSync=false);
	void push(const Material &mat, Rect rect, bool doSync=false);
	void push(const Material &mat, vector<Rect> rects, bool doSync=false);

	Rect bbox();
	void merge(bool doSync=false);

	void clear();
	void emit(gdstk::Library &lib) const;
};

bool minOffset(int *offset, const Tech &tech, int axis, Layer &l0, int l0Shift, Layer &l1, int l1Shift, int spacing=0, bool mergeNet=true);
bool minOffset(int *offset, const Tech &tech, int axis, Layout &left, int leftShift, Layout &right, int rightShift, int substrateMode=Layout::DEFAULT, int routingMode=Layout::DEFAULT);

}
