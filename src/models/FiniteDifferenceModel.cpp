#include "models/FiniteDifferenceModel.hpp"
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <cmath>
#include <algorithm>
#include <vector>
#include <stdexcept>

// Constructeur
FiniteDifferenceModel::FiniteDifferenceModel(int timeSteps, int assetSteps)
    : timeSteps_(timeSteps), assetSteps_(assetSteps) {
    if (timeSteps <= 0 || assetSteps <= 0) {
        throw std::invalid_argument("timeSteps and assetSteps must be positive");
    }
}

// Getter pour assetSteps_
int FiniteDifferenceModel::getAssetSteps() const {
    return assetSteps_;
}

// Getter pour timeSteps_
int FiniteDifferenceModel::getTimeSteps() const {
    return timeSteps_;
}

// Implémentation de calculatePrice pour les options européennes
double FiniteDifferenceModel::calculatePrice(const Option& option) const {
    const double T = option.getTimeToMaturity();
    const double S_0 = option.getSpotPrice();
    const double K = option.getStrikePrice();
    const double sigma = option.getVolatility();
    const double r = option.getRiskFreeRate();
    const bool isCall = option.getType() == "call";

    // Ajuster S_max pour qu'il soit supérieur à S_0
    const double S_max = std::max(S_0 * 1.5, K * 2.0); // S_max >= S_0
    const int M = assetSteps_;
    const double ds = S_max / M;

    // Calculer le nombre de pas de temps en fonction de la volatilité maximale
    const double sigma2_max = std::pow(sigma * S_max, 2) / std::pow(ds, 2);
    const int N = static_cast<int>(sigma2_max * T) + 1;
    const double dt = T / N;

    using Eigen::SparseMatrix;
    using Eigen::SparseLU;
    using Eigen::Triplet;
    using Eigen::VectorXd;

    // Matrices C et D pour le schéma de Crank-Nicolson
    SparseMatrix<double> C(M + 1, M + 1);
    SparseMatrix<double> D(M + 1, M + 1);
    std::vector<Triplet<double>> tripletsC, tripletsD;
    tripletsC.reserve(3 * M);
    tripletsD.reserve(3 * M);

    // Conditions aux limites
    tripletsC.emplace_back(0, 0, 1.0);
    tripletsD.emplace_back(0, 0, 1.0);
    tripletsC.emplace_back(M, M, 1.0);
    tripletsD.emplace_back(M, M, 1.0);

    // Remplissage des matrices C et D
    for (int i = 1; i < M; ++i) {
        const double S = i * ds;
        const double sigma2 = std::pow(sigma * S, 2) / std::pow(ds, 2);
        const double mu = r * S / ds;

        const double alpha = 0.25 * dt * (sigma2 - mu);
        const double beta = -0.5 * dt * (sigma2 + r);
        const double gamma = 0.25 * dt * (sigma2 + mu);

        tripletsC.emplace_back(i, i - 1, -alpha);
        tripletsC.emplace_back(i, i, 1 - beta);
        tripletsC.emplace_back(i, i + 1, -gamma);

        tripletsD.emplace_back(i, i - 1, alpha);
        tripletsD.emplace_back(i, i, 1 + beta);
        tripletsD.emplace_back(i, i + 1, gamma);
    }

    C.setFromTriplets(tripletsC.begin(), tripletsC.end());
    D.setFromTriplets(tripletsD.begin(), tripletsD.end());

    // Conditions initiales
    VectorXd F = VectorXd::Zero(M + 1);
    for (int i = 0; i <= M; ++i) {
        double S = i * ds;
        F(i) = isCall ? std::max(S - K, 0.0) : std::max(K - S, 0.0);
    }

    // Pré-factorisation du solveur
    SparseLU<SparseMatrix<double>> solver;
    solver.compute(C);
    if (solver.info() != Eigen::Success) {
        throw std::runtime_error("Échec de la décomposition LU");
    }

    // Résolution du système linéaire
    for (int t = N - 1; t >= 0; --t) {
        const double tau = (N - t) * dt;

        // Mise à jour des conditions aux frontières
        F(0) = isCall ? 0.0 : K * std::exp(-r * tau);
        F(M) = isCall ? S_max - K * std::exp(-r * tau) : 0.0;

        VectorXd b = D * F;
        F = solver.solve(b);

        if (solver.info() != Eigen::Success) {
            throw std::runtime_error("Échec de la résolution du système");
        }
    }

    // Interpolation pour obtenir le prix final
    const int index = static_cast<int>(S_0 / ds);
    if (index < 0 || index >= M) {
        throw std::out_of_range("Index invalide lors de l'interpolation : S_0 = " + std::to_string(S_0) + ", ds = " + std::to_string(ds) + ", index = " + std::to_string(index));
    }
    const double theta = (S_0 - index * ds) / ds;
    return std::max((1 - theta) * F(index) + theta * F(index + 1), 0.0);
}