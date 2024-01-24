#include "Tech.h"

namespace ruler {

Material::Material() {
	this->name = "";
	this->major = 0;
	this->minor = 0;
	this->minWidth = 0;
}

Material::Material(string name, int major, int minor) {
	this->name = name;
	this->major = major;
	this->minor = minor;
	this->minWidth = 0;
}

Material::~Material() {
}

Diffusion::Diffusion() {
	layer = -1;
	overhangX = 0;
	overhangY = 0;
}

Diffusion::Diffusion(int layer, int overhangX, int overhangY) {
	this->layer = layer;
	this->overhangX = overhangX;
	this->overhangY = overhangY;
}

Diffusion::~Diffusion() {
}

Model::Model() {
	name = "";
	type = NMOS;
	polyOverhang = 0;
}

Model::Model(int type, string name, int polyOverhang) {
	this->name = name;
	this->type = type;
	this->polyOverhang = polyOverhang;
}

Model::~Model() {
}

Routing::Routing() {
	drawing = -1;
	pin = -1;
	label = -1;
}

Routing::Routing(int drawing, int pin, int label) {
	this->drawing = drawing;
	this->pin = pin;
	this->label = label;
}

Routing::~Routing() {
}

Via::Via() {
	upLevel = 0;
	downLevel = 0;
	drawing = -1;
	downLo = 0;
	downHi = 0;
	upLo = 0;
	upHi = 0;
}

Via::Via(int downLevel, int upLevel, int layer, int downLo, int downHi, int upLo, int upHi) {
	this->downLevel = downLevel;
	this->upLevel = upLevel;
	this->drawing = layer;
	this->downLo = downLo;
	this->downHi = downHi;
	this->upLo = upLo;
	this->upHi = upHi;
}

Via::~Via() {
}

Tech::Tech() {
	dbunit = 5e-3;

	// TODO(edward.bingham) hardcoding tech configuration values for now, but
	// this should be parsed in from python
	mats.push_back(Material("diff.drawing", 65, 20));
	mats.back().minWidth = 30;
	mats.push_back(Material("tap.drawing", 65, 44));
	mats.push_back(Material("nwell.drawing", 64, 20));
	mats.push_back(Material("dnwell.drawing", 64, 18));
	mats.push_back(Material("pwbm.drawing", 19, 44));
	mats.push_back(Material("pwde.drawing", 124, 20));
	mats.push_back(Material("hvtr.drawing", 18, 20));
	mats.push_back(Material("hvtp.drawing", 78, 44));
	mats.back().minWidth = 76;
	mats.push_back(Material("ldntm.drawing", 11, 44));
	mats.push_back(Material("hvi.drawing", 75, 20));
	mats.push_back(Material("tunm.drawing", 80, 20));
	mats.push_back(Material("lvtn.drawing", 125, 44));
	mats.push_back(Material("poly.drawing", 66, 20));
	mats.back().minWidth = 30;
	mats.push_back(Material("hvntm.drawing", 125, 20));
	mats.push_back(Material("nsdm.drawing", 93, 44));
	mats.push_back(Material("psdm.drawing", 94, 20));
	mats.push_back(Material("rpm.drawing", 86, 20));
	mats.push_back(Material("urpm.drawing", 79, 20));
	mats.push_back(Material("npc.drawing", 95, 20));
	mats.push_back(Material("licon1.drawing", 66, 44));
	mats.back().minWidth = 34;
	mats.push_back(Material("li1.drawing", 67, 20));
	mats.back().minWidth = 34;
	mats.push_back(Material("mcon.drawing", 67, 44));
	mats.back().minWidth = 34;
	mats.push_back(Material("met1.drawing", 68, 20));
	mats.back().minWidth = 28;
	mats.push_back(Material("via.drawing", 68, 44));
	mats.back().minWidth = 30;
	mats.push_back(Material("met2.drawing", 69, 20));
	mats.back().minWidth = 28;
	mats.push_back(Material("via2.drawing", 69, 44));
	mats.back().minWidth = 40;
	mats.push_back(Material("met3.drawing", 70, 20));
	mats.back().minWidth = 60;
	mats.push_back(Material("via3.drawing", 70, 44));
	mats.back().minWidth = 40;
	mats.push_back(Material("met4.drawing", 71, 20));
	mats.back().minWidth = 60;
	mats.push_back(Material("via4.drawing", 71, 44));
	mats.back().minWidth = 160;
	mats.push_back(Material("met5.drawing", 72, 20));
	mats.back().minWidth = 320;
	mats.push_back(Material("pad.drawing", 76, 20));
	mats.push_back(Material("nsm.drawing", 61, 20));
	mats.push_back(Material("capm.drawing", 89, 44));
	mats.push_back(Material("cap2m.drawing", 97, 44));
	mats.push_back(Material("vhvi.drawing", 74, 21));
	mats.push_back(Material("uhvi.drawing", 74, 22));
	mats.push_back(Material("npn.drawing", 82, 20));
	mats.push_back(Material("inductor.drawing", 82, 24));
	mats.push_back(Material("capacitor.drawing", 82, 64));
	mats.push_back(Material("pnp.drawing", 82, 44));
	mats.push_back(Material("lvsPrune.drawing", 84, 44));
	mats.push_back(Material("ncm.drawing", 92, 44));
	mats.push_back(Material("padcenter.drawing", 81, 20));
	mats.push_back(Material("target.drawing", 76, 44));
	mats.push_back(Material("areaid.sl.identifier", 81, 1));
	mats.push_back(Material("areaid.ce.identifier", 81, 2));
	mats.push_back(Material("areaid.fe.identifier", 81, 3));
	mats.push_back(Material("areaid.sc.identifier", 81, 4));
	mats.push_back(Material("areaid.sf.identifier", 81, 6));
	mats.push_back(Material("areaid.sl.identifier1", 81, 7));
	mats.push_back(Material("areaid.sr.identifier", 81, 8));
	mats.push_back(Material("areaid.mt.identifier", 81, 10));
	mats.push_back(Material("areaid.dt.identifier", 81, 11));
	mats.push_back(Material("areaid.ft.identifier", 81, 12));
	mats.push_back(Material("areaid.ww.identifier", 81, 13));
	mats.push_back(Material("areaid.ld.identifier", 81, 14));
	mats.push_back(Material("areaid.ns.identifier", 81, 15));
	mats.push_back(Material("areaid.ij.identifier", 81, 17));
	mats.push_back(Material("areaid.zr.identifier", 81, 18));
	mats.push_back(Material("areaid.ed.identifier", 81, 19));
	mats.push_back(Material("areaid.de.identifier", 81, 23));
	mats.push_back(Material("areaid.rd.identifier", 81, 24));
	mats.push_back(Material("areaid.dn.identifier", 81, 50));
	mats.push_back(Material("areaid.cr.identifier", 81, 51));
	mats.push_back(Material("areaid.cd.identifier", 81, 52));
	mats.push_back(Material("areaid.st.identifier", 81, 53));
	mats.push_back(Material("areaid.op.identifier", 81, 54));
	mats.push_back(Material("areaid.en.identifier", 81, 57));
	mats.push_back(Material("areaid.en20.identifier", 81, 58));
	mats.push_back(Material("areaid.le.identifier", 81, 60));
	mats.push_back(Material("areaid.hl.identifier", 81, 63));
	mats.push_back(Material("areaid.sd.identifier", 81, 70));
	mats.push_back(Material("areaid.po.identifier", 81, 81));
	mats.push_back(Material("areaid.it.identifier", 81, 84));
	mats.push_back(Material("areaid.et.identifier", 81, 101));
	mats.push_back(Material("areaid.lvt.identifier", 81, 108));
	mats.push_back(Material("areaid.re.identifier", 81, 125));
	mats.push_back(Material("fom.dummy", 22, 23));
	mats.push_back(Material("poly.gate", 66, 9));
	mats.push_back(Material("poly.model", 66, 83));
	mats.push_back(Material("poly.resistor", 66, 13));
	mats.push_back(Material("diff.resistor", 65, 13));
	mats.push_back(Material("pwell.resistor", 64, 13));
	mats.push_back(Material("li1.resistor", 67, 13));
	mats.push_back(Material("diff.highVoltage", 65, 8));
	mats.push_back(Material("met4.fuse", 71, 17));
	mats.push_back(Material("inductor.terminal1", 82, 26));
	mats.push_back(Material("inductor.terminal2", 82, 27));
	mats.push_back(Material("inductor.terminal3", 82, 28));
	mats.push_back(Material("li1.block", 67, 10));
	mats.push_back(Material("met1.block", 68, 10));
	mats.push_back(Material("met2.block", 69, 10));
	mats.push_back(Material("met3.block", 70, 10));
	mats.push_back(Material("met4.block", 71, 10));
	mats.push_back(Material("met5.block", 72, 10));
	mats.push_back(Material("prbndry.boundary", 235, 4));
	mats.push_back(Material("diff.boundary", 65, 4));
	mats.push_back(Material("tap.boundary", 65, 60));
	mats.push_back(Material("mcon.boundary", 67, 60));
	mats.push_back(Material("poly.boundary", 66, 4));
	mats.push_back(Material("via.boundary", 68, 60));
	mats.push_back(Material("via2.boundary", 69, 60));
	mats.push_back(Material("via3.boundary", 70, 60));
	mats.push_back(Material("via4.boundary", 71, 60));
	mats.push_back(Material("li1.label", 67, 5));
	mats.push_back(Material("met1.label", 68, 5));
	mats.push_back(Material("met2.label", 69, 5));
	mats.push_back(Material("met3.label", 70, 5));
	mats.push_back(Material("met4.label", 71, 5));
	mats.push_back(Material("met5.label", 72, 5));
	mats.push_back(Material("poly.label", 66, 5));
	mats.push_back(Material("diff.label", 65, 6));
	mats.push_back(Material("pwell.label", 64, 59));
	mats.push_back(Material("pwelliso.label", 44, 5));
	mats.push_back(Material("pad.label", 76, 5));
	mats.push_back(Material("tap.label", 65, 5));
	mats.push_back(Material("nwell.label", 64, 5));
	mats.push_back(Material("inductor.label", 82, 25));
	mats.push_back(Material("text.label", 83, 44));
	mats.push_back(Material("li1.net", 67, 23));
	mats.push_back(Material("met1.net", 68, 23));
	mats.push_back(Material("met2.net", 69, 23));
	mats.push_back(Material("met3.net", 70, 23));
	mats.push_back(Material("met4.net", 71, 23));
	mats.push_back(Material("met5.net", 72, 23));
	mats.push_back(Material("poly.net", 66, 23));
	mats.push_back(Material("diff.net", 65, 23));
	mats.push_back(Material("li1.pin", 67, 16));
	mats.push_back(Material("met1.pin", 68, 16));
	mats.push_back(Material("met2.pin", 69, 16));
	mats.push_back(Material("met3.pin", 70, 16));
	mats.push_back(Material("met4.pin", 71, 16));
	mats.push_back(Material("met5.pin", 72, 16));
	mats.push_back(Material("poly.pin", 66, 16));
	mats.push_back(Material("diff.pin", 65, 16));
	mats.push_back(Material("nwell.pin", 64, 16));
	mats.push_back(Material("pad.pin", 76, 16));
	mats.push_back(Material("pwell.pin", 122, 16));
	mats.push_back(Material("pwelliso.pin", 44, 16));
	mats.push_back(Material("nwell.pin1", 64, 0));
	mats.push_back(Material("poly.pin1", 66, 0));
	mats.push_back(Material("li1.pin1", 67, 0));
	mats.push_back(Material("met1.pin1", 68, 0));
	mats.push_back(Material("met2.pin1", 69, 0));
	mats.push_back(Material("met3.pin1", 70, 0));
	mats.push_back(Material("met4.pin1", 71, 0));
	mats.push_back(Material("met5.pin1", 72, 0));
	mats.push_back(Material("pad.pin1", 76, 0));
	mats.push_back(Material("pwell.pin1", 122, 0));
	mats.push_back(Material("diff.cut", 65, 14));
	mats.push_back(Material("poly.cut", 66, 14));
	mats.push_back(Material("li1.cut", 67, 14));
	mats.push_back(Material("met1.cut", 68, 14));
	mats.push_back(Material("met2.cut", 69, 14));
	mats.push_back(Material("met3.cut", 70, 14));
	mats.push_back(Material("met4.cut", 71, 14));
	mats.push_back(Material("met5.cut", 72, 14));
	mats.push_back(Material("met5.probe", 72, 25));
	mats.push_back(Material("met4.probe", 71, 25));
	mats.push_back(Material("met3.probe", 70, 25));
	mats.push_back(Material("met2.probe", 69, 25));
	mats.push_back(Material("met1.probe", 68, 25));
	mats.push_back(Material("li1.probe", 67, 25));
	mats.push_back(Material("poly.probe", 66, 25));
	mats.push_back(Material("poly.short", 66, 15));
	mats.push_back(Material("li1.short", 67, 15));
	mats.push_back(Material("met1.short", 68, 15));
	mats.push_back(Material("met2.short", 69, 15));
	mats.push_back(Material("met3.short", 70, 15));
	mats.push_back(Material("met4.short", 71, 15));
	mats.push_back(Material("met5.short", 72, 15));
	mats.push_back(Material("cncm.mask", 17, 0));
	mats.push_back(Material("crpm.mask", 96, 0));
	mats.push_back(Material("cpdm.mask", 37, 0));
	mats.push_back(Material("cnsm.mask", 22, 0));
	mats.push_back(Material("cmm5.mask", 59, 0));
	mats.push_back(Material("cviam4.mask", 58, 0));
	mats.push_back(Material("cmm4.mask", 51, 0));
	mats.push_back(Material("cviam3.mask", 50, 0));
	mats.push_back(Material("cmm3.mask", 34, 0));
	mats.push_back(Material("cviam2.mask", 44, 0));
	mats.push_back(Material("cmm2.mask", 41, 0));
	mats.push_back(Material("cviam.mask", 40, 0));
	mats.push_back(Material("cmm1.mask", 36, 0));
	mats.push_back(Material("ctm1.mask", 35, 0));
	mats.push_back(Material("cli1m.mask", 56, 0));
	mats.push_back(Material("clicm1.mask", 43, 0));
	mats.push_back(Material("cpsdm.mask", 32, 0));
	mats.push_back(Material("cnsdm.mask", 30, 0));
	mats.push_back(Material("cldntm.mask", 11, 0));
	mats.push_back(Material("cnpc.mask", 49, 0));
	mats.push_back(Material("chvntm.mask", 39, 0));
	mats.push_back(Material("cntm.mask", 27, 0));
	mats.push_back(Material("cp1m.mask", 28, 0));
	mats.push_back(Material("clvom.mask", 46, 0));
	mats.push_back(Material("conom.mask", 88, 0));
	mats.push_back(Material("ctunm.mask", 20, 0));
	mats.push_back(Material("chvtrm.mask", 98, 0));
	mats.push_back(Material("chvtpm.mask", 97, 0));
	mats.push_back(Material("clvtnm.mask", 25, 0));
	mats.push_back(Material("cnwm.mask", 21, 0));
	mats.push_back(Material("cdnm.mask", 48, 0));
	mats.push_back(Material("cfom.mask", 23, 0));
	mats.push_back(Material("cfom.drawing", 22, 20));
	mats.push_back(Material("clvtnm.drawing", 25, 44));
	mats.push_back(Material("chvtpm.drawing", 88, 44));
	mats.push_back(Material("conom.drawing", 87, 44));
	mats.push_back(Material("clvom.drawing", 45, 20));
	mats.push_back(Material("cntm.drawing", 26, 20));
	mats.push_back(Material("chvntm.drawing", 38, 20));
	mats.push_back(Material("cnpc.drawing", 44, 20));
	mats.push_back(Material("cnsdm.drawing", 29, 20));
	mats.push_back(Material("cpsdm.drawing", 31, 20));
	mats.push_back(Material("cli1m.drawing", 115, 44));
	mats.push_back(Material("cviam3.drawing", 112, 20));
	mats.push_back(Material("cviam4.drawing", 117, 20));
	mats.push_back(Material("cncm.drawing", 96, 44));
	mats.push_back(Material("cntm.maskAdd", 26, 21));
	mats.push_back(Material("clvtnm.maskAdd", 25, 43));
	mats.push_back(Material("chvtpm.maskAdd", 97, 43));
	mats.push_back(Material("cli1m.maskAdd", 115, 43));
	mats.push_back(Material("clicm1.maskAdd", 106, 43));
	mats.push_back(Material("cpsdm.maskAdd", 31, 21));
	mats.push_back(Material("cnsdm.maskAdd", 29, 21));
	mats.push_back(Material("cp1m.maskAdd", 33, 43));
	mats.push_back(Material("cfom.maskAdd", 22, 21));
	mats.push_back(Material("cntm.maskDrop", 26, 22));
	mats.push_back(Material("clvtnm.maskDrop", 25, 42));
	mats.push_back(Material("chvtpm.maskDrop", 97, 42));
	mats.push_back(Material("cli1m.maskDrop", 115, 42));
	mats.push_back(Material("clicm1.maskDrop", 106, 42));
	mats.push_back(Material("cpsdm.maskDrop", 31, 22));
	mats.push_back(Material("cnsdm.maskDrop", 29, 22));
	mats.push_back(Material("cp1m.maskDrop", 33, 42));
	mats.push_back(Material("cfom.maskDrop", 22, 22));
	mats.push_back(Material("cmm4.waffleDrop", 112, 4));
	mats.push_back(Material("cmm3.waffleDrop", 107, 24));
	mats.push_back(Material("cmm2.waffleDrop", 105, 52));
	mats.push_back(Material("cmm1.waffleDrop", 62, 24));
	mats.push_back(Material("cp1m.waffleDrop", 33, 24));
	mats.push_back(Material("cfom.waffleDrop", 22, 24));
	mats.push_back(Material("cmm5.waffleDrop", 117, 4));

	models.push_back(Model(Model::NMOS, "sky130_fd_pr__nfet_01v8", 26));
	models.back().mats.push_back(Diffusion(findMat("diff.drawing"), 50, 0));
	models.back().mats.push_back(Diffusion(findMat("nsdm.drawing"), 25, 25));
	models.push_back(Model(Model::PMOS, "sky130_fd_pr__pfet_01v8", 26));
	models.back().mats.push_back(Diffusion(findMat("diff.drawing"), 50, 0));
	models.back().mats.push_back(Diffusion(findMat("psdm.drawing"), 25, 25));
	models.back().mats.push_back(Diffusion(findMat("nwell.drawing"), 36, 36));
	models.push_back(Model(Model::PMOS, "sky130_fd_pr__pfet_01v8_hvt", 26));
	models.back().mats.push_back(Diffusion(findMat("diff.drawing"), 50, 0));
	models.back().mats.push_back(Diffusion(findMat("psdm.drawing"), 25, 25));
	models.back().mats.push_back(Diffusion(findMat("hvtp.drawing"), 11, 11));
	models.back().mats.push_back(Diffusion(findMat("nwell.drawing"), 36, 36));


	vias.push_back(Via(-1, 1, findMat("licon1.drawing"), 8, 12, 0, 16));
	vias.push_back(Via(-2, 1, findMat("licon1.drawing"), 8, 12, 0, 16));
	vias.push_back(Via(-3, 1, findMat("licon1.drawing"), 8, 12, 0, 16));

	vias.push_back(Via(0, 1, findMat("licon1.drawing"), 10, 16, 0, 16));
	vias.push_back(Via(1, 2, findMat("mcon.drawing"), 0, 0, 6, 12));
	vias.push_back(Via(2, 3, findMat("via.drawing"), 11, 11, 11, 17));
	vias.push_back(Via(3, 4, findMat("via2.drawing"), 8, 17, 13, 13));
	vias.push_back(Via(4, 5, findMat("via3.drawing"), 12, 18, 13, 13));
	vias.push_back(Via(5, 6, findMat("via4.drawing"), 38, 38, 62, 62));

	wires.push_back(Routing(findMat("poly.drawing"), findMat("poly.pin"), findMat("poly.label")));
	wires.push_back(Routing(findMat("li1.drawing"), findMat("li1.pin"), findMat("li1.label")));
	wires.push_back(Routing(findMat("met1.drawing"), findMat("met1.pin"), findMat("met1.label")));
	wires.push_back(Routing(findMat("met2.drawing"), findMat("met2.pin"), findMat("met2.label")));
	wires.push_back(Routing(findMat("met3.drawing"), findMat("met3.pin"), findMat("met3.label")));
	wires.push_back(Routing(findMat("met4.drawing"), findMat("met4.pin"), findMat("met4.label")));
	wires.push_back(Routing(findMat("met5.drawing"), findMat("met5.pin"), findMat("met5.label")));

	setSpacing(findMat("diff.drawing"), findMat("diff.drawing"), 54);
	setSpacing(findMat("hvtp.drawing"), findMat("hvtp.drawing"), 76);
	setSpacing(findMat("poly.drawing"), findMat("poly.drawing"), 42);
	setSpacing(findMat("licon1.drawing"), findMat("licon1.drawing"), 34);
	setSpacing(findMat("li1.drawing"), findMat("li1.drawing"), 34);
	setSpacing(findMat("mcon.drawing"), findMat("mcon.drawing"), 38);
	setSpacing(findMat("met1.drawing"), findMat("met1.drawing"), 28);
	setSpacing(findMat("via.drawing"), findMat("via.drawing"), 34);
	setSpacing(findMat("met2.drawing"), findMat("met2.drawing"), 28);
	setSpacing(findMat("via2.drawing"), findMat("via2.drawing"), 40);
	setSpacing(findMat("met3.drawing"), findMat("met3.drawing"), 60);
	setSpacing(findMat("via3.drawing"), findMat("via3.drawing"), 40);
	setSpacing(findMat("met4.drawing"), findMat("met4.drawing"), 60);
	setSpacing(findMat("via4.drawing"), findMat("via4.drawing"), 160);
	setSpacing(findMat("met5.drawing"), findMat("met5.drawing"), 320);
	setSpacing(findMat("poly.drawing"), findMat("licon1.drawing"), 18);

	boundary = findMat("areaid.sc.identifier");
}

Tech::~Tech() {
}

void Tech::setSpacing(int l0, int l1, int value) {
	auto result = spacing.insert(pair<pair<int, int>, int>(pair<int, int>(min(l0,l1), max(l0,l1)), value));
	if (not result.second) {
		result.first->second = value;
	}
}

int Tech::findSpacing(int l0, int l1) const {
	auto i = spacing.find(pair<int, int>(min(l0,l1), max(l0,l1)));
	if (i != spacing.end()) {
		return i->second;
	}
	return -1;
}

int Tech::findMat(string name) const {
	for (int i = 0; i < (int)mats.size(); i++) {
		if (mats[i].name == name) {
			return i;
		}
	}

	return -1;
}

int Tech::findModel(string name) const {
	for (int i = 0; i < (int)models.size(); i++) {
		if (models[i].name == name) {
			return i;
		}
	}

	return -1;
}

vector<int> Tech::findVias(int downLevel, int upLevel) const {
	int curr = downLevel;
	
	vector<int> result;
	bool done = false;
	while (not done) {
		done = true;
		for (int i = 0; curr != upLevel and i < (int)vias.size(); i++) {
			if (vias[i].downLevel == curr) {
				result.push_back(i);
				curr = vias[i].upLevel;
				done = false;
			}
		}
	}
	return result;
}

// TODO(edward.bingham) instead of manually computing spacing for different situations, we should pre-layout each transistor, contact, and wire, and then use the DRC rules directly on the geometry to compute spacing. This would ensure that every cell is guaranteed to be DRC error free.

}
