/*
 * This file is part of the Multivariate Splines library.
 * Copyright (C) 2012 Bjarne Grimstad (bjarne.grimstad@gmail.com)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/


#ifndef MS_GENERALDEFINITIONS_H
#define MS_GENERALDEFINITIONS_H

#ifndef NDEBUG
    #include <iostream>
#endif // NDEBUG

#include <exception>
#include <stdexcept>

#include <vector>
#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <iomanip>
#include <limits>

#define MS_SAVE_DOUBLE_PRECISION 17

namespace MultivariateSplines
{

// Eigen vectors
typedef Eigen::VectorXd DenseVector;
typedef Eigen::SparseVector<double> SparseVector;

// Eigen matrices
typedef Eigen::MatrixXd DenseMatrix;
typedef Eigen::SparseMatrix<double> SparseMatrix; // declares a column-major sparse matrix type of double

class Exception : public std::exception
{
private:
    std::string __what;

public:

    Exception(const std::string& what)
        : __what(what)
    {
    }

    const char* what() const throw()
    {
        return this->__what.c_str();
    }
};

//Simple definition of checked strto* functions according to the implementations of sto* C++11 functions at:
//  http://msdn.microsoft.com/en-us/library/ee404775.aspx
//  http://msdn.microsoft.com/en-us/library/ee404860.aspx
//  https://gcc.gnu.org/svn/gcc/trunk/libstdc++-v3/include/bits/basic_string.h
//  https://gcc.gnu.org/svn/gcc/trunk/libstdc++-v3/include/ext/string_conversions.h

double checked_strtod(const char* _Str, char** _Eptr);

int checked_strtol(const char* _Str, char** _Eptr, size_t _Base = 10);

} // namespace MultivariateSplines

#endif // MS_GENERALDEFINITIONS_H
