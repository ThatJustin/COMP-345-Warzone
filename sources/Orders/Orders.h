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

    virtual string showDescription() = 0;

    friend ostream& operator<<(ostream& stream, const Orders& orders);

    virtual void toStreamInsertion(std::ostream& os) const;

    Orders& operator=(const Orders& order);

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

    int getNumberOfArmyUnits();

    void setNumberOfArmyUnits(int numberOfArmyUnits);

    Territory* getTargetTerritory();

    void setTargetTerritory(Territory* targetTerritory);

    OrderType getOrderType() override;

    std::string showDescription() override;

    Deploy& operator=(const Deploy& deploy);

    void toStreamInsertion(std::ostream& os) const override;

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

    int getNumberOfArmyUnits();

    void setNumberOfArmyUnits(int numberOfArmyUnits);

    string showDescription() override;

    Territory* getSourceTerritory();

    void setSourceTerritory(Territory* sourceTerritory);

    Territory* getTargetTerritory();

    void setTargetTerritory(Territory* targetTerritory);

    OrderType getOrderType() override;

    Advance& operator=(const Advance& advance);

    void toStreamInsertion(std::ostream& os) const override;

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

    string showDescription() override;

    Territory* getTargetTerritory();

    void setTargetTerritory(Territory* targetTerritory);

    OrderType getOrderType() override;

    Bomb& operator=(const Bomb& bomb);

    void toStreamInsertion(std::ostream& os) const override;

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

    string showDescription() override;

    Territory* getTargetTerritory();

    void setTargetTerritory(Territory* targetTerritory);

    OrderType getOrderType() override;

    Blockade& operator=(const Blockade& blockade);

    void toStreamInsertion(std::ostream& os) const override;

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

    int getNumberOfArmyUnits();

    void setNumberOfArmyUnits(int numberOfArmyUnits);

    Territory* getSourceTerritory();

    void setSourceTerritory(Territory* sourceTerritory);

    string showDescription() override;

    Territory* getTargetTerritory();

    void setTargetTerritory(Territory* targetTerritory);

    OrderType getOrderType() override;

    Airlift& operator=(const Airlift& airlift);

    void toStreamInsertion(std::ostream& os) const override;

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

    string showDescription() override;

    Player* getTargetPlayer();

    void setTargetPlayer(Player* targetPlayer);

    OrderType getOrderType() override;

    Negotiate& operator=(const Negotiate& negotiate);

    void toStreamInsertion(std::ostream& os) const override;

private:
    Player* m_targetPlayer;
};

class OrdersList {
private:
    std::vector<Orders*> list;
public:
    OrdersList();

    OrdersList(const OrdersList& ol);

    ~OrdersList();

    OrdersList& operator=(const OrdersList& orderslist);

    friend ostream& operator<<(ostream& stream, const OrdersList& ordersList);

    void move(int from, int to);

    void remove(int index);

    void addOrder(Orders* o);

    vector<Orders*> getOrdersList();

    void displayList();
};

//free functions
Orders* createOrderByCardType(int cardType);

string getNameByOrderType(OrderType cardType);

#endif //COMP_345_ORDERS_H
