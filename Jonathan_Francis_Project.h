// Jonathan Francis Project
// PHYS 30762 Programming in C++
// AC Circuit
// Classes File

#ifndef PROJECT_HEADER_H
#define PROJECT_HEADER_H

#include <iostream>
#include <set>
#include <vector>
#include <cmath>
#include <string>
#include <memory>

// Define the complex number class
class complex
{
// Friend functions
friend std::ostream & operator<<(std::ostream &os, const complex &z); // Overload the << operator

private:
    double re,im;
public:
    // Constructors & destructor
    complex() {
        re = im = 0;
    }   

    complex(double const real_part, double const im_part): re{real_part}, im{im_part} {
    }

    ~complex() {
    }

    // Member functions
    double const get_real() const;
    double const get_imaginary() const;
    double const get_modulus() const;
    double const get_argument() const;
    complex const get_complex_conjugate() const;
    complex operator+(const complex &a) const;
    complex operator-(const complex &a) const;
    complex operator*(const complex &a) const;
    complex operator/(const complex &a) const;
};


// Define the component class
class component
{
protected:
    complex impedance;
public:
    static inline double frequency = 0;

    // Constructors and destructor
    component() {
    }

    component(complex impedance_temp): impedance{impedance_temp} {
    }

    virtual ~component() {
    }

    // Member Functions
    virtual complex const get_impedance() const = 0;
    virtual double const get_impedance_magnitude() const = 0;
    virtual void const set_impedance() = 0;
    virtual std::string const get_name() const = 0;
    virtual double const get_phase_difference() const = 0;
};

class resistor: public component
{
protected:
    double resistance;
    std::string resistor_name;
public:
    // Constructors and destructor
    resistor(): component{}, resistor_name{"Default Resistor"} {
    }

    resistor(double resistance_temp, std::string const resistor_name_temp): resistance{resistance_temp}, resistor_name{resistor_name_temp} {
        complex impedance_temp{resistance,0};
        impedance = impedance_temp;
    }

    ~resistor() {
    }

    // Member functions
    complex const get_impedance() const;
    double const get_impedance_magnitude() const;
    void const set_impedance();
    double const get_phase_difference() const;
    std::string const get_name() const;
};

class non_ideal_resistor: public resistor
{
private:
    double parasitic_capacitance;
    double lead_inductance;
public:
    // Constructors and destructor
    non_ideal_resistor(): resistor{} {
    }

    non_ideal_resistor(double parasitic_capacitance_temp, double lead_inductance_temp, double resistance_temp, std::string resistor_name_temp): parasitic_capacitance{parasitic_capacitance_temp}, lead_inductance{lead_inductance_temp}, resistor{resistance_temp, resistor_name_temp} {
        double temp_real = resistance * (pow(2*M_PI*frequency,2) * lead_inductance * parasitic_capacitance * (resistance - 1) + 1) / (1 + pow(2*M_PI*frequency,2) * pow(resistance,2) * pow(parasitic_capacitance,2));
        double temp_imaginary = 2*M_PI*frequency * (lead_inductance - pow(resistance,2) * parasitic_capacitance + pow(2*M_PI*frequency,2) * lead_inductance * pow(parasitic_capacitance,2) * pow(resistance,2)) / (1 + pow(2*M_PI*frequency,2) * pow(resistance,2) * pow(parasitic_capacitance,2)); 
        complex impedance_temp{temp_real,temp_imaginary};
        impedance = impedance_temp;
    }

    ~non_ideal_resistor() {
    }

    // Member functions
    complex const get_impedance() const;
    double const get_impedance_magnitude() const;
    void const set_impedance();
    double const get_phase_difference() const;
    std::string const get_name() const;
};

class capacitor: public component
{
protected:
    double capacitance;
    std::string capacitor_name;
public:
    // Constructors and destructor
    capacitor(): component{}, capacitor_name{"Default Capacitor"} {
    }

    capacitor(double capacitance_temp, std::string const capacitor_name_temp): capacitance{capacitance_temp}, capacitor_name{capacitor_name_temp} {
        complex impedance_temp{0,-1/(2*M_PI*capacitance*frequency)};
        impedance = impedance_temp;
    }

    ~capacitor() {
    }

    // Member functions
    complex const get_impedance() const;
    double const get_impedance_magnitude() const;
    void const set_impedance();
    double const get_phase_difference() const;
    std::string const get_name() const;
};

class non_ideal_capacitor: public capacitor
{
private:
    double lead_inductance;
    double plate_resistance;
public:
    // Constructors and destructor
    non_ideal_capacitor(): capacitor{} {
    }

    non_ideal_capacitor(double plate_resistance_temp, double lead_inductance_temp, double capacitance_temp, std::string capacitor_name_temp): plate_resistance{plate_resistance_temp}, lead_inductance{lead_inductance_temp}, capacitor{capacitance_temp, capacitor_name_temp} {
        complex impedance_temp{plate_resistance,(2*M_PI*frequency * lead_inductance) - 1/(2*M_PI*frequency * capacitance)};
        impedance = impedance_temp;
    }

