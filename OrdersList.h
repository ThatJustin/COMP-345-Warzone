//
// Created by Alex De Luca on 2022-09-14.
//

#ifndef COMP_345_ORDERSLIST_H
#define COMP_345_ORDERSLIST_H
#include "Orders.h"
#include <vector>
class OrdersList {
private:
    std::vector <Orders*> list;
public:
    OrdersList(const OrdersList& ol);
    void move(int to_move, int move_to);
    void remove(int order);
};


#endif //COMP_345_ORDERSLIST_H
