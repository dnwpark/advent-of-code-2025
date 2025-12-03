#include <fstream>
#include <numeric>
#include <print>
#include <string>

void
part_1()
{
    std::ifstream file("day_3/input.txt");
    std::string z_line;

    int i_result = 0;

    while (std::getline(file, z_line))
    {
        const auto iter_first = std::max_element(z_line.begin(), z_line.end()-1);
        const auto iter_second = std::max_element(iter_first+1, z_line.end());
        i_result += std::stoi(std::string() + *iter_first + *iter_second);
    }

    std::println("{}", i_result);
}

void
part_2()
{
    std::ifstream file("day_3/input.txt");
    std::string z_line;

    const int i_digit_count = 12;

    unsigned long long i_result = 0;

    while (std::getline(file, z_line))
    {
        std::string z_number;

        auto iter_start = z_line.begin();
        for (int i_digit_offset = i_digit_count; i_digit_offset --> 0;)
        {
            const auto iter_digit = std::max_element(iter_start, z_line.end()-i_digit_offset);
            z_number += *iter_digit;
            iter_start = iter_digit+1;
        }

        i_result += std::stoull(z_number);
    }

    std::println("{}", i_result);
}

int
main()
{
    part_1();
    part_2();
}
