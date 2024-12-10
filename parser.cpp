#include "database.cpp"

void create_table() {
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

        h.name.pop_back();
        x.insert(h);
    }
    tables[x.name] = x;
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
        a.pop_back();
        add.pb(a);
        cin >> a;
    }
    a.pop_back();
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
        a.pop_back();
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
    string a;
    cin >> a;
    table x = tables[a];
    cin >> a; //set


}

void check() {
    string s;
    cin >> s;
    for (int i = 0; i < s.length(); i++) if (s[i] < 'a') s[i] += 32;
    if (s == "create") {
        cin >> s;
        create_table();
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
