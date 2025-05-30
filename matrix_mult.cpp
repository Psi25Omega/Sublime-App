#include <iostream>
#include <vector>

// Given implementation
std::vector<std::vector<int>> multiply(const std::vector<std::vector<int>> & A, const std::vector<std::vector<int>> & B)
{
    int r1 = A.size();
    int c1 = A[0].size();
    int c2 = B[0].size();
    std::vector<std::vector<int>> result(r1, std::vector<int>(c2, 0));

    for (int i = 0; i < r1; ++i)
    {
        for (int j = 0; j < c2; ++j)
        {
            for (int k = 0; k < c1; ++k)
            {
                result[i][j] += A[i][k]*B[k][j];
            }
        }
    }
    return result;
}



int main()
{
    return 0;
}