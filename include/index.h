//
// Created by liuxi on 2019/1/15.
//

#ifndef MATRIX2_INDEX_H
#define MATRIX2_INDEX_H

#include <vector>
#include <type_traits>

namespace Mx {
    template <typename T>
    struct Index: std::vector<T>{
        Index() {}
        Index(int count, const T& v): vector<T>(count,v) {}
        Index(const std::initializer_list<T> &init) : vector(init) {}
        Index(const Index<T>& idx): vector(idx) {}
        Index(const std::vector<T>& idx): vector(idx) {}

        template <typename A>
        Index<T> operator[](Index<A> idx) {
            vector<T> vec;
            for(auto i:idx)
                vec.push_back((*this)[i]);
            return Index<T>(vec);
        }

        template <typename A>
        T &operator[](A idx) {
            return std::vector<T>::operator[](idx);
        }

        template <typename A>
        const T &operator[](A idx) const {
            return std::vector<T>::operator[](idx);
        }

        static Index<T> Linear(T from, T end, T stride = T(1)) {
            Index<T> idx;
            T cur = from;
            if(cur<end) {
                while(cur<end) {
                    idx.push_back(cur);
                    cur+=stride;
                }
            }
            else {
                while(cur>end) {
                    idx.push_back(cur);
                    cur+=stride;
                }
            }
            return idx;
        }

        virtual ~Index() = default;
    };
}

#endif //MATRIX2_INDEX_H
