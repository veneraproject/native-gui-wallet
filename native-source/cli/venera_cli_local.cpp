#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#endif
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <ctime>
#include <cctype>
#include <cstdint>
#include <csignal>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include <utility>

#include <curl/curl.h>
#include <json-c/json.h>
#include "json_compat.h"
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/x509v3.h>
#include <openssl/rsa.h>

#ifdef _WIN32

#include <conio.h>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#else
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <termios.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#ifdef __linux__
#include <sys/prctl.h>
#include <sys/auxv.h>
#endif
#endif

namespace fs = std::filesystem;
using namespace std::chrono_literals;
#include "windows_support.h"
static void writePrivateExclusive(const fs::path&,const std::string&);

namespace ansi {
constexpr const char* reset   = "\033[0m";
constexpr const char* bold    = "\033[1m";
constexpr const char* dim     = "\033[2m";
constexpr const char* red     = "\033[31m";
constexpr const char* green   = "\033[32m";
constexpr const char* yellow  = "\033[33m";
constexpr const char* blue    = "\033[34m";
constexpr const char* magenta = "\033[35m";
constexpr const char* cyan    = "\033[36m";
constexpr const char* white   = "\033[37m";
constexpr const char* gray    = "\033[90m";
constexpr const char* brightMagenta = "\033[95m";
constexpr const char* brightCyan    = "\033[96m";
}

#include "interrupts.h"

static void enableAnsiColors() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD mode = 0;
    if (!GetConsoleMode(hOut, &mode)) return;
    SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

static void redrawCommandLine(const std::string& prompt, const std::string& line) {
    std::cout << "\r\033[2K" << prompt << line << std::flush;
}

 
 
static bool readInputLine(std::string& out) {
    out.clear();
    for(;;){
        wallet_interrupts::check();
        char c=0;
#ifdef _WIN32
        DWORD n=0;const bool ok=ReadFile(GetStdHandle(STD_INPUT_HANDLE),&c,1,&n,nullptr)!=0;
        if(!ok&&GetLastError()==ERROR_OPERATION_ABORTED){wallet_interrupts::check();continue;}
        wallet_interrupts::check();if(!ok||!n)return !out.empty();
#else
        auto n=::read(STDIN_FILENO,&c,1);
        if(n<0&&errno==EINTR)continue;
        if(n<=0)return !out.empty();
#endif
        if(c==3){wallet_interrupts::onInterrupt(SIGINT);wallet_interrupts::check();continue;}
        wallet_interrupts::inputReceived();
        if(c=='\n')return true;
        if(c!='\r')out.push_back(c);
        if(out.size()>16384)throw std::runtime_error("Input line too long");
    }
}

static void protectProcessMemory() {
#ifndef _WIN32
    struct rlimit limit{0,0};
    if(setrlimit(RLIMIT_CORE,&limit)!=0)throw std::runtime_error("Cannot disable core dumps");
#ifdef __linux__
    if(prctl(PR_SET_DUMPABLE,0)<0)throw std::runtime_error("Cannot restrict memory dumps");
#endif
#endif
}

