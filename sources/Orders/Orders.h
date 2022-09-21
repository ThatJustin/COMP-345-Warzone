//
// Created by Alex De Luca on 2022-09-14.
//

#ifndef COMP_345_ORDERS_H
#define COMP_345_ORDERS_H

#include <vector>
#include "../Cards/Cards.h"

class Territory;

class Player;

class Orders {
public:
    Orders();

    ~Orders();

    Orders* createOrderByCardType(CardType orderType);

    virtual bool validate() = 0;

    virtual void execute() = 0;
};

class Deploy : public Orders {
public:
    Deploy();

    Deploy(int numberOfArmyUnits, Territory* targetTerritory);

    ~Deploy();

    bool validate() override;

    void execute() override;

    int getNumberOfArmyUnits() const;

    void setNumberOfArmyUnits(int numberOfArmyUnits);

    Territory* getTargetTerritory() const;

    void setTargetTerritory(Territory* targetTerritory);

private:
    int m_numberOfArmyUnits;
    Territory* m_targetTerritory;
};

class Advance : public Orders {
public:
    Advance();

    Advance(int numberOfArmyUnits, Territory* sourceTerritory, Territory* targetTerritory);

    ~Advance();

    bool validate() override;

    void execute() override;

    int getNumberOfArmyUnits() const;

    void setNumberOfArmyUnits(int numberOfArmyUnits);

    Territory* getSourceTerritory() const;

    void setSourceTerritory(Territory* sourceTerritory);

    Territory* getTargetTerritory() const;

    void setTargetTerritory(Territory* targetTerritory);

private:
    int m_numberOfArmyUnits;
    Territory* m_sourceTerritory;
    Territory* m_targetTerritory;
};

class Bomb : public Orders {
public:
    Bomb();

    explicit Bomb(Territory* targetTerritory);

    ~Bomb();

    bool validate() override;

    void execute() override;

    Territory* getTargetTerritory() const;

    void setTargetTerritory(Territory* targetTerritory);

private:
    Territory* m_targetTerritory;
};

class Blockade : public Orders {

public:
    Blockade();

    explicit Blockade(Territory* targetTerritory);

    ~Blockade();

    bool validate() override;

    void execute() override;

    Territory* getTargetTerritory() const;

    void setTargetTerritory(Territory* targetTerritory);

private:
    Territory* m_targetTerritory;
};

class Airlift : public Orders {
public:


    Airlift();

    Airlift(int mNumberOfArmyUnits, Territory* mSourceTerritory, Territory* mTargetTerritory);

    ~Airlift();

    bool validate() override;

    void execute() override;

    int getNumberOfArmyUnits() const;

    void setNumberOfArmyUnits(int numberOfArmyUnits);

    Territory* getSourceTerritory() const;

    void setSourceTerritory(Territory* sourceTerritory);

    Territory* getTargetTerritory() const;

    void setTargetTerritory(Territory* targetTerritory);

private:
    int m_numberOfArmyUnits;
    Territory* m_sourceTerritory;
    Territory* m_targetTerritory;
};

class Negotiate : public Orders {
public:
    Negotiate();

    explicit Negotiate(Player* targetPlayer);

    ~Negotiate();

    bool validate() override;

    void execute() override;

    Player* getTargetPlayer() const;

    void setTargetPlayer(Player* targetPlayer);

private:
    Player* m_targetPlayer;
};

class OrdersList {
private:
    std::vector<Orders*> list;
public:
    OrdersList();

    OrdersList(const OrdersList& ol);

    void move(int to_move, int move_to);

    void remove(int order);

    void add(Orders* o);

    vector<Orders*> getOrdersList();
};

#endif //COMP_345_ORDERS_H
