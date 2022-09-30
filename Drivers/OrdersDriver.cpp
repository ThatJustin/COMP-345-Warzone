
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
    cout<<"Added Deploy:"<<endl;
    ol->add(de);
    cout<<endl<<"Added Advance: "<<endl;
    ol->add(ad);
    cout<<endl<<"Added Bomb: "<<endl;
    ol->add(bo);
    cout<<endl<<"Added Blockade: "<<endl;
    ol->add(bl);
    cout<<endl<<"Added Airlift: "<<endl;
    ol->add(ai);
    cout<<endl<<"Added Negotiate: "<<endl;
    ol->add(ne);
    cout<<endl;

    ol->displayList();
    cout<<endl;
    cout<<"/////////////////////////////////////"<<endl;
    ol->move(4, 2);
    ol->displayList();


    delete ol;
}