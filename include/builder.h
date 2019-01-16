//
// Created by liuxi on 2019/1/16.
//

#ifndef MATRIX2_BUILDER_H
#define MATRIX2_BUILDER_H

#include "../include/matrix.h"
#include <vector>

namespace Mx {

    template <typename T>
    class MatrixBuilder {
    public:
        virtual Size compute_size() = 0;
        virtual void before_build(const Matrix<T>&mat) {};
        virtual void actual_build(const Matrix<T>&mat) = 0;
        virtual Matrix<T> build() {
            Matrix<T> mat(this->compute_size(),T(0));
            this->before_build(mat);
            this->actual_build(mat);
            return mat;
        }
    };

    template <typename T>
    class RowwiseBuilder : public MatrixBuilder<T> {
        std::vector<MatrixView<T>> _rows;
    public:
        RowwiseBuilder() {}

        virtual ~RowwiseBuilder() {}

        virtual void add(MatrixView<T> mv) {
            _rows.push_back(mv);
        }

        virtual void clear() {
            _rows.clear();
        }

        Size compute_size() override {
            unsigned int rn=0;
            unsigned int cn=0;
            for(auto&e:_rows) {
                cn = ((cn>e->cols())?cn:e->cols());
                rn += e->rows();
            }
            return Size(rn,cn);
        }

        void actual_build(const Matrix<T> &mat) override {
            unsigned int r=0;
            for(auto&e:_rows) {
                mat->block(Range::BegSize({r,0},e->size())) = e;
                r += e->rows();
            }
        }
    };
}

#endif //MATRIX2_BUILDER_H
