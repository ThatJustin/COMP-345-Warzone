#pragma once

#include <vector>
#include <ostream>

using std::vector;

class Territory;

class GameEngine;

class Player;

class Orders;

class Deck;

class PlayerStrategy {
public:
    explicit PlayerStrategy(Player* pPlayer);

    virtual vector<Territory*> toDefend() = 0;

    virtual vector<Territory*> toAttack() = 0;

    virtual bool issueOrder(GameEngine* gameEngine) = 0;

    Player* player;

    vector<Orders*> ordersList;

private:
    Player* getPlayer() const;
};

class HumanPlayerStrategy : public PlayerStrategy {
public:
    explicit HumanPlayerStrategy(Player* pPlayer);

    vector<Territory*> toDefend() override;

    vector<Territory*> toAttack() override;

    bool issueOrder(GameEngine* gameEngine) override;
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
    explicit AggressivePlayerStrategy(Player* pPlayer);

    vector<Territory*> toDefend() override;

    vector<Territory*> toAttack() override;

    bool issueOrder(GameEngine* gameEngine) override;
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    // Parameterized constructor taking a pointer to a player object as a parameter
    explicit BenevolentPlayerStrategy(Player* pPlayer);

    // Copy constructor
    BenevolentPlayerStrategy(Player* pPlayer, const BenevolentPlayerStrategy& benevolent_player_strategy);

    // Assignment operator overload
    BenevolentPlayerStrategy& operator=(const BenevolentPlayerStrategy& benevolent_player_strategy);

    // Stream insertion operator overload
    friend std::ostream& operator<<(std::ostream& outs, const BenevolentPlayerStrategy& benevolent_player_strategy);

    // Method to return a vector of territories that the player wants to defend
    vector<Territory*> toDefend() override;

    // Method to return a vector of territories that the player wants to attack
    vector<Territory*> toAttack() override;

    // Method to issue an order
    bool issueOrder(GameEngine* gameEngine) override;
};

class NeutralPlayerStrategy : public PlayerStrategy {
public:
    explicit NeutralPlayerStrategy(Player* pPlayer);

    vector<Territory*> toDefend() override;

    vector<Territory*> toAttack() override;

    bool issueOrder(GameEngine* gameEngine) override;

    bool isattacked;
};

class CheaterPlayerStrategy : public PlayerStrategy {
public:
    explicit CheaterPlayerStrategy(Player* pPlayer);

    vector<Territory*> toDefend() override;

    vector<Territory*> toAttack() override;

    bool issueOrder(GameEngine* gameEngine) override;

    // Copy constructor
    CheaterPlayerStrategy(Player* pPlayer, const CheaterPlayerStrategy& cheater_player_strategy);

    // Assignment operator overload
    CheaterPlayerStrategy& operator=(const CheaterPlayerStrategy& cheater_player_strategy);

    // Stream insertion operator overload
    friend std::ostream& operator<<(std::ostream& outs, const CheaterPlayerStrategy& cheater_player_strategy);

};

class DefaultPlayerStrategy : public PlayerStrategy {
public:
    explicit DefaultPlayerStrategy(Player* pPlayer);

    vector<Territory*> toDefend() override;

    vector<Territory*> toAttack() override;

    bool issueOrder(GameEngine* gameEngine) override;
};