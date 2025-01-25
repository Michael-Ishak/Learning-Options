#include <cmath>
#include <numbers>

class NumericalOperations{
    protected:
        static inline double exp_minus_t_squared(double t) {
            return std::exp(-t * t);
        }
        static inline double NormalDistributionCDF(double x){
            return 0.5 * (1 + std::erf(x));
        }
        static inline double NormalDistribtuionPDF(double x){
            return (1/(std::sqrt(std::numbers::pi))) * std::exp(-(std::pow(x, 2))/2);
        }
};

class Option: protected NumericalOperations{
    protected:
        double s_0, K, r, sigma, T;
    
    public:
        Option(double s_0, double K, double r, double sigma, double T) : s_0(s_0), K(K), r(r), sigma(sigma), T(T) {}

        [[nodiscard]] virtual double price() const = 0;
        [[nodiscard]] virtual double delta() const = 0;
        [[nodiscard]] virtual double gamma() const = 0;
        [[nodiscard]] virtual double theta() const = 0;
        [[nodiscard]] virtual double vega() const = 0;
        [[nodiscard]] virtual double rho() const = 0;

        virtual ~Option() = default;

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
        return s_0 * NormalDistributionCDF(d_one()) - K * std::exp(-r * T) * NormalDistributionCDF(d_two());
    }

    [[nodiscard]] double delta() const override {
        return NormalDistributionCDF(d_one());
    }

    [[nodiscard]] double gamma() const override{
        return NormalDistribtuionPDF(d_one())/(s_0 * sigma * std::sqrt(T));
    }

    [[nodiscard]] double theta() const override{
        return -1 * (s_0 * NormalDistribtuionPDF(d_one()) * sigma) / (2* std::sqrt(T) - r * K * std::exp(-r * T) * NormalDistributionCDF(d_two()));
    }

    [[nodiscard]] double vega() const override{
        return s_0 * NormalDistribtuionPDF(d_one()) * std::sqrt(T);
    }

    [[nodiscard]] double rho() const override{
        return K * T * exp(-r * T) * NormalDistributionCDF(d_two());
    }
};

class Put : public Option {
public:
    Put(double s_0, double K, double r, double sigma, double T)
        : Option(s_0, K, r, sigma, T) {}

    [[nodiscard]] double put() const { return price(); }

    [[nodiscard]] double price() const override{
        return K * std::exp(-r * T) * NormalDistributionCDF(-d_two()) - s_0 * NormalDistributionCDF(-d_one());
    }
};

// int main() {
//     Call callOption(100, 100, 0.05, 0.2, 1);
//     std::cout << "Call Price: " << callOption.price() << std::endl;

//     Put putOption(100, 100, 0.05, 0.2, 1);
//     std::cout << "Put Price: " << putOption.price() << std::endl;

//     return 0;
// }
