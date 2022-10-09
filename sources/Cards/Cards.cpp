#include <iostream>
#include "Cards.h"
#include "../Orders/Orders.h"
#include "../Player/Player.h"
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

/**
 * cards default constructor
 */
Cards::Cards() { //constructor definition outside the class

};

/**
 * prevent leaks
 */
Cards::~Cards() {

}

/**
 * cards parametrised constructor
 * @param type
 */
Cards::Cards(CardsType type) {
    this->type = type;
};

/**
 * copy constructor
 * @param cards
 */
Cards::Cards(const Cards& cards) {
    this->type = cards.type;
};

/**
 * card types
 * Each card has a type from: bomb, reinforcement, blockade, airlift, and diplomacy.
 * @param name
 * @return
 */
CardsType Cards::getType(const string& name) {

    if (name == "bomb") {
        return BOMB;
    } else if (name == "reinforcement") {
        return REINFORCEMENT;
    } else if (name == "blockade") {
        return BLOCKADE;
    } else if (name == "airlift") {
        return AIRLIFT;
    }
    return DIPLOMACY;
}

/**
 * A Deck object contains a finite collection of Warzone cards/default constructor
 */
Deck::Deck() { //constructor definition
    this->cards = vector<Cards*>();
};

/**
 * prevent leaks
 */
Deck::~Deck() {
    for (auto& cardss: this->cards) {//auto detect object type
        delete cardss;
        cardss = nullptr;
    }
    cards.clear();
}

/**
 * cards parametries constructor
 * @param card
 */
Deck::Deck(const vector<Cards*>& card) {
    this->cards = card;
};

/**
 * copy constructor
 * @param deck
 */
Deck::Deck(const Deck& deck) {
    this->cards = deck.cards;
};

/**
 * Once a card has been played, it is removed from the hand and put back into the deck
 * @param card
 */
void Deck::addCard(Cards* card) {
    if (card != nullptr) {
        cout << "Adding card to deck..." << getNameByCardType(card->getType()) << endl;
        this->cards.push_back(card);
    } else {
        cout << "Tried to add null card to deck." << endl;
    }
}

/**
 * get the amount of cards in the set
 * @return
 */
int Deck::cardSize() {
    return cards.size();
}

/**
 * return the cards in the deck
 * @return
 */
std::vector<Cards*> Deck::getCards() {
    return cards;
}

/**
 * Deck has a draw() method that allows a player to draw a card at random from the cards remaining in the deck
 * @param hand
 */
void Deck::draw(Player* player) {
    //implement code to draw cards
    if (Deck::cardSize() > 0) {//verify if there are still card in the deck
        std::random_device rd; //define random number generator
        default_random_engine randomEngine(rd()); //generate random number
        uniform_int_distribution<int> dis(0, cards.size() - 1); //set potential number

        int randCardIdx = dis(randomEngine); //pull a number from the amount of cards
        player->getHandCards()->addCard(cards[randCardIdx]); //add random card to hand
        cards.erase(cards.begin() + randCardIdx); //remove card from deck
    } else {
        cout << "Tried to pull a card from an empty deck" << endl;
    }
}

/**
 * A Hand object is a finite collection of Warzone cards.
 */
Hand::Hand() {
    this->cards = vector<Cards*>();
};

/**
 * cards parametries constructor
 * @param cards
 */
Hand::Hand(vector<Cards*> cards) {
    this->cards = std::move(cards);
};

/**
 * copy constructor
 * @param hand
 */
Hand::Hand(const Hand& hand) {
    this->cards = hand.cards;
};

/**
 * remaining in the deck and place it in their hand
 * @param card
 */
void Hand::addCard(Cards* card) {
    cout << "Adding card to Hand " << getNameByCardType(card->getType()) << endl;
    this->cards.push_back(card);
}

/**
 * prevent leaks
 */
Hand::~Hand() {
    for (auto& cardss: this->cards) {
        delete cardss;
        cardss = nullptr;
    }
    this->cards.clear();
}

/**
 * return cards in the hand
 * @return
 */
std::vector<Cards*> Hand::getCards() {
    return cards;
}

/**
 * Each card has a play() method that enables a player to use it during game play by creating special orders.
 * Once a card has been played, it is removed from the hand and put back into the deck
 * @param ol
 * @param hand
 * @param deck
 */
void Cards::play(Player* player, Deck* deck) {
    cout << "Playing card " << getNameByCardType(getType()) << endl;
    cout << endl;

    Orders* orders = createOrderByCardType(this->getType()); //create an order with the card type

    player->getOrdersList()->add(orders); //add card type to the list

    deck->addCard(createCardByCardType(this->getType())); //add card base on their type

    player->getHandCards()->remove(this); //remove the cards by type from hand
}

/**
 * return the type of the cards
 * @return
 */
CardsType& Cards::getType() {
    return type;
}

/**
 * return the name of the cards type
 * @param cardsType
 * @return
 */
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

/**
 * output streams for cards
 * @param stream
 * @param cards
 * @return
 */
std::ostream& operator<<(std::ostream& stream, const Cards& cards) {
    stream << "The Cards type is:(" << cards.type << ")";
    return stream;
}

/**
 * assignment operator
 * @param cards
 * @return
 */
Cards& Cards::operator=(const Cards& cards) {
    return *this;
}

/**
 * return the type of cards, null if the cards type dosent exist
 * @param type
 * @return
 */
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

/**
 * output streams for hand
 * @param stream
 * @param hand
 * @return
 */
std::ostream& operator<<(std::ostream& stream, const Hand& hand) {
    stream << "Hand has:(" << hand.cards.size() << ") cards." << endl;
    for (auto cardss: hand.cards) { //loop through all cards in the hand
        stream << getNameByCardType(cardss->getType()) << ", ";
    }
    stream << endl;
    return stream;
}

/**
 * assignment operator
 * @param Hand
 * @return
 */
Hand& Hand::operator=(const Hand& Hand) {
    this->cards = Hand.cards;
    return *this;
}

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

/**
 * output streams for deck
 * @param stream
 * @param deck
 * @return
 */
std::ostream& operator<<(std::ostream& stream, const Deck& deck) {
    stream << "Deck has:(" << deck.cards.size() << ") cards." << endl;
    for (auto cardss: deck.cards) {
        stream << getNameByCardType(cardss->getType()) << ", ";
    }
    stream << endl;
    return stream;
}

/**
 * assignment operator
 * @param deck
 * @return
 */
Deck& Deck::operator=(const Deck& deck) {
    this->cards = deck.cards;
    return *this;
}