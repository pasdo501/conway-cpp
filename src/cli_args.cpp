#include <iostream>
#include <getopt.h>
#include "cli_args.hpp"

using namespace std;
using namespace cli_args;


int Parser::parse_args(int argc, char *argv[])
{
    int opt;
    int option_index = 0;
    int width = -1;
    int height = -1;
    float prob_alive = -1.f;
    int max_generations = -1;

    int errors = 0x00;

    static struct option long_options[] = {
        { "width", required_argument, 0, 'w' },
        { "height", required_argument, 0, 'h' },
        { "probability", required_argument, 0, 'p' },
        { "generations", required_argument, 0, 'g' },
        { "help", no_argument, 0, 'i' }
    };

    while ((opt = getopt_long(argc, argv, ":w:h:p:g:i", long_options, &option_index)) != -1) {
        switch (opt)
        {
        case 'w':
            try {
                width = stoi(optarg);
            } catch(invalid_argument&) {
                cerr << "Invalid argument for width: '" << optarg << "'" << endl;
                return EXIT_FAILURE;
            }
            break;
        case 'h':
            try {
                height = stoi(optarg);
            } catch (invalid_argument&) {
                cerr << "Invalid argument for height: '" << optarg << "'" << endl;
                return EXIT_FAILURE;
            }
            break;
        case 'p':
            try {
                prob_alive = stof(optarg);
            } catch (invalid_argument&) {
                cerr << "Invalid argument for probability: '" << optarg << "'" << endl;
                return EXIT_FAILURE;
            }
            break;
        case 'g':
            try {
                max_generations = stoi(optarg);
            } catch (invalid_argument&) {
                cerr << "Invalid argument for generations: '" << optarg << "'" << endl;
                return EXIT_FAILURE;
            }
            break;
        case 'i':
            cout << "Usage: bin/conway [options]" << endl;
            cout << "Options" << endl;
            cout << "-----------------------------------------------------------" << endl;
            cout << "-w / --width <width>                  The width of the grid" << endl;
            cout << "-h / --height <height>                The height of the grid" << endl;
            cout << "-p / --probability <probability>      The probability of each cell being alive at the start of the simulation" << endl;
            cout << "-g / --generation <generations>       The number of generations the simulation will run for" << endl;
            cout << "-i / --help                           Display this help text again" << endl;
            cout << "width, height, and generations all require positive integers as input" << endl;
            cout << "probability takes a positive float between 0 and 1.0 inclusive" << endl;
            cout << "\nYou can enter as many or as few options as you wish; remaining options will be requested by the command line." << endl;
            return EXIT_FAILURE;
        case ':':
            cout << "Error: Option '" << (get_optstring_by_char(static_cast<char>(optopt))) << "' needs a value" << endl;
            errors += 1;
            break;
        case '?':
            cout << "Unknown option: " << static_cast<char>(optopt) << endl;
            break;
        }
    }

    if (errors > 0) return EXIT_FAILURE;

    if (width == -1) {
        cout << "Enter width of grid: ";
        cin >> width;
    }
    if (!valid_int(width)) errors |= Errors::invalid_width;

    if (height == -1) {
        cout << "Enter height of grid: ";
        cin >> height;
    }
    if (!valid_int(height)) errors |= Errors::invalid_height;

    if (max_generations == -1) {
        cout << "Enter the number of generations: ";
        cin >> max_generations;
    }
    if (!valid_int(max_generations)) errors |= Errors::invalid_gen;

    if (prob_alive < 0.f) {
        cout << "Enter chance of being alive (number between 0 & 1): " << endl;
        cin >> prob_alive;
    }
    if (!valid_prob(prob_alive)) errors |= Errors::invalid_prob;

    if (errors > 0) {
        display_errors(errors);
        return EXIT_FAILURE;
    } else {
        this->height = height;
        this->width = width;
        this->generations = max_generations;
        this->prob_alive = static_cast<int>(prob_alive * 10);
        return EXIT_SUCCESS;
    }
}

void Parser::display_errors(int errors)
{
    if (errors & Errors::invalid_gen)
        cout << "Invalid number of generations (needs to be greater than 0)" << endl;
    
    if (errors & Errors::invalid_height)
        cout << "Invalid height entered (needs to be greater than 0)" << endl;

    if (errors & Errors::invalid_prob)
        cout << "Invalid probability entered (needs to be between 0 & 1 inclusive)" << endl;
    
    if (errors & Errors::invalid_width)
        cout << "Invalid width entered (needs to be greater than 0)" << endl;
}

string Parser::get_optstring_by_char(char c)
{
    switch (c)
    {
    case 'w':
        return "-w / --width";
    case 'h':
        return "-h / --height";
    case 'p':
        return "-p / --probability";
    case 'g':
        return "-g / --generations";
    default:
        return "";
    }
}