#include "sources/Orders/Orders.h"
#include "sources/Player/Player.h"
#include "sources/Map/Map.h"
#include "sources/Cards/Cards.h"
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

void testOrderExecution(){
    Deck* deck = new Deck();
    cout << deck->cardSize() << endl;
    Player* p1 = new Player("p1");
    Player* p2 = new Player("p2");
    Player* p3 = new Player("p3");
    Player* p4 = new Player("p4");

    Territory* t1 = new Territory(0,"t1",p1);
    Territory* t1_1 = new Territory(0,"t1",p1);
    Territory* t2 = new Territory(0,"t2",p2);
    Territory* t3 = new Territory(0,"t3",p3);
    Territory* t3_2 = new Territory(0,"t3",p3);
    Territory* t3_3 = new Territory(0,"t3",p3);
    Territory* t4 = new Territory(0,"t3",p4);
    p1->addTerritory(t1);

    t1->addAdjacentTerritory(t1_1);
    t1_1->addAdjacentTerritory(t1);

    t1->addAdjacentTerritory(t3);
    t3->addAdjacentTerritory(t1);
    t1->addAdjacentTerritory(t4);
    t4->addAdjacentTerritory(t1);

    t3->setNumberOfArmies(5);
    t4->setNumberOfArmies(5);


    /**DEPLOY */

    //deploy invalid due to player not owning the target
    Deploy* deploy_invalid = new Deploy(p1,10,t4);
    deploy_invalid->execute();

    Deploy* deploy= new Deploy(p1,10,t1_1);
    deploy->execute();


    /** BOMB */

//    Bomb* bomb_invalid = new Bomb(p1,t3_2);
//    bomb_invalid->execute();
//
//    Bomb* bomb = new Bomb(p1,t3);
//    bomb->execute();

    /** Advance */

//    invalid advance: territories not adjacent
//    Advance* invalid_advance_1 = new Advance(p1,12,t1,t1_1,deck);
//    invalid_advance_1->execute();

//    valid advance: move army between territories
//    Advance* advance_move = new Advance(p1,12,t1,t1_1,deck);
//    advance_move->execute();
//
//    cout << who owns t3?  << endl;
//    cout << t3->getPlayerName() << endl;
//    Advance* advance = new Advance(p1,30,t1,t3,deck);
//    advance->execute();
//    cout << who owns t3?  << endl;
//    cout << t3->getPlayerName() << endl;
//    cout << how many cards are on the deck?  << endl;
//    cout << deck->cardSize() << endl;

    /** negotiate */

//    Negotiate* negotiate = new Negotiate(p1,p4);
//    negotiate->execute();
//
//    Advance* advance_negotiate = new Advance(p1,30,t1,t4,deck);
//    advance_negotiate->execute();

    /**blockade */

//    Blockade* blockade_invalid = new Blockade(p1,t4);
//    blockade_invalid->execute();
//
//    Blockade* blockade = new Blockade(p1,t1_1);
//    blockade->execute();
//
//    cout <<  who owns t1_1?  << endl;
//    cout <<  t1_1->getPlayerName() << endl;

    /** Airlift */

//    Airlift* airlift_invalid = new Airlift(p1,10,t1,t4);
//    airlift_invalid->execute();
//
//    Airlift* airlift = new Airlift(p1,10,t1,t1_1);
//    airlift->execute();


}