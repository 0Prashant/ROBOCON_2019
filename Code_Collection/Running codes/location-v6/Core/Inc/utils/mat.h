/*
 * mat.h
 * 
 * Created : 11/12/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _MAT_H_
#define _MAT_H_

#include <math.h>
#include "vec3.h"
#include "main.h"

#define MAX_MATRIX_ROWS         3
#define MAX_MATRIX_COLS         3

class Mat
{
public:
        Mat() : Mat(0,0) { }
        Mat(uint8_t rows, uint8_t columns) {
                if (!(rows < MAX_MATRIX_ROWS && columns < MAX_MATRIX_COLS)) {
                        _Error_Handler(__FILE__, __LINE__);
                }
                rows_ = rows;
                cols_ = columns;

                fill(0);
        }
        Mat(Mat &&) = default;
        Mat &operator=(Mat &&) = default;
        ~Mat() { }

        float &at(uint8_t i, uint8_t j) {
                if (!(i < rows_ && j < cols_)) {
                        _Error_Handler(__FILE__, __LINE__);
                }
                return matrix_[i][j];
        }

        friend void swap(Mat &first, Mat &second) {
                swap_Element(first.rows_, second.rows_);
                swap_Element(first.cols_, second.cols_);
                for (uint8_t i = 0; i < MAX_MATRIX_ROWS; ++i) {
                        for (uint8_t j = 0; j < MAX_MATRIX_COLS; ++j) {
                                swap_Element(first.matrix_[i][j], second.matrix_[i][j]);
                        }
                }
        }

        Mat(const Mat &m) {
                rows_ = m.rows_;
                cols_ = m.cols_;
                
                for (uint8_t i = 0; i < MAX_MATRIX_ROWS; ++i) {
                        for (uint8_t j = 0; j < MAX_MATRIX_COLS; ++j) {
                                matrix_[i][j] = m.matrix_[i][j];
                        }
                }
        }

        Mat &operator=(const Mat &m) {
                Mat temp(m);
                swap(*this, temp);

                return *this;
        }

        Mat &operator+=(const Mat &rhs) {
                if (rhs.rows_ != rows_ || rhs.cols_ != cols_) {
                        _Error_Handler(__FILE__, __LINE__);
                }

                for (uint8_t i = 0; i < rows_; ++i) {
                        for (uint8_t j = 0; j < cols_; ++j) {
                                matrix_[i][j] += rhs.matrix_[i][j];
                        }
                }

                return *this;
        }

        Mat &operator-=(const Mat &rhs) {
                if (rhs.rows_ != rows_ || rhs.cols_ != cols_) {
                        _Error_Handler(__FILE__, __LINE__);
                }

                for (uint8_t i = 0; i < rows_; ++i) {
                        for (uint8_t j = 0; j < cols_; ++j) {
                                matrix_[i][j] -= rhs.matrix_[i][j];
                        }
                }

                return *this;
        }

        friend Mat operator+(Mat lhs, const Mat &rhs) {
                lhs += rhs;
                return lhs;
        }

        friend Mat operator-(Mat lhs, const Mat &rhs) {
                lhs -= rhs;
                return lhs;
        }

        Mat mult(const Mat &m) {
                if (cols_ != m.rows_) {
                        _Error_Handler(__FILE__, __LINE__);
                }

                Mat product(rows_, m.cols_);
                float sum;
                for (uint8_t i = 0; i < rows_; ++i) {
                        for (uint8_t j = 0; j < m.cols_; ++j) {
                                sum = 0;
                                for (uint8_t k = 0; k < cols_; ++k) {
                                        sum += matrix_[i][k] * m.matrix_[k][j];
                                }
                                product.matrix_[i][j] = sum;
                        }
                }

                return product;
        }

        Mat mult_EW(float num) {
                Mat temp(rows_, cols_);
                for (uint8_t i = 0; i < rows_; ++i) {
                        for (uint8_t j = 0; j < cols_; ++j) {
                                temp.matrix_[i][j] *= num;
                        }
                }

                return temp;
        }

        Mat inverse() {
                if (rows_ != cols_) {
                        _Error_Handler(__FILE__, __LINE__);
                }
                
                Mat inv(rows_, cols_);
                if (rows_ == 1) {
                        if (fabsf(matrix_[0][0]) < 0.000001) {
                                _Error_Handler(__FILE__, __LINE__);
                        }

                        inv.matrix_[0][0] = 1.0 / (float)(matrix_[0][0]);
                }
                else if (rows_ == 2) {
                        float d = matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
                        if (fabsf(d) < 0.000001) {
                                _Error_Handler(__FILE__, __LINE__);
                        }
                        else {
                                inv.matrix_[0][0] = matrix_[1][1] / d;
                                inv.matrix_[0][1] = matrix_[0][1] * (-1.0) / d;
                                inv.matrix_[1][0] = matrix_[1][0] * (-1.0) / d;
                                inv.matrix_[1][1] = matrix_[0][0] / d;
                        }
                }

                return inv;
        }

        Mat transpose() {
                Mat trans(cols_, rows_);
                for (uint8_t i = 0; i < rows_; ++i) {
                        for (uint8_t j = 0; j < cols_; ++j) {
                                trans.matrix_[i][j] = matrix_[j][i];
                        }
                }
                return trans;
        }

        void fill(float num) {
                for (uint8_t i = 0; i < rows_; ++i) {
                        for (uint8_t j = 0; j < cols_; ++j) {
                                matrix_[i][j] = num;
                        }
                }
        }

        static Mat ident(uint8_t n) {
                Mat I(n,n);
                I.fill(0);
                for (uint8_t i = 0; i < n; ++i) {
                        I.matrix_[i][i] = 1;
                }
                return I;
        }

        void print() const {
                for (uint8_t i = 0; i < rows_; ++i) {
                        for (uint8_t j = 0; j < cols_; ++j) {
                                printf("%f\t", matrix_[i][j]);
                        }
                        printf("\n");
                }
        }

private:
        float matrix_[MAX_MATRIX_ROWS][MAX_MATRIX_COLS];
        uint8_t rows_, cols_;
};

#endif // !_MAT_H_
