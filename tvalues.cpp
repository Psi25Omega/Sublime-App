// Program to calc t-values for data.dat

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>

using namespace std;

double mean(const vector<double> & v)
{
    double sum = 0.0;
    for (double val: v) {
        sum += val;
    }
    return sum/v.size();
}

double variance(const vector<double> & v, double mean)
{
    double sum = 0.0;
    for (double val: v){
        sum += (val - mean)*(val - mean);
    }
    return sum/(v.size() - 1);
}

// Computing t-value
double t_value(const vector<double> & v, double hyp_mean)
{
    double mu = mean(v);
    double std = sqrt(variance(v, mu));
    int n = v.size();

    double t_value = (mu - hyp_mean) / (std / sqrt(n));
    return t_value;
}

int main()
{
    ifstream file("data.dat");
    if (!file.is_open())
    {
        return -1;
    }
    vector <double> a, b, c, d, e;
    string line;

    while (getline(file, line))
    {
        istringstream ss(line);
        double va, vb, vc, vd, ve, vy;
        if (ss >> va >> vb >> vc >> vd >> ve >> vy)
        {
            a.push_back(va);
            b.push_back(vb);
            c.push_back(vc);
            d.push_back(vd);
            e.push_back(ve);
        }
    }

    cout << "a t-value: " << t_value(a, 0.0) << endl;
    cout << "b t-value: " << t_value(b, 0.0) << endl;
    cout << "c t-value: " << t_value(c, 0.0) << endl;
    cout << "d t-value: " << t_value(d, 0.0) << endl;
    cout << "e t-value: " << t_value(e, 0.0) << endl;

    return 0;
}

// Using libraries to calculate t-values
    