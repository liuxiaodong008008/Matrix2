//
// Created by liuxi on 2019/1/15.
//

#ifndef MATRIX2_MATRIX_H
#define MATRIX2_MATRIX_H

#include <iostream>
#include <string>
#include "../include/utility.h"
#include "../include/index.h"
#include "../include/iterator.h"

namespace Mx {
    template <typename T> class MatrixLike;
    template <typename T> class MatrixView;
    template <typename T> class MatrixIndexView;
    template <typename T> class MatrixRangeView;
    template <typename T> class Matrix;

    template <typename T> class _MatrixLike;
    template <typename T> class _MatrixView;
    template <typename T> class _MatrixIndexView;
    template <typename T> class _MatrixRangeView;
    template <typename T> class _Matrix;
    template <typename T> class _MatrixInputter;

    class _IsMatrix{
    public:
        virtual ~_IsMatrix() = default;
    };

    class HasSize {
    public:
        virtual Size size() const = 0;

        virtual ~HasSize() = default;
    };

    template <typename T>
    class _MatrixLike : public _IsMatrix, public HasSize {
    public:
        virtual ~_MatrixLike() = default;

        virtual T &operator()(unsigned int row, unsigned int col) {
            return (*this)(row*this->size().cols+col);
        };

        virtual const T &operator()(unsigned int row, unsigned int col) const {
            return (*this)(row*this->size().cols+col);
        };

        virtual T &operator()(const Position& pos) {
            return (*this)(pos.row*this->size().cols+pos.col);
        };

        virtual const T &operator()(const Position& pos) const {
            return (*this)(pos.row*this->size().cols+pos.col);
        };

        template <typename A=T> A at(unsigned int row, unsigned int col) const {
           return (A)((*this)(row,col));
        };

        template <typename A=T> A at(const Position& pos) const {
            return (A)((*this)(pos));
        };

        template <typename A=T> A at(unsigned int idx) const {
            return (A)((*this)(idx));
        };

        template <typename A> Matrix<A> cast() const {
            Matrix<A> mat(this->size());
            unsigned int cnt = this->count();
            for(unsigned int i=0;i<cnt;i++)
                mat(i) = (*this)(i);
            return mat;
        };

        virtual Matrix<T> t() const {
            Matrix<T> mat(new _Matrix<T>(this->size().swap()));
			unsigned int rn = this->rows();
            unsigned int cn = this->cols();
            for(unsigned int r=0;r<rn;r++)
                for(unsigned int c=0;c<cn;c++)
                    mat(c,r) = (*this)(r,c);

            return mat;
        };

        virtual unsigned int rows() const {
            return this->size().rows;
        };

        virtual unsigned int cols() const {
            return this->size().cols;
        };

        virtual unsigned int count() const {
            return this->size().count();
        };

        virtual MatrixView<T> row(unsigned int r) const {
            return this->block(Range::FromTo(r,0,r+1,this->cols()));
        };

        virtual MatrixView<T> col(unsigned int c) const {
            return this->block(Range::FromTo(0,c,this->rows(),c+1));
        };

        virtual MatrixView<T> row(unsigned int from, unsigned int cnt) const {
            return this->block(Range::FromTo(from,0,from+cnt,this->cols()));
        };

        virtual MatrixView<T> col(unsigned int from, unsigned int cnt) const {
            return this->block(Range::FromTo(0,from,this->rows(),from+cnt));
        };

        virtual MatrixView<T> row(const Index<unsigned int> &idx) const {
            return this->block(idx,this->col_index());
        };

        virtual MatrixView<T> col(const Index<unsigned int> &idx) const {
            return this->block(this->row_index(), idx);
        };


        virtual MatrixView<T> block(unsigned int beg_row, unsigned int beg_col,
                                    unsigned int row_cnt, unsigned int col_cnt) const {
            return this->block(Range({beg_row,beg_col},{row_cnt,col_cnt}));
        };


        template<typename Func> _MatrixLike<T>& apply_inplace(const Func& func) {
            unsigned int cnt = this->count();
            for(unsigned int i=0;i<cnt;i++)
                func((*this)(i));
            return *this;
        }

