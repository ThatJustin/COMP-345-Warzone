//
// Created by Alex De Luca on 2022-09-14.
//

#include "OrdersList.h"
#include <vector>
OrdersList::OrdersList(const OrdersList& ol){
    list = ol.list;
    list_size = ol.list_size;
}

void OrdersList::move(int to_move, int move_to) {
    Orders* temp;
    temp = &list[move_to];
    list[move_to]=list[to_move];
    list.push_back(*temp);
}

void OrdersList::remove(){

}


