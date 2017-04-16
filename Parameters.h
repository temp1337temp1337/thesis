//
// Created by edaravig on 16/4/2017.
//

#ifndef THESIS_GITHUB_PARAMETERS_H
#define THESIS_GITHUB_PARAMETERS_H


class Parameters {

private:

    int dimension;
    double density;
    bool first;
    bool second_linear;
    bool second_extreme;
    const char* strategies_filename;
    int second_step;
    int second_start;
    int second_end;
    int number_of_instances;
    const char* logs_filename;

public:

    void set_dimension(int dimension);
    void set_density(double density);
    void set_first(bool first);
    void set_second_linear(bool _second_linear);
    void set_second_extreme(bool _second_extreme, const char* _strategies_filename);
    void set_second_step(int _second_step);
    void set_second_start(int _second_start);
    void set_second_end(int _second_end);
    void set_number_of_instances(int _number_of_instances);
    void set_log_filename(const char* _logs_filename);

    int get_dimension();
    double get_density();
    bool get_first();
    bool get_second_linear();
    bool get_second_extreme();
    int get_second_step();
    int get_second_start();
    int get_second_end();
    int get_number_of_instances();
    const char* get_strategies_filename();
    const char* get_logs_filename();
};


#endif //THESIS_GITHUB_PARAMETERS_H
