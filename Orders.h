//
// Created by Alex De Luca on 2022-09-14.
//

#ifndef COMP_345_ORDERS_H
#define COMP_345_ORDERS_H


class Orders {
public:
    bool validate();
    void execute();
};

class Deploy : Orders{
};

class Advance : Orders{

};

class Bomb : Orders{

};

class Blockade : Orders{

};

class Airlift : Orders{

};

class Negotiate : Orders{

};
#endif //COMP_345_ORDERS_H
