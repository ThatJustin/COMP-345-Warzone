#include "iostream"
#include "sources/Cards/Cards.h"
#include "vector"

//A free function named testCards()
void testCards() {

    //creates a deck of cards of all different kinds
    std::vector<Cards*> DeckCards;
    Cards* bomb = new Cards (CardsType::BOMB);
    Cards* reinforcement = new Cards (CardsType::REINFORCEMENT);
    Cards* blockade = new Cards(CardsType::BLOCKADE);
    Cards* airlift = new Cards(CardsType::AIRLIFT);
    Cards* diplomacy = new Cards(CardsType::DIPLOMACY);

    DeckCards.push_back(bomb);
    DeckCards.push_back(reinforcement);
    DeckCards.push_back(blockade);
    DeckCards.push_back(airlift);
    DeckCards.push_back(diplomacy);

    std::cout <<DeckCards.size()<<std::endl;
    std::cout <<DeckCards.at(1)->getType("airlift")<<std::endl;//testing

    //creates a hand object that is filled by drawing cards from the deck by repeatedly calling its draw() method
    Hand* hand = new Hand();
    Deck* deck = new Deck( DeckCards);

    std::cout <<*hand<<std::endl;
    std::cout <<*deck<<std::endl;
    //deck->addcards(DeckCards.at(1));
    hand->addcards(deck->draw());
    std::cout <<*hand<<std::endl;
    std::cout <<*deck<<std::endl;
    hand->addcards(deck->draw());
    std::cout <<*hand<<std::endl;
    std::cout <<*deck<<std::endl;
    hand->addcards(deck->draw());
    std::cout <<*hand<<std::endl;
    std::cout <<*deck<<std::endl;
    hand->addcards(deck->draw());
    std::cout <<*hand<<std::endl;
    std::cout <<*deck<<std::endl;
    hand->addcards(deck->draw());
    std::cout <<*hand<<std::endl;
    std::cout <<*deck<<std::endl;
    //hand->draw();
    //hand->draw();
    //hand->draw();
    //hand->draw();
    std::cout <<std::endl;

    //then calls the play() method of all cards in the hand, resulting in the cards being put back in the deck.

    //handle leaks
    //Deck::~Deck();
}
/*
//where all the action occurs
int main() {

    //testing
    std::cout << "Hello, World!" << std::endl;

    testCards();
    return 0;
}*/