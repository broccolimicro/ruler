#include "Tech.h"

namespace ruler {

Paint::Paint() {
	this->name = "";
	this->major = 0;
	this->minor = 0;
	this->minWidth = 0;
	this->fill = false;
}

Paint::Paint(string name, int major, int minor) {
	this->name = name;
	this->major = major;
	this->minor = minor;
	this->minWidth = 0;
	this->fill = false;
}

Paint::~Paint() {
}

Material::Material() {
	draw = -1;
	label = -1;
	pin = -1;
}

Material::Material(int draw, int label, int pin) {
	this->draw = draw;
	this->pin = pin;
	this->label = label;
}

Material::~Material() {
}

Diffusion::Diffusion() : Material() {
	overhang = vec2i(0,0);
}

Diffusion::Diffusion(int draw, int label, int pin, vec2i overhang) : Material(draw, label, pin) {
	this->overhang = overhang;
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

Routing::Routing() : Material() {
}

Routing::Routing(int draw, int label, int pin) : Material(draw, label, pin) {
}

Routing::~Routing() {
}

Via::Via() : Material() {
	upLevel = 0;
	downLevel = 0;
	dn = vec2i(0,0);
	up = vec2i(0,0);
}

Via::Via(int draw, int label, int pin, int downLevel, int upLevel, int downLo, int downHi, int upLo, int upHi) : Material(draw, label, pin) {
	this->downLevel = downLevel;
	this->upLevel = upLevel;
	this->dn = vec2i(downLo, downHi);
	this->up = vec2i(upLo, upHi);
}

Via::~Via() {
}

Tech::Tech() {
	dbunit = 5e-3;

	// TODO(edward.bingham) hardcoding tech configuration values for now, but
	// this should be parsed in from python
	paint.push_back(Paint("diff.drawing", 65, 20));
	paint.back().minWidth = 30;
	paint.push_back(Paint("tap.drawing", 65, 44));
	paint.push_back(Paint("nwell.drawing", 64, 20));
	paint.back().fill = true;
	paint.push_back(Paint("dnwell.drawing", 64, 18));
	paint.back().fill = true;
	paint.push_back(Paint("pwbm.drawing", 19, 44));
	paint.push_back(Paint("pwde.drawing", 124, 20));
	paint.push_back(Paint("hvtr.drawing", 18, 20));
	paint.push_back(Paint("hvtp.drawing", 78, 44));
	paint.back().minWidth = 76;
	paint.back().fill = true;
	paint.push_back(Paint("ldntm.drawing", 11, 44));
	paint.push_back(Paint("hvi.drawing", 75, 20));
	paint.push_back(Paint("tunm.drawing", 80, 20));
	paint.push_back(Paint("lvtn.drawing", 125, 44));
	paint.push_back(Paint("poly.drawing", 66, 20));
	paint.back().minWidth = 30;
	paint.push_back(Paint("hvntm.drawing", 125, 20));
	paint.push_back(Paint("nsdm.drawing", 93, 44));
	paint.back().fill = true;
	paint.push_back(Paint("psdm.drawing", 94, 20));
	paint.back().fill = true;
	paint.push_back(Paint("rpm.drawing", 86, 20));
	paint.push_back(Paint("urpm.drawing", 79, 20));
	paint.push_back(Paint("npc.drawing", 95, 20));
	paint.push_back(Paint("licon1.drawing", 66, 44));
	paint.back().minWidth = 34;
	paint.push_back(Paint("li1.drawing", 67, 20));
	paint.back().minWidth = 34;
	paint.push_back(Paint("mcon.drawing", 67, 44));
	paint.back().minWidth = 34;
	paint.push_back(Paint("met1.drawing", 68, 20));
	paint.back().minWidth = 28;
	paint.push_back(Paint("via.drawing", 68, 44));
	paint.back().minWidth = 30;
	paint.push_back(Paint("met2.drawing", 69, 20));
	paint.back().minWidth = 28;
	paint.push_back(Paint("via2.drawing", 69, 44));
	paint.back().minWidth = 40;
	paint.push_back(Paint("met3.drawing", 70, 20));
	paint.back().minWidth = 60;
	paint.push_back(Paint("via3.drawing", 70, 44));
	paint.back().minWidth = 40;
	paint.push_back(Paint("met4.drawing", 71, 20));
	paint.back().minWidth = 60;
	paint.push_back(Paint("via4.drawing", 71, 44));
	paint.back().minWidth = 160;
	paint.push_back(Paint("met5.drawing", 72, 20));
	paint.back().minWidth = 320;
	paint.push_back(Paint("pad.drawing", 76, 20));
	paint.push_back(Paint("nsm.drawing", 61, 20));
	paint.push_back(Paint("capm.drawing", 89, 44));
	paint.push_back(Paint("cap2m.drawing", 97, 44));
	paint.push_back(Paint("vhvi.drawing", 74, 21));
	paint.push_back(Paint("uhvi.drawing", 74, 22));
	paint.push_back(Paint("npn.drawing", 82, 20));
	paint.push_back(Paint("inductor.drawing", 82, 24));
	paint.push_back(Paint("capacitor.drawing", 82, 64));
	paint.push_back(Paint("pnp.drawing", 82, 44));
	paint.push_back(Paint("lvsPrune.drawing", 84, 44));
	paint.push_back(Paint("ncm.drawing", 92, 44));
	paint.push_back(Paint("padcenter.drawing", 81, 20));
	paint.push_back(Paint("target.drawing", 76, 44));
	paint.push_back(Paint("areaid.sl.identifier", 81, 1));
	paint.push_back(Paint("areaid.ce.identifier", 81, 2));
	paint.push_back(Paint("areaid.fe.identifier", 81, 3));
	paint.push_back(Paint("areaid.sc.identifier", 81, 4));
	paint.push_back(Paint("areaid.sf.identifier", 81, 6));
	paint.push_back(Paint("areaid.sl.identifier1", 81, 7));
	paint.push_back(Paint("areaid.sr.identifier", 81, 8));
	paint.push_back(Paint("areaid.mt.identifier", 81, 10));
	paint.push_back(Paint("areaid.dt.identifier", 81, 11));
	paint.push_back(Paint("areaid.ft.identifier", 81, 12));
	paint.push_back(Paint("areaid.ww.identifier", 81, 13));
	paint.push_back(Paint("areaid.ld.identifier", 81, 14));
	paint.push_back(Paint("areaid.ns.identifier", 81, 15));
	paint.push_back(Paint("areaid.ij.identifier", 81, 17));
	paint.push_back(Paint("areaid.zr.identifier", 81, 18));
	paint.push_back(Paint("areaid.ed.identifier", 81, 19));
	paint.push_back(Paint("areaid.de.identifier", 81, 23));
	paint.push_back(Paint("areaid.rd.identifier", 81, 24));
	paint.push_back(Paint("areaid.dn.identifier", 81, 50));
	paint.push_back(Paint("areaid.cr.identifier", 81, 51));
	paint.push_back(Paint("areaid.cd.identifier", 81, 52));
	paint.push_back(Paint("areaid.st.identifier", 81, 53));
	paint.push_back(Paint("areaid.op.identifier", 81, 54));
	paint.push_back(Paint("areaid.en.identifier", 81, 57));
	paint.push_back(Paint("areaid.en20.identifier", 81, 58));
	paint.push_back(Paint("areaid.le.identifier", 81, 60));
	paint.push_back(Paint("areaid.hl.identifier", 81, 63));
	paint.push_back(Paint("areaid.sd.identifier", 81, 70));
	paint.push_back(Paint("areaid.po.identifier", 81, 81));
	paint.push_back(Paint("areaid.it.identifier", 81, 84));
	paint.push_back(Paint("areaid.et.identifier", 81, 101));
	paint.push_back(Paint("areaid.lvt.identifier", 81, 108));
	paint.push_back(Paint("areaid.re.identifier", 81, 125));
	paint.push_back(Paint("fom.dummy", 22, 23));
	paint.push_back(Paint("poly.gate", 66, 9));
	paint.push_back(Paint("poly.model", 66, 83));
	paint.push_back(Paint("poly.resistor", 66, 13));
	paint.push_back(Paint("diff.resistor", 65, 13));
	paint.push_back(Paint("pwell.resistor", 64, 13));
	paint.push_back(Paint("li1.resistor", 67, 13));
	paint.push_back(Paint("diff.highVoltage", 65, 8));
	paint.push_back(Paint("met4.fuse", 71, 17));
	paint.push_back(Paint("inductor.terminal1", 82, 26));
	paint.push_back(Paint("inductor.terminal2", 82, 27));
	paint.push_back(Paint("inductor.terminal3", 82, 28));
	paint.push_back(Paint("li1.block", 67, 10));
	paint.push_back(Paint("met1.block", 68, 10));
	paint.push_back(Paint("met2.block", 69, 10));
	paint.push_back(Paint("met3.block", 70, 10));
	paint.push_back(Paint("met4.block", 71, 10));
	paint.push_back(Paint("met5.block", 72, 10));
	paint.push_back(Paint("prbndry.boundary", 235, 4));
	paint.push_back(Paint("diff.boundary", 65, 4));
	paint.push_back(Paint("tap.boundary", 65, 60));
	paint.push_back(Paint("mcon.boundary", 67, 60));
	paint.push_back(Paint("poly.boundary", 66, 4));
	paint.push_back(Paint("via.boundary", 68, 60));
	paint.push_back(Paint("via2.boundary", 69, 60));
	paint.push_back(Paint("via3.boundary", 70, 60));
	paint.push_back(Paint("via4.boundary", 71, 60));
	paint.push_back(Paint("li1.label", 67, 5));
	paint.push_back(Paint("met1.label", 68, 5));
	paint.push_back(Paint("met2.label", 69, 5));
	paint.push_back(Paint("met3.label", 70, 5));
	paint.push_back(Paint("met4.label", 71, 5));
	paint.push_back(Paint("met5.label", 72, 5));
	paint.push_back(Paint("poly.label", 66, 5));
	paint.push_back(Paint("diff.label", 65, 6));
	paint.push_back(Paint("pwell.label", 64, 59));
	paint.push_back(Paint("pwelliso.label", 44, 5));
	paint.push_back(Paint("pad.label", 76, 5));
	paint.push_back(Paint("tap.label", 65, 5));
	paint.push_back(Paint("nwell.label", 64, 5));
	paint.push_back(Paint("inductor.label", 82, 25));
	paint.push_back(Paint("text.label", 83, 44));
	paint.push_back(Paint("li1.net", 67, 23));
	paint.push_back(Paint("met1.net", 68, 23));
	paint.push_back(Paint("met2.net", 69, 23));
	paint.push_back(Paint("met3.net", 70, 23));
	paint.push_back(Paint("met4.net", 71, 23));
	paint.push_back(Paint("met5.net", 72, 23));
	paint.push_back(Paint("poly.net", 66, 23));
	paint.push_back(Paint("diff.net", 65, 23));
	paint.push_back(Paint("li1.pin", 67, 16));
	paint.push_back(Paint("met1.pin", 68, 16));
	paint.push_back(Paint("met2.pin", 69, 16));
	paint.push_back(Paint("met3.pin", 70, 16));
	paint.push_back(Paint("met4.pin", 71, 16));
	paint.push_back(Paint("met5.pin", 72, 16));
	paint.push_back(Paint("poly.pin", 66, 16));
	paint.push_back(Paint("diff.pin", 65, 16));
	paint.push_back(Paint("nwell.pin", 64, 16));
	paint.push_back(Paint("pad.pin", 76, 16));
	paint.push_back(Paint("pwell.pin", 122, 16));
	paint.push_back(Paint("pwelliso.pin", 44, 16));
	paint.push_back(Paint("nwell.pin1", 64, 0));
	paint.push_back(Paint("poly.pin1", 66, 0));
	paint.push_back(Paint("li1.pin1", 67, 0));
	paint.push_back(Paint("met1.pin1", 68, 0));
	paint.push_back(Paint("met2.pin1", 69, 0));
	paint.push_back(Paint("met3.pin1", 70, 0));
	paint.push_back(Paint("met4.pin1", 71, 0));
	paint.push_back(Paint("met5.pin1", 72, 0));
	paint.push_back(Paint("pad.pin1", 76, 0));
	paint.push_back(Paint("pwell.pin1", 122, 0));
	paint.push_back(Paint("diff.cut", 65, 14));
	paint.push_back(Paint("poly.cut", 66, 14));
	paint.push_back(Paint("li1.cut", 67, 14));
	paint.push_back(Paint("met1.cut", 68, 14));
	paint.push_back(Paint("met2.cut", 69, 14));
	paint.push_back(Paint("met3.cut", 70, 14));
	paint.push_back(Paint("met4.cut", 71, 14));
	paint.push_back(Paint("met5.cut", 72, 14));
	paint.push_back(Paint("met5.probe", 72, 25));
	paint.push_back(Paint("met4.probe", 71, 25));
	paint.push_back(Paint("met3.probe", 70, 25));
	paint.push_back(Paint("met2.probe", 69, 25));
	paint.push_back(Paint("met1.probe", 68, 25));
	paint.push_back(Paint("li1.probe", 67, 25));
	paint.push_back(Paint("poly.probe", 66, 25));
	paint.push_back(Paint("poly.short", 66, 15));
	paint.push_back(Paint("li1.short", 67, 15));
	paint.push_back(Paint("met1.short", 68, 15));
	paint.push_back(Paint("met2.short", 69, 15));
	paint.push_back(Paint("met3.short", 70, 15));
	paint.push_back(Paint("met4.short", 71, 15));
	paint.push_back(Paint("met5.short", 72, 15));
	paint.push_back(Paint("cncm.mask", 17, 0));
	paint.push_back(Paint("crpm.mask", 96, 0));
	paint.push_back(Paint("cpdm.mask", 37, 0));
	paint.push_back(Paint("cnsm.mask", 22, 0));
	paint.push_back(Paint("cmm5.mask", 59, 0));
	paint.push_back(Paint("cviam4.mask", 58, 0));
	paint.push_back(Paint("cmm4.mask", 51, 0));
	paint.push_back(Paint("cviam3.mask", 50, 0));
	paint.push_back(Paint("cmm3.mask", 34, 0));
	paint.push_back(Paint("cviam2.mask", 44, 0));
	paint.push_back(Paint("cmm2.mask", 41, 0));
	paint.push_back(Paint("cviam.mask", 40, 0));
	paint.push_back(Paint("cmm1.mask", 36, 0));
	paint.push_back(Paint("ctm1.mask", 35, 0));
	paint.push_back(Paint("cli1m.mask", 56, 0));
	paint.push_back(Paint("clicm1.mask", 43, 0));
	paint.push_back(Paint("cpsdm.mask", 32, 0));
	paint.push_back(Paint("cnsdm.mask", 30, 0));
	paint.push_back(Paint("cldntm.mask", 11, 0));
	paint.push_back(Paint("cnpc.mask", 49, 0));
	paint.push_back(Paint("chvntm.mask", 39, 0));
	paint.push_back(Paint("cntm.mask", 27, 0));
	paint.push_back(Paint("cp1m.mask", 28, 0));
	paint.push_back(Paint("clvom.mask", 46, 0));
	paint.push_back(Paint("conom.mask", 88, 0));
	paint.push_back(Paint("ctunm.mask", 20, 0));
	paint.push_back(Paint("chvtrm.mask", 98, 0));
	paint.push_back(Paint("chvtpm.mask", 97, 0));
	paint.push_back(Paint("clvtnm.mask", 25, 0));
	paint.push_back(Paint("cnwm.mask", 21, 0));
	paint.push_back(Paint("cdnm.mask", 48, 0));
	paint.push_back(Paint("cfom.mask", 23, 0));
	paint.push_back(Paint("cfom.drawing", 22, 20));
	paint.push_back(Paint("clvtnm.drawing", 25, 44));
	paint.push_back(Paint("chvtpm.drawing", 88, 44));
	paint.push_back(Paint("conom.drawing", 87, 44));
	paint.push_back(Paint("clvom.drawing", 45, 20));
	paint.push_back(Paint("cntm.drawing", 26, 20));
	paint.push_back(Paint("chvntm.drawing", 38, 20));
	paint.push_back(Paint("cnpc.drawing", 44, 20));
	paint.push_back(Paint("cnsdm.drawing", 29, 20));
	paint.push_back(Paint("cpsdm.drawing", 31, 20));
	paint.push_back(Paint("cli1m.drawing", 115, 44));
	paint.push_back(Paint("cviam3.drawing", 112, 20));
	paint.push_back(Paint("cviam4.drawing", 117, 20));
	paint.push_back(Paint("cncm.drawing", 96, 44));
	paint.push_back(Paint("cntm.maskAdd", 26, 21));
	paint.push_back(Paint("clvtnm.maskAdd", 25, 43));
	paint.push_back(Paint("chvtpm.maskAdd", 97, 43));
	paint.push_back(Paint("cli1m.maskAdd", 115, 43));
	paint.push_back(Paint("clicm1.maskAdd", 106, 43));
	paint.push_back(Paint("cpsdm.maskAdd", 31, 21));
	paint.push_back(Paint("cnsdm.maskAdd", 29, 21));
	paint.push_back(Paint("cp1m.maskAdd", 33, 43));
	paint.push_back(Paint("cfom.maskAdd", 22, 21));
	paint.push_back(Paint("cntm.maskDrop", 26, 22));
	paint.push_back(Paint("clvtnm.maskDrop", 25, 42));
	paint.push_back(Paint("chvtpm.maskDrop", 97, 42));
	paint.push_back(Paint("cli1m.maskDrop", 115, 42));
	paint.push_back(Paint("clicm1.maskDrop", 106, 42));
	paint.push_back(Paint("cpsdm.maskDrop", 31, 22));
	paint.push_back(Paint("cnsdm.maskDrop", 29, 22));
	paint.push_back(Paint("cp1m.maskDrop", 33, 42));
	paint.push_back(Paint("cfom.maskDrop", 22, 22));
	paint.push_back(Paint("cmm4.waffleDrop", 112, 4));
	paint.push_back(Paint("cmm3.waffleDrop", 107, 24));
	paint.push_back(Paint("cmm2.waffleDrop", 105, 52));
	paint.push_back(Paint("cmm1.waffleDrop", 62, 24));
	paint.push_back(Paint("cp1m.waffleDrop", 33, 24));
	paint.push_back(Paint("cfom.waffleDrop", 22, 24));
	paint.push_back(Paint("cmm5.waffleDrop", 117, 4));

	models.push_back(Model(Model::NMOS, "sky130_fd_pr__nfet_01v8", 26));
	models.back().paint.push_back(Diffusion(findPaint("diff.drawing"), -1, -1, vec2i(50, 0)));
	models.back().paint.push_back(Diffusion(findPaint("nsdm.drawing"), -1, -1, vec2i(25, 25)));
	models.push_back(Model(Model::PMOS, "sky130_fd_pr__pfet_01v8", 26));
	models.back().paint.push_back(Diffusion(findPaint("diff.drawing"), -1, -1, vec2i(50, 0)));
	models.back().paint.push_back(Diffusion(findPaint("psdm.drawing"), -1, -1, vec2i(25, 25)));
	models.back().paint.push_back(Diffusion(findPaint("nwell.drawing"), -1, -1, vec2i(36, 36)));
	models.push_back(Model(Model::PMOS, "sky130_fd_pr__pfet_01v8_hvt", 26));
	models.back().paint.push_back(Diffusion(findPaint("diff.drawing"), -1, -1, vec2i(50, 0)));
	models.back().paint.push_back(Diffusion(findPaint("psdm.drawing"), -1, -1, vec2i(25, 25)));
	models.back().paint.push_back(Diffusion(findPaint("hvtp.drawing"), -1, -1, vec2i(11, 11)));
	models.back().paint.push_back(Diffusion(findPaint("nwell.drawing"), -1, -1, vec2i(36, 36)));


	vias.push_back(Via(findPaint("licon1.drawing"), -1, -1, -1, 1, 8, 12, 0, 16));
	vias.push_back(Via(findPaint("licon1.drawing"), -1, -1, -2, 1, 8, 12, 0, 16));
	vias.push_back(Via(findPaint("licon1.drawing"), -1, -1, -3, 1, 8, 12, 0, 16));

	vias.push_back(Via(findPaint("licon1.drawing"), -1, -1, 0, 1, 10, 16, 0, 16));
	vias.push_back(Via(findPaint("mcon.drawing"), -1, -1, 1, 2, 0, 0, 6, 12));
	vias.push_back(Via(findPaint("via.drawing"), -1, -1, 2, 3, 11, 11, 11, 17));
	vias.push_back(Via(findPaint("via2.drawing"), -1, -1, 3, 4, 8, 17, 13, 13));
	vias.push_back(Via(findPaint("via3.drawing"), -1, -1, 4, 5, 12, 18, 13, 13));
	vias.push_back(Via(findPaint("via4.drawing"), -1, -1, 5, 6, 38, 38, 62, 62));

	wires.push_back(Routing(findPaint("poly.drawing"), findPaint("poly.label"), findPaint("poly.pin")));
	wires.push_back(Routing(findPaint("li1.drawing"), findPaint("li1.label"), findPaint("li1.pin")));
	wires.push_back(Routing(findPaint("met1.drawing"), findPaint("met1.label"), findPaint("met1.pin")));
	wires.push_back(Routing(findPaint("met2.drawing"), findPaint("met2.label"), findPaint("met2.pin")));
	wires.push_back(Routing(findPaint("met3.drawing"), findPaint("met3.label"), findPaint("met3.pin")));
	wires.push_back(Routing(findPaint("met4.drawing"), findPaint("met4.label"), findPaint("met4.pin")));
	wires.push_back(Routing(findPaint("met5.drawing"), findPaint("met5.label"), findPaint("met5.pin")));

	setSpacing(findPaint("diff.drawing"), findPaint("diff.drawing"), 54);
	setSpacing(findPaint("hvtp.drawing"), findPaint("hvtp.drawing"), 76);
	setSpacing(findPaint("poly.drawing"), findPaint("poly.drawing"), 42);
	setSpacing(findPaint("licon1.drawing"), findPaint("licon1.drawing"), 34);
	setSpacing(findPaint("li1.drawing"), findPaint("li1.drawing"), 34);
	setSpacing(findPaint("mcon.drawing"), findPaint("mcon.drawing"), 38);
	setSpacing(findPaint("met1.drawing"), findPaint("met1.drawing"), 28);
	setSpacing(findPaint("via.drawing"), findPaint("via.drawing"), 34);
	setSpacing(findPaint("met2.drawing"), findPaint("met2.drawing"), 28);
	setSpacing(findPaint("via2.drawing"), findPaint("via2.drawing"), 40);
	setSpacing(findPaint("met3.drawing"), findPaint("met3.drawing"), 60);
	setSpacing(findPaint("via3.drawing"), findPaint("via3.drawing"), 40);
	setSpacing(findPaint("met4.drawing"), findPaint("met4.drawing"), 60);
	setSpacing(findPaint("via4.drawing"), findPaint("via4.drawing"), 160);
	setSpacing(findPaint("met5.drawing"), findPaint("met5.drawing"), 320);
	setSpacing(findPaint("poly.drawing"), findPaint("licon1.drawing"), 18);
	//setSpacing(findPaint("diff.drawing"), findPaint("nwell.drawing"), 68);
	//setSpacing(findPaint("poly.drawing"), findPaint("diff.drawing"), 38);
	//setSpacing(findPaint("licon1.drawing"), findPaint("diff.drawing"), 38);

	boundary = findPaint("areaid.sc.identifier");
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

int Tech::findPaint(string name) const {
	for (int i = 0; i < (int)paint.size(); i++) {
		if (paint[i].name == name) {
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
