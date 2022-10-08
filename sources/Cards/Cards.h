#pragma once

#include "iostream"
#include "vector"

class Deck; //define the deck class
class Hand; //define the hand class
class OrdersList; //define the orderlist class
class Orders;     //define the orders class
class Player;     //define the player class

/**
 * enum containing each of the Cards Type
 */
enum CardsType {
    BOMB, REINFORCEMENT, BLOCKADE, AIRLIFT, DIPLOMACY
};

/**
 * create the cards class with all the appropriate attribute
 */
class Cards {

private:
    CardsType type;

    friend std::ostream& operator<<(std::ostream& stream, const Cards& cards); //output stream for the card class

public:
    Cards(); //cards default constructor
    Cards(CardsType type); //cards parametrised constructor
    Cards(const Cards& cards); //cards copy constructor
    ~Cards(); //cards deletor
    CardsType getType(std::string name) const; //card type with the enum element name
    CardsType& getType(); //return the type of the card
    void play(Player* player, Deck* deck); //play method for the card class
    Cards& operator=(const Cards& cards); //assignment operator
};

/**
 * create the deck class with all the appropriate attribute
 */
class Deck {

private:
    std::vector<Cards*> cards; //list of cards
    friend std::ostream& operator<<(std::ostream& stream, const Deck& deck); //output stream for the deck class
    Deck& operator=(const Deck& deck); //assignment operator

public:
    Deck(); //deck default constructor
    Deck(const std::vector<Cards*>& cards); //deck parametrised constructor
    Deck(const Deck& deck); //deck copy constructor
    ~Deck(); //deck deletor
    void addCard(Cards* card); //add cards to the deck method
    std::vector<Cards*> getCards(); //return the cards vector
    int cardSize(); //the amount of cards in the deck
    void draw(Player* player); //draw method that draws a card amd adds to hand
};

/**
 * create the deck class with all the appropriate attribute
 */
class Hand {

private:
    std::vector<Cards*> cards; //list of cards in the hand
    friend std::ostream& operator<<(std::ostream& stream, const Hand& hand); //output stream for the hand class
    Hand& operator=(const Hand& deck); //assignment operator

public:
    Hand();//hand default constructor
    Hand(std::vector<Cards*> cards); //hand parametrized constructor
    Hand(const Hand& hand); //hand copy constructor
    ~Hand(); //hand deletor
    void addCard(Cards* card); //method that allow cards to be added to the hand
    void remove(Cards* card);
    std::vector<Cards*> getCards(); //return cards in the hand
};

/**
 * return the cards type by name through a string
 * @param cardsType
 * @return
 */
std::string getNameByCardType(CardsType cardsType);

/**
 * return the cards type through a card
 * @param cardsType
 * @return
 */
Cards* createCardByCardType(CardsType cardsType);

/**
 * define the stream operator for the classes
 * @param stream
 * @param cards
 * @return
 */
std::ostream& operator<<(std::ostream& stream, const Cards& cards);

std::ostream& operator<<(std::ostream& stream, const Deck& deck);

std::ostream& operator<<(std::ostream& stream, const Hand& hand);