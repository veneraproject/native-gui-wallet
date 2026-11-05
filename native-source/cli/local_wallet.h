#pragma once
 

static void writePrivateExclusive(const fs::path& path,const std::string& bytes) {
#ifdef _WIN32
    windows_wallet::PrivateSecurity security;
    evm::check(bytes.size()<=MAXDWORD,"Private wallet file is too large");
    HANDLE file=CreateFileW(path.wstring().c_str(),GENERIC_WRITE,0,&security.attributes,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,nullptr);
    evm::check(file!=INVALID_HANDLE_VALUE,"Cannot create file (it may already exist)");
    DWORD written=0;bool ok=WriteFile(file,bytes.data(),static_cast<DWORD>(bytes.size()),&written,nullptr)&&written==bytes.size()&&FlushFileBuffers(file);
    CloseHandle(file);if(!ok){DeleteFileW(path.wstring().c_str());throw std::runtime_error("Wallet write failed");}
#else
    int fd=open(path.c_str(),O_WRONLY|O_CREAT|O_EXCL,0600);
    evm::check(fd>=0,"Cannot create file (it may already exist)");
    size_t pos=0;bool ok=true;
    while(pos<bytes.size()){auto n=write(fd,bytes.data()+pos,bytes.size()-pos);if(n<0&&errno==EINTR)continue;if(n<=0){ok=false;break;}pos+=static_cast<size_t>(n);}
    if(ok)ok=fsync(fd)==0;
    if(close(fd)!=0)ok=false;
    if(!ok){unlink(path.c_str());throw std::runtime_error("Wallet write failed");}
#endif
}

#include "local_tls.h"

static bool removePrivateTree(const fs::path& path) noexcept {
    if(path.empty())return true;
    for(unsigned attempt=0;attempt<100;++attempt){
#ifndef _WIN32
         
         
        for(const auto* name:{"rpc.conf","tls.key","tls.crt","request.json","response.json"})
            unlink((path/name).c_str());
        if(rmdir(path.c_str())==0||errno==ENOENT)return true;
#else
        std::error_code error;fs::remove_all(path,error);
        if(!error&&!fs::exists(path,error)&&!error)return true;
#endif
        std::this_thread::sleep_for(20ms);
    }
    return false;
}

static void encryptWalletFile(const fs::path& path,const std::string& seed,const std::string& password) {
    auto salt=evm::random(cfg::SALT_SIZE),iv=evm::random(cfg::IV_SIZE);
    auto derived=deriveWalletKey(password,salt.data(),cfg::WALLET_KDF_ITERATIONS);
    evm::Secret key(evm::Bytes(derived.begin(),derived.end()));OPENSSL_cleanse(derived.data(),derived.size());
    evm::Owner<EVP_CIPHER_CTX,EVP_CIPHER_CTX_free> ctx(EVP_CIPHER_CTX_new(),EVP_CIPHER_CTX_free);
    evm::check(bool(ctx),"Cannot allocate wallet cipher");
    evm::check(EVP_EncryptInit_ex(ctx.get(),EVP_aes_256_gcm(),nullptr,nullptr,nullptr)==1&&
        EVP_CIPHER_CTX_ctrl(ctx.get(),EVP_CTRL_GCM_SET_IVLEN,static_cast<int>(iv.size()),nullptr)==1&&
        EVP_EncryptInit_ex(ctx.get(),nullptr,nullptr,key.bytes.data(),iv.data())==1,"Cannot initialize wallet cipher");
    evm::Bytes ciphertext(seed.size()+EVP_MAX_BLOCK_LENGTH),tag(cfg::TAG_SIZE);int n=0,total=0;
    evm::check(EVP_EncryptUpdate(ctx.get(),ciphertext.data(),&n,reinterpret_cast<const unsigned char*>(seed.data()),static_cast<int>(seed.size()))==1,"Wallet encryption failed");total=n;
    evm::check(EVP_EncryptFinal_ex(ctx.get(),ciphertext.data()+total,&n)==1,"Wallet encryption failed");total+=n;
    evm::check(EVP_CIPHER_CTX_ctrl(ctx.get(),EVP_CTRL_GCM_GET_TAG,static_cast<int>(tag.size()),tag.data())==1,"Cannot authenticate wallet file");
    std::string output(cfg::WALLET_MAGIC,sizeof(cfg::WALLET_MAGIC));
    for(int shift=24;shift>=0;shift-=8)output+=static_cast<char>((cfg::WALLET_KDF_ITERATIONS>>shift)&255);
    for(const auto* part:{&salt,&iv,&tag})output.append(reinterpret_cast<const char*>(part->data()),part->size());
    output.append(reinterpret_cast<const char*>(ciphertext.data()),static_cast<size_t>(total));
    writePrivateExclusive(path,output);
}

