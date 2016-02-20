/*
Kite2D Game Framework.
Copyright (C) 2010-2015  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
USA
*/
#include <stdio.h>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <streambuf>
#include <stack>
#include <utility>
#include <cctype>
#include <tinydir/tinydir.h>

std::unordered_map<std::string, std::string> strmap;

#define CLS_ATTRIB 6
#define FUNC_ATTRIB 6
#define VAR_ATTRIB 6

enum ParsState {
	PS_WORD = 0,	// all names (ex: class, void, myName, ...)
	PS_ENDTOKEN,	// (;)
	PS_MODTOKEN,	// (*&)
	PS_SPLTOKEN,	// (,)
	PS_COLTOKEN,	// (:)
	PS_OTHTOKEN,	// (+-=!)
	PS_STRING,		// string token (ex: @)
	PS_BODY_START,	// all types of body (supported: (, [, {)
	PS_BODY_END,	// all types of body (supported: ), ], })
	PS_END			// end of file
};

enum MClassBaseAccs {
	CA_PUBLIC = 0,
	CA_PROTECTED,
	CA_PRIVATE
};

enum MClassType {
	CT_CLASS = 0,
	CT_STRUCT
};

enum MExportState{
	ES_EXPORT = 0,
	ES_IMPORT,
	ES_NONE
};

struct MFParam {
	std::string name;
	std::string type;
	std::string defval;
};
struct MFunction {
	std::string name;
	std::string ret;
	std::string tokparam;
	bool ista;
	bool inl;
	bool ext;
	bool cons;
	bool vir;
	bool explct;
	std::vector<MFParam> params;

	MFunction() :
		ista(false), inl(false), ext(false),
		cons(false), vir(false), explct(false)
	{}
};

struct MProperty {
	std::string name;
	MFunction get;
	MFunction set;
	std::string comment;
	std::string type;
};

struct MVariable {
	std::string name;
	std::string type;
	std::string tokparam;
	std::string arrSize;
	bool cons;
	bool ista;
	bool isArray;
	bool isRef;
	bool isPtr;

	MVariable() :
		cons(false), ista(false), isArray(false),
		isRef(false), isPtr(false)
	{}
};

struct MClassBase {
	std::string name;
	MClassBaseAccs accs;

	MClassBase() :
		accs(CA_PRIVATE) {}
};

struct MClass {
	std::string name;
	std::string tokparam;
	MClassType type;
	MExportState exstate;
	MFunction constructure;
	std::string flags;
	std::vector<MClassBase> bases;
	std::vector<MProperty> props;
	std::vector<MFunction> funcs;
	std::vector<MVariable> vars;

	MClass() :
		exstate(ES_NONE) {}
};

struct MEnum {
	std::string name;
	std::string tokparam;
	std::string type;
	std::vector<std::pair<std::string, std::string>> members;  // pair<name, value>
};

