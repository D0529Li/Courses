#include <iostream>
#include <cmath>
#include <random>
#include <vector>

using namespace std;

bool testing;
const int NUM_OF_MSG = 1;

// [0] is for 4-room model; [1] is for Maze model; [2] is for empty model.
bool validLocation[3][5][5] = {{{1, 1, 0, 1, 1}, {1, 1, 1, 1, 1}, {0, 0, 1, 0, 0}, {1, 1, 1, 1, 1}, {1, 1, 0, 1, 1}},
                               {{1, 1, 1, 1, 1}, {1, 0, 0, 0, 1}, {1, 1, 1, 0, 1}, {1, 0, 1, 1, 1}, {1, 0, 0, 0, 0}},
                               {{1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}}};

class Model
{
public:
    int modelNumber, n_ep;
    double epsilon, delta, gamma, alpha, alphaDecreaseRate;
    double q_sender[5][5][NUM_OF_MSG], q_receiver[5][5][NUM_OF_MSG][4];
    int prize_x, prize_y, x, y, message;
    Model(int model) : modelNumber{model}, delta{0.05}, gamma{0.95} {}
    void run(); // run train and test for all N_ep and delta
    void train();
    void test();
    void resetQs();
    void generatePrize(); // generate a prize at a random valid location
    bool turnoff();
    bool explore();
    bool gotPrize();
    int sendMessage();
    void update_receiver_q(int action, bool prize);
    double takeActions();
    bool move(int action);
    void update_alpha();
    void resetLocation();
    void update_sender_q(bool prize);
};

double getRandomNumber() // random float number between 0 and 1
{
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist(0, RAND_MAX);
    return (double)dist(rng) / (RAND_MAX + 1);
}

void Model::resetQs()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            for (int k = 0; k < NUM_OF_MSG; k++)
            {
                q_sender[i][j][k] = 0.0;
                for (int l = 0; l < 4; l++)
                {
                    q_receiver[i][j][k][l] = 0.0;
                }
            }
        }
    }
}

bool Model::move(int action) // action number: 0-up 1-down 2-left 3-right
{
    switch (action)
    {
    case 0:
        if (x <= 0 || !validLocation[modelNumber][x - 1][y])
            return false;
        x--;
        break;
    case 1:
        if (x >= 4 || !validLocation[modelNumber][x + 1][y])
            return false;
        x++;
        break;
    case 2:
        if (y <= 0 || !validLocation[modelNumber][x][y - 1])
            return false;
        y--;
        break;
    case 3:
        if (y >= 4 || !validLocation[modelNumber][x][y + 1])
            return false;
        y++;
        break;
    default:
        cerr << "action number out of bound!" << action << endl;
        return false;
    }
    return true;
}

bool Model::explore()
{
    if (getRandomNumber() < epsilon)
        return true;
    return false;
}

void Model::update_alpha()
{
    alpha -= alphaDecreaseRate;
}

void Model::update_sender_q(bool prize)
{
    int reward = 0;
    if (prize)
        reward = 1;
    q_sender[prize_x][prize_y][message] = (1 - alpha) * q_sender[prize_x][prize_y][message] + alpha * (double)reward;
}

void Model::update_receiver_q(int action, bool prize) // action number: 0-up 1-down 2-left 3-right
{
    int xx = x, yy = y;
    switch (action)
    {
    case 0:
        xx++;
        break;
    case 1:
        xx--;
        break;
    case 2:
        yy++;
        break;
    case 3:
        yy--;
        break;
    }
    double max = -1;
    for (int i = 0; i < 4; i++)
    {
        if (q_receiver[x][y][message][i] > max)
        {
            max = q_receiver[x][y][message][i];
        }
    }
    int reward = 0;
    if (prize)
        reward = 1;
    q_receiver[xx][yy][message][action] = (1 - alpha) * q_receiver[xx][yy][message][action] + alpha * ((double)reward + gamma * max);
}

double Model::takeActions() // returns the discounted reward
{
    int steps = 0;
    bool prize;
    do
    {
        steps++;
        int action = -1;
        double max = -1.0;
        for (int i = 0; i < 4; i++)
        {
            if (q_receiver[x][y][message][i] > max)
            {
                action = i;
                max = q_receiver[x][y][message][i];
            }
        }
        vector<int> vec;
        for (int i = 0; i < 4; i++)
        {
            if (q_receiver[x][y][message][i] == max)
                vec.push_back(i);
        }
        action = vec[(int)(getRandomNumber() * vec.size())];
        if (!testing && explore())
        {
            action = (int)(getRandomNumber() * 4);
        }
        if (!move(action)) // invalid move
            continue;
        prize = gotPrize();
        if (!testing)
            update_receiver_q(action, prize);
        if (prize)
            break;
    } while (!turnoff());
    if (!testing)
        update_sender_q(prize);
    int reward = 0;
    if (prize)
        reward = 1;
    return pow(gamma, steps) * (double)reward;
}

int Model::sendMessage()
{
    int action = -1;
    double max = -1.0;
    for (int i = 0; i < NUM_OF_MSG; i++)
    {
        if (q_sender[prize_x][prize_y][i] > max)
        {
            action = i;
            max = q_sender[prize_x][prize_y][i];
        }
    }
    vector<int> vec;
    for (int i = 0; i < NUM_OF_MSG; i++)
    {
        if (q_sender[prize_x][prize_y][i] == max)
            vec.push_back(i);
    }
    action = vec[(int)(getRandomNumber() * vec.size())];
    if (!testing && explore())
        action = (int)(getRandomNumber() * NUM_OF_MSG);
    return action;
}

bool Model::gotPrize()
{
    if (x == prize_x && y == prize_y)
        return true;
    return false;
}

void Model::resetLocation()
{
    x = 2;
    y = 2;
}

void Model::generatePrize()
{
    int xx = (int)(getRandomNumber() * 5);
    int yy = (int)(getRandomNumber() * 5);
    while (xx == 2 || !validLocation[modelNumber][xx][yy])
    {
        xx = (int)(getRandomNumber() * 5);
        yy = (int)(getRandomNumber() * 5);
    }
    prize_x = xx;
    prize_y = yy;
}

bool Model::turnoff()
{
    if (getRandomNumber() < delta)
        return true;
    return false;
}

void Model::train()
{
    for (int episode = 0; episode < n_ep; episode++)
    {
        resetLocation();
        generatePrize();
        message = sendMessage();
        takeActions();
        update_alpha();
    }
}

void Model::test()
{
    double old_epsilon = epsilon;
    epsilon = 0.0;
    double discountedAverageReward = 0.0;
    for (int i = 0; i < 1000; i++)
    {
        resetLocation();
        generatePrize();
        message = sendMessage();
        discountedAverageReward += takeActions();
    }
    discountedAverageReward /= 1000;
    cout << "N_ep=" << n_ep << " epsilon=" << old_epsilon << " " << discountedAverageReward << endl;
}

void Model::run()
{
    int set_n_ep[6] = {10, 100, 1000, 10000, 50000, 1000000};
    double set_epsilon[1] = {0.1};
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            for (int k = 0; k < 10; k++)
            {
                n_ep = set_n_ep[i];
                epsilon = set_epsilon[j];
                alpha = 0.9;
                alphaDecreaseRate = (0.9 - 0.01) / n_ep;
                resetQs();
                testing = false;
                train();
                testing = true;
                test();
            }
        }
    }
}

int main()
{
    Model model(2);
    model.run();
    return 0;
}