static bool readCommandLine(
    const std::string& prompt,
    std::vector<std::string>& history,
    std::string& out)
{
#ifndef _WIN32
    if (!isatty(STDIN_FILENO)) {
        std::cout << prompt << std::flush;
        return readInputLine(out);
    }
#endif

    std::string line;
    std::size_t historyIndex = history.size();
    redrawCommandLine(prompt, line);

#ifdef _WIN32
    for (;;) {
        wallet_interrupts::check();
        int ch = _getch();
        if (ch == 3) {
            wallet_interrupts::onInterrupt(SIGINT);wallet_interrupts::check();line.clear();redrawCommandLine(prompt,line);continue;
        }
        wallet_interrupts::inputReceived();
        if (ch == '\r' || ch == '\n') {
            std::cout << "\n";
            break;
        }
        if (ch == 8 || ch == 127) {
            if (!line.empty()) line.pop_back();
            redrawCommandLine(prompt, line);
            continue;
        }
        if (ch == 0 || ch == 224) {
            const int ext = _getch();
            if (ext == 72) {  
                if (!history.empty() && historyIndex > 0) {
                    --historyIndex;
                    line = history[historyIndex];
                }
                redrawCommandLine(prompt, line);
            } else if (ext == 80) {  
                if (historyIndex < history.size()) ++historyIndex;
                line = historyIndex < history.size() ? history[historyIndex] : "";
                redrawCommandLine(prompt, line);
            }
            continue;
        }
        if (ch >= 32 && ch <= 126) {
            line.push_back(static_cast<char>(ch));
            redrawCommandLine(prompt, line);
        }
    }
#else
    termios oldt{};
    if (tcgetattr(STDIN_FILENO, &oldt) != 0) {
        std::cout << prompt << std::flush;
        return readInputLine(out);
    }

    termios raw = oldt;
    raw.c_lflag &= static_cast<tcflag_t>(~(ICANON | ECHO));
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    auto restoreTerminal = [&]() {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    };

    try {
        for (;;) {
            wallet_interrupts::check();
            unsigned char ch = 0;
            const ssize_t n = ::read(STDIN_FILENO, &ch, 1);
            if(n<0&&errno==EINTR){line.clear();wallet_interrupts::check();redrawCommandLine(prompt,line);continue;}
            if (n <= 0) {
                restoreTerminal();
                std::cout << "\n";
                return false;
            }
            if (ch == 3) {  
                wallet_interrupts::onInterrupt(SIGINT);wallet_interrupts::check();line.clear();redrawCommandLine(prompt,line);continue;
            }
            wallet_interrupts::inputReceived();
            if (ch == 4 && line.empty()) {  
                restoreTerminal();
                std::cout << "\n";
                return false;
            }
            if (ch == '\r' || ch == '\n') {
                restoreTerminal();
                std::cout << "\n";
                break;
            }
            if (ch == 127 || ch == 8) {
                if (!line.empty()) line.pop_back();
                redrawCommandLine(prompt, line);
                continue;
            }
            if (ch == 27) {  
                unsigned char a = 0, b = 0;
                if (::read(STDIN_FILENO, &a, 1) == 1 && a == '[' &&
                    ::read(STDIN_FILENO, &b, 1) == 1) {
                    if (b == 'A') {  
                        if (!history.empty() && historyIndex > 0) {
                            --historyIndex;
                            line = history[historyIndex];
                        }
                    } else if (b == 'B') {  
                        if (historyIndex < history.size()) ++historyIndex;
                        line = historyIndex < history.size() ? history[historyIndex] : "";
                    }
                    redrawCommandLine(prompt, line);
                }
                continue;
            }
            if (ch >= 32 && ch <= 126) {
                line.push_back(static_cast<char>(ch));
                redrawCommandLine(prompt, line);
            }
        }
    } catch (...) {
        restoreTerminal();
        throw;
    }
#endif

    out = line;
    if (!line.empty() && (history.empty() || history.back() != line)) {
        history.push_back(line);
        if (history.size() > 200) history.erase(history.begin());
    }
    return true;
}

namespace cfg {
constexpr std::uint32_t WALLET_KDF_ITERATIONS = 600000;
constexpr std::size_t SALT_SIZE = 16;
constexpr std::size_t IV_SIZE = 12;
constexpr std::size_t TAG_SIZE = 16;
constexpr char WALLET_MAGIC[8] = {'V','N','R','K','E','Y','0','2'};
}

struct JsonDeleter {
    void operator()(json_object* p) const noexcept {
        if (p) json_object_put(p);
    }
};
using JsonPtr = std::unique_ptr<json_object, JsonDeleter>;

static std::string trim(std::string s) {
    const auto notSpace = [](unsigned char c) { return !std::isspace(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), notSpace));
    s.erase(std::find_if(s.rbegin(), s.rend(), notSpace).base(), s.end());
    return s;
}

static std::string lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return s;
}

static std::vector<std::string> splitWords(const std::string& s) {
    std::istringstream in(s);
    std::vector<std::string> out;
    std::string w;
    while (in >> w) out.push_back(w);
    return out;
}

static std::vector<std::string> splitCommand(const std::string& s) {
    return splitWords(s);
}

static bool yesNo(const std::string& prompt, bool defaultYes) {
    std::cout << ansi::yellow << prompt << (defaultYes ? " [Y/n]: " : " [y/N]: ") << ansi::reset << std::flush;
    std::string answer;
    readInputLine(answer);
    answer = lower(trim(answer));
    if (answer.empty()) return defaultYes;
    return answer == "y" || answer == "yes";
}

