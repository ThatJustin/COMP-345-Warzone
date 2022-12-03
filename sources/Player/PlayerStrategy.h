#pragma once

#include <vector>

using std::vector;

class Territory;

class GameEngine;

class Player;

class PlayerStrategy {
public:
    explicit PlayerStrategy(Player* pPlayer);

    virtual vector<Territory*> toDefend() = 0;

    virtual vector<Territory*> toAttack() = 0;

    virtual bool issueOrder(GameEngine* gameEngine) = 0;
    Player* player;
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
};

class CheaterPlayerStrategy : public PlayerStrategy {
public:
    explicit CheaterPlayerStrategy(Player* pPlayer);

    vector<Territory*> toDefend() override;

    vector<Territory*> toAttack() override;

    bool issueOrder(GameEngine* gameEngine) override;
};

class DefaultPlayerStrategy : public PlayerStrategy {
public:
    explicit DefaultPlayerStrategy(Player* pPlayer);

    vector<Territory*> toDefend() override;

    vector<Territory*> toAttack() override;

    bool issueOrder(GameEngine* gameEngine) override;
};