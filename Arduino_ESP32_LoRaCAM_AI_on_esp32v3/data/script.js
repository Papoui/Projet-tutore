// Fonction générique pour faire un POST avec des données JSON
async function postHTTP(url, data) {
    const response = await fetch(url, {
        method: 'POST',
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(data)
    });
    if (!response.ok) throw new Error(`Erreur HTTP ${response.status} : ${await response.text()}`);
}

// Fonction générique pour faire un GET et retourner les données JSON
async function getHTTP(url) {
    const response = await fetch(url);
    if (!response.ok) throw new Error(`Erreur HTTP ${response.status} : ${await response.text()}`);
    return await response.json();
}