        template<typename Func, typename A> _MatrixLike<T>& apply_inplace_2(const Func& func, const MatrixLike<A>& mat) {
            unsigned int cnt = this->count();
            for(unsigned int i=0;i<cnt;i++)
                func((*this)(i),mat(i));
            return *this;
        }

        template<typename Func> Matrix<Func(T)> apply(const Func& func) {
            Matrix<Func(T,A)> ret(new _Matrix<T>(this->size()));
            unsigned int cnt = this->count();
            for(unsigned int i=0;i<cnt;i++)
                ret(i) = func((*this)(i));
            return ret;
        }

        template<typename Func, typename A> Matrix<Func(T,A)> apply_2(const Func& func, const MatrixLike<A>& mat) {
            Matrix<Func(T,A)> ret(new _Matrix<T>(this->size()));
            unsigned int cnt = this->count();
            for(unsigned int i=0;i<cnt;i++)
                ret(i) = func((*this)(i),mat(i));
            return ret;
        }

        virtual Matrix<T> copy() const {
            Matrix<T> ret = new _Matrix<T>(this->size());
            unsigned int cnt = this->count();
            for(unsigned int i=0;i<cnt;i++)
                ret(i) = (*this)(i);
            return ret;
        };

        template <typename A> _MatrixLike<T>& operator=(const MatrixLike<A>& _mat) const {
            this->apply_inplace_2([](auto& x, auto& y){x=y;}, _mat);
            return *this;
        };

        virtual _VectorItems<T> row_items() {
            return {*this,Range({0,0},{1,this->cols()}),Position(1,0)};
        }

        virtual _VectorItems<T> col_items() {
            return {*this,Range({0,0},{this->rows(),1}),Position(0,1)};
        }

        virtual MatrixView<T> block(const Range &rng) const = 0;
        virtual MatrixView<T> block(const Index<unsigned int> &ridx,
                                    const Index<unsigned int> &cidx) const = 0;
        virtual T &operator()(unsigned int idx) = 0;
        virtual const T &operator()(unsigned int idx) const = 0;
        virtual MatrixLike<T> clone() const = 0;
        virtual Size size() const = 0;
        virtual std::string class_name() const = 0;
        virtual Index<unsigned int> row_index() const = 0;
        virtual Index<unsigned int> col_index() const = 0;

        virtual void destroy() {
            delete this;
        }

        virtual _MatrixInputter<T> inputter(unsigned int idx = 0) {
            return _MatrixInputter<T>(*this,idx);
        }

        virtual _MatrixInputter<T> inputter(unsigned int row, unsigned int col) {
            return _MatrixInputter<T>(*this,row*this->cols()+col);
        }
    };

    template <typename T>
    class _MatrixView : public _MatrixLike<T> {
        _Matrix<T> & _mat;
    public:
        _Matrix<T> & refmat() {
            return _mat;
        };

        const _Matrix<T> & refmat() const {
            return _mat;
        };

        virtual MatrixView<T> block(const Range &rng) const = 0;

        virtual MatrixView<T> block(const Index<unsigned int> &ridx,
                                    const Index<unsigned int> &cidx) const = 0;

        virtual T &operator()(unsigned int idx) = 0;

        virtual const T &operator()(unsigned int idx) const = 0;

        virtual T &operator()(unsigned int row, unsigned int col) {
            return (*this)(row*this->size().cols+col);
        };

        virtual const T &operator()(unsigned int row, unsigned int col) const {
            return (*this)(row*this->size().cols+col);
        };

        virtual T &operator()(const Position& pos) {
            return (*this)(pos.row*this->size().cols+pos.col);
        };

        virtual const T &operator()(const Position& pos) const {
            return (*this)(pos.row*this->size().cols+pos.col);
        };

        virtual MatrixLike<T> clone() const = 0;

        virtual Size size() const = 0;

        virtual std::string class_name() const = 0;

        virtual Index<unsigned int> row_index() const = 0;

        virtual Index<unsigned int> col_index() const = 0;

        _MatrixView(_Matrix<T> &_mat) : _mat(_mat) {}
		
		virtual void destroy() {
			delete this;
		}
    };

    template <typename T>
    class _MatrixIndexView : public _MatrixView<T> {
        Index<unsigned int> _row_index;
        Index<unsigned int> _col_index;
    public:

