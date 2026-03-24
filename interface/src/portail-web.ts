import { html, LitElement } from "lit";
import { customElement, state } from "lit/decorators.js";
import { Router } from "@lit-labs/router";
import axios from "axios";

import "./pages/wifi-page";
import "./pages/lora-page";
import "./pages/loracam-page";
import "./components/nav-bar";

import type { WifiConfig, LoRaConfig, LoRaCamConfig } from "./interfaces/config";

const urlCamera = `http://${window.location.hostname}:80`;

@customElement('portail-web')
export class PortailWeb extends LitElement {

    @state()
    private configs: {
        wifi: WifiConfig | null;
        lora: LoRaConfig | null;
        loracam: LoRaCamConfig | null;
    } = {
            wifi: null,
            lora: null,
            loracam: null
        };

    private async loadConfig(type: 'wifi' | 'lora' | 'loracam') {
        if (this.configs[type] !== null) return;
        try {
            const response = await axios.get(`/api/config/${type}`);
            this.configs = { ...this.configs, [type]: response.data };
        } catch (error) {
            console.error(error);
        }
    }

    private async handleSave(event: CustomEvent, type: 'wifi' | 'lora' | 'loracam') {
        const newConfig = event.detail;
        try {
            await axios.post(`/api/config/${type}`, newConfig);
            this.configs = { ...this.configs, [type]: newConfig };
        } catch (error) {
            console.error(error);
        }
    }

    private router = new Router(this, [
        { path: '/', render: () => html`<h1>Accueil</h1>` },
        {
            path: '/wifi',
            enter: () => { this.loadConfig('wifi'); return true; },
            render: () => html`<wifi-page 
                .config=${this.configs.wifi} 
                @save=${(e: CustomEvent) => this.handleSave(e, 'wifi')}>
            </wifi-page>`
        },
        {
            path: '/lora',
            enter: () => { this.loadConfig('lora'); return true; },
            render: () => html`<lora-page 
                .config=${this.configs.lora} 
                @save=${(e: CustomEvent) => this.handleSave(e, 'lora')}>
            </lora-page>`
        },
        {
            path: '/loracam',
            enter: () => { this.loadConfig('loracam'); return true; },
            render: () => html`<loracam-page 
                .config=${this.configs.loracam} 
                @save=${(e: CustomEvent) => this.handleSave(e, 'loracam')}>
            </loracam-page>`
        },
        { path: '/camera', render: () => html`<iframe src=${urlCamera} style="flex-grow: 1; border: none;"></iframe>` },
    ]);

    protected render(): unknown {
        return html`
            <header>
                <nav-bar currentPath="${window.location.pathname}"></nav-bar>
            </header>
            <main class="container">
                ${this.router.outlet()}
            </main>
        `;
    }

    protected createRenderRoot(): HTMLElement | DocumentFragment { return this; }
}

declare global {
    interface HTMLElementTagNameMap {
        'portail-web': PortailWeb
    }
}