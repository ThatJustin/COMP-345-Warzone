//
//Implement a group of C++ classes that implements a deck and hand of Warzone cards. Each card has a type
//from: bomb, reinforcement, blockade, airlift and diplomacy. The Deck class must have a draw() method that
//allows a player to draw a card at random from the cards remaining in the deck and place it in their hand of cards.
//The objects of type Card must have a play() method that is called to play the card, which creates an order and
//adds it to the player’s list of orders and then returns the card to the deck. All the classes/functions that you
//implement for this component must all reside in a single .cpp/.h file duo named Cards.cpp/Cards.h. You must
//deliver a file named CardsDriver.cpp file that contains a free function named testCards() that creates a deck
//of Warzone cards, then create a hand object that is filled by drawing cards from the deck.
//

#include <iostream>
#include "Cards.h"
#include <vector>
using namespace std;

//cards default constructor
Cards::Cards(){ //constructor definition outside of the class

};

//cards parametries constructor
Cards::Cards(CardsType type){
    this->type = type;
};

//copy constructor
Cards::Cards(const Cards& cards){
    this->type = cards.type;
};

//prevent leaks
Cards::~Cards(){

//    delete CardsType;
//    delete Deck;
//    delete Hand;
}

//card types
//Each card has a type from: bomb, reinforcement, blockade, airlift, and diplomacy.
//implement cards type
CardsType Cards::getType() const {
    //CardsType result;
    //return result;
}

//A Deck object contains a finite collection of Warzone cards.
Deck::Deck(){ //constructor definition outside of the class

};

//cards parametries constructor
Deck::Deck(vector<Cards*>cards){
    this->cards = cards;
};

//copy constructor
Deck::Deck(const Deck& deck){
    this->cards = deck.cards;
};

//Once a card has been played, it is removed from the hand and put back into the deck
void Deck::addcards(Cards *cards) {
    //implement cards addding
    Deck::cards.push_back(cards);
}

//prevent leaks
Deck::~Deck(){

    //delete addcards();
}

std::vector<Cards *> Deck::getcards() const {
    return std::vector<Cards *>();
}


//A Hand object is a finite collection of Warzone cards.
Hand::Hand(){ //constructor definition outside of the class

};

//cards parametries constructor
Hand::Hand(vector <Cards*> cards){
    this->cards = cards;
};

//copy constructor
Hand::Hand(const Hand& hand){
    this->cards = hand.cards;
};

//Hand has a draw() method that allows a player to draw a card at random from the cards remaining in the deck
Cards* Hand::draw() {
    //implement code to draw cards
    if(Hand::cardsize() > 0){
        int randcard = rand()% cards.size();
        Cards* card = cards[randcard];
        Hand::addcards(card);
        return card;
    }
}

//remaining in the deck and place it in their hand
void Hand::addcards(Cards* cards) {
    //implement cards adding
    Hand::cards.push_back(cards);
}

//Once a card has been played, it is removed from the hand and put back into the deck
void Hand::removecards(Cards *cards) {
    //implement cards remove
    Hand::cards.pop_back();
}

//prevent leaks
Hand::~Hand(){

    delete draw();
}

int Hand::cardsize() {
    return cards.size();
}

std::vector<Cards *> Hand::getcards() {
    return std::vector<Cards *>();
}

//Each card has a play() method that enables a player to use it during game play by creating special orders.
//Once a card has been played, it is removed from the hand and put back into the deck

//streams
std::ostream& operator<<(std::ostream& stream, const Cards cards){
    //stream << "Cards (" <<cards.type <<")";
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Hand& hand){
    stream <<"Cards (" <<hand.cards.size() << ")";
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Deck& deck){
    stream << "Deck (" <<deck.cards.size() << ")";
    return stream;
}