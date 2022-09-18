//
// Created by Alex De Luca on 2022-09-14.
//

#ifndef COMP_345_ORDERS_H
#define COMP_345_ORDERS_H

#include <vector>

class Orders {
public:
    bool validate();

    void execute();
};

class OrdersList {
private:
    std::vector<Orders*> list;
    int list_size;
public:
    OrdersList(const OrdersList& ol);
    void move(int to_move, int move_to);
    void remove();
};

class Deploy : Orders {
};

class Advance : Orders {

};

class Bomb : Orders {

};

class Blockade : Orders {

};

class Airlift : Orders {

};

class Negotiate : Orders {

};

#endif //COMP_345_ORDERS_H
