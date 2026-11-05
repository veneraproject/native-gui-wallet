'use strict';
function trustedSender({frameUrl,parent,senderId,windowId,operation=''}){
 if(senderId!==windowId||!frameUrl)return false;
 try{const u=new URL(frameUrl);return u.protocol==='venera:'&&u.hostname==='ui'&&!u.username&&!u.password&&!u.port&&/^\/[a-z0-9-]+\.html$/.test(u.pathname)&&(!parent||operation.startsWith('public.'));}catch{return false;}
}
module.exports={trustedSender};
