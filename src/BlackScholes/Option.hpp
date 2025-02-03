#ifndef OPTION_HPP
#define OPTION_HPP

#include "NumericalOperations.hpp"

class Option : protected NumericalOperations {
    protected:
        double s_0, K, r, sigma, T;

    public:
        Option(double s_0, double K, double r, double sigma, double T)
            : s_0(s_0), K(K), r(r), sigma(sigma), T(T) {}

        [[nodiscard]] virtual double price() const = 0;
        [[nodiscard]] virtual double delta() const = 0;
        [[nodiscard]] virtual double gamma() const = 0;
        [[nodiscard]] virtual double theta() const = 0;
        [[nodiscard]] virtual double vega() const = 0;
        [[nodiscard]] virtual double rho() const = 0;

        virtual ~Option() = default;

    protected:
        [[nodiscard]] double d_one() const {
            return (std::log(s_0 / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
        }

        [[nodiscard]] double d_two() const {
            return d_one() - sigma * std::sqrt(T);
        }
};

#endif
