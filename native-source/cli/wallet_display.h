#pragma once
 
static uint64_t optionalNumber(json_object* o,const char* name,uint64_t fallback=0) {
    json_object* v=nullptr;return json_object_object_get_ex(o,name,&v)&&v?venera_json_get_uint64(v):fallback;
}
static bool optionalBool(json_object* o,const char* name,bool fallback=false) {
    json_object* v=nullptr;return json_object_object_get_ex(o,name,&v)&&v?json_object_get_int64(v)!=0:fallback;
}
static std::string displayVnr(uint64_t atomic,unsigned decimals) {
    evm::check(decimals<=9,"Invalid VNR precision");
    uint64_t scale=1,base=1;
    for(unsigned i=decimals;i<9;++i)scale*=10;
    for(unsigned i=0;i<decimals;++i)base*=10;
    auto rounded=atomic/scale;
    if(atomic%scale>=(scale+1)/2)++rounded;
    std::ostringstream text;text<<rounded/base;
    if(decimals)text<<'.'<<std::setw(decimals)<<std::setfill('0')<<rounded%base;
    return text.str();
}
static std::string utcTimestamp(uint64_t seconds) {
    if(!seconds||seconds>static_cast<uint64_t>(std::numeric_limits<std::time_t>::max()))return "-";
    std::time_t value=static_cast<std::time_t>(seconds);std::tm parts{};
#ifdef _WIN32
    if(gmtime_s(&parts,&value)!=0)return "-";
#else
    if(!gmtime_r(&value,&parts))return "-";
#endif
    std::ostringstream text;text<<std::put_time(&parts,"%Y-%m-%d %H:%M:%S");return text.str();
}
static void showNodeStatus(json_object* info) {
    std::cout<<ansi::green
        <<"Version: 0.2.2 - Vibrant Venera\n"
        <<"Status: "<<textField(info,"status","Unknown")<<'\n'
        <<"Height: "<<number(info,"height")<<'\n'
        <<"Total burned: "<<displayVnr(optionalNumber(info,"total_burned"),2)<<" VNR\n"
        <<"Peers (in/out): "<<optionalNumber(info,"incoming_connections_count")<<'/'
        <<optionalNumber(info,"outgoing_connections_count")<<ansi::reset<<'\n';
}
static void showHistory(json_object* result,uint64_t walletHeight) {
    struct Row{json_object* entry;std::string group;};std::vector<Row> rows;
    for(const auto* group:{"in","out","pending","failed","pool"}){
        json_object* list=nullptr;if(!json_object_object_get_ex(result,group,&list)||!list)continue;
        evm::check(json_object_get_type(list)==json_type_array,"Invalid history response");
        for(size_t i=0;i<json_object_array_length(list);++i){
            auto entry=json_object_array_get_idx(list,i);
            evm::check(entry&&json_object_get_type(entry)==json_type_object,"Invalid history entry");rows.push_back({entry,group});
        }
    }
    if(rows.empty()){std::cout<<"No transactions.\n";return;}
    std::stable_sort(rows.begin(),rows.end(),[](const Row& a,const Row& b){return optionalNumber(a.entry,"timestamp")>optionalNumber(b.entry,"timestamp");});
    std::cout<<std::left<<std::setw(22)<<"Timestamp (UTC)"<<std::setw(15)<<"Type"<<std::setw(23)<<"Amount (VNR)"
        <<std::setw(12)<<"Height"<<std::setw(16)<<"Confirmations"<<"Unlock time\n";
    for(const auto& row:rows){
        bool incoming=row.group=="in"||row.group=="pool";
        const char* color=incoming?ansi::green:ansi::red;
        auto height=optionalNumber(row.entry,"height");
        bool unconfirmed=row.group=="pending"||row.group=="pool"||row.group=="failed";
        auto confirmations=optionalNumber(row.entry,"confirmations",!unconfirmed&&height&&walletHeight>height?walletHeight-height:0);
        auto unlock=optionalNumber(row.entry,"unlock_time");
        bool locked=unconfirmed||optionalBool(row.entry,"locked",unlock<500000000?unlock>walletHeight:unlock>static_cast<uint64_t>(std::time(nullptr)));
        std::string unlockText=unlock?(unlock<500000000?"height "+std::to_string(unlock):utcTimestamp(unlock)+" UTC"):"0";
        unlockText+=locked?" (pending)":" (unlocked)";
        std::string type=incoming?"in":"out";
        if(row.group=="pending"||row.group=="pool")type+=" (pending)";
        if(row.group=="failed")type+=" (failed)";
        std::cout<<color<<std::left<<std::setw(22)<<utcTimestamp(optionalNumber(row.entry,"timestamp"))<<std::setw(15)<<type
            <<std::setw(23)<<displayVnr(number(row.entry,"amount"),8)<<std::setw(12)<<height<<std::setw(16)<<confirmations
            <<(locked?ansi::yellow:color)<<unlockText<<ansi::reset<<'\n';
        auto hash=textField(row.entry,"txid");
        std::cout<<color<<"    Transaction hash: "<<(hash.empty()?"-":hash)<<ansi::reset<<'\n';
    }
    std::cout<<std::right;
}

class SyncProgress {
    std::mutex mutex_;std::condition_variable wake_;std::thread ticker_;
    uint64_t height_,target_;bool done_=false,terminal_=false;unsigned frame_=0;
    void draw(){
        std::cout<<(terminal_?"\r\033[2K":"")<<"Sync: "<<height_<<'/'<<std::max(height_,target_);
        if(terminal_)std::cout<<' '<<"|/-\\"[frame_++%4];else std::cout<<'\n';
        std::cout<<std::flush;
    }
public:
    SyncProgress(uint64_t height,uint64_t target):height_(height),target_(target){
#ifdef _WIN32
        DWORD mode=0;terminal_=GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),&mode)!=0;
#else
        terminal_=isatty(STDOUT_FILENO)!=0;
#endif
        draw();
        if(terminal_)ticker_=std::thread([this]{std::unique_lock<std::mutex> lock(mutex_);while(!wake_.wait_for(lock,250ms,[this]{return done_;}))draw();});
    }
    void update(uint64_t height){std::lock_guard<std::mutex> lock(mutex_);if(height!=height_){height_=height;draw();}}
    ~SyncProgress(){
        {std::lock_guard<std::mutex> lock(mutex_);done_=true;}wake_.notify_one();if(ticker_.joinable())ticker_.join();
        if(terminal_)std::cout<<"\r\033[2KSync: "<<height_<<'/'<<std::max(height_,target_)<<'\n'<<std::flush;
    }
};
static uint64_t refreshWithProgress(Rpc& wallet,const std::string& daemonUrl) {
    auto version=wallet.call("get_version",obj().get());
    evm::check(optionalBool(version.get(),"refresh_progress"),"Rebuild venera-rpc from this package to enable sync progress");
    Rpc daemon(daemonUrl,"","",30);auto count=daemon.call("get_block_count",obj().get());auto target=number(count.get(),"count");
    auto current=wallet.call("get_height",obj().get());auto height=number(current.get(),"height");
    SyncProgress progress(height,target);
    for(;;){
        auto p=obj();json_object_object_add(p.get(),"max_blocks",venera_json_new_uint64(1000));
        auto result=wallet.call("refresh",p.get());auto next=number(result.get(),"height");progress.update(next);
        if(next>=target)return next;
        evm::check(next!=height||optionalNumber(result.get(),"blocks_fetched")!=0,"Wallet scan made no progress; check that the daemon is synchronized and run refresh again");
        height=next;
    }
}
