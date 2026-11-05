#pragma once
 
 
static RpcTls createLocalTlsIdentity(const fs::path& directory) {
    using Bio=std::unique_ptr<BIO,decltype(&BIO_free)>;
    using Cert=std::unique_ptr<X509,decltype(&X509_free)>;
    using Pkey=std::unique_ptr<EVP_PKEY,decltype(&EVP_PKEY_free)>;
    using Pctx=std::unique_ptr<EVP_PKEY_CTX,decltype(&EVP_PKEY_CTX_free)>;
    Pctx context(EVP_PKEY_CTX_new_id(EVP_PKEY_RSA,nullptr),EVP_PKEY_CTX_free);
    evm::check(bool(context)&&EVP_PKEY_keygen_init(context.get())>0&&EVP_PKEY_CTX_set_rsa_keygen_bits(context.get(),2048)>0,"Cannot initialize local TLS identity");
    EVP_PKEY* raw=nullptr;evm::check(EVP_PKEY_keygen(context.get(),&raw)>0,"Cannot generate local TLS identity");
    Pkey key(raw,EVP_PKEY_free);Cert cert(X509_new(),X509_free);
    evm::check(bool(cert)&&X509_set_version(cert.get(),2)==1,"Cannot allocate local TLS certificate");
    auto serial=evm::random(16);serial[0]&=0x7f;serial[0]|=1;
    auto serialNumber=evm::fromBytes(serial);ASN1_INTEGER* serialAsn=BN_to_ASN1_INTEGER(serialNumber.get(),nullptr);
    evm::check(serialAsn!=nullptr,"Cannot create TLS serial");
    const bool serialOk=X509_set_serialNumber(cert.get(),serialAsn)==1;ASN1_INTEGER_free(serialAsn);
    evm::check(serialOk&&X509_gmtime_adj(X509_getm_notBefore(cert.get()),-60)&&X509_gmtime_adj(X509_getm_notAfter(cert.get()),86400)&&X509_set_pubkey(cert.get(),key.get())==1,"Cannot initialize TLS certificate");
    auto name=X509_get_subject_name(cert.get());
    evm::check(name&&X509_NAME_add_entry_by_txt(name,"CN",MBSTRING_ASC,reinterpret_cast<const unsigned char*>("Venera local wallet session"),-1,-1,0)==1&&X509_set_issuer_name(cert.get(),name)==1,"Cannot set TLS subject");
    X509V3_CTX extensions;X509V3_set_ctx(&extensions,cert.get(),cert.get(),nullptr,nullptr,0);
    for(const auto& entry:std::vector<std::pair<int,const char*>>{{NID_basic_constraints,"critical,CA:FALSE"},{NID_key_usage,"critical,digitalSignature,keyEncipherment"},{NID_ext_key_usage,"serverAuth"},{NID_subject_alt_name,"IP:127.0.0.1"}}){
        X509_EXTENSION* extension=X509V3_EXT_conf_nid(nullptr,&extensions,entry.first,const_cast<char*>(entry.second));
        evm::check(extension!=nullptr,"Cannot create TLS extension");
        const bool ok=X509_add_ext(cert.get(),extension,-1)==1;X509_EXTENSION_free(extension);evm::check(ok,"Cannot set TLS extension");
    }
    evm::check(X509_sign(cert.get(),key.get(),EVP_sha256())>0,"Cannot sign local TLS certificate");
    Bio privatePem(BIO_new(BIO_s_mem()),BIO_free),certificatePem(BIO_new(BIO_s_mem()),BIO_free);
    evm::check(bool(privatePem)&&bool(certificatePem)&&PEM_write_bio_PrivateKey(privatePem.get(),key.get(),nullptr,nullptr,0,nullptr,nullptr)==1&&PEM_write_bio_X509(certificatePem.get(),cert.get())==1,"Cannot serialize TLS identity");
    char* bytes=nullptr;long length=BIO_get_mem_data(privatePem.get(),&bytes);
    evm::check(length>0,"Empty TLS key");evm::Password pem(std::string(bytes,static_cast<size_t>(length)));OPENSSL_cleanse(bytes,static_cast<size_t>(length));
    writePrivateExclusive(directory/"tls.key",pem.value);
    length=BIO_get_mem_data(certificatePem.get(),&bytes);evm::check(length>0,"Empty TLS certificate");
    writePrivateExclusive(directory/"tls.crt",std::string(bytes,static_cast<size_t>(length)));
    const int derLength=i2d_PUBKEY(key.get(),nullptr);evm::check(derLength>0,"Cannot encode TLS public key");
    evm::Bytes der(static_cast<size_t>(derLength));auto cursor=der.data();evm::check(i2d_PUBKEY(key.get(),&cursor)==derLength,"Cannot encode TLS pin");
    std::array<unsigned char,32> digest{};evm::check(SHA256(der.data(),der.size(),digest.data())!=nullptr,"Cannot hash TLS pin");
    std::array<unsigned char,45> base64{};evm::check(EVP_EncodeBlock(base64.data(),digest.data(),digest.size())==44,"Cannot encode TLS pin");
    return {fs::absolute(directory/"tls.crt").string(),"sha256//"+std::string(reinterpret_cast<char*>(base64.data()),44)};
}