        _MatrixIndexView(
                _Matrix<T> &_mat,
                const Index<unsigned int> &_rows,
                const Index<unsigned int> &_cols)
                : _MatrixView(_mat), _row_index(_rows), _col_index(_cols) {}

        virtual T &operator()(unsigned int row, unsigned int col) {
            return (*this)(row*this->size().cols+col);
        };

        virtual const T &operator()(unsigned int row, unsigned int col) const {
            return (*this)(row*this->size().cols+col);
        };

        virtual T &operator()(const Position& pos) {
            return (*this)(pos.row*this->size().cols+pos.col);
        };

        virtual const T &operator()(const Position& pos) const {
            return (*this)(pos.row*this->size().cols+pos.col);
        };

        virtual T &operator()(unsigned int idx) {
            return this->refmat()(idx/this->cols(),idx%this->cols());
        }

        virtual const T &operator()(unsigned int idx) const {
            return this->refmat()(idx/this->cols(),idx%this->cols());
        }

        virtual MatrixLike<T> clone() const {
            return new _MatrixIndexView<T>(const_cast<_Matrix<T>&>(this->refmat()),_row_index,_col_index);
        }

        virtual Size size() const {
            return Size(_row_index.size(), _col_index.size());
        }

        virtual std::string class_name() const {
            return "_MatrixIndexView";
        }

        virtual Index<unsigned int> row_index() const {
            return _row_index;
        }

        virtual Index<unsigned int> col_index() const {
            return _col_index;
        }

        virtual MatrixView<T> block(const Range &rng) const {
            Index<unsigned int> ridx, cidx;
            for(size_t r=rng.beg.row;r<rng.beg.row+rng.size.rows;r++)
                ridx.push_back(this->_row_index[r]);
            for(size_t c=rng.beg.col;c<rng.beg.col+rng.size.cols;c++)
                cidx.push_back(this->_col_index[c]);
            return new _MatrixIndexView<T>(const_cast<_Matrix<T>&>(this->refmat()),ridx,cidx);
        }

        virtual MatrixView<T> block(const Index<unsigned int> &ridx, const Index<unsigned int> &cidx) const {
            Index<unsigned int> nridx, ncidx;
            for(auto r:ridx)
                nridx.push_back(this->_row_index[ridx[r]]);
            for(auto c:cidx)
                ncidx.push_back(this->_col_index[cidx[c]]);
            return new _MatrixIndexView<T>(const_cast<_Matrix<T>&>(this->refmat()),nridx,ncidx);
        }



        _MatrixIndexView<T> &operator=(const _MatrixIndexView<T> &miv) {
            unsigned int cnt = miv.count();
            for(unsigned int i=0;i<cnt;i++)
                (*this)(i) = miv(i);
            return *this;
        }

        template <typename A> _MatrixIndexView<T>& operator=(const MatrixLike<A>& _mat) const {
            this->apply_inplace_2([](auto& x, auto& y){x=y;}, _mat);
            return *this;
        };

		virtual void destroy() {
			delete this;
		}
    };

    template <typename T>
    class _MatrixRangeView : public _MatrixView<T> {
        Range _rang;
    public:
        _MatrixRangeView(_Matrix<T> &_mat, const Range &_rang) : _MatrixView(_mat), _rang(_rang) {}

        virtual T &operator()(unsigned int row, unsigned int col) {
			return (*this)(row*this->size().cols+col);
        };

        virtual const T &operator()(unsigned int row, unsigned int col) const {
			return (*this)(row*this->size().cols+col);
        };

        virtual T &operator()(const Position& pos) {
            return (*this)(pos.row*this->size().cols+pos.col);
        };

        virtual const T &operator()(const Position& pos) const {
            return (*this)(pos.row*this->size().cols+pos.col);
        };

        virtual T &operator()(unsigned int idx) {
            return this->refmat()(idx/this->cols()+range().beg.row,idx%this->cols()+range().beg.col);
        }

        virtual const T &operator()(unsigned int idx) const {
            return this->refmat()(idx/this->cols()+range().beg.row,idx%this->cols()+range().beg.col);
        }

        virtual MatrixLike<T> clone() const {
            return new _MatrixRangeView<T>(const_cast<_Matrix<T>&>(this->refmat()),this->range());
        }

