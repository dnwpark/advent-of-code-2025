#include <cstdint>
#include <fstream>
#include <print>
#include <ranges>
#include <sstream>
#include <vector>

using Number = std::uint64_t;

void
part_1()
{
    std::ifstream file("day_6/input.txt");

    using CalcResults = std::vector<Number>;
    std::vector<CalcResults> aai_results;

    std::string z_line;
    while (std::getline(file, z_line))
    {
        if (z_line[0] == '+' || z_line[0] == '*')
            break;

        std::istringstream line_stream(z_line);

        std::size_t i_index = 0;
        std::string z_number;
        while (std::getline(line_stream, z_number, ' '))
        {
            if (z_number.empty()) // whitespace
                continue;

            CalcResults& ai_results =
                i_index < aai_results.size() ?
                    aai_results[i_index] :
                    aai_results.emplace_back(CalcResults{0, 1})
                ;

            const Number i_number = std::stoull(z_number);
            ai_results[0] += i_number;
            ai_results[1] *= i_number;

            i_index += 1;
        }
    }

    Number i_result = 0;
    {
        std::istringstream line_stream(z_line);

        std::size_t i_index = 0;
        std::string z_operator;
        while (std::getline(line_stream, z_operator, ' '))
        {
            if (z_operator.empty()) // whitespace
                continue;

            i_result += aai_results[i_index][std::size_t(z_operator[0] == '*')];

            i_index += 1;
        }
    }

    std::println("{}", i_result);
}

void
part_2()
{
    std::ifstream file("day_6/input.txt");

    std::vector<Number> ai_numbers;

    std::string z_line;
    while (std::getline(file, z_line))
    {
        if (z_line[0] == '+' || z_line[0] == '*')
            break;

        for (std::size_t i_index = 0; i_index < z_line.size(); ++i_index)
        {
            Number& i_number =
                i_index < ai_numbers.size() ?
                    ai_numbers[i_index] :
                    ai_numbers.emplace_back(0)
                ;

            const char& c_digit = z_line[i_index];
            if (c_digit != ' ')
            {
                i_number *= 10;
                i_number += (c_digit - '0');
            }
        }
    }

    Number i_result = 0;
    {
        std::istringstream line_stream(z_line);

        Number i_intermediate = 0;
        std::size_t i_number_index = 0;

        std::string z_operator;
        while (std::getline(line_stream, z_operator, ' '))
        {
            if (z_operator.empty()) // whitespace
                continue;

            const bool b_mult = z_operator[0] == '*';
            i_intermediate = b_mult;

            for (; i_number_index < ai_numbers.size(); ++i_number_index)
            {
                const Number& i_number = ai_numbers[i_number_index];
                if (i_number == 0)
                    break;
                if (b_mult)
                    i_intermediate *= i_number;
                else
                    i_intermediate += i_number;
            }

            i_result += i_intermediate;
            i_number_index += 1;
        }
    }

    std::println("{}", i_result);
}

int
main()
{
    part_1();
    part_2();
}
