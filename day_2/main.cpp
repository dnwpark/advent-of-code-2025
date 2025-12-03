#include <cstdint>
#include <cmath>
#include <fstream>
#include <map>
#include <print>
#include <string>

using Number = unsigned long long;

int
digit_length(Number i_number)
{
    int i_result = 0;
    while (i_number)
    {
        i_result += 1;
        i_number /= 10;
    }
    return i_result;
}

Number
sum_invalids(
    Number i_lower,
    Number i_upper,
    Number i_length,
    Number i_repeat_count
    )
{
    const Number i_base_repeater = std::pow(10ll, i_length/i_repeat_count);
    Number i_repeater = 1;
    for (Number i_repeat = 1; i_repeat < i_repeat_count; ++i_repeat)
    {
        i_repeater *= i_base_repeater;
        i_repeater += 1;
    }

    Number i_result = 0;
    for (
        Number i_factor = ((i_lower-1) / i_repeater) + 1;
        i_factor <= i_upper / i_repeater;
        ++i_factor
        )
    {
        i_result += i_factor * i_repeater;
    }

    return i_result;
}

void
part_1()
{
    std::ifstream file("day_2/input.txt");
    std::string z_line;

    const Number i_repeat_count = 2;

    Number i_result = 0;
    while (!file.eof()) {
        std::getline(file, z_line, '-');
        Number i_lower = std::stoll(z_line);
        std::getline(file, z_line, ',');
        Number i_upper = std::stoll(z_line);

        Number i_lower_length = digit_length(i_lower);
        const Number i_upper_length = digit_length(i_upper);

        while (i_lower_length <= i_upper_length)
        {
            const Number i_next_lower = std::pow(10ll, i_lower_length);

            if (i_lower_length % i_repeat_count == 0)
            {
                i_result += sum_invalids(
                    i_lower,
                    i_lower_length < i_upper_length ?
                        (i_next_lower-1) :
                        i_upper,
                    i_lower_length,
                    i_repeat_count
                    );
            }

            i_lower = i_next_lower;
            i_lower_length += 1;
        }
    }

    std::println("{}", i_result);
};

void
part_2()
{
    std::ifstream file("day_2/input.txt");
    std::string z_line;

    Number i_result = 0;
    while (!file.eof()) {
        std::getline(file, z_line, '-');
        Number i_lower = std::stoll(z_line);
        std::getline(file, z_line, ',');
        Number i_upper = std::stoll(z_line);

        Number i_lower_length = digit_length(i_lower);
        const Number i_upper_length = digit_length(i_upper);

        while (i_lower_length <= i_upper_length)
        {
            const Number i_next_lower = std::pow(10ll, i_lower_length);

            std::map<Number, Number> ai_partial_results;
            for (
                Number i_repeat_count = i_lower_length;
                i_repeat_count >= 2;
                --i_repeat_count
                )
            {
                if (i_lower_length % i_repeat_count != 0)
                    continue;

                ai_partial_results[i_repeat_count] = sum_invalids(
                    i_lower,
                    i_lower_length < i_upper_length ?
                        (i_next_lower-1) :
                        i_upper,
                    i_lower_length,
                    i_repeat_count
                    );
            }

            for (const auto [i_repeat_count, i_partial_result]: ai_partial_results)
            {
                Number i_overlap = 2;
                for (const auto [i_inner, _]: ai_partial_results)
                {
                    i_overlap -= (i_repeat_count % i_inner == 0);
                }
                i_result += i_overlap * i_partial_result;
            }

            i_lower = i_next_lower;
            i_lower_length += 1;
        }
    }

    std::println("{}", i_result);
};

int
main()
{
    part_1();
    part_2();
    return 0;
}