        virtual Size size() const {
            return this->_rang.size;
        }

        virtual std::string class_name() const {
            return "_MatrixRangeView";
        }

        virtual Range range() const {
            return this->_rang;
        }

        virtual MatrixView<T> block(const Range &rng) const {
            return new _MatrixRangeView(const_cast<_Matrix<T>&>(this->refmat()),
                    Range({this->range().beg.row+rng.beg.row,this->range().beg.col+rng.beg.col},
                          rng.size));
        }

        virtual MatrixView<T> block(const Index<unsigned int> &ridx, const Index<unsigned int> &cidx) const {
            Index<unsigned int> cridx = this->row_index();
            Index<unsigned int> ccidx = this->col_index();
            Index<unsigned int> nridx;
            Index<unsigned int> ncidx;
            for(unsigned int i=0;i<ridx.size();i++) nridx.push_back(cridx[ridx[i]]);
            for(unsigned int i=0;i<cidx.size();i++) ncidx.push_back(ccidx[cidx[i]]);
            return new _MatrixIndexView<T>(const_cast<_Matrix<T>&>(this->refmat()),nridx,ncidx);
        }

        virtual Index<unsigned int> row_index() const {
            return Index<unsigned int>::Linear(this->range().beg.row,this->range().beg.row+this->range().size.rows);
        }

        virtual Index<unsigned int> col_index() const {
            return Index<unsigned int>::Linear(this->range().beg.col,this->range().beg.col+this->range().size.cols);
        }

        _MatrixRangeView<T> &operator=(const _MatrixRangeView<T> &mrv) {
            unsigned int cnt = mrv.count();
            for(unsigned int i=0;i<cnt;i++)
                (*this)(i) = mrv(i);
            return *this;
        }

        template <typename A> _MatrixRangeView<T>& operator=(const MatrixLike<A>& _mat) const {
            this->apply_inplace_2([](auto& x, auto& y){x=y;}, _mat);
            return *this;
        };

		virtual void destroy() {
			delete this;
		}
    };

    template <typename T>
    class _Matrix : public _MatrixLike<T> {
        T * _data;
        Size _size;
    public:

        _Matrix(const Size &size) : _size(size), _data(new T[size.count()]) {};

        _Matrix(const Size &size, T v) : _size(size), _data(new T[size.count()]) {
            this->apply_inplace([&](auto &x){x=v;});
        };

        _Matrix(unsigned int rows, unsigned int cols) : _size(rows,cols), _data(new T[rows*cols]) {};

        _Matrix(unsigned int rows, unsigned int cols, T v) : _size(rows,cols), _data(new T[rows*cols]) {
            this->apply_inplace([&](auto &x){x=v;});
        };

        _Matrix(_Matrix<T>& me) : _size(me._size), _data(new T[me.count()]) {
            this->apply_inplace_2([](auto &x, auto &y){x=y;},me);
        };

        _Matrix(_Matrix<T>&& me) : _size(me._size), _data(me._data) {
            me._data = nullptr;
        };

        virtual MatrixView<T> block(unsigned int beg_row, unsigned int beg_col,
                                    unsigned int row_cnt, unsigned int col_cnt) const {
            return this->block(Range({beg_row,beg_col},{row_cnt,col_cnt}));
        };

        virtual MatrixView<T> block(const Range &rng) const {
            return new _MatrixRangeView<T>(const_cast<_Matrix<T>&>(*this),rng);
        }

        virtual MatrixView<T> block(const Index<unsigned int> &ridx, const Index<unsigned int> &cidx) const {
            return new _MatrixIndexView<T>(const_cast<_Matrix<T>&>(*this),ridx,cidx);
        }

        virtual T &operator()(unsigned int row, unsigned int col) {
            return (*this)(row*this->size().cols+col);
        };

        virtual const T &operator()(unsigned int row, unsigned int col) const {
            return (*this)(row*this->size().cols+col);
        };

        virtual T &operator()(const Position& pos) {
            return (*this)(pos.row*this->size().cols+pos.col);
        };

        virtual const T &operator()(const Position& pos) const {
            return (*this)(pos.row*this->size().cols+pos.col);
        };

        virtual T &operator()(unsigned int idx) {
            return _data[idx];
        }

