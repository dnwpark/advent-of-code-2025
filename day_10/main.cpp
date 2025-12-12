#include <algorithm>
#include <fstream>
#include <numeric>
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

struct Fraction
{
    int i_num;
    int i_den;

    template<class T_Out>
    T_Out
    format_to(T_Out out) const
    {
        out = std::format_to(out, "(");
        if (i_den == 1)
        {
            out = std::format_to(out, "{}", i_num);
        }
        else
        {
            out = std::format_to(out, "{}/", i_num);
            out = std::format_to(out, "{}", i_den);
        }
        return std::format_to(out, ")");
    }

    void
    simplify()
    {
        if (i_num == 0)
        {
            i_den = 1;
            return;
        }
        if (i_den < 0)
        {
            i_num *= -1;
            i_den *= -1;
        }
        const int i_gcd = std::gcd(std::abs(i_num), std::abs(i_den));
        i_num /= i_gcd;
        i_den /= i_gcd;
    }
};

Fraction
operator+(Fraction lhs, const Fraction& rhs)
{
    lhs.i_num = lhs.i_num*rhs.i_den + rhs.i_num*lhs.i_den;
    lhs.i_den *= rhs.i_den;
    lhs.simplify();
    return lhs;
}

Fraction
operator-(Fraction lhs, const Fraction& rhs)
{
    lhs.i_num = lhs.i_num*rhs.i_den - rhs.i_num*lhs.i_den;
    lhs.i_den *= rhs.i_den;
    lhs.simplify();
    return lhs;
}

Fraction
operator*(Fraction lhs, const Fraction& rhs)
{
    lhs.i_num *= rhs.i_num;
    lhs.i_den *= rhs.i_den;
    lhs.simplify();
    return lhs;
}

Fraction
operator/(Fraction lhs, const Fraction& rhs)
{
    lhs.i_num *= rhs.i_den;
    lhs.i_den *= rhs.i_num;
    lhs.simplify();
    return lhs;
}

template<>
struct std::formatter<Fraction> {
    constexpr auto parse(const std::format_parse_context& ctx) {
        return ctx.begin();
    }
    template<class FormatContext>
    auto format(const Fraction& rhs, FormatContext& ctx) const {
        return rhs.format_to(ctx.out());
    }
};

std::optional<std::vector<Fraction>>
solve(
    std::vector<Fraction> a_target,
    std::vector<std::vector<Fraction>> aa_matrix
    )
{
    auto x_for_each_vector = [&](auto x_func)
    {
        x_func(a_target);
        for (std::vector<Fraction>& a_vector : aa_matrix)
        {
            x_func(a_vector);
        }
    };

    std::vector<Fraction> a_result(aa_matrix.size());
    std::vector<std::size_t> ai_row_indexes(a_target.size());

    for (std::size_t i_col = 0; i_col < aa_matrix.size(); ++i_col)
    {
        {
            const auto iter_valid = std::find_if(
                aa_matrix[i_col].begin() + i_col,
                aa_matrix[i_col].end(),
                [](const Fraction& value) {return value.i_num != 0;}
                );

            if (iter_valid == aa_matrix[i_col].end())
                return {};

            const std::size_t i_switch = iter_valid - aa_matrix[i_col].begin();

            if (i_switch != i_col)
            {
                x_for_each_vector(
                    [&](std::vector<Fraction>& a_vector)
                    {
                        std::swap(a_vector[i_col], a_vector[i_switch]);
                    }
                    );
            }
        }

        {
            const Fraction divisor = aa_matrix[i_col][i_col];
            x_for_each_vector(
                [&](std::vector<Fraction>& a_vector)
                {
                    a_vector[i_col] = a_vector[i_col] / divisor;
                }
                );
        }

        for (std::size_t i_row = i_col+1; i_row < a_target.size(); ++i_row)
        {
            const Fraction factor = aa_matrix[i_col][i_row];

            x_for_each_vector(
                [&](std::vector<Fraction>& a_vector)
                {
                    a_vector[i_row] =
                        a_vector[i_row] -
                        factor * a_vector[i_col]
                        ;
                }
                );
        }
    }

    for (std::size_t i_col = aa_matrix.size(); i_col --> 0;)
    {
        for (std::size_t i_row = i_col; i_row --> 0;)
        {
            if (i_row == i_col)
                continue;

            const Fraction factor = aa_matrix[i_col][i_row];

            x_for_each_vector(
                [&](std::vector<Fraction>& a_vector)
                {
                    a_vector[i_row] =
                        a_vector[i_row] -
                        factor * a_vector[i_col]
                        ;
                }
                );
        }
    }

    // check trailing numbers
    for (std::size_t i_row = aa_matrix.size(); i_row < a_target.size(); ++i_row)
    {
        Fraction check = a_target[i_row];
        for (std::size_t i_col = 0; i_col < aa_matrix.size(); ++i_col)
        {
            check = check - a_target[i_col] * aa_matrix[i_col][i_row];
        }

        if (check.i_num != 0)
            return {};
    }

    a_target.resize(aa_matrix.size());

    // no fractional presses
    if (std::ranges::any_of(a_target, [](const Fraction& value){return value.i_den != 1;}))
        return {};

    // no negative presses
    if (std::ranges::any_of(a_target, [](const Fraction& value){return value.i_num < 0;}))
        return {};

    return a_target;
}

