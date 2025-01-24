#include <iostream>
#include <cmath>
#include <numbers>

class NumericalOperations{
    protected:
        static inline double exp_minus_t_squared(double t) {
            return std::exp(-t * t);
    }
        static double NormalDistributionCDF(double x){
            return 0.5 * (1 + std::erf(x));
        }
};

class Option: protected NumericalOperations{
    protected:
        double s_0, K, r, sigma, T;
    
    public:
        Option(double s_0, double K, double r, double sigma, double T) : s_0(s_0), K(K), r(r), sigma(sigma), T(T) {}

        virtual ~Option() {};

        virtual double price() const = 0;
    protected:
        [[nodiscard]] double d_one() const {
            return (std::log(s_0/K) + T * (r + std::pow(sigma, 2))/2)/(sigma * std::sqrt(T));
        }
       [[nodiscard]] double d_two() const {
            return d_one() - (sigma * std::sqrt(T));
        }
};

class Call : public Option {
public:
    Call(double s_0, double K, double r, double sigma, double T)
        : Option(s_0, K, r, sigma, T) {}

    [[nodiscard]] double call() const { return price(); }

    [[nodiscard]] double price() const override{
        double d1 = d_one();
        double d2 = d_two();
        return s_0 * NormalDistributionCDF(d1) - K * std::exp(-r * T) * NormalDistributionCDF(d2);
    }
};

class Put : public Option {
public:
    Put(double s_0, double K, double r, double sigma, double T)
        : Option(s_0, K, r, sigma, T) {}

    [[nodiscard]] double put() const { return price(); }

    [[nodiscard]] double price() const override{
        double d1 = d_one();
        double d2 = d_two();
        return K * std::exp(-r * T) * NormalDistributionCDF(-d2) - s_0 * NormalDistributionCDF(-d1);
    }
};

int main() {
    Call callOption(100, 100, 0.05, 0.2, 1);
    std::cout << "Call Price: " << callOption.price() << std::endl;

    Put putOption(100, 100, 0.05, 0.2, 1);
    std::cout << "Put Price: " << putOption.price() << std::endl;

    return 0;
}
