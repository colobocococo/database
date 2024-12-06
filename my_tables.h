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

int to_int(string s) {
    int ans = 0;
    for (int i = 0; i < s.length(); i++) {
        ans *= 10;
        ans += int(s[i] - '0');
    }
    return ans;
}

bool check_condition(string s, bool type, string sign, string val) {
    if (sign == eq) return s == val;
    if (sign == neq) return s != val;
    if (type) {
        int a = to_int(s), b = to_int(val);
        if (sign == les) return a < b;
        if (sign == mor) return a > b;
        if (sign == les_e) return a <= b;
        if (sign == mor_e) return a >= b;
    }

    if (sign == les) return s < val;
    if (sign == mor) return s > val;
    if (sign == les_e) return s <= val;
    if (sign == mor_e) return s >= val;
    return false;
}

class column {
public:
    string name;
    bool type;  //0 for string, 1 for int/bool
    int size = 0;
    vc <string> data;

    void pop_back() {
        data.pop_back();
        size--;
    }

    void insert(string val) {
        data.pb(val);
        size++;
    }

    void erase(int v) {
        vc <string> new_data;
        for (int i = 0; i < size; i++) {
            if (i != v) new_data.pb(data[i]);
        }
        data = new_data;
        size--;
    }
};

struct condition {
    string add;
    string name;
    string sign;
    string value;
};

class table {
public:
    string name;
    int size = 0;
    vc <column> data;

    void insert(column x) {
        data.pb(x);
        size++;
    }

    void pop_back() {
        data.pop_back();
        size--;
    }

    void insert_str(vc <string> s) {
        for (int i = 0; i < size; i++) {
            data[i].insert(s[i]);
        }
    }

    void erase_row(int v) {
        for (int i = 0; i < size; i++) {
            data[i].erase(v);
        }
    }

    void erase_col(int v) {
        vc <column> new_data;
        for (int i = 0; i < size; i++) {
            if (i != v) new_data.pb(data[i]);
        }
        size--;
        data = new_data;
    }

    bool check(int ind, vc <condition> arr) {
        bool ans = true;
        int cur = 0;
        for (int i = 0; i < size; i++) {
            if (arr[cur].name == data[i].name) {
                bool new_cond = check_condition(data[i].data[ind], data[i].type, arr[cur].sign, arr[cur].value);
                string add = arr[cur].add;
                if (add == "&&") ans &= new_cond;
                if (add == "||") ans |= new_cond;
            }
        }

        //cout << ans << ' ';
        return ans;
    }

    void select(set <string> nms, vc <condition> & arr) {
        vc <bool> try_delete(size);
        for (int i = 0; i < size; i++) {
            try_delete[i] = !nms.count(data[i].name);
        }

        vc <int> to_delete;
        for (int i = 0; i < data[0].size; i++) {
            if (!check(i, arr)) to_delete.pb(i);
        }

        for (int i = to_delete.size() - 1; i >= 0; i--) {
            int v = to_delete[i];
            erase_row(v);
        }

        for (int i = size - 1; i >= 0; i--) {
            if (try_delete[i]) erase_col(i);
        }
    }

    void delete_rows(vc <condition> & arr) {
        for (int i = data[0].size - 1; i >= 0; i--) {
            if (check(i, arr)) erase_row(i);
        }
    }

    void print() {
        for (int i = 0; i < size; i++) cout << data[i].name << ' ';
        for (int i = 0; i < data[0].size; i++) {
            cout << "\n";
            for (int j = 0; j < size; j++) cout << data[j].data[i] << ' ';
        }
        cout << "\n";
    }
};

map <string, table> tables;
set <string> names;

void cut_end(string & x) {
    x = x.substr(0, x.size() - 1);
}

void cut_begin(string & x) {
    x = x.substr(1);
}

table create_table() {
    table x;
    cin >> x.name;
    string a = "x";
    while (a.end()[-1] != bracket_close) {
        cin >> a;
        if (a[0] == bracket_open) cut_begin(a);
        if (a[0] == '{') {
            cut_begin(a);
            while (a.end()[-1] != figure_close) { //reading attributes
                cin >> a;
                if (a[0] == 'u') {}
                if (a[0] == 'a') {}
                if (a[0] == 'k') {}
            }
            cin >> a;
        }

        //cout << a << "\n";

        column h;
        h.name = a;

        cin >> a;
        h.type = true;
        if (a[0] == 's' || a[1] == 'y') h.type = false;
        else if (a == "bool") cin >> a, cin >> a; //

        cut_end(h.name);
        names.insert(h.name);
        x.insert(h);
    }

    return x;
}

void insert() {
    string a;
    vc <string> add;

    int v = 0;
    cin >> a;
    while (a.back() != bracket_close) {
        while (a[0] == bracket_open || a[0] == ',') cut_begin(a);
        if (a.empty()) {
            cin >> a;
            continue;
        }
        if (names.count(a)) cin >> a >> a;
        cut_end(a);
        add.pb(a);
        cin >> a;
    }
    cut_end(a);
    add.pb(a);
    //cout << add.size();

    cin >> a >> a;

    table x = tables[a];
    for (int i = 0; i < add.size(); i++) {
        x.data[i].insert(add[i]);
    }

    tables[x.name] = x;
    //x.print();
}

void select() {
    //cout << "selecting...\n";
    set <string> col_names;
    string a;
    cin >> a;
    while (a.back() == ',') {
        cut_end(a);
        col_names.insert(a);
        cin >> a;
    }
    col_names.insert(a);

    //cout << "all columns\n";
    cin >> a >> a; //from *name_table*
    //cout << "name " << a << "\n";
    table x = tables[a];
    cin >> a; //where

    //cout << a << ' ';
    //now conditions
    vc <condition> cnd;

    string add = "&&", name, sgn, val;
    cin >> name >> sgn >> val;
    while (val.back() != ask_close) {
        cnd.pb(condition{name, sgn, val});

        cin >> add >> name >> sgn >> val;
    }
    cut_end(val);
    cnd.pb(condition{add, name, sgn, val});
    x.select(col_names, cnd);
    tables[x.name] = x;
}

void update() {

}

void erase() {
    string a;
    cin >> a;
    table x = tables[a];
    cin >> a;

    vc <condition> cnd;
    string add = "&&", name, sgn, val;
    cin >> name >> sgn >> val;
    while (val.back() != ask_close) {
        cnd.pb(condition{name, sgn, val});

        cin >> add >> name >> sgn >> val;
    }
    cut_end(val);
    cnd.pb(condition{add, name, sgn, val});
    x.delete_rows(cnd);
    tables[x.name] = x;
}

void check() {
    string s;
    cin >> s;
    if (s == "create") {
        cin >> s;
        table x = create_table();
        tables[x.name] = x;
    }

    if (s == "insert") insert();
    if (s == "select") select();
    if (s == "delete") erase();
    if (s == "update") update();
}

void show() {
    string name;
    cin >> name;
    tables[name].print();
}
