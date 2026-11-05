#pragma once
#include <future>
#include <map>
                                                                                
namespace desktop {
static constexpr const char* STAKING_ADDRESS="VENeRa1zWhh2AWLi78SUg2HApA6SF69cKRTNsf7YrKxZSmXMCe1mSZyT41SPV8u1h2KJEeSCB6B4iUzHSVeAqwEkYYummpduFj218";
static void boolean(json_object* o,const char* k,bool v){json_object_object_add(o,k,json_object_new_boolean(v));}
static void scrub(json_object* o,const char* k){json_object* v=nullptr;if(o&&json_object_object_get_ex(o,k,&v)&&v&&json_object_get_type(v)==json_type_string){auto* p=const_cast<char*>(json_object_get_string(v));if(p)OPENSSL_cleanse(p,std::strlen(p));}}
static void precise(json_object* o){
    if(!o)return;
    if(json_object_get_type(o)==json_type_array){for(size_t i=0;i<json_object_array_length(o);++i)precise(json_object_array_get_idx(o,i));return;}
    if(json_object_get_type(o)!=json_type_object)return;
    for(const char* key:{"balance","unlocked_balance","amount","fee","amount_burnt","amount_burned_atomic"}){json_object* v=nullptr;if(json_object_object_get_ex(o,key,&v)&&v&&json_object_get_type(v)==json_type_int)str(o,key,std::to_string(venera_json_get_uint64(v)));}
    for(const char* key:{"in","out","pending","failed","pool","transfers","transfer","destinations"}){json_object* v=nullptr;if(json_object_object_get_ex(o,key,&v))precise(v);}
}
class Service {
    WalletService service_;fs::path file_;std::string vnr_,bsc_,daemon_="http://127.0.0.1:29930";const char* cli_;
    evm::Secret evmKey_{evm::Bytes{}};JsonPtr pending_{nullptr};std::vector<evm::Bytes> evmBase_;uint64_t evmNonce_=0;
    std::future<void> scan_;std::atomic<bool> scanning_{false};JsonPtr lastBalance_{nullptr};std::map<std::string,JsonPtr> memoryReceipts_,memoryRewards_;
    void drainScan(){if(scan_.valid()){if(scanning_)wallet_interrupts::desktopCancel.store(true);scan_.wait();try{scan_.get();}catch(...){}wallet_interrupts::desktopCancel.store(false);}scanning_=false;}
    Rpc& wallet(){evm::check(bool(service_.rpc),"Wallet is locked");return *service_.rpc;}
    void checkPassword(json_object* p){evm::Password pass(textField(p,"password"));evm::Password seed(decryptWalletFile(file_.string(),pass.value));evm::check(splitWords(seed.value).size()==25,"Invalid wallet file");}
    JsonPtr accounts(){auto o=obj();str(o.get(),"vnr",vnr_);str(o.get(),"evm",bsc_);str(o.get(),"file",file_.string());return o;}
    void saveReceipt(json_object* value){memoryReceipts_[file_.string()+":"+textField(value,"id")]=parse(encode(value));}
    json_object* memoryQueue(){auto& q=memoryRewards_[file_.string()];if(!q){q=obj();str(q.get(),"wallet_address",vnr_);json_object_object_add(q.get(),"pending",json_object_new_array());}return q.get();}
    void enqueueMemory(const std::string& action,const std::string& tx,unsigned days){rewards::hash(tx);auto q=memoryQueue();json_object* list=nullptr;json_object_object_get_ex(q,"pending",&list);for(size_t i=0;i<json_object_array_length(list);++i){auto item=json_object_array_get_idx(list,i);if(textField(item,"txid")==tx){evm::check(textField(item,"action")==action&&number(item,"lock_days")==days,"Transaction receipt terms differ");return;}}auto item=obj();str(item.get(),"action",action);str(item.get(),"txid",tx);json_object_object_add(item.get(),"lock_days",venera_json_new_uint64(days));json_object_array_add(list,item.release());}

public:
    explicit Service(const char* cli):cli_(cli){}
    ~Service(){lock();}
    void lock(){drainScan();lastBalance_.reset();if(service_.rpc){try{service_.rpc->call("store",obj().get());}catch(...){}}service_.stop();pending_.reset();evmBase_.clear();if(!evmKey_.bytes.empty())OPENSSL_cleanse(evmKey_.bytes.data(),evmKey_.bytes.size());evmKey_.bytes.clear();vnr_.clear();bsc_.clear();file_.clear();}
    JsonPtr open(const std::string& mode,json_object* p){
        lock();file_=fs::absolute(textField(p,"file"));evm::check(file_.extension()==".keys"&&!file_.filename().empty(),"Choose a .keys wallet file");
        evm::Password pass(textField(p,"password")),seed(textField(p,"seed"));evm::check(!pass.value.empty()&&pass.value.size()<=1024,"Enter a wallet password");
        evm::check(std::all_of(pass.value.begin(),pass.value.end(),[](unsigned char c){return c>=32&&c<=126;}),"Use printable ASCII for CLI-compatible passwords");
        const bool creating=mode=="create",importing=mode=="import";
        evm::check(!(creating||importing)||!fs::exists(file_),"Wallet file already exists; choose another name");
        if(!creating&&!importing)seed.value=decryptWalletFile(file_.string(),pass.value);
        if(!creating)evm::check(splitWords(trim(seed.value)).size()==25,"Expected a 25-word VNR seed");
        auto config=obj();const auto cache=fs::path(file_.string()+".cache");
        try{
            service_.start(config.get(),cache,daemon_,cli_,true);auto req=obj();str(req.get(),"filename","wallet");str(req.get(),"password",pass.value);
            if(creating){str(req.get(),"language","English");wallet().call("create_wallet",req.get());seed.value=queryWalletSecret(wallet(),"mnemonic");encryptWalletFile(file_,seed.value,pass.value);}
            else if(fs::exists(cache/"wallet.keys")){wallet().call("open_wallet",req.get());evm::Password cached(queryWalletSecret(wallet(),"mnemonic"));evm::check(splitWords(lower(cached.value))==splitWords(lower(seed.value)),"Wallet cache does not match .keys file");}
            else{str(req.get(),"seed",trim(seed.value));json_object_object_add(req.get(),"restore_height",venera_json_new_uint64(0));wallet().call("restore_deterministic_wallet",req.get());if(importing)encryptWalletFile(file_,seed.value,pass.value);}
            auto address=wallet().call("get_address",obj().get());vnr_=textField(address.get(),"address");
            evm::Password spend(queryWalletSecret(wallet(),"spend_key"));evm::Secret bytes(evm::unhex(spend.value));evmKey_.bytes=evm::fromVnrSpendKey(bytes.bytes);bsc_=evm::address(evmKey_.bytes);
            auto refresh=obj();boolean(refresh.get(),"enable",false);wallet().call("auto_refresh",refresh.get());wallet().call("store",obj().get());
            auto out=accounts();if(creating)str(out.get(),"seed",seed.value);return out;
        }catch(...){lock();throw;}
    }
    JsonPtr balance(){
        wallet();
        if(scanning_){auto result=lastBalance_?parse(encode(lastBalance_.get())):accounts();if(!lastBalance_){str(result.get(),"balance","0");str(result.get(),"unlocked","0");json_object_object_add(result.get(),"height",venera_json_new_uint64(0));}boolean(result.get(),"scanning",true);return result;}
        if(scan_.valid())scan_.get();
        auto b=wallet().call("get_balance",obj().get()),h=wallet().call("get_height",obj().get());auto o=accounts();str(o.get(),"balance",amount(number(b.get(),"balance")));str(o.get(),"unlocked",amount(number(b.get(),"unlocked_balance")));json_object_object_add(o.get(),"height",venera_json_new_uint64(number(h.get(),"height")));boolean(o.get(),"scanning",false);lastBalance_=parse(encode(o.get()));return o;
    }
    JsonPtr startScan(){
        wallet();if(scanning_)return balance();if(scan_.valid())scan_.get();auto current=balance();Rpc daemon(daemon_,"","",5);auto info=daemon.call("get_info",obj().get());if(number(current.get(),"height")>=number(info.get(),"height"))return current;scanning_=true;
        scan_=std::async(std::launch::async,[this]{try{wallet().call("refresh",obj().get());wallet().call("store",obj().get());scanning_=false;}catch(...){scanning_=false;throw;}});
        return balance();
    }
    JsonPtr evmBalances(){evm::check(!bsc_.empty(),"Wallet is locked");Rpc rpc(evm::DEFAULT_RPC,"","",30);evm::chain(rpc);auto o=obj();auto b=evm::nativeBalance(rpc,bsc_,"latest");str(o.get(),"bnb",evm::display(b.get(),18));for(const char* name:{"USDT","USDC"}){auto t=evm::token(rpc,name);auto n=evm::tokenBalance(rpc,t,bsc_);str(o.get(),lower(name).c_str(),evm::display(n.get(),t.decimals));}return o;}
    JsonPtr prepare(json_object* p){
        wallet();evm::check(!pending_,"Confirm or cancel the previous transaction first");
        auto id=evm::hex(evm::random(16)),asset=lower(textField(p,"asset")),action=lower(textField(p,"action","send")),value=textField(p,"amount"),to=textField(p,"recipient");
        evm::check(action=="send"||action=="burn"||action=="stake","Unsupported transaction action");
        if(action=="stake")evm::check(to==STAKING_ADDRESS,"Unexpected staking destination");
        auto pending=obj();str(pending.get(),"id",id);str(pending.get(),"asset",asset);str(pending.get(),"action",action);str(pending.get(),"amount",value);str(pending.get(),"recipient",to);
        if(asset=="vnr"){
            evm::check(!scanning_,"Wallet is still scanning");
            auto h=wallet().call("get_height",obj().get());Rpc daemon(daemon_,"","",5);auto info=daemon.call("get_info",obj().get());evm::check(number(h.get(),"height")>=number(info.get(),"height"),"Wallet scanning is not complete; refresh before spending");
            auto req=obj();boolean(req.get(),"do_not_relay",true);boolean(req.get(),"get_tx_metadata",true);
            if(action=="burn")json_object_object_add(req.get(),"amount",venera_json_new_uint64(atomicAmount(value)));
            else{auto valid=obj();str(valid.get(),"address",to);auto check=wallet().call("validate_address",valid.get());json_object* good=nullptr;evm::check(json_object_object_get_ex(check.get(),"valid",&good)&&json_object_get_int64(good),"Invalid VNR recipient");auto d=obj();str(d.get(),"address",to);json_object_object_add(d.get(),"amount",venera_json_new_uint64(atomicAmount(value)));auto list=json_object_new_array();json_object_array_add(list,d.release());json_object_object_add(req.get(),"destinations",list);}
            auto result=wallet().call(action=="burn"?"burn":"transfer",req.get());
            auto meta=json_object_new_array(),hashes=json_object_new_array();uint64_t fee=0;
            if(action=="burn"){json_object *m=nullptr,*hs=nullptr,*fs=nullptr;evm::check(json_object_object_get_ex(result.get(),"tx_metadata_list",&m)&&json_object_object_get_ex(result.get(),"tx_hash_list",&hs)&&json_object_object_get_ex(result.get(),"fee_list",&fs)&&json_object_array_length(m)>0&&json_object_array_length(m)==json_object_array_length(hs),"Missing burn transaction metadata");for(size_t i=0;i<json_object_array_length(m);++i){json_object_array_add(meta,json_object_get(json_object_array_get_idx(m,i)));json_object_array_add(hashes,json_object_get(json_object_array_get_idx(hs,i)));auto f=venera_json_get_uint64(json_object_array_get_idx(fs,i));evm::check(fee<=UINT64_MAX-f,"Fee overflow");fee+=f;}}
            else{json_object_array_add(meta,json_object_new_string(textField(result.get(),"tx_metadata").c_str()));json_object_array_add(hashes,json_object_new_string(textField(result.get(),"tx_hash").c_str()));fee=number(result.get(),"fee");}
            json_object_object_add(pending.get(),"metadata",meta);json_object_object_add(pending.get(),"hashes",hashes);str(pending.get(),"fee",amount(fee));
        }else{
            evm::check(action=="send"&&(asset=="bnb"||asset=="usdt"||asset=="usdc"),"Unsupported BSC transfer");to=evm::validAddress(to);str(pending.get(),"recipient",to);Rpc rpc(evm::DEFAULT_RPC,"","",30);evm::chain(rpc);bool native=asset=="bnb";evm::Token token{"",18};if(!native)token=evm::token(rpc,asset=="usdt"?"USDT":"USDC");auto units=evm::units(value,token.decimals);auto n=evm::fromBytes(units);
            if(!native){auto balance=evm::tokenBalance(rpc,token,bsc_);evm::check(BN_cmp(balance.get(),n.get())>=0,"Insufficient token balance");}
            auto np=evm::params({bsc_,"pending"}),empty=evm::array();auto nr=evm::request(rpc,"eth_getTransactionCount",np.get()),pr=evm::request(rpc,"eth_gasPrice",empty.get());evmNonce_=evm::smallQuantity(evm::string(nr.get()));auto price=evm::smallQuantity(evm::string(pr.get()));evm::check(price>0&&price<=10000000000000ULL,"Gas price exceeds safety limit");
            auto data=native?std::string("0x"):"0xa9059cbb"+evm::word(evm::unhex(to))+evm::word(units);auto tx=obj();str(tx.get(),"from",bsc_);str(tx.get(),"to",native?to:token.address);str(tx.get(),"value",native?evm::hexQuantity(units):"0x0");str(tx.get(),"gasPrice",evm::quantity(price));str(tx.get(),"data",data);auto ep=evm::array();json_object_array_add(ep.get(),json_object_get(tx.get()));auto estimate=evm::request(rpc,"eth_estimateGas",ep.get());auto gas=evm::smallQuantity(evm::string(estimate.get()));evm::check(gas>=21000&&gas<=4166666,"Gas estimate exceeds safety limit");gas=(gas*120+99)/100;
            auto fee=evm::fromBytes(evm::integer(gas)),pb=evm::fromBytes(evm::integer(price));evm::Context ctx(BN_CTX_new(),BN_CTX_free);evm::check(bool(ctx)&&BN_mul(fee.get(),fee.get(),pb.get(),ctx.get())==1,"Fee calculation failed");auto needed=evm::bn();BN_copy(needed.get(),fee.get());if(native)BN_add(needed.get(),needed.get(),n.get());auto balance=evm::nativeBalance(rpc,bsc_,"pending");evm::check(BN_cmp(balance.get(),needed.get())>=0,"Insufficient BNB for gas and value");
            str(tx.get(),"gas",evm::quantity(gas));if(!native){auto sim=evm::array();json_object_array_add(sim.get(),json_object_get(tx.get()));json_object_array_add(sim.get(),json_object_new_string("pending"));auto r=evm::request(rpc,"eth_call",sim.get());auto s=evm::string(r.get());evm::check(s=="0x"||evm::smallQuantity(s)==1,"Token transfer simulation failed");}
            evmBase_={evm::integer(evmNonce_),evm::integer(price),evm::integer(gas),evm::unhex(native?to:token.address),native?units:evm::Bytes{},evm::unhex(data)};str(pending.get(),"fee",evm::display(fee.get(),18));str(pending.get(),"fee_asset","BNB");str(pending.get(),"contract",native?"native BNB":token.address);
        }
        json_object_object_add(pending.get(),"created_at",venera_json_new_uint64(std::time(nullptr)));json_object_object_add(pending.get(),"lock_days",venera_json_new_uint64(textField(p,"lock_days").empty()?0:std::stoul(textField(p,"lock_days"))));
        auto out=obj();for(const char* k:{"id","asset","action","amount","recipient","fee","fee_asset","contract"})str(out.get(),k,textField(pending.get(),k));pending_=std::move(pending);return out;
    }
    JsonPtr confirm(json_object* p){
        wallet();evm::check(bool(pending_)&&textField(p,"id")==textField(pending_.get(),"id"),"Unknown prepared transaction");evm::check(std::time(nullptr)<=number(pending_.get(),"created_at")+300,"Prepared transaction expired; cancel and prepare again");
        auto out=obj();auto asset=textField(pending_.get(),"asset"),action=textField(pending_.get(),"action");
        if(asset!="vnr"){
            Rpc rpc(evm::DEFAULT_RPC,"","",30);evm::chain(rpc);auto np=evm::params({bsc_,"pending"});auto nr=evm::request(rpc,"eth_getTransactionCount",np.get());evm::check(evm::smallQuantity(evm::string(nr.get()))==evmNonce_,"BSC nonce changed; cancel and prepare again");auto signedTx=evm::sign(evmBase_,56,evmKey_.bytes);str(pending_.get(),"raw_transaction","0x"+evm::hex(signedTx.raw));str(pending_.get(),"tx_hash",signedTx.hash);saveReceipt(pending_.get());auto params=evm::params({"0x"+evm::hex(signedTx.raw)});try{auto r=evm::request(rpc,"eth_sendRawTransaction",params.get());evm::check(lower(evm::string(r.get()))==signedTx.hash,"BSC RPC hash mismatch");str(out.get(),"status","broadcast");}catch(...){str(out.get(),"status","unknown");}str(out.get(),"tx_hash",signedTx.hash);
        }else{
            json_object *meta=nullptr,*hashes=nullptr;json_object_object_get_ex(pending_.get(),"metadata",&meta);json_object_object_get_ex(pending_.get(),"hashes",&hashes);saveReceipt(pending_.get());bool uncertain=false;
            if(action=="burn"||action=="stake")for(size_t i=0;i<json_object_array_length(hashes);++i)enqueueMemory(action,json_object_get_string(json_object_array_get_idx(hashes,i)),static_cast<unsigned>(number(pending_.get(),"lock_days")));
            for(size_t i=0;i<json_object_array_length(meta);++i){auto req=obj();str(req.get(),"hex",json_object_get_string(json_object_array_get_idx(meta,i)));try{wallet().call("relay_tx",req.get());}catch(...){uncertain=true;}}
            str(out.get(),"tx_hash",json_object_get_string(json_object_array_get_idx(hashes,0)));json_object_object_add(out.get(),"tx_hashes",json_object_get(hashes));str(out.get(),"status",uncertain?"unknown":"broadcast");try{wallet().call("store",obj().get());}catch(...){str(out.get(),"status","unknown");}
        }
        str(pending_.get(),"status",textField(out.get(),"status"));try{saveReceipt(pending_.get());}catch(...){str(out.get(),"status","unknown");}pending_.reset();evmBase_.clear();return out;
    }
    JsonPtr authorize(json_object* p){
        wallet();evm::check(!scanning_,"Wallet is still scanning; reward signatures wait for scanning");auto action=textField(p,"action"),nonce=textField(p,"nonce"),tx=textField(p,"txid"),recipient=lower(textField(p,"erc20_address")),id=textField(p,"request_id");
        evm::check(action=="profile"||action=="burn"||action=="stake"||action=="burn_stats"||action=="stake_stats"||action=="claim_burn_yield"||action=="claim_status","Invalid reward action");
        evm::check(textField(p,"audience")==rewards::API_ORIGIN&&textField(p,"wallet_address")==vnr_,"Reward intent does not match this wallet");
        auto expires=number(p,"expires_at"),days=number(p,"lock_days"),now=static_cast<uint64_t>(std::time(nullptr));evm::check(expires>=now&&expires<=now+360&&std::regex_match(nonce,std::regex("[0-9a-f]{64}")),"Invalid reward challenge");
        evm::check((action=="burn"||action=="stake")?std::regex_match(tx,std::regex("[0-9a-f]{64}")):tx.empty(),"Invalid transaction intent");evm::check(action=="stake"?(days==30||days==60||days==120||days==360):days==0,"Invalid lock days");
        evm::check((action=="profile"||action=="claim_burn_yield")?recipient==lower(bsc_):recipient.empty(),"Rewards must pay this wallet's local BSC address");evm::check((action=="claim_burn_yield"||action=="claim_status")?std::regex_match(id,std::regex("[0-9a-f]{32}")):id.empty(),"Invalid request ID");
        auto message=std::string("Venera local rewards v1 mainnet\n")+rewards::API_ORIGIN+"\n"+action+"\n"+vnr_+"\n"+tx+"\n"+std::to_string(days)+"\n"+recipient+"\n"+id+"\nNonce: "+nonce+"\nExpires: "+std::to_string(expires)+"\n";evm::check(message==textField(p,"message"),"Backend changed the signed request intent");
        auto sign=obj();str(sign.get(),"data",message);str(sign.get(),"signature_type","spend");auto sig=wallet().call("sign",sign.get());auto out=obj();str(out.get(),"wallet_signature",textField(sig.get(),"signature"));evm::check(textField(out.get(),"wallet_signature").rfind("SigV2",0)==0,"Updated wallet RPC signatures required");
        if(action=="burn"||action=="stake"){auto proof=obj();str(proof.get(),"txid",tx);str(proof.get(),"message",message);if(action=="stake")str(proof.get(),"address",STAKING_ADDRESS);auto r=wallet().call(action=="burn"?"get_spend_proof":"get_tx_proof",proof.get());str(out.get(),action=="burn"?"spend_proof":"tx_proof",textField(r.get(),"signature"));}return out;
    }
    JsonPtr call(const std::string& method,json_object* p){
        if(method=="ping"){auto o=obj();str(o.get(),"protocol","venera-desktop-2");return o;}
        if(method=="create"||method=="import"||method=="open")return open(method,p);
        if(method=="lock"){lock();return obj();}
        if(method=="accounts"){wallet();return accounts();}
        if(method=="balance")return balance();
        if(method=="evm_balance")return evmBalances();
        if(method=="refresh_async")return startScan();
        if(method=="refresh"){drainScan();auto r=wallet().call("refresh",obj().get());wallet().call("store",obj().get());return balance();}
        if(method=="history"){auto req=obj();for(const char* k:{"in","out","pending","failed","pool"})boolean(req.get(),k,true);auto r=wallet().call("get_transfers",req.get());precise(r.get());return r;}
        if(method=="prepare")return prepare(p);
        if(method=="confirm")return confirm(p);
        if(method=="cancel"){pending_.reset();evmBase_.clear();return obj();}
        if(method=="rewards_queue"){wallet();return parse(encode(memoryQueue()));}
        if(method=="ack_reward"){wallet();auto tx=textField(p,"txid");rewards::hash(tx);auto q=parse(encode(memoryQueue()));json_object* list=nullptr;json_object_object_get_ex(q.get(),"pending",&list);auto remaining=json_object_new_array();for(size_t i=0;i<json_object_array_length(list);++i){auto item=json_object_array_get_idx(list,i);if(textField(item,"txid")!=tx)json_object_array_add(remaining,json_object_get(item));}json_object_object_add(q.get(),"pending",remaining);memoryRewards_[file_.string()]=std::move(q);return obj();}
        if(method=="desktop_receipt"){wallet();auto out=obj();auto it=memoryReceipts_.find(file_.string()+":"+textField(p,"id"));if(it!=memoryReceipts_.end()){auto copy=parse(encode(it->second.get()));json_object_object_add(out.get(),"receipt",copy.release());}else json_object_object_add(out.get(),"receipt",nullptr);return out;}
        if(method=="receipt"){wallet();auto hash=textField(p,"hash");evm::check(std::regex_match(hash,std::regex("0x[0-9a-fA-F]{64}")),"Expected a BSC transaction hash");Rpc rpc(evm::DEFAULT_RPC,"","",30);evm::chain(rpc);auto args=evm::params({hash});auto receipt=evm::request(rpc,"eth_getTransactionReceipt",args.get());auto result=obj();json_object_object_add(result.get(),"receipt",receipt.release());return result;}
        if(method=="authorize_reward")return authorize(p);
        if(method=="verify_password"){wallet();checkPassword(p);return obj();}
        if(method=="export_evm"){wallet();checkPassword(p);auto o=obj();str(o.get(),"private_key","0x"+evm::hex(evmKey_.bytes));return o;}
        if(method=="export_seed"){wallet();checkPassword(p);auto o=obj();evm::Password seed(queryWalletSecret(wallet(),"mnemonic"));str(o.get(),"seed",seed.value);return o;}
        throw std::runtime_error("Unsupported desktop operation");
    }
};
static int run(const char* cli){
    Service service(cli);std::string line;
    while(std::getline(std::cin,line)){auto response=obj();JsonPtr request(nullptr);try{evm::check(line.size()<=1024*1024,"Desktop request too large");request=parse(line);str(response.get(),"id",textField(request.get(),"id"));json_object* p=nullptr;if(!json_object_object_get_ex(request.get(),"params",&p)||!p){auto empty=obj();json_object_object_add(request.get(),"params",empty.release());json_object_object_get_ex(request.get(),"params",&p);}auto result=service.call(textField(request.get(),"method"),p);json_object_object_add(response.get(),"result",result.release());}catch(const std::exception& e){str(response.get(),"error",e.what());}auto output=encode(response.get());std::cout<<output<<'\n'<<std::flush;cleanse(output);if(request){json_object* p=nullptr;if(json_object_object_get_ex(request.get(),"params",&p)){scrub(p,"password");scrub(p,"seed");}}json_object* sensitive=nullptr;if(json_object_object_get_ex(response.get(),"result",&sensitive)){scrub(sensitive,"private_key");scrub(sensitive,"seed");}cleanse(line);}
    return 0;
}
}
