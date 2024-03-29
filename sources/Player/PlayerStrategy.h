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
    explicit PlayerStrategy(Player* pPlayer, std::string sType);

    virtual vector<Territory*> toDefend() = 0;

    virtual vector<Territory*> toAttack() = 0;

    virtual bool issueOrder(GameEngine* gameEngine) = 0;

    virtual ~PlayerStrategy();

    Player* player;

    vector<Orders*> ordersList;

    std::string getType();

    // Copy constructor
    PlayerStrategy(Player* pPlayer, const PlayerStrategy& playerStrategy);

    // Assignment operator overload
    PlayerStrategy& operator=(const PlayerStrategy& playerStrategy);

    // Stream insertion operator overload
    friend std::ostream& operator<<(std::ostream& outs, const PlayerStrategy& playerStrategy);

private:
    Player* getPlayer() const;

    std::string type = "Default";
};

class HumanPlayerStrategy : public PlayerStrategy {
public:
    explicit HumanPlayerStrategy(Player* pPlayer);

    vector<Territory*> toDefend() override;

    vector<Territory*> toAttack() override;

    ~HumanPlayerStrategy() override;

    bool issueOrder(GameEngine* gameEngine) override;

    // Copy constructor
    HumanPlayerStrategy(Player* pPlayer, const HumanPlayerStrategy& humanPlayerStrategy);

    // Assignment operator overload
    HumanPlayerStrategy& operator=(const HumanPlayerStrategy& humanPlayerStrategy);

    // Stream insertion operator overload
    friend std::ostream& operator<<(std::ostream& outs, const HumanPlayerStrategy& humanPlayerStrategy);
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
    explicit AggressivePlayerStrategy(Player* pPlayer);

    vector<Territory*> toDefend() override;

    vector<Territory*> toAttack() override;

    ~AggressivePlayerStrategy() override;

    bool issueOrder(GameEngine* gameEngine) override;

    // Copy constructor
    AggressivePlayerStrategy(Player* pPlayer, const AggressivePlayerStrategy& aggressivePlayerStrategy);

    // Assignment operator overload
    AggressivePlayerStrategy& operator=(const AggressivePlayerStrategy& aggressivePlayerStrategy);

    // Stream insertion operator overload
    friend std::ostream& operator<<(std::ostream& outs, const AggressivePlayerStrategy& aggressivePlayerStrategy);
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    // Parameterized constructor taking a pointer to a player object as a parameter
    explicit BenevolentPlayerStrategy(Player* pPlayer);

    // Copy constructor
    BenevolentPlayerStrategy(Player* pPlayer, const BenevolentPlayerStrategy& benevolent_player_strategy);

    ~BenevolentPlayerStrategy() override;

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

    ~NeutralPlayerStrategy() override;

    bool issueOrder(GameEngine* gameEngine) override;

    // Copy constructor
    NeutralPlayerStrategy(Player* pPlayer, const NeutralPlayerStrategy& neutralPlayerStrategy);

    // Assignment operator overload
    NeutralPlayerStrategy& operator=(const NeutralPlayerStrategy& neutralPlayerStrategy);

    // Stream insertion operator overload
    friend std::ostream& operator<<(std::ostream& outs, const NeutralPlayerStrategy& neutralPlayerStrategy);
};

class CheaterPlayerStrategy : public PlayerStrategy {
public:
    explicit CheaterPlayerStrategy(Player* pPlayer);

    vector<Territory*> toDefend() override;

    vector<Territory*> toAttack() override;

    ~CheaterPlayerStrategy() override;

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

    ~DefaultPlayerStrategy() override;

    bool issueOrder(GameEngine* gameEngine) override;

    // Copy constructor
    DefaultPlayerStrategy(Player* pPlayer, const DefaultPlayerStrategy& defaultPlayerStrategy);

    // Assignment operator overload
    DefaultPlayerStrategy& operator=(const DefaultPlayerStrategy& defaultPlayerStrategy);

    // Stream insertion operator overload
    friend std::ostream& operator<<(std::ostream& outs, const DefaultPlayerStrategy& defaultPlayerStrategy);
};