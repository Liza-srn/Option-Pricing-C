-- Script pour exécuter les calculs d'options pricing
on run
	tell application "Microsoft Excel"
		-- Récupérer les valeurs depuis Excel
		tell active sheet
			set spotPrice to value of range "B2"
			set strikePrice to value of range "B3"
			set riskFreeRate to value of range "B4"
			set volatility to value of range "B5"
			set timeToMaturity to value of range "B6"
			set optionType to value of range "B7"
			
			-- Mettre à jour le statut
			set value of range "B9" to "Calcul en cours..."
		end tell
	end tell
	
	-- Convertir en chaînes de caractères et remplacer les virgules par des points
	set spotPrice to my formatNumber(spotPrice)
	set strikePrice to my formatNumber(strikePrice)
	set riskFreeRate to my formatNumber(riskFreeRate)
	set volatility to my formatNumber(volatility)
	set timeToMaturity to my formatNumber(timeToMaturity)
	
	-- Chemin du projet
	set projectPath to "/Users/lizasehrine/Downloads/OptionsPricingCPP-main"
	
	-- Supprimer l'ancien fichier de complétion s'il existe
	do shell script "rm -f " & quoted form of (projectPath & "/execution_complete.txt")
	
	-- Construire la commande
	set cmdString to "cd " & quoted form of projectPath & " && ./run_pricing.sh " & spotPrice & " " & strikePrice & " " & riskFreeRate & " " & volatility & " " & timeToMaturity & " " & optionType
	
	-- Afficher la commande pour vérification
	display dialog "Exécution de la commande: " & cmdString buttons {"Annuler", "Exécuter"} default button "Exécuter"
	
	-- Exécuter la commande
	tell application "Terminal"
		do script cmdString
	end tell
	
	-- Attendre que le processus termine, mais avec un délai maximum
	set maxWaitTime to 180 -- Attendre au maximum 60 secondes
	set startTime to current date
	
	repeat
		delay 2
		try
			set checkResult to do shell script "test -f " & quoted form of (projectPath & "/execution_complete.txt") & " && echo 'exists' || echo 'not found'"
			if checkResult is "exists" then
				exit repeat
			end if
		end try
		
		-- Vérifier si on a dépassé le temps maximum
		set currentTime to current date
		set elapsedTime to (currentTime - startTime)
		if elapsedTime > maxWaitTime then
			display dialog "Le temps d'attente a été dépassé. Veuillez vérifier manuellement si le calcul est terminé." buttons {"OK"} default button "OK"
			exit repeat
		end if
	end repeat
	
	-- Une fois terminé, informer l'utilisateur
	tell application "Microsoft Excel"
		tell active sheet
			set value of range "B9" to "Calcul terminé"
		end tell
	end tell
	
	display dialog "Le calcul est terminé. Retournez à Excel et exécutez la macro 'ImporterDonneesCSV' pour importer les résultats." buttons {"OK"} default button "OK"
end run

-- Fonction pour formater correctement un nombre
on formatNumber(theNumber)
	set numberString to theNumber as string
	if numberString contains "," then
		set oldDelimiters to AppleScript's text item delimiters
		set AppleScript's text item delimiters to ","
		set theTextItems to text items of numberString
		set AppleScript's text item delimiters to "."
		set formattedNumber to theTextItems as string
		set AppleScript's text item delimiters to oldDelimiters
		return formattedNumber
	else
		return numberString
	end if
end formatNumber