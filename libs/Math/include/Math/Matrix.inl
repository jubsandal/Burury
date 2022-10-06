/**********************************************************************
*                               Matrix                               *
**********************************************************************/

template<typename T>
Matrix<T>::Matrix() :
    Matrix(2, 2)
{ }

template<typename T>
Matrix<T>::Matrix(int r, int c) :
    rows(r),
    cols(c),
    m_matrix(new T[this->rows * this->cols])
{
    assert(r > 0 && c > 0);
    T l_val;
    for (int l_row = 0; l_row < this->rows; ++l_row) {
        for (int l_col = 0; l_col < this->cols; ++l_col) {
            if (l_row - l_col == 0) {
                l_val = 1;
            } else {
                l_val = 0;
            }
            m_matrix[l_row * this->cols + l_col] = l_val;
        }
    }
}


template<typename T>
Matrix<T>::Matrix(const Matrix<T>& other) :
    cols(other.cols),
    rows(other.rows),
    m_matrix(new T[this->cols * this->rows])
{
    assert(this->rows > 0 && this->cols > 0);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            this->m_matrix[i * cols + j] = other(i, j);
            /* this->m_matrix[i * cols + j] = other[i * other.cols + j]; */
        }
    }
}

//its ok?
template<typename T>
Matrix<T>::Matrix(const std::initializer_list<const std::initializer_list<T>>& Tlist) :
    Matrix(Tlist.size(), Tlist.begin()->size())
{
    /* for (auto& x : Tlist) */
    /*     assert(x.size() > this->cols); */
    auto it = Tlist.begin();  // iterate through outer list
    for (int l_r = 0; l_r < this->rows; l_r++, it++) {
        auto inIt = it->begin();  // iterate through inner list
        for (int l_c = 0; l_c < this->cols; ++l_c, ++inIt) {
            m_matrix[l_r * this->cols + l_c] = *inIt;
        }
    }
}

template<typename T>
Matrix<T>::~Matrix()
{
    delete [] m_matrix;
}

template<typename T>
void Matrix<T>::apply(std::function<T (T)> f)
{
    for (int l_row = 0; l_row < this->rows; ++l_row) {
        for (int l_col = 0; l_col < this->cols; ++l_col) {
            m_matrix[l_row * this->cols + l_col] = f(m_matrix[l_row * this->cols + l_col]);
        }
    }
}

template<typename T>
void Matrix<T>::apply(std::function<T (int r, int c, T)> f)
{
    for (int l_row = 0; l_row < this->rows; ++l_row) {
        for (int l_col = 0; l_col < this->cols; ++l_col) {
            m_matrix[l_row * this->cols + l_col] = f(l_row, l_col, m_matrix[l_row * this->cols + l_col]);
        }
    }
}

template<typename T>
bool Matrix<T>::isQuad() const
{
    return this->cols == this->rows;
}

template<typename T>
Matrix<T> Matrix<T>::getPowerOf(int d) const
{
    assert(isQuad());
    Matrix<T> ret(*this);
    while (--d > 0) {
        ret = ret * (*this);
    }
    return ret;
}

template<typename T>
Matrix<T> Matrix<T>::getTransposition() const
{
    assert(this->isQuad());
    Matrix<T> ret(3, 3);
    for (int r = 0; r < ret.rows; r++) {
        for (int c = 0; c < ret.cols; c++) {
            for (int i = 0; i < ret.rows; i++) {
                ret(r, c) = m_matrix[c * cols + r];
            }
        }
    }

    return ret;
}

template<typename T>
void Matrix<T>::transpose()
{
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            for (int i = 0; i < rows; i++) {
                this->m_matrix[r * cols + c] = this->m_matrix[c * cols + r];
            }
        }
    }
}

