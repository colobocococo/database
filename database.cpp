#include "my_tables.h"

//delete first symbol
void cut_begin(string & a) {
    a = a.substr(1);
}

//convert string to int
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

bool check_int(char p) {
    return '0' <= p && p <= '9';
}

//column
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

//condition plus logic symbol
struct condition {
    string add;
    vc <string> arr;
};

//arithmetic expression
struct number {
    char op;
    int val;
};

//number operation
void res (number & a, number & b) {
    char op = b.op;
    if (op == '+') a.val += b.val;
    if (op == '-') a.val -= b.val;
    if (op == '*') a.val *= b.val;
    if (op == '/') a.val /= b.val;
    if (op == '%') a.val %= b.val;
    b.op = 'x';
}

//recalculate array of numbers
void refill(vc <number> & arr, char op) {
    for (int l = 0; l+1 < arr.size(); l++) {
        if (arr[l].op == 'x') continue;
        for (int r = l+1; r < arr.size() && (arr[r].op == op || arr[r].op == 'x'); r++) {
            res(arr[l], arr[r]);
        }
    }
}

//calculate all of the numbers
int sum_int(vc <number> & arr) {
    for (char op: {'*', '/', '%', '-', '+'}) {
        refill(arr, op);
    }

    return arr[0].val;
}

set <string> used_names; //names of columns
class table {
public:
    string name;
    map <string, int> index; //index of column by name
    int size = 0;
    vc <column> data;

    void insert(column x) {
        data.pb(x);
        index[x.name] = size++;
        used_names.insert(x.name);
    }

    void pop_back() {
        data.pop_back();
        size--;
    }

    //add new string to table
    void insert_str(vc <string> s) {
        for (int i = 0; i < size; i++) {
            data[i].insert(s[i]);
        }
    }

    //erase string
    void erase_row(int v) {
        for (int i = 0; i < size; i++) {
            data[i].erase(v);
        }
    }

    //erase column
    void erase_col(int v) {
        vc <column> new_data;
        for (int i = 0; i < size; i++) {
            if (i != v) new_data.pb(data[i]);
        }
        size--;
        data = new_data;
    }

    //find column index by name
    int col_ind(string s) {
        return index[s];
    }

    //check condition in string
    bool check1(int ind, vc <string> s) {
        bool flag = s[0][0] == '|' || data[col_ind(s[0])].type || check_int(s[0][0]); //finding type 1 for int 0 for string
        if (s.size() == 1) { //if bool
            return data[col_ind(s[0])].data[ind] == "true";
        }

        //split;
        bool sign_exists = false;
        string sign; //find operator of comparison
        vc <string> q, w; //split into two operands
        if (flag) q.pb("+"), w.pb("+");
        for (int i = 0; i < s.size(); i++) {
            //if it's an operator
            if (operators.count(s[i])) {
                sign_exists = true;
                sign = s[i];
                continue;
            }

            if (!sign_exists) q.pb(s[i]); //first half
            else w.pb(s[i]); //second half
        }

        if (flag) { //integer
            vc <number> arr_q, arr_w;
            for (int i = 0; i < q.size(); i += 2) {
                string a = q[i], b = q[i+1]; //creating number with operation and integer
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

            //same for the second half
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

            //check condition
            if (sign == les) return a < b;
            if (sign == mor) return a > b;
            if (sign == les_e) return a <= b;
            if (sign == mor_e) return a >= b;
            if (sign == neq) return a != b;
            if (sign == eq) return a == b;
            return false;
        }

        //for strings it's much simpler
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

    //check all conditions
    bool check(int ind, vc <condition> arr) {
        queue <pair <string, bool>> plus;
        for (int i = 0; i < arr.size(); i++) {
            bool new_cond = check1(ind, arr[i].arr); //checker for each column
            //cout << arr[i].add << ' ';
            plus.push({arr[i].add, new_cond});
        }

        bool ans = false; //firstly we calculate && symbol than ||
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
            try_delete[i] = !nms.count(data[i].name); //delete all columns that weren't selected
        }

        //check all strings
        vc <int> to_delete;
        for (int i = 0; i < data[0].size; i++) {
            if (!check(i, arr)) to_delete.pb(i);
        }

        //delete which does not follow the condition
        for (int i = to_delete.size() - 1; i >= 0; i--) {
            int v = to_delete[i];
            erase_row(v);
        }

        //delete not selected columns
        for (int i = size - 1; i >= 0; i--) {
            if (try_delete[i]) erase_col(i);
        }
    }

    void delete_rows(vc <condition> arr) {
        for (int i = data[0].size - 1; i >= 0; i--) {
            if (check(i, arr)) erase_row(i);
        }
    }

    //update string if condition
    void update1(int ind, vc <string> ups) {
        //for (string s: ups) cout << s << ' ';
        //cout << "\n";
        //return;
        string nm = ups[0]; //name col
        ups[1] = "+";   //instead of "="
        if (data[col_ind(nm)].type) {
            //same syntax as ckeck1
            vc <number> q;
            for (int i = 1; i < ups.size(); i += 2) {
                string a = ups[i], b = ups[i+1];
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
                q.pb(number{a[0], val});
            }

            data[col_ind(nm)].data[ind] = to_string(sum_int(q));
        }
        else {
            //string concatenation
            string s;
            for (int i = 2; i < ups.size(); i += 2) s += ups[i];
            data[col_ind(nm)].data[ind] = s;
        }
    }

    void update(vc <vc <string>> ups, vc <condition> & arr) {
        for (int i = 0; i < data[0].size; i++) {
            if (check(i, arr)) {
                for (vc <string> s: ups) update1(i, s); //if condition follows we update string
            }
        }
    }

    //show table
    void print() {
        for (int i = 0; i < size; i++) cout << data[i].name << ' ';
        for (int i = 0; i < data[0].size; i++) {
            cout << "\n";
            for (int j = 0; j < size; j++) cout << data[j].data[i] << ' ';
        }
        cout << "\n";
    }
};

//tables which exist
map <string, table> tables;

//condition parser
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
