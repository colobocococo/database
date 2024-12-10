#include "my_tables.h"
int to_int(string s) {
    if (s == "false") return 0;
    if (s == "true") return 1;
    if (s[0] == '|') return s.length() - 2;
    int ans = 0;
    for (int i = 0; i < s.length(); i++) {
        ans *= 10;
        ans += int(s[i] - '0');
    }
    return ans;
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
    vc <string> arr;
};

struct number {
    char op;
    int val;
};

void res (number & a, number & b) {
    char op = b.op;
    if (op == '+') a.val += b.val;
    if (op == '-') a.val -= b.val;
    if (op == '*') a.val *= b.val;
    if (op == '/') a.val /= b.val;
    if (op == '%') a.val %= b.val;
    b.op = 'x';
}

void refill(vc <number> & arr, char op) {
    for (int l = 0; l+1 < arr.size(); l++) {
        if (arr[l].op == 'x') continue;
        for (int r = l+1; r < arr.size() && (arr[r].op == op || arr[r].op == 'x'); r++) {
            res(arr[l], arr[r]);
        }
    }
}

int sum_int(vc <number> & arr) {
    for (char op: {'*', '/', '%', '-', '+'}) {
        refill(arr, op);
    }

    return arr[0].val;
}

class table {
public:
    string name;
    map <string, int> index;
    int size = 0;
    vc <column> data;

    void insert(column x) {
        data.pb(x);
        index[x.name] = size++;
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

    int col_ind(string s) {
        return index[s];
    }

    bool check1(int ind, vc <string> s) {

        bool flag = s[0][0] == '|' || data[col_ind(s[0])].type;
        if (s.size() == 1) {
            return data[col_ind(s[0])].data[ind] == "true";
        }

        //split;
        bool sign_exists = false;
        string sign;
        vc <string> q, w;
        if (flag) q.pb("+"), w.pb("+");
        for (int i = 0; i < s.size(); i++) {
            if (operators.count(s[i])) {
                sign_exists = true;
                sign = s[i];
                continue;
            }

            if (!sign_exists) q.pb(s[i]);
            else w.pb(s[i]);
        }

        if (flag) {
            vc <number> arr_q, arr_w;
            for (int i = 0; i < q.size(); i += 2) {
                string a = q[i], b = q[i+1];
                int val;
                if (b[0] == '|') {
                    cut_begin(b);
                    b.pop_back();
                    if (!index.count(b)) val = b.length();
                    else val = int(data[col_ind(b)].data[ind].length());
                }
                else {
                    if (!index.count(b)) val = to_int(b);
                    else val = to_int(data[col_ind(b)].data[ind]);
                }
                arr_q.pb(number{a[0], val});
            }

            for (int i = 0; i < w.size(); i += 2) {
                string a = w[i], b = w[i+1];
                int val;
                if (b[0] == '|') {
                    cut_begin(b);
                    b.pop_back();
                    if (!index.count(b)) val = b.length();
                    else val = int(data[col_ind(b)].data[ind].length());
                }
                else {
                    if (!index.count(b)) val = to_int(b);
                    else val = to_int(data[col_ind(b)].data[ind]);
                }
                arr_w.pb(number{a[0], val});
            }

            int a = sum_int(arr_q), b = sum_int(arr_w);
            //cout << a << sign << b << "\n";

            if (sign == les) return a < b;
            if (sign == mor) return a > b;
            if (sign == les_e) return a <= b;
            if (sign == mor_e) return a >= b;
            if (sign == neq) return a != b;
            if (sign == eq) return a == b;
            return false;
        }

        string sq, sw;
        for (string a: q) {
            if (a != "+"){
                if (index.count(a)) sq += data[col_ind(a)].data[ind];
                else sq += a;
            }
        }
        for (string a: w) {
            if (a != "+") {
                if (index.count(a)) sw += data[col_ind(a)].data[ind];
                else sw += a;
            }
        }

        string a = sq, b = sw;
        if (sign == les) return a < b;
        if (sign == mor) return a > b;
        if (sign == les_e) return a <= b;
        if (sign == mor_e) return a >= b;
        if (sign == neq) return a != b;
        if (sign == eq) return a == b;
        return false;
    }

    bool check(int ind, vc <condition> arr) {
        queue <pair <string, bool>> plus;
        for (int i = 0; i < arr.size(); i++) {
            bool new_cond = check1(ind, arr[i].arr);
            //cout << arr[i].add << ' ';
            plus.push({arr[i].add, new_cond});
        }

        bool ans = false;
        while (!plus.empty()) {
            bool cond = plus.front().second;
            plus.pop();
            while (!plus.empty() && plus.front().first == "&&") {
                cond &= plus.front().second;
                plus.pop();
            }
            ans |= cond;
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

vc <condition> read(const string add = "||") {
    string a;
    vc <string> s;
    cin >> a;
    while (a.back() != ';' && a != "&&" && a != "||") {
        s.pb(a);
        cin >> a;
    }

    vc <condition> ans;
    if (a.back() != ';') {
        ans = read(a);
    }
    else {
        a.pop_back();
        s.pb(a);
    }

    ans.pb(condition{add, s});
    return ans;
}

void cut_end(string & x) {
    x = x.substr(0, x.size() - 1);
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
    vc <condition> cnd = read();
    reverse(all(cnd));
    x.select(col_names, cnd);
    tables[x.name] = x;
    x.print();
}

void erase() {
    string a;
    cin >> a;
    table x = tables[a];
    cin >> a;

    vc <condition> cnd = read();
    reverse(all(cnd));
    x.delete_rows(cnd);
    tables[x.name] = x;
    x.print();
}

void update() {

}

void check() {
    string s;
    cin >> s;
    for (int i = 0; i < s.length(); i++) if (s[i] < 'a') s[i] += 32;
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