static std::string queryWalletSecret(Rpc& rpc,const std::string& kind) {
    auto p=obj();str(p.get(),"key_type",kind);auto response=rpc.call("query_key",p.get());
    auto result=textField(response.get(),"key");json_object* value=nullptr;
    if(json_object_object_get_ex(response.get(),"key",&value)&&value){auto raw=const_cast<char*>(json_object_get_string(value));if(raw)OPENSSL_cleanse(raw,std::strlen(raw));}
    return result;
}

static fs::path executablePath(const fs::path&) {
     
#ifdef _WIN32
    std::vector<wchar_t> buffer(32768);auto n=GetModuleFileNameW(nullptr,buffer.data(),static_cast<DWORD>(buffer.size()));
    evm::check(n>0&&n<buffer.size(),"Cannot locate running CLI");return fs::canonical(fs::path(std::wstring(buffer.data(),n)));
#elif defined(__linux__)
    std::vector<char> buffer(65536);auto n=readlink("/proc/self/exe",buffer.data(),buffer.size());
    if(n>0&&static_cast<size_t>(n)<buffer.size())return fs::canonical(fs::path(std::string(buffer.data(),static_cast<size_t>(n))));
     
     
    const auto kernelName=reinterpret_cast<const char*>(getauxval(AT_EXECFN));
    evm::check(kernelName&&*kernelName,"Cannot locate running CLI");return fs::canonical(fs::absolute(kernelName));
#else
    throw std::runtime_error("Secure executable discovery is supported on Linux and Windows only");
#endif
}
static fs::path trustedExecutable(const fs::path& candidate) {
    const auto path=fs::absolute(candidate).lexically_normal();
    evm::check(fs::is_regular_file(fs::symlink_status(path)),"Wallet RPC must be a regular executable beside the CLI (no symlink)");
#ifndef _WIN32
    const auto uid=geteuid();
    for(auto part=path;!part.empty();part=part.parent_path()){
        struct stat info{};evm::check(lstat(part.c_str(),&info)==0&&!S_ISLNK(info.st_mode),"Unsafe executable path");
        evm::check(info.st_uid==0||info.st_uid==uid,"Executable path has an untrusted owner");
        const bool sharedSticky=part!=path&&S_ISDIR(info.st_mode)&&(info.st_mode&S_ISVTX)&&info.st_uid==0;
        evm::check(!(info.st_mode&(S_IWGRP|S_IWOTH))||sharedSticky,"Executable path is writable by other users; remove group/world write access");
        if(part==part.parent_path())break;
    }
    evm::check(access(path.c_str(),X_OK)==0,"Wallet RPC is not executable");
#else
    windows_wallet::trustedPath(path);
#endif
    return path;
}
static fs::path findWalletRpc(json_object* config,const char* cli) {
#ifdef VENERA_TEST_EXTERNAL_RPC
     
    auto configured=textField(config,"wallet_rpc_executable");
    if(configured.empty()&&std::getenv("VENERA_WALLET_RPC_BINARY"))configured=std::getenv("VENERA_WALLET_RPC_BINARY");
    if(!configured.empty())return trustedExecutable(configured);
#else
    if(!textField(config,"wallet_rpc_executable").empty())throw std::runtime_error("Remove wallet_rpc_executable from settings; install venera-rpc beside the CLI");
    (void)config;
#endif
#ifdef _WIN32
    const std::string name="venera-rpc.exe";
#else
    const std::string name="venera-rpc";
#endif
    const auto self=trustedExecutable(executablePath(cli));
    return trustedExecutable(self.parent_path()/name);
}