static std::string readHidden(const std::string& prompt) {
    std::cout << prompt << std::flush;
    std::string result;
    try{
#ifdef _WIN32
    while (true) {
        int ch = _getch();
        if (ch == '\r' || ch == '\n') break;
        if (ch == 8) {
            if (!result.empty()) result.pop_back();
            continue;
        }
        if (ch == 3) {wallet_interrupts::onInterrupt(SIGINT);wallet_interrupts::check();continue;}
        wallet_interrupts::inputReceived();
        if (ch >= 32 && ch <= 126) result.push_back(static_cast<char>(ch));
    }
    std::cout << "\n";
#else
    termios oldt{};
    if (tcgetattr(STDIN_FILENO, &oldt) != 0) {
        if(!readInputLine(result))throw std::runtime_error("No password/input provided");
        return result;
    }
    termios newt = oldt;
    newt.c_lflag &= static_cast<tcflag_t>(~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    try{if(!readInputLine(result))throw std::runtime_error("No password/input provided");}
    catch(...){tcsetattr(STDIN_FILENO,TCSANOW,&oldt);throw;}
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\n";
#endif
    return result;
    }catch(...){if(!result.empty())OPENSSL_cleanse(result.data(),result.size());throw;}
}

static std::string walletPath(std::string name) {
    name = trim(name);
    if (name.size() < 5 || lower(name.substr(name.size() - 5)) != ".keys") {
        name += ".keys";
    }
    return name;
}

static void cleanse(std::string& s) {
    if (!s.empty()) OPENSSL_cleanse(s.data(), s.size());
    s.clear();
    s.shrink_to_fit();
}

static std::uint32_t readU32(std::ifstream& in) {
    unsigned char b[4]{};
    in.read(reinterpret_cast<char*>(b), 4);
    if (!in) throw std::runtime_error("Invalid wallet file header");
    return (static_cast<std::uint32_t>(b[0]) << 24) |
           (static_cast<std::uint32_t>(b[1]) << 16) |
           (static_cast<std::uint32_t>(b[2]) << 8) |
           static_cast<std::uint32_t>(b[3]);
}

static std::array<unsigned char, 32> deriveWalletKey(
    const std::string& password,
    const unsigned char* salt,
    std::uint32_t iterations)
{
    std::array<unsigned char, 32> key{};
    if (password.empty()) throw std::runtime_error("Wallet password cannot be empty");
    if (PKCS5_PBKDF2_HMAC(
            password.c_str(), static_cast<int>(password.size()),
            salt, static_cast<int>(cfg::SALT_SIZE),
            static_cast<int>(iterations), EVP_sha256(),
            static_cast<int>(key.size()), key.data()) != 1) {
        throw std::runtime_error("Unable to derive wallet encryption key");
    }
    return key;
}

static std::string decryptWalletFile(
    const std::string& filename,
    const std::string& password)
{
    if (fs::file_size(filename) > 1024 * 1024) throw std::runtime_error("Wallet file is too large");
    std::ifstream in(filename, std::ios::binary);
    if (!in) throw std::runtime_error("Wallet file not found: " + filename);

    char magic[8]{};
    in.read(magic, 8);
    if (!in || std::memcmp(magic, cfg::WALLET_MAGIC, 8) != 0) {
        throw std::runtime_error("Not a Venera CLI v0.2 wallet file");
    }

    const std::uint32_t iterations = readU32(in);
    if (iterations < 10000 || iterations > 5000000) {
        throw std::runtime_error("Invalid wallet KDF parameters");
    }

    std::array<unsigned char, cfg::SALT_SIZE> salt{};
    std::array<unsigned char, cfg::IV_SIZE> iv{};
    std::array<unsigned char, cfg::TAG_SIZE> tag{};

    in.read(reinterpret_cast<char*>(salt.data()), static_cast<std::streamsize>(salt.size()));
    in.read(reinterpret_cast<char*>(iv.data()), static_cast<std::streamsize>(iv.size()));
    in.read(reinterpret_cast<char*>(tag.data()), static_cast<std::streamsize>(tag.size()));
    if (!in) throw std::runtime_error("Truncated wallet file");

    std::vector<unsigned char> ciphertext(
        (std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    if (ciphertext.empty()) throw std::runtime_error("Wallet file has no encrypted payload");

    auto key = deriveWalletKey(password, salt.data(), iterations);
    EVP_CIPHER_CTX* raw = EVP_CIPHER_CTX_new();
    if (!raw) {
        OPENSSL_cleanse(key.data(), key.size());
        throw std::runtime_error("Unable to create cipher context");
    }
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(raw, EVP_CIPHER_CTX_free);

    if (EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_gcm(), nullptr, nullptr, nullptr) != 1 ||
        EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_SET_IVLEN, static_cast<int>(iv.size()), nullptr) != 1 ||
        EVP_DecryptInit_ex(ctx.get(), nullptr, nullptr, key.data(), iv.data()) != 1) {
        OPENSSL_cleanse(key.data(), key.size());
        throw std::runtime_error("Unable to initialize wallet decryption");
    }

    std::vector<unsigned char> plaintext(ciphertext.size() + EVP_MAX_BLOCK_LENGTH);
    int len = 0;
    int total = 0;
    if (EVP_DecryptUpdate(
            ctx.get(), plaintext.data(), &len,
            ciphertext.data(), static_cast<int>(ciphertext.size())) != 1) {
        OPENSSL_cleanse(key.data(), key.size());
        OPENSSL_cleanse(plaintext.data(), plaintext.size());
        throw std::runtime_error("Wallet decryption failed");
    }
    total += len;

    if (EVP_CIPHER_CTX_ctrl(
            ctx.get(), EVP_CTRL_GCM_SET_TAG,
            static_cast<int>(tag.size()), tag.data()) != 1) {
        OPENSSL_cleanse(key.data(), key.size());
        OPENSSL_cleanse(plaintext.data(), plaintext.size());
        throw std::runtime_error("Wallet authentication setup failed");
    }

    const int finalResult = EVP_DecryptFinal_ex(ctx.get(), plaintext.data() + total, &len);
    OPENSSL_cleanse(key.data(), key.size());

    if (finalResult <= 0) {
        OPENSSL_cleanse(plaintext.data(), plaintext.size());
        throw std::runtime_error("Incorrect password or corrupted wallet file");
    }
    total += len;

    std::string seed(reinterpret_cast<char*>(plaintext.data()), static_cast<std::size_t>(total));
    OPENSSL_cleanse(plaintext.data(), plaintext.size());
    return seed;
}

#include <limits>
#include <cstdlib>
#include <regex>

static std::string textField(json_object* o, const char* k, std::string fallback="") {
    json_object* v=nullptr;
    return o && json_object_object_get_ex(o,k,&v) && v ? json_object_get_string(v) : fallback;
}
static JsonPtr parse(const std::string& s) {
    const auto compatible = venera_json_prepare_parse(s);
    JsonPtr p(json_tokener_parse(compatible.c_str()));
    if (!p || json_object_get_type(p.get())!=json_type_object) throw std::runtime_error("Invalid JSON response/configuration");
    return p;
}
static void str(json_object* o,const char* k,const std::string& v) {json_object_object_add(o,k,json_object_new_string(v.c_str()));}
static std::string encode(json_object* o) {return venera_json_restore_uint64(json_object_to_json_string_ext(o,JSON_C_TO_STRING_PLAIN));}
static size_t receiveHttp(char* p,size_t a,size_t b,void* out) {
    auto& s=*static_cast<std::string*>(out);
    if(a*b>32*1024*1024-s.size()) return 0;
    s.append(p,a*b);return a*b;
}
static bool loopback(const std::string& url) {
    return std::regex_match(url,std::regex(R"(https?://(127\.0\.0\.1|localhost|\[::1\])(:[0-9]+)?/?)"));
}
static void validUrl(const std::string& u, bool localOnly=false) {
    if (!std::regex_match(u,std::regex(R"(https?://[A-Za-z0-9.\[\]:-]+/?)")))
        throw std::runtime_error("Expected http(s)://host:port without credentials/path");
    if(localOnly&&!loopback(u)) throw std::runtime_error("Wallet RPC must be on loopback; seeds/keys are sent only to your local wallet process");
    if(!loopback(u)&&u.rfind("https://",0)!=0) throw std::runtime_error("Remote daemon RPC requires HTTPS");
}
struct RpcTls {std::string caFile,publicKeyPin;};
class Rpc {
    std::string url_,user_,pass_;
    long timeout_;
    RpcTls tls_;
public:
    Rpc(std::string u,std::string user="",std::string pass="",long timeout=1800,RpcTls tls={}):url_(std::move(u)),user_(std::move(user)),pass_(std::move(pass)),timeout_(timeout),tls_(std::move(tls)){
        if(!tls_.caFile.empty()||!tls_.publicKeyPin.empty()){
            if(!loopback(url_)||url_.rfind("https://",0)!=0||tls_.caFile.empty()||tls_.publicKeyPin.rfind("sha256//",0)!=0)
                throw std::runtime_error("Invalid pinned local RPC transport");
        }
#ifndef VENERA_TEST_EXTERNAL_RPC
        if(!user_.empty()&&tls_.caFile.empty())throw std::runtime_error("Wallet RPC requires authenticated local TLS");
#endif
    }
    ~Rpc(){cleanse(pass_);}
    JsonPtr post(const std::string& path,json_object* body,bool acceptError=false) {
        wallet_interrupts::check();
        std::unique_ptr<CURL,decltype(&curl_easy_cleanup)> c(curl_easy_init(),curl_easy_cleanup);
        if(!c)throw std::runtime_error("curl init failed");
        std::string out,payload=encode(body),url=url_+path;
        struct Scrub {std::string& value;~Scrub(){cleanse(value);}} scrubOut{out},scrubPayload{payload};
        curl_slist* h=curl_slist_append(nullptr,"Content-Type: application/json");
        std::unique_ptr<curl_slist,decltype(&curl_slist_free_all)> hdr(h,curl_slist_free_all);
        curl_easy_setopt(c.get(),CURLOPT_URL,url.c_str());
#if LIBCURL_VERSION_NUM >= 0x075500
        const auto protocolResult=curl_easy_setopt(c.get(),CURLOPT_PROTOCOLS_STR,"http,https");
#else
         
        const auto protocolResult=curl_easy_setopt(c.get(),CURLOPT_PROTOCOLS,
            static_cast<long>(CURLPROTO_HTTP | CURLPROTO_HTTPS));
#endif
        if(protocolResult!=CURLE_OK)throw std::runtime_error("Cannot restrict RPC transport to HTTP/HTTPS");
        curl_easy_setopt(c.get(),CURLOPT_FOLLOWLOCATION,0L);
        curl_easy_setopt(c.get(),CURLOPT_NOSIGNAL,1L);
        curl_easy_setopt(c.get(),CURLOPT_NOPROXY,"localhost,127.0.0.1,::1");
        curl_easy_setopt(c.get(),CURLOPT_POSTFIELDS,payload.c_str());
        curl_easy_setopt(c.get(),CURLOPT_HTTPHEADER,h);
        curl_easy_setopt(c.get(),CURLOPT_WRITEFUNCTION,receiveHttp);
        curl_easy_setopt(c.get(),CURLOPT_WRITEDATA,&out);
        curl_easy_setopt(c.get(),CURLOPT_CONNECTTIMEOUT,std::min(10L,timeout_));
        curl_easy_setopt(c.get(),CURLOPT_TIMEOUT,timeout_);
        curl_easy_setopt(c.get(),CURLOPT_NOPROGRESS,0L);
        curl_easy_setopt(c.get(),CURLOPT_XFERINFOFUNCTION,wallet_interrupts::transferProgress);
        curl_easy_setopt(c.get(),CURLOPT_SSL_VERIFYPEER,1L);
        curl_easy_setopt(c.get(),CURLOPT_SSL_VERIFYHOST,2L);
#if defined(_WIN32) && LIBCURL_VERSION_NUM >= 0x080d00
        if(tls_.caFile.empty()){
            if(curl_easy_setopt(c.get(),CURLOPT_SSL_OPTIONS,static_cast<long>(CURLSSLOPT_NATIVE_CA))!=CURLE_OK)throw std::runtime_error("Windows native certificate verification is unavailable");
            curl_easy_setopt(c.get(),CURLOPT_CAINFO,static_cast<const char*>(nullptr));
            curl_easy_setopt(c.get(),CURLOPT_CAPATH,static_cast<const char*>(nullptr));
        }
#endif
        if(!tls_.caFile.empty()){
            if(curl_easy_setopt(c.get(),CURLOPT_CAINFO,tls_.caFile.c_str())!=CURLE_OK||
               curl_easy_setopt(c.get(),CURLOPT_PINNEDPUBLICKEY,tls_.publicKeyPin.c_str())!=CURLE_OK)
                throw std::runtime_error("This curl build cannot authenticate the local wallet server");
        }
        if(!user_.empty()) {
            curl_easy_setopt(c.get(),CURLOPT_HTTPAUTH,CURLAUTH_DIGEST);
            curl_easy_setopt(c.get(),CURLOPT_USERNAME,user_.c_str());
            curl_easy_setopt(c.get(),CURLOPT_PASSWORD,pass_.c_str());
        }
        auto rc=curl_easy_perform(c.get());cleanse(payload);
        wallet_interrupts::check();
        if(rc!=CURLE_OK)throw std::runtime_error(std::string("RPC connection failed: ")+curl_easy_strerror(rc));
        long status=0;curl_easy_getinfo(c.get(),CURLINFO_RESPONSE_CODE,&status);
        if(status!=200&&!acceptError)throw std::runtime_error("RPC HTTP status "+std::to_string(status));
        return parse(out);
    }
    JsonPtr call(const std::string& method,json_object* params,const std::string& path="/json_rpc",bool allowNull=false) {
        JsonPtr q(json_object_new_object());str(q.get(),"jsonrpc","2.0");str(q.get(),"id","wallet");str(q.get(),"method",method);
        json_object_object_add(q.get(),"params",json_object_get(params));
        auto r=post(path,q.get());json_object* value=nullptr;
        if(json_object_object_get_ex(r.get(),"error",&value))throw std::runtime_error("RPC: "+textField(value,"message","Unknown error"));
        if(!json_object_object_get_ex(r.get(),"result",&value))throw std::runtime_error("RPC response missing result");
        if(!value){if(allowNull)return JsonPtr(nullptr);throw std::runtime_error("RPC returned null result");}
        return JsonPtr(json_object_get(value));
    }
};
static JsonPtr obj(){return JsonPtr(json_object_new_object());}

static uint64_t atomicAmount(const std::string& s) {
    if(!std::regex_match(s,std::regex(R"([0-9]+(\.[0-9]{1,9})?)")))throw std::runtime_error("VNR amount requires positive decimal with at most 9 decimals");
    auto pos=s.find('.');auto whole=s.substr(0,pos);auto frac=pos==s.npos?std::string():s.substr(pos+1);frac.append(9-frac.size(),'0');
    std::string digits=whole+frac;uint64_t n=0;
    for(char c:digits){unsigned d=c-'0';if(n>(UINT64_MAX-d)/10)throw std::runtime_error("Amount overflow");n=n*10+d;}
    if(!n)throw std::runtime_error("Amount must be positive");
    return n;
}
static std::string amount(uint64_t n){std::ostringstream o;o<<n/1000000000<<'.'<<std::setw(9)<<std::setfill('0')<<n%1000000000;return o.str();}
static uint64_t number(json_object* o,const char* k){json_object* v=nullptr;if(!json_object_object_get_ex(o,k,&v))throw std::runtime_error(std::string("Missing ")+k);return venera_json_get_uint64(v);}
using ProcessId=std::intptr_t;
static ProcessId launch(const std::vector<std::string>& args, bool background=false,const fs::path& logPath="venera-node.log",bool stopWithParent=false) {
    std::vector<const char*> av;for(auto& a:args)av.push_back(a.c_str());av.push_back(nullptr);
    std::cout.flush();std::cerr.flush();
    wallet_interrupts::ForegroundScope foreground(!background);
#ifdef _WIN32
    auto result=windows_wallet::launch(args,background,logPath,stopWithParent);
    if(!background&&result==130)throw WalletExit{};
    if(!background&&(result==131||result==132)){wallet_interrupts::armed=1;return result==131?0:1;}
    return result;
#else
#ifdef __linux__
    const pid_t parent=getpid();
#endif
    pid_t child=fork();if(child<0)throw std::runtime_error("fork failed");
    if(!child){
        if(!background)std::signal(SIGINT,SIG_IGN);  
#ifdef __linux__
        if(stopWithParent&&(prctl(PR_SET_PDEATHSIG,SIGTERM)<0||getppid()!=parent))_exit(126);
#endif
        if(background){setsid();int log=open(logPath.c_str(),O_WRONLY|O_CREAT|O_APPEND,0600);if(log<0)_exit(126);dup2(log,1);dup2(log,2);close(log);int in=open("/dev/null",O_RDONLY);dup2(in,0);close(in);}
        execvp(av[0],const_cast<char* const*>(av.data()));_exit(127);
    }
    if(background)return child;
    int status;pid_t waited;do{waited=waitpid(child,&status,0);}while(waited<0&&errno==EINTR);
    if(waited<0)throw std::runtime_error("waitpid failed");
    if(WIFEXITED(status)&&WEXITSTATUS(status)==130)throw WalletExit{};
     
     
    if(WIFEXITED(status)&&(WEXITSTATUS(status)==131||WEXITSTATUS(status)==132)){
        wallet_interrupts::armed=1;return WEXITSTATUS(status)==131?0:1;
    }
    return WIFEXITED(status)?WEXITSTATUS(status):1;
#endif
}
static void replaceWalletFile(const fs::path& from,const fs::path& to) {
#ifdef _WIN32
    if(!MoveFileExW(from.wstring().c_str(),to.wstring().c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH))throw std::runtime_error("Cannot replace wallet settings/receipts");
#else
    fs::rename(from,to);
#endif
}
static void saveConfig(const fs::path& path,json_object* o) {
    const auto temp=path.string()+".tmp";
#ifndef _WIN32
    int fd=open(temp.c_str(),O_WRONLY|O_CREAT|O_EXCL,0600);if(fd<0)throw std::runtime_error("Cannot create settings temp file");close(fd);
#endif
#ifdef _WIN32
    writePrivateExclusive(temp,encode(o)+"\n");
#else
    {std::ofstream f(temp);f<<encode(o)<<'\n';if(!f)throw std::runtime_error("Settings write failed");}
#endif
    replaceWalletFile(temp,path);
}
#include "native_evm.h"
#include "local_wallet.h"
#include "wallet_display.h"
#include "wallet_session.h"

static void showHelp() {
    std::cout<<ansi::bold<<"Commands:\n"<<ansi::reset;
    for(const auto& entry:std::vector<std::pair<std::string,std::string>>{
        {"balance","[VNR|BNB|USDT|USDC|all]"},
        {"refresh",""},
        {"receive","[VNR|BNB|USDT|USDC]"},
        {"send","VNR|BNB|USDT|USDC ADDRESS AMOUNT"},
        {"burn","AMOUNT"},
        {"stake","AMOUNT 30|60|120|360"},
        {"burn_stats",""},
        {"stake_stats",""},
        {"claim_burn_yield",""},
        {"claim_status","[REQUEST_ID]"},
        {"rewards_sync",""},
        {"register_burn","TXID (retry only)"},
        {"register_stake","TXID DAYS (retry only)"},
        {"history",""},
        {"seed",""},
        {"receipt","HASH"},
        {"set-node","http(s)://HOST:RPC_PORT"},
        {"status",""},
        {"help",""},
        {"quit",""}}){
        std::cout<<"  "<<ansi::cyan<<ansi::bold<<std::left<<std::setw(20)<<entry.first
            <<ansi::reset<<ansi::dim<<entry.second<<ansi::reset<<'\n';
    }
    std::cout<<std::right;
}
#include "desktop_service.h"
int main(int argc,char** argv) {
    const bool helper=argc==4&&std::string(argv[1])=="--wallet-operation";
    try {
#ifndef _WIN32
        umask(0077);
#endif
        if(curl_global_init(CURL_GLOBAL_DEFAULT)!=CURLE_OK)throw std::runtime_error("curl init failed");
        if(argc==2&&std::string(argv[1])=="--self-test"){evm::selfTest();return 0;}
         
        if(argc>=2&&std::string(argv[1])=="--check-bsc-rpc"){
            std::string url=evm::DEFAULT_RPC;
#ifdef VENERA_TEST_EXTERNAL_RPC
            if(argc==3)url=argv[2];
            else if(argc!=2)throw std::runtime_error("Usage: --check-bsc-rpc");
#else
            if(argc!=2)throw std::runtime_error("Usage: --check-bsc-rpc");
#endif
            evm::checkRpcConnection(url);return 0;
        }
        protectProcessMemory();
        wallet_interrupts::install();
        if(argc==2&&std::string(argv[1])=="--desktop-service")return desktop::run(argv[0]);
        if(helper){auto result=walletOperation(argv[2],argv[3],argv[0]);return result==0&&wallet_interrupts::armed?131:result;}
        enableAnsiColors();
        fs::path cfgPath=argc>1?argv[1]:"venera-local.json";
        auto config=obj();
        if(fs::exists(cfgPath)){std::ifstream f(cfgPath);config=parse(std::string(std::istreambuf_iterator<char>(f),{}));}
        auto external=textField(config.get(),"wallet_rpc");if(!external.empty())validUrl(external,true);
        auto daemonUrl=textField(config.get(),"daemon_rpc","http://127.0.0.1:29930");validUrl(daemonUrl);
        WalletSnapshot snapshot;
        runWalletOperation(argv[0],cfgPath,snapshot,"startup");
        showVnrBalance(snapshot,daemonUrl);
        std::cout<<"Type help for commands.\n";
        std::vector<std::string> history;
        for(std::string line;readCommandLine(std::string(ansi::cyan)+"venera> "+ansi::reset,history,line);) {
            auto a=splitCommand(line);if(a.empty())continue;
            try {
                if(fs::exists(cfgPath)){std::ifstream f(cfgPath);config=parse(std::string(std::istreambuf_iterator<char>(f),{}));}
                auto p=obj();auto cmd=lower(a[0]);
                if(cmd=="quit"||cmd=="exit")break;
                if(cmd=="help"){showHelp();continue;}
                if(cmd=="evm") {
                    std::vector<std::string> args(a.begin()+1,a.end());
                    if(!args.empty()&&lower(args[0])=="send")runWalletOperation(argv[0],cfgPath,snapshot,"bsc",args);
                    else evm::command(args,config.get(),cfgPath,snapshot.bsc);
                    continue;
                }
                if(cmd=="set-node") {
                    if(a.size()!=2)throw std::runtime_error("Usage: set-node URL");
                    validUrl(a[1]);
                    daemonUrl=a[1];str(config.get(),"daemon_rpc",daemonUrl);saveConfig(cfgPath,config.get());snapshot.possiblyStale=true;std::cout<<ansi::green<<"Daemon endpoint saved; applied on next refresh."<<ansi::reset<<'\n';continue;
                }
                if(cmd=="status"){Rpc daemon(daemonUrl);auto r=daemon.call("get_info",p.get());showNodeStatus(r.get());continue;}
                if(cmd=="close")break;
                if(cmd=="receipt"){
                    evm::command(a,config.get(),cfgPath,snapshot.bsc);continue;
                }
                if(cmd=="send"&&a.size()>=2&&lower(a[1])!="vnr"){
                    runWalletOperation(argv[0],cfgPath,snapshot,"bsc",a);continue;
                }
                if(cmd=="seed") {runWalletOperation(argv[0],cfgPath,snapshot,"seed");continue;}
                if(cmd=="receive"){
                    if(a.size()>2)throw std::runtime_error("Usage: receive [VNR|BNB|USDT|USDC]");
                    auto asset=a.size()==2?lower(a[1]):"all";
                    if(asset!="all"&&asset!="vnr"&&asset!="bnb"&&asset!="usdt"&&asset!="usdc")throw std::runtime_error("Unknown asset");
                    if(asset=="all"||asset=="vnr")std::cout<<ansi::cyan<<"VNR: "<<snapshot.vnr<<ansi::reset<<'\n';
                    if(asset!="vnr")std::cout<<ansi::cyan<<"BSC (BNB/USDT/USDC): "<<snapshot.bsc<<ansi::reset<<'\n';
                    continue;
                }
                if(cmd=="refresh") {
                    if(a.size()!=1)throw std::runtime_error("Usage: refresh");
                    runWalletOperation(argv[0],cfgPath,snapshot,"refresh");showVnrBalance(snapshot,daemonUrl);continue;
                }
                if(cmd=="balance") {
                    if(a.size()>2)throw std::runtime_error("Usage: balance [VNR|BNB|USDT|USDC|all]");
                    auto asset=a.size()==2?lower(a[1]):"vnr";
                    if(asset=="vnr"||asset=="all")showVnrBalance(snapshot,daemonUrl);
                    if(asset!="vnr")evm::command({"balance",asset},config.get(),cfgPath,snapshot.bsc);
                    continue;
                }
                if(cmd=="history") {runWalletOperation(argv[0],cfgPath,snapshot,"history");continue;}
                if(cmd=="burn") {
                    if(a.size()!=2)throw std::runtime_error("Usage: burn AMOUNT");
                    atomicAmount(a[1]);runWalletOperation(argv[0],cfgPath,snapshot,"vnr",a);continue;
                }
                if(cmd=="send") {
                    if(a.size()!=4||lower(a[1])!="vnr")throw std::runtime_error("Usage: send VNR|BNB|USDT|USDC ADDRESS AMOUNT");
                    atomicAmount(a[3]);runWalletOperation(argv[0],cfgPath,snapshot,"vnr",a);continue;
                }
                if(cmd=="stake"||cmd=="burn_stats"||cmd=="stake_stats"||cmd=="claim_burn_yield"||cmd=="claim_status"||cmd=="rewards_sync"||cmd=="register_burn"||cmd=="register_stake"){
                    rewards::origin(config.get());
                    if(cmd=="stake"){if(a.size()!=3||!rewards::lockPeriod(a[2]))throw std::runtime_error("Usage: stake AMOUNT 30|60|120|360");atomicAmount(a[1]);}
                    else if(cmd=="register_burn"){if(a.size()!=2)throw std::runtime_error("Usage: register_burn TXID");rewards::hash(a[1]);}
                    else if(cmd=="register_stake"){if(a.size()!=3||!rewards::lockPeriod(a[2]))throw std::runtime_error("Usage: register_stake TXID 30|60|120|360");rewards::hash(a[1]);}
                    else if(cmd=="claim_status"){if(a.size()>2)throw std::runtime_error("Usage: claim_status [REQUEST_ID]");}
                    else if(a.size()!=1)throw std::runtime_error("Unexpected command arguments");
                    runWalletOperation(argv[0],cfgPath,snapshot,"rewards",a);continue;
                }
                throw std::runtime_error("Unknown command; type help");
            }catch(const std::exception& e){std::cerr<<ansi::red<<"Error: "<<e.what()<<ansi::reset<<'\n';}
        }
        return 0;
    }catch(const WalletExit&){return helper?130:0;}
    catch(const std::exception& e){
        if(wallet_interrupts::quit)return helper?130:0;
        wallet_interrupts::showNotice();
        std::cerr<<ansi::red<<"Error: "<<e.what()<<ansi::reset<<'\n';return helper&&wallet_interrupts::armed?132:1;
    }
}
