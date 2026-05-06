// Generic functions for POSTing JSON data.
async function postHTTP(url, data) {
    const response = await fetch(url, {
        method: 'POST',
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(data)
    });
    if (!response.ok) throw new Error(`Erreur HTTP ${response.status} : ${await response.text()}`);
}

// Generic functions for GETting JSON data.
async function getHTTP(url) {
    const response = await fetch(url);
    if (!response.ok) throw new Error(`Erreur HTTP ${response.status} : ${await response.text()}`);
    return await response.json();
}