static unsigned walletRpcPort() {
#ifndef _WIN32
    int socketFd=socket(AF_INET,SOCK_STREAM,0);
    evm::check(socketFd>=0,"Cannot allocate local wallet socket");
    sockaddr_in address{};address.sin_family=AF_INET;address.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    socklen_t size=sizeof(address);
    bool ok=bind(socketFd,reinterpret_cast<sockaddr*>(&address),sizeof(address))==0&&
        getsockname(socketFd,reinterpret_cast<sockaddr*>(&address),&size)==0;
    close(socketFd);
    evm::check(ok,"Cannot find an available local wallet port");
    return ntohs(address.sin_port);
#else
    SOCKET socketFd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    evm::check(socketFd!=INVALID_SOCKET,"Cannot allocate local wallet socket");
    sockaddr_in address{};address.sin_family=AF_INET;address.sin_addr.s_addr=htonl(INADDR_LOOPBACK);int size=sizeof(address);
    bool ok=bind(socketFd,reinterpret_cast<sockaddr*>(&address),sizeof(address))==0&&getsockname(socketFd,reinterpret_cast<sockaddr*>(&address),&size)==0;
    closesocket(socketFd);evm::check(ok,"Cannot find an available local wallet port");return ntohs(address.sin_port);
#endif
}