template<typename T>
T Matrix<T>::minor(int l_row, int l_col) const
{
    Matrix<T> mtx(rows - 1, cols - 1);
    int l_cur_row = 0, l_cur_col = 0;

    for (int i = 0; i < this->rows; i++) {
        if (i != l_row) {
            l_cur_col = 0;
            for (int j = 0; j < cols; j++) {
                if (j != l_col) {
                    mtx(l_cur_row, l_cur_col) = m_matrix[i * cols + j];
                    l_cur_col++;
                }
            }
            l_cur_row++;
        }
    }

    return mtx.determinant();
}

template<typename T>
T Matrix<T>::cofactor(int row, int col) const
{
    T cofactor = minor(row, col);

    return ((row + col) % 2 == 0) ? cofactor : -cofactor;
}

/* template<typename T> */
/* void Matrix<T>::toTriangle() */
/* { */
/*     T l_mul; */
/*     for (int l_line = 0; l_line < (rows - 1); l_line++) { */
/*         for (int l_row = 0; l_row < (rows - 1); l_row++) { */
/*             l_mul = -1 * (this->m_matrix[(l_row + 1) * cols + l_line] / */
/*                     this->m_matrix[l_line * cols + l_line]); */
/*             for (int l_col = 0; l_col < cols; l_col++) { */
/*                 this->m_matrix[(l_row + 1) * cols + l_col] = this->m_matrix[(l_row + 1) * cols + l_col] + */
/*                     this->m_matrix[l_line * cols + l_col] * l_mul; */
/*             } */
/*         } */
/*     } */
/* } */

/* template<typename T> */
/* Matrix<T> Matrix<T>::getTriangle() const */
/* { */
/*     Matrix<T> ret(*this); */
/*     ret.toTriangl(); */
/*     return ret; */
/* } */

template<typename T>
T Matrix<T>::determinant() const
{
    T det = 0;

    switch (rows) {
        /* 2x2 */
        case 2:
            det = m_matrix[0] * m_matrix[3] -
                  m_matrix[1] * m_matrix[2];
            break;

        /* 3x3 */
        case 3:
            det = (m_matrix[0] * m_matrix[4] * m_matrix[8] +
               m_matrix[3] * m_matrix[7] * m_matrix[2] +
               m_matrix[1] * m_matrix[5] * m_matrix[6]) -
              (m_matrix[2] * m_matrix[4] * m_matrix[6] +
               m_matrix[1] * m_matrix[3] * m_matrix[8] +
               m_matrix[0] * m_matrix[5] * m_matrix[7]);
            break;

        /* higer */
        default:;
            int l_row = this->maxNulFactorRow();
            for (int col = 0; col < cols; col++) {
                det += m_matrix[l_row * cols + col] * this->cofactor(l_row, col);
            }
    }

    return det;
}

template<typename T>
int Matrix<T>::maxNulFactorRow() const
{
    int l_max, l_cur, ret;
    ret = l_max = 0;

    for (int l_row = 0; l_row < rows; l_row++) {
        l_cur = 0;
        for (int l_col = 0; l_col < cols; l_col++) {
            if (m_matrix[l_row * cols + l_col] == 0) {
                l_cur++;
            }
        }
        if (l_cur > l_max) {
            l_max = l_cur;
            ret = l_row;
        }
    }

    return ret;
}

/* template<typename T> */
/* operator Matrix<T>::Matrix4x4<T>() */
/* { */
/*     return Matrix4x4<T>(*this); */
/* } */

template<typename T>
Matrix<T> operator+(const Matrix<T>& m1, const Matrix<T>& m2)
{
    assert(m1.cols == m2.cols && m1.rows == m2.rows);
    /* add check for eq r1, r2; c1, c2: <05-01-22, siisgoo> */
    Matrix<T> ret(m1.rows, m1.cols);
    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            ret(i, j) = m1(i, j) + m2(i, j);
        }
    }
    return ret;
}

template<typename T>
Matrix<T> operator-(const Matrix<T>& m1, const Matrix<T>& m2)
{
    assert(m1.cols == m2.cols && m1.rows == m2.rows);

    Matrix<T> ret(m1.rows, m1.cols);
    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            ret(i, j) = m1(i, j) - m2(i, j);
        }
    }
    return ret;
}