bool isNumber(const std::string& s) {
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

unsigned int tokenCounter(const std::string &Content, const std::string &Token) {
	if (Content.empty()) {
		return 0;
	}

	size_t pos = 0;
	unsigned int count = 0;
	while ((pos = Content.find(Token, pos)) != std::string::npos) {
		pos += Token.length();
		++count;
	}

	return count;
}

// get unique string key
std::string getKey() {
	std::string key("@");
	static unsigned int num = 0;
	key.append(std::to_string(++num));
	return key;
}

// remove anythings after given token
void removeAfter(std::string &Content, const std::string Tok) {
	size_t tpos = 0;
	if ((tpos = Content.find(Tok)) != std::string::npos) {
		Content.erase(tpos, Content.length());
	}
}

// remove given token
void removeTok(std::string &Content, const std::string Tok) {
	size_t tpos = 0;
	while ((tpos = Content.find(Tok)) != std::string::npos) {
		Content.erase(tpos, Tok.length());
	}
}

void replaceTok(std::string &Content, char Target, char Replace) {
	for (size_t i = 0; i < Content.length(); i++) {
		if (Content[i] == Target)
			Content[i] = Replace;
	}
}

// get position of specifi token
unsigned int getTokPos(const std::string &Content, unsigned int Pos, char tok, bool Toend = false) {
	if (Content.empty())
		return 0;

	if (Pos >= Content.length())
		return 0;

	// ignore space and new line
	for (auto i = Pos; i < Content.length(); i++) {
		if (Content[i] == ' ' || Content[i] == '\n' || Content[i] == '\t') {
			continue;

		} else if (Content[i] == tok) {
			return i + 1;

		} else if (!Toend){
			std::string::npos;
		}
	}
	return std::string::npos;;
}

// checking specific char token
bool checkTok(const std::string &Content, unsigned int Pos, char tok) {
	if (Content.empty())
		return false;

	if (Pos >= Content.length())
		return false;

	// ignore space and new line
	for (auto i = Pos; i < Content.length(); i++) {
		if (Content[i] == ' ' || Content[i] == '\n' || Content[i] == '\t') {
			continue;

		} else if (Content[i] == tok) {
			return true;

		} else {
			return false;
		}
	}

	return false;
}

// checking next input (ignore space and new line)
ParsState checkNextRaw(const std::string &Content, unsigned int Pos, bool Colon) {
	if (Content.empty())
		return PS_END;

	if (Pos >= Content.length())
		return PS_END;

	// ignore space and new line
	for (auto i = Pos; i < Content.length(); i++) {
		if (Content[i] == ' ' || Content[i] == '\n' || Content[i] == '\t'){
			continue;

		} else if (Content[i] == '(' || Content[i] == '[' || Content[i] == '{') {
			return PS_BODY_START;

		}else if (Content[i] == ')' || Content[i] == ']' || Content[i] == '}'){
			return PS_BODY_END;

		} else if (Content[i] == '=' || Content[i] == '-' || Content[i] == '+') {
			return PS_OTHTOKEN;

		} else if (Content[i] == ',') {
			return PS_SPLTOKEN;

		} else if ( Content[i] == ';') {
			return PS_ENDTOKEN;

		} else if (Content[i] == '&' || Content[i] == '*' ) {
			return PS_MODTOKEN;

		}else if (Colon && Content[i] == ':'){
			return PS_COLTOKEN;

		} else if (Content[i] == '@') {
			return PS_STRING;

		} else {
			return PS_WORD;
		}
	}

	return PS_END;
}

ParsState checkNext(const std::string &Content, unsigned int Pos) {
	return checkNextRaw(Content, Pos, true);
}

// get next word (token: sapce or new-line "=+-[]{}())
unsigned int getNextWord(const std::string &Content, unsigned int Pos, std::string &OutTok, bool Colon) {
	OutTok.clear();
	if (Content.empty())
		return 0;

	size_t begin = 0;
	size_t end = 0;
	for (auto i = Pos; i < Content.length(); i++) {
		if (Content[i] == ' ' || Content[i] == '\n' ||
			Content[i] == '(' || Content[i] == ')' ||
			Content[i] == '[' || Content[i] == ']' ||
			Content[i] == '{' || Content[i] == '}' ||
			Content[i] == ',' || Content[i] == '=' ||
			Content[i] == '+' || Content[i] == '-' ||
			Content[i] == '&' || Content[i] == '|' ||
			Content[i] == '*' ||
			Content[i] == '\t' || Content[i] == ';') {
			continue;

		} else if(Colon && Content[i] == ':'){
			continue;
		}else{
			begin = i;
			break;
		}
	}

	for (auto i = begin; i < Content.length(); i++) {
		 if (Content[i] == ' ' || Content[i] == '\n' ||
				   Content[i] == '(' || Content[i] == ')' ||
				   Content[i] == '[' || Content[i] == ']' ||
				   Content[i] == '{' || Content[i] == '}' ||
				   Content[i] == ',' || Content[i] == '=' ||
				   Content[i] == '+' || Content[i] == '-' ||
				   Content[i] == '|' || Content[i] == ';' ||
				   Content[i] == '\t'){
			end = i;
			break;
		} else if (Colon && Content[i] == ':') {
			end = i;
			break;
		}
	}

	OutTok = Content.substr(begin, (end - begin));
	return end;
}

// get body 
unsigned int getNextBody(const std::string &Content, unsigned int Pos, std::string &OutBody) {
	OutBody.clear();
	if (Content.empty())
		return Pos;

	size_t begin = 0;
	char btok;
	char etok;
	size_t end = 0;
	for (auto i = Pos; i < Content.length(); i++) {
		if (Content[i] == ' ' || Content[i] == '\n' || Content[i] == '\t') {
			continue;

		} else if (Content[i] == '(') {
			begin = i;
			btok = '(';
			etok = ')';
			break;

		} else if (Content[i] == '[') {
			begin = i;
			btok = '[';
			etok = ']';
			break;

		}else if (Content[i] == '{'){
			begin = i;
			btok = '{';
			etok = '}';
			break;

		}else {
			return 0;
		}
	}

	unsigned int deep = 0;
	for (auto i = begin; i < Content.length(); i++) {
		// deep-in
		if (Content[i] == btok) {
			++deep;
			continue;
		}

		// deep-out
		if (Content[i] == etok) {
			--deep;
		}

		// end of body
		if (Content[i] == etok && deep == 0) {
			end = i + 1;
			break;
		}

		// bad structure (missing token)
		if (i >= (Content.length() - 1)) {
			printf("incorrect body structure.\n");
			end = Content.length();
			break;
		}
	}

	OutBody = Content.substr(begin, (end - begin));

	return end;
}

// safe token finder
unsigned int findToken(const std::string &Content, unsigned int Pos, const std::string &Token) {
	size_t pos = 0;
	size_t tpos = Pos;

	if (Content.empty()) {
		return std::string::npos;
	}

	while ((pos = Content.find(Token, tpos)) != std::string::npos) {
		tpos = pos + Token.length();

		// token at the start of file
		if (pos > 0) {

			// check before token
			if (Content[pos - 1] != ' ' && Content[pos - 1] != '\t' && Content[pos - 1] != '\n') {
				continue;
			}
		}

		// check after token
		if (checkNext(Content, tpos) != PS_BODY_START) {
			continue;
		}

		return tpos;
	}

	return std::string::npos;
}

// remove strings ("...")
void collapseStrings(std::string &Content) {
	if (Content.empty())
		return;

	size_t pos = 0;
	size_t tpos = 0;
	size_t tsize = strlen("\"");
	while ((pos = Content.find("\"", tpos)) != std::string::npos) {
		tpos = pos + tsize;

		// get current line
		size_t line;
		if ((line = Content.find("\n", tpos)) == std::string::npos)
			line = Content.length(); // we are at the end of file

		// string lenght
		size_t len = 0;
		if ((len = Content.find("\"", tpos)) != std::string::npos && len < line) {
			std::string key = getKey();
			strmap.insert({ key, Content.substr(pos + 1, (len + tsize) - (pos + 2)) }); // pos + 1: remove " token from both side
			Content.replace(pos, (len + tsize) - pos, key);
		}// else: (ex: //comm"ent or there is no any quotes in the current line)
	}
}

// remove commnets (//)
void removeFSComments(std::string &Content) {
	if (Content.empty())
		return;

	size_t pos = 0;
	size_t tpos = 0;
	size_t tsize = strlen("//");
	while ((pos = Content.find("//", tpos)) != std::string::npos) {
		tpos = pos + tsize;

		// get current line
		size_t line = 0; // we are at the begining of the file by default
		if ((line = Content.find("\n", tpos)) != std::string::npos)
			Content.erase(pos, line - pos);
	}
}

// remove commnets (/*)
void removeSSComments(std::string &Content) {
	if (Content.empty())
		return;

	size_t pos = 0;
	size_t tpos = 0;
	size_t tsize = strlen("/*");
	while ((pos = Content.find("/*", tpos)) != std::string::npos) {
		tpos = pos + tsize;

		// get end cm token (*/)
		size_t ntok = 0;
		if ((ntok = Content.find("*/", tpos)) != std::string::npos) {
			int lines = tokenCounter(Content.substr(pos, (ntok + tsize) - pos), "\n");
			Content.replace(pos, (ntok + tsize) - pos, lines, '\n');
		}
	}
}

void splitBy(const std::string &Content, std::string Tok, std::vector<std::string> &OutList) {
	OutList.clear();
	size_t pos = 0;
	size_t tsize = Tok.length();
	std::string tstr = Content;

	tstr.append(Tok);

	while ((pos = tstr.find(Tok)) != std::string::npos) {
		if (!tstr.substr(0, pos).empty())
			OutList.push_back(tstr.substr(0, pos));
		tstr.erase(0, pos + tsize);
	}
}

// split a list of parameter by token (,)
void splitParamRaw(const std::string &Content, std::vector<std::string> &OutList, bool IgnoreSN) {
	OutList.clear();

	std::string temp = Content;
	if (IgnoreSN) {
		removeTok(temp, " ");
		removeTok(temp, "\n");
		removeTok(temp, "\t");
	}
	removeTok(temp, "(");
	removeTok(temp, ")");
	removeTok(temp, "[");
	removeTok(temp, "]");
	removeTok(temp, "{");
	removeTok(temp, "}");

	if (temp.empty())
		return;

	if (temp[temp.length() - 1] != ',')
		temp.append(",");

	size_t pos = 0;
	size_t tsize = strlen(",");
	while ((pos = temp.find(",")) != std::string::npos) {
		if (!temp.substr(0, pos).empty()) {
			OutList.push_back(temp.substr(0, pos));
		}
		temp.erase(0, pos + tsize);
	}
}

void splitParam(const std::string &Content, std::vector<std::string> &OutList) {
	splitParamRaw(Content, OutList, true);
}

bool procFunc(const std::string &Content, MFunction &Func, unsigned int Pos) {
	std::string output;
	std::vector<std::string> params;
	if (checkNext(Content, Pos) != PS_BODY_START) {
		printf("error: missing ().\n");
		return false;
	}

	// its a generic function processor so we dont check number of token parameters here,
	// and there is no warning for empty parameter list
	Pos = getNextBody(Content, Pos, output);
	Func.tokparam = output;

	// check function attributes 
	if (checkNextRaw(Content, Pos, false) != PS_WORD) {
		printf("error: missing function name/attributes.\n");
		return false;
	}

	std::vector<std::string> vList;
	std::string retType;
	vList.reserve(FUNC_ATTRIB);
	for (auto i = 0; i < FUNC_ATTRIB; i++) {
		if (checkNextRaw(Content, Pos, false) != PS_BODY_START) {
			Pos = getNextWord(Content, Pos, output, false);

			// pointer or ref (return value)
			/*if (checkNextRaw(Content, Pos, false) == PS_MODTOKEN) {
				if (checkTok(Content, Pos, '*')) {
					retType = " *";
				} else if (checkTok(Content, Pos, '&')) {
					retType = " &";
				}
			}*/

			if (output == "inline") {
				Func.inl = true;
			} else if (output == "static") {
				Func.ista = true;
			} else if (output == "extern") {
				Func.ext = true;
			} else if (output == "virtual") {
				Func.vir = true;
			} else if (output == "explicit") {
				Func.explct = true;
			}

			vList.push_back(output);
		} else {
			break;
		}
	}

	if (vList.empty()) {
		printf("error: could not extract function name. something is wrong.");
		return false;
	}

	Func.name = vList.back();

	// return type
	if (vList.size() > 1 && !Func.explct) {
		Func.ret = vList[vList.size() - 2] + retType;
	}

	// get function parameter list
	if (checkNextRaw(Content, Pos, false) != PS_BODY_START) {
		printf("error: missing \"()\" ==> function name: %s", Func.name.c_str());
		return false;
	}

	Pos = getNextBody(Content, Pos, output);
	splitParamRaw(output, params, false);

	std::vector<std::string> pinfo;
	if (!params.empty()) {
		for (size_t i = 0; i < params.size(); i++) {

			MFParam tpar;

			// default value
			splitBy(params[i], "=", pinfo);
			if (pinfo.size() > 1) {
				removeTok(pinfo[1], " ");
				removeTok(pinfo[1], "\n");
				removeTok(pinfo[1], "\t");
				removeAfter(params[i], "=");
				tpar.defval = pinfo[1]; // default value
			}

			// name and type
			std::string tmod;
			/*if (params[i].find("*") != std::string::npos) {
				tmod = " *";
			} else if (params[i].find("&") != std::string::npos) {
				tmod = " &";
			}*/
			removeTok(params[i], "*");
			removeTok(params[i], "&");
			splitBy(params[i], " ", pinfo);
			if (pinfo.size() < 2) {
				printf("error: missing parameter name ==> function name: %s\n", Func.name.c_str());
				return false;
			}


			// ex: char param
			if (pinfo.size() == 2) {
				tpar.type = pinfo[0] + tmod; // type
				tpar.name = pinfo[1]; // name
			}

			// ex: const char param
			if (pinfo.size() == 3) {
				tpar.type = pinfo[1] + tmod; // type
				tpar.name = pinfo[2]; // name
			}
			
			Func.params.push_back(tpar);
		}
	}

	// check const
	if (checkNext(Content, Pos) == PS_WORD) {
		for (auto i = 0; i < FUNC_ATTRIB; i++) {
			if (checkNext(Content, Pos) == PS_WORD) {
				Pos = getNextWord(Content, Pos, output, false);

				if (output == "const") {
					Func.cons = true;
				}
			} else {
				break;
			}
		}
	}
	
	return true;
}

// parse all property in the content
bool procAllFunc(std::string &Content, std::vector<MFunction> &Funcs) {
	size_t pos = 0;
	unsigned int count = 0;
	bool ret = true;
	Funcs.clear();

	if (Content.empty()) {
		return false;
	}

	while ((pos = findToken(Content, pos, "KMETA_FUNCTION")) != std::string::npos) {
		MFunction tfun;
		if (procFunc(Content, tfun, pos)) {
			Funcs.push_back(tfun);

		} else {
			ret = false;
			printf("Function token number: %u\n", count);
		}
		++count;
	}

	return ret;
}

bool procConstructure(const std::string &Content, MFunction &Func) {
	size_t pos = 0;

	// only 1 constructure supported at this time
	if ((pos = findToken(Content, 0, "KMETA_CONSTRUCTURE")) != std::string::npos) {

		if (procFunc(Content, Func, pos)) {
			return true;
		}

	} else {
		// no constructure
		return true;
	}

	return false;
}

bool procProp(const std::vector<MFunction> &Allprop, std::vector<MProperty> &Output) {
	std::vector<std::string> param;
	std::unordered_map<std::string, std::pair<MProperty, bool>> map;
	Output.clear();

	if (Allprop.empty()) {
		return true;
	}

	for (size_t i = 0; i < Allprop.size(); i++) {

		// exteract token parameter
		splitParam(Allprop[i].tokparam, param);
		
		if (param.empty()) {
			printf("error: missing property info ==> function name: %s\n", Allprop[i].name.c_str());
			return false;
		} else if (param.size() > 2) {
			printf("warning: extera property info (ignored) ==> function name: %s\n", Allprop[i].name.c_str());
		}


		// extract string name from string map
		auto pname = strmap.find(param[0]);
		if (pname == strmap.end()) {
			printf("error: could not extract prperty name from string map ==> function name: %s\n", Allprop[i].name.c_str());
			return false;
		}

		auto prop = map.find(pname->second);
		auto *pptr = &map[pname->second];
		pptr->first.name = pname->second;

		// new property
		if (prop == map.end()) {
			pptr->second = false;

		// registered property
		} else {

			// check overload
			if (prop->second.second) {
				printf("error: property overload not supported ==> property name: %s\n", pname->second.c_str());
				return false;
			}

			prop->second.second = true;
		}

		// have comment
		if (param.size() > 1) {
			pname = strmap.find(param[1]);
			if (pname == strmap.end()) {
				printf("error: could not extract prperty comment from string map ==> function name: %s\n", Allprop[i].name.c_str());
				return false;
			}
			pptr->first.comment = pname->second;
		}

		// check function type (setter or getter)
		// getter
		if (Allprop[i].params.empty()) {
			pptr->first.get = Allprop[i];
			pptr->first.type = Allprop[i].ret;

			// setter
		} else if (Allprop[i].params.size() == 1) {
			pptr->first.set = Allprop[i];
			pptr->first.type = Allprop[i].params[0].type;

			// more than 1 parameter (not supported)
		} else {
			printf("error: properties with more than 1 parameter not supported ==> function name: %s\n", Allprop[i].name.c_str());
			return false;
		}

	}

	// type match - iterate over all registered properties
	for (auto it = map.begin(); it != map.end(); ++it) {
		if (it->second.second) {
			if (it->second.first.get.ret != it->second.first.set.params[0].type) {
				printf("error: setter/getter type mismatch ==> property name: %s\n", it->second.first.name.c_str());
				return false;
			}
		}

		Output.push_back(it->second.first);
	}

	return true;
}

// parse all property in the content
bool procAllProp(const std::string &Content, std::vector<MProperty> &Props) {
	size_t pos = 0;
	std::vector<MFunction> allprop;
	unsigned int count = 0;
	bool ret = true;
	Props.clear();

	if (Content.empty()) {
		return false;
	}

	while ((pos = findToken(Content, pos, "KMETA_PROPERTY")) != std::string::npos) {
		MFunction tfunc;

		if (procFunc(Content, tfunc, pos)) {
			allprop.push_back(tfunc);

		} else {
			ret = false;
			printf("property token number: %u\n", count);
		}
		++count;
	}

	if (ret) {
		if (!procProp(allprop, Props)) {
			ret = false;
			printf("property token number: %u\n", count);
		}
	}

	return ret;
}

bool procVar(const std::string &Content, MVariable &Var, unsigned int Pos) {
	std::string output;
	if (checkNext(Content, Pos) != PS_BODY_START) {
		printf("error: missing ().\n");
		return false;
	}

	// its a generic variable processor so we dont check number of token parameters here,
	// and there is no warning for empty parameter list
	Pos = getNextBody(Content, Pos, output);
	Var.tokparam = output;

	// check variable
	if (checkNextRaw(Content, Pos, false) != PS_WORD) {
		printf("error: missing variable name/modifiers.\n");
		return false;
	}

	// exteract variable name/attribute/array/size/....
	auto tpos = getTokPos(Content, Pos, ';', true);
	--tpos;
	std::vector<std::string> vList;
	vList.reserve(VAR_ATTRIB);
	for (auto i = 0; i < VAR_ATTRIB; i++) {
		if (Pos < tpos) {
			if (checkNextRaw(Content, Pos, false) == PS_BODY_START){
				Pos = getNextBody(Content, Pos, output);
				Var.isArray = true;
				removeTok(output, "[");
				removeTok(output, "]");
				Var.arrSize = output;
				continue;

			// pointer or reference
			} else if (checkNextRaw(Content, Pos, false) == PS_MODTOKEN) {
				if (checkTok(Content, Pos, '*')) {
					Var.isPtr = true;
				} else if (checkTok(Content, Pos, '&')) {
					Var.isRef = true;
				}
			}

			Pos = getNextWord(Content, Pos, output, false);

			if (output == "static") {
				Var.ista = true;
				continue;
			} else if (output == "const") {
				Var.cons = true;
				continue;
			}

			vList.push_back(output);
		} else {
			break;
		}
	}

	if (vList.empty() || vList.size() < 2) {
		printf("error: could not extract variable name/type. something is wrong.");
		return false;
	}

	// name
	Var.name = vList.back();
	// type
	Var.type = vList[vList.size() - 2];

	return true;
}

// parse all CLASS tokens
bool procAllVar(const std::string &Content, std::vector<MVariable> &Vars) {
	size_t pos = 0;
	unsigned int count = 0;
	bool ret = true;
	Vars.clear();

	if (Content.empty()) {
		return false;
	}

	while ((pos = findToken(Content, pos, "KMETA_VARIABLE")) != std::string::npos) {
		MVariable tvar;
		if (procVar(Content, tvar, pos)) {
			Vars.push_back(tvar);

		} else {
			printf("variable token number: %u \n", count);
			ret = false;
		}
		++count;
	}

	return ret;
}

// parse and extract class information
bool procClass(const std::string &Content, MClass &Cls, unsigned int Pos) {
	size_t pos;
	std::string cbody;
	std::string output;
	std::vector<std::string> params;

	// checking token parameter(s)
	if (checkNext(Content, Pos) != PS_BODY_START) {
		printf("error: missing ().\n");
		return false;
	}

	// extract token parameter(s)
	pos = getNextBody(Content, Pos, output);
	Cls.tokparam = output;

	// checking type token (class or struct)
	if (checkNext(Content, pos) != PS_WORD) {
		printf("error: missing class/struct type.\n");
		return false;
	}

	pos = getNextWord(Content, pos, output, true);
	if (output == "class") {
		Cls.type = CT_CLASS;

	} else if (output == "struct") {
		Cls.type = CT_STRUCT;

	} else {
		printf("error: incorrect type (%s).\n", output.c_str());
		return false;
	}

	// checking expoert/import macro and name of class
	if (checkNext(Content, pos) != PS_WORD) {
		printf("error: missing export/import or class name.\n");
		return false;
	}

	// search CLS_ATTRIB word for class name and its attributes
	for (auto i = 0; i < CLS_ATTRIB; i++){
		if (checkNext(Content, pos) == PS_WORD) {
			pos = getNextWord(Content, pos, output, true);

			if (output == "KITE_FUNC_EXPORT") {
				Cls.exstate = ES_EXPORT;
			} else if (output == "KITE_FUNC_IMPORT") {
				Cls.exstate = ES_IMPORT;
			}// else ES_NONE by default

		} else {
			Cls.name = output;
			break;
		}
	}

	// checking base classes
	// with base(s)
	if (checkNext(Content, pos) == PS_COLTOKEN) {
		bool kobj = false;
		if (!checkTok(Content, pos, ':')) {
			printf("error: missing \":\" token.\n");
			return false;
		}
		pos = getTokPos(Content, pos, ':');

		if (checkNextRaw(Content, pos, false) != PS_WORD) {
			printf("error: missing base(es).\n");
			return false;
		}

		while (checkNextRaw(Content, pos, false) == PS_WORD || checkNextRaw(Content, pos, false) == PS_SPLTOKEN) {
			pos = getNextWord(Content, pos, output, true);
			MClassBase base;
			if (output == "public") {
				base.accs = CA_PUBLIC;
				pos = getNextWord(Content, pos, output, true);
				base.name = output;
				Cls.bases.push_back(base);

			} else if (output == "private") {
				base.accs = CA_PRIVATE;
				pos = getNextWord(Content, pos, output, true);
				base.name = output;
				Cls.bases.push_back(base);

			} else if (output == "protected") {
				base.accs = CA_PROTECTED;
				pos = getNextWord(Content, pos, output, true);
				base.name = output;
				Cls.bases.push_back(base);

			} else {
				base.accs = CA_PRIVATE;
				base.name = output;
				Cls.bases.push_back(base);
			}

		}
	}

	
	// class body
	size_t cpos = 0;
	if (checkNext(Content, pos) == PS_BODY_START) {

		// get class body
		getNextBody(Content, pos, cbody);

		if (!procConstructure(cbody, Cls.constructure)) {
			return false;
		}

		// default contructure
		/*if (Cls.constructure.name.empty()) {
			Cls.constructure.name = Cls.name;
		}*/
		
		// parse all properties in the class body
		if (!procAllProp(cbody, Cls.props)) {
			return false;
		}

		// parse all function in the class body
		if (!procAllFunc(cbody, Cls.funcs)) {
			return false;
		}

		if (!procAllVar(cbody, Cls.vars)) {
			return false;
		}

	// incorrect structure
	} else {
		printf("error: missing class\\structure body.\n");
		return false;
	}

	return true;
}

// parse all CLASS tokens
bool procAllClass(const std::string &Content, std::vector<MClass> &Classes) {
	size_t pos = 0;
	unsigned int count = 0;
	bool ret = true;
	Classes.clear();

	if (Content.empty()) {
		return false;
	}

	while ((pos = findToken(Content, pos, "KMETA_CLASS")) != std::string::npos) {
		MClass tcls;
		if (procClass(Content, tcls, pos)) {
			Classes.push_back(tcls);

		} else {
			unsigned int line = tokenCounter(Content.substr(0, pos), "\n");
			printf("line number: %u ==> token name: KMETA_CLASS ==> token number: %u \n", ++line, count);
			ret = false;
		}
		++count;
	}

	return ret;
}

bool procEnumMem(const std::string &Content, MEnum &Enm) {
	std::vector<std::string> param;
	std::vector<std::string> defval;
	Enm.members.clear();
	splitParam(Content, param);
	if (param.empty()) {
		printf("error: empty enum.\n");
		return false;
	}

	// member value
	for (size_t i = 0; i < param.size(); i++) {
		splitBy(param[i], "=", defval);

		// without value
		if (defval.size() == 1) {
			Enm.members.push_back({ param[i], "" });
			continue;

		// with value
		} else if (defval.size() == 2) {
			removeAfter(param[i], "=");
			Enm.members.push_back({ param[i], defval[1] });

		// extra or incorrect state
		} else {
			printf("error: incorrect enum member defention.\n");
			return false;
		}
	}

	return true;
}

bool procEnum(const std::string &Content, MEnum &Enm, unsigned int Pos) {
	size_t pos;
	std::string ebody;
	std::string output;
	if (Content.empty()) {
		return false;
	}

	// checking token parameter(s)
	if (checkNext(Content, Pos) != PS_BODY_START) {
		printf("error: missing ().\n");
		return false;
	}

	// extract token parameter(s)
	pos = getNextBody(Content, Pos, output);
	Enm.tokparam = output;

	// checking type token (class or struct)
	if (checkNext(Content, pos) != PS_WORD) {
		printf("error: missing enum type.\n");
		return false;
	}

	// enum token
	pos = getNextWord(Content, pos, output, true);
	if (output != "enum") {
		printf("error: incorrect type (%s)\n", output.c_str());
		return false;
	}

	// class token
	if (checkNext(Content, pos) != PS_WORD) {
		printf("error: missing enum class type.\n");
		return false;
	}

	pos = getNextWord(Content, pos, output, true);
	if (output != "class") {
		printf("error: only enum class (scoped enum) supported.\n");
		return false;
	}

	// name
	if (checkNext(Content, pos) != PS_WORD) {
		printf("error: missing enum name.\n");
		return false;
	}
	pos = getNextWord(Content, pos, output, true);
	Enm.name = output;

	// type
	if (checkNext(Content, pos) == PS_COLTOKEN) {
		pos = getTokPos(Content, pos, ':');

		if (checkNextRaw(Content, pos, false) != PS_WORD) {
			printf("error: missing enum type.\n");
			return false;
		}

		pos = getNextWord(Content, pos, output, true);
		Enm.type = output;
	} else {
		// int by default
		Enm.type = "I32";
	}
	
	// parse enum memebers
	if (checkNext(Content, pos) != PS_BODY_START) {
		printf("error: incorrect enum defention.\n");
		return false;
	}
	getNextBody(Content, pos, ebody);

	if (!procEnumMem(ebody, Enm)) {
		return false;
	}

	return true;
}

bool procAllEnum(const std::string &Content, std::vector<MEnum> &Enums) {
	size_t pos = 0;
	unsigned int count = 0;
	bool ret = true;
	Enums.clear();

	if (Content.empty()) {
		return false;
	}

	while ((pos = findToken(Content, pos, "KMETA_ENUM")) != std::string::npos) {
		MEnum tenu;
		if (procEnum(Content, tenu, pos)) {
			Enums.push_back(tenu);

		} else {
			unsigned int line = tokenCounter(Content.substr(0, pos), "\n");
			printf("line number: %u ==> token name: KMETA_ENUM ==> token number: %u \n", ++line, count);
			ret = false;
		}
		++count;
	}

	return ret;
}

bool isIgnored(const std::string &Contetnt) {
	if (Contetnt.find("KMETA_IGNORED") != std::string::npos) {
		return true;
	}
	return false;
}

// main parser
bool parse(std::string &Content, std::vector<MClass> &Classes, std::vector<MEnum> &Enums) {
	Classes.clear();
	Enums.clear();
	if (Content.empty()) {
		printf("empty file.\n");
		return false;
	}	

	bool ret = true;

	collapseStrings(Content);
	removeFSComments(Content);
	removeSSComments(Content);
	removeTok(Content, "public:");
	removeTok(Content, "private:");
	removeTok(Content, "protected:");

	if (isIgnored(Content)) {
		printf("ignored.\n");
		return false;
	}

	if (!procAllClass(Content, Classes))
		ret = false;

	if (!procAllEnum(Content, Enums))
		ret = false;

	return ret;
}

void createMacros(const std::vector<MClass> &Cls, const std::vector<MEnum> &Enms, std::string &Output) {
	Output.clear();

	// enums
	for (size_t i = 0; i < Enms.size(); i++) {
		std::string upname = Enms[i].name;

		// declaration
		Output.append("\n// ----[auto generated: " + Enms[i].name + " body macro]----\n");
		transform(upname.begin(), upname.end(), upname.begin(), toupper);
		Output.append("#define KMETA_" + upname + "_BODY() \\\n"
					  "namespace Internal{\\\n"
					  "KITE_FUNC_EXPORT extern void registerMeta_" + Enms[i].name + "(lua_State *Lua = nullptr);}\n");

		// definition
		std::string eParam = Enms[i].tokparam;
		replaceTok(eParam, ',', '|');
		Output.append("\n// ----[auto generated: " + Enms[i].name + " source macro]----\n");
		Output.append("#define KMETA_" + upname + "_SOURCE() \\\n"
					  "namespace Internal{\\\n"
					  "void registerMeta_" + Enms[i].name + "(lua_State *Lua){\\\n"
					  "static bool inite = true;\\\n"
					  "static KMetaEnum instance(\"" + Enms[i].name + "\"," + eParam + ", sizeof(" + Enms[i].type + "), "
					  "\"" + Enms[i].type + "\");\\\n"
					  "if (inite) {\\\n"
					  "KMetaManager::setMeta((KMetaObject *)&instance);\\\n");

		// enum members
		Output.append("KMetaEnumMember *mem = 0;\\\n");
		for (size_t count = 0; count < Enms[i].members.size(); count++) {
			std::string vc = std::to_string(count);
			if (!Enms[i].members[count].second.empty())
				vc = Enms[i].members[count].second;

			Output.append("mem = new KMetaEnumMember(\"" + Enms[i].members[count].first + "\", " + vc + ", "
						  + std::to_string(count) + ");\\\n");

			Output.append("instance.addMember(mem);\\\n");
		}

		// lua binding
		// enum members
		Output.append("const KMetaObject *minfo = KMetaManager::getMeta(\"" + Enms[i].name + "\");\\\n"
			"if (Lua != nullptr && (minfo->getFlag() & SCRIPTABLE)){\\\n"
			"LuaIntf::LuaBinding(Lua).beginModule(\"Kite\")\\\n"
			".beginModule(\"" + Enms[i].name + "\")\\\n");
		for (size_t count = 0; count < Enms[i].members.size(); count++) {

			Output.append(".addConstant(\"" + Enms[i].members[count].first + "\", " + Enms[i].name + "::" 
						  + Enms[i].members[count].first + ")\\\n");
		}

		// end of lua binding
		Output.append(".endModule().endModule();}\\\n");

		// end of definition
		Output.append("} inite = false;}}\n");
	}

	// class
	for (size_t i = 0; i < Cls.size(); i++) {
		std::string upname = Cls[i].name;
		std::string cParam = Cls[i].tokparam;
		replaceTok(cParam, '|', ',');
		std::vector<std::string> ctags;
		splitParam(cParam, ctags);

		// is seializable
		bool isPOD = false;
		if (std::find(ctags.begin(), ctags.end(), "POD") != ctags.end()) {
			isPOD = true;
		}

		// is scriptable
		bool isResource = false;
		if (std::find(ctags.begin(), ctags.end(), "RESOURCE") != ctags.end()) {
			isResource = true;
		}

		// is editable
		bool isComponent = false;
		if (std::find(ctags.begin(), ctags.end(), "COMPONENT") != ctags.end()) {
			isComponent = true;
		}

		// is entity
		bool isEntity = false;
		if (std::find(ctags.begin(), ctags.end(), "ENTITY") != ctags.end()) {
			isEntity = true;
		}

		// is system
		bool isSystem = false;
		if (std::find(ctags.begin(), ctags.end(), "SYSTEM") != ctags.end()) {
			isSystem = true;
		}

		// is abstract
		bool isAbstract = false;
		if (std::find(ctags.begin(), ctags.end(), "ABSTRACT") != ctags.end()) {
			isAbstract = true;
		}

		// is scriptable
		bool isScriptable = false;
		if (std::find(ctags.begin(), ctags.end(), "SCRIPTABLE") != ctags.end()) {
			isScriptable = true;
		}

		// class without any flag will ignored
		if (!isPOD && !isResource && !isComponent && !isSystem && !isScriptable) {
			printf("message: class without any supported flags. %s ignored. \n", Cls[i].name.c_str());
			continue;
		}

		// linkage state
		std::string exstae;
		if (Cls[i].exstate == ES_NONE) {
			exstae = "KITE_FUNC_EXPORT ";
		}

		// class body
		// private Section:
		Output.append("\n// ----[auto generated: " + Cls[i].name + " body macro]----\n");
		transform(upname.begin(), upname.end(), upname.begin(), toupper);
		Output.append("#define KMETA_" + upname + "_BODY() \\\n"
					  "private:\\\n");
		
		// factory function (with default constructure)
		if ((isPOD || isComponent) && !isAbstract) {
			Output.append(exstae + "static KObject *_createNew(KBaseStorage &Allocator);\\\n");
		}

		// properties (only editable classes allow properties)
		if (isComponent) {
			for (size_t count = 0; count < Cls[i].props.size(); count++) {
				if (!Cls[i].props[count].set.name.empty()) {
					Output.append("inline void _prp" + Cls[i].name + "set" + Cls[i].props[count].name + "(KRefVariant Value){ " +
								  Cls[i].props[count].set.name + "(Value.getValue<" + Cls[i].props[count].type + ">()); }\\\n");
				}

				if (!Cls[i].props[count].get.name.empty()) {
					Output.append("inline void _prp" + Cls[i].name + "get" + Cls[i].props[count].name + "(KRefVariant Value) { " +
								  "Value.copyByVal(" + Cls[i].props[count].get.name + "()); }\\\n");
				}
			}
		} 
		
		// public section:
		// register functions
		Output.append("public:\\\n" +
					  exstae + "static void registerMeta(lua_State *Lua = nullptr);\\\n"
					  "const std::string &getClassName() const { static std::string name(\"" + Cls[i].name + "\");\\\n"
					  "return name;}\\\n");

		// serializable
		if (isPOD || isComponent || isEntity) {
			Output.append("friend KBaseSerial &operator<<(KBaseSerial &Out, " + Cls[i].name + " &Value) {\\\n"
						  "Value.serial(Out, KST_SERIALIZE); return Out;}\\\n"
						  "friend KBaseSerial &operator>>(KBaseSerial &In, " + Cls[i].name + " &Value) {\\\n"
						  "Value.serial(In, KST_DESERIALIZE); return In;}\\\n"
						  "protected:\\\n" +
						  exstae + "void serial(KBaseSerial &Serializer, KSerialStateTypes State);\n");
		}

		// defention
		Output.append("\n// ----[auto generated: " + Cls[i].name + " source macro]----\n");
		Output.append("#define KMETA_" + upname + "_SOURCE()\\\n");

		if ((isPOD || isComponent) && !isAbstract) {
			Output.append("KObject *" + Cls[i].name + "::_createNew(KBaseStorage &Allocator){\\\n"
							"return allocateNew<" + Cls[i].name + ">(Allocator);}\\\n");
		}

		replaceTok(cParam, ',', '|');
		Output.append("void " + Cls[i].name + "::registerMeta(lua_State *Lua){\\\n"
					  "static bool inite = true;\\\n"
					  "static KMetaClass instance(\"" + Cls[i].name + "\"," + cParam + ", sizeof(" + Cls[i].name + "));\\\n"
					  "if (inite) {\\\n"
					  "KMetaManager::setMeta((KMetaObject *)&instance);\\\n");

		if ((isPOD || isComponent) && !isAbstract){
			Output.append("KMetaManager::setFactory(\"" + Cls[i].name + "\", " + Cls[i].name + "::_createNew);\\\n");
		}

		// bases
		for (size_t count = 0; count < Cls[i].bases.size(); count++) {
			std::string acc;
			if (Cls[i].bases[count].accs == CA_PUBLIC) { acc = "KMB_PUBLIC"; }
			if (Cls[i].bases[count].accs == CA_PRIVATE) { acc = "KMB_PRIVATE"; }
			if (Cls[i].bases[count].accs == CA_PROTECTED) { acc = "KMB_PROTECTED"; }
			Output.append("instance.addBase(new KMetaBase(\"" + Cls[i].bases[count].name + "\", "
						  + acc + "));\\\n");
		}

		// properties (meta)
		for (size_t count = 0; count < Cls[i].props.size(); count++) {
			std::string prpType("KMP_BOTH");
			if (Cls[i].props[count].get.name.empty()) {
				prpType = "KMP_SETTER";
			} else if (Cls[i].props[count].set.name.empty()) {
				prpType = "KMP_GETTER";
			}
			Output.append("instance.addProperty(new KMetaProperty(\"" + Cls[i].props[count].name + "\", \""
							+ Cls[i].props[count].type + "\", \"" + Cls[i].props[count].comment + "\", " + prpType + "));\\\n");

			// insert to property map
			// only allowed for components
			if (isComponent) {
				if (!Cls[i].props[count].get.name.empty()) {
					Output.append("prpMap.insert({ \"_prp" + Cls[i].name + "get" + Cls[i].props[count].name
								  + "\", (void (KComponent::*)(KRefVariant)) &" + Cls[i].name + "::"
								  + "_prp" + Cls[i].name + "get" + Cls[i].props[count].name + "});\\\n");
				}

				if (!Cls[i].props[count].set.name.empty()) {
					Output.append("prpMap.insert({ \"_prp" + Cls[i].name + "set" + Cls[i].props[count].name
								  + "\", (void (KComponent::*)(KRefVariant)) &" + Cls[i].name + "::"
								  + "_prp" + Cls[i].name + "set" + Cls[i].props[count].name + "});\\\n");
				}
			}
		}

		// functions
		if (!Cls[i].funcs.empty()) {
			Output.append("KMetaFunction *fun;\\\n");
		}
		for (size_t count = 0; count < Cls[i].funcs.size(); count++) {
			std::string ista = "false";
			if (Cls[i].funcs[count].ista) {
				ista = "true";
			} 
			Output.append("fun = new KMetaFunction(\"" + Cls[i].funcs[count].name + "\", \"" + Cls[i].funcs[count].ret + "\", " + ista + ");\\\n");

			// function parameters
			for (size_t pcount = 0; pcount < Cls[i].funcs[count].params.size(); pcount++) {
				//Output.append("fun->paramsName.push_back(\"" + Cls[i].funcs[count].params[pcount].name + "\");\\\n");
				Output.append("fun->paramsType.push_back(\"" + Cls[i].funcs[count].params[pcount].type + "\");\\\n");
			}
			Output.append("instance.addFunction(fun);\\\n");
		}

		// lua binding 
		Output.append("const KMetaObject *minfo = KMetaManager::getMeta(\"" + Cls[i].name + "\");\\\n"
					  "if (Lua != nullptr && (minfo->getFlag() & SCRIPTABLE)) { \\\n"
					  "LuaIntf::LuaBinding(Lua).beginModule(\"Kite\").beginClass<"
					  + Cls[i].name + ">(\"" + Cls[i].name + "\")\\\n");

		// constructure
		std::string param;
		std::string tok;
		if (!Cls[i].constructure.name.empty() && !isAbstract) {
			if (!Cls[i].constructure.params.empty()) {
				for (size_t count = 0; count < Cls[i].constructure.params.size(); count++) {
					if (!Cls[i].constructure.params[count].defval.empty()) {

						// nullptr not supported in lua binding so we change it with 0
						std::string newDefVal = Cls[i].constructure.params[count].defval;
						if (Cls[i].constructure.params[count].defval == "nullptr") {
							newDefVal = "0";
						}

						param.append(tok + "LuaIntf::_def<" + Cls[i].constructure.params[count].type + ", " + newDefVal + ">");
					} else {
						param.append(tok + "LuaIntf::_opt<" + Cls[i].constructure.params[count].type + ">");
					}
					tok = ",";
				}
			}
			Output.append(".addConstructor(LUA_ARGS(" + param + "))\\\n");
		}

		// properties
		for (size_t count = 0; count < Cls[i].props.size(); count++) {
			if (!Cls[i].props[count].get.name.empty() && !Cls[i].props[count].set.name.empty()) {
				Output.append(".addProperty(\"" + Cls[i].props[count].name + "\", &" + Cls[i].name + "::"
							  + Cls[i].props[count].get.name + ", &" + Cls[i].name + "::" + Cls[i].props[count].set.name + ")\\\n");
			} else if (Cls[i].props[count].set.name.empty()) {
				Output.append(".addProperty(\"" + Cls[i].props[count].name + "\", &" + Cls[i].name + "::"
							  + Cls[i].props[count].get.name +")\\\n");
			}
		}

		// functions 
		param = "";
		tok = "";
		std::string fista;
		for (size_t count = 0; count < Cls[i].funcs.size(); count++) {
			
			if (Cls[i].funcs[count].ista) {
				fista = "addStaticFunction";
			} else {
				fista = "addFunction";
			}
			//if (Cls[i].funcs[count].params.empty()) {
			std::vector<std::string> tpar;
			splitParam(Cls[i].funcs[count].tokparam, tpar);
			if (tpar.empty()) {
				Output.append("." + fista + "(\"" + Cls[i].funcs[count].name + "\", &" + Cls[i].name + "::" + Cls[i].funcs[count].name + ")\\\n");
			} else {
				auto found = strmap.find(tpar[0]);
				if (found != strmap.end()) {
					Output.append("." + fista + "(\"" + found->second + "\", &" + Cls[i].name + "::" + Cls[i].funcs[count].name + ")\\\n");
				} 
			}
			// we dont need register function parameters in lua
			/*} else {
				for (size_t pcount = 0; pcount < Cls[i].funcs[count].params.size(); pcount++) {
					if (!Cls[i].funcs[count].params[pcount].defval.empty()) {
						param.append(tok + Cls[i].funcs[count].params[pcount].type);
					} else {
						param.append(tok + "LuaIntf::_def<" + Cls[i].funcs[count].params[pcount].type + ", " + Cls[i].funcs[count].params[pcount].defval + ">");
					}
					tok = ",";
				}
				Output.append("." + fista + "(\"" + Cls[i].funcs[count].name + "\", &" + Cls[i].name + "::" + Cls[i].funcs[count].name
							  + ", LUA_ARGS(" + param + "))\\\n");
			}*/
		}

		// variables
		for (size_t count = 0; count < Cls[i].vars.size(); count++) {
			std::string func;

			// we dont support arrays in script
			if (Cls[i].vars[count].isArray) {
				continue;
			}

			// static/non-static variable
			if (Cls[i].vars[count].ista) {
				func = ".addStaticVariable";
			} else {
				func = ".addVariable";
			}

			std::vector<std::string> vparam;
			splitParam(Cls[i].vars[count].tokparam, vparam);

			if (std::find(vparam.begin(), vparam.end(), "SCRIPTABLE") != vparam.end()) {
				std::string write;
				if (Cls[i].vars[count].cons) {
					write = "false";
				} else {
					write = "true";
				}

				Output.append(func + "(\"" + Cls[i].vars[count].name + "\", &" + Cls[i].name + "::" 
								+ Cls[i].vars[count].name + ", " + write + ")");
				break;
			}
		}

		// end of lua binding
		Output.append(".endClass().endModule();}\\\n");

		// end of rgisterMeta
		if (isPOD || isComponent || isEntity) {
			Output.append("} inite = false;}\\\n");
		} else {
			Output.append("} inite = false;}\n");
		}

		// serial definition
		if (isPOD || isComponent || isEntity) {
			Output.append("void " + Cls[i].name + "::serial(KBaseSerial &Serializer, KSerialStateTypes State){\\\n");

			// serialize base class(es)
			std::string ser;
			std::string dser;
			for (size_t bcount = 0; bcount < Cls[i].bases.size(); bcount++) {
				ser.append(Cls[i].bases[bcount].name + " *bptr" + std::to_string(bcount)
							+ " = (" + Cls[i].bases[bcount].name + " *)this; Serializer << bptr" + std::to_string(bcount) + ";\\\n");
				dser.append(Cls[i].bases[bcount].name + " *bptr" + std::to_string(bcount)
							+ " = (" + Cls[i].bases[bcount].name + " *)this; Serializer >> bptr" + std::to_string(bcount) + ";\\\n");
			}

			// serialize
			Output.append("if (State == KST_SERIALIZE) {\\\n");
			if (!Cls[i].bases.empty()) {
				Output.append(ser);
			}

			// variables
			for (size_t vcont = 0; vcont < Cls[i].vars.size(); vcont++) {

				std::vector<std::string> vparam;
				splitParam(Cls[i].vars[vcont].tokparam, vparam);
				Output.append("Serializer << " + Cls[i].vars[vcont].name + ";\\\n");
			}

			// deserialize
			Output.append("} else if (State == KST_DESERIALIZE) {\\\n");
			Output.append(dser);

			// variables
			for (size_t vcont = 0; vcont < Cls[i].vars.size(); vcont++) {
				std::vector<std::string> vparam;
				splitParam(Cls[i].vars[vcont].tokparam, vparam);
				Output.append("Serializer >> " + Cls[i].vars[vcont].name + ";\\\n");
			}

			// end of serial()
			Output.append("}}\n");
		} 

		// interp()
		/*Output.append("void KColor::interp(F32 Time, const KObject &Start, const KObject &End,\\\n"
					  "F32 Duration, KInterpolationTypes FType) {\\\n"
					  "const " + Cls[i].name + " &start = (const " + Cls[i].name + " &)Start;\\\n"
					  "const " + Cls[i].name + " &end = (const " + Cls[i].name + " &)End;\\\n");
		for (size_t count = 0; count < Cls[i].props.size(); count++) {
			std::string val(Cls[i].props[count].name + "prp");
			Output.append(Cls[i].props[count].type + " " + val + ";\\\n"
						  + "interpolate(" + val + ", Time, start." + Cls[i].props[count].get.name
						  + "(), end." + Cls[i].props[count].get.name + "(), Duration, FType);\\\n"
						  + Cls[i].props[count].set.name + "(" + val + ");\\\n");
		}

		// end of interp()
		Output.append("}\n");*/
	}
}

void createHead(std::string &Output) {
	Output.clear();

	Output.append("\n// ----[auto generated header file]----\n");
	Output.append("#ifndef KITEMETA_H\n"
				  "#define KITEMETA_H\n\n"
				  "#include \"Kite/core/kcoredef.h\"\n"
				  "#include \"Kite/meta/kmetadef.h\"\n\n"
				  "KMETA\n"
				  "namespace Kite{\n"
				  "KITE_FUNC_EXPORT extern void registerKiteMeta(lua_State *Lua = nullptr);}\n"
				  "#endif // KITEMETA_H");
}

void createSource(const std::vector<std::string> &Files, const std::vector<MClass> &Cls,
				  const std::vector<MEnum> &Ens, std::string &Output) {


	Output.clear();
	Output.append("\n// ----[auto generated source file]----\n");

	// POD header file
	Output.append("#include \"Kite/meta/kmetapod.h\"\n");

	// add headers
	for (size_t i = 0; i < Files.size(); i++) {
		Output.append("#include \"" + Files[i] + "\"\n");
	}

	Output.append("namespace Kite{\n"
				  "void registerKiteMeta(lua_State *Lua){\n"
				  "Internal::registerMetaPOD();\n");

	// register classes
	for (size_t i = 0; i < Cls.size(); i++) {
		Output.append(Cls[i].name + "::registerMeta(Lua);\n");
	}

	// register enums
	for (size_t i = 0; i < Ens.size(); i++) {
		Output.append("Internal::registerMeta_" + Ens[i].name + "(Lua);\n");
	}

	Output.append("}}");

}

bool procFile(const std::string &FAddress, const std::string &OAdress, std::vector<MClass> &Cls, std::vector<MEnum> &Ens) {
	bool ret = true;
	std::string macro;
	std::vector<MClass> lcls;
	std::vector<MEnum> lens;
	std::ifstream in(FAddress);
	std::string content((std::istreambuf_iterator<char>(in)),
					std::istreambuf_iterator<char>());

	if (!parse(content, lcls, lens))
		return false;

	createMacros(lcls, lens, macro);

	if (macro.empty())
		return false;

	unsigned int pos = 0;
	if ((pos = FAddress.rfind("/")) == std::string::npos) {
		printf("cant fix output file address");
		return false;
	}

	unsigned int epos = 0;
	if ((epos = FAddress.rfind(".")) == std::string::npos) {
		printf("cant retrieve file extension");
		return false;
	}


	std::ofstream out(OAdress + "/" + FAddress.substr(pos, (epos - pos)) + ".khgen.h");
	out << macro;
	out.close();

	Cls.insert(Cls.end(), lcls.begin(), lcls.end());
	Ens.insert(Ens.end(), lens.begin(), lens.end());

	return true;
}

void createStaticName(std::vector<MClass> Cls, std::vector<MEnum> Enm, std::string &Output) {
	Output.clear();

	// headers
	Output.append("\n// ----[auto generated header file]----\n");
	Output.append("#include \"Kite/meta/kmetadef.h\"\n");
	Output.append("#include \"Kite/core/kcoredef.h\"\n\n");

	Output.append("namespace Kite{\n");
	Output.append("template<typename T>\n");
	Output.append("struct getMetaName;\n\n");

	Output.append("#define KMETA_REG_NAME(TYPE)\\\n");
	Output.append("template<>\\\n");
	Output.append("struct getMetaName<TYPE> {\\\n");
	Output.append("static const char *name;};\\\n");
	Output.append("const char *getMetaName<TYPE>::name = #TYPE\n\n");

	// pod types
	Output.append("KMETA_REG_NAME(I8);\n");
	Output.append("KMETA_REG_NAME(I16);\n");
	Output.append("KMETA_REG_NAME(I32);\n");
	Output.append("KMETA_REG_NAME(I64);\n");
	Output.append("KMETA_REG_NAME(U8);\n");
	Output.append("KMETA_REG_NAME(U16);\n");
	Output.append("KMETA_REG_NAME(U32);\n");
	Output.append("KMETA_REG_NAME(U64);\n");
	Output.append("KMETA_REG_NAME(F32);\n");
	Output.append("KMETA_REG_NAME(F64);\n");
	Output.append("KMETA_REG_NAME(bool);\n");

	for (size_t i = 0; i < Cls.size(); i++) {
		Output.append("KMETA_REG_NAME(" + Cls[i].name + ");\n");
	}

	for (size_t i = 0; i < Cls.size(); i++) {
		Output.append("KMETA_REG_NAME(" + Enm[i].name + ");\n");
	}

	Output.append("}\n");
}

int main(int argc, char* argv[]) {

	if (argc < 4) {
		printf("missing input parameter\nusage: khparser.exe -i [includeDirectory] -o [outputDirectory]");
		return 0;
	}

	std::string baseadr;
	std::string outadr;
	for (int i = 0; i < argc; i++) {
		if (std::string(argv[i]) == "-i" && i < (argc - 1)) {
			baseadr = argv[i + 1];
		} else if (std::string(argv[i]) == "-o" && i < (argc - 1)) {
			outadr = argv[i + 1];
		}
	}

	std::vector<std::string> hadrs;
	std::vector<MClass> cls;
	std::vector<MEnum> ens;
	std::stack<std::string> dirstack;
	tinydir_dir dir;
	tinydir_open(&dir, baseadr.c_str());

	unsigned int fcounter = 0;
	unsigned int dcounter = 0;
	unsigned int gcounter = 0;
	unsigned int skipped = 0;
	while (true) {
		if (dir.has_next) {
			tinydir_file file;
			tinydir_readfile(&dir, &file);

			if (file.is_dir) {
				if (std::string(file.name) != "." && std::string(file.name) != "..") {
					dirstack.push(file.path);
					++dcounter;
				}

			} else if ((std::string(file.name).find(".khgen.") != std::string::npos) ||
					   (std::string(file.name).find(".cpp") != std::string::npos)||
					   (std::string(file.path).find("removed") != std::string::npos)) {
				printf("skipped file: %s\n", file.path);
				++skipped;

			} else {
				printf("parse file: %s\n", file.path);
				if (procFile(std::string(file.path), outadr, cls, ens)) {
					hadrs.push_back(std::string(file.path).substr(baseadr.length() + 1)); // + 1 for "/"
					++gcounter;
				}
				++fcounter;
			}

			tinydir_next(&dir);
		} else {
			if (!dirstack.empty()) {
				std::string top = dirstack.top();
				dirstack.pop();
				tinydir_open(&dir, top.c_str());
			} else {
				break;
			}
		}

	}

	tinydir_close(&dir);

	// create register function
	std::string header;
	std::string source;
	hadrs.push_back("kmeta.khgen.h");
	if (!cls.empty() || !ens.empty()) {
		createHead(header);
		createSource(hadrs, cls, ens, source);
	}

	std::ofstream out(outadr + "/kmeta.khgen.h");
	out << header;
	out.close();

	out.open(outadr + "/kmeta.khgen.cpp");
	out << source;
	out.close();

	printf("\n---------[ Kite header parser log ]---------\n%u\tfile(s) parsed\n%u\tdirectory searched\n%u\tfile skipped\n%u\tfile generated\n",
		   fcounter, dcounter, skipped, gcounter);

	return 0;
}