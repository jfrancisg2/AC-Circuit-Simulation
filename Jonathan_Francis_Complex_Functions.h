// Jonathan Francis Project
// PHYS 30762 Programming in C++
// AC Circuit
// Complex Function Definitions

#pragma once
#include <cmath>
#include <iomanip>
#include "Jonathan_Francis_Project.h"

//                                        //
// COMPLEX MEMBER (AND FRIEND) FUNCTIONS  //
//                                        //

// Return real component
double const complex::get_real() const
{
    return re;
}

// Return imaginary component
double const complex::get_imaginary() const
{
    return im;
}

// Return modulus
double const complex::get_modulus() const
{
    return sqrt(pow(re,2) + pow(im,2));
}

// Return argument
double const complex::get_argument() const
{
    return atan2(im,re);
}

// Return complex conjugate
complex const complex::get_complex_conjugate() const
{
    double conjugate_im = -im;
    complex temp{re,conjugate_im};
    return temp;
}
    
/* Overload + operator for adding complex numbers
Argument: complex number (format a+bi)
*/
complex complex::operator+(const complex &a) const
{
    complex temp{re + a.get_real(), im + a.get_imaginary()};
    return temp;
}

/* Overload - operator for subtracting complex numbers
Argument: complex number (format a+bi)
*/
complex complex::operator-(const complex &a) const
{
    complex temp{re - a.get_real(), im - a.get_imaginary()};
    return temp;
}

/* Overload * operator for multiplying complex numbers
Argument: complex number (format a+bi)
*/
complex complex::operator*(const complex &a) const
{
    complex temp{(re*a.get_real()) - (im*a.get_imaginary()) , (re*a.get_imaginary()) + (im*a.get_real())};
    return temp;
}

/* Overload / operator for dividing complex numbers
Argument: complex number (format a+bi)
*/
complex complex::operator/(const complex &a) const
{
    if (a.get_imaginary() == 0 && a.get_real() == 0) {
        std::cout << "Error: division by zero" << std::endl;
        throw;
    }
    complex temp{((re*a.get_real()) + (im*a.get_imaginary()))/((pow(a.get_real(),2)) + pow(a.get_imaginary(),2)) , ((im*a.get_real()) - (re*a.get_imaginary()))/((pow(a.get_real(),2)) + pow(a.get_imaginary(),2))};
    return temp;
}

/* Overload << operator for complex numbers
 Arguments: ostream, complex number (format a+bi)
*/
std::ostream & operator<<(std::ostream &os, const complex &z)
{
    if (z.get_imaginary() < 0) {
        os << std::setprecision(5) << z.get_real() << " - " << fabs(z.get_imaginary()) << "i";
    
    }

    else {
        os << std::setprecision(5) << z.get_real() << " + " << z.get_imaginary() << "i";
    }

    return os;
}