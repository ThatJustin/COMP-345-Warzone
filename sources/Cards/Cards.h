#pragma once
#include "iostream"
#include "vector"
#include "sources/Orders/Orders.h"

class Deck;
class Hand;
class SpecialOrders;//what is it?

class OrdersList;
class Orders;

//enum class CardsType;
enum CardsType {BOMB, REINFORCEMENT, BLOCKADE, AIRLIFT, DIPLOMACY};

class Cards{

private:
    CardsType type;
    friend std::ostream& operator <<(std::ostream& stream, const Cards& cards);

public:
    Cards();
    Cards(CardsType type);
    Cards(const Cards& cards);
    ~Cards();
    CardsType getType(std::string name) const;
    std::vector<SpecialOrders*> so;
    //void Play(SpecialOrders& so, Hand& hand, Deck& deck);
    void Play(OrdersList& ol,Orders& order, Hand& hand, Deck& deck);
};

class Deck{

private:
    std::vector<Cards*> cards;
    friend std::ostream& operator <<(std::ostream& stream, const Deck& deck);

public:
    Deck();
    Deck(std::vector<Cards*> cards);
    Deck(const Deck& deck);
    ~Deck();
    void addcards(Cards* cards);
    void removecards(Cards* cards);
    std::vector<Cards*> getcards() const;
    int cardsize();
    Cards* draw();
};

class Hand{

private:
    std::vector<Cards*> cards;
    friend std::ostream& operator <<(std::ostream& stream, const Hand& hand);

public:
    Hand();
    Hand(std::vector<Cards*>cards);
    Hand(const Hand& hand);
    ~Hand();
    void addcards(Cards* cards);
    void removecards(Cards* cards);
    std::vector<Cards*> getcards();
};

//define the stream operator
std::ostream& operator <<(std::ostream& stream, const Cards& cards);
std::ostream& operator <<(std::ostream& stream, const Deck& deck);
std::ostream& operator <<(std::ostream& stream, const Hand& hand);