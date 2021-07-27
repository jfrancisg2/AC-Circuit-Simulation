// Jonathan Francis Project
// PHYS 30762 Programming in C++
// AC Circuit
// Component Function Definitions

#pragma once
#include <cmath>
#include "Jonathan_Francis_Project.h"

//                           //
// RESISTOR MEMBER FUNCTIONS //
//                           //

// Return impedance
complex const resistor::get_impedance() const
{
    return impedance;
}

// Return impedance magnitude
double const resistor::get_impedance_magnitude() const
{
    return impedance.get_modulus();
}

// Set impedance
void const resistor::set_impedance()
{
    complex impedance_temp{resistance,0};
    impedance = impedance_temp;
}

// Return the phase difference between two components
double const resistor::get_phase_difference() const
{
    return impedance.get_argument();
}

// Get name
std::string const resistor::get_name() const
{
    return resistor_name;
}

//                                     //
// NON IDEAL RESISTOR MEMBER FUNCTIONS //
//                                     //

// Return impedance
complex const non_ideal_resistor::get_impedance() const
{
    return impedance;
}

// Return impedance magnitude
double const non_ideal_resistor::get_impedance_magnitude() const
{
    return impedance.get_modulus();
}

// Set impedance
void const non_ideal_resistor::set_impedance()
{
    double temp_real = resistance * (pow(2*M_PI*frequency,2) * lead_inductance * parasitic_capacitance * (resistance - 1) + 1) / (1 + pow(2*M_PI*frequency,2) * pow(resistance,2) * pow(parasitic_capacitance,2));
    double temp_imaginary = 2*M_PI*frequency * (lead_inductance - pow(resistance,2) * parasitic_capacitance + pow(2*M_PI*frequency,2) * lead_inductance * pow(parasitic_capacitance,2) * pow(resistance,2)) / (1 + pow(2*M_PI*frequency,2) * pow(resistance,2) * pow(parasitic_capacitance,2)); 
    complex impedance_temp{temp_real,temp_imaginary};
    impedance = impedance_temp;
}

// Return the phase difference between two components
double const non_ideal_resistor::get_phase_difference() const
{
    return impedance.get_argument();
}

// Get name
std::string const non_ideal_resistor::get_name() const
{
    return resistor_name;
}


//                            //
// CAPACITOR MEMBER FUNCTIONS //
//                            //

// Return impedance
complex const capacitor::get_impedance() const
{
    return impedance;
}

// Return impedance magnitude
double const capacitor::get_impedance_magnitude() const
{
    return impedance.get_modulus();
}

// Set impedance
void const capacitor::set_impedance()
{
    complex impedance_temp{0,-1/(2*M_PI*capacitance*frequency)};
    impedance = impedance_temp;
}

// Return the phase difference between two components
double const capacitor::get_phase_difference() const
{
    return impedance.get_argument();
}

// Get name
std::string const capacitor::get_name() const
{
    return capacitor_name;
}

//                                      //
// NON IDEAL CAPACITOR MEMBER FUNCTIONS //
//                                      //

// Return impedance
complex const non_ideal_capacitor::get_impedance() const
{
    return impedance;
}

// Return impedance magnitude
double const non_ideal_capacitor::get_impedance_magnitude() const
{
    return impedance.get_modulus();
}

// Set impedance
void const non_ideal_capacitor::set_impedance()
{
    complex impedance_temp{plate_resistance,(2*M_PI*frequency * lead_inductance) - 1/(2*M_PI*frequency * capacitance)};
    impedance = impedance_temp;
}

// Return the phase difference between two components
double const non_ideal_capacitor::get_phase_difference() const
{
    return impedance.get_argument();
}

// Get name
std::string const non_ideal_capacitor::get_name() const
{
    return capacitor_name;
}

//                           //
// INDUCTOR MEMBER FUNCTIONS //
//                           //

// Return impedance
complex const inductor::get_impedance() const
{
    return impedance;
}

// Return impedance magnitude
double const inductor::get_impedance_magnitude() const
{
    return impedance.get_modulus();
}

// Set impedance
void const inductor::set_impedance()
{
    complex impedance_temp{0,2*M_PI*inductance*frequency};
    impedance = impedance_temp;
}

// Return the phase difference between two components
double const inductor::get_phase_difference() const
{
    return impedance.get_argument();
}

// Get name
std::string const inductor::get_name() const
{
    return inductor_name;
}

//                           //
// NON IDEAL INDUCTOR MEMBER FUNCTIONS //
//                           //

// Return impedance
complex const non_ideal_inductor::get_impedance() const
{
    return impedance;
}

// Return impedance magnitude
double const non_ideal_inductor::get_impedance_magnitude() const
{
    return impedance.get_modulus();
}

void const non_ideal_inductor::set_impedance()
{
    double temp_real = parasitic_resistance / (pow(1 - pow(2*M_PI*frequency,2) * inductance * parasitic_capacitance,2) + pow(2*M_PI*frequency,2) * pow(parasitic_capacitance,2) * pow(parasitic_resistance,2));
    double temp_imaginary = 2*M_PI*frequency * (inductance - pow(2*M_PI*frequency,2) * pow(inductance,2) * parasitic_capacitance - parasitic_capacitance * pow(parasitic_resistance,2)) / (pow(1 - pow(2*M_PI*frequency,2) * inductance * parasitic_capacitance,2) + pow(2*M_PI*frequency,2) * pow(parasitic_capacitance,2) * pow(parasitic_resistance,2));
    complex impedance_temp{temp_real,temp_imaginary};
    impedance = impedance_temp;
}

// Return the phase difference between two components
double const non_ideal_inductor::get_phase_difference() const
{
    return impedance.get_argument();
}

// Get name
std::string const non_ideal_inductor::get_name() const
{
    return inductor_name;
}