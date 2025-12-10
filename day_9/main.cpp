#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <map>
#include <print>
#include <sstream>
#include <vector>

using Number = std::int64_t;
using Point = std::array<Number, 2>;

void
part_1()
{
    std::ifstream file("day_9/input.txt");

    std::vector<Point> aai_points;
    {
        std::string z_line;
        while (std::getline(file, z_line))
        {
            Point& ai_point = aai_points.emplace_back();

            std::stringstream ss(z_line);
            std::string z_number;
            std::getline(ss, z_number, ',');
            std::from_chars(z_number.data(), z_number.data()+z_number.size(), ai_point[0]);
            std::getline(ss, z_number, ',');
            std::from_chars(z_number.data(), z_number.data()+z_number.size(), ai_point[1]);
        }
    }

    std::ranges::sort(aai_points);

    Number i_result = 0;

    std::map<Number, Number> ai_y_to_first_x;
    for (const Point& ai_point : aai_points)
    {
        for (const auto& [i_other_y, i_other_x] : ai_y_to_first_x)
        {
            const Number i_area = (ai_point[0] - i_other_x + 1) * (ai_point[1] - i_other_y + 1);
            if (i_area > i_result)
            {
                i_result = i_area;
            }
        }

        if (!ai_y_to_first_x.contains(ai_point[1]))
        {
            ai_y_to_first_x[ai_point[1]] = ai_point[0];
        }
    }

    std::println("{}", i_result);
}

struct Interval
{
    Number i_left;
    bool b_bot_red;
    Number i_bot;
    Number i_top;
    bool b_top_red;

    template<class T_Out>
    T_Out format_to(T_Out out) const
    {
        out = std::format_to(out, "Interval({{");
        out = std::format_to(out, "{},", i_left);
        out = std::format_to(out, "{},", b_bot_red);
        out = std::format_to(out, "{},", i_bot);
        out = std::format_to(out, "{},", i_top);
        out = std::format_to(out, "{},", b_top_red);
        return std::format_to(out, "}})");
    }
};

template<>
struct std::formatter<Interval> {
    constexpr auto parse(const std::format_parse_context& ctx) {
        return ctx.begin();
    }
    template<class FormatContext>
    auto format(const Interval& rhs, FormatContext& ctx) const {
        return rhs.format_to(ctx.out());
    }
};

