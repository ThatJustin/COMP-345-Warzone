//
// Created by Alex De Luca on 2022-09-14.
//

#ifndef COMP_345_ORDERSLIST_H
#define COMP_345_ORDERSLIST_H
#include "Orders.h"
#include <vector>
class OrdersList {
private:
    vector <Orders> list;
    int list_size;
public:
    void move();
    void remove();
};


#endif //COMP_345_ORDERSLIST_H
