const char config_page_html[] = R"=====(
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Configuration</title>
</head>
<body>
    <h1>Configuration</h1>

    <form id="configForm" onsubmit="event.preventDefault(); saveConfig();">
        SSID: <input type="text" id="ssid" name="ssid"><br>
        Mot de passe: <input type="password" id="password" name="password"><br>
        <b>Lire au démarrage:</b> <input type="checkbox" id="boot" name="boot"><br><br>
        
        <input type="submit" value="Sauvegarder">
    </form>
    <hr>
    <button onclick="manualRead()">Recharger depuis l'EEPROM</button>
    <p id="status" style="color: grey;"></p>

    <script>
        // 1. Fonction pour charger les données (GET)
        function loadConfig() {
            document.getElementById('status').innerText = "Chargement...";
            
            fetch('/api/config')
                .then(response => response.json())
                .then(data => {
                    // Remplissage des champs avec le JSON reçu
                    document.getElementById('ssid').value = data.ssid;
                    document.getElementById('password').value = data.password;
                    document.getElementById('boot').checked = data.boot;
                    document.getElementById('status').innerText = "Données chargées.";
                })
                .catch(err => {
                    console.error(err);
                    document.getElementById('status').innerText = "Erreur de chargement :\n" + err;
                });
        }

        // 2. Fonction pour sauvegarder (POST)
        function saveConfig() {
            document.getElementById('status').innerText = "Sauvegarde...";
            
            // On prépare les données comme un formulaire URL-encoded
            const params = new URLSearchParams();
            params.append('ssid', document.getElementById('ssid').value);
            params.append('password', document.getElementById('password').value);
            // Pour les checkbox, on envoie '1' si coché, sinon on n'envoie rien (ou '0')
            if(document.getElementById('boot').checked) params.append('boot', '1');

            fetch('/api/save', {
                method: 'POST',
                body: params
            })
            .then(res => {
                if (res.ok) document.getElementById('status').innerText = "Sauvegardé avec succès !";
                else document.getElementById('status').innerText = "Erreur sauvegarde.";
            });
        }

        // 3. Fonction de relecture manuelle (EEPROM -> RAM -> JSON)
        function manualRead() {
            fetch('/api/read') // Déclenche le loadFromEEPROM côté ESP
                .then(() => loadConfig()); // Recharge l'interface ensuite
        }

        // Au chargement de la page, on lance le fetch
        window.onload = loadConfig;
    </script>
</body>
</html>
)=====";