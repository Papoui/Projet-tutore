import { html, LitElement } from "lit";
import { customElement, property } from "lit/decorators.js";

import logo from "../assets/agrifuture_logo.png";

interface NavLink {
    path: string;
    label: string;
}

@customElement('nav-bar')
export class NavBar extends LitElement {

    @property({ type: String })
    private currentPath = '/';

    private navLinks: NavLink[] = [
        { path: '/', label: 'Accueil' },
        { path: '/wifi', label: 'Paramètres Wifi' },
        { path: '/lora', label: 'Paramètres LoRa' },
        { path: '/loracam', label: 'Paramètres LoRaCam' },
        { path: '/camera', label: 'Caméra' }
    ];

    protected render(): unknown {
        return html`
            <nav class="nav">
                <div class="nav-left">
                    <a class="brand" href="/">
                        <img src=${logo} alt="AgriFuture Logo" style="max-height: 40px;">
                    </a>
                    <div class="tabs" id="nav-tabs">
                        ${this.navLinks.map(link => html`
                            <a 
                                href="${link.path}" 
                                class="${this.currentPath === link.path ? 'active' : ''}"
                            >
                                ${link.label}
                            </a>
                        `)}
                    </div>
                </div>
            </nav>
        `;
    }

    protected createRenderRoot(): HTMLElement | DocumentFragment { return this; }
}