#include "sources/Orders/Orders.h"

using namespace std;

void testOrdersLists(){
    //creating order objects
    Orders* de = new Deploy();
    Orders* ad = new Advance();
    Orders* bo = new Bomb();
    Orders* bl = new Blockade();
    Orders* ai = new Airlift();
    Orders* ne = new Negotiate();

    //testing validation
    de->validate();
    ad->validate();
    bo->validate();
    bl->validate();
    ai->validate();
    ne->validate();

    //adding the orders to the orders list
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
    cout<<endl;

    //moving the 4th order to the 2nd position
    ol->move(4, 2);
    ol->displayList();

    cout<<endl;

    //removing the order in the 3rd position
    ol->remove(3);
    cout<<"/////////////////////////////////////"<<endl;
    ol->displayList();

    //adding back the removed order
    ol->add(ad);
    ol->displayList();

    cout<<endl;
    cout<<"/////////////////////////////////////"<<endl;
    cout<<endl;

    //for loop through the orders list to execute them all
    for(auto i=0;i<ol->getOrdersList().size();i++){
        ol->getOrdersList().at(i)->execute();
    }

    //delete the orders list once completed
    delete ol;
}