        virtual const T &operator()(unsigned int idx) const {
            return _data[idx];
        }

        virtual MatrixLike<T> clone() const {
            auto p = new _Matrix<T>(this->size());
            *p = *this;
            return p;
        }

        virtual Size size() const {
            return this->_size;
        }

        virtual std::string class_name() const {
            return "_Matrix";
        }

        virtual Index<unsigned int> row_index() const {
            return Index<unsigned int>::Linear(0,this->rows());
        }

        virtual Index<unsigned int> col_index() const {
            return Index<unsigned int>::Linear(0,this->cols());
        }

        //using _MatrixLike<T>::operator<<;
        //using _MatrixLike<T>::operator=;
        using _MatrixLike<T>::operator();

        _Matrix<T>&operator=(const _Matrix<T>& _mat) {
            unsigned int cnt = this->count();
            for(unsigned int i=0;i<cnt;i++)
                (*this)(i) = _mat(i);
            return *this;
        };

        _Matrix<T>&operator=(_Matrix<T>&& _mat) {
            if(this->_data != nullptr){
                delete this->_data;
                this->_data = nullptr;
            }
            this->_size = _mat._size;
            this->_data = _mat._data;
            _mat._data = nullptr;
            return *this;
        };

        template <typename A> _Matrix<T>& operator=(const MatrixLike<A>& _mat) const {
            this->apply_inplace_2([](auto& x, auto& y){x=y;}, _mat);
            return *this;
        };

		virtual void destroy() {
			delete this;
		}

		~_Matrix() {
			delete _data;
			_data = nullptr;
		}
    };

    class Wrapper {
        void * _ptr;
    protected:
        Wrapper(void *ptr) : _ptr(ptr) {};
    public:
        virtual const void *get_ptr() const {
            return _ptr;
        }

        virtual void *get_ptr() {
            return _ptr;
        }

        virtual void set_ptr(void *ptr) {
            Wrapper::_ptr = ptr;
        }

        virtual ~Wrapper() {

        }
    };

    template <typename T>
    class MatrixLike : public Wrapper {
    public:
        _MatrixLike<T>* operator->() {
            return (_MatrixLike<T>*)get_ptr();
        };

        const _MatrixLike<T>* operator->() const {
            return (const _MatrixLike<T>*)get_ptr();
        };

        MatrixLike(void *ptr) : Wrapper(ptr) {};

        MatrixLike(const MatrixLike<T> &mat) : MatrixLike<T>(mat->clone()){};

        MatrixLike(MatrixLike<T> &&mat) : Wrapper(mat.get_ptr()) {
            mat.set_ptr(nullptr);
        };

        virtual ~MatrixLike() {
			if (get_ptr() != nullptr) {
				this->operator->()->destroy();
				set_ptr(nullptr);
			}
        };

        virtual T &operator()(unsigned int row, unsigned int col) {
            return this->operator->()->operator()(row,col);
        };

        virtual T &operator()(const Position& pos) {
            return this->operator->()->operator()(pos);
        };

        virtual T &operator()(unsigned int idx) {
            return this->operator->()->operator()(idx);
        };

        virtual const T &operator()(unsigned int row, unsigned int col) const {
            return this->operator->()->operator()(row,col);
        };

        virtual const T &operator()(const Position& pos) const {
            return this->operator->()->operator()(pos);
        };

        virtual const T &operator()(unsigned int idx) const {
            return this->operator->()->operator()(idx);
        };

        template <typename A> MatrixLike<T> &operator=(const MatrixLike<A> &mat) {
            this->operator->()->apply_inplace_2([](auto&x,auto&y){x=y;},mat);
            return *this;
        }

        virtual MatrixLike<T> &operator=(const MatrixLike<T> &mat) {
            this->operator->()->apply_inplace_2([](auto&x,auto&y){x=y;},mat);
            return *this;
        }
    };

    template <typename T>
    class MatrixView : public MatrixLike<T> {
    public:
        _MatrixView<T>* operator->() {
            return (_MatrixView<T>*)get_ptr();
        };

        const _MatrixView<T>* operator->() const {
            return (const _MatrixView<T>*)get_ptr();
        };

        MatrixView(void *ptr) : MatrixLike<T>(ptr) {};

