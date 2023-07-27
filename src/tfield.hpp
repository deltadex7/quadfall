#ifndef QUADFALL_T_FIELD_HPP
#define QUADFALL_T_FIELD_HPP

#include <algorithm>

#include "cell.hpp"

namespace qf
{

    struct Row {
        size_t length = 10;
        Cell content[10];

        bool full() { return std::all_of(content, content+10, Cell::isFilled);}
        void draw(float x, float y, int size) {
            for (int i = 0; i < length; ++i) {
                content[i].draw(x + (i * size), y, size);
            }
        }

        Row() : 
        content{Cell(), Cell(), Cell(), Cell(), Cell(),
                Cell(), Cell(), Cell(), Cell(), Cell()} {
            // stub blank
        }

        Row(Cell c1,Cell c2,Cell c3,Cell c4,Cell c5,Cell c6,Cell c7,Cell c8,Cell c9,Cell c10):
        content{c1, c2, c3, c4, c5, c6, c7, c8, c9, c10} {
            // stub blank
        }

    };
} // namespace qf


#endif // !QUADFALL_T_FIELD_HPP