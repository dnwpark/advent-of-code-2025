#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <numeric>
#include <print>
#include <ranges>
#include <sstream>
#include <vector>

using Number = double;
using Point = std::array<Number, 3>;
using Distance = double;

Distance
point_distance(const Point& ad_lhs, const Point& ad_rhs)
{
    return std::sqrt(
        std::ranges::fold_left(
            std::views::zip(ad_lhs, ad_rhs),
            Number{0},
            [](Number d_sum, auto d_pair)
            {
                return d_sum + std::pow(std::get<0>(d_pair) - std::get<1>(d_pair), 2);
            }
            )
        );
};

std::vector<Point>
read_points(std::ifstream& file)
{
    std::vector<Point> aad_points;
    {
        std::string z_line;
        while (std::getline(file, z_line))
        {
            Point& ad_point = aad_points.emplace_back();

            std::stringstream ss(z_line);
            std::string z_number;
            std::getline(ss, z_number, ',');
            std::from_chars(z_number.data(), z_number.data()+z_number.size(), ad_point[0]);
            std::getline(ss, z_number, ',');
            std::from_chars(z_number.data(), z_number.data()+z_number.size(), ad_point[1]);
            std::getline(ss, z_number, ',');
            std::from_chars(z_number.data(), z_number.data()+z_number.size(), ad_point[2]);
        }
    }

    return aad_points;
}

std::vector<std::tuple<double, std::size_t, std::size_t>>
get_connections(const std::vector<Point>& aad_points)
{
    std::vector<std::tuple<double, std::size_t, std::size_t>> a_connections;
    for (std::size_t i_a = 0; i_a <  aad_points.size(); ++i_a)
    {
        for (std::size_t i_b = i_a+1; i_b <  aad_points.size(); ++i_b)
        {
            const Distance d_distance = point_distance(aad_points[i_a], aad_points[i_b]);
            a_connections.push_back({d_distance, i_a, i_b});
        }
    }

    std::ranges::sort(a_connections);

    return a_connections;
}

void
part_1()
{
    std::ifstream file("day_8/input.txt");

    const std::vector<Point> aad_points = read_points(file);
    const std::vector<std::tuple<double, std::size_t, std::size_t>> a_connections =
        get_connections(aad_points);

    std::vector<std::size_t> ai_group_sizes(1, 0);
    {
        std::size_t i_group_count = 0;
        std::vector<std::size_t> ai_point_groups(aad_points.size(), 0);
        for (const auto&[d_distance, i_a, i_b] : a_connections | std::views::take(1000))
        {
            std::size_t& i_group_a = ai_point_groups[i_a];
            std::size_t& i_group_b = ai_point_groups[i_b];

            if (i_group_a == 0 && i_group_b == 0)
            {
                i_group_count += 1;
                i_group_a = i_group_count;
                i_group_b = i_group_count;
                ai_group_sizes.push_back(2);
            }
            else if (i_group_a == 0)
            {
                i_group_a = i_group_b;
                ai_group_sizes[i_group_b] += 1;
            }
            else if (i_group_b == 0)
            {
                i_group_b = i_group_a;
                ai_group_sizes[i_group_a] += 1;
            }
            else if (i_group_a != i_group_b)
            {
                const std::size_t i_from = i_group_b;
                const std::size_t i_to = i_group_a;
                for (std::size_t& i_group_c : ai_point_groups)
                {
                    if (i_group_c == i_from)
                    {
                        i_group_c = i_to;
                    }
                }
                ai_group_sizes[i_to] += ai_group_sizes[i_from];
                ai_group_sizes[i_from] = 0;
            }
        }
    }

    std::ranges::sort(ai_group_sizes);

    const int i_result = std::ranges::fold_left(
        ai_group_sizes | std::views::drop(1) | std::views::reverse | std::views::take(3),
        1,
        std::multiplies()
        );

    std::println("{}", i_result);
}

void
part_2()
{
    std::ifstream file("day_8/input.txt");

    const std::vector<Point> aad_points = read_points(file);
    const std::vector<std::tuple<double, std::size_t, std::size_t>> a_connections =
        get_connections(aad_points);

    const Point* p_point_a = nullptr;
    const Point* p_point_b = nullptr;
    {
        std::size_t i_group_count = 0;
        std::vector<std::size_t> ai_point_groups(aad_points.size(), 0);
        std::vector<std::size_t> ai_group_sizes(1, 0);
        for (const auto&[d_distance, i_a, i_b] : a_connections)
        {
            std::size_t& i_group_a = ai_point_groups[i_a];
            std::size_t& i_group_b = ai_point_groups[i_b];
            p_point_a = &aad_points[i_a];
            p_point_b = &aad_points[i_b];

            if (i_group_a == 0 && i_group_b == 0)
            {
                i_group_count += 1;
                i_group_a = i_group_count;
                i_group_b = i_group_count;
                ai_group_sizes.push_back(2);
            }
            else if (i_group_a == 0)
            {
                i_group_a = i_group_b;
                ai_group_sizes[i_group_b] += 1;
                if (ai_group_sizes[i_group_b] == aad_points.size())
                    break;
            }
            else if (i_group_b == 0)
            {
                i_group_b = i_group_a;
                ai_group_sizes[i_group_a] += 1;
                if (ai_group_sizes[i_group_a] == aad_points.size())
                    break;
            }
            else if (i_group_a != i_group_b)
            {
                const std::size_t i_from = i_group_b;
                const std::size_t i_to = i_group_a;
                for (std::size_t& i_group_c : ai_point_groups)
                {
                    if (i_group_c == i_from)
                    {
                        i_group_c = i_to;
                    }
                }
                ai_group_sizes[i_to] += ai_group_sizes[i_from];
                ai_group_sizes[i_from] = 0;

                if (ai_group_sizes[i_to] == aad_points.size())
                    break;
            }
        }
    }

    const int i_result = p_point_a->at(0) * p_point_b->at(0);

    std::println("{}", i_result);
}

int
main()
{
    part_1();
    part_2();
}