        MatrixView(const MatrixView<T> &mat) : MatrixLike<T>(mat->clone()) {};

        MatrixView(MatrixView<T> &&mat) : MatrixLike<T>(mat) {};

        virtual ~MatrixView() {
			if (get_ptr() != nullptr) {
				this->operator->()->destroy();
				set_ptr(nullptr);
			}
        };

        template <typename A> MatrixView<T> &operator=(const MatrixLike<A> &mat) {
            this->operator->()->apply_inplace_2([](auto&x,auto&y){x=y;},mat);
            return *this;
        }

        virtual MatrixView<T> &operator=(const MatrixView<T> &mat) {
            this->operator->()->apply_inplace_2([](auto&x,auto&y){x=y;},mat);
            return *this;
        }

        virtual T &operator()(unsigned int row, unsigned int col) {
            return this->operator->()->operator()(row,col);
        };

        virtual T &operator()(const Position& pos) {
            return this->operator->()->operator()(pos);
        };

        virtual T &operator()(unsigned int idx) {
            return this->operator->()->operator()(idx);
        };

        virtual const T &operator()(unsigned int row, unsigned int col) const {
            return this->operator->()->operator()(row,col);
        };

        virtual const T &operator()(const Position& pos) const {
            return this->operator->()->operator()(pos);
        };

        virtual const T &operator()(unsigned int idx) const {
            return this->operator->()->operator()(idx);
        };
    };

    template <typename T>
    class MatrixIndexView : public MatrixView<T> {
    public:
        _MatrixIndexView<T>* operator->() {
            return (_MatrixIndexView<T>*)get_ptr();
        };

        const _MatrixIndexView<T>* operator->() const {
            return (const _MatrixIndexView<T>*)get_ptr();
        };

        MatrixIndexView(void *ptr) : MatrixView<T>(ptr) {};

        MatrixIndexView(const MatrixIndexView<T> &mat) : MatrixView<T>(mat->clone()) {};

        MatrixIndexView(const MatrixIndexView<T> &&mat) : MatrixView<T>(mat) {};

        virtual ~MatrixIndexView() {
			if (get_ptr() != nullptr) {
				this->operator->()->destroy();
				set_ptr(nullptr);
			}
        };

        template <typename A> MatrixIndexView<T> &operator=(const MatrixLike<A> &mat) {
            this->operator->()->apply_inplace_2([](auto&x,auto&y){x=y;},mat);
            return *this;
        }

        virtual MatrixIndexView<T> &operator=(const MatrixIndexView<T> &mat) {
            this->operator->()->apply_inplace_2([](auto&x,auto&y){x=y;},mat);
            return *this;
        }

        virtual T &operator()(unsigned int row, unsigned int col) {
            return this->operator->()->operator()(row,col);
        };

        virtual T &operator()(const Position& pos) {
            return this->operator->()->operator()(pos);
        };

        virtual T &operator()(unsigned int idx) {
            return this->operator->()->operator()(idx);
        };

        virtual const T &operator()(unsigned int row, unsigned int col) const {
            return this->operator->()->operator()(row,col);
        };

        virtual const T &operator()(const Position& pos) const {
            return this->operator->()->operator()(pos);
        };

        virtual const T &operator()(unsigned int idx) const {
            return this->operator->()->operator()(idx);
        };
    };

    template <typename T>
    class MatrixRangeView : public MatrixView<T> {
    public:
        _MatrixRangeView<T>* operator->() {
            return (_MatrixRangeView<T>*)get_ptr();
        };

        const _MatrixRangeView<T>* operator->() const {
            return (const _MatrixRangeView<T>*)get_ptr();
        };

        MatrixRangeView(void *ptr) : MatrixView<T>(ptr) {};

        MatrixRangeView(const MatrixRangeView<T> &mat) : MatrixView<T>(mat->clone()) {};

        MatrixRangeView(MatrixRangeView<T> &&mat) : MatrixView<T>(mat) {};

        virtual ~MatrixRangeView() {
			if (get_ptr() != nullptr) {
				this->operator->()->destroy();
				set_ptr(nullptr);
			}
        };

        template <typename A> MatrixRangeView<T> &operator=(const MatrixLike<A> &mat) {
            this->operator->()->apply_inplace_2([](auto&x,auto&y){x=y;},mat);
            return *this;
        }

