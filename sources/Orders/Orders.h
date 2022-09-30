//
// Created by Alex De Luca on 2022-09-14.
//

#ifndef COMP_345_ORDERS_H
#define COMP_345_ORDERS_H

#include <vector>
#include <iostream>

class Territory;

class Cards;

class Player;

using namespace std;

enum class OrderType {
    DEPLOY, ADVANCE, BOMB, BLOCKADE, AIRLIFT, NEGOTIATE
};

class Orders {

public:
    Orders();

    virtual ~Orders();

    virtual bool validate() = 0;

    virtual OrderType getOrderType() = 0;

    virtual void execute() = 0;

    virtual Orders* copy() = 0;

    friend ostream& operator<<(ostream& stream, const Orders& orders);

};

class Deploy : public Orders {
public:
    Deploy();

    Deploy(const Deploy& deploy);

    Deploy(int numberOfArmyUnits, Territory* targetTerritory);

    ~Deploy() override;

    bool validate() override;

    void execute() override;

    Orders* copy() override;

    int getNumberOfArmyUnits() const;

    void setNumberOfArmyUnits(int numberOfArmyUnits);

    Territory* getTargetTerritory();

    void setTargetTerritory(Territory* targetTerritory);

    OrderType getOrderType() override;

    std::string toString() const;

private:
    int m_numberOfArmyUnits;
    Territory* m_targetTerritory;
};

class Advance : public Orders {
public:
    Advance();

    Advance(const Advance& advance);

    Advance(int numberOfArmyUnits, Territory* sourceTerritory, Territory* targetTerritory);

    ~Advance() override;

    bool validate() override;

    void execute() override;

    Orders* copy() override;

    int getNumberOfArmyUnits() const;

    void setNumberOfArmyUnits(int numberOfArmyUnits);

    string toString() const;

    Territory* getSourceTerritory();

    void setSourceTerritory(Territory* sourceTerritory);

    Territory* getTargetTerritory();

    void setTargetTerritory(Territory* targetTerritory);

    OrderType getOrderType() override;

private:
    int m_numberOfArmyUnits;
    Territory* m_sourceTerritory;
    Territory* m_targetTerritory;
};

class Bomb : public Orders {
public:
    Bomb();

    Bomb(const Bomb& bomb);

    explicit Bomb(Territory* targetTerritory);

    ~Bomb() override;

    bool validate() override;

    void execute() override;

    Orders* copy() override;

    string toString() const;

    Territory* getTargetTerritory();

    void setTargetTerritory(Territory* targetTerritory);

    OrderType getOrderType() override;

private:
    Territory* m_targetTerritory;
};

class Blockade : public Orders {
public:
    Blockade();

    Blockade(const Blockade& blockade);

    explicit Blockade(Territory* targetTerritory);

    ~Blockade() override;

    bool validate() override;

    void execute() override;

    Orders* copy() override;

    string toString() const;

    Territory* getTargetTerritory();

    void setTargetTerritory(Territory* targetTerritory);

    OrderType getOrderType() override;

private:
    Territory* m_targetTerritory;
};

class Airlift : public Orders {
public:
    Airlift();

    Airlift(const Airlift& airlift);

    Airlift(int mNumberOfArmyUnits, Territory* mSourceTerritory, Territory* mTargetTerritory);

    ~Airlift() override;

    bool validate() override;

    void execute() override;

    Orders* copy() override;

    int getNumberOfArmyUnits() const;

    void setNumberOfArmyUnits(int numberOfArmyUnits);

    Territory* getSourceTerritory();

    void setSourceTerritory(Territory* sourceTerritory);

    string toString() const;

    Territory* getTargetTerritory();

    void setTargetTerritory(Territory* targetTerritory);

    OrderType getOrderType() override;

private:
    int m_numberOfArmyUnits;
    Territory* m_sourceTerritory;
    Territory* m_targetTerritory;
};

class Negotiate : public Orders {
public:
    Negotiate();

    Negotiate(const Negotiate& negotiate);

    explicit Negotiate(Player* targetPlayer);

    ~Negotiate() override;

    bool validate() override;

    void execute() override;

    Orders* copy() override;

    string toString() const;

    Player* getTargetPlayer();

    void setTargetPlayer(Player* targetPlayer);

    OrderType getOrderType() override;

private:
    Player* m_targetPlayer;
};

class OrdersList {
private:
    //std::vector<Orders*> list;
public:
    OrdersList();

    OrdersList(const OrdersList& ol);

    ~OrdersList();

    void move(int to_move, int move_to);

    void remove(int order);

    void add(Orders* o);

    vector<Orders*> getOrdersList();

    std::vector<Orders*> list;//needed for cards play method to work from private to public
};

//free functions
Orders* createOrderByCardType(int cardType);

string getNameByOrderType(OrderType cardType);

std::ostream& operator <<(std::ostream& stream, const OrdersList& ordersList); //testing add stream
#endif //COMP_345_ORDERS_H
