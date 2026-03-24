import { html, LitElement } from "lit";
import { customElement, property } from "lit/decorators.js";
import type { WifiConfig } from "../interfaces/config";

@customElement('wifi-page')
export class WifiPage extends LitElement {

    @property({ type: Object })
    config: WifiConfig | null = null;

    private handleSubmit(event: Event) {
        event.preventDefault();
        const form = event.target as HTMLFormElement;
        const formData = new FormData(form);

        const newConfig: WifiConfig = {
            ssid: formData.get('ssid') as string || '',
            password: formData.get('password') as string || '',
            accessPoint: formData.get('accessPoint') === 'on'
        };

        this.dispatchEvent(new CustomEvent('save', {
            detail: newConfig,
            bubbles: true,
            composed: true
        }));
    }

    protected render(): unknown {

        console.log(this.config);

        if (!this.config) { return html`<p>Chargement...</p>`; }

        return html`
            <h2>Paramètres Wifi</h2>
            <form @submit="${this.handleSubmit}">
                <fieldset>
                    <p>
                        <label>SSID</label>
                        <input type="text" name="ssid" .value="${this.config.ssid}">
                    </p>
                    <p>
                        <label>Mot de passe</label>
                        <input type="text" name="password" .value="${this.config.password || ''}">
                    </p>
                    <p>
                        <label>Mode point d'accès</label>
                        <input type="checkbox" name="accessPoint" ?checked="${this.config.accessPoint}">
                    </p>
                    <button type="submit" class="button primary">Sauvegarder</button>
                </fieldset>
            </form>
        `;
    }

    protected createRenderRoot(): HTMLElement | DocumentFragment { return this; }
}

declare global {
    interface HTMLElementTagNameMap {
        'wifi-page': WifiPage
    }
}