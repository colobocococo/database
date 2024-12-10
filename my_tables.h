#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <cmath>
#include <stack>
#include <bitset>
#include <climits>
#include <string>
using namespace std;


#define vc vector
#define pip pair <int, int>
#define pb push_back
#define ll long long
//#define int long long
#define mp(a, b) make_pair(a, b)
#define all(s) s.begin(), s.end()
#define back() end()[-1]

const char bracket_close = ')';
const char bracket_open = '(';
const char figure_close = '}';
const char figure_open = '{';
const char ask_close = ';';

const string les = "<";
const string mor = ">";
const string les_e = "<=";
const string mor_e = ">=";
const string eq = "==";
const string neq = "!=";

set <string> operators = {les, mor, les_e, mor_e, eq, neq};
set <string> operations = {"+", "-", "%", "/", "*"};

void cut_begin(string & a) {
    a = a.substr(1);
}

