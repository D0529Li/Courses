#include <iostream>
#include <vector>
#include <tgmath.h>
#include <map>

using namespace std;

class Factor
{
public:
    vector<char> variables;
    vector<double> vals;
};

Factor restrict(Factor fac, char var, bool val)
{
    int size = fac.variables.size();
    int index = 0;
    for (; index < size; index++)
    {
        if (fac.variables[index] == var)
            break;
    }
    int loopBound = pow(2, size - index - 1);
    Factor resFac;
    resFac.variables = fac.variables;
    resFac.variables.erase(resFac.variables.begin() + index);
    int i = 0;
    bool toggle = val;
    for (auto x : fac.vals)
    {
        if (toggle)
            resFac.vals.push_back(x);
        i++;
        if (i == loopBound)
        {
            i = 0;
            toggle = !toggle;
        }
    }
    return resFac;
}

Factor productFactor(Factor fac1, Factor fac2)
{
    Factor resFac;
    resFac.variables = fac1.variables;
    for (auto var : fac2.variables)
        resFac.variables.push_back(var);
    for (auto x : fac1.vals)
    {
        for (auto y : fac2.vals)
        {
            resFac.vals.push_back(x * y);
        }
    }
    vector<pair<int, int>> dupSet; // check if there are duplicate variables
    for (int i = 0; i < resFac.variables.size(); i++)
    {
        for (int j = i + 1; j < resFac.variables.size(); j++)
        {
            if (resFac.variables[i] == resFac.variables[j])
            {
                dupSet.push_back(pair(i, j));
                break;
            }
        }
    }
    if (dupSet.size() != 0)
    {
        map<int, int> removeValues;
        map<int, int> removeVars;
        for (pair<int, int> p : dupSet)
        {
            int size = resFac.variables.size();
            int b = pow(2, size - p.second - 1);
            int i = b;
            while (i < resFac.vals.size())
            {
                for (int j = i; j < i + 2 * b; j++)
                    removeValues[j] = 1;
                i += 4 * b;
            }
            removeVars[p.second] = 1;
        }
        for (auto rit = removeValues.crbegin(); rit != removeValues.crend(); rit++)
        {
            resFac.vals.erase(resFac.vals.begin() + rit->first);
        }
        for (auto rit = removeVars.crbegin(); rit != removeVars.crend(); rit++)
        {
            resFac.variables.erase(resFac.variables.begin() + rit->first);
        }
    }
    return resFac;
}

Factor sumout(Factor fac, char var)
{
    Factor resFac;
    int size = fac.variables.size();
    int index = 0;
    for (; index < size; index++)
    {
        if (fac.variables[index] == var)
            break;
    }
    resFac.variables = fac.variables;
    resFac.variables.erase(resFac.variables.begin() + index);
    int b = pow(2, size - index - 1);
    int i = 0;
    while (i < fac.vals.size())
    {
        for (int j = i; j < i + b; j++)
            resFac.vals.push_back(fac.vals[j] + fac.vals[j + b]);
        i += 2 * b;
    }
    return resFac;
}

Factor normalize(Factor fac)
{
    Factor resFac;
    resFac.variables = fac.variables;
    double sum = 0.0;
    for (auto e : fac.vals)
    {
        sum += e;
    }
    for (auto e : fac.vals)
    {
        resFac.vals.push_back(e / sum);
    }
    return resFac;
}

Factor inference(vector<Factor> vecFacs, vector<char> vecVars, vector<char> hiddenVars, vector<pair<int, bool>> vecEvidence)
{
    vector<Factor> newVec;
    for (pair p : vecEvidence)
    {
        for (Factor f : vecFacs)
            newVec.push_back(restrict(f, p.first, p.second));
    }
    Factor resFac = newVec[0];
    for (int i = 1; i < newVec.size(); i++)
        resFac = productFactor(resFac, newVec[i]);
    for (char ch : hiddenVars)
    {
        resFac = sumout(resFac, ch);
    }
    resFac = normalize(resFac);
    return resFac;
}

// int main()
// {
//     Factor fac1, fac2;
//     fac1.variables.push_back('A');
//     fac1.variables.push_back('B');
//     fac1.variables.push_back('C');
//     fac1.vals.push_back(0.03);
//     fac1.vals.push_back(0.07);
//     fac1.vals.push_back(0.54);
//     fac1.vals.push_back(0.36);
//     fac1.vals.push_back(0.06);
//     fac1.vals.push_back(0.14);
//     fac1.vals.push_back(0.48);
//     fac1.vals.push_back(0.32);
//     Factor newFac = normalize(fac1);
//     for (auto ch : newFac.variables)
//     {
//         cout << ch << " ";
//     }
//     cout << endl;
//     for (auto ch : newFac.vals)
//     {
//         cout << ch << " ";
//     }
//     cout << endl;
//     return 0;
// }
