#include "database.cpp"

using namespace std;

//creAte table users (id: int, login: string, bytes: bytes, is_there: bool)
//iNsErt (, 9, "coloboc343", 0x357644, true) to users
//iNsERt (, 7, "ColobocLSR", 0x557544, false) to users
//deletE users where id < 9;
//seleCT id, is_there from users where id < 8 || is_there == false;
//select login from users where login == "coloboc + 343";
//delete users where login == "coloboc + 343";
//creAte table users (id: int, login: string, bytes: bytes, is_there: bool)
//iNsErt (, 9, "coloboc343", 0x357644, true) to users
//iNsERt (, 7, "ColobocLSR", 0x557544, false) to users
//delete users where |login| == |"coloboc666"|
void solve () {
    int tm = 4;
    while (tm--) check();
}

int main() {
    //ios::sync_with_stdio(false);cin.tie(nullptr);cout.tie(nullptr);
    int test = 1;
    //cin >> test;
    while (test--) solve();
    return 0;
}
