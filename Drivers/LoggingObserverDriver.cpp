#include "sources/GameEngine/CommandProcessor.h"
#include "sources/GameEngine/GameEngine.h"
#include "sources/Orders/Orders.h"

void testLoggingObserver(){
    Orders* de = new Deploy();
    Orders* ad = new Advance();
    Orders* bo = new Bomb();
    Orders* bl = new Blockade();
    Orders* ai = new Airlift();
    Orders* ne = new Negotiate();

    OrdersList *ol = new OrdersList();
    ol->addOrder(de);
    ol->addOrder(ad);
    ol->addOrder(bo);
    ol->addOrder(bl);
    ol->addOrder(ai);
    ol->addOrder(ne);

}