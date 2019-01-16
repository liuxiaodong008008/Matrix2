//
// Created by liuxi on 2019/1/16.
//

#ifndef MATRIX2_ITERATOR_H
#define MATRIX2_ITERATOR_H

#include <iostream>
#include "../include/utility.h"

namespace Mx {

    template <typename T> class _MatrixLike;
    template <typename T> class MatrixView;

    template <typename T>
    class _Iterator {
    protected:
        _MatrixLike<T> & _mat;
        Range _cur_range;
        bool _is_end;
    public:
        _Iterator(_MatrixLike<T> &_mat, const Range &_cur_range)
        : _mat(_mat), _cur_range(_cur_range), _is_end(false) {}

        _Iterator(_MatrixLike<T> &_mat, const Range &_cur_range, bool _is_end)
        : _mat(_mat), _cur_range(_cur_range), _is_end(_is_end) {}

        virtual ~_Iterator() {}

        bool operator==(const _Iterator<T> &rhs) const {
            return ((&_mat) == (&rhs._mat) &&
                    _is_end && rhs._is_end ) 
				|| ((&_mat) == (&rhs._mat) &&
                   _cur_range == rhs._cur_range &&
				   _is_end == rhs._is_end);
        }

        bool operator!=(const _Iterator<T> &rhs) const {
            return !(rhs == *this);
        }

        _Iterator<T>&operator++() {
            this->forward();
            _is_end = _is_end || !this->valid();
            return *this;
        }

        MatrixView<T> operator*() {
            return this->get();
        }

        virtual MatrixView<T> get() {
            return _mat.block(_cur_range);
        }

        virtual void forward() = 0;
        virtual bool valid() = 0;
        virtual _Iterator<T> * clone() = 0;
    };

    template <typename T>
    class Iterator {
    protected:
        _Iterator<T> * _ptr;
    public:
        Iterator(_Iterator<T> *_ptr) : _ptr(_ptr) {}
        Iterator(Iterator<T> && it) : _ptr(it._ptr) {
            it._ptr = nullptr;
        }
        Iterator(const Iterator<T> & it) : _ptr(it._ptr->clone()) {}

        virtual ~Iterator() {
            if(_ptr!= nullptr) {
                delete _ptr;
                _ptr = nullptr;
            }
        }

        bool operator==(const Iterator &rhs) const {
            return (*_ptr) == (*rhs._ptr);
        }

        bool operator!=(const Iterator &rhs) const {
            return !(rhs == *this);
        }

        Iterator<T>& operator++() {
            this->_ptr->operator++();
            return *this;
        }

        MatrixView<T> operator*() {
            return _ptr->get();
        }

        MatrixView<T> get() {
            return _ptr->get();
        }
    };

    template <typename T>
    class _SingleDirectionIterator: public _Iterator<T> {
    protected:
        Position _offset;
    public:
        _SingleDirectionIterator(_MatrixLike<T> &_mat, const Range &_cur_range, const Position &_offset)
        : _Iterator(_mat, _cur_range), _offset(_offset) {
            _is_end = !this->valid();
        }

        _SingleDirectionIterator(_MatrixLike<T> &_mat, const Range &_cur_range, bool _is_end, const Position &_offset)
                : _Iterator(_mat, _cur_range, _is_end), _offset(_offset) {}

        virtual ~_SingleDirectionIterator() {}

        void forward() override {
            _cur_range.beg.row += _offset.row;
            _cur_range.beg.col += _offset.col;
        }

        bool valid() override {
            return Range({0,0},_mat.size()).cover(_cur_range);
        }

        _Iterator<T> *clone() override {
            return new _SingleDirectionIterator(_mat, _cur_range, _offset);
        }
    };

    template <typename T>
    class _Items {
    protected:
        _MatrixLike<T> & _mat;
    public:
        _Items(_MatrixLike<T> &_mat) : _mat(_mat) {}

        virtual ~_Items() {}

        virtual Iterator<T> begin() = 0;
        virtual Iterator<T> end() = 0;
    };

    template <typename T>
    class _VectorItems : public _Items<T> {
    protected:
        Range _init_range;
        Position _offset;
    public:
        _VectorItems(_MatrixLike<T> &_mat, const Range &_init_range, const Position &_offset)
        : _Items(_mat), _init_range(_init_range), _offset(_offset) {}

        virtual ~_VectorItems() {}

        virtual Iterator<T> begin() {
            return new _SingleDirectionIterator<T>(_mat,_init_range,_offset);
        };

        virtual Iterator<T> end() {
            return new _SingleDirectionIterator<T>(_mat,_init_range,true,_offset);
        };
    };
};

#endif //MATRIX2_ITERATOR_H
