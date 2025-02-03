#ifndef NUMERICAL_OPERATIONS_HPP
#define NUMERICAL_OPERATIONS_HPP

#include <cmath>
#include <numbers>

class NumericalOperations {
    protected:
        static inline double NormalDistributionCDF(double x) {
            return 0.5 * (1 + std::erf(x));
        }

        static inline double NormalDistributionPDF(double x) {
            return (1.0 / (std::sqrt(2 * std::numbers::pi))) * std::exp(-0.5 * std::pow(x, 2));
        }
};

#endif
