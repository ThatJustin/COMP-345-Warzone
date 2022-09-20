//
// Created by Alex De Luca on 2022-09-14.
//

#ifndef COMP_345_ORDERS_H
#define COMP_345_ORDERS_H

#include <vector>

class Orders {
public:
    Orders();
    bool validate();
    void execute();
};

class Deploy : public Orders{

};

class Advance : public Orders{

};

class Bomb : public Orders{

};

class Blockade : public Orders{

};

class Airlift : public Orders{

};

class Negotiate : public Orders{

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
