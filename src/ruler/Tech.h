#pragma once

#include <string>
#include <vector>

using namespace std;

namespace ruler {

struct Material {
	Material();
	Material(string name, int major = 0, int minor = 0);
	~Material();

	string name;
	int major;
	int minor;

	int minSpacing;
	int minWidth;
};

struct Diffusion {
	Diffusion();
	Diffusion(int layer, int overhangX, int overhangY);
	~Diffusion();

	// these index into Tech::mats
	int layer;

	int overhangX;
	int overhangY;
};

struct Model {
	Model();
	Model(int type, string name, int viaPolySpacing, int polyOverhang);
	~Model();

	enum {
		NMOS = 0,
		PMOS = 1,
	};
	int type;
	
	string name;

	// Start top down
	vector<Diffusion> mats;
	int viaPolySpacing;
	int polyOverhang;
};

struct Routing {
	Routing();
	Routing(int drawing, int pin, int label);
	~Routing();
	
	// index into Tech::mats
	int drawing;
	int pin;
	int label;
};

struct Via {
	Via();
	Via(int downLevel, int upLevel, int layer, int downLo = 0, int downHi = 0, int upLo = 0, int upHi = 0);
	~Via();

	// index into Tech::wires when >= 0
	// index into Tech::models when < 0
	int downLevel;
	int upLevel;
	
	// index into Tech::mats
	int drawing;

	int downLo;
	int downHi;
	int upLo;
	int upHi;
};

struct Solution;
struct Index;

struct Tech {
	Tech();
	~Tech();
	
	double dbunit;

	int boundary;
	vector<Material> mats;
	vector<Model> models;
	vector<Via> vias;
	vector<Routing> wires;
	
	int findMat(string name) const;
	int findModel(string name) const;
	vector<int> findVias(int downLevel, int upLevel) const;
};

}
