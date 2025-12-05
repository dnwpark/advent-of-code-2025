#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <print>
#include <ranges>
#include <vector>

using Number = std::uint64_t;
using Interval = std::array<Number,2>;

std::vector<Interval>
read_intervals(std::ifstream& file)
{
    std::vector<Interval> aai_intervals;

    std::string z_line;
    while (std::getline(file, z_line))
    {
        if (z_line.empty())
            break;

        const std::size_t i_dash = z_line.find('-');

        Number i_lower = 0;
        Number i_upper = 0;
        std::from_chars(z_line.data(), z_line.data()+i_dash, i_lower);
        std::from_chars(z_line.data()+i_dash+1, z_line.data()+z_line.size(), i_upper);

        Interval interval = {i_lower, i_upper+1};

        auto iter_insert = std::ranges::lower_bound(aai_intervals, interval);
        aai_intervals.insert(iter_insert, std::move(interval));
    }

    for (auto iter_merge = aai_intervals.begin(); iter_merge != aai_intervals.end()-1;)
    {
        if ((iter_merge+1)->at(0) <= iter_merge->at(1))
        {
            iter_merge->at(1) = std::max(iter_merge->at(1), (iter_merge+1)->at(1));
            aai_intervals.erase(iter_merge+1);
        }
        else
        {
            ++iter_merge;
        }
    }

    return aai_intervals;
}

void
part_1()
{
    std::ifstream file("day_5/input.txt");

    const std::vector<Interval> aai_intervals = read_intervals(file);

    int i_result = 0;

    std::string z_line;
    while (std::getline(file, z_line))
    {
        Number i_target = 0;
        std::from_chars(z_line.data(), z_line.data()+z_line.size(), i_target);
        const Interval ai_target = {i_target, std::numeric_limits<Number>::max()};

        const auto iter_curr = std::ranges::upper_bound(aai_intervals, ai_target);
        if (iter_curr == aai_intervals.begin())
            continue;

        if (i_target < (iter_curr-1)->at(1))
        {
            i_result += 1;
        }
    }

    std::println("{}", i_result);
}

void
part_2()
{
    std::ifstream file("day_5/input.txt");

    const std::vector<Interval> aai_intervals = read_intervals(file);

    Number i_result = 0;
    for (const Interval& ai_interval : aai_intervals)
    {
        i_result += ai_interval[1] - ai_interval[0];
    }

    std::println("{}", i_result);
}

int
main()
{
    part_1();
    part_2();
}
