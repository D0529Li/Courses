#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <time.h>
#include <cmath>

using namespace std;

int train_s[2000], train_f[2000], train_d[2000], train_gene[2000], train_ds[2000];
int test_s[2000], test_f[2000], test_d[2000], test_gene[2000], test_ds[2000];
double p_ds[3] = {0.5, 0.25, 0.25}, p_gene[2] = {0.9, 0.1};                                             // Value with certainty
double p_f[3] = {0.1, 0.8, 0.2}, p_d[3] = {0.1, 0.2, 0.8}, p_s[6] = {0.05, 0.5, 0.5, 0.01, 0.01, 0.01}; // Guessed probabilities

double getRandomNumber(int x)
{
    return (double)rand() / RAND_MAX / 2.5 * x;
}

void addNoises(int x)
{
    for (int i = 0; i < 3; i++)
    {
        double rand1 = getRandomNumber(x);
        double rand2 = getRandomNumber(x);
        p_ds[i] = (p_ds[i] + rand1) / (1 + rand1 + rand2);

        rand1 = getRandomNumber(x);
        rand2 = getRandomNumber(x);
        p_f[i] = (p_f[i] + rand1) / (1 + rand1 + rand2);

        rand1 = getRandomNumber(x);
        rand2 = getRandomNumber(x);
        p_d[i] = (p_d[i] + rand1) / (1 + rand1 + rand2);

        rand1 = getRandomNumber(x);
        rand2 = getRandomNumber(x);
        p_s[i] = (p_s[i] + rand1) / (1 + rand1 + rand2);

        if (i < 2)
        {
            rand1 = getRandomNumber(x);
            rand2 = getRandomNumber(x);
            p_f[i] = (p_f[i] + rand1) / (1 + rand1 + rand2);
        }
    }
    for (int i = 3; i < 6; i++)
    {
        double rand1 = getRandomNumber(x);
        double rand2 = getRandomNumber(x);
        p_s[i] = (p_s[i] + rand1) / (1 + rand1 + rand2);
    }
}

void resetCPT()
{
    p_ds[0] = 0.5;
    p_ds[1] = 0.25;
    p_ds[2] = 0.25;
    p_gene[0] = 0.9;
    p_gene[1] = 0.1;
    p_f[0] = 0.1;
    p_f[1] = 0.8;
    p_f[2] = 0.2;
    p_d[0] = 0.1;
    p_d[1] = 0.2;
    p_d[2] = 0.8;
    p_s[0] = 0.05;
    p_s[1] = 0.5;
    p_s[2] = 0.5;
    p_s[3] = 0.01;
    p_s[4] = 0.01;
    p_s[5] = 0.01;
}

double getSD(float p[], double mean)
{
    double sd = 0.0;
    for (int i = 0; i < 20; i++)
        sd += pow(p[i] - mean, 2);
    return sqrt(sd / 10);
}

double getTestAccuracy()
{
    double accuracy;
    int tr = 0;
    for (int n = 0; n < 100; n++)
    {
        double p[3] = {1.0, 1.0, 1.0};
        for (int c = 0; c < 3; c++) // 0 - no  1 - mild  2 - severe
        {
            p[c] *= (test_f[n] == 1 ? p_f[c] : (1.0 - p_f[c]));
            p[c] *= (test_d[n] == 1 ? p_d[c] : (1.0 - p_d[c]));
            p[c] *= p_gene[test_gene[n]];
            p[c] *= (test_s[n] == 1 ? p_s[test_gene[n] * 3 + c] : (1.0 - p_s[test_gene[n] * 3 + c]));
        }
        double m = max(max(p[0], p[1]), p[2]);
        int selection;
        if (m == p[0])
            selection = 0;
        else if (m == p[1])
            selection = 1;
        else
            selection = 2;

        if (selection == test_ds[n])
            tr++;
    }
    accuracy = (float)tr / 100.0;
    return accuracy;
}

