#include <fstream>
#include <print>
#include <string>

const int i_initial = 50;
const int i_size = 100;

void
part_1()
{
    std::ifstream file("day_1/input.txt");
    std::string z_line;

    int i_result = 0;

    int i_dial = i_initial;
    while (std::getline(file, z_line))
    {
        i_dial += (z_line[0] == 'R' ? 1 : -1) * std::stoi(z_line.substr(1));
        i_dial %= i_size;
        if (i_dial < 0)
            i_dial += i_size;

        if (i_dial == 0)
            i_result += 1;
    }

    std::println("{}", i_result);
}

void
part_2()
{
    std::ifstream file("day_1/input.txt");
    std::string z_line;

    int i_result = 0;

    int i_dial = i_initial;
    bool b_from_zero = i_initial == 0;
    while (std::getline(file, z_line))
    {
        const int i_temp = i_dial + (z_line[0] == 'R' ? 1 : -1) * std::stoi(z_line.substr(1));
        i_dial = i_temp % i_size;
        if (i_dial < 0)
            i_dial += i_size;

        if (i_temp == 0)
            i_result += !b_from_zero;
        else if (i_temp < 0)
            i_result += -i_temp / i_size + !b_from_zero;
        else 
            i_result += i_temp / i_size;

        b_from_zero = i_dial == 0;
    }

    std::println("{} ", i_result);
}

int
main() {
    part_1();
    part_2();
}
