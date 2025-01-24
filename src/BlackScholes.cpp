#include <iostream>
#include <cmath>
#include <numbers>

class NumericalOperations{
    protected:
        static double exp_minus_t_squared(double t) {
            return std::exp(-t * t);
    }
        static double integrate(double a, double b, int num_intervals = 10000) {
            double step = (b - a) / num_intervals;
            double integral = 0.0;

            for (int i = 0; i < num_intervals; ++i) {
                double t1 = a + i * step;
                double t2 = a + (i + 1) * step;
                integral += 0.5 * (exp_minus_t_squared(t1) + exp_minus_t_squared(t2)) * step;
            }
            return integral;
        }
        static double erf(double x) {
            return std::erf(x);
        }
        double NormalDistributionCDF(double x){
            return 0.5 * (1 + erf(x));
        }
};

class Option: protected NumericalOperations{
    protected:
        double s_0;                     //  Current stock price
        double K;                       //  Strike price
        double r;                        // Risk-free rate
        double sigma;                    //  Volatility of the underlying
        double T;                        //  Time to maturity in years
    
    public:
        Option(double s_0, double K, double r, double sigma, double T) : s_0(s_0), K(K), r(r), sigma(sigma), T(T) {}

    protected:
        double d_one(){
            return (std::log(s_0/K) + T * (r + std::pow(sigma, 2))/2)/(sigma * std::sqrt(T));
        }
        double d_two(){
            return d_one() - (sigma * std::sqrt(T));
        }
};

class Call: protected Option{
    public:
        Call(double s_0, double K, double r, double sigma, double T)
            : Option(s_0, K, r, sigma, T) {}

        double call(){
            double d1 = d_one();
            double d2 = d_two();
            return s_0 * NormalDistributionCDF(d1) - K*std::exp(-r * T)*NormalDistributionCDF(d2);
        }
};

class Put: protected Option{
    public:
        Put(double s_0, double K, double r, double sigma, double T)
        : Option(s_0, K, r, sigma, T) {}
        
        double put(){
            double d1 = d_one();
            double d2 = d_two();
            return K*std::exp(-r * T)*NormalDistributionCDF(-d2) - s_0 * NormalDistributionCDF(-d1);
        }
};

int main() {
    Call callOption(100, 100, 0.05, 0.2, 1);
    std::cout << "Call Price: " << callOption.call() << std::endl;

    Put putOption(100, 100, 0.05, 0.2, 1);
    std::cout << "Put Price: " << putOption.put() << std::endl;

    return 0;
}