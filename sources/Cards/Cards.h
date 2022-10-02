#pragma once
#include "iostream"
#include "vector"
#include "sources/Orders/Orders.h"


class Deck; //define the deck class
class Hand; //define the hand class
class OrdersList; //define the orderlist class
class Orders;     //define the orders class

//enum containing each of the Cards Type
enum CardsType {BOMB, REINFORCEMENT, BLOCKADE, AIRLIFT, DIPLOMACY};

//create the cards class with all the appropriate attribute
class Cards{

private:
    CardsType type;
    friend std::ostream& operator <<(std::ostream& stream, const Cards& cards); //output stream for the card class

public:
    Cards(); //cards default constructor
    Cards(CardsType type); //cards parametrised constructor
    Cards(const Cards& cards); //cards copy constructor
    ~Cards(); //cards deletor
    CardsType getType(std::string name) const; //card type with the enum element name
    CardsType getType(); //return the type of the card
    void Play(OrdersList* ol, Hand* hand, Deck* deck); //play method for the card class
};

//create the deck class with all the appropriate attribute
class Deck{

private:
    std::vector<Cards*> cards; //list of cards
    friend std::ostream& operator <<(std::ostream& stream, const Deck& deck); //output stream for the deck class

public:
    Deck(); //deck default constructor
    Deck(const std::vector<Cards*>& cards); //deck parametrised constructor
    Deck(const Deck& deck); //deck copy constructor
    ~Deck(); //deck deletor
    void addcards(Cards* cards); //add cards to the deck method
    void removecards(Cards* cards); //remove cards from the deck method
    std::vector<Cards*> getcards() const; //return the cards list
    int cardsize(); //the amount of cards in the deck
    Cards* draw(); //draw method that allow cards to be pick from deck to hand
};

//create the deck class with all the appropriate attribute
class Hand{

private:
    std::vector<Cards*> cards; //list of cards in the hand
    friend std::ostream& operator <<(std::ostream& stream, const Hand& hand); //output stream for the hand class

public:
    Hand();//hand default constructor
    Hand(std::vector<Cards*>cards); //hand parametrized constructor
    Hand(const Hand& hand); //hand copy constructor
    ~Hand(); //hand deletor
    void addcards(Cards* cards); //method that allow cards to be added to the hand
    void removecards(Cards* cards); //method that allow cards to be removed from the hand
    std::vector<Cards*> getcards(); //return cards in the hand
};

//return the cards type by name through a string
string getNameByCardType(CardsType cardsType);

//define the stream operator for the classes
std::ostream& operator <<(std::ostream& stream, const Cards& cards);
std::ostream& operator <<(std::ostream& stream, const Deck& deck);
std::ostream& operator <<(std::ostream& stream, const Hand& hand);