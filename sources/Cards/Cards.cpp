//
//Implement a group of C++ classes that implements a deck and hand of Warzone cards. _/
//Each card has a type from: bomb, reinforcement, blockade, airlift and diplomacy. _/
//The Deck class must have a draw() method that allows a player to draw a card at random from the cards remaining in the deck _/
//and place it in their hand of cards. _/
//The objects of type Card must have a play() method that is called to play the card, which creates an order and_/
//adds it to the player’s list of orders and then returns the card to the deck._/
//All the classes/functions that you implement for this component must all reside in a single .cpp/.h file duo named Cards.cpp/Cards.h._/
//You must deliver a file named CardsDriver.cpp file that contains a free function named testCards() that creates a deck of Warzone cards_/
//then create a hand object that is filled by drawing cards from the deck._/
//

#include <iostream>
#include "Cards.h"
#include "../Player/Player.h"
#include <utility>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

//cards default constructor
Cards::Cards() { //constructor definition outside of the class

};

//prevent leaks
Cards::~Cards() {

}

//cards parametrised constructor
Cards::Cards(CardsType type) {
    this->type = type;
};

//copy constructor
Cards::Cards(const Cards& cards) {
    this->type = cards.type;
};

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
Deck::Deck() { //constructor definition
    this->cards = vector<Cards*>();
};

//prevent leaks
Deck::~Deck() {
    for (auto& cardss: this->cards) {
        delete cardss;
        cardss = nullptr;
    }
    cards.clear();
}

//cards parametries constructor
Deck::Deck(const vector<Cards*>& card) {
    this->cards = card;
};

//copy constructor
Deck::Deck(const Deck& deck) {
    this->cards = deck.cards;
};

//Once a card has been played, it is removed from the hand and put back into the deck
void Deck::addCard(Cards* card) {
    if (card != nullptr) {
        cout << "Adding card to deck..." << endl;
        this->cards.push_back(card);
    } else {
        cout << "Tried to add null card to deck." << endl;
    }
}

//get the amount of cards in the set
int Deck::cardSize() {
    return cards.size();
}

//return the cards in the deck
std::vector<Cards*> Deck::getCards() {
    return cards;
}

//Hand has a draw() method that allows a player to draw a card at random from the cards remaining in the deck
void Deck::draw(Hand* hand) {
    //implement code to draw cards
    if (Deck::cardSize() > 0) {//verify if there are still card in the deck
        // Used to generate a random number
        std::random_device rd;
        default_random_engine randomEngine(rd());
        uniform_int_distribution<int> dis(0, cards.size() - 1);

        int randCardIdx = dis(randomEngine); //pull a number from the amount of cards
        hand->addCard(cards[randCardIdx]); // add the card to the hand
        cards.erase(cards.begin() + randCardIdx); // delete it from the deck
    } else {
        cout << "Tried to pull a card from an empty deck." << endl;
    }
}


//A Hand object is a finite collection of Warzone cards.
Hand::Hand() { //constructor definition outside the class
    this->cards = vector<Cards*>();
};

//cards parametries constructor
Hand::Hand(vector<Cards*> cards) {
    this->cards = std::move(cards);
};

//copy constructor
Hand::Hand(const Hand& hand) {
    this->cards = hand.cards;
};

//remaining in the deck and place it in their hand
void Hand::addCard(Cards* card) {
    this->cards.push_back(card);
}

//prevent leaks
Hand::~Hand() {
    for (auto& cardss: this->cards) {
        delete cardss;
        cardss = nullptr;
    }
    this->cards.clear();
}

//return cards in the hand
std::vector<Cards*> Hand::getCards() {
    return cards;
}

//Each card has a play() method that enables a player to use it during game play by creating special orders.
//Once a card has been played, it is removed from the hand and put back into the deck
void Cards::play(OrdersList* ol, Hand* hand, Deck* deck) {
    cout << "Playing card " << getNameByCardType(getType()) << endl;

    Orders* orders = createOrderByCardType(this->getType()); //create an order with the card type
    ol->add(orders); //add card type to the list

    //  cout << "Card " << getNameByCardType(getType()) << " is going back to the deck!" << endl;
    deck->addCard(createCardByCardType(this->getType()));
    //  cout << "Card " << getNameByCardType(getType()) << " is being removed from players hand." << endl;
    hand->remove(this);
}

//return the type of the cards
CardsType& Cards::getType() {
    return type;
}

//return the name of the cards type
string getNameByCardType(CardsType cardsType) {
    switch (cardsType) {
        case BOMB:
            return "bomb";
        case REINFORCEMENT:
            return "reinforcement";
        case BLOCKADE:
            return "blockade";
        case AIRLIFT:
            return "airlift";
        case DIPLOMACY:
            return "diplomamcy";
    }
    return "none";
}

//output streams for cards
std::ostream& operator<<(std::ostream& stream, const Cards& cards) {
    stream << "The Cards type is:(" << cards.type << ")" << endl;
    return stream;
}

Cards& Cards::operator=(const Cards& cards) {
    return *this;
}

Cards* createCardByCardType(CardsType type) {
    switch (type) {
        case BOMB:
            return new Cards(BOMB);
        case REINFORCEMENT:
            return new Cards(REINFORCEMENT);
        case BLOCKADE:
            return new Cards(BLOCKADE);
        case AIRLIFT:
            return new Cards(AIRLIFT);
        case DIPLOMACY:
            return new Cards(DIPLOMACY);
    }
    return nullptr;
}

//output streams for hand
std::ostream& operator<<(std::ostream& stream, const Hand& hand) {
    stream << "Hand has: (" << hand.cards.size() << ") cards." << endl;
    for (auto cardss: hand.cards) {
        stream << getNameByCardType(cardss->getType()) << endl;
    }
    stream << endl;
    return stream;
}

Hand& Hand::operator=(const Hand& Hand) {
    this->cards = Hand.cards;
    return *this;
}

/**
 * Removes a card based on the pointer in the hands vector.
 * @param card
 */
void Hand::remove(Cards* card) {
    //If it's null return
    if (card == nullptr) {
        cout << "Invalid null card trying to be removed." << endl;
        return;
    }
    //Ensure the card is actually in the hand
    if (std::find(cards.begin(), cards.end(), card) != cards.end()) {
        //remove it
        cards.erase(std::remove(cards.begin(), cards.end(), card), cards.end());
    } else {
        cout << "Unable to find card in hand." << endl;
    }
}

//output streams for deck
std::ostream& operator<<(std::ostream& stream, const Deck& deck) {
    stream << "Deck has: (" << deck.cards.size() << ") cards." << endl;
    for (auto cardss: deck.cards) {
        stream << getNameByCardType(cardss->getType()) << endl;
    }
    stream << endl;
    return stream;
}

Deck& Deck::operator=(const Deck& deck) {
    this->cards = deck.cards;
    return *this;
}