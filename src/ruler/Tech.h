#pragma once

#include <string>
#include <vector>
#include <map>

#include "vector.h"

using namespace std;

namespace ruler {

struct Paint {
	Paint();
	Paint(string name, int major = 0, int minor = 0);
	~Paint();

	string name;
	int major;
	int minor;

	int minWidth;
	bool fill;
};

struct Material {
	Material();
	Material(int draw, int label, int pin);
	~Material();
	
	// these index into Tech::paint
	int draw;
	int label;
	int pin;
};

struct Diffusion : Material {
	Diffusion();
	Diffusion(int draw, int label, int pin, int overhangX, int overhangY);
	~Diffusion();

	int overhangX;
	int overhangY;
};

struct Model {
	Model();
	Model(int type, string name, int polyOverhang);
	~Model();

	enum {
		NMOS = 0,
		PMOS = 1,
	};
	int type;
	
	string name;

	// Start top down
	vector<Diffusion> paint;
	int polyOverhang;
};

struct Routing : Material {
	Routing();
	Routing(int draw, int label, int pin);
	~Routing();
};

struct Via : Material {
	Via();
	Via(int draw, int label, int pin, int downLevel, int upLevel, int downLo = 0, int downHi = 0, int upLo = 0, int upHi = 0);
	~Via();

	// index into Tech::wires when >= 0
	// index into Tech::models when < 0
	int downLevel;
	int upLevel;

	// asymmetric enclosure rules with index 1 being the longer of the two
	vec2i dn;
	vec2i up;	
};

struct Tech {
	Tech();
	~Tech();
	
	double dbunit;

	int boundary;
	vector<Paint> paint;
	vector<Model> models;
	vector<Via> vias;
	vector<Routing> wires;
	map<pair<int, int>, int> spacing;

	void setSpacing(int l0, int l1, int value);	
	int findSpacing(int l0, int l1) const;
	int findPaint(string name) const;
	int findModel(string name) const;
	vector<int> findVias(int downLevel, int upLevel) const;
};

}
