##Multivariate Splines
Multivariate Splines is a function approximation library implementing various multivariate splines in C++. It contains the following implementations:

1. a speedy implementation of the tensor product [B-spline](http://en.wikipedia.org/wiki/B-spline), and 
2. a simple implementation of [radial basis function splines](http://en.wikipedia.org/wiki/Radial_basis_function), including the [thin plate spline](http://en.wikipedia.org/wiki/Thin_plate_spline).

The B-spline may approximate any multivariate function sampled on a grid. The user may construct a linear (degree 1) or cubic (degree 3) spline that interpolates the data. The B-spline is constructed from the samples by solving a linear system. A modern desktop computer limits the number of samples to about 100 000 when constructing a B-spline - evaluation of the spline is very fast due to the local support property of B-splines. 

The user may create a penalized B-spline (P-spline) that smooths the data instead of interpolating it. The construction of a P-spline is more computationally demanding than the B-spline - a large least-square problem must be solved - bringing the limit on the number of samples down to about 10 000.

When sampling is expensive and/or scattered (not on a grid) the radial basis function splines may be utilized for function approximation. The user should expect a high computational cost for constructing and evaluating a radial basis function spline, even with a modest number of samples (up to about 1 000 samples). 

The library is based on the C++ template linear algebra library [Eigen](http://eigen.tuxfamily.org); its sparse matrix support is particularly important for the speed of the tensor product B-spline implementation.

###Author's note:
Multivariate Splines implements various splines for function approximation with the purpose of utilizing the splines/approximations in mathematical programming (nonlinear optimization). Thus, special attention has been given to functionality that may support a nonlinear optimization solver. For example, the B-spline implementation includes evaluation of the Jacobian and Hessian.

NOTE: focus has not been on curve fitting, NURBS, etc.

NOTE: general implementation which is readily extended with new functionality. The author welcomes new contributions and improvements to the code. 

NOTE: the goal is to create an open, general, and fast library for multivariate splines.

###Requirements for use: 
* [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)


##Compiling the library

###Compile on UNIX
####Requirements
* [CMake](http://www.cmake.org/)
* [Git](http://git-scm.com/)
* [GCC](https://gcc.gnu.org/) or an equivalent C++11 compiler

1. Download and install [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)
2. `sudo apt-get update && sudo apt-get install git cmake build-essential`
3. `git clone https://github.com/bgrimstad/multivariate-splines.git`
4. `cd multivariate-splines`
5. `mkdir build && cd build`
6. `cmake ..`
7. `make`
8. `make install`

####Troubleshooting
`fatal error: Eigen/Dense: No such file or directory`: The compiler could not find Eigen. You either need to install Eigen, and then run step #6 again (with `-DEIGEN_DIRECTORY="/path/to/eigen"` if Eigen did not install to the default directory (/usr/local/include/eigen3)).

`make: *** [install] Error 1`: You probably need elevated rights to install the library because you are trying to write to a directory you don't have permission to write to. Either change the install paths via the options, or run step #8 again like this: `sudo make install`.

Remember to add the Eigen directory to your include path.


###Compile on Windows

1. Clone https://github.com/bgrimstad/multivariate-bsplines
2. Download Eigen: http://eigen.tuxfamily.org/index.php?title=Main_Page
  1. Extract the zip-file into a new folder, and write down the location of that folder
3. Download and install CMake: http://www.cmake.org/
4. Download and install Qt Creator: http://qt-project.org/downloads
  1. Make sure that MinGW is marked for installation
5. Run Qt Creator, select `Open project`
  1. Navigate into the multivariate-bsplines folder, select `CMakeLists.txt`
  2. In the arguments field, write: `-DEIGEN_DIRECTORY=C:/path/to/eigen/from/step/2.1`
  3. Run CMake
6. Now you can build the library with Qt Creator, and the library files will be output to your build directory.

* You may have to add -static as a flag to your linker if you are compiling with MinGW.
* C++11 must be enabled.
* If you get asked to specify where CMake is located, then you can typically find the file cmake.exe in C:\Program Files (x86)\CMake\bin.


###Options:
These options go along with UNIX step #6 or Windows step #5.2, and are used like this:

*     -DEIGEN_DIRECTORY=/home/me/eigen

*     -DEIGEN_DIRECTORY=/path/to/eigen -DHEADER_DIRECTORY=/home/me/c++/multivariate-bsplines/includes

The syntax is: `-D<VARIABLE_NAME>=<VARIABLE_VALUE>`. If you have any spaces in your value you must surround it with double quotes (").

| Variable name     | Default value                 | Description                               |
| ----------------- | ----------------------------- | ----------------------------------------- |
| EIGEN_DIRECTORY   | /usr/local/include/eigen3     | Path to the Eigen lib.                    |
| HEADER_DIRECTORY  | include                       | Where the headers should be installed.    |
| LIBRARY_DIRECTORY | lib                           | Where to install the library file.        |

Note for the header and library paths:
If the path is relative (the first character is not / on UNIX or C:/ (or equivalent) on Windows), then the actual path used will be relative to [CMAKE_INSTALL_PREFIX](http://www.cmake.org/cmake/help/v2.8.12/cmake.html#variable:CMAKE_INSTALL_PREFIX).

###Usage
This is a simple example demonstrating the use of Multivariate Splines. Note that there is no restrictions to the dimension of x (except that it has to be >= 1, of course).

Remember to compile with a c++11 compatible compiler! That means you probably have to add a flag when compiling.

```c++
#include <iostream>
#include "datatable.h"
#include "bspline.h"
#include "pspline.h"
#include "rbfspline.h"

using std::cout;
using std::endl;

using namespace MultivariateSplines;

// Six-hump camelback function
double f(DenseVector x)
{
    assert(x.rows() == 2);
    return (4 - 2.1*x(0)*x(0) + (1/3.)*x(0)*x(0)*x(0)*x(0))*x(0)*x(0) + x(0)*x(1) + (-4 + 4*x(1)*x(1))*x(1)*x(1);
}

int main(int argc, char *argv[])
{
    // Create new DataTable to manage samples
    DataTable samples;

    // Sample function
    DenseVector x(2);
    double y;
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            // Sample function at x
            x(0) = i*0.1;
            x(1) = j*0.1;
            y = f(x);

            // Store sample
            samples.addSample(x,y);
        }
    }

    // Build B-splines that interpolate the samples
    BSpline bspline1(samples, BSplineType::LINEAR);
    BSpline bspline3(samples, BSplineType::CUBIC_FREE);

    // Build penalized B-spline (P-spline) that smooths the samples
    PSpline pspline(samples, 0.03);

    // Build radial basis function spline that interpolate the samples
    RBFSpline rbfspline(samples, RadialBasisFunctionType::THIN_PLATE_SPLINE);

    // Evaluate the splines at x = (1,1)
    x(0) = 1; x(1) = 1;
    cout << "-------------------------------------------------"     << endl;
    cout << "Function at x: \t\t\t"         << f(x)                 << endl;
    cout << "Linear B-spline at x: \t\t"    << bspline1.eval(x)     << endl;
    cout << "Cubic B-spline at x: \t\t"     << bspline3.eval(x)     << endl;
    cout << "P-spline at x: \t\t\t"         << pspline.eval(x)      << endl;
    cout << "Thin-plate spline at x:\t\t"   << rbfspline.eval(x)    << endl;
    cout << "-------------------------------------------------"     << endl;

    return 0;
}
```

###Sampling with DataTable
Function samples are managed by and stored in the DataTable data structure. The following code snippet shows how DataTable is used to manage samples. 
```c++
// Create new data structure
DataTable samples; 

// Add some samples (x,y), where y = f(x)
samples.addSample(1,0);
samples.addSample(2,5);
samples.addSample(3,10);
samples.addSample(4,15);

// The order in which the samples are added do not matter since DataTable sorts the samples internally.
```
The grid, meaning all the values of x where you have sampled the function, must be complete. This means that if you have sampled the function in `x = [0 0]`, `x = [1 0]` and `x = [2 1]`, you must also sample the function in `x = [1 1]`, `x = [0 1]` and `x = [2 0]`. You must have sampled the function in all permutations of x within the possible values of x<sub>0</sub>, x<sub>1</sub> ... x<sub>n</sub>. The number of samples will then (disregarding duplicates) be num(x<sub>0</sub>) * num(x<sub>1</sub>) * ... * num(x<sub>n</sub>) where num(x) is the number of distinct values of x the the function has been sampled in. You can check if the grid is complete by calling `isGridComplete()` on your SortedDataTable.


This is an **incomplete** grid:

| x<sub>0</sub>   | x<sub>1</sub>   | y   |
| --------------- | --------------- | --- |
| 2.1             | 1               | -7  |
| 2.3             | 3               | 10  |
| 2.1             | 3               | 9.3 |


This is a **complete** grid:

| x<sub>0</sub>   | x<sub>1</sub>   | y   |
| --------------- | --------------- | --- |
| 2.1             | 1               | -7  |
| 2.3             | 3               | 10  |
| 2.1             | 3               | 9.3 |
| 2.3             | 1               | 0   |

Please note that whether the grid is complete or not only depends on the values of x, not those of y.
