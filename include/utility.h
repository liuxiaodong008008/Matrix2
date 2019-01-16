//
// Created by liuxi on 2019/1/15.
//

#ifndef MATRIX2_UTILITY_H
#define MATRIX2_UTILITY_H

#include <iostream>

namespace Mx{

    class Size {
    public:
        unsigned int rows;
        unsigned int cols;

        Size(unsigned int rows, unsigned int cols) : rows(rows), cols(cols) {}

        Size(const Size &size) : rows(size.rows), cols(size.cols) {};

        unsigned int count() const {
            return rows*cols;
        };

        Size swap() const {
            return Size(cols,rows);
        };

        bool operator==(const Size &rhs) const {
            return rows == rhs.rows &&
                   cols == rhs.cols;
        }

        bool operator!=(const Size &rhs) const {
            return !(rhs == *this);
        }
    };

    class Position {
    public:
        unsigned int row;
        unsigned int col;
        Position(unsigned int row, unsigned int col) : row(row), col(col) {}
        Position(const Position &position) : row(position.row), col(position.col) {};

        bool operator==(const Position &rhs) const {
            return row == rhs.row &&
                   col == rhs.col;
        }

        bool operator!=(const Position &rhs) const {
            return !(rhs == *this);
        }
    };

    class Range {
    public:
        Position beg;
        Size size;

        Range(const Position &beg, const Size &size) : beg(beg), size(size) {}

        Range(const Range &range) : beg(range.beg), size(range.size) {};

        unsigned int count() const {
            return size.count();
        };

        bool cover(const Range &range) const {
            return beg.row<=range.beg.row && beg.col<=range.beg.col
                && range.beg.row+range.size.rows <= beg.row+size.rows
                && range.beg.col+range.size.cols <= beg.col+size.cols;
        };

        unsigned int index(const Position &pos) const {
            return pos.row*size.cols+pos.col;
        };

        bool operator==(const Range &rhs) const {
            return beg == rhs.beg &&
                   size == rhs.size;
        }

        bool operator!=(const Range &rhs) const {
            return !(rhs == *this);
        }

        static Range FromTo(const Position &beg,const Position &end) {
            return Range({beg.row,beg.col},{end.row-beg.row,end.col-beg.col});
        };

        static Range FromTo(unsigned int beg_row, unsigned int beg_col, unsigned int end_row, unsigned int end_col) {
            return Range({beg_row,beg_col},{end_row-beg_row,end_col-beg_col});
        };

        static Range BegSize(const Position &beg, const Size &size) {
            return Range(beg,size);
        };

        static Range BegSize(unsigned int beg_row, unsigned int beg_col, unsigned int rows, unsigned int cols) {
            return Range({beg_row,beg_col},{rows,cols});
        };
    };
}

#endif //MATRIX2_UTILITY_H
