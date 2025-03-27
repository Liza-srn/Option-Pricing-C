#!/bin/bash

# Chemin du projet
PROJECT_PATH="/Users/lizasehrine/Downloads/OptionsPricingCPP-main"

# Lire les paramètres du fichier
PARAMS_FILE="$PROJECT_PATH/params.txt"
if [ -f "$PARAMS_FILE" ]; then
    # Lire les paramètres ligne par ligne
    SPOT_PRICE=$(sed -n '1p' "$PARAMS_FILE")
    STRIKE_PRICE=$(sed -n '2p' "$PARAMS_FILE")
    RISK_FREE_RATE=$(sed -n '3p' "$PARAMS_FILE")
    VOLATILITY=$(sed -n '4p' "$PARAMS_FILE")
    TIME_TO_MATURITY=$(sed -n '5p' "$PARAMS_FILE")
    OPTION_TYPE=$(sed -n '6p' "$PARAMS_FILE")
    
    # Exécuter avec les paramètres
    cd "$PROJECT_PATH"
    ./run_pricing.sh "$SPOT_PRICE" "$STRIKE_PRICE" "$RISK_FREE_RATE" "$VOLATILITY" "$TIME_TO_MATURITY" "$OPTION_TYPE"
else
    echo "Fichier de paramètres non trouvé!"
    exit 1
fi