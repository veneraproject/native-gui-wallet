#pragma once
#include "json_compat.h"
 
#include <openssl/ec.h>
#include <openssl/hmac.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <openssl/bn.h>
#include <initializer_list>
#include <string_view>

namespace evm {
using Bytes=std::vector<unsigned char>;
template<class T, void(*Free)(T*)> using Owner=std::unique_ptr<T,decltype(Free)>;
using BN=Owner<BIGNUM,BN_clear_free>;
using Context=Owner<BN_CTX,BN_CTX_free>;
using Point=Owner<EC_POINT,EC_POINT_free>;
using Key=Owner<EC_KEY,EC_KEY_free>;
using Signature=Owner<ECDSA_SIG,ECDSA_SIG_free>;
static void check(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
static BN bn(){BN p(BN_new(),BN_clear_free);check(bool(p),"BN allocation failed");return p;}
struct Secret {
    Bytes bytes;
    explicit Secret(Bytes b):bytes(std::move(b)){}
    Secret(const Secret&)=delete;Secret& operator=(const Secret&)=delete;
    ~Secret(){if(!bytes.empty())OPENSSL_cleanse(bytes.data(),bytes.size());}
};
static std::string hex(const Bytes& data){const char* h="0123456789abcdef";std::string out;out.reserve(data.size()*2);for(auto b:data){out+=h[b>>4];out+=h[b&15];}return out;}
static Bytes unhex(const std::string& input){std::string_view s(input);if(s.rfind("0x",0)==0)s.remove_prefix(2);check(s.size()%2==0&&s.size()<=2097152,"Invalid hex length");Bytes out;for(size_t i=0;i<s.size();i+=2){auto digit=[](char c)->int{if(c>='0'&&c<='9')return c-'0';if(c>='a'&&c<='f')return c-'a'+10;if(c>='A'&&c<='F')return c-'A'+10;throw std::runtime_error("Invalid hexadecimal data");};out.push_back(static_cast<unsigned char>((digit(s[i])<<4)|digit(s[i+1])));}return out;}
static uint64_t rotate(uint64_t x,unsigned r){return r?((x<<r)|(x>>(64-r))):x;}
 
static Bytes keccak(const Bytes& input){
    static const uint64_t rc[24]={0x0000000000000001ULL,0x0000000000008082ULL,0x800000000000808aULL,0x8000000080008000ULL,0x000000000000808bULL,0x0000000080000001ULL,0x8000000080008081ULL,0x8000000000008009ULL,0x000000000000008aULL,0x0000000000000088ULL,0x0000000080008009ULL,0x000000008000000aULL,0x000000008000808bULL,0x800000000000008bULL,0x8000000000008089ULL,0x8000000000008003ULL,0x8000000000008002ULL,0x8000000000000080ULL,0x000000000000800aULL,0x800000008000000aULL,0x8000000080008081ULL,0x8000000000008080ULL,0x0000000080000001ULL,0x8000000080008008ULL};
    static const unsigned rot[25]={0,1,62,28,27,36,44,6,55,20,3,10,43,25,39,41,45,15,21,8,18,2,61,56,14};
    uint64_t state[25]={};Bytes padded=input;padded.push_back(1);while(padded.size()%136!=135)padded.push_back(0);padded.push_back(0x80);
     
    if(input.size()%136==135){padded=input;padded.push_back(0x81);}
    for(size_t pos=0;pos<padded.size();pos+=136){
        for(unsigned i=0;i<136;++i)state[i/8]^=uint64_t(padded[pos+i])<<(8*(i%8));
        for(auto constant:rc){uint64_t c[5],d[5],b[25];
            for(unsigned x=0;x<5;++x)c[x]=state[x]^state[x+5]^state[x+10]^state[x+15]^state[x+20];
            for(unsigned x=0;x<5;++x)d[x]=c[(x+4)%5]^rotate(c[(x+1)%5],1);
            for(unsigned y=0;y<5;++y)for(unsigned x=0;x<5;++x){unsigned i=x+5*y;state[i]^=d[x];b[y+5*((2*x+3*y)%5)]=rotate(state[i],rot[i]);}
            for(unsigned y=0;y<5;++y)for(unsigned x=0;x<5;++x)state[x+5*y]=b[x+5*y]^((~b[(x+1)%5+5*y])&b[(x+2)%5+5*y]);
            state[0]^=constant;
        }
    }
    Bytes out(32);for(unsigned i=0;i<32;++i)out[i]=static_cast<unsigned char>(state[i/8]>>(8*(i%8)));
    OPENSSL_cleanse(state,sizeof(state));OPENSSL_cleanse(padded.data(),padded.size());return out;
}
static Bytes random(size_t n){Bytes out(n);check(RAND_bytes(out.data(),static_cast<int>(n))==1,"Secure randomness failed");return out;}
static BN fromBytes(const Bytes& v){BN p(BN_bin2bn(v.data(),static_cast<int>(v.size()),nullptr),BN_clear_free);check(bool(p),"Invalid integer");return p;}
static Bytes bigBytes(const BIGNUM* v,size_t width=0){size_t n=static_cast<size_t>(BN_num_bytes(v));check(!width||n<=width,"Integer too large");Bytes out(width?width:n);if(n)check(BN_bn2bin(v,out.data()+out.size()-n)==static_cast<int>(n),"Integer encoding failed");return out;}
static Bytes integer(uint64_t n){Bytes out;while(n){out.insert(out.begin(),static_cast<unsigned char>(n));n>>=8;}return out;}
static std::string hexQuantity(const Bytes& n){auto h=hex(n);while(h.size()>1&&h[0]=='0')h.erase(0,1);return "0x"+(h.empty()?"0":h);}
static std::string quantity(uint64_t n){auto h=hex(integer(n));if(h.empty())h="0";if(h.size()>1&&h[0]=='0')h.erase(0,1);return "0x"+h;}
static BN parseQuantity(const std::string& s){check(std::regex_match(s,std::regex("0x[0-9a-fA-F]{1,64}")),"Invalid RPC uint256");std::string h=s.substr(2);if(h.size()%2)h="0"+h;return fromBytes(unhex(h));}
static uint64_t smallQuantity(const std::string& s){auto p=parseQuantity(s);check(BN_num_bits(p.get())<=64,"RPC integer exceeds uint64");auto b=bigBytes(p.get());uint64_t v=0;for(auto c:b)v=(v<<8)|c;return v;}
static Bytes units(const std::string& s,unsigned decimals){
    check(decimals<=36&&s.size()<=120&&std::regex_match(s,std::regex("[0-9]+(\\.[0-9]+)?")),"Use a positive plain decimal amount");
    auto dot=s.find('.');std::string whole=s.substr(0,dot),fraction=dot==s.npos?"":s.substr(dot+1);check(fraction.size()<=decimals,"Amount has excess decimal precision");fraction.append(decimals-fraction.size(),'0');auto p=bn();
    for(char c:whole+fraction){check(BN_mul_word(p.get(),10)==1&&BN_add_word(p.get(),c-'0')==1,"Amount conversion failed");check(BN_num_bits(p.get())<=256,"Amount exceeds uint256");}
    check(!BN_is_zero(p.get()),"Amount must be positive");return bigBytes(p.get());
}
static std::string display(const BIGNUM* n,unsigned decimals){char* raw=BN_bn2dec(n);check(raw,"Decimal conversion failed");std::string s(raw);OPENSSL_free(raw);if(!decimals)return s;if(s.size()<=decimals)s.insert(0,decimals+1-s.size(),'0');s.insert(s.size()-decimals,1,'.');while(s.back()=='0')s.pop_back();if(s.back()=='.')s.pop_back();return s;}
static Bytes rlpLength(size_t n,unsigned prefix){if(n<56)return Bytes{static_cast<unsigned char>(prefix+n)};auto l=integer(n);Bytes out{static_cast<unsigned char>(prefix+55+l.size())};out.insert(out.end(),l.begin(),l.end());return out;}
static Bytes rlp(const Bytes& value){if(value.size()==1&&value[0]<128)return value;auto out=rlpLength(value.size(),0x80);out.insert(out.end(),value.begin(),value.end());return out;}
static Bytes list(const std::vector<Bytes>& fields){Bytes data;for(auto& field:fields){auto encoded=rlp(field);data.insert(data.end(),encoded.begin(),encoded.end());}auto out=rlpLength(data.size(),0xc0);out.insert(out.end(),data.begin(),data.end());return out;}
static Key key(const Bytes& secret){
    check(secret.size()==32,"Private key must be exactly 32 bytes");Key k(EC_KEY_new_by_curve_name(NID_secp256k1),EC_KEY_free);check(bool(k),"secp256k1 unavailable");auto privateKey=fromBytes(secret);BN_set_flags(privateKey.get(),BN_FLG_CONSTTIME);const auto* group=EC_KEY_get0_group(k.get());auto order=bn();check(EC_GROUP_get_order(group,order.get(),nullptr)==1,"Curve order failed");check(!BN_is_zero(privateKey.get())&&BN_cmp(privateKey.get(),order.get())<0,"Private key outside secp256k1 range");
    Point pub(EC_POINT_new(group),EC_POINT_free);check(bool(pub)&&EC_POINT_mul(group,pub.get(),privateKey.get(),nullptr,nullptr,nullptr)==1,"Public key derivation failed");check(EC_KEY_set_private_key(k.get(),privateKey.get())==1&&EC_KEY_set_public_key(k.get(),pub.get())==1&&EC_KEY_check_key(k.get())==1,"Key setup failed");return k;
}
static std::string checksum(std::string value){
    check(std::regex_match(value,std::regex("0x[0-9a-fA-F]{40}")),"Invalid EVM address");std::string body=lower(value.substr(2));auto hash=hex(keccak(Bytes(body.begin(),body.end())));std::string out="0x";
    for(size_t i=0;i<40;++i){char c=body[i];if(c>='a'&&c<='f'&&hash[i]>='8')c=static_cast<char>(std::toupper(c));out+=c;}
    return out;
}
static std::string address(const Bytes& secret){auto k=key(secret);Bytes pub(65);check(EC_POINT_point2oct(EC_KEY_get0_group(k.get()),EC_KEY_get0_public_key(k.get()),POINT_CONVERSION_UNCOMPRESSED,pub.data(),pub.size(),nullptr)==65,"Public key encoding failed");pub.erase(pub.begin());auto hash=keccak(pub);return checksum("0x"+hex(Bytes(hash.end()-20,hash.end())));}
static std::string validAddress(const std::string& value){auto normalized=checksum(value);auto body=value.substr(2);auto low=lower(body);std::string upper=body;for(auto& c:upper)c=static_cast<char>(std::toupper(c));check(body==low||body==upper||value==normalized,"EVM address checksum mismatch");check(unhex(value)!=Bytes(20,0),"Zero address rejected");return normalized;}
 
 
static Point recover(const EC_GROUP* group,const Bytes& digest,const BIGNUM* r,const BIGNUM* s,unsigned parity){
    Context ctx(BN_CTX_new(),BN_CTX_free);check(bool(ctx),"BN context failed");auto order=bn();check(EC_GROUP_get_order(group,order.get(),ctx.get())==1,"Curve order failed");
    Point point(EC_POINT_new(group),EC_POINT_free),q(EC_POINT_new(group),EC_POINT_free),multiple(EC_POINT_new(group),EC_POINT_free);check(bool(point)&&bool(q)&&bool(multiple),"Curve allocation failed");
    if(EC_POINT_set_compressed_coordinates(group,point.get(),r,parity,ctx.get())!=1)return Point(nullptr,EC_POINT_free);
    check(EC_POINT_mul(group,multiple.get(),nullptr,point.get(),order.get(),ctx.get())==1,"Point order check failed");if(!EC_POINT_is_at_infinity(group,multiple.get()))return Point(nullptr,EC_POINT_free);
    auto e=fromBytes(digest);auto negative=bn();auto rinv=bn();auto scalarG=bn();auto scalarR=bn();
    check(BN_mod_sub(negative.get(),order.get(),e.get(),order.get(),ctx.get())==1&&BN_mod_inverse(rinv.get(),r,order.get(),ctx.get())!=nullptr,"Recovery scalar failed");
    check(BN_mod_mul(scalarG.get(),negative.get(),rinv.get(),order.get(),ctx.get())==1&&BN_mod_mul(scalarR.get(),s,rinv.get(),order.get(),ctx.get())==1,"Recovery multiplication failed");
    check(EC_POINT_mul(group,q.get(),scalarG.get(),point.get(),scalarR.get(),ctx.get())==1,"Public key recovery failed");return q;
}
struct Signed{Bytes raw;std::string hash;};
static Signed sign(const std::vector<Bytes>& base,uint64_t chain,const Bytes& secret){
    check(base.size()==6&&chain<=1000000,"Invalid transaction fields");auto k=key(secret);auto fields=base;fields.push_back(integer(chain));fields.push_back({});fields.push_back({});auto digest=keccak(list(fields));
    const auto* group=EC_KEY_get0_group(k.get());auto order=bn();auto half=bn();check(EC_GROUP_get_order(group,order.get(),nullptr)==1&&BN_rshift1(half.get(),order.get())==1,"Curve order failed");
    for(unsigned attempt=0;attempt<8;++attempt){Signature sig(ECDSA_do_sign(digest.data(),32,k.get()),ECDSA_SIG_free);check(bool(sig),"ECDSA signing failed");const BIGNUM* oldR=nullptr;const BIGNUM* oldS=nullptr;ECDSA_SIG_get0(sig.get(),&oldR,&oldS);BN r(BN_dup(oldR),BN_clear_free),s(BN_dup(oldS),BN_clear_free);check(bool(r)&&bool(s),"Signature allocation failed");if(BN_cmp(s.get(),half.get())>0)check(BN_sub(s.get(),order.get(),s.get())==1,"Low-S normalization failed");
        int parity=-1;for(unsigned p=0;p<2;++p){auto recovered=recover(group,digest,r.get(),s.get(),p);if(recovered&&EC_POINT_cmp(group,recovered.get(),EC_KEY_get0_public_key(k.get()),nullptr)==0){parity=static_cast<int>(p);break;}}
        if(parity<0)continue;  
        Signature normalized(ECDSA_SIG_new(),ECDSA_SIG_free);check(bool(normalized),"Signature allocation failed");BIGNUM* rr=BN_dup(r.get());BIGNUM* ss=BN_dup(s.get());if(!rr||!ss||ECDSA_SIG_set0(normalized.get(),rr,ss)!=1){BN_clear_free(rr);BN_clear_free(ss);throw std::runtime_error("Signature setup failed");}
        check(ECDSA_do_verify(digest.data(),32,normalized.get(),k.get())==1,"Local signature verification failed");
        auto out=base;out.push_back(integer(chain*2+35+static_cast<unsigned>(parity)));out.push_back(bigBytes(r.get()));out.push_back(bigBytes(s.get()));auto raw=list(out);return {raw,"0x"+hex(keccak(raw))};
    }throw std::runtime_error("Could not produce a recoverable EVM signature");
}
static json_object* field(json_object* o,const char* name){json_object* value=nullptr;check(o&&json_object_get_type(o)==json_type_object&&json_object_object_get_ex(o,name,&value)&&value,"Missing keystore/RPC field");return value;}
static std::string string(json_object* v){check(v&&json_object_get_type(v)==json_type_string,"Expected RPC/keystore string");return json_object_get_string(v);}
static Bytes aes(const Bytes& input,const Bytes& derived,const Bytes& iv){check(derived.size()==32&&iv.size()==16&&input.size()==32,"Invalid keystore cipher parameters");Owner<EVP_CIPHER_CTX,EVP_CIPHER_CTX_free> ctx(EVP_CIPHER_CTX_new(),EVP_CIPHER_CTX_free);check(bool(ctx),"Cipher allocation failed");Bytes out(48);int size=0,tail=0;check(EVP_EncryptInit_ex(ctx.get(),EVP_aes_128_ctr(),nullptr,derived.data(),iv.data())==1&&EVP_EncryptUpdate(ctx.get(),out.data(),&size,input.data(),32)==1&&EVP_EncryptFinal_ex(ctx.get(),out.data()+size,&tail)==1,"AES-CTR operation failed");out.resize(size+tail);check(out.size()==32,"Invalid cipher result");return out;}
static Bytes derive(const std::string& password,json_object* crypto){
    auto* params=field(crypto,"kdfparams");auto salt=unhex(string(field(params,"salt")));check(salt.size()>=16&&salt.size()<=64&&number(params,"dklen")==32,"Invalid keystore KDF parameters");Bytes result(32);auto kdf=textField(crypto,"kdf");
    if(kdf=="pbkdf2"){uint64_t count=number(params,"c");check(count>=10000&&count<=10000000&&textField(params,"prf")=="hmac-sha256","Unsupported PBKDF2 parameters");check(PKCS5_PBKDF2_HMAC(password.c_str(),static_cast<int>(password.size()),salt.data(),static_cast<int>(salt.size()),static_cast<int>(count),EVP_sha256(),32,result.data())==1,"PBKDF2 failed");}
    else if(kdf=="scrypt"){auto n=number(params,"n"),r=number(params,"r"),p=number(params,"p");check(n>=2&&n<=262144&&(n&(n-1))==0&&r>=1&&r<=8&&p>=1&&p<=16&&n*r*p<=16777216,"Unsupported scrypt resource parameters");check(EVP_PBE_scrypt(password.data(),password.size(),salt.data(),salt.size(),n,r,p,512ULL*1024*1024,result.data(),result.size())==1,"scrypt failed");}
    else throw std::runtime_error("Unsupported keystore KDF");
    return result;
}
static Bytes mac(const Bytes& derived,const Bytes& ciphertext){Bytes data(derived.begin()+16,derived.end());data.insert(data.end(),ciphertext.begin(),ciphertext.end());auto result=keccak(data);OPENSSL_cleanse(data.data(),data.size());return result;}
static JsonPtr encrypt(const Bytes& secret,const std::string& password){
    auto out=obj(),crypto=obj(),params=obj(),cipherparams=obj();auto salt=random(32),iv=random(16);str(params.get(),"salt",hex(salt));str(params.get(),"prf","hmac-sha256");json_object_object_add(params.get(),"c",venera_json_new_uint64(600000));json_object_object_add(params.get(),"dklen",venera_json_new_uint64(32));str(crypto.get(),"kdf","pbkdf2");json_object_object_add(crypto.get(),"kdfparams",params.release());Secret derived(derive(password,crypto.get()));auto ciphertext=aes(secret,derived.bytes,iv);str(crypto.get(),"cipher","aes-128-ctr");str(cipherparams.get(),"iv",hex(iv));json_object_object_add(crypto.get(),"cipherparams",cipherparams.release());str(crypto.get(),"ciphertext",hex(ciphertext));str(crypto.get(),"mac",hex(mac(derived.bytes,ciphertext)));json_object_object_add(out.get(),"crypto",crypto.release());str(out.get(),"address",lower(address(secret).substr(2)));json_object_object_add(out.get(),"version",venera_json_new_uint64(3));auto uuid=random(16);uuid[6]=(uuid[6]&15)|64;uuid[8]=(uuid[8]&63)|128;auto h=hex(uuid);str(out.get(),"id",h.substr(0,8)+"-"+h.substr(8,4)+"-"+h.substr(12,4)+"-"+h.substr(16,4)+"-"+h.substr(20));return out;
}
static Bytes decrypt(json_object* root,const std::string& password){
    check(number(root,"version")==3,"Expected Ethereum V3 keystore");json_object* crypto=nullptr;if(!json_object_object_get_ex(root,"crypto",&crypto))crypto=field(root,"Crypto");check(textField(crypto,"cipher")=="aes-128-ctr","Unsupported keystore cipher");auto cipher=unhex(string(field(crypto,"ciphertext"))),expected=unhex(string(field(crypto,"mac")));check(cipher.size()==32&&expected.size()==32,"Invalid keystore sizes");Secret derived(derive(password,crypto));auto actual=mac(derived.bytes,cipher);check(CRYPTO_memcmp(actual.data(),expected.data(),32)==0,"Incorrect password or corrupted keystore");auto iv=unhex(string(field(field(crypto,"cipherparams"),"iv")));auto secret=aes(cipher,derived.bytes,iv);try{check(lower(address(secret).substr(2))==lower(textField(root,"address")),"Keystore address does not match private key");}catch(...){OPENSSL_cleanse(secret.data(),secret.size());throw;}return secret;
}
struct Password{std::string value;explicit Password(std::string v):value(std::move(v)){} Password(const Password&)=delete;Password& operator=(const Password&)=delete;~Password(){cleanse(value);}};
static const char* DEFAULT_RPC="https://bsc-dataseed.binance.org/";
static const char* USDT="0x55d398326f99059fF775485246999027B3197955";
static const char* USDC="0x8AC76a51cc950d9822D68b83fE1Ad97B32Cd580d";
static std::string rpcUrl(json_object* config){
#ifdef VENERA_TEST_EXTERNAL_RPC
    return textField(config,"bsc_rpc",DEFAULT_RPC);  
#else
    (void)config;return DEFAULT_RPC;
#endif
}
static void validateRpcUrl(const std::string& url){
    check(url.size()<4096&&url.find_first_of(" \r\n\t@#")==std::string::npos,"Invalid BSC RPC URL");
    Owner<CURLU,curl_url_cleanup> parsed(curl_url(),curl_url_cleanup);check(bool(parsed)&&curl_url_set(parsed.get(),CURLUPART_URL,url.c_str(),0)==CURLUE_OK,"Invalid BSC RPC URL");
    char* rawScheme=nullptr;char* rawHost=nullptr;check(curl_url_get(parsed.get(),CURLUPART_SCHEME,&rawScheme,0)==CURLUE_OK,"Missing URL scheme");std::string scheme(rawScheme);curl_free(rawScheme);check(curl_url_get(parsed.get(),CURLUPART_HOST,&rawHost,0)==CURLUE_OK,"Missing URL host");std::string host(rawHost);curl_free(rawHost);
    check(scheme=="https"||(scheme=="http"&&(host=="127.0.0.1"||host=="localhost"||host=="[::1]")),"Remote BSC RPC requires HTTPS");
}
static JsonPtr array(){return JsonPtr(json_object_new_array());}
static JsonPtr params(std::initializer_list<std::string> strings){auto result=array();for(auto& s:strings)json_object_array_add(result.get(),json_object_new_string(s.c_str()));return result;}
static JsonPtr request(Rpc& rpc,const std::string& method,json_object* p){return rpc.call(method,p,"",true);}
static void chain(Rpc& rpc){auto p=array();auto result=request(rpc,"eth_chainId",p.get());check(smallQuantity(string(result.get()))==56,"Expected BSC mainnet chain ID 56");}
static void checkRpcConnection(const std::string& url){
    std::cout<<"BSC public RPC check; no wallet loaded.\n"<<"Transport: "<<curl_version()<<'\n'
        <<"Validating URL..."<<std::endl;
    validateRpcUrl(url);
    std::cout<<"Requesting eth_chainId over verified transport (DNS/connect/TLS)..."<<std::endl;
    Rpc rpc(url,"","",30);chain(rpc);
    std::cout<<"BSC RPC OK; chain ID 56.\n";
}
static std::string call(Rpc& rpc,const std::string& to,const std::string& data,const std::string& block="latest"){
    auto p=array(),transaction=obj();str(transaction.get(),"to",to);str(transaction.get(),"data",data);json_object_array_add(p.get(),transaction.release());json_object_array_add(p.get(),json_object_new_string(block.c_str()));auto result=request(rpc,"eth_call",p.get());return string(result.get());
}
struct Token{std::string address;unsigned decimals;};
static Token token(Rpc& rpc,const std::string& symbol){
    std::string contract=symbol=="USDT"?USDT:USDC;unsigned decimals=18;
    contract=validAddress(contract);auto p=params({contract,"latest"});auto code=request(rpc,"eth_getCode",p.get());check(!unhex(string(code.get())).empty(),"Token has no contract code on this chain");
    check(smallQuantity(call(rpc,contract,"0x313ce567"))==decimals,"On-chain token decimals differ from configuration");return {contract,decimals};
}
static std::string word(const Bytes& value){check(value.size()<=32,"ABI word overflow");return std::string((32-value.size())*2,'0')+hex(value);}
static BN tokenBalance(Rpc& rpc,const Token& t,const std::string& owner){return parseQuantity(call(rpc,t.address,"0x70a08231"+word(unhex(owner))));}
static BN nativeBalance(Rpc& rpc,const std::string& owner,const std::string& block){auto p=params({owner,block});auto result=request(rpc,"eth_getBalance",p.get());return parseQuantity(string(result.get()));}
static void send(Rpc& rpc,json_object*,const std::vector<std::string>& args,const Bytes& secret){
     
    check(args.size()==4,"Usage: send BNB|USDT|USDC ADDRESS AMOUNT");std::string symbol=args[1];for(auto& c:symbol)c=static_cast<char>(std::toupper(c));check(symbol=="BNB"||symbol=="USDT"||symbol=="USDC","Unsupported BSC asset");
    auto sender=address(secret);auto recipient=validAddress(args[2]);bool native=symbol=="BNB";Token asset{"",18};if(!native)asset=token(rpc,symbol);auto value=units(args[3],asset.decimals);auto n=fromBytes(value);
    std::string data="0x";if(!native){auto available=tokenBalance(rpc,asset,sender);check(BN_cmp(available.get(),n.get())>=0,"Insufficient token balance");data="0xa9059cbb"+word(unhex(recipient))+word(value);}
    auto nonceParams=params({sender,"pending"}),empty=array();auto nonceResult=request(rpc,"eth_getTransactionCount",nonceParams.get()),priceResult=request(rpc,"eth_gasPrice",empty.get());auto nonce=smallQuantity(string(nonceResult.get())),price=smallQuantity(string(priceResult.get()));check(price>0&&price<=10000000000000ULL,"RPC gas price outside safety limit");
    auto transaction=obj();str(transaction.get(),"from",sender);str(transaction.get(),"to",native?recipient:asset.address);str(transaction.get(),"value",native?hexQuantity(value):"0x0");str(transaction.get(),"gasPrice",quantity(price));str(transaction.get(),"data",data);
    auto estimateParams=array();json_object_array_add(estimateParams.get(),json_object_get(transaction.get()));auto estimate=request(rpc,"eth_estimateGas",estimateParams.get());auto estimatedGas=smallQuantity(string(estimate.get()));check(estimatedGas>=21000&&estimatedGas<=4166666,"RPC gas estimate outside safety limit");auto gas=(estimatedGas*120+99)/100;
    auto fee=fromBytes(integer(gas)),priceBN=fromBytes(integer(price));Context ctx(BN_CTX_new(),BN_CTX_free);check(bool(ctx)&&BN_mul(fee.get(),fee.get(),priceBN.get(),ctx.get())==1,"Fee arithmetic failed");auto needed=bn();check(BN_copy(needed.get(),fee.get()),"Fee copy failed");if(native)check(BN_add(needed.get(),needed.get(),n.get())==1,"Value arithmetic failed");auto bnb=nativeBalance(rpc,sender,"pending");check(BN_cmp(bnb.get(),needed.get())>=0,"Insufficient BNB for value and maximum gas fee");
     
     
    str(transaction.get(),"gas",quantity(gas));
    if(!native){auto simulation=array();json_object_array_add(simulation.get(),json_object_get(transaction.get()));json_object_array_add(simulation.get(),json_object_new_string("pending"));auto result=request(rpc,"eth_call",simulation.get());auto response=string(result.get());check(response=="0x"||smallQuantity(response)==1,"Token transfer simulation returned false");}
    std::cout<<ansi::cyan<<"BSC mainnet (56)\nFrom: "<<sender<<"\nTo: "<<recipient<<"\nSend: "<<display(n.get(),asset.decimals)<<' '<<symbol<<"\nContract: "<<(native?"native BNB":asset.address)<<"\nGas estimate: "<<estimatedGas<<"\nGas limit: "<<gas<<"\nGas price: "<<display(priceBN.get(),9)<<" gwei\nMaximum gas fee: "<<display(fee.get(),18)<<" BNB\nNonce: "<<nonce<<ansi::reset<<'\n';
    if(!yesNo("Sign and broadcast this BSC transaction?",false)){std::cout<<ansi::yellow<<"Cancelled; nothing signed or broadcast."<<ansi::reset<<'\n';return;}
    std::vector<Bytes> base={integer(nonce),integer(price),integer(gas),unhex(native?recipient:asset.address),native?value:Bytes{},unhex(data)};auto signedTx=sign(base,56,secret);chain(rpc);
    std::cout<<"Signed transaction hash: "<<signedTx.hash<<'\n'<<std::flush;auto broadcast=params({"0x"+hex(signedTx.raw)});
    try{auto result=request(rpc,"eth_sendRawTransaction",broadcast.get());check(lower(string(result.get()))==signedTx.hash,"RPC returned a different transaction hash");}
    catch(...){std::cerr<<"Broadcast outcome uncertain. Check evm receipt "<<signedTx.hash<<" before retrying.\n";throw;}
    std::cout<<ansi::green<<"Broadcast accepted. Check receipt HASH for confirmation."<<ansi::reset<<'\n';
}
 
 
static Bytes fromVnrSpendKey(const Bytes& spend) {
    check(spend.size()==32,"Invalid VNR spend key");
    for(unsigned counter=0;counter<256;++counter) {
        const std::string domain="Venera BSC account v1:"+std::to_string(counter);
        Bytes result(32);unsigned length=0;
        check(HMAC(EVP_sha256(),spend.data(),static_cast<int>(spend.size()),
            reinterpret_cast<const unsigned char*>(domain.data()),domain.size(),result.data(),&length)&&length==32,"BSC key derivation failed");
        try { key(result); return result; } catch(...) { OPENSSL_cleanse(result.data(),result.size()); }
    }
    throw std::runtime_error("Cannot derive a BSC account");
}
static void command(const std::vector<std::string>& args,json_object* config,const fs::path&,const std::string& publicAddress,const Bytes* secret=nullptr){
    check(!args.empty(),"Use balance/send/receipt");auto cmd=lower(args[0]);
    check(cmd=="address"||cmd=="balance"||cmd=="send"||cmd=="receipt","Unknown BSC command");
    if(cmd=="address"){check(args.size()==1,"Usage: receive BNB|USDT|USDC");std::cout<<validAddress(publicAddress)<<'\n';return;}
    std::string url=rpcUrl(config);
    validateRpcUrl(url);Rpc rpc(url,"","",30);
    if(cmd=="balance"){
        check(args.size()<=2,"Usage: balance BNB|USDT|USDC|all");auto selected=args.size()==2?lower(args[1]):"all";
        check(selected=="all"||selected=="bnb"||selected=="usdt"||selected=="usdc","Unknown BSC asset");
        auto owner=validAddress(publicAddress);std::cout<<ansi::cyan<<"BSC address: "<<owner<<ansi::reset<<std::endl;
        chain(rpc);
        if(selected=="all"||selected=="bnb"){auto balance=nativeBalance(rpc,owner,"latest");std::cout<<ansi::green<<"BNB: "<<display(balance.get(),18)<<ansi::reset<<'\n';}
        for(const auto* symbol:{"USDT","USDC"})if(selected=="all"||selected==lower(symbol))try{auto asset=token(rpc,symbol);auto value=tokenBalance(rpc,asset,owner);std::cout<<ansi::green<<symbol<<": "<<display(value.get(),asset.decimals)<<ansi::reset<<'\n';}catch(const std::exception& e){std::cout<<ansi::yellow<<symbol<<": "<<e.what()<<ansi::reset<<'\n';}
        return;
    }
    chain(rpc);
    if(cmd=="send"){check(secret!=nullptr,"Sending requires a password-unlocked signer");check(address(*secret)==publicAddress,"Signing key does not match selected account");send(rpc,config,args,*secret);return;}
    if(cmd=="receipt"){check(args.size()==2&&std::regex_match(args[1],std::regex("0x[0-9a-fA-F]{64}")),"Usage: receipt 0xHASH");auto p=params({args[1]});auto receipt=request(rpc,"eth_getTransactionReceipt",p.get());if(!receipt){std::cout<<"No receipt yet (pending, unknown or dropped).\n";return;}check(json_object_get_type(receipt.get())==json_type_object,"Invalid receipt");std::cout<<(smallQuantity(string(field(receipt.get(),"status")))==1?"Success":"FAILED (gas may still be charged)")<<'\n'<<json_object_to_json_string_ext(receipt.get(),JSON_C_TO_STRING_PRETTY)<<'\n';return;}
    throw std::runtime_error("Unknown BSC command");
}
static void selfTest(){
#ifndef VENERA_TEST_EXTERNAL_RPC
    auto legacyConfig=obj();str(legacyConfig.get(),"bsc_rpc","http://127.0.0.1:1");
    check(rpcUrl(legacyConfig.get())==DEFAULT_RPC,"Production BSC endpoint must be fixed");
#endif
    check(hex(fromVnrSpendKey(Bytes(32,1)))=="3cf65b26356abddf51277dd9411696f4f0b89a2662119a4da1f96c04340a08b4","VNR/BSC account derivation vector failed");
    check(hex(keccak({}))=="c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470","Keccak empty vector failed");check(hex(keccak(Bytes{'a','b','c'}))=="4e03657aea45a94fc7d47ba826c8d667c0d1e6e33a64a036ec44f58fa12d6c45","Keccak abc vector failed");
    check(address(unhex(std::string(63,'0')+"1"))=="0x7E5F4552091A69125d5DfCb7b8C2659029395Bdf","secp256k1 address vector failed");
    check(checksum(lower(USDT))==USDT&&checksum(lower(USDC))==USDC,"Default token checksum failed");
    auto value=units("1.000000000000000001",18);auto n=fromBytes(value);check(display(n.get(),18)=="1.000000000000000001","Exact amount test failed");
    std::vector<Bytes> base={integer(9),integer(20000000000ULL),integer(21000),unhex("3535353535353535353535353535353535353535"),units("1",18),{}};
    auto signing=base;signing.push_back(integer(1));signing.push_back({});signing.push_back({});auto digest=keccak(list(signing));check(hex(digest)=="daf5a779ae972f972197303d7b574746c7ef83eadac0f2791ad23db92e4c8e53","EIP-155 signing hash vector failed");
    auto secret=unhex(std::string(64,'4'));for(auto& b:secret)b=0x46;Secret testSecret(std::move(secret));auto k=key(testSecret.bytes);auto r=fromBytes(unhex("28ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276")),s=fromBytes(unhex("67cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83"));auto recovered=recover(EC_KEY_get0_group(k.get()),digest,r.get(),s.get(),0);check(recovered&&EC_POINT_cmp(EC_KEY_get0_group(k.get()),recovered.get(),EC_KEY_get0_public_key(k.get()),nullptr)==0,"EIP-155 signature recovery vector failed");
    for(unsigned i=0;i<12;++i){auto result=sign(base,56,testSecret.bytes);check(!result.raw.empty()&&result.hash.size()==66,"Native recoverable signing failed");}
    auto encrypted=encrypt(testSecret.bytes,"self-test password only");Secret decrypted(decrypt(encrypted.get(),"self-test password only"));check(decrypted.bytes==testSecret.bytes,"V3 keystore roundtrip failed");bool rejected=false;try{Secret wrong(decrypt(encrypted.get(),"incorrect password"));}catch(...){rejected=true;}check(rejected,"Keystore accepted wrong password");
    std::cout<<"Native EVM self-test passed: Keccak, address/checksum, EIP-155, recoverable low-S signatures, exact amounts and V3 keystore.\n";
}
}  
