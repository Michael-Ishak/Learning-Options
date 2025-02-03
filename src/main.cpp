#include <iostream>
#include "BlackScholes.hpp"

using namespace std;

int main() {
    Call callOption(100, 100, 0.05, 0.2, 1);
    std::cout << "Call Price: " << callOption.price() << std::endl;
    std::cout << "Delta: " << callOption.delta() << std::endl;
    std::cout << "Gamma: " << callOption.gamma() << std::endl;
    std::cout << "Theta: " << callOption.theta() << std::endl;
    std::cout << "Vega: " << callOption.vega() << std::endl;
    std::cout << "Rho: " << callOption.rho() << std::endl;

    Put putOption(100, 100, 0.05, 0.2, 1);
    std::cout << "Put Price: " << putOption.price() << std::endl;
    std::cout << "Delta: " << putOption.delta() << std::endl;
    std::cout << "Gamma: " << putOption.gamma() << std::endl;
    std::cout << "Theta: " << putOption.theta() << std::endl;
    std::cout << "Vega: " << putOption.vega() << std::endl;
    std::cout << "Rho: " << putOption.rho() << std::endl;

    return 0;
}