class WalletService {
    ProcessId process_=-1;
    fs::path sessionDirectory_;
    std::string url_,user_;
    evm::Password auth_{""};
    RpcTls tls_;
    bool running() {
        if(process_<0)return false;
#ifdef _WIN32
        if(WaitForSingleObject(reinterpret_cast<HANDLE>(process_),0)==WAIT_TIMEOUT)return true;
        CloseHandle(reinterpret_cast<HANDLE>(process_));
#else
        int status;auto result=waitpid(static_cast<pid_t>(process_),&status,WNOHANG);
        if(result==0||(result<0&&errno==EINTR))return true;
#endif
        process_=-1;return false;
    }
public:
    std::unique_ptr<Rpc> rpc;
    ~WalletService() {stop();}
    bool stop() {
        if(process_>=0){
            try{wallet_interrupts::CleanupScope cleanup;Rpc control(url_,user_,auth_.value,1,tls_);control.call("stop_wallet",obj().get());}catch(...){}
            for(int i=0;i<10&&running();++i)std::this_thread::sleep_for(100ms);
            if(running()){
#ifdef _WIN32
                TerminateProcess(reinterpret_cast<HANDLE>(process_),1);WaitForSingleObject(reinterpret_cast<HANDLE>(process_),1000);CloseHandle(reinterpret_cast<HANDLE>(process_));
#else
                kill(static_cast<pid_t>(process_),SIGTERM);
                for(int i=0;i<10&&running();++i)std::this_thread::sleep_for(100ms);
                if(running()){kill(static_cast<pid_t>(process_),SIGKILL);int status;while(waitpid(static_cast<pid_t>(process_),&status,0)<0&&errno==EINTR){}}
#endif
                process_=-1;
            }
        }
        if(!sessionDirectory_.empty()){
            if(!removePrivateTree(sessionDirectory_)){
                std::cerr<<ansi::yellow<<"Warning: temporary wallet credentials could not be removed: "<<sessionDirectory_<<ansi::reset<<'\n';return false;
            }
            sessionDirectory_.clear();
        }
        rpc.reset();return true;
    }
    void start(json_object* config,const fs::path& cache,const std::string& daemonUrl,const char* cli,bool quiet=false) {
        auto external=textField(config,"wallet_rpc");
        if(!external.empty()){
#ifndef VENERA_TEST_EXTERNAL_RPC
            throw std::runtime_error("Remove wallet_rpc from settings: locked sessions require a managed local venera-rpc process");
#else
            validUrl(external,true);
            std::string user=std::getenv("VENERA_RPC_USER")?std::getenv("VENERA_RPC_USER"):"venera";
            evm::Password pass(std::getenv("VENERA_RPC_PASSWORD")?std::getenv("VENERA_RPC_PASSWORD"):"");
            rpc=std::make_unique<Rpc>(external,user,pass.value);return;
#endif
        }
        const auto binary=findWalletRpc(config,cli);
        fs::create_directories(cache);
#ifndef _WIN32
        evm::check(chmod(cache.c_str(),0700)==0,"Cannot protect wallet cache directory");
#else
        evm::check(windows_wallet::privateDirectory(cache,true),"Cannot protect Windows wallet cache");
#endif
        sessionDirectory_=cache/(".rpc-"+evm::hex(evm::random(12)));
#ifdef _WIN32
        evm::check(windows_wallet::privateDirectory(sessionDirectory_),"Cannot create private Windows wallet session");
#else
        evm::check(fs::create_directory(sessionDirectory_),"Cannot create local wallet session");
#endif
#ifndef _WIN32
        evm::check(chmod(sessionDirectory_.c_str(),0700)==0,"Cannot protect wallet session directory");
#endif
        auto port=walletRpcPort();
        tls_=createLocalTlsIdentity(sessionDirectory_);
        url_="https://127.0.0.1:"+std::to_string(port);user_="venera";auth_.value=evm::hex(evm::random(32));
#ifdef _WIN32
        const fs::path nullLog("NUL");
#else
        const fs::path nullLog("/dev/null");
#endif
        const auto cachePath=fs::absolute(cache).string(),logPath=quiet?nullLog.string():fs::absolute(cache/"rpc.log").string();
        evm::check(cachePath.find_first_of("\r\n")==std::string::npos&&logPath.find_first_of("\r\n")==std::string::npos,"Invalid wallet path");
        evm::Password settings("wallet-dir="+cachePath+"\ndaemon-address="+daemonUrl+"\nrpc-bind-ip=127.0.0.1\nrpc-bind-port="+std::to_string(port)+"\nrpc-login="+user_+":"+auth_.value+"\nrpc-ssl=enabled\nrpc-ssl-private-key="+fs::absolute(sessionDirectory_/"tls.key").string()+"\nrpc-ssl-certificate="+tls_.caFile+"\nlog-file="+logPath+"\nlog-level=0\nmax-log-file-size=0\n");
        const auto configPath=sessionDirectory_/"rpc.conf";writePrivateExclusive(configPath,settings.value);cleanse(settings.value);
        process_=launch({binary.string(),"--config-file",fs::absolute(configPath).string()},true,quiet?nullLog:cache/"rpc-console.log",true);
        if(process_<0)throw std::runtime_error("Cannot start local wallet service");
        Rpc probe(url_,user_,auth_.value,1,tls_);
        for(int i=0;i<100;++i){
            if(!running())throw std::runtime_error(quiet?"Local wallet service exited":"Local wallet service exited; see "+logPath);
            try{probe.call("get_version",obj().get());rpc=std::make_unique<Rpc>(url_,user_,auth_.value,1800,tls_);return;}catch(...){}
            std::this_thread::sleep_for(100ms);
        }
        throw std::runtime_error(quiet?"Local wallet service did not become ready":"Local wallet service did not become ready; see "+logPath);
    }
};

