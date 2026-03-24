import { defineConfig } from 'vite'

export default defineConfig({
    build: {
        assetsDir: '',
        rollupOptions: {
            output: {
                entryFileNames: '[name]-[hash].js',
                chunkFileNames: '[name]-[hash].js',
                assetFileNames: '[name]-[hash].[ext]'
            }
        }
    }
})