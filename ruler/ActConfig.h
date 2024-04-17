#pragma once

#include "conf.h"
#include <string>
#include <map>
#include <vector>

#include "Tech.h"

using namespace std;

namespace ruler {

struct ActConfig {
	string mangleChars;
	string mangleLetter;
	vector<pair<string, vector<int> > > mtrls;

	void loadValue(const Tech &tech, pgen::conf_t lang, pgen::lexer_t &lexer, pgen::token_t &value, string name, map<string, string> &mtrlMap);
	void loadBlock(const Tech &tech, pgen::conf_t lang, pgen::lexer_t &lexer, pgen::token_t &block, string name);
	bool load(const Tech &tech, string path);

	string replaceEnvVariables(string input) const;
	string mangleName(string name) const;
	string demangleName(string name) const;
};

}
