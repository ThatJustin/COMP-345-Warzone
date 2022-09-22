//
//Implement a group of C++ classes that implements a deck and hand of Warzone cards. _/
//Each card has a type from: bomb, reinforcement, blockade, airlift and diplomacy. _/
//The Deck class must have a draw() method that allows a player to draw a card at random from the cards remaining in the deck _/
//and place it in their hand of cards. _/
//The objects of type Card must have a play() method that is called to play the card, which creates an order and
//adds it to the player’s list of orders and then returns the card to the deck.
//All the classes/functions that you implement for this component must all reside in a single .cpp/.h file duo named Cards.cpp/Cards.h._/
//You must deliver a file named CardsDriver.cpp file that contains a free function named testCards() that creates a deck of Warzone cards,
//then create a hand object that is filled by drawing cards from the deck.
//

#include <iostream>
#include "Cards.h"
#include <vector>

using namespace std;

//cards default constructor
Cards::Cards() { //constructor definition outside of the class

};

//cards parametries constructor
Cards::Cards(CardsType type) {
    this->type = type;
};

//copy constructor
Cards::Cards(const Cards& cards) {
    this->type = cards.type;
};

//prevent leaks
Cards::~Cards() {

//    delete CardsType;
//    delete Deck;
//    delete Hand;
}

//card types
//Each card has a type from: bomb, reinforcement, blockade, airlift, and diplomacy.
CardsType Cards::getType(string name) const {

    if (name == "bomb") {
        return BOMB;
    } else if (name == "reinforcement") {
        return REINFORCEMENT;
    } else if (name == "blockade") {
        return BLOCKADE;
    } else if (name == "airlift") {
        return AIRLIFT;
    } else if (name == "diplomacy") {
        return DIPLOMACY;
    }
}

//A Deck object contains a finite collection of Warzone cards.
Deck::Deck() { //constructor definition outside of the class

};

//cards parametries constructor
Deck::Deck(const vector<Cards*>& card) {
    this->cards = vector<Cards*>(card.size());
    for (auto& temp: card) {
        this->cards.push_back(new Cards(*temp));
    }
};

//copy constructor
Deck::Deck(const Deck& deck) {
    this->cards = deck.cards;
};

//Once a card has been played, it is removed from the hand and put back into the deck
void Deck::addcards(Cards* cards) {
    //implement cards addding
    Deck::cards.push_back(cards);
}

void Deck::removecards(Cards* cards) {
    //implement cards remove
    Deck::cards.pop_back();
}

//get the amount of cards in the set
int Deck::cardsize() {
    return cards.size();
}

std::vector<Cards*> Deck::getcards() const {
    return cards;
}

//Hand has a draw() method that allows a player to draw a card at random from the cards remaining in the deck
Cards* Deck::draw() {
    //implement code to draw cards
    if (Deck::cardsize() > 0) {//verify if there are still card in the deck
        int randcard = rand() % cards.size();
        Cards* card = cards[randcard];
        removecards(card);
        return card;
    }
}

//prevent leaks
Deck::~Deck() {

    delete draw();
}

//A Hand object is a finite collection of Warzone cards.
Hand::Hand() { //constructor definition outside of the class

};

//cards parametries constructor
Hand::Hand(vector<Cards*> cards) {
    this->cards = cards;
};

//copy constructor
Hand::Hand(const Hand& hand) {
    this->cards = vector<Cards*>(hand.cards.size());
    for (auto& temp: hand.cards) {
        this->cards.push_back(new Cards(*temp));
    }
};

//remaining in the deck and place it in their hand
void Hand::addcards(Cards* cards) {
    //implement cards adding
    Hand::cards.push_back(cards);
}

//Once a card has been played, it is removed from the hand and put back into the deck
void Hand::removecards(Cards* cards) {//testing
    //implement cards remove
    Hand::cards.pop_back();//testing
}

//prevent leaks
Hand::~Hand() {

    // delete draw();
}

std::vector<Cards*> Hand::getcards() {
    return std::vector<Cards*>();
}

//Each card has a play() method that enables a player to use it during game play by creating special orders.
//Once a card has been played, it is removed from the hand and put back into the deck
void Cards::Play(OrdersList& ol, Orders& orders, Hand& hand, Deck& deck) {

    //handle special orders
    //vector<OrdersList*> orderlist = orders;

    deck.addcards(this);
    hand.removecards(this);
}

//streams
std::ostream& operator<<(std::ostream& stream, const Cards cards) {
    //stream << "Cards (" <<cards.getType(///name) <<")";
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Hand& hand) {
    stream << "Hand (" << hand.cards.size() << ")";
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Deck& deck) {
    stream << "Deck (" << deck.cards.size() << ")";
    return stream;
}