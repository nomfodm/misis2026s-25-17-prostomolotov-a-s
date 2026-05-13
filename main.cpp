//
// Created by nomfodm on 21.04.2026.
//

#include "prj.labs/bitsetd/bitsetd.hpp"

#include <iostream>
int main() {
    BitsetD bitset(100, false);
    // bitset.fill(false);

    // bitset.resize(80, true);

    bitset.set(35, true);
    bitset.set(29, true);
    bitset.set(25, true);
    bitset.set(3, true);
    bitset.set(2, true);
    bitset.set(1, true);
    bitset.set(0, true);
    //
    // ~bitset;

    // bitset >>= 20;
    bitset.shift(5);

    for (size_t i = 0; i < 100; i++) {
        if (bitset.get(i))
            std::cout << i << std::endl;
    }


    return 0;
}
