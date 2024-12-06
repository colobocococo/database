#include "my_tables.h"

using namespace std;


//create table users (id: int, login: string)
//insert (, 9, "coloboc343") to users
//insert (, 7, "ColobocLSR") to users
//delete users where id < 9;
//select id from users where id < 10;
void solve () {
    int tm = 4;
    while (tm--) check();
    show();
}

int main() {
    //ios::sync_with_stdio(false);cin.tie(nullptr);cout.tie(nullptr);
    int test = 1;
    //cin >> test;
    while (test--) solve();
    return 0;
}
