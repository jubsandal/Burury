#ifndef MATRIX_H_J2PRSDIL
#define MATRIX_H_J2PRSDIL

#include <initializer_list>
#include <type_traits>
#include <functional>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <string>
#include <cmath>

template<typename T>
class Matrix;
template<typename T>
class Matrix4x4;

template<typename T>
class Matrix {
    public:
        Matrix(); // 2x2
        Matrix(int row, int col);
        Matrix(const std::initializer_list<const std::initializer_list<T>>& Tlist);
        Matrix(const Matrix<T>& other);
        virtual ~Matrix();

        Matrix<T>         getPowerOf(int d) const;    /* GET MATRIX EXPONENTIATION */
        virtual void      transpose();                /* TRANSPOSE THIS MATRIX */
        virtual Matrix<T> getTransposition() const;   /* GET TRANSPOSED MATRIX */
        T                 minor(int r, int c) const;  /* GET MATRIX MINOR */
        T                 cofactor(int r, int c) const; /* GET MATRIX COFACTOR */
        /* void              toTriangle();               /1* TRANSLATE THIS MATRIX TO TRIANGLE *1/ */
        /* Matrix<T>         getTriangle() const;        /1* GET TRANSLATED THIS MATRIX TO TRIANGLE *1/ */
        T                 determinant() const;        /* GET MATRIX DETERMINANT */

        /* APPLY FUNCTION(curr, curc, val) TO ALL VALUES IN MATRIX
         * WHERE curr - CURRENT ROW, curc - CURRENT COLUMN, val - VALUE IN {curr, curc}
         * return value of the function then equas to curent iteration matrix cell */
        void apply(std::function<T (int curr, int curc, T)>);
        void apply(std::function<T (T)>); /* APPLY FUNCTION(val) TO ALL VALUE IN MATRIX */
        bool isQuad() const;

        virtual T& operator()(int row, int col); // GET MATRIX CELL IN {row, col}
        virtual T& operator()(int row, int col) const;
        T& operator[](const int ind);            // GET MATRIX CELL IN {num}
        T& operator[](const int ind) const;

        Matrix<T>& operator=(const Matrix<T>& other);

        const int rows;
        const int cols;

    protected:
        int maxNulFactorRow() const;

        //mb unique_ptr?
        T *m_matrix;
};

template <typename T> std::ostream& operator<<(std::ostream &out, const Matrix<T>& v);

template<typename T> Matrix<T> operator+ (const Matrix<T>& m1, const Matrix<T>& m2);  // MATRIX SUM //
template<typename T> Matrix<T> operator- (const Matrix<T>& m1, const Matrix<T>& m2);  // MATRIX SUB //
template<typename T> Matrix<T> operator* (const Matrix<T>& m1, const Matrix<T>& m2);  // MATRIX MUL BY MATRIX //
template<typename T> Matrix<T> operator* (const Matrix<T>& m1, const T&         val); // MATRIX MUL BY VALUE //
template<typename T> bool      operator==(const Matrix<T>& m1, const Matrix<T>& m2);  // MATRIX EQUALITY //
template<typename T> bool      operator!=(const Matrix<T>& m1, const Matrix<T>& m2);  // MATRIX UNEQUALITY //

template<typename T>
class Matrix4x4 : public Matrix<T> {
    public:
        explicit Matrix4x4();
        Matrix4x4(const std::initializer_list<const std::initializer_list<T>>& Tlist);
        virtual ~Matrix4x4();

        Matrix4x4& createScale(T scale) const;                   /* RETURN NEW SCALE BY scale MATRIX */
        Matrix4x4& createRotation(T yaw, T pitch, T roll) const; /* RETURN NEW ROTATION BY y,p,r MATRIX */
        Matrix4x4& createTranslation(T x, T y, T z, T w) const;  /* RETURN NEW TRANSLATION BY x,y,z,w MATRIX */

        virtual T& operator()(int row, int col);
        virtual T& operator()(int row, int col) const;

        virtual Matrix4x4<T>& operator=(const Matrix4x4<T>& other);
};

template<typename T> Matrix4x4<T> operator+ (const Matrix4x4<T>& m1, const Matrix4x4<T>& m2);  // MATRIX SUM //
template<typename T> Matrix4x4<T> operator- (const Matrix4x4<T>& m1, const Matrix4x4<T>& m2);  // MATRIX SUB //
template<typename T> Matrix4x4<T> operator* (const Matrix4x4<T>& m1, const Matrix4x4<T>& m2);  // MATRIX MUL BY MATRIX //
template<typename T> Matrix4x4<T> operator* (const Matrix4x4<T>& m1, const T&         val); // MATRIX MUL BY VALUE //

#include "Matrix.inl"

typedef Matrix<float>        Matrixf;
typedef Matrix<int>          Matrixi;
typedef Matrix<unsigned int> Matrixu;

typedef Matrix4x4<float>        Matrix4x4f;
typedef Matrix4x4<int>          Matrix4x4i;
typedef Matrix4x4<unsigned int> Matrix4x4u;

#endif /* end of include guard: MATRIX_H_J2PRSDIL */
