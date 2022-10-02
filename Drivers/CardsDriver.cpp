#include "iostream"
#include "sources/Cards/Cards.h"
#include "vector"

//A free function named testCards()
void testCards() {

    //creates a deck of cards of all different kinds
    std::cout <<std::endl;
    std::vector<Cards*> DeckCards;
    Cards* bomb = new Cards (CardsType::BOMB); //create type bomb
    Cards* reinforcement = new Cards (CardsType::REINFORCEMENT); //create type reinforcement
    Cards* blockade = new Cards(CardsType::BLOCKADE); //create type blockade
    Cards* airlift = new Cards(CardsType::AIRLIFT); //create type airlift
    Cards* diplomacy = new Cards(CardsType::DIPLOMACY); //create type diplomacy

    OrdersList* ordersList = new OrdersList(); //create the orderlist

    DeckCards.push_back(bomb); //add type bomb to the deckcards list
    DeckCards.push_back(reinforcement); //add type reinforcement to the deckcards list
    DeckCards.push_back(blockade); //add type blockade to the deckcards list
    DeckCards.push_back(airlift); //add type airlift to the deckcards list
    DeckCards.push_back(diplomacy); //add type diplomacy to the deckcards list

    std::cout <<"There is: "<< DeckCards.size() << " cards in the deck." <<std::endl; //test to see the amount of cards in the deck

    //std::cout <<DeckCards.at(1)->getType("airlift")<<std::endl;//test to see if the type airlift is in the deck
    std::cout <<std::endl;

    //creates a hand object that is filled by drawing cards from the deck by repeatedly calling its draw() method
    Hand* hand = new Hand();
    Deck* deck = new Deck( DeckCards);

    //test cards being put from deck to hand
    std::cout <<*hand<<std::endl; //see how many cards are in the hand
    std::cout <<*deck<<std::endl; //see how many card are in the deck
    hand->addcards(deck->draw()); //add card drawn from deck into hand
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
    std::cout <<std::endl;

    //then calls the play() method of all cards in the hand, resulting in the cards being put back in the deck.
    bomb->Play(ordersList,hand,deck);//get the cards type, remove from hand, put in deck
    reinforcement->Play(ordersList,hand,deck);
    blockade->Play(ordersList,hand,deck);
    airlift->Play(ordersList,hand,deck);
    diplomacy->Play(ordersList,hand,deck);
    std::cout << "The cards are played: " <<std::endl;
    std::cout << "There are: " <<*ordersList<<std::endl;
    std::cout <<std::endl;

    std::cout <<*hand <<" After the play method was called." <<std::endl;
    std::cout <<"After play is called: " <<*deck << " Back in the deck."<<std::endl;

    //handle leaks for the cards type
    delete bomb;
    delete reinforcement;
    delete blockade;
    delete airlift;
    delete diplomacy;

    //handle leaks for the classes
    delete hand;
    delete deck;
    delete ordersList;
}
/*
//where all the action occurs
int main() {

    //testing
    testCards();
    return 0;
}*/