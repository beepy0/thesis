#include <iostream>
#include <cstdlib>

using namespace std;

void calculateSum();

int main() {
    cout << "Starting a massive loop that sums numbers:" << endl;
    calculateSum();

    return 0;
}

void calculateSum() {
    srand(1);
    double sum = 0;
    int valuesArray[10] = {5, 214, 35666, 14215215, 22, 124541, 1, 34, 5, 4848};
    for (int i=0; i<1000000000;i++) {
        sum += valuesArray[rand() % 10];
        sum /= valuesArray[rand() % 10];
    }
    cout << "The resulted sum is: " << sum << endl;
}
