#pragma once
 
 
namespace rewards {
static constexpr const char* API_ORIGIN="https://wallet.veneralabs.org";
static std::string origin(json_object* config) {
    std::string url=API_ORIGIN;
#ifdef VENERA_TEST_EXTERNAL_RPC
    auto testUrl=textField(config,"rewards_test_api");if(!testUrl.empty())url=testUrl;
#else
    (void)config;
#endif
    validUrl(url);while(!url.empty()&&url.back()=='/')url.pop_back();return url;
}
static bool configured(json_object* config){
#ifdef VENERA_TEST_EXTERNAL_RPC
    return !textField(config,"rewards_test_api").empty();
#else
    (void)config;return true;
#endif
}
static bool lockPeriod(const std::string& s){return s=="30"||s=="60"||s=="120"||s=="360";}
static void hash(const std::string& tx){evm::check(std::regex_match(tx,std::regex("[0-9a-f]{64}")),"Expected a full VNR transaction hash");}
static std::string endpoint(const std::string& action){
    for(const auto& route:std::vector<std::pair<const char*,const char*>>{
        {"info","/api/local_rewards_info"},{"challenge","/api/local_wallet_challenge"},
        {"burn","/api/local_burn"},{"stake","/api/local_stake"},
        {"burn_stats","/api/local_burn_stats"},{"stake_stats","/api/local_stake_stats"},
        {"claim_burn_yield","/api/claim_burn_yield"},{"claim_status","/api/claim_burn_yield_status"},
        {"staking_det","/api/staking_det"}})if(action==route.first)return route.second;
    throw std::runtime_error("Unknown rewards endpoint");
}
static JsonPtr request(Rpc& api,const std::string& action,json_object* payload) {
    auto result=api.post(endpoint(action),payload,true);
    json_object* error=nullptr;
    if(json_object_object_get_ex(result.get(),"error",&error)){
        auto message=textField(result.get(),"error","Rewards request rejected");
         
        for(auto& c:message)if(static_cast<unsigned char>(c)<32||static_cast<unsigned char>(c)==127)c=' ';
        if(message=="Treasury has insufficient USDT")throw std::runtime_error(message+", please try again later");
        throw std::runtime_error("Rewards: "+message.substr(0,240));
    }
    return result;
}
static void preflight(json_object* config,bool burn) {
    Rpc api(origin(config),"","",20);auto result=request(api,"info",obj().get());
    evm::check(textField(result.get(),"network")=="mainnet","Rewards backend is not mainnet");
    if(burn){
        json_object* policy=nullptr;evm::check(json_object_object_get_ex(result.get(),"burn_policy",&policy)&&policy,"Backend burn reward policy is unavailable; nothing sent");
        std::cout<<ansi::cyan<<"Burn multiplier: "<<textField(policy,"burn_multiplier")<<"; points per VNR: "<<textField(policy,"point_rate")<<ansi::reset<<'\n';
    }
}
static std::string stakingAddress(json_object* config,unsigned days) {
    Rpc api(origin(config),"","",20);auto response=request(api,"staking_det",obj().get());
    auto nonce=textField(response.get(),"nonce"),payload=textField(response.get(),"payload");
    evm::check(textField(response.get(),"encoding")=="xor-sha256-hex-v1"&&
        std::regex_match(nonce,std::regex("[0-9a-f]{32}"))&&payload.size()>=2&&payload.size()<=4096&&payload.size()%2==0&&
        std::all_of(payload.begin(),payload.end(),[](unsigned char c){return (c>='0'&&c<='9')||(c>='a'&&c<='f');}),"Invalid staking details response");
    auto decoded=evm::unhex(payload);auto material=std::string("Venera staking details obfuscation v1:")+nonce;
    unsigned char mask[SHA256_DIGEST_LENGTH];
    evm::check(SHA256(reinterpret_cast<const unsigned char*>(material.data()),material.size(),mask)!=nullptr,"Cannot decode staking details");
    for(size_t i=0;i<decoded.size();++i)decoded[i]^=mask[i%sizeof(mask)];
    auto details=parse(std::string(decoded.begin(),decoded.end()));
    auto expires=number(details.get(),"expires_at"),now=static_cast<uint64_t>(std::time(nullptr));
    evm::check(textField(details.get(),"network")=="mainnet"&&expires>=now&&expires<=now+360,"Expired or wrong-network staking details");
    auto address=textField(details.get(),"address");
    evm::check(std::regex_match(address,std::regex("[1-9A-HJ-NP-Za-km-z]{90,120}")),"Invalid staking destination");
    json_object* locks=nullptr;bool supported=false;
    evm::check(json_object_object_get_ex(details.get(),"lock_days",&locks)&&locks&&json_object_get_type(locks)==json_type_array&&json_object_array_length(locks)<=4,"Invalid staking lock periods");
    for(size_t i=0;i<json_object_array_length(locks);++i){
        auto value=json_object_array_get_idx(locks,i);evm::check(value&&json_object_get_type(value)==json_type_int,"Invalid staking lock period");
        auto lock=json_object_get_int64(value);evm::check(lock==30||lock==60||lock==120||lock==360,"Unsupported staking lock period");
        if(static_cast<uint64_t>(lock)==days)supported=true;
    }
    evm::check(supported,"Selected staking lock period is unavailable");return address;
}
static JsonPtr authorized(Rpc& wallet,json_object* config,const std::string& address,
        const std::string& action,const std::string& tx="",unsigned days=0,
        const std::string& bsc="",const std::string& id="") {
    auto url=origin(config);Rpc api(url,"","",30);
    auto p=obj();str(p.get(),"audience",url);str(p.get(),"action",action);str(p.get(),"wallet_address",address);
    str(p.get(),"txid",tx);str(p.get(),"erc20_address",lower(bsc));str(p.get(),"request_id",id);
    json_object_object_add(p.get(),"lock_days",venera_json_new_uint64(days));
    auto challenge=request(api,"challenge",p.get());
    auto nonce=textField(challenge.get(),"nonce"),token=textField(challenge.get(),"challenge_id");
    auto expires=number(challenge.get(),"expires_at");
    auto now=static_cast<uint64_t>(std::time(nullptr));
    evm::check(std::regex_match(nonce,std::regex("[0-9a-f]{64}"))&&expires>=now&&expires<=now+360&&token.size()<=4096,"Invalid rewards challenge");
    auto message=std::string("Venera local rewards v1 mainnet\n")+url+"\n"+action+"\n"+address+"\n"+tx+"\n"+std::to_string(days)+"\n"+lower(bsc)+"\n"+id+"\nNonce: "+nonce+"\nExpires: "+std::to_string(expires)+"\n";
    evm::check(textField(challenge.get(),"message")==message,"Rewards server changed the signed request intent");
    auto sign=obj();str(sign.get(),"data",message);str(sign.get(),"signature_type","spend");
    json_object_object_add(sign.get(),"account_index",venera_json_new_uint64(0));
    json_object_object_add(sign.get(),"address_index",venera_json_new_uint64(0));
    auto signedMessage=wallet.call("sign",sign.get());
    auto signature=textField(signedMessage.get(),"signature");
    evm::check(signature.rfind("SigV2",0)==0,"Updated wallet RPC spend signatures are required");
    auto submit=obj();str(submit.get(),"challenge_id",token);str(submit.get(),"wallet_signature",signature);
    if(action=="burn"||action=="stake"){
        auto proof=obj();str(proof.get(),"txid",tx);str(proof.get(),"message",message);
        if(action=="stake")str(proof.get(),"address",stakingAddress(config,days));
        auto result=wallet.call(action=="stake"?"get_tx_proof":"get_spend_proof",proof.get());
        auto value=textField(result.get(),"signature");evm::check(!value.empty(),"Wallet did not return a transaction proof");
        str(submit.get(),action=="stake"?"tx_proof":"spend_proof",value);
    }
    return request(api,action,submit.get());
}
static fs::path queuePath(const fs::path& wallet){return fs::path(wallet.string()+".rewards.json");}
static JsonPtr queue(const fs::path& wallet,const std::string& address) {
    auto path=queuePath(wallet);
    auto q=fs::exists(path)?readJsonFile(path):obj();
    evm::check(textField(q.get(),"wallet_address",address)==address,"Rewards receipt file belongs to another wallet");
    str(q.get(),"wallet_address",address);json_object* pending=nullptr;
    if(!json_object_object_get_ex(q.get(),"pending",&pending))json_object_object_add(q.get(),"pending",json_object_new_array());
    else evm::check(pending&&json_object_get_type(pending)==json_type_array,"Invalid rewards receipts");
    return q;
}
static void save(const fs::path& wallet,json_object* q) {
    auto path=queuePath(wallet),temporary=fs::path(path.string()+".tmp-"+evm::hex(evm::random(16)));
    try{
        writePrivateExclusive(temporary,encode(q));
#ifndef _WIN32
        int fd=open(temporary.c_str(),O_RDONLY);evm::check(fd>=0,"Cannot open rewards receipts");
        auto result=fsync(fd);close(fd);evm::check(result==0,"Cannot persist rewards receipts");
#endif
        replaceWalletFile(temporary,path);
#ifndef _WIN32
        int dir=open(fs::absolute(path).parent_path().c_str(),O_RDONLY|O_DIRECTORY);
        evm::check(dir>=0,"Cannot open receipt directory");auto synced=fsync(dir);close(dir);
        evm::check(synced==0,"Cannot persist receipt directory");
#endif
    }catch(...){fs::remove(temporary);throw;}
}
static void enqueue(const fs::path& file,const std::string& address,const std::string& action,
        const std::string& tx,unsigned days) {
    hash(tx);auto q=queue(file,address);json_object* pending=nullptr;json_object_object_get_ex(q.get(),"pending",&pending);
    for(size_t i=0;i<json_object_array_length(pending);++i){auto item=json_object_array_get_idx(pending,i);if(textField(item,"txid")==tx){
        evm::check(textField(item,"action")==action&&number(item,"lock_days")==days,"Transaction receipt terms differ");return;
    }}
    auto item=obj();str(item.get(),"action",action);str(item.get(),"txid",tx);json_object_object_add(item.get(),"lock_days",venera_json_new_uint64(days));
    json_object_array_add(pending,item.release());save(file,q.get());
}
static void sync(Rpc& wallet,json_object* config,const fs::path& file,const std::string& address) {
    origin(config);auto q=queue(file,address);json_object* pending=nullptr;json_object_object_get_ex(q.get(),"pending",&pending);
    auto remaining=json_object_new_array();std::unique_ptr<json_object,decltype(&json_object_put)> guard(remaining,json_object_put);
    for(size_t i=0;i<json_object_array_length(pending);++i){
        auto item=json_object_array_get_idx(pending,i);auto tx=textField(item,"txid");
        try{
            hash(tx);auto action=textField(item,"action");evm::check(action=="burn"||action=="stake","Invalid rewards receipt");
            auto days=number(item,"lock_days");evm::check(days<=360,"Invalid stake receipt");
            auto response=authorized(wallet,config,address,action,tx,static_cast<unsigned>(days));
            auto status=textField(response.get(),"status");
            evm::check(status=="registered"||status=="pending","Invalid registration response");
            if(action=="stake"&&status=="registered")std::cout<<ansi::green<<"Stake "<<tx<<" successful"<<ansi::reset<<'\n';
            else std::cout<<(status=="registered"?ansi::green:ansi::yellow)<<action<<" "<<tx<<": "<<status<<ansi::reset<<'\n';
            if(status=="registered")continue;
        }catch(const std::exception& e){std::cout<<ansi::yellow<<"Registration saved for retry: "<<tx<<" ("<<e.what()<<")"<<ansi::reset<<'\n';}
        json_object_array_add(remaining,json_object_get(item));
    }
    json_object_object_add(q.get(),"pending",guard.release());save(file,q.get());
    if(json_object_array_length(remaining))std::cout<<ansi::yellow<<"Run rewards_sync after confirmations. Do not send the funds again."<<ansi::reset<<'\n';
}
static uint64_t stakeAtoms(json_object* data,const char* field) {
     
     
    auto text=textField(data,field,"0");size_t end=0;long double value=0;
    try{value=std::stold(text,&end);}catch(...){throw std::runtime_error("Invalid staking amount");}
    evm::check(end==text.size()&&std::isfinite(value)&&value>=0,"Invalid staking amount");
    auto atoms=std::round(value*1000000000.L);
    evm::check(atoms<std::ldexp(1.L,64),"Staking amount overflow");return static_cast<uint64_t>(atoms);
}
static std::string compactVnr(uint64_t atoms) {
    auto value=amount(atoms);while(value.back()=='0')value.pop_back();if(value.back()=='.')value.pop_back();return value;
}
static void showStats(json_object* response,bool burn) {
    json_object* data=nullptr;evm::check(json_object_object_get_ex(response,"data",&data)&&data,"Missing rewards statistics");
    if(burn){
        json_object* burns=nullptr;uint64_t total=0;
        if(json_object_object_get_ex(data,"burns",&burns)&&burns&&json_object_get_type(burns)==json_type_array){
            for(size_t i=0;i<json_object_array_length(burns);++i){auto n=number(json_object_array_get_idx(burns,i),"amount_burned_atomic");evm::check(total<=UINT64_MAX-n,"Burn total overflow");total+=n;}
        }
        std::cout<<ansi::green<<"Burned: "<<amount(total)<<" VNR\n";
        for(const auto& field:std::vector<std::pair<const char*,const char*>>{{"burn_points","Burn points"},{"accumulated_usdt","Claimable USDT"},{"yield_earned_total_usdt","Lifetime USDT yield"},{"estimated_daily_usdt","Estimated daily USDT"}})
            std::cout<<field.second<<": "<<textField(data,field.first,"0")<<'\n';
        if(!textField(data,"active_claim").empty())std::cout<<"Active claim: "<<textField(data,"active_claim")<<'\n';
        std::cout<<ansi::reset;
    }else{
        auto principal=stakeAtoms(data,"amount_s_total"),expected=stakeAtoms(data,"amount_out_total");
        evm::check(expected>=principal,"Invalid expected staking return");
        std::cout<<ansi::green<<"Staked: "<<textField(data,"amount_s_total","0")<<" VNR; expected yield: "<<compactVnr(expected-principal)<<" VNR\n"<<ansi::reset;
        json_object* stakes=nullptr;if(json_object_object_get_ex(data,"stakes",&stakes)&&stakes&&json_object_get_type(stakes)==json_type_array)
            for(size_t i=0;i<json_object_array_length(stakes);++i){auto s=json_object_array_get_idx(stakes,i);std::cout<<ansi::yellow<<textField(s,"amount_s","0")<<" VNR; "<<textField(s,"lock_days")<<" days; matures: "<<utcTimestamp(number(s,"timestamp_out"))<<" UTC; "<<textField(s,"status","locked")<<"\n"<<textField(s,"tx_hash")<<ansi::reset<<'\n';}
    }
}
static void claim(Rpc& wallet,json_object* config,const fs::path& file,const std::string& address,
        const std::string& bsc,bool statusOnly,const std::string& explicitId="") {
    origin(config);auto q=queue(file,address);
    std::string id=explicitId;
    if(id.empty())id=textField(q.get(),statusOnly?"last_claim":"active_claim");
    if(statusOnly)evm::check(!id.empty(),"No saved claim; use claim_status REQUEST_ID");
    else if(id.empty())id=evm::hex(evm::random(16));
    evm::check(std::regex_match(id,std::regex("[0-9a-f]{32}")),"Invalid claim ID");
    if(!statusOnly){
        std::cout<<ansi::cyan<<"Claim burn yield as USDT on BSC to "<<bsc<<ansi::reset<<'\n';
        if(!yesNo("Submit this claim?",false))return;
        str(q.get(),"active_claim",id);str(q.get(),"last_claim",id);save(file,q.get());
    }
    std::cout<<"Claim ID: "<<id<<'\n';
    auto response=authorized(wallet,config,address,statusOnly?"claim_status":"claim_burn_yield","",0,statusOnly?"":bsc,id);
    auto status=textField(response.get(),"status");
    evm::check(status=="prepared"||status=="pending"||status=="paid"||status=="failed"||status=="manual_review","Invalid claim response");
    std::cout<<(status=="paid"?ansi::green:ansi::yellow)<<"USDT claim: "<<status<<"; "<<textField(response.get(),"amount_usdt")<<" USDT\n"<<textField(response.get(),"tx_hash")<<ansi::reset<<'\n';
    if((status=="paid"||status=="failed")&&textField(q.get(),"active_claim")==id){str(q.get(),"active_claim","");save(file,q.get());}
}
}  