std::optional<int>
optional_min(const std::optional<int>& oi_lhs, const std::optional<int>& oi_rhs)
{
    if (oi_lhs && oi_rhs)
        return std::min(*oi_lhs, *oi_rhs);
    else if (oi_lhs)
        return oi_lhs;
    else
        return oi_rhs;
}

std::optional<int>
search_circuits(
    const std::vector<Fraction> a_target,
    const std::vector<std::vector<Fraction>>& aa_circuits,
    std::vector<std::vector<Fraction>>& aa_matrix,
    std::size_t i_remaining,
    std::size_t i_start
    )
{
    if (i_remaining == 0)
        return {};
    if (i_remaining + i_start > aa_circuits.size())
        return {};

    std::optional<int> oi_result;

    for (std::size_t i_index = i_start; i_index < aa_circuits.size(); ++i_index)
    {
        aa_matrix.push_back(aa_circuits[i_index]);

        if (i_remaining == 1)
        {
            if (auto ao_solution = solve(a_target, aa_matrix))
            {
                const int i_solution_count = std::ranges::fold_left(
                    *ao_solution,
                    0,
                    [](int i_lhs, const Fraction& rhs)
                    {
                        return i_lhs + rhs.i_num;
                    }
                    );
                oi_result = optional_min(oi_result, i_solution_count);
            }
        }

        oi_result = optional_min(
            oi_result,
            search_circuits(a_target, aa_circuits, aa_matrix, i_remaining-1, i_index+1)
            );

        aa_matrix.pop_back();
    }

    return oi_result;
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

        std::vector<std::vector<Fraction>> aa_vectors;
        for (const Circuit& ai_circuit : aai_circuits)
        {
            std::vector<Fraction>& ai_vector = aa_vectors.emplace_back(z_target.size()-2, Fraction{0,1});
            for (const Light& i_light : ai_circuit)
            {
                ai_vector[i_light] = Fraction{1,1};
            }
        }

        std::vector<Fraction> a_joltages;
        {
            std::string z_joltage;
            std::getline(ss, z_joltage, '}');
            std::stringstream ss_joltage(z_joltage);
            while (!ss_joltage.eof())
            {
                Fraction& joltage = a_joltages.emplace_back(Fraction{0,1});
                std::getline(ss_joltage, z_joltage, ',');
                std::from_chars(z_joltage.data(), z_joltage.data()+z_joltage.size(), joltage.i_num);
            }
        }

        // std::println("  {}", z_target);
        // std::println("  {}", aai_circuits);
        // std::println("  {}", aa_vectors);
        // std::println("  {}", a_joltages);
        // std::println();

        std::optional<int> oi_count;
        for (
            std::size_t i_remaining = 1;
            i_remaining <= std::min(a_joltages.size(), aa_vectors.size());
            ++i_remaining
            )
        {
            std::vector<std::vector<Fraction>> temp;
            if (const std::optional<int> oi_curr_count =
                search_circuits(a_joltages, aa_vectors, temp, i_remaining, 0)
                )
            {
                oi_count = optional_min(oi_count, oi_curr_count);
            }
        }
        if (oi_count)
        {
            i_result += *oi_count;
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
