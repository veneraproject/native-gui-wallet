                                       
#pragma once
#ifdef _WIN32
#include <aclapi.h>
#include <sddl.h>

namespace windows_wallet {
struct Handle {
    HANDLE value=INVALID_HANDLE_VALUE;
    explicit Handle(HANDLE h=INVALID_HANDLE_VALUE):value(h){}
    ~Handle(){if(value&&value!=INVALID_HANDLE_VALUE)CloseHandle(value);}
    Handle(const Handle&)=delete;
    Handle& operator=(const Handle&)=delete;
    HANDLE release(){auto h=value;value=INVALID_HANDLE_VALUE;return h;}
};
static std::wstring currentSid(){
    Handle token;HANDLE h=nullptr;
    if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&h))throw std::runtime_error("Cannot read Windows user token");
    token.value=h;DWORD size=0;GetTokenInformation(h,TokenUser,nullptr,0,&size);
    std::vector<unsigned char> data(size);
    if(!size||!GetTokenInformation(h,TokenUser,data.data(),size,&size))throw std::runtime_error("Cannot read Windows user identity");
    LPWSTR sid=nullptr;
    if(!ConvertSidToStringSidW(reinterpret_cast<TOKEN_USER*>(data.data())->User.Sid,&sid))throw std::runtime_error("Cannot encode Windows identity");
    std::wstring result(sid);LocalFree(sid);return result;
}
struct PrivateSecurity {
    PSECURITY_DESCRIPTOR descriptor=nullptr;
    SECURITY_ATTRIBUTES attributes{};
    PrivateSecurity(){
        const auto text=L"D:P(A;OICI;FA;;;SY)(A;OICI;FA;;;BA)(A;OICI;FA;;;"+currentSid()+L")";
        if(!ConvertStringSecurityDescriptorToSecurityDescriptorW(text.c_str(),SDDL_REVISION_1,&descriptor,nullptr))throw std::runtime_error("Cannot create private Windows security descriptor");
        attributes.nLength=sizeof(attributes);attributes.lpSecurityDescriptor=descriptor;attributes.bInheritHandle=FALSE;
    }
    ~PrivateSecurity(){if(descriptor)LocalFree(descriptor);}
};
static bool privateDirectory(const fs::path& path,bool allowExisting=false){
    PrivateSecurity security;
    if(CreateDirectoryW(path.wstring().c_str(),&security.attributes))return true;
    if(!allowExisting||GetLastError()!=ERROR_ALREADY_EXISTS)return false;
    const auto flags=GetFileAttributesW(path.wstring().c_str());
    if(flags==INVALID_FILE_ATTRIBUTES||!(flags&FILE_ATTRIBUTE_DIRECTORY)||(flags&FILE_ATTRIBUTE_REPARSE_POINT))return false;
    BOOL present=FALSE,defaulted=FALSE;PACL acl=nullptr;
    if(!GetSecurityDescriptorDacl(security.descriptor,&present,&acl,&defaulted)||!present)return false;
    auto name=path.wstring();
    return SetNamedSecurityInfoW(&name[0],SE_FILE_OBJECT,DACL_SECURITY_INFORMATION|PROTECTED_DACL_SECURITY_INFORMATION,nullptr,nullptr,acl,nullptr)==ERROR_SUCCESS;
}
static bool trustedSid(PSID sid){
    LPWSTR encoded=nullptr;if(!sid||!ConvertSidToStringSidW(sid,&encoded))return false;
    std::wstring value(encoded);LocalFree(encoded);
    return value==currentSid()||value==L"S-1-5-18"||value==L"S-1-5-32-544"||
        value==L"S-1-5-80-956008885-3418522649-1831038044-1853292631-2271478464";
}
static void trustedPath(const fs::path& path){
    for(auto part=path;!part.empty();part=part.parent_path()){
        auto name=part.wstring();const auto flags=GetFileAttributesW(name.c_str());
        if(flags==INVALID_FILE_ATTRIBUTES||(flags&FILE_ATTRIBUTE_REPARSE_POINT))throw std::runtime_error("Executable path contains an unsafe Windows reparse point");
        PSECURITY_DESCRIPTOR descriptor=nullptr;PSID owner=nullptr;PACL acl=nullptr;
        auto result=GetNamedSecurityInfoW(&name[0],SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,&owner,nullptr,&acl,nullptr,&descriptor);
        if(result!=ERROR_SUCCESS||!descriptor)throw std::runtime_error("Cannot check executable Windows permissions");
        bool safe=acl&&trustedSid(owner);
        DWORD dangerous=DELETE|WRITE_DAC|WRITE_OWNER|FILE_DELETE_CHILD|GENERIC_ALL|GENERIC_WRITE;
        if(part==path||part==path.parent_path())dangerous|=FILE_WRITE_DATA|FILE_APPEND_DATA;
        if(acl)for(DWORD i=0;i<acl->AceCount&&safe;++i){
            void* raw=nullptr;if(!GetAce(acl,i,&raw)){safe=false;break;}
            auto header=static_cast<ACE_HEADER*>(raw);if(header->AceFlags&INHERIT_ONLY_ACE)continue;
            if(header->AceType==ACCESS_ALLOWED_ACE_TYPE){auto ace=static_cast<ACCESS_ALLOWED_ACE*>(raw);if((ace->Mask&dangerous)&&!trustedSid(&ace->SidStart))safe=false;}
            else if(header->AceType!=ACCESS_DENIED_ACE_TYPE)safe=false;
        }
        LocalFree(descriptor);
        if(!safe)throw std::runtime_error("Executable directory is writable by an untrusted Windows account: "+part.string());
        if(part==part.parent_path())break;
    }
}
static std::string quote(const std::string& value){
    std::string out="\"";size_t slashes=0;
    for(char c:value){
        if(c=='\\'){++slashes;continue;}
        if(c=='\"'){out.append(slashes*2+1,'\\');out+='\"';}
        else {out.append(slashes,'\\');out+=c;}
        slashes=0;
    }
    out.append(slashes*2,'\\');out+='\"';return out;
}
static HANDLE childJob(){
    static Handle job([]{
        HANDLE h=CreateJobObjectW(nullptr,nullptr);if(!h)throw std::runtime_error("Cannot create wallet process job");
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION info{};info.BasicLimitInformation.LimitFlags=JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        if(!SetInformationJobObject(h,JobObjectExtendedLimitInformation,&info,sizeof(info))){CloseHandle(h);throw std::runtime_error("Cannot protect child process lifetime");}
        return h;
    }());return job.value;
}
static std::intptr_t launch(const std::vector<std::string>& args,bool background,const fs::path& logPath,bool stopWithParent){
    if(args.empty()||!fs::path(args[0]).is_absolute())throw std::runtime_error("Expected an absolute wallet executable path");
    std::string command;for(const auto& arg:args){if(!command.empty())command+=' ';command+=quote(arg);}
    std::vector<char> buffer(command.begin(),command.end());buffer.push_back(0);
    STARTUPINFOEXA startup{};startup.StartupInfo.cb=sizeof(startup);
    startup.StartupInfo.dwFlags=STARTF_USESTDHANDLES;
    Handle input,output,error;
    if(background){
        SECURITY_ATTRIBUTES attributes{sizeof(attributes),nullptr,TRUE};
        input.value=CreateFileW(L"NUL",GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,&attributes,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr);
        output.value=CreateFileW(logPath.wstring().c_str(),FILE_APPEND_DATA,FILE_SHARE_READ|FILE_SHARE_WRITE,&attributes,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,nullptr);
        if(output.value!=INVALID_HANDLE_VALUE)DuplicateHandle(GetCurrentProcess(),output.value,GetCurrentProcess(),&error.value,0,TRUE,DUPLICATE_SAME_ACCESS);
    }else{
        for(auto pair:{std::make_pair(&input,STD_INPUT_HANDLE),std::make_pair(&output,STD_OUTPUT_HANDLE),std::make_pair(&error,STD_ERROR_HANDLE)})
            if(!DuplicateHandle(GetCurrentProcess(),GetStdHandle(pair.second),GetCurrentProcess(),&pair.first->value,0,TRUE,DUPLICATE_SAME_ACCESS))throw std::runtime_error("Cannot inherit wallet terminal");
    }
    if(input.value==INVALID_HANDLE_VALUE||output.value==INVALID_HANDLE_VALUE||error.value==INVALID_HANDLE_VALUE)throw std::runtime_error("Cannot open child process streams");
    HANDLE inherited[]={input.value,output.value,error.value};SIZE_T size=0;
    InitializeProcThreadAttributeList(nullptr,1,0,&size);std::vector<unsigned char> attributes(size);
    startup.lpAttributeList=reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(attributes.data());
    if(!InitializeProcThreadAttributeList(startup.lpAttributeList,1,0,&size))throw std::runtime_error("Cannot initialize child handle list");
    struct AttributeCleanup{LPPROC_THREAD_ATTRIBUTE_LIST p;~AttributeCleanup(){DeleteProcThreadAttributeList(p);}} cleanup{startup.lpAttributeList};
    if(!UpdateProcThreadAttribute(startup.lpAttributeList,0,PROC_THREAD_ATTRIBUTE_HANDLE_LIST,inherited,sizeof(inherited),nullptr,nullptr))throw std::runtime_error("Cannot restrict child handle inheritance");
    startup.StartupInfo.hStdInput=input.value;startup.StartupInfo.hStdOutput=output.value;startup.StartupInfo.hStdError=error.value;
    HANDLE job=stopWithParent?childJob():nullptr;PROCESS_INFORMATION process{};
    DWORD flags=CREATE_SUSPENDED|EXTENDED_STARTUPINFO_PRESENT|(background?CREATE_NO_WINDOW:0);
    if(!CreateProcessA(args[0].c_str(),buffer.data(),nullptr,nullptr,TRUE,flags,nullptr,nullptr,&startup.StartupInfo,&process))throw std::runtime_error("Cannot launch local wallet process");
    Handle child(process.hProcess),thread(process.hThread);
    if((job&&!AssignProcessToJobObject(job,child.value))||ResumeThread(thread.value)==static_cast<DWORD>(-1)){
        TerminateProcess(child.value,126);WaitForSingleObject(child.value,5000);throw std::runtime_error("Cannot supervise local wallet process");
    }
    if(background)return reinterpret_cast<std::intptr_t>(child.release());
    if(WaitForSingleObject(child.value,INFINITE)!=WAIT_OBJECT_0)throw std::runtime_error("Cannot wait for wallet operation");
    DWORD status=1;if(!GetExitCodeProcess(child.value,&status))throw std::runtime_error("Cannot read wallet operation status");return status;
}
}
#endif
