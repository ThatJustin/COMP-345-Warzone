#include "sources/Orders/Orders.h"
#include "sources/Player/Player.h"
#include "sources/Map/Map.h"
#include "sources/Cards/Cards.h"

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

void testOrderExecution() {
    Deck* deck = new Deck();
    cout << "Setting up test data..." << endl << endl;
    Player* p1 = new Player("Player1");
    Player* p2 = new Player("Player2");
    Player* p3 = new Player("Player3");
    Player* p4 = new Player("Player4");

    //Mimicking Neutral PLayer from GameEngine
    Player* neutral = new Player("Neutral");

    Territory* t1 = new Territory(0, "t1", p1);
    Territory* t1_1 = new Territory(0, "t1", p1);
    Territory* t1_2 = new Territory(0, "t1_2", p1);
    Territory* t2 = new Territory(0, "t2", p2);
    Territory* t3 = new Territory(0, "t3", p3);
    Territory* t3_2 = new Territory(0, "t3", p3);
    Territory* t3_3 = new Territory(0, "t3", p3);
    Territory* t4 = new Territory(0, "t3", p4);
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
    cout << "Testing invalid Deploy order." << endl;
    Deploy* deploy_invalid = new Deploy(p1, 10, t4);
    deploy_invalid->execute();
    cout << endl;
    cout << "Testing valid Deploy order." << endl;
    Deploy* deploy = new Deploy(p1, 10, t1_1);
    deploy->execute();
    cout << endl;

    /** BOMB */

    cout << "Testing invalid Bomb order." << endl;
    Bomb* bomb_invalid = new Bomb(p1, t3_2);
    bomb_invalid->execute();
    cout << endl;
    cout << "Testing valid Bomb order." << endl;
    Bomb* bomb = new Bomb(p1, t3);
    bomb->execute();
    cout << endl;
    /** Advance */

    cout << "Testing invalid advance order (invalid advance: territories not adjacent)" << endl;
    Advance* invalid_advance_1 = new Advance(p1, 12, t1, t2, deck);
    invalid_advance_1->execute();
    cout << endl;

    cout << "Testing valid advance order (valid advance: move army between territories)" << endl;
    t1->setNumberOfArmies(20);
    t1_1->setNumberOfArmies(33);
    Advance* advance_move = new Advance(p1, 12, t1, t1_1, deck);
    advance_move->execute();
    cout << endl;

    cout << "Testing successfully conquering a territory" << endl;
    cout << "How many cards are in the deck? " << deck->cardSize() << endl;
    cout << "Who owns t3? " << t3->getPlayerName() << endl;
    Advance* advance = new Advance(p1, 30, t1, t3, deck);
    advance->execute();
    cout << "Who owns t3? " << t3->getPlayerName() << endl;
    cout << "How many cards are in the deck after conquering a territory? " << deck->cardSize() << endl;
    cout << endl;

    cout << "Testing invalid Advance order (they are negotiating)" << endl;
    p1->setNegotiationWith(t4->getTerritoryOwner());
    t4->getTerritoryOwner()->setNegotiationWith(p1);
    Advance* advance_negotiate = new Advance(p1, 30, t1, t4, deck);
    advance_negotiate->execute();
    cout << endl;

    /** negotiate */

    cout << "Testing valid Negotiate order." << endl;
    //Order is valid.
    Negotiate* negotiate = new Negotiate(p1, p4);
    negotiate->execute();
    cout << endl;

    cout << "Testing invalid Negotiate order." << endl;
    //Failed to execute order, cannot negotiate with yourself.
    Negotiate* negotiate_invalid = new Negotiate(p1, p1);
    negotiate_invalid->execute();
    cout << endl;

    /**blockade */

    cout << "Testing invalid Blockade order." << endl;
    Blockade* blockade_invalid = new Blockade(p1, neutral, t4);
    blockade_invalid->execute();
    cout << endl;

    cout << "Testing valid Blockade order." << endl;
    Blockade* blockade = new Blockade(p1, neutral, t1_1);
    blockade->execute();
    cout << "Who owns the territory now? " << t1_1->getPlayerName() << endl;
    cout << endl;

    /** Airlift */

    cout << "Testing invalid Airlift order." << endl;
    Airlift* airlift_invalid = new Airlift(p1, 10, t1, t4);
    airlift_invalid->execute();
    cout << endl;

    cout << "Testing valid Airlift order." << endl;
    Airlift* airlift = new Airlift(p1, 10, t1, t1_2);
    airlift->execute();
    cout << endl;

    delete deck;
    delete p1;
    delete p2;
    delete p3;
    delete p4;
    delete neutral;

    delete t1;
    delete t1_1;
    delete t1_2;
    delete t2;
    delete t3;
    delete t3_2;
    delete t3_3;
    delete t4;

}