int main()
{
    srand(time(0));
    ifstream in;
    in.open("trainData.txt");
    for (int i = 0; i < 2000; i++)
        in >> train_s[i] >> train_f[i] >> train_d[i] >> train_gene[i] >> train_ds[i];
    in.close();
    in.open("testData.txt");
    for (int i = 0; i < 100; i++)
        in >> test_s[i] >> test_f[i] >> test_d[i] >> test_gene[i] >> test_ds[i];
    in.close();
    for (int i = 0; i < 20; i++) // delta = 0.2*i
    {
        float accuracies_before[20], accuracies_after[20];
        for (int j = 0; j < 20; j++) // 20 trials for each delta
        {
            resetCPT();
            double p_res[6000];
            double prev_sum = 100.0;
            double sum = 0.0;
            addNoises(i);
            // calculating accuracy before EM
            accuracies_before[j] = getTestAccuracy();
            while (abs(sum - prev_sum) > 0.01)
            {
                prev_sum = sum;
                sum = 0.0;
                for (int n = 0; n < 2000; n++) // item number
                {
                    // D.S. is given
                    if (train_ds[n] != -1)
                    {
                        for (int c = 0; c < 3; c++)
                        {
                            p_res[3 * n + c] = 0;
                        }
                        p_res[3 * n + train_ds[n]] = 1;
                        continue;
                    }
                    // D.S. is NOT given
                    for (int c = 0; c < 3; c++) // 0 - no  1 - mild  2 - severe
                    {

                        int index = 3 * n + c;
                        p_res[index] = 1;
                        p_res[index] *= (train_f[n] == 1 ? p_f[c] : (1.0 - p_f[c]));
                        p_res[index] *= (train_d[n] == 1 ? p_d[c] : (1.0 - p_d[c]));
                        p_res[index] *= p_gene[train_gene[n]];
                        p_res[index] *= (train_s[n] == 1 ? p_s[train_gene[n] * 3 + c] : (1.0 - p_s[train_gene[n] * 3 + c]));
                        sum += p_res[index];
                    }
                }
                // Modify the initial guesses on CPTs
                double sum_f[3] = {0.0, 0.0, 0.0}, sum_d[3] = {0.0, 0.0, 0.0}, sum_s[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
                double sum_ds[3] = {0.0, 0.0, 0.0};      // sum of all weights where ds = 0/1/2
                double sum_ds_gene[3] = {0.0, 0.0, 0.0}; // sum of all weights where gene = 1 AND ds = 0/1/2;
                                                         // sum_ds[i] - sum_ds_gene[i] is gene = 0 AND ds = 0/1/2
                for (int n = 0; n < 2000; n++)
                {
                    for (int c = 0; c < 3; c++) // 0 - no  1 - mild  2 - severe
                    {
                        // denominators
                        sum_ds[c] += p_res[3 * n + c];
                        if (train_gene[n] == 1)
                            sum_ds_gene[c] += p_res[3 * n + 1];
                        // numerators
                        if (train_f[n] == 1)
                            sum_f[c] += p_res[3 * n + c];
                        if (train_d[n] == 1)
                            sum_d[c] += p_res[3 * n + c];
                        if (train_s[n] == 1)
                            sum_s[3 * train_gene[n] + c] += p_res[3 * n + c];
                    }
                }
                for (int c = 0; c < 3; c++)
                {
                    p_f[c] = sum_f[c] / sum_ds[c];
                    p_d[c] = sum_d[c] / sum_ds[c];
                    p_s[c] = sum_s[c] / (sum_ds[c] - sum_ds_gene[c]);
                }
                for (int c = 4; c < 6; c++)
                {
                    p_s[c] = sum_s[c] / sum_ds_gene[c];
                }
            }
            // calculating accuracy after EM
            accuracies_after[j] = getTestAccuracy();
        }
        double sum_before = 0.0, sum_after = 0.0;
        for (int j = 0; j < 20; j++)
        {
            sum_before += accuracies_before[j];
            sum_after += accuracies_after[j];
        }
        double mean_before = sum_before / 20;
        double mean_after = sum_after / 20;
        double sd_before = getSD(accuracies_before, mean_before);
        double sd_after = getSD(accuracies_after, mean_after);
        cout << "i = " << i << endl
             << mean_before << "  " << sd_before << endl
             << mean_after << "  " << sd_after << endl;
    }
    return 0;
}
