#pragma once
 
 

struct WalletSnapshot {
    fs::path file;
    std::string vnr,bsc;
    uint64_t balance=0,unlocked=0,height=0;
    bool hasBalance=false,possiblyStale=false;
};
static JsonPtr snapshotJson(const WalletSnapshot& s) {
    auto o=obj();str(o.get(),"file",s.file.string());str(o.get(),"vnr",s.vnr);str(o.get(),"bsc",s.bsc);
    json_object_object_add(o.get(),"balance",venera_json_new_uint64(s.balance));
    json_object_object_add(o.get(),"unlocked",venera_json_new_uint64(s.unlocked));
    json_object_object_add(o.get(),"height",venera_json_new_uint64(s.height));
    json_object_object_add(o.get(),"has_balance",json_object_new_boolean(s.hasBalance));
    json_object_object_add(o.get(),"possibly_stale",json_object_new_boolean(s.possiblyStale));
    return o;
}
static WalletSnapshot readSnapshot(json_object* o) {
    WalletSnapshot s;s.file=textField(o,"file");s.vnr=textField(o,"vnr");s.bsc=textField(o,"bsc");
    s.balance=number(o,"balance");s.unlocked=number(o,"unlocked");s.height=number(o,"height");
    json_object* b=nullptr;if(json_object_object_get_ex(o,"has_balance",&b))s.hasBalance=json_object_get_int64(b)!=0;
    if(json_object_object_get_ex(o,"possibly_stale",&b))s.possiblyStale=json_object_get_int64(b)!=0;
    return s;
}
static JsonPtr readJsonFile(const fs::path& path) {
    std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error("Cannot read operation/settings file");
    std::string text((std::istreambuf_iterator<char>(f)),{});
    evm::check(text.size()<=1024*1024,"Operation/settings file too large");return parse(text);
}
static JsonPtr readSettings(const fs::path& path) {return fs::exists(path)?readJsonFile(path):obj();}
#include "local_rewards.h"
static void refreshSnapshot(Rpc& wallet,WalletSnapshot& s,const std::string& daemonUrl) {
    auto height=refreshWithProgress(wallet,daemonUrl);
    auto b=wallet.call("get_balance",obj().get());
    s.balance=number(b.get(),"balance");s.unlocked=number(b.get(),"unlocked_balance");s.height=height;s.hasBalance=true;s.possiblyStale=false;
}
static void sendVnr(Rpc& wallet,const std::vector<std::string>& a,const std::function<void(const std::string&)>& beforeRelay={},bool staking=false) {
    evm::check(!a.empty(),"Missing transfer arguments");
    bool burn=lower(a[0])=="burn";
    evm::check((burn&&a.size()==2)||(!burn&&a.size()==4&&lower(a[0])=="send"&&lower(a[1])=="vnr"),"Invalid VNR transfer arguments");
    auto p=obj();
    auto n=atomicAmount(a.back());
    json_object_object_add(p.get(),"do_not_relay",json_object_new_boolean(true));
    if(burn){json_object_object_add(p.get(),"amount",venera_json_new_uint64(n));json_object_object_add(p.get(),"get_tx_metadata",json_object_new_boolean(true));}
    else {
        auto destination=obj();str(destination.get(),"address",a[2]);json_object_object_add(destination.get(),"amount",venera_json_new_uint64(n));
        auto list=json_object_new_array();json_object_array_add(list,destination.release());json_object_object_add(p.get(),"destinations",list);
        json_object_object_add(p.get(),"get_tx_metadata",json_object_new_boolean(true));
    }
    auto r=wallet.call(burn?"burn":"transfer",p.get());
    if(staking)std::cout<<ansi::cyan<<"Staking Transaction\nAmount: "<<amount(n)<<" VNR"<<ansi::reset<<'\n';
    else std::cout<<(burn?ansi::yellow:ansi::cyan)<<(burn?"Burn ":"Send ")<<amount(n)<<" VNR"<<(burn?" irreversibly":" to "+a[2])<<ansi::reset<<'\n';
    if(!burn)std::cout<<"Fee: "<<amount(number(r.get(),"fee"))<<" VNR\n";
    else {json_object* fees=nullptr;if(!json_object_object_get_ex(r.get(),"fee_list",&fees))throw std::runtime_error("Burn missing fees");std::cout<<"Fees (atomic VNR): "<<encode(fees)<<'\n';}
    std::cout<<"Signed transaction: "<<textField(r.get(),"tx_hash",textField(r.get(),"tx_hash_list","see history"))<<'\n';
    if(!yesNo("Broadcast this transaction?",false)){std::cout<<ansi::yellow<<"Cancelled; nothing broadcast."<<ansi::reset<<'\n';return;}
    if(!burn){auto relay=obj();auto metadata=textField(r.get(),"tx_metadata");if(metadata.empty())throw std::runtime_error("Missing signed transaction metadata");
        if(beforeRelay)beforeRelay(textField(r.get(),"tx_hash"));
        str(relay.get(),"hex",metadata);auto sent=wallet.call("relay_tx",relay.get());std::cout<<textField(sent.get(),"tx_hash")<<'\n';}
    else {
        json_object* metadata=nullptr;
        if(!json_object_object_get_ex(r.get(),"tx_metadata_list",&metadata)||!json_object_array_length(metadata))
            throw std::runtime_error("Missing burn metadata; use the updated venera-rpc from this package");
        for(size_t i=0;i<json_object_array_length(metadata);++i){
            if(beforeRelay){json_object* hashes=nullptr;evm::check(json_object_object_get_ex(r.get(),"tx_hash_list",&hashes)&&hashes&&json_object_array_length(hashes)==json_object_array_length(metadata),"Missing burn transaction hashes");beforeRelay(json_object_get_string(json_object_array_get_idx(hashes,i)));}
            auto relay=obj();str(relay.get(),"hex",json_object_get_string(json_object_array_get_idx(metadata,i)));
            auto sent=wallet.call("relay_tx",relay.get());std::cout<<textField(sent.get(),"tx_hash")<<'\n';
        }
    }

}
static int walletOperation(const fs::path& requestPath,const fs::path& responsePath,const char* cli) {
    auto request=readJsonFile(requestPath);auto cfgPath=fs::path(textField(request.get(),"config"));
    auto config=readSettings(cfgPath);auto mode=textField(request.get(),"mode");
    json_object* previous=nullptr;evm::check(json_object_object_get_ex(request.get(),"snapshot",&previous),"Missing wallet snapshot");
    auto snapshot=readSnapshot(previous);
    std::vector<std::string> args;json_object* list=nullptr;
    if(json_object_object_get_ex(request.get(),"args",&list)){
        evm::check(json_object_get_type(list)==json_type_array,"Invalid operation arguments");
        for(size_t i=0;i<json_object_array_length(list);++i){
            auto item=json_object_array_get_idx(list,i);evm::check(item&&json_object_get_type(item)==json_type_string,"Invalid operation argument");
            args.push_back(json_object_get_string(item));
        }
    }
    auto daemon=textField(config.get(),"daemon_rpc","http://127.0.0.1:29930");validUrl(daemon);
    {
        WalletService service;std::string vnr;
        try{
        evm::Secret bsc(openLocalWallet(service,config.get(),daemon,cli,vnr,snapshot.file,mode=="startup"));Rpc& wallet=*service.rpc;
        auto bscAddress=evm::address(bsc.bytes);
        if(mode!="startup")evm::check(vnr==snapshot.vnr&&bscAddress==snapshot.bsc,"Unlocked wallet does not match the selected accounts");
        if(mode!="bsc"){OPENSSL_cleanse(bsc.bytes.data(),bsc.bytes.size());bsc.bytes.clear();}
        snapshot.vnr=vnr;snapshot.bsc=bscAddress;
        if(mode=="startup"||mode=="refresh"){
            refreshSnapshot(wallet,snapshot,daemon);
            if(mode=="refresh"&&rewards::configured(config.get())&&fs::exists(rewards::queuePath(snapshot.file)))rewards::sync(wallet,config.get(),snapshot.file,vnr);
        }
        else if(mode=="vnr"){
            refreshWithProgress(wallet,daemon);
            if(!args.empty()&&lower(args[0])=="burn"&&rewards::configured(config.get())){
                rewards::preflight(config.get(),true);
                sendVnr(wallet,args,[&](const std::string& tx){rewards::enqueue(snapshot.file,vnr,"burn",tx,0);});
            }else {
                if(!args.empty()&&lower(args[0])=="burn")std::cout<<ansi::yellow<<"Rewards backend is not configured. This burn will not be registered automatically; keep its transaction hash."<<ansi::reset<<'\n';
                sendVnr(wallet,args);
            }
             
             
            wallet.call("store",obj().get());
            if(!args.empty()&&lower(args[0])=="burn"&&rewards::configured(config.get()))rewards::sync(wallet,config.get(),snapshot.file,vnr);
            refreshSnapshot(wallet,snapshot,daemon);
        }else if(mode=="rewards"){
            rewards::origin(config.get());evm::check(!args.empty(),"Missing rewards command");auto cmd=lower(args[0]);
            if(cmd=="stake"){
                evm::check(args.size()==3&&rewards::lockPeriod(args[2]),"Usage: stake AMOUNT 30|60|120|360");
                rewards::preflight(config.get(),false);
                auto days=static_cast<unsigned>(std::stoul(args[2]));atomicAmount(args[1]);refreshWithProgress(wallet,daemon);
                auto stakingAddress=rewards::stakingAddress(config.get(),days);
                std::cout<<ansi::yellow<<"Stake for "<<days<<" days; returns are handled by the staking backend."<<ansi::reset<<'\n';
                sendVnr(wallet,{"send","vnr",stakingAddress,args[1]},[&](const std::string& tx){rewards::enqueue(snapshot.file,vnr,"stake",tx,days);},true);
                wallet.call("store",obj().get());rewards::sync(wallet,config.get(),snapshot.file,vnr);refreshSnapshot(wallet,snapshot,daemon);
            }else if(cmd=="rewards_sync")rewards::sync(wallet,config.get(),snapshot.file,vnr);
            else if(cmd=="register_burn"||cmd=="register_stake"){
                evm::check((cmd=="register_burn"&&args.size()==2)||(cmd=="register_stake"&&args.size()==3&&rewards::lockPeriod(args[2])),"Usage: register_burn TXID | register_stake TXID 30|60|120|360");
                rewards::enqueue(snapshot.file,vnr,cmd=="register_burn"?"burn":"stake",args[1],cmd=="register_burn"?0:static_cast<unsigned>(std::stoul(args[2])));
                rewards::sync(wallet,config.get(),snapshot.file,vnr);
            }else if(cmd=="burn_stats"||cmd=="stake_stats"){
                auto result=rewards::authorized(wallet,config.get(),vnr,cmd);rewards::showStats(result.get(),cmd=="burn_stats");
            }else if(cmd=="claim_burn_yield"||cmd=="claim_status"){
                rewards::claim(wallet,config.get(),snapshot.file,vnr,snapshot.bsc,cmd=="claim_status",args.size()==2?args[1]:"");
            }else throw std::runtime_error("Unknown rewards command");
        }else if(mode=="bsc"){
            evm::command(args,config.get(),cfgPath,snapshot.bsc,&bsc.bytes);
        }else if(mode=="history"){
            auto p=obj();for(auto k:{"in","out","pending","failed","pool"})json_object_object_add(p.get(),k,json_object_new_boolean(true));
            auto height=wallet.call("get_height",obj().get());
            auto history=wallet.call("get_transfers",p.get());showHistory(history.get(),number(height.get(),"height"));
        }else if(mode=="seed"){
            if(yesNo("Display the secret mnemonic on this terminal?",false)){
                evm::Password seed(queryWalletSecret(wallet,"mnemonic"));std::cout<<seed.value<<'\n';
            }
        }else throw std::runtime_error("Unknown wallet operation");
        wallet.call("store",obj().get());
        wallet.call("close_wallet",obj().get());
        }catch(...){service.stop();throw;}
        evm::check(service.stop(),"Temporary wallet credentials could not be removed");
    }  
    wallet_interrupts::check();
    auto result=snapshotJson(snapshot);writePrivateExclusive(responsePath,encode(result.get()));
    return 0;
}

