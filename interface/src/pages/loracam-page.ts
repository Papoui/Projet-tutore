import { html, LitElement } from "lit";
import { customElement, property } from "lit/decorators.js";
import type { LoRaCamConfig } from "../interfaces/config";

@customElement('loracam-page')
export class LoracamPage extends LitElement {

    @property({ type: Object })
    config: LoRaCamConfig | null = null;

    private handleSubmit(event: Event) {
        event.preventDefault();
        const form = event.target as HTMLFormElement;
        const formData = new FormData(form);

        const newConfig: LoRaCamConfig = {
            quality: Number(formData.get('quality')),
            mss: Number(formData.get('mss'))
        };

        this.dispatchEvent(new CustomEvent('save', {
            detail: newConfig,
            bubbles: true,
            composed: true
        }));
    }

    protected render(): unknown {
        if (!this.config) { return html`<p>Chargement...</p>`; }

        return html`
            <h2>Paramètres LoRaCam</h2>
            <form @submit="${this.handleSubmit}">
                <fieldset>
                    <p>
                        <label>Quality Factor</label>
                        <input type="number" name="quality" min="5" max="100" .value="${this.config.quality}">
                    </p>
                    <p>
                        <label>MSS</label>
                        <input type="number" name="mss" min="20" max="240" .value="${this.config.mss}">
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
        'loracam-page': LoracamPage
    }
}