#ifndef QRK_MATRIX
#define QRK_MATRIX

#include <array>
#include <iostream>
#include <vector>

///////////////////////////////////////////////////////////////////////////
// Row major matrix library with addition, subtraction, and multiplication.
//
// To define a new matrix type:
// typedef Matrix<type, columns, rows> name;
//
// or directly use the matrix class:
// Matrix<type, columns, rows> name(*matrix data*)
///////////////////////////////////////////////////////////////////////////
namespace qrk {
//main matrix class
template<typename mat_type, size_t columns, size_t rows>
class Matrix {
public:
    Matrix(){};
    Matrix(const std::array<std::array<mat_type, rows>, columns> &_matrix)
        : data(_matrix) {}
    Matrix(const std::vector<std::vector<mat_type>> &_matrix) {
        if (_matrix.size() == this->data.size() &&
            _matrix[0].size() == this->data[0].size()) {
            for (int i = 0; i < this->data.size(); i++)
                for (int j = 0; j < this->data[0].size(); j++)
                    this->data[i][j] = _matrix[i][j];
        } else
            std::cout << "could not copy vector, incompatible size. Size of "
                         "this: "
                      << this->data.size() << " " << this->data[0].size()
                      << " Size of copied: " << _matrix.size() << " "
                      << _matrix[0].size() << std::endl;
    }

    //operator definitions
    Matrix operator+(mat_type &scalar) {
        for (int i = 0; i < this->data.size(); i++)
            for (int j = 0; j < this->data[0].size(); j++)
                this->data[i][j] += scalar;
        return *this;
    }
    Matrix operator+(Matrix &other) {
        if (this->data.size() != other.data.size()) return *this;
        if (this->data[0].size() != other.data[0].size()) return *this;

        for (int i = 0; i < this->data.size(); i++)
            for (int j = 0; j < this->data[0].size(); j++)
                this->data[i][j] += other.data[i][j];
        return *this;
    }
    Matrix operator-(mat_type &scalar) {
        for (int i = 0; i < this->data.size(); i++)
            for (int j = 0; j < this->data[0].size(); j++)
                this->data[i][j] -= scalar;
        return *this;
    }
    Matrix operator-(Matrix &other) {
        if (this->data.size() != other.data.size()) return *this;
        if (this->data[0].size() != other.data[0].size()) return *this;

        for (int i = 0; i < this->data.size(); i++)
            for (int j = 0; j < this->data[0].size(); j++)
                this->data[i][j] -= other.data[i][j];
        return *this;
    }
    Matrix operator*(mat_type &scalar) {
        for (int i = 0; i < this->data.size(); i++)
            for (int j = 0; j < this->data[0].size(); j++)
                this->data[i][j] *= scalar;
    }
    Matrix operator*(Matrix &other) {
        if (this->data[0].size() != other.data.size()) return *this;
        std::vector<std::vector<mat_type>> result(
                this->data[0].size(),
                std::vector<mat_type>(other.data.size(), 0));
        for (int i = 0; i < this->data.size(); i++)
            for (int j = 0; j < other.data[0].size(); j++)
                for (int k = 0; k < this->data.size(); k++)
                    result[i][j] += this->data[i][k] * other.data[k][j];
        return result;
    }
    Matrix operator=(Matrix &other) {
        if (this->data.size() != other.data.size()) return *this;
        if (this->data[0].size() != other.data[0].size()) return *this;

        for (int i = 0; i < this->data.size(); i++)
            for (int j = 0; j < this->data[0].size(); j++)
                this->data[i][j] = other.data[i][j];
        return *this;
    }

    Matrix TransposeMatrix() {
        std::array<std::array<mat_type, columns>, rows> result;
        for (int i = 0; i < this->data.size(); i++)
            for (int j = 0; j < this->data[0].size(); j++)
                result[i][j] = data[j][i];
        return Matrix(result);
    }

    //		columns ↓	rows ↓
    std::array<std::array<mat_type, columns>, rows> data;
};

//type definitions for common matrices
typedef Matrix<float, 2, 2> mat2;
typedef mat2 mat2x2;
typedef Matrix<float, 3, 3> mat3;
typedef mat3 mat3x3;
typedef Matrix<float, 4, 4> mat4;
typedef mat4 mat4x4;

typedef Matrix<float, 2, 1> mat2x1;
typedef Matrix<float, 2, 3> mat2x3;
typedef Matrix<float, 2, 4> mat2x4;

typedef Matrix<float, 3, 1> mat3x1;
typedef Matrix<float, 3, 2> mat3x2;
typedef Matrix<float, 3, 4> mat3x4;

typedef Matrix<float, 4, 1> mat4x1;
typedef Matrix<float, 4, 2> mat4x2;
typedef Matrix<float, 4, 3> mat4x3;
}// namespace qrk
#endif// !QRK_MATRIX