void
part_2()
{
    std::ifstream file("day_9/input.txt");

    std::vector<Point> aai_points;
    {
        std::string z_line;
        while (std::getline(file, z_line))
        {
            Point& ai_point = aai_points.emplace_back();

            std::stringstream ss(z_line);
            std::string z_number;
            std::getline(ss, z_number, ',');
            std::from_chars(z_number.data(), z_number.data()+z_number.size(), ai_point[0]);
            std::getline(ss, z_number, ',');
            std::from_chars(z_number.data(), z_number.data()+z_number.size(), ai_point[1]);
        }
    }

    std::ranges::sort(aai_points);

    Number i_result = 0;

    std::vector<Interval> a_intervals;
    for (std::size_t i_point_index = 0; i_point_index < aai_points.size(); i_point_index += 2)
    {
        const Point& ai_point_a = aai_points[i_point_index];
        const Point& ai_point_b = aai_points[i_point_index+1];

        // prevent iterator invalidation
        a_intervals.reserve(std::max(a_intervals.capacity(), a_intervals.size()+1));

        auto iter_lower = std::ranges::find_if(
            a_intervals,
            [&](const Interval& interval){return interval.i_top > ai_point_a[1];}
            );
        auto iter_upper = std::ranges::find_if(
            a_intervals,
            [&](const Interval& interval){return interval.i_bot >= ai_point_b[1];}
            );

        if (iter_lower == iter_upper)
        {
            // update intervals
            a_intervals.insert(
                iter_lower,
                Interval{
                    ai_point_a[0],
                    true,
                    ai_point_a[1],
                    ai_point_b[1],
                    true
                    }
                );

            // calculate areas
            {
                Number i_left = 0;
                auto iter_bot = iter_lower;
                while (iter_bot != a_intervals.begin())
                {
                    if (iter_bot->i_bot == (iter_bot-1)->i_top)
                        iter_bot -= 1;
                    else
                        break;

                    if (i_left <= iter_bot->i_left)
                    {
                        i_left = iter_bot->i_left;
                        const Number i_area =
                            (ai_point_a[0] - iter_bot->i_left + 1) *
                            (ai_point_a[1] - iter_bot->i_bot + 1)
                            ;
                        if (i_area > i_result && iter_bot->b_bot_red)
                        {
                            i_result = i_area;
                        }
                    }
                }
            }
            {
                Number i_left = 0;
                auto iter_top = iter_upper;
                while (iter_top+1 != a_intervals.end())
                {
                    if (iter_top->i_top == (iter_top+1)->i_bot)
                        iter_top += 1;
                    else
                        break;

                    if (i_left <= iter_top->i_left)
                    {
                        i_left = iter_top->i_left;
                        const Number i_area =
                            (ai_point_b[0] - iter_top->i_left + 1) *
                            (iter_top->i_top - ai_point_b[1] + 1)
                            ;
                        if (i_area > i_result && iter_top->b_top_red)
                        {
                            i_result = i_area;
                        }
                    }
                }
            }
        }
        else
        {
            // calculate areas
            {
                Number i_left = 0;
                auto iter_bot = iter_upper-1;
                while (iter_bot != a_intervals.begin())
                {
                    if (i_left <= iter_bot->i_left)
                    {
                        i_left = iter_bot->i_left;
                        const Number i_area =
                            (ai_point_b[0] - iter_bot->i_left + 1) *
                            (ai_point_b[1] - iter_bot->i_bot + 1)
                            ;
                        if (i_area > i_result && iter_bot->b_bot_red)
                        {
                            i_result = i_area;
                        }
                    }

                    if (iter_bot->i_bot == (iter_bot-1)->i_top)
                        iter_bot -= 1;
                    else
                        break;
                }
            }
            {
                Number i_left = 0;
                auto iter_top = iter_lower;
                while (iter_top != a_intervals.end())
                {
                    if (i_left <= iter_top->i_left)
                    {
                        i_left = iter_top->i_left;
                        const Number i_area =
                            (ai_point_a[0] - iter_top->i_left + 1) *
                            (iter_top->i_top - ai_point_a[1] + 1)
                            ;
                        if (i_area > i_result && iter_top->b_top_red)
                        {
                            i_result = i_area;
                        }
                    }

                    if (iter_top->i_top == (iter_top+1)->i_bot)
                        iter_top += 1;
                    else
                        break;
                }
            }

            // update intervals
            for (auto iter_curr = iter_lower; iter_curr != iter_upper;)
            {
                if (ai_point_a[1] <= iter_curr->i_bot && iter_curr->i_top <= ai_point_b[1])
                {
                    a_intervals.erase(iter_curr);
                    iter_upper -= 1;
                }
                else if (ai_point_a[1] <= iter_curr->i_bot)
                {
                    iter_curr->i_bot = ai_point_b[1];
                    iter_curr->b_bot_red = false;
                    iter_curr += 1;
                }
                else if (iter_curr->i_top <= ai_point_b[1])
                {
                    iter_curr->i_top = ai_point_a[1];
                    iter_curr->b_top_red = false;
                    iter_curr += 1;
                }
                else
                {
                    a_intervals.insert(
                        iter_curr+1,
                        Interval{
                            iter_curr->i_left,
                            false,
                            ai_point_b[1],
                            iter_curr->i_top,
                            iter_curr->b_top_red,
                            }
                        );
                    iter_curr->i_top = ai_point_a[1];
                    iter_curr->b_top_red = false;
                    iter_curr += 2;
                    iter_upper += 1;
                }
            }
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
