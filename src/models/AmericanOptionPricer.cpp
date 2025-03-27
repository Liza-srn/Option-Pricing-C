#include "models/AmericanOptionPricer.hpp"
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

// Constructeur
AmericanOptionPricer::AmericanOptionPricer(double S_now, double K, double T, double Vol, double r, int I, double delta_t, std::string optionType)
    : S_now(S_now), K(K), T(T), Vol(Vol), r(r), I(I), delta_t(delta_t), optionType(optionType) {
    // Calcul du prix de l'option américaine
    Price = calculatePriceImpl(); // Utilise la méthode interne pour calculer le prix
}

// Méthode interne pour calculer le prix de l'option américaine
double AmericanOptionPricer::calculatePriceImpl() const {
    int i, j, k;
    double error, diff;
    const double MAXk = 100;          // Nombre maximum d'itérations SOR
    const double TOL = 1e-8;          // Tolérance pour SOR
    const double DELTA_S = 3 * K / I; // Taille d'un pas pour le sous-jacent
    const int i_STAR = static_cast<int>(S_now / DELTA_S);
    const double WEIGHT = (S_now - i_STAR * DELTA_S) / DELTA_S;

    const int J = static_cast<int>(T / delta_t);
    double adjusted_delta_t = T / J;

    const double W = 1.5; // Facteur de relaxation pour SOR

    std::vector<double> predictor(I + 1, 0.0);
    std::vector<double> Vprevious_j(I + 1, 0.0);
    std::vector<double> Vcurrent_j(I + 1, 0.0);
    std::vector<double> fix(I + 1, 0.0);
    std::vector<double> a(I + 1, 0.0);
    std::vector<double> b(I + 1, 0.0);
    std::vector<double> c(I + 1, 0.0);
    std::vector<double> A(I + 1, 0.0);
    std::vector<double> B(I + 1, 0.0);
    std::vector<double> C(I + 1, 0.0);

    // Initialisation des conditions initiales
    for (i = 0; i <= I; i++) {
        double S = i * DELTA_S;
        if (optionType == "call") {
            Vprevious_j[i] = std::max(0.0, S - K); // Condition initiale pour une option call
        } else {
            Vprevious_j[i] = std::max(0.0, K - S); // Condition initiale pour une option put
        }
        fix[i] = Vprevious_j[i];
    }

    // Construction des coefficients
    for (i = 1; i <= I - 1; i++) {
        a[i] = adjusted_delta_t / 4 * (Vol * Vol * i * i - r * i);
        b[i] = (1 - adjusted_delta_t / 2 * (r + Vol * Vol * i * i));
        c[i] = (adjusted_delta_t / 4 * (Vol * Vol * i * i + r * i));
        A[i] = -a[i];
        B[i] = (1 + adjusted_delta_t / 2 * (r + Vol * Vol * i * i));
        C[i] = -c[i];
    }

    // Résolution Crank-Nicholson avec SOR
    for (j = 1; j <= J; j++) {
        if (optionType == "call") {
            Vcurrent_j[0] = 0; // Limite inférieure pour une option call
            Vcurrent_j[I] = I * DELTA_S - K * std::exp(-r * (T - j * adjusted_delta_t)); // Limite supérieure pour une option call
        } else {
            Vcurrent_j[0] = K * std::exp(-r * (T - j * adjusted_delta_t)); // Limite inférieure pour une option put
            Vcurrent_j[I] = 0; // Limite supérieure pour une option put
        }

        for (i = 1; i <= I - 1; i++) {
            Vcurrent_j[i] = Vprevious_j[i];
        }

        for (i = 1; i <= I - 1; i++) {
            predictor[i] = a[i] * Vprevious_j[i - 1] + b[i] * Vprevious_j[i] + c[i] * Vprevious_j[i + 1];
        }

        k = 0;
        do {
            error = 0.0;
            for (i = 1; i <= I - 1; i++) {
                diff = (predictor[i] - A[i] * Vcurrent_j[i - 1] - C[i] * Vcurrent_j[i + 1]) / B[i] - Vcurrent_j[i];
                error += diff * diff;
                Vcurrent_j[i] += W * diff;
            }
            k++;
        } while (error > TOL && k < MAXk);

        // Exercice anticipé pour une option américaine
        for (i = 1; i <= I - 1; i++) {
            if (optionType == "call") {
                Vcurrent_j[i] = std::max(Vcurrent_j[i], i * DELTA_S - K); // Exercice anticipé pour une option call
            } else {
                Vcurrent_j[i] = std::max(Vcurrent_j[i], K - i * DELTA_S); // Exercice anticipé pour une option put
            }
        }

        for (i = 1; i <= I - 1; i++) {
            Vprevious_j[i] = Vcurrent_j[i];
        }
    }

    // Interpolation pour obtenir le prix final
    if (i_STAR + 1 <= I) {
        return (1 - WEIGHT) * Vprevious_j[i_STAR] + WEIGHT * Vprevious_j[i_STAR + 1];
    } else {
        return Vprevious_j[i_STAR];
    }
}

// Méthode pour obtenir le prix de l'option
double AmericanOptionPricer::Value() const {
    return Price;
}

// Méthode pour calculer le prix de l'option 
double AmericanOptionPricer::calculatePrice(const Option& option) const {
    // Crée une nouvelle instance de AmericanOptionPricer avec les paramètres de l'option
    AmericanOptionPricer pricer(option.getSpotPrice(), option.getStrikePrice(), option.getTimeToMaturity(),
                                option.getVolatility(), option.getRiskFreeRate(), I, delta_t, option.getType());
    return pricer.Value();
}

// Destructeur
AmericanOptionPricer::~AmericanOptionPricer() {}