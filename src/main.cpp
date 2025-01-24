#include <iostream>
#include "BlackScholes.cpp"

using namespace std;

int main() {
    Call callOption(100, 100, 0.05, 0.2, 1);
    std::cout << "Call Price: " << callOption.price() << std::endl;

    Put putOption(100, 100, 0.05, 0.2, 1);
    std::cout << "Put Price: " << putOption.price() << std::endl;

    return 0;
}
