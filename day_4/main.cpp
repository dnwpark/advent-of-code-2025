#include <fstream>
#include <print>
#include <set>
#include <vector>

void
part_1()
{
    std::ifstream file("day_4/input.txt");
    std::string z_line;

    int i_result = 0;

    std::vector<int> ai_prev_adj;
    std::vector<int> ai_curr_adj;
    std::vector<int> ai_next_adj;

    std::vector<int> ai_curr_full;
    std::vector<int> ai_next_full;

    auto x_count_accessible = [&]()
    {
        for (
            std::size_t i_index = 0;
            i_index < ai_curr_adj.size();
            ++i_index
            )
        {
            const int i_total_adj =
                ai_prev_adj[i_index] +
                ai_curr_adj[i_index] +
                ai_next_adj[i_index]
                ;
            if (ai_curr_full[i_index] && i_total_adj <= 4)
            {
                i_result += 1;
            }
        }
    };

    while (std::getline(file, z_line))
    {
        ai_next_adj.resize(z_line.size(), 0);
        ai_next_full.resize(z_line.size(), 0);
        for (
            std::size_t i_outer = 0;
            i_outer < ai_next_adj.size();
            ++i_outer
            )
        {
            if (z_line[i_outer] == '@')
            {
                ai_next_full[i_outer] = 1;
            }

            for (
                std::size_t i_inner = i_outer ? i_outer-1 : 0;
                i_inner < std::min(i_outer+2, ai_next_adj.size());
                ++i_inner
                )
            {
                if (z_line[i_inner] == '@')
                {
                    ai_next_adj[i_outer] += 1;
                }
            }
        }

        if (ai_curr_adj.empty())
        {
            ai_prev_adj.resize(ai_next_adj.size(), 0);
            ai_curr_adj = std::move(ai_next_adj);
            ai_curr_full = std::move(ai_next_full);
            continue;
        }

        x_count_accessible();

        ai_prev_adj = std::move(ai_curr_adj);
        ai_curr_adj = std::move(ai_next_adj);
        ai_curr_full = std::move(ai_next_full);
    }

    ai_next_adj.resize(ai_curr_adj.size(), 0);
    x_count_accessible();

    std::println("{}", i_result);
}

void
part_2()
{
    std::ifstream file("day_4/input.txt");
    std::string z_line;

    using Coord = std::array<std::size_t, 2>;
    std::vector<Coord> aai_full_coords;
    std::set<Coord> ai_full_coord_set;

    std::size_t i_row_count = 0;
    std::size_t i_col_count = 0;
    while (std::getline(file, z_line))
    {
        for (std::size_t i_col = 0; i_col < z_line.size(); ++i_col)
        {
            if (z_line[i_col] == '@')
            {
                const Coord ai_coord = {i_row_count, i_col};
                aai_full_coords.push_back(ai_coord);
                ai_full_coord_set.insert(ai_coord);
            }
        }

        i_row_count += 1;
        i_col_count = z_line.size();
    }

    auto x_for_adjacent = [&](const Coord& ai_coord, auto x_func) {
        for (
            std::size_t i_row = ai_coord[0] ? ai_coord[0]-1 : 0;
            i_row < std::min(ai_coord[0]+2, i_row_count);
            ++i_row
            )
        {
            for (
                std::size_t i_col = ai_coord[1] ? ai_coord[1]-1 : 0;
                i_col < std::min(ai_coord[1]+2, i_col_count);
                ++i_col
                )
            {
                x_func(Coord{i_row, i_col});
            }
        }
    };

    std::vector<std::vector<int>> aai_adj_counts(
        i_row_count,
        std::vector<int>(i_col_count, 0)
        );
    for (const Coord& ai_coord : aai_full_coords)
    {
        x_for_adjacent(
            ai_coord,
            [&](const Coord& ai_adjacent)
            {
                aai_adj_counts[ai_adjacent[0]][ai_adjacent[1]] += 1;
            }
            );
    }

    auto x_is_accessible = [&](const Coord& ai_coord)
    {
        return aai_adj_counts[ai_coord[0]][ai_coord[1]] <= 4;
    };

    std::vector<Coord> aai_access_coords;
    for (const Coord& ai_coord : aai_full_coords)
    {
        if (x_is_accessible(ai_coord))
        {
            aai_access_coords.push_back(ai_coord);
        }
    }

    int i_result = 0;
    while (!aai_access_coords.empty())
    {
        const Coord ai_coord = aai_access_coords.back();
        aai_access_coords.pop_back();
        ai_full_coord_set.erase(ai_coord);

        i_result += 1;

        x_for_adjacent(
            ai_coord,
            [&](const Coord& ai_adjacent)
            {
                const bool b_was_accessible = x_is_accessible(ai_adjacent);

                aai_adj_counts[ai_adjacent[0]][ai_adjacent[1]] -= 1;

                const bool b_is_accessible = x_is_accessible(ai_adjacent);

                if (!b_was_accessible &&
                    b_is_accessible &&
                    ai_full_coord_set.contains(ai_adjacent)
                    )
                {
                    aai_access_coords.push_back(ai_adjacent);
                }
            }
            );

    }

    std::println("{}", i_result);
}

int
main()
{
    part_1();
    part_2();
}