template<typename T>
Matrix<T> operator*(const Matrix<T>& m1, const Matrix<T>& m2)
{
    assert(m1.cols == m2.cols && m1.rows == m2.rows);
    T sum;
    Matrix<T> ret(m1.rows, m2.cols);
    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            sum = 0;
            for (int v = 0; v < m1.rows; v++) {
                sum += m1(i, v) * m2(v, j);
            }
            ret(i, j) = sum;
        }
    }
    return ret;
}

template<typename T>
Matrix<T> operator*(const Matrix<T>& m1, const T& val)
{
    Matrix<T> ret(m1.rows, m1.cols);
    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            ret(i, j) = m1(i, j) * val;
        }
    }
    return ret;
}

template<typename T>
T& Matrix<T>::operator()(int row, int col)
{
    assert(row <= rows && col <= cols);
    return m_matrix[row * this->cols + col];
}

template<typename T>
T& Matrix<T>::operator()(int row, int col) const
{
    assert(row <= rows && col <= cols);
    return m_matrix[row * this->cols + col];
}

template<typename T>
T& Matrix<T>::operator[](const int ind)
{
    assert(ind <= cols*rows);
    return m_matrix[ind];
}

template<typename T>
T& Matrix<T>::operator[](const int ind) const
{
    assert(ind <= cols*rows);
    return m_matrix[ind];
}

template<typename T>
bool operator==(const Matrix<T>& m1, const Matrix<T>& m2)
{
    assert(m1.cols == m2.cols && m1.rows == m2.rows);

    const float epsilon = 0.00001;
    for (int i = 0; i < m1.rows; ++i) {
        for (int j = 0; j < m2.cols; ++j) {
            if(std::is_same<T, float>::value || std::is_same<T, double>::value) {
                if (!(std::abs(m1(i, j) - m2(i, j)) <=
                            (std::max(std::abs(m1(i, j)), std::abs(m2(i, j))) * epsilon))) { //thanks, knoth
                    return false;
                }
            } else {
                if (m1(i, j) != m2(i, j)) {
                    return false;
                }
            }
        }
    }

    return true;
}

template<typename T>
bool operator!=(const Matrix<T>& m1, const Matrix<T>& m2)
{
    return !(m1 == m2);
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other)
{
    assert(this->cols == other.cols && this->rows == other.rows);
    this->apply([other](int r, int c, T cur) -> T {
            return other(r, c);
        });
    return *this;
}

static int get_float_digits(double num)
{
    int digits = 0;
    double ori = num; //storing original number
    long num2  = num;
    while (num2 > 0) {
        digits++;
        num2 = num2/10;
    }
    if (ori == 0)
        digits=1;
    num = ori;
    double no_float;
    no_float = ori*(pow(10, (16-digits)));
    long long int total=(long long int)no_float;
    int no_of_digits, extrazeroes=0;
    for(int i = 0; i < 16; i++)
    {
        int dig;
        dig   = total%10;
        total = total/10;
        if(dig != 0)
            break;
        else
            extrazeroes++;
    }
    no_of_digits = 16-extrazeroes;
    return no_of_digits;
}

template <typename T>
std::ostream& operator<<(std::ostream &out, const Matrix<T>& mtx)
{
    unsigned int max_len;
    {
        std::string buf;
        unsigned int cur_len = 0;
        max_len = 0;
        for (int l_row = 0; l_row < mtx.rows; l_row++) {
            for (int l_col = 0; l_col < mtx.cols; l_col++) {
                cur_len = get_float_digits(mtx(l_row, l_col));
                if (mtx(l_row, l_col) < 0) {
                    cur_len++;
                }
                if (cur_len > max_len) {
                    max_len = cur_len;
                }
            }
        }
    }

    std::string prefix("Matrix[" + std::to_string(mtx.rows) + ", " + std::to_string(mtx.cols) + "] = ");
    out << prefix;

    const int token_syms = 2; //todo
    for (int l_row = 0; l_row < mtx.rows; l_row++) {
        if (l_row > 0) { //white spaces
            for (int i = 0; i < prefix.length(); i++) {
                out << " ";
            }
        }
        out << "| ";
        for (int l_col = 0; l_col < mtx.cols; l_col++) {
            /* out << std::setw(max_len) << std::setprecision(token_syms) << mtx(l_row, l_col); */
            out << std::setw(max_len) << mtx(l_row, l_col);
            if (l_col != mtx.cols-1) {
                out << ", ";
            }
        }
        out << " |" << std::endl;
    }

   return out;
}

