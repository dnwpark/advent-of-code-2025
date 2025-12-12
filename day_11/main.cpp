#include <algorithm>
#include <fstream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Number = std::int64_t;

Number
count_paths(
    const std::unordered_map<std::string, std::vector<std::string>>& aaz_outgoing,
    const std::unordered_map<std::string, std::vector<std::string>>& aaz_incoming,
    const std::string& z_from,
    const std::string& z_to
    )
{
    std::unordered_set<std::string> az_downstream = {z_from};
    {
        std::vector<std::string> az_active = {z_from};

        while (!az_active.empty())
        {
            const std::string z_curr = az_active.back();
            az_active.pop_back();
            if (!aaz_outgoing.contains(z_curr))
                continue;

            const std::vector<std::string>& az_sinks = aaz_outgoing.at(z_curr);

            az_active.append_range(
                az_sinks |
                std::views::filter(
                    [&](const std::string& z_node){return !az_downstream.contains(z_node);}
                    )
                );
            az_downstream.insert_range(az_sinks);
        }
    }

    if (!az_downstream.contains(z_to))
        return 0;

    std::unordered_map<std::string, std::vector<std::string>> aaz_culled_incoming = {{z_from, {}}};
    for (const auto&[z_curr, az_incoming] : aaz_incoming)
    {
        if (!az_downstream.contains(z_curr))
            continue;

        std::vector<std::string>& az_culled_incoming = aaz_culled_incoming[z_curr];
        az_culled_incoming.append_range(
            az_incoming |
            std::views::filter(
                [&](const std::string& z_node){return az_downstream.contains(z_node);}
                )
            );
    }

    std::unordered_map<std::string, std::size_t> ai_unprocessed;
    for (const auto&[z_curr, az_incoming] : aaz_culled_incoming)
    {
        ai_unprocessed[z_curr] = az_incoming.size();
    }

    std::unordered_map<std::string, Number> ai_count = {{z_from, 1}};

    while (!ai_unprocessed.empty())
    {
        auto iter = std::ranges::find_if(
            ai_unprocessed,
            [&](const auto& entry){return entry.second == 0;}
            );

        const std::string& z_node = iter->first;

        if (aaz_outgoing.contains(z_node))
        {
            for (const std::string& z_sink : aaz_outgoing.at(z_node))
            {
                ai_unprocessed[z_sink] -= 1;
                ai_count[z_sink] += ai_count[z_node];
            }
        }

        ai_unprocessed.erase(iter);
    }

    return ai_count.at(z_to);
}

void
part_1()
{
    std::ifstream file("day_11/input.txt");

    std::unordered_map<std::string, std::vector<std::string>> aaz_outgoing;
    std::unordered_map<std::string, std::vector<std::string>> aaz_incoming;

    std::string z_line;
    while (std::getline(file, z_line))
    {
        std::stringstream ss(z_line);

        std::string z_source;
        std::getline(ss, z_source, ':');

        std::vector<std::string>& az_outgoing = aaz_outgoing[z_source];

        ss.get();

        while (!ss.eof())
        {
            std::string z_sink;
            std::getline(ss, z_sink, ' ');
            az_outgoing.push_back(z_sink);
            aaz_incoming[z_sink].push_back(z_source);
        }
    }

    const Number i_result = count_paths(
        aaz_outgoing,
        aaz_incoming,
        "you",
        "out"
        );

    std::println("{}", i_result);
}

void
part_2()
{
    std::ifstream file("day_11/input.txt");

    std::unordered_map<std::string, std::vector<std::string>> aaz_outgoing;
    std::unordered_map<std::string, std::vector<std::string>> aaz_incoming;

    std::string z_line;
    while (std::getline(file, z_line))
    {
        std::stringstream ss(z_line);

        std::string z_source;
        std::getline(ss, z_source, ':');

        std::vector<std::string>& az_outgoing = aaz_outgoing[z_source];

        ss.get();

        while (!ss.eof())
        {
            std::string z_sink;
            std::getline(ss, z_sink, ' ');
            az_outgoing.push_back(z_sink);
            aaz_incoming[z_sink].push_back(z_source);
        }
    }

    const Number i_svr_fft = count_paths(
        aaz_outgoing,
        aaz_incoming,
        "svr",
        "fft"
        );
    const Number i_fft_dac = count_paths(
        aaz_outgoing,
        aaz_incoming,
        "fft",
        "dac"
        );
    const Number i_dac_out = count_paths(
        aaz_outgoing,
        aaz_incoming,
        "dac",
        "out"
        );

    const Number i_svr_dac = count_paths(
        aaz_outgoing,
        aaz_incoming,
        "svr",
        "dac"
        );
    const Number i_dac_fft = count_paths(
        aaz_outgoing,
        aaz_incoming,
        "dac",
        "fft"
        );
    const Number i_fft_out = count_paths(
        aaz_outgoing,
        aaz_incoming,
        "fft",
        "out"
        );

    const Number i_result =
        i_svr_dac * i_dac_fft * i_fft_out +
        i_svr_fft * i_fft_dac * i_dac_out
        ;

    std::println("{}", i_result);
}

int
main()
{
    part_1();
    part_2();
}
