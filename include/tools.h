//
// Created by liuxi on 2019/1/17.
//

#ifndef MATRIX2_TOOLS_H
#define MATRIX2_TOOLS_H

#include "../include/matrix.h"

namespace Mx {
    template <typename T> Matrix<T> Eye(unsigned int n) {
        Matrix<T> ret(n,n,T(0));
        for(int i=0;i<n;i++) ret(i,i) = T(1);
        return ret;
    }
}

#endif //MATRIX2_TOOLS_H
