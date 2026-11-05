'use strict';
const { spawn } = require('node:child_process');
const readline = require('node:readline');
class NativeTransport {
  constructor(binary, options={}) { this.binary=binary; this.spawn=options.spawn||spawn; this.pending=new Map(); this.serial=Promise.resolve(); this.sequence=0; }
  async start() {
    if(this.child) return;
    const child=this.spawn(this.binary,['--desktop-service'],{stdio:['pipe','pipe','pipe'],windowsHide:true,shell:false}); this.child=child;
    child.on('error',e=>this.fail(e)); child.on('exit',()=>{this.child=null;this.fail(new Error('Local wallet service stopped'));});
    child.stderr.on('data',()=>{});                                                             
    const lines=readline.createInterface({input:child.stdout});
    lines.on('line',line=>{ if(line.length>32*1024*1024){this.fail(new Error('Oversized native response'));return;} let message;try{message=JSON.parse(line);}catch{return this.fail(new Error('Invalid native wallet response'));} const p=this.pending.get(message.id);if(!p)return;this.pending.delete(message.id);clearTimeout(p.timer);if(message.error){const e=new Error(message.error);e.nativeError=true;p.reject(e);}else p.resolve(message.result); });
    const info=await this.call('ping');if(info?.protocol!=='venera-desktop-2'){child.stdin.end();throw new Error('Rebuild venera-local-cli: memory-only desktop protocol required');}
  }
  fail(error) { for(const p of this.pending.values()){clearTimeout(p.timer);p.reject(error);}this.pending.clear(); }
  call(method,params={}) {
    const job=()=>new Promise((resolve,reject)=>{
      if(!this.child?.stdin.writable)return reject(new Error('Native wallet service is not running'));
      const id=String(++this.sequence); const timer=setTimeout(()=>{this.pending.delete(id);reject(new Error('Local wallet request timed out; check transaction history before retrying'));},method==='refresh'?1800000:180000);
      this.pending.set(id,{resolve,reject,timer});let line=JSON.stringify({id,method,params});this.child.stdin.write(line+'\n',e=>{if(e){clearTimeout(timer);this.pending.delete(id);reject(e);}});line='';
    });
    const result=this.serial.then(job,job); this.serial=result.catch(()=>{}); return result;
  }
  async close(){if(!this.child)return;const child=this.child;try{await Promise.race([this.call('lock'),new Promise(r=>setTimeout(r,5000))]);}catch{}if(child.stdin.writable)child.stdin.end();setTimeout(()=>{try{child.kill();}catch{}},3000).unref();}
}
module.exports={NativeTransport};
