#!/bin/bash
# Script pour lancer le script AppleScript depuis Excel

# Chemin du script AppleScript
SCRIPT_PATH="/Users/lizasehrine/Downloads/OptionsPricingCPP-main/run_options_pricing.applescript"

# Exécuter le script AppleScript
osascript "$SCRIPT_PATH"

# Créer un indicateur que le script a été lancé
touch "/Users/lizasehrine/Downloads/OptionsPricingCPP-main/script_launched.txt"