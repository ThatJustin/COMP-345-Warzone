#pragma once
#include "iostream"
#include "vector"

class Deck;
class Hand;

enum class CardsType;

class Cards{

private:
    CardsType type;
    friend std::ostream& operator <<(std::ostream& stream, const Cards& cards);

public:
    Cards();
    Cards(CardsType type);
    Cards(const Cards& cards);
    ~Cards();
    CardsType getType() const;
    //void play
    //cardstype
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
    //int cardsize();
    std::vector<Cards*> getcards() const;
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
    Cards* draw();
    void addcards(Cards* cards);
    void removecards(Cards* cards);
    int cardsize();
    std::vector<Cards*> getcards();
};

std::ostream& operator <<(std::ostream& stream, const Cards& cards);
std::ostream& operator <<(std::ostream& stream, const Deck& deck);
std::ostream& operator <<(std::ostream& stream, const Hand& hand);