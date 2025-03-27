#!/bin/bash
# Script pour exécuter le modèle de pricing d'options depuis Excel via la ligne de commande

# Vérifier le nombre d'arguments
if [ "$#" -ne 6 ]; then
    echo "Usage: $0 <spot_price> <strike_price> <risk_free_rate> <volatility> <time_to_maturity> <option_type>"
    echo "Exemple: $0 100 100 0.05 0.2 1 put"
    exit 1
fi

# Récupérer les arguments
SPOT_PRICE=$1
STRIKE_PRICE=$2
RISK_FREE_RATE=$3
VOLATILITY=$4
TIME_TO_MATURITY=$5
OPTION_TYPE=$6

# Chemin vers l'exécutable
EXECUTABLE="./bin/price_opt"

# Vérifier que l'exécutable existe
if [ ! -f "$EXECUTABLE" ]; then
    echo "Erreur: L'exécutable $EXECUTABLE n'existe pas."
    echo "Veuillez compiler le projet d'abord."
    exit 1
fi

# Afficher les paramètres
echo "Exécution du modèle avec les paramètres:"
echo "  Prix du sous-jacent: $SPOT_PRICE"
echo "  Prix d'exercice: $STRIKE_PRICE"
echo "  Taux sans risque: $RISK_FREE_RATE"
echo "  Volatilité: $VOLATILITY"
echo "  Temps jusqu'à maturité: $TIME_TO_MATURITY"
echo "  Type d'option: $OPTION_TYPE"
echo ""

# Créer le dossier output s'il n'existe pas
mkdir -p output

# Exécuter le programme C++
echo "Exécution du programme..."
"$EXECUTABLE" "$SPOT_PRICE" "$STRIKE_PRICE" "$RISK_FREE_RATE" "$VOLATILITY" "$TIME_TO_MATURITY" "$OPTION_TYPE"

# Vérifier si l'exécution a réussi
if [ $? -eq 0 ]; then
    echo ""
    echo "Calcul terminé avec succès."
    
    # Vérifier que les fichiers CSV ont été générés
    if [ -f "output/black_scholes_data.csv" ] && [ -f "output/finite_difference_data.csv" ] && [ -f "output/american_option_data.csv" ]; then
        echo "Tous les fichiers CSV ont été générés correctement."
        
        # Créer un fichier pour indiquer que l'exécution est terminée
        touch "execution_complete.txt"
    else
        echo "Attention: Certains fichiers CSV sont manquants."
        
        # Lister les fichiers disponibles dans le dossier output
        echo "Fichiers disponibles dans le dossier output:"
        ls -la output/
    fi
else
    echo ""
    echo "Erreur lors de l'exécution du programme."
    exit 1
fi

TIMESTAMP=$(date +"%Y%m%d_%H%M%S")

# Exécuter la commande et capturer la sortie dans un fichier
"$EXECUTABLE" "$SPOT_PRICE" "$STRIKE_PRICE" "$RISK_FREE_RATE" "$VOLATILITY" "$TIME_TO_MATURITY" "$OPTION_TYPE" | tee output/terminal_output.txt


# Attendre que l'utilisateur appuie sur une touche avant de fermer la fenêtre
echo ""
echo "Appuyez sur Entrée pour fermer cette fenêtre..."
read