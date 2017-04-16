//
// Created by edaravig on 16/4/2017.
//

#include "Parameters.h"

void Parameters::set_dimension(int dimension) {
    this->dimension = dimension;
}

void Parameters::set_density(double density) {
    this->density = density;
}

void Parameters::set_first(bool first) {
    this->first = first;
}

void Parameters::set_second_linear(bool second_linear) {
    this->second_linear = second_linear;
}

void Parameters::set_second_extreme(bool second_extreme, const char* _strategies_filename) {
    this->second_extreme = second_extreme;
    this->strategies_filename = strategies_filename;
}

void Parameters::set_second_step(int second_step) {
    this->second_step = second_step;
}

void Parameters::set_second_start(int second_start) {
    this->second_start = second_start;
}

void Parameters::set_second_end(int second_end) {
    this->second_end = second_end;
}

void Parameters::set_number_of_instances(int number_of_instances) {
    this->number_of_instances = number_of_instances;
}

void Parameters::set_log_filename(const char* logs_filename) {
    this->logs_filename =logs_filename;
}

int Parameters::get_dimension() {
    return dimension;
}

double Parameters::get_density() {
    return density;
}

bool Parameters::get_first() {
    return first;
}

bool Parameters::get_second_linear() {
    return second_linear;
}

bool Parameters::get_second_extreme() {
    return second_extreme;
}

int Parameters::get_second_step() {
    return second_step;
}

int Parameters::get_second_start() {
    return second_start;
}

int Parameters::get_second_end() {
    return second_end;
}

int Parameters::get_number_of_instances() {
    return number_of_instances;
}

const char* Parameters::get_logs_filename() {
    return logs_filename;
}

const char* Parameters::get_strategies_filename() {
    return strategies_filename;
}