/**********************************************************************
*                             Matrix4x4                              *
**********************************************************************/

template<typename T>
Matrix4x4<T>::Matrix4x4() :
    Matrix<T>::Matrix(4, 4)
{ }

template<typename T>
Matrix4x4<T>::Matrix4x4(const std::initializer_list<const std::initializer_list<T>>& Tlist) :
    Matrix4x4()
{
    assert(Tlist.size() == 4 && Tlist.begin()->size() == 4);
    auto it = Tlist.begin();
    for (int l_r = 0; l_r < this->rows; l_r++, it++) {
        auto inIt = it->begin();
        for (int l_c = 0; l_c < this->cols; ++l_c, ++inIt) {
            this->m_matrix[l_r * this->cols + l_c] = *inIt;
        }
    }
}

template <typename T>
Matrix4x4<T>::~Matrix4x4()
{  }

template<typename T>
Matrix4x4<T>& Matrix4x4<T>::createScale(T scale) const
{
    Matrix4x4<T> ret;

    return ret;
}

template<typename T>
Matrix4x4<T>& Matrix4x4<T>::createRotation(T yaw, T pitch, T roll) const
{
    Matrix4x4<T> ret;

    return ret;
}

template<typename T>
Matrix4x4<T>& Matrix4x4<T>::createTranslation(T x, T y, T z, T w) const
{
    Matrix4x4<T> ret;

    return ret;
}

template<typename T>
T& Matrix4x4<T>::operator()(int row, int col)
{
    assert(row >= 0 && row < 4 &&
            col >= 0 && col < 4);
    return this->m_matrix[row * this->cols + col];
}

template<typename T>
T& Matrix4x4<T>::operator()(int row, int col) const
{
    assert(row >= 0 && row < 4 &&
            col >= 0 && col < 4);
    return this->m_matrix[row * this->cols + col];
}

template<typename T>
Matrix4x4<T>& Matrix4x4<T>::operator=(const Matrix4x4<T>& other)
{
    this->apply([other](int r, int c, T cur) -> T {
            return other(r, c);
        });
    return *this;
}

template<typename T>
Matrix4x4<T> operator+(const Matrix4x4<T>& m1, const Matrix4x4<T>& m2)
{
    assert(m1.cols == m2.cols && m1.rows == m2.rows);
    /* add check for eq r1, r2; c1, c2: <05-01-22, siisgoo> */
    Matrix4x4<T> ret;
    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            ret(i, j) = m1(i, j) + m2(i, j);
        }
    }
    return ret;
}

template<typename T>
Matrix4x4<T> operator-(const Matrix4x4<T>& m1, const Matrix4x4<T>& m2)
{
    Matrix4x4<T> ret;
    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            ret(i, j) = m1(i, j) - m2(i, j);
        }
    }
    return ret;
}

template<typename T>
Matrix4x4<T> operator*(const Matrix4x4<T>& m1, const Matrix4x4<T>& m2)
{
    T sum;
    Matrix4x4<T> ret;
    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            sum = 0;
            for (int v = 0; v < m1.rows; v++) {
                sum += m1(i, v) * m2(v, j);
            }
            ret(i, j) = sum;
        }
    }
    return ret;
}

template<typename T>
Matrix4x4<T> operator*(const Matrix4x4<T>& m1, const T& val)
{
    Matrix4x4<T> ret;
    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            ret(i, j) = m1(i, j) * val;
        }
    }
    return ret;
}
