//
// Created by Alex De Luca on 2022-09-14.
//

#ifndef COMP_345_ORDERS_H
#define COMP_345_ORDERS_H

#include <vector>

class Orders {
    friend std::ostream &operator<<(std::ostream &out, Orders const &order);
public:
    bool validate();
    void execute();
};

class Deploy : public Orders{
    string toString()const;
};

class Advance : public Orders{
    string toString()const;

};

class Bomb : public Orders{
    string toString()const;

};

class Blockade : public Orders{
    string toString()const;

};

class Airlift : public Orders{
    string toString()const;

};

class Negotiate : public Orders{
    string toString()const;

};

class OrdersList {
private:
    std::vector <Orders*> list;
public:
    OrdersList();
    OrdersList(const OrdersList& ol);
    ~OrdersList();
    void move(int to_move, int move_to);
    void remove(int order);
    void add(Orders* o);
};

#endif //COMP_345_ORDERS_H
