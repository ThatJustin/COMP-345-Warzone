#include "iostream"
#include "sources/Cards/Cards.h"
#include "sources/Orders/Orders.h"
#include "sources/Player/Player.h"
#include "vector"

/**
 * method to test the cards functions
 */
void testCards() {

    Cards* bomb = new Cards(CardsType::BOMB); //create type bomb
    Cards* blockade = new Cards(CardsType::BLOCKADE); //create type blockade
    Cards* reinforcement = new Cards(CardsType::REINFORCEMENT); //create type reinforcement
    Cards* airlift = new Cards(CardsType::AIRLIFT); //create type airlift
    Cards* diplomacy = new Cards(CardsType::DIPLOMACY); //create type diplomacy
    Player* player = new Player("Rickky Bobby");

    std::vector<Cards*> deckCards; //create a vector of cards for the deck
    deckCards.push_back(bomb); //add type bomb to the deckcards list
    deckCards.push_back(reinforcement); //add type reinforcement to the deckcards list
    deckCards.push_back(blockade); //add type blockade to the deckcards list
    deckCards.push_back(airlift); //add type airlift to the deckcards list
    deckCards.push_back(diplomacy); //add type diplomacy to the deckcards list

    Deck* deck = new Deck(deckCards);

    cout << "Before Deck Draw" << endl;;

    std::cout << *deck << std::endl;
    std::cout << *player->getHandCards() << std::endl;

    //Simulate drawing from the deck
    deck->draw(player);
    deck->draw(player);
    deck->draw(player);
    deck->draw(player);
    deck->draw(player);

    cout << "After Deck draw " << endl;;

    std::cout << *deck << std::endl;
    std::cout << *player->getHandCards() << std::endl;

    //Play the cards
    for (auto handCards: player->getHandCards()->getCards()) {
        handCards->play(player, deck);
    }

    cout << "\r\nAfter Card Play " << endl;;

    std::cout << *player->getHandCards() << std::endl;
    std::cout << *deck << std::endl;

    //clean up prevent leaks
    delete deck;
    delete player;
}