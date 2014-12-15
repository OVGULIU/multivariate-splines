/*
 * This file is part of the Multivariate Splines library.
 * Copyright (C) 2012 Bjarne Grimstad (bjarne.grimstad@gmail.com)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <generaldefinitions.h>

namespace MultivariateSplines
{

//Simple definition of checked strto* functions according to the implementations of sto* C++11 functions at:
//  http://msdn.microsoft.com/en-us/library/ee404775.aspx
//  http://msdn.microsoft.com/en-us/library/ee404860.aspx
//  https://gcc.gnu.org/svn/gcc/trunk/libstdc++-v3/include/bits/basic_string.h
//  https://gcc.gnu.org/svn/gcc/trunk/libstdc++-v3/include/ext/string_conversions.h

double checked_strtod(const char* _Str, char** _Eptr) {

    double _Ret;
    char* _EptrTmp;

    errno = 0;

    _Ret = std::strtod(_Str, &_EptrTmp);

    if (_EptrTmp == _Str)
    {
        throw std::invalid_argument("strtod");
    }
    else if (errno == ERANGE)
    {
        throw std::out_of_range("strtod");
    }
    else
    {
        if(_Eptr != nullptr)
        {
            *_Eptr = _EptrTmp;
        }

        return _Ret;
    }
}

int checked_strtol(const char* _Str, char** _Eptr, size_t _Base) {

    long _Ret;
    char* _EptrTmp;

    errno = 0;

    _Ret = std::strtol(_Str, &_EptrTmp, _Base);

    if (_EptrTmp == _Str)
    {
        throw std::invalid_argument("strtol");
    }
    else if (errno == ERANGE ||
            (_Ret < std::numeric_limits<int>::min() || _Ret > std::numeric_limits<int>::max()))
    {
        throw std::out_of_range("strtol");
    }
    else
    {
        if (_Eptr != nullptr)
        {
            *_Eptr = _EptrTmp;
        }

        return _Ret;
    }
}

} // namespace MultivariateSplines
