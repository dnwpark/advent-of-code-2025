#include <algorithm>
#include <fstream>
#include <print>
#include <vector>

void
part_1()
{
    std::ifstream file("day_7/input.txt");

    int i_result = 0;

    std::string z_line;
    std::getline(file, z_line);

    std::vector<int> ai_beams(z_line.size(), 0);
    ai_beams.at(z_line.find('S')) = 1;

    while (std::getline(file, z_line))
    {
        std::vector<int> ai_next_beams(ai_beams.size(), 0);

        for (std::size_t i_index = 0; i_index < ai_beams.size(); ++i_index)
        {
            if (z_line[i_index] == '^')
            {
                if (ai_beams[i_index])
                {
                    if (i_index > 0)
                    {
                        ai_next_beams[i_index-1] = 1;
                    }
                    if (i_index < ai_beams.size()-1)
                    {
                        ai_next_beams[i_index+1] = 1;
                    }
                    i_result += 1;
                }
            }
            else
            {
                if (ai_beams[i_index])
                {
                    ai_next_beams[i_index] = 1;
                }
            }
        }

        ai_beams = std::move(ai_next_beams);
    }

    std::println("{}", i_result);
}

void
part_2()
{
    using Number = std::uint64_t;

    std::ifstream file("day_7/input.txt");

    std::string z_line;
    std::getline(file, z_line);

    std::vector<Number> ai_beams(z_line.size(), 0);
    ai_beams.at(z_line.find('S')) = 1;

    while (std::getline(file, z_line))
    {
        std::vector<Number> ai_next_beams(ai_beams.size(), 0);

        for (std::size_t i_index = 0; i_index < ai_beams.size(); ++i_index)
        {
            if (z_line[i_index] == '^')
            {
                if (ai_beams[i_index])
                {
                    if (i_index > 0)
                    {
                        ai_next_beams[i_index-1] += ai_beams[i_index];
                    }
                    if (i_index < ai_beams.size()-1)
                    {
                        ai_next_beams[i_index+1] += ai_beams[i_index];
                    }
                }
            }
            else
            {
                if (ai_beams[i_index])
                {
                    ai_next_beams[i_index] += ai_beams[i_index];
                }
            }
        }

        ai_beams = std::move(ai_next_beams);
    }

    Number i_result = std::ranges::fold_left(ai_beams, 0, std::plus());

    std::println("{}", i_result);
}

int
main()
{
    part_1();
    part_2();
}
