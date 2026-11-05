'use strict';
const {app,BrowserWindow,ipcMain,Menu,screen,dialog,protocol,net,shell}=require('electron');
const {trustedSender}=require('./desktop/security.cjs');
const path=require('node:path');const {pathToFileURL}=require('node:url');const {Controller}=require('./desktop/controller.cjs');
protocol.registerSchemesAsPrivileged([{scheme:'venera',privileges:{standard:true,secure:true,supportFetchAPI:true,corsEnabled:true}}]);
if(process.env.VENERA_DISABLE_GPU==='1')app.disableHardwareAcceleration();
app.commandLine.appendSwitch('disable-logging');
let win,controller,quitting=false;
const root=path.join(__dirname,'app');
function openExternal(url){try{const u=new URL(url);if(u.protocol==='https:'&&['veneralabs.org','pool.veneralabs.org','github.com','bscscan.com','discord.gg','discord.com'].includes(u.hostname))shell.openExternal(url);}catch{}}
function trusted(event,operation){const frame=event.senderFrame;return trustedSender({frameUrl:frame?.url,parent:frame?.parent,senderId:event.sender.id,windowId:win?.webContents.id,operation});}
app.whenReady().then(async()=>{
 protocol.handle('venera',request=>{const url=new URL(request.url);if(url.hostname!=='ui')return new Response('Forbidden',{status:403});let name;try{name=decodeURIComponent(url.pathname);}catch{return new Response('',{status:400});}const file=path.resolve(root,'.'+name);if(!file.startsWith(root+path.sep))return new Response('Forbidden',{status:403});return net.fetch(pathToFileURL(file).toString());});
 const {width,height}=screen.getPrimaryDisplay().workAreaSize;
 controller=new Controller({nativeDir:app.isPackaged?path.join(process.resourcesPath,'native'):path.join(__dirname,'native','bin'),dataDir:path.join(app.getPath('userData'),'native-wallet'),dialog});
 win=new BrowserWindow({width:Math.min(1280,width),height:Math.min(780,height),show:false,resizable:false,frame:false,transparent:true,backgroundColor:'#00000000',title:'Venera Native Wallet',icon:path.join(root,'assets/images/icon.ico'),webPreferences:{preload:path.join(__dirname,'preload.js'),contextIsolation:true,nodeIntegration:false,sandbox:true,devTools:false}});
 Menu.setApplicationMenu(null);win.webContents.session.setPermissionRequestHandler((_wc,permission,callback)=>callback(permission==='clipboard-sanitized-write'));
 win.webContents.on('will-navigate',(event,url)=>{if(!url.startsWith('venera://ui/')){event.preventDefault();openExternal(url);}});
 win.webContents.setWindowOpenHandler(({url})=>{openExternal(url);return {action:'deny'};});
 ipcMain.handle('venera:request',async(event,operation,params)=>{if(!trusted(event,String(operation)))throw new Error('Untrusted wallet IPC sender');try{return {ok:true,value:await controller.handle(operation,params)};}catch(e){return {ok:false,error:e.message||'Operation failed'};}});
 ipcMain.on('window-minimize',e=>{if(trusted(e,''))win.minimize();});ipcMain.on('window-close',e=>{if(trusted(e,''))win.close();});
 win.once('ready-to-show',()=>win.show());await win.loadURL('venera://ui/wallet.html');
});
app.on('before-quit',event=>{if(quitting)return;event.preventDefault();quitting=true;Promise.resolve(controller?.close()).finally(()=>app.quit());});
app.on('window-all-closed',()=>app.quit());
