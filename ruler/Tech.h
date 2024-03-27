#pragma once

#include <string>
#include <vector>
#include <map>

#include "vector.h"

using namespace std;

int flip(int idx);

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

	// negative index into Tech::rules
	vector<int> out;
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
	Diffusion(int draw, int label, int pin, vec2i overhang);
	~Diffusion();

	vec2i overhang;
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

struct Rule {
	Rule();
	Rule(int type, vector<int> operands=vector<int>(), vector<int> params=vector<int>());
	~Rule();

	enum {
		NOT = 0,
		AND = 1,
		OR = 2,
		INTERACT = 3,
		NOT_INTERACT = 4,
		SPACING = 5,
	};

	int type;

	// positive operands refer to paint layers (index into Tech::paint)
	// negative operands refer to operation outputs (index into Tech::rules)
	vector<int> operands;

	// These are constant valued parameters to be used in more complex operations
	vector<int> params;

	// negative index into Tech::rules
	vector<int> out;

	bool isOperator() const;
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
	vector<Rule> rules;

	int getOr(int l0, int l1) const;
	int setOr(int l0, int l1);
	int getAnd(int l0, int l1) const;
	int setAnd(int l0, int l1);
	int getNot(int l) const;
	int setNot(int l);
	int getInteract(int l0, int l1) const;
	int setInteract(int l0, int l1);
	int getNotInteract(int l0, int l1) const;
	int setNotInteract(int l0, int l1);

	int getSpacing(int l0, int l1) const;
	int setSpacing(int l0, int l1, int value);

	string print(int layer) const;
	int findPaint(string name) const;
	int findModel(string name) const;
	vector<int> findVias(int downLevel, int upLevel) const;

	bool isRouting(int layer) const;
	bool isSubstrate(int layer) const;
};

}
