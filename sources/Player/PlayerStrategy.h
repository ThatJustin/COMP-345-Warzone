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

    bool isattacked;
};

class CheaterPlayerStrategy : public PlayerStrategy {
public:
    explicit CheaterPlayerStrategy(Player* pPlayer);

    vector<Territory*> toDefend() override;

    vector<Territory*> toAttack() override;

    bool issueOrder(GameEngine* gameEngine) override;


    // Copy constructor
    CheaterPlayerStrategy(Player *pPlayer, const CheaterPlayerStrategy& cheater_player_strategy);

    // Assignment operator overload
    CheaterPlayerStrategy& operator=(const CheaterPlayerStrategy& cheater_player_strategy);

    // Stream insertion operator overload
    friend std::ostream& operator<<(std::ostream& outs, const CheaterPlayerStrategy& cheater_player_strategy);


};