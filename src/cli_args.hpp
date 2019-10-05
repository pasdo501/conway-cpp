#pragma once

namespace cli_args
{
    class Parser
    {
        public:
            Parser() { };
            ~Parser() { };
            int parse_args(int argc, char *argv[]);
            int get_prob_alive() { return prob_alive; };
            int get_height() { return height; };
            int get_width() { return width; };
            int get_generations() { return generations; };
        private:
            enum Errors
            {
                invalid_width = 1,
                invalid_height = 2,
                invalid_prob = 4,
                invalid_gen = 8
            };
            int width;
            int height;
            int generations;
            int prob_alive;
            bool valid_prob(float prob) { return prob >= 0 && prob <= 1.f; };
            bool valid_int(int num) { return num > 0; };
            void display_errors(int errors);
            std::string get_optstring_by_char(char c);
    };
}