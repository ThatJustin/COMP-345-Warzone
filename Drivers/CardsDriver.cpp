#include "iostream"
#include "sources/Cards/Cards.h"
#include "sources/Player/Player.h"
#include "vector"

void testCards() {


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

    Deck* deck = new Deck(deckCards);
    Hand* hand = new Hand();
    cout << "Before Deck Draw" << endl;;

    std::cout << *deck << std::endl;
    std::cout << *hand << std::endl;

    //Simulate drawing from the deck
    deck->draw(hand);
    deck->draw(hand);
    deck->draw(hand);
    deck->draw(hand);
    deck->draw(hand);

    cout << "After Deck draw " << endl;;

    std::cout << *deck << std::endl;
    std::cout << *hand << std::endl;

    //Play the cards
    for (auto handCards: hand->getCards()) {
        handCards->play(ordersList, hand, deck);
    }

    cout << "\r\nAfter Card Play " << endl;;

    std::cout << *hand << std::endl;
    std::cout << *deck << std::endl;

    // clean up

    delete deck;
    delete hand;
    delete ordersList;
}