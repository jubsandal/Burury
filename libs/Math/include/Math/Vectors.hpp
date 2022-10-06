#ifndef VECTORS_HPP_ZXYM1JZV
#define VECTORS_HPP_ZXYM1JZV

#include "General/Matrix.hpp"
#include <cstddef>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <cassert>

template <typename T>
class Vector : public Matrix<T> {
    public:
        Vector(); //2x1
        Vector(const int co);
        Vector(const std::initializer_list<T>& Tlist);
        Vector(const Vector<T>& other);
        virtual ~Vector();

        virtual T& operator()(int const& i);
        virtual T& operator()(int const& i) const;

        Vector<T>& operator=(const Vector<T>& rhs);

        Vector<T>  getNormalized() const;
        T           magnitude() const;
        T           dotProduct(Vector<T>& s) const; //скалярное произведение
        T           projection(Vector<T>& s) const;
        Vector<T>& truncate(T val) const;

        /* const std::size_t size; */
        const int size;

    private:
        using Matrix<T>::cols;
        using Matrix<T>::rows;
};

template <typename T> std::ostream& operator<<(std::ostream &out, const Vector<T>& v);

template <typename T> Vector<T> operator+(const Vector<T>& v1, const Vector<T>& v2);
template <typename T> Vector<T> operator-(const Vector<T>& v1, const Vector<T>& v2);
template <typename T> Vector<T> operator*(const Vector<T>& v1, const Vector<T>& v2); //cross product
template <typename T> Vector<T> operator*(const Vector<T>& v1, const T& val);
template <typename T> Vector<T> operator*(const Vector<T>& v1, const Matrix<T>& m1);
template <typename T> Vector<T> operator*(const Matrix<T>& m1, const Vector<T>& v1);

template <typename T>
class Vector4 : Vector<T> {
    public:
        Vector4();
        Vector4(const std::initializer_list<T>& Tlist);
        ~Vector4();

    private:
};

#include "Vectors.inl"

typedef Vector<int>          Vectori;
typedef Vector<unsigned int> Vectoru;
typedef Vector<float>        Vectorf;

#endif /* end of include guard: VECTORS_HPP_ZXYM1JZV */
