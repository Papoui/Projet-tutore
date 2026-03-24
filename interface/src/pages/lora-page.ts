import { html, LitElement } from "lit";
import { customElement, property } from "lit/decorators.js";
import type { LoRaConfig } from "../interfaces/config";

@customElement('lora-page')
export class LoraPage extends LitElement {

    @property({ type: Object })
    config: LoRaConfig | null = null;

    private handleSubmit(event: Event) {
        event.preventDefault();
        const form = event.target as HTMLFormElement;
        const formData = new FormData(form);

        const newConfig: LoRaConfig = {
            bw: Number(formData.get('bw')),
            sf: Number(formData.get('sf')),
            frequency: Number(formData.get('frequency')),
            devAddr: formData.get('devAddr') as string,
            appSKey: formData.get('appSKey') as string,
            nwkSKey: formData.get('nwkSKey') as string
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
            <h2>Paramètres LoRa</h2>
            <form @submit="${this.handleSubmit}">
                <fieldset>
                    <p>
                        <label>BW</label>
                        <select name="bw" .value="${this.config.bw}">
                            <option value="125">125kHz</option>
                            <option value="250">250kHz</option>
                            <option value="500">500kHz</option>
                        </select>
                    </p>
                    <p>
                        <label>SF</label>
                        <select name="sf" .value="${this.config.sf}">
                            <option value="7">7</option>
                            <option value="8">8</option>
                            <option value="9">9</option>
                            <option value="10">10</option>
                            <option value="11">11</option>
                            <option value="12">12</option>
                        </select>
                    </p>
                    <p>
                        <label>Fréquence</label>
                        <div class="grouped">
                            <input type="number" step="0.1" name="frequency" .value="${this.config.frequency}">
                            <span class="button outline">kHz</span>
                        </div>
                    </p>
                    <p>
                        <label>Device address</label>
                        <div class="grouped">
                            <input type="text" name="devAddr" .value="${this.config.devAddr}">
                            <span class="button outline">0x</span>
                        </div>
                    </p>
                    <p>
                        <label>AppSKey</label>
                        <div class="grouped">
                            <input type="text" name="appSKey" .value="${this.config.appSKey}">
                            <span class="button outline">0x</span>
                        </div>
                    </p>
                    <p>
                        <label>NwkSKey</label>
                        <div class="grouped">
                            <input type="text" name="nwkSKey" .value="${this.config.nwkSKey}">
                            <span class="button outline">0x</span>
                        </div>
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
        'lora-page': LoraPage
    }
}