#pragma once
#include <atomic>

 
 
struct WalletExit {};
namespace wallet_interrupts {
static std::atomic<bool> desktopCancel{false};
static volatile std::sig_atomic_t armed=0,notice=0,quit=0,foregroundChild=0,cleaningUp=0;
static void onInterrupt(int) {
#ifdef _WIN32
    std::signal(SIGINT,onInterrupt);
#endif
    if(foregroundChild)return;  
    if(armed)quit=1;
    else {armed=1;notice=1;}
}
static void inputReceived(){armed=0;}
static void showNotice(){
    if(notice){notice=0;std::cerr<<"\n"<<ansi::yellow<<"Press Ctrl+C again to exit."<<ansi::reset<<'\n'<<std::flush;}
}
static void check(){if(cleaningUp)return;showNotice();if(quit)throw WalletExit{};}
static void install(){
#ifdef _WIN32
    std::signal(SIGINT,onInterrupt);
#else
    struct sigaction action{};action.sa_handler=onInterrupt;sigemptyset(&action.sa_mask);
    if(sigaction(SIGINT,&action,nullptr)!=0)throw std::runtime_error("Cannot install keyboard interrupt handler");
#endif
}
struct ForegroundScope {
    bool active;
    explicit ForegroundScope(bool value):active(value){if(active)foregroundChild=1;}
    ~ForegroundScope(){if(active){foregroundChild=0;notice=0;}}
};
struct CleanupScope {
    std::sig_atomic_t previous;
    CleanupScope():previous(cleaningUp){cleaningUp=1;}
    ~CleanupScope(){cleaningUp=previous;}
};
static int transferProgress(void*,curl_off_t,curl_off_t,curl_off_t,curl_off_t) noexcept {
    if(desktopCancel.load())return 1;
    if(cleaningUp)return 0;  
    try{showNotice();}catch(...){return 1;}
    return quit?1:0;
}
}
