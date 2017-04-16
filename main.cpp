#include <iostream>
#include "Parameters.h"

void experiment(Parameters p);

/**
 * main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char* argv[]) {

    Parameters p;
    p.set_dimension(80);
    p.set_density(1.0);
    p.set_first(true);
    p.set_second_linear(true);
    p.set_second_extreme(true, "/home/edaravig/fplll-v5/share/fplll/strategies/default.json");
    p.set_second_step(1);
    p.set_second_start(30);
    p.set_second_end(62);
    p.set_number_of_instances(3);
    p.set_log_filename("logfile.txt");
    experiment(p);

    return EXIT_SUCCESS;
}
