#include <algorithm>
#include <fstream>
#include <print>
#include <ranges>
#include <sstream>
#include <vector>

using Toggle = int;
using Light = std::size_t;
using Circuit = std::vector<Light>;
using Joltage = int;

bool
flip_switches_1(
    const Toggle i_target,
    const std::vector<Toggle>& ai_toggles,
    Toggle& i_current,
    std::size_t i_remaining,
    std::size_t i_start
    )
{
    if (i_remaining == 0)
        return {};
    if (i_remaining + i_start > ai_toggles.size())
        return {};

    for (std::size_t i_index = i_start; i_index < ai_toggles.size(); ++i_index)
    {
        i_current ^= ai_toggles[i_index];
        if (i_remaining == 1 && i_target == i_current)
            return true;

        if (flip_switches_1(i_target, ai_toggles, i_current, i_remaining-1, i_index+1))
            return true;

        i_current ^= ai_toggles[i_index];
    }

    return false;
}

void
part_1()
{
    std::ifstream file("day_10/input.txt");

    int i_result = 0;

    std::string z_line;
    while (std::getline(file, z_line))
    {
        std::stringstream ss(z_line);

        Toggle i_target = 0;
        {
            std::string z_target;
            std::getline(ss, z_target, ' ');

            Toggle i_index = 1;
            for (char c_char : z_target | std::views::drop(1))
            {
                if (c_char == '#')
                {
                    i_target |= i_index;
                }
                i_index <<= 1;
            }
        }

        std::vector<Circuit> aai_circuits;

        while (!ss.eof())
        {
            char c_init = ss.get();

            if (c_init == '{')
                break;

            std::string z_circuit;
            std::getline(ss, z_circuit, ')');

            std::stringstream ss_circuit(z_circuit);
            Circuit& ai_circuit = aai_circuits.emplace_back();
            std::string z_switch;
            while(!ss_circuit.eof())
            {
                Light& i_light = ai_circuit.emplace_back();
                std::getline(ss_circuit, z_switch, ',');
                std::from_chars(z_switch.data(), z_switch.data()+z_switch.size(), i_light);
            }
            ss.get();
        }

        std::vector<Toggle> ai_toggles;
        for (const Circuit& ai_circuit : aai_circuits)
        {
            Toggle& i_toggle = ai_toggles.emplace_back(0);
            for (Light i_light : ai_circuit)
            {
                i_toggle |= 1 << i_light;
            }
        }

        for (std::size_t i_count = 0; i_count < ai_toggles.size(); ++i_count)
        {
            Toggle i_current = 0;
            if (flip_switches_1(
                i_target,
                ai_toggles,
                i_current,
                i_count,
                0
                ))
            {
                i_result += i_count;
                break;
            }
        }
    }

    std::println("{}", i_result);
}

bool
flip_switches_2(
    const std::vector<int>& ai_target,
    const std::vector<std::vector<int>>& aai_vectors,
    std::vector<int>& ai_current,
    std::size_t i_remaining
    )
{
    // brute force lol
    if (i_remaining == 0)
        return {};

    for (std::size_t i_index = 0; i_index < aai_vectors.size(); ++i_index)
    {
        for (std::size_t i_inner = 0; i_inner < ai_target.size(); ++i_inner)
        {
            ai_current[i_inner] += aai_vectors[i_index][i_inner];
        }

        if (i_remaining == 1 && ai_current == ai_target)
            return true;

        if (flip_switches_2(ai_target, aai_vectors, ai_current, i_remaining-1))
            return true;

        for (std::size_t i_inner = 0; i_inner < ai_target.size(); ++i_inner)
        {
            ai_current[i_inner] -= aai_vectors[i_index][i_inner];
        }
    }

    return false;
}

void
part_2()
{
    std::ifstream file("day_10/input.txt");

    int i_result = 0;

    std::string z_line;
    while (std::getline(file, z_line))
    {
        std::stringstream ss(z_line);

        std::string z_target;
        std::getline(ss, z_target, ' ');

        std::vector<Circuit> aai_circuits;

        while (!ss.eof())
        {
            char c_init = ss.get();

            if (c_init == '{')
                break;

            std::string z_circuit;
            std::getline(ss, z_circuit, ')');

            std::stringstream ss_circuit(z_circuit);
            Circuit& ai_circuit = aai_circuits.emplace_back();
            std::string z_switch;
            while(!ss_circuit.eof())
            {
                Light& i_light = ai_circuit.emplace_back();
                std::getline(ss_circuit, z_switch, ',');
                std::from_chars(z_switch.data(), z_switch.data()+z_switch.size(), i_light);
            }
            ss.get();
        }

        std::vector<std::vector<int>> aai_vectors;
        for (const Circuit& ai_circuit : aai_circuits)
        {
            std::vector<int>& ai_vector = aai_vectors.emplace_back(z_target.size()-2, 0);
            for (const Light& i_light : ai_circuit)
            {
                ai_vector[i_light] = 1;
            }
        }

        std::vector<Joltage> ai_joltages;
        {
            std::string z_joltage;
            std::getline(ss, z_joltage, '}');
            std::stringstream ss_joltage(z_joltage);
            while (!ss_joltage.eof())
            {
                Joltage& i_joltage = ai_joltages.emplace_back();
                std::getline(ss_joltage, z_joltage, ',');
                std::from_chars(z_joltage.data(), z_joltage.data()+z_joltage.size(), i_joltage);
            }
        }

        std::println("  {}", z_target);
        std::println("  {}", aai_circuits);
        std::println("  {}", aai_vectors);
        std::println("  {}", ai_joltages);

        for (int i_count = 1; true; ++i_count)
        {
            std::vector<int> ai_current(ai_joltages.size(), 0);
            if (flip_switches_2(
                ai_joltages,
                aai_vectors,
                ai_current,
                i_count
                ))
            {
                std::println("  {}", i_count);
                i_result += i_count;
                break;
            }
        }
        std::println();
    }

    std::println("{}", i_result);
}

int
main()
{
    part_1();
    part_2();
}