        virtual MatrixRangeView<T> &operator=(const MatrixRangeView<T> &mat) {
            this->operator->()->apply_inplace_2([](auto&x,auto&y){x=y;},mat);
            return *this;
        }

        virtual T &operator()(unsigned int row, unsigned int col) {
            return this->operator->()->operator()(row,col);
        };

        virtual T &operator()(const Position& pos) {
            return this->operator->()->operator()(pos);
        };

        virtual T &operator()(unsigned int idx) {
            return this->operator->()->operator()(idx);
        };

        virtual const T &operator()(unsigned int row, unsigned int col) const {
            return this->operator->()->operator()(row,col);
        };

        virtual const T &operator()(const Position& pos) const {
            return this->operator->()->operator()(pos);
        };

        virtual const T &operator()(unsigned int idx) const {
            return this->operator->()->operator()(idx);
        };
    };

    template <typename T>
    class Matrix : public MatrixLike<T> {
    public:
        _Matrix<T>* operator->() {
            return (_Matrix<T>*)get_ptr();
        };

        const _Matrix<T>* operator->() const {
            return (const _Matrix<T>*)get_ptr();
        };

        Matrix(void *ptr) : MatrixLike<T>(ptr) {};

        Matrix(const Matrix<T> &mat) : MatrixLike<T>(mat->clone()) {};

        Matrix(Matrix<T> &&mat) : MatrixLike<T>(mat) {};

        Matrix(unsigned int rows,unsigned int cols,T v) : MatrixLike<T>(new _Matrix<T>(rows,cols,v)) {}

        Matrix(const Size &size,T v) : MatrixLike<T>(new _Matrix<T>(size,v)) {}

        Matrix(const Size &size) : MatrixLike<T>(new _Matrix<T>(size)) {}

        virtual ~Matrix() {
			if (get_ptr() != nullptr) {
				this->operator->()->destroy();
				set_ptr(nullptr);
			}
        };

        template <typename A> Matrix<T> &operator=(const MatrixLike<A> &mat) {
            this->operator->()->apply_inplace_2([](auto&x,auto&y){x=y;},mat);
            return *this;
        }

        virtual Matrix<T> &operator=(const Matrix<T> &mat) {
            this->operator->()->apply_inplace_2([](auto&x,auto&y){x=y;},mat);
            return *this;
        }

        virtual T &operator()(unsigned int row, unsigned int col) {
            return this->operator->()->operator()(row,col);
        };

        virtual T &operator()(const Position& pos) {
            return this->operator->()->operator()(pos);
        };

        virtual T &operator()(unsigned int idx) {
            return this->operator->()->operator()(idx);
        };

        virtual const T &operator()(unsigned int row, unsigned int col) const {
            return this->operator->()->operator()(row,col);
        };

        virtual const T &operator()(const Position& pos) const {
            return this->operator->()->operator()(pos);
        };

        virtual const T &operator()(unsigned int idx) const {
            return this->operator->()->operator()(idx);
        };
    };

    template <typename T>
    std::ostream &operator<<(std::ostream &os, const MatrixLike<T> &mat) {
        unsigned int rn = mat->rows();
        unsigned int cn = mat->cols();
        os<<mat->class_name()<<": "<<rn<<"x"<<cn<<endl;
        for(unsigned int r=0;r<rn;r++) {
            os<<"\t";
            for(unsigned int c=0;c<cn;c++)
                os<<mat(r,c)<<(c!=cn-1?",\t":"\n");
        }
        return os;
    }

    template <typename T>
    class _MatrixInputter {
        _MatrixLike<T>& _mat;
        unsigned int _idx;
    public:
        template <typename A>
        _MatrixInputter<T> &operator,(const A&a) {
            _mat(_idx) = a;
            _idx = (_idx + 1) % _mat.count();
            return *this;
        }

        template <typename A>
        _MatrixInputter<T> &operator<<(const A&a) {
            _mat(_idx) = a;
            _idx = (_idx + 1) % _mat.count();
            return *this;
        }

        _MatrixInputter(_MatrixLike<T> &mat, unsigned int idx=0) : _mat(mat), _idx(idx) {}
    };
}


#endif //MATRIX2_MATRIX_H
