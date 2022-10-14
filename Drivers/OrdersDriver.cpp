#include "sources/Orders/Orders.h"

using namespace std;

void testOrdersLists() {
    //creating order objects
    Orders* de = new Deploy();
    Orders* ad = new Advance();
    Orders* bo = new Bomb();
    Orders* bl = new Blockade();
    Orders* ai = new Airlift();
    Orders* ne = new Negotiate();

    //adding the orders to the orders list
    OrdersList* ol = new OrdersList();
    cout << "Added Deploy Order to Orderslist.";
    ol->add(de);
    cout << endl << "Added Advance Order to Orderslist.";
    ol->add(ad);
    cout << endl << "Added Bomb Order to Orderslist.";
    ol->add(bo);
    cout << endl << "Added Blockade Order to Orderslist.";
    ol->add(bl);
    cout << endl << "Added Airlift Order to Orderslist.";
    ol->add(ai);
    cout << endl << "Added Negotiate Order to Orderslist.";
    ol->add(ne);
    cout << endl << endl;

    cout << *ol << endl;

    ol->displayList();

    cout << "(Index's start at 1) Moving order at position 4 to 2." << endl;
    ol->move(4, 2);
    ol->displayList();

    cout << "(Index's start at 1) Removing order at position 3." << endl;
    ol->remove(3);
    ol->displayList();

    cout << "Adding back the order that was just removed." << endl;
    ol->add(ad);
    ol->displayList();

    cout << "Testing validate and execute for orders." << endl;

    for (auto* order: ol->getOrdersList()) {
        if (order->validate()) {
            cout << "Order is valid. ";
            order->execute();
        } else {
            cout << "Order is NOT valid. ";
        }
    }

    //delete the orders list once completed
    delete ol;
}