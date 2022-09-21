
#include "sources/Orders/Orders.h"

using namespace std;
void testOrdersLists(){
    Orders* de = new Deploy();
    Orders* ad = new Advance();
    Orders* bo = new Bomb();
    Orders* bl = new Blockade();
    Orders* ai = new Airlift();
    Orders* ne = new Negotiate();

    OrdersList* ol = new OrdersList();
    ol->add(de);
    ol->add(ad);
    ol->add(bo);
    ol->add(bl);
    ol->add(ai);
    ol->add(ne);


    delete ol;
}