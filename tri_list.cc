#include "tri_list.h"



int main() {

    tri_list<int, float, bool> l3 = {true, 4.f, true, 5.f, true, 1, 1, 1};
    l3.push_back(0);

    tri_list<int, float, bool> l({1, 2, true, 4.f});

    l.modify_only<int>([] (int x) {return x * 2;});
    //l.modify_only<int, decltype([] (int x) {return x + 2;})>();
    return 0;
}