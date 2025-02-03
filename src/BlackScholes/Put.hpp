#ifndef PUT_HPP
#define PUT_HPP

#include "Option.hpp"

class Put : public Option {
    public:
        Put(double s_0, double K, double r, double sigma, double T)
            : Option(s_0, K, r, sigma, T) {}

        [[nodiscard]] double price() const override {
            return K * std::exp(-r * T) * NormalDistributionCDF(-d_two()) - s_0 * NormalDistributionCDF(-d_one());
        }

        [[nodiscard]] double delta() const override {
            return NormalDistributionCDF(d_one()) - 1;
        }

        [[nodiscard]] double gamma() const override {
            return NormalDistributionPDF(d_one()) / (s_0 * sigma * std::sqrt(T));
        }

        [[nodiscard]] double theta() const override {
            double term1 = -(s_0 * NormalDistributionPDF(d_one()) * sigma) / (2 * std::sqrt(T));
            double term2 = r * K * std::exp(-r * T) * NormalDistributionCDF(-d_two());
            return term1 + term2;
        }

        [[nodiscard]] double vega() const override {
            return s_0 * NormalDistributionPDF(d_one()) * std::sqrt(T);
        }

        [[nodiscard]] double rho() const override {
            return -K * T * std::exp(-r * T) * NormalDistributionCDF(-d_two()) / 100.0;
        }
};

#endif
