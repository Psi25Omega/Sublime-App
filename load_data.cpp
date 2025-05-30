#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<int> split(const string & line)
{
    vector<int> result;

    string number;
    for (char ch : line)
    {
        if (ch == ',')
        {
            if (!number.empty())
            {
                result.push_back(stoi(number)); // stoi - string to integer
                number.clear();
            }
        }
        else
        {
            number += ch;
        }
    }
    if (!number.empty())
    {
        result.push_back(stoi(number));
    }
    return result;
}

int main()
{
    ifstream file("diabetes.csv");
    if (!file.is_open())
    {
        cerr << "Error opening file." << std::endl;
        return 1;
    }

    string line;
    int current_line = 0;
    string line2025, line3106;

    while (getline(file, line))
    {
        current_line ++;
        if (current_line == 2025) line2025 = line;
        if (current_line == 3106) line3106 = line;
    }

    std::cout << "Line 2025: " << line2025 << std::endl;
    std::cout << "Line 3106: " << line3106 << std::endl;

    file.close();

    vector<int> data2025 = split(line2025);
    vector<int> data3106 = split(line3106);

    return 0;
}
