#pragma once

#include <vector>

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

    bool isattacked;
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    explicit BenevolentPlayerStrategy(Player* pPlayer);

    vector<Territory*> toDefend() override;

    vector<Territory*> toAttack() override;

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