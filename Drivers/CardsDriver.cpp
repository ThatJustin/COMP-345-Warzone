#include "iostream"
#include "sources/Cards/Cards.h"
#include "sources/Player/Player.h"
#include "vector"

//A free function named testCards()
void testCards() {

    std::cout << std::endl;
    Cards* bomb = new Cards(CardsType::BOMB); //create type bomb
    Cards* blockade = new Cards(CardsType::BLOCKADE); //create type blockade
    Cards* reinforcement = new Cards(CardsType::REINFORCEMENT); //create type reinforcement
    Cards* airlift = new Cards(CardsType::AIRLIFT); //create type airlift
    Cards* diplomacy = new Cards(CardsType::DIPLOMACY); //create type diplomacy

    OrdersList* ordersList = new OrdersList(); //create the orderlist
    std::vector<Cards*> deckCards;
    deckCards.push_back(bomb); //add type bomb to the deckcards list
    deckCards.push_back(reinforcement); //add type reinforcement to the deckcards list
    deckCards.push_back(blockade); //add type blockade to the deckcards list
    deckCards.push_back(airlift); //add type airlift to the deckcards list
    deckCards.push_back(diplomacy); //add type diplomacy to the deckcards list

    //creates a hand object that is filled by drawing cards from the deck by repeatedly calling its draw() method
    Hand* hand = new Hand();
    Deck* deck = new Deck(deckCards);

    std::cout << *deck << std::endl;

    //draw a card
    deck->draw(hand);

    std::cout << *deck << std::endl;

    for (auto handCards : hand->getCards()) {
        handCards->play(ordersList, hand, deck);
    }

    std::cout << std::endl;

    std::cout << "The cards are played: " << std::endl;
    std::cout << "There are: " << *ordersList << std::endl;
    std::cout << std::endl;

    std::cout << *hand << " After the play method was called." << std::endl;
    std::cout << "After play is called: " << *deck << " Back in the deck." << std::endl;

    //handle leaks for the classes
    delete hand;
    delete deck;
    delete ordersList;
}