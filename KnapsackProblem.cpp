#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <ctime>
using namespace std;

struct Item {
    double value;  // wartość przedmiotu
    int weight;  // waga przedmiotu
};

bool cmp(Item a, Item b) {
    double r1 = a.value / a.weight;
    double r2 = b.value / b.weight;
    return r1 > r2;
}
double knapsack(int W, vector<Item> items) {
    sort(items.begin(), items.end(), cmp);

    int currentWeight = 0;  // current weight
    double finalValue = 0.0;  // final value of contained items

    // While there is still space in the knapsack and there are available items
    for (int i = 0; W > 0 && i < items.size(); i++) {
        // If you can take a full item
        if (i >= items.size()) {
            break;
        }
        if (currentWeight + items[i].weight <= W) {
            currentWeight += items[i].weight;  // adding full item
            finalValue += items[i].value;  // adding value of item
        }
        else {
            break;  // leaving loop because there is no space
        }
    }

    return finalValue;
}
// Dynamic knapsack
// n - number of items
// W - capacity of knapsack
// wt - table of weights
// val - table of values
int dynamicknapsack(int n, int W, vector<int>& wt, vector<int>& val) {
    vector<vector<int>> dp(n + 1, vector<int>(W + 1));
    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (i == 0 || w == 0)
                dp[i][w] = 0;
            else if (wt[i - 1] <= w)
                dp[i][w] = max(val[i - 1] + dp[i - 1][w - wt[i - 1]], dp[i - 1][w]);
            else
                dp[i][w] = dp[i - 1][w];
        }
    }
    return dp[n][W];
}
double fractionalKnapsack(int W, vector<Item> items) {
    sort(items.begin(), items.end(), cmp);
    int currentWeight = 0;
    double finalValue = 0.0;
    for (int i = 0; W > 0 && i < items.size(); i++) {
        if (currentWeight + items[i].weight <= W) {
            currentWeight += items[i].weight;
            finalValue += items[i].value;
        }
        else {
            int remaining = W - currentWeight;
            finalValue += items[i].value * ((double)remaining / items[i].weight);
            break;
        }
    }

    return finalValue;
}


void test() {
    int W = 50;
    vector<Item> items = { {40, 25}, {68, 42}, {56, 20} };
    double result = knapsack(W, items);
    double expected = 96;
    double epsilon = 0.0001;
    if (abs(result - expected) < epsilon)
        cout << "Optimal solution" << endl;
    else
        cout << "Suboptimal solution" << endl;
}
void testDynamicKnapsack() {
    int W = 50;
    vector<Item> items = { {40, 25}, {68, 42}, {56, 20} };
    vector<int> weight;
    vector<int> values;
    for (auto i : items) {
        weight.push_back(i.weight);
        values.push_back(i.value);
    }
    int result = dynamicknapsack(items.size(), W, weight, values);
    double expected = 96;
    if (result == expected)
        cout << "Optimal solution" << endl;
    else
        cout << "Suboptimal solution" << endl;
}
double evaluateDynamic(int W, vector<int>& wt, vector<int>& val) {
    int dynamicValue = dynamicknapsack(wt.size(), W, wt, val);
    double optimalValue = dynamicknapsack(wt.size(), W, wt, val);
    cout << "The value obtained with dynamic method: " << dynamicValue << endl;
    cout << "Optimal value: " << optimalValue << endl;
    return dynamicValue / optimalValue;
}
double evaluateGreedy(int W, vector<Item> items, vector<int>& wt, vector<int>& val) {
    double greedyValue = knapsack(W, items);
    double optimalValue = dynamicknapsack(items.size(), W, wt, val);
    cout << "The value obtained with greedy method: " << greedyValue << endl;
    cout << "Optimal value: " << optimalValue << endl;
    return greedyValue / optimalValue;

}
double evaluateFractional(int W, vector<Item> items) {
    double fractionalValue = fractionalKnapsack(W, items);;
    cout << "The value obtained with greedy method in the fractional knapsack: " << fractionalValue << endl;
    return 1;
}
void evaluateAll(int W, vector<Item> items, vector<int> wt, vector<int> val) {
    cout << "Evaluation of the greedy method optimization: " << evaluateGreedy(W, items, wt, val) << endl;
    cout << endl;
    cout << "Evaluation of the dynamic method optimization: " << evaluateDynamic(W, wt, val) << endl;
    cout << endl;
    cout << "Evaluation of the greedy method optimization in the fractional knapsack: " << evaluateFractional(W, items) << endl;
}
int main() {
    int num_items = 30000;
    int knapsack_capacity = 1000000;

    ofstream dataFile;
    dataFile.open("data.txt");

    dataFile << num_items << " " << knapsack_capacity << endl;
    for (int i = 0; i < num_items; i++) {
        int value = rand() % 10 + 1;
        int weight = rand() % 10 + 1;
        dataFile << value << " " << weight << endl;
    }

    dataFile.close();
    int n = 0, w, W;
    cout << "Enter the capacity: ";
    cin >> W;
    cout << endl;
    int v;
    fstream data;
    data.open("data.txt", ios::in);
    if (!data.good()) {
        cout << "There is a problem with the file" << endl;
    }
    else {
        vector<Item> items;
        vector<Item> items2;
        vector<int> wt;
        vector<int> val;
        while (!data.eof()) {

            data >> v >> w;
            items.push_back(Item());
            items2.push_back(Item());
            items[n].value = v;
            items2[n].value = v;
            items[n].weight = w;
            items2[n].weight = w;
            val.push_back(v);
            wt.push_back(w);
            n++;
        }
        cout << "\033[32mKNAPSACK PROBLEM - GREEDY METHOD\033[0m" << endl;
        clock_t start = clock();
        cout << "Result: " << knapsack(W, items) << endl;
        clock_t end = clock();
        double result_time1 = (double)(end - start) / CLOCKS_PER_SEC;
        test();
        cout << "Time: " << result_time1 << "s";
        cout << endl;
        cout << endl;
        cout << "\033[32mKNAPSACK PROBLEM - DYNAMIC METHOD\033[0m" << endl;
        start = clock();
        cout << "Result: " << dynamicknapsack(n, W, wt, val) << endl;
        testDynamicKnapsack();
        end = clock();
        double result_time2 = (double)(end - start) / CLOCKS_PER_SEC;
        cout << "Time: " << result_time2 << "s";
        cout << endl;
        cout << endl;
        cout << "\033[32mFRANCTIONAL KNAPSACK PROBLEM\033[0m" << endl;
        start = clock();
        cout << "Result: " << fractionalKnapsack(W, items) << endl;
        end = clock();
        double result_time3 = (double)(end - start) / CLOCKS_PER_SEC;
        cout << "Time: " << result_time3 << "s";
        cout << endl;
        cout << endl;    
        cout << "\033[31mEVALUATION\033[0m" << endl;   
        evaluateAll(W, items, wt, val); 
    }
    data.close();
    return 0;
}