static evm::Bytes openLocalWallet(WalletService& service,json_object* config,const std::string& daemonUrl,const char* cli,std::string& vnrAddress,fs::path& selectedFile,bool showDetails=true) {
    std::string input=selectedFile.string();
    if(input.empty()){
        std::cout<<"Venera CLI Wallet - v0.2.2\nWallet key file to restore, or 'create' / 'import':\n> "<<std::flush;
        if(!readInputLine(input))throw std::runtime_error("No wallet input provided");
        input=trim(input);
    }
    const bool create=lower(input)=="create";
    const bool importing=lower(input)=="import",newFile=create||importing;
    if(newFile){std::cout<<"Wallet file name: "<<std::flush;if(!readInputLine(input))throw std::runtime_error("No wallet file name provided");}
    evm::check(!trim(input).empty(),"No wallet file name provided");
    const fs::path file=fs::absolute(walletPath(input)),cache=file.string()+".cache";
    selectedFile=file;
    if(newFile&&fs::exists(file))throw std::runtime_error("Wallet file already exists; enter its name to restore it");
    if(newFile&&fs::exists(cache))throw std::runtime_error("Wallet cache already exists; choose a new file name");
    if(!newFile&&!fs::is_regular_file(file))throw std::runtime_error("Wallet file not found: "+file.string());
    evm::Password password(readHidden("Wallet password: "));evm::check(!password.value.empty(),"Wallet password cannot be empty");
    evm::Password seed("");
    if(newFile){evm::Password confirm(readHidden("Confirm password: "));evm::check(confirm.value==password.value,"Passwords differ");}
    if(importing){seed.value=trim(readHidden("Existing 25-word VNR seed: "));evm::check(splitWords(seed.value).size()==25,"Expected a 25-word VNR seed");}
    else if(!create){seed.value=decryptWalletFile(file.string(),password.value);evm::check(splitWords(seed.value).size()==25,"Wallet file must contain a 25-word VNR seed");}
     
    const auto external=textField(config,"wallet_rpc");if(!external.empty())validUrl(external,true);
    service.start(config,cache,daemonUrl,cli);Rpc& wallet=*service.rpc;
    auto automatic=obj();json_object_object_add(automatic.get(),"enable",json_object_new_boolean(false));wallet.call("auto_refresh",automatic.get());
    auto request=obj();str(request.get(),"filename","wallet");str(request.get(),"password",password.value);
    if(create){str(request.get(),"language","English");wallet.call("create_wallet",request.get());seed.value=queryWalletSecret(wallet,"mnemonic");evm::check(splitWords(seed.value).size()==25,"Local wallet returned an invalid VNR seed");encryptWalletFile(file,seed.value,password.value);}
    else if(fs::exists(cache/"wallet.keys")){
        wallet.call("open_wallet",request.get());evm::Password cached(queryWalletSecret(wallet,"mnemonic"));
        evm::check(splitWords(lower(cached.value))==splitWords(lower(seed.value)),"Wallet cache does not match this key file; move its .cache directory and restore again");
    }else{
        str(request.get(),"seed",seed.value);json_object_object_add(request.get(),"restore_height",venera_json_new_uint64(0));
        wallet.call("restore_deterministic_wallet",request.get());
         
        if(importing)encryptWalletFile(file,seed.value,password.value);
    }
     
    request=obj();cleanse(password.value);cleanse(seed.value);
    str(request.get(),"address",daemonUrl);json_object_object_add(request.get(),"trusted",json_object_new_boolean(loopback(daemonUrl)));wallet.call("set_daemon",request.get());
    auto address=wallet.call("get_address",obj().get());vnrAddress=textField(address.get(),"address");evm::check(!vnrAddress.empty(),"Local wallet returned no VNR address");
    evm::Password spendHex(queryWalletSecret(wallet,"spend_key"));evm::Secret spend(evm::unhex(spendHex.value));cleanse(spendHex.value);
    auto bsc=evm::fromVnrSpendKey(spend.bytes);wallet.call("store",obj().get());
    if(showDetails)std::cout<<(create?"Wallet created: ":importing?"Wallet imported: ":"Wallet restored: ")<<file<<"\nVNR address: "<<vnrAddress<<"\nBSC address (BNB/USDT/USDC): "<<evm::address(bsc)<<"\n";
    if(newFile)std::cout<<"Back up this encrypted .keys file and its wallet password; they restore both accounts.\n";
    return bsc;
}