class OperationDirectory {
public:
    fs::path path;
    explicit OperationDirectory(const fs::path& base){
        path=base/(".venera-operation-"+evm::hex(evm::random(16)));
#ifdef _WIN32
        evm::check(windows_wallet::privateDirectory(path),"Cannot create private Windows operation directory");
#else
        evm::check(fs::create_directory(path),"Cannot create operation directory");
#endif
#ifndef _WIN32
        if(chmod(path.c_str(),0700)!=0){fs::remove(path);throw std::runtime_error("Cannot protect operation directory");}
#endif
    }
    void close(){if(path.empty())return;evm::check(removePrivateTree(path),"Cannot remove temporary operation files");path.clear();}
    ~OperationDirectory(){if(!path.empty()&&!removePrivateTree(path))std::cerr<<ansi::yellow<<"Warning: temporary operation files could not be removed: "<<path<<ansi::reset<<'\n';}
};
static void runWalletOperation(const char* cli,const fs::path& cfgPath,WalletSnapshot& snapshot,const std::string& mode,const std::vector<std::string>& args={}) {
    OperationDirectory dir(fs::absolute(cfgPath).parent_path());
    try{
    auto request=obj();str(request.get(),"config",fs::absolute(cfgPath).string());str(request.get(),"mode",mode);
    auto previous=snapshotJson(snapshot);json_object_object_add(request.get(),"snapshot",previous.release());
    auto list=json_object_new_array();for(const auto& a:args)json_object_array_add(list,json_object_new_string(a.c_str()));json_object_object_add(request.get(),"args",list);
    auto requestPath=dir.path/"request.json",responsePath=dir.path/"response.json";
    writePrivateExclusive(requestPath,encode(request.get()));
    auto executable=executablePath(cli);evm::check(!executable.empty(),"Cannot locate CLI executable");
    auto status=launch({executable.string(),"--wallet-operation",requestPath.string(),responsePath.string()},false,"",true);
    if(status!=0){
        if(mode=="vnr"||mode=="refresh"||mode=="rewards")snapshot.possiblyStale=true;
        throw std::runtime_error("Operation failed; spending is locked. If broadcasting was attempted, check transaction status before retrying.");
    }
    auto result=readJsonFile(responsePath);auto updated=readSnapshot(result.get());
    evm::check(!updated.file.empty()&&!updated.vnr.empty()&&!updated.bsc.empty(),"Invalid account snapshot");
    if(mode!="startup")evm::check(updated.file==snapshot.file&&updated.vnr==snapshot.vnr&&updated.bsc==snapshot.bsc,"Account snapshot mismatch");
    dir.close();
    snapshot=std::move(updated);
    }catch(...){dir.close();throw;}
}
static void showVnrBalance(const WalletSnapshot& snapshot,const std::string& daemonUrl) {
    evm::check(snapshot.hasBalance,"No VNR balance snapshot; run refresh");
    std::cout<<ansi::green<<"VNR total: "<<amount(snapshot.balance)<<"; unlocked: "<<amount(snapshot.unlocked)<<" (cached)"<<ansi::reset<<'\n';
    std::cout<<"Wallet scanned height: "<<snapshot.height<<'\n';
    if(snapshot.possiblyStale)std::cout<<ansi::yellow<<"Wallet state may have changed; run refresh."<<ansi::reset<<'\n';
    try{
        Rpc daemon(daemonUrl,"","",2);auto info=daemon.call("get_info",obj().get());auto height=number(info.get(),"height");
        std::cout<<"Daemon height: "<<height<<'\n';
        if(height>snapshot.height)std::cout<<ansi::yellow<<"New blocks available; run refresh to update VNR balances."<<ansi::reset<<'\n';
    }catch(const std::exception&){std::cout<<ansi::yellow<<"Daemon height unavailable; cached balance may be outdated."<<ansi::reset<<'\n';}
}
