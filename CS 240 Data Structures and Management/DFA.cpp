#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

void generateTransitionFunction(vector<unordered_map<char, int>>& vec, string pattern, const vector<char>& alphabet) {
	int m = pattern.length(); // # of states
	unordered_map<char, int> map;
	for (char c : alphabet) // c: next read character
	{
		if (c == pattern[0]) continue;
		map[c] = 0;
	}
	map[pattern[0]] = 1;
	vec.push_back(map);
	for (int i = 1; i <= m; i++) // i: current state
	{
		map.clear();
		for (char c : alphabet) // c: next read character
		{
			if (c == pattern[i]) {
				map[c] = i + 1;
				continue;
			}
			string read = pattern.substr(0, i);
			int j;
			for (j = i; j > 0; j--) // j: longest length
			{
				//if (read.length() < 1) break;
				if (pattern.substr(0, j) == read.substr(i - j + 1, j - 1) + c) break;
			}
			map[c] = j;
		}
		vec.push_back(map);
	}
}

void findMatches(vector<int>& res, const vector<unordered_map<char, int>>& vec, string pattern, string text) {
	int state = 0;
	int m = pattern.length();
	char c;
	for (int i = 0; i < text.length(); i++)
	{
		c = text[i];
		unordered_map<char, int> map = vec.at(state);
		state = map[c];
		if (state == m) {
			res.push_back(i - m + 1);
		}
	}
}

void outputTriples(ofstream& o, const vector<unordered_map<char, int>>& vec, const vector<char>& alphabet) {
	for (size_t i = 0; i < vec.size(); i++) {
		for (char c : alphabet) {
			o << i << " " << c << " " << vec[i].at(c) << endl;
		}
	}
}

void outputMatches(ofstream& o, vector<int> res) {
	for (int index : res) {
		o << index << endl;
	}
}

const string patternFileName = "a5pattern.txt";
const string textFileName = "a5text.txt";
const string triplesFileName = "a5delta.txt";
const string matchesFileName = "a5matches.txt";

int main() {
	ifstream inPattern{ patternFileName };
	ifstream inText{ textFileName };
	ofstream outTriples{ triplesFileName };
	ofstream outMatches{ matchesFileName };
	string pattern;
	string text;
	string s;
	getline(inPattern, pattern);
	while (getline(inText, s)) {
		text += s;
	}
	vector<char> alphabet; // alphabet of the pattern
	s = pattern;
	sort(s.begin(), s.end());
	alphabet.push_back(s[0]);
	for (size_t i = 1; i < s.size(); i++) {
		if (s[i] != s[i - 1]) alphabet.push_back(s[i]);
	}
	vector<unordered_map<char, int>> delta;
	vector<int> matches;
	generateTransitionFunction(delta, pattern, alphabet);
	findMatches(matches, delta, pattern, text);
	outputTriples(outTriples, delta, alphabet);
	outputMatches(outMatches, matches);
	return 0;
}