    ~non_ideal_capacitor() {
    }

    // Member functions
    complex const get_impedance() const;
    double const get_impedance_magnitude() const;
    void const set_impedance();
    double const get_phase_difference() const;
    std::string const get_name() const;
};

class inductor: public component
{
protected:
    double inductance;
    std::string inductor_name;
public:
    // Constructors and destructor
    inductor(): component{}, inductor_name{"Default Inductor"} {
    }

    inductor(double inductance_temp, std::string const inductor_name_temp): inductance{inductance_temp}, inductor_name{inductor_name_temp} {
        complex impedance_temp{0,2*M_PI*inductance*frequency};
        impedance = impedance_temp;
    }

    ~inductor() {
    }

    // Member functions
    complex const get_impedance() const;
    double const get_impedance_magnitude() const;
    void const set_impedance();
    double const get_phase_difference() const;
    std::string const get_name() const;
};

class non_ideal_inductor: public inductor
{
private:
    double parasitic_resistance;
    double parasitic_capacitance;
public:
    // Constructors and destructor
    non_ideal_inductor(): inductor{} {
    }

    non_ideal_inductor(double parasitic_resistance_temp, double parasitic_capacitance_temp, double inductance_temp, std::string inductor_name_temp): parasitic_resistance{parasitic_resistance_temp}, parasitic_capacitance{parasitic_capacitance_temp}, inductor{inductance_temp, inductor_name_temp} {
        double temp_real = parasitic_resistance / (pow(1 - pow(2*M_PI*frequency,2) * inductance * parasitic_capacitance,2) + pow(2*M_PI*frequency,2) * pow(parasitic_capacitance,2) * pow(parasitic_resistance,2));
        double temp_imaginary = 2*M_PI*frequency * (inductance - pow(2*M_PI*frequency,2) * pow(inductance,2) * parasitic_capacitance - parasitic_capacitance * pow(parasitic_resistance,2)) / (pow(1 - pow(2*M_PI*frequency,2) * inductance * parasitic_capacitance,2) + pow(2*M_PI*frequency,2) * pow(parasitic_capacitance,2) * pow(parasitic_resistance,2));
        complex impedance_temp{temp_real,temp_imaginary};
        impedance = impedance_temp;
    }

    ~non_ideal_inductor() {
    }

    // Member functions
    complex const get_impedance() const;
    double const get_impedance_magnitude() const;
    void const set_impedance();
    double const get_phase_difference() const;
    std::string const get_name() const;
};

// Data structure to store a circuit connection
struct link {
	std::shared_ptr<component> start;
    std::shared_ptr<component> end;
};

// A class to represent a circuit object
class circuit
{
private:
    // A vector of vectors to represent an adjacency list
    std::vector<std::vector<std::shared_ptr<component>>> adjacency_list;
    // A set to store all components in the circuit
    std::set<std::shared_ptr<component>> circuit_components;
public:
	// Parametrized constructor and destructor
	circuit(std::vector<link> const &links, int N) {

		// resize the vector to hold `N` elements of type `vector<std::shared_ptr<component>>`
        adjacency_list.resize(N);

        int counter = 0;

		// Add links to the circuit
		for (auto &link: links) {
            if (counter == N) {
                break;
            }

            // Insert at the end
			adjacency_list[counter].emplace_back(link.start);
            adjacency_list[counter].emplace_back(link.end);
            circuit_components.insert(link.start);
            circuit_components.insert(link.end);
            counter++;
		}
	}

    ~circuit() {
    }

    // Member Functions
    std::vector<std::vector<std::shared_ptr<component>>> const get_adjacency_list() const;
    std::set<std::shared_ptr<component>> const get_circuit_components() const;
    double const get_total_phase_shift(complex total_impedance) const;
    complex const get_total_impedance(std::vector<std::vector<std::shared_ptr<component>>> circuit_segments, std::vector<std::shared_ptr<component>> parallel_starter, std::vector<std::shared_ptr<component>> parallel_ender) const;
    complex const get_impedance_of_segment(std::vector<std::shared_ptr<component>> segment, std::vector<std::shared_ptr<component>> parallel_starter, std::vector<std::shared_ptr<component>> parallel_ender) const;
    std::vector<std::vector<std::shared_ptr<component>>> const get_parallel_segments(std::vector<std::vector<std::shared_ptr<component>>> circuit_segments,std::vector<std::shared_ptr<component>> parallel_starter, std::vector<std::shared_ptr<component>> parallel_ender) const;
    std::vector<std::vector<std::shared_ptr<component>>> const get_circuit_segments(std::shared_ptr<component> comp) const;
    std::vector<std::shared_ptr<component>> const get_parallel_starters(std::vector<std::vector<std::shared_ptr<component>>> circuit_segments, int N) const;
    std::vector<std::shared_ptr<component>> const get_parallel_enders(std::vector<std::vector<std::shared_ptr<component>>> circuit_segments, int N) const;
    std::vector<std::shared_ptr<component>> const find_paths(std::shared_ptr<component> comp) const;
};

#endif