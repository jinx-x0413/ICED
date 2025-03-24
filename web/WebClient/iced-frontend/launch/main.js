// main.js
const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('path');
const { exec } = require('child_process');

function createWindow() {
    const win = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            preload: path.join(__dirname, 'preload.js'),
            contextIsolation: true,
            enableRemoteModule: false,
            nodeIntegration: false
        }
    });

    win.loadURL('http://localhost:3000'); // 리액트 개발 서버 URL
}

app.whenReady().then(createWindow);

ipcMain.on('run-client', (event) => {
    exec('C:\\Users\\dongjin\\Desktop\\UEtest\\UE_Package_ver.0.0.0-pre\\ShippingSample.exe', (error, stdout, stderr) => {
        if (error) {
            console.error(`실행 오류: ${stderr}`);
            event.reply('run-client-response', `실행 오류: ${stderr}`);
            return;
        }
        console.log(`클라이언트 시작: ${stdout}`);
        event.reply('run-client-response', `클라이언트 시작: ${stdout}`);
    });
});

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') app.quit();
});
