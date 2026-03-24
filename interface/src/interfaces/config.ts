export interface WifiConfig {
    ssid: string;
    password: string;
    accessPoint: boolean;
}

export interface LoRaConfig {
    bw: number;
    sf: number;
    frequency: number;
    devAddr: string;
    appSKey: string;
    nwkSKey: string;
}

export interface LoRaCamConfig {
    quality: number;
    mss: number;
}