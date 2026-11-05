'use strict';
const {contextBridge,ipcRenderer}=require('electron');
contextBridge.exposeInMainWorld('electronAPI',{minimize:()=>ipcRenderer.send('window-minimize'),close:()=>ipcRenderer.send('window-close')});
contextBridge.exposeInMainWorld('veneraAPI',{call:async(operation,params={})=>{const result=await ipcRenderer.invoke('venera:request',operation,params);if(!result.ok)throw new Error(result.error);return result.value;}});
