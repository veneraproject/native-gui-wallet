const fs=require('node:fs'),path=require('node:path');
const source=path.resolve(__dirname,'../node_modules/@fortawesome/fontawesome-free');const target=path.resolve(__dirname,'../app/assets/fontawesome');
fs.mkdirSync(path.join(target,'css'),{recursive:true});fs.copyFileSync(path.join(source,'css/all.min.css'),path.join(target,'css/all.min.css'));fs.cpSync(path.join(source,'webfonts'),path.join(target,'webfonts'),{recursive:true});
fs.copyFileSync(path.join(source,'LICENSE.txt'),path.join(target,'LICENSE.txt'));
const manrope=path.resolve(__dirname,'../node_modules/@fontsource/manrope');
const fonts=path.resolve(__dirname,'../app/assets/fonts');
for(const weight of [200,300,400,500,600,700,800])fs.copyFileSync(path.join(manrope,'files',`manrope-latin-${weight}-normal.woff2`),path.join(fonts,`manrope-latin-${weight}-normal.woff2`));
for(const name of ['LICENSE','LICENSE.txt','OFL.txt'])if(fs.existsSync(path.join(manrope,name)))fs.copyFileSync(path.join(manrope,name),path.join(fonts,'manrope-'+name));
