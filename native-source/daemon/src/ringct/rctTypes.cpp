                                       
                                           
  
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#include "misc_log_ex.h"
#include "cryptonote_config.h"
#include "rctTypes.h"
#include "int-util.h"
using namespace crypto;
using namespace std;

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "ringct"

namespace rct {

     
     
     
    
    void dp(key a) {
        int j = 0;
        printf("\"");
        for (j = 0; j < 32; j++) {
            printf("%02x", (unsigned char)a.bytes[j]);
        }
        printf("\"");
        printf("\n");
    }

    void dp(bool a) {
        printf(" ... %s ... ", a ? "true" : "false");
        printf("\n");
    }

    void dp(const char * a, int l) {
        int j = 0;
        printf("\"");
        for (j = 0; j < l; j++) {
            printf("%02x", (unsigned char)a[j]);
        }
        printf("\"");
        printf("\n");
    }
    void dp(keyV a) {
        size_t j = 0;
        printf("[");
        for (j = 0; j < a.size(); j++) {
            dp(a[j]);
            if (j < a.size() - 1) {
                printf(",");
            }
        }
        printf("]");
        printf("\n");
    }
    void dp(keyM a) {
        size_t j = 0;
        printf("[");
        for (j = 0; j < a.size(); j++) {
            dp(a[j]);
            if (j < a.size() - 1) {
                printf(",");
            }
        }
        printf("]");
        printf("\n");
    }
    void dp(xmr_amount vali) {
        printf("x: ");
        std::cout << vali;
        printf("\n\n");
    }

    void dp(int vali) {
        printf("x: %d\n", vali);
        printf("\n");
    }
    void dp(bits amountb) {
        for (int i = 0; i < 64; i++) {
            printf("%d", amountb[i]);
        }
        printf("\n");

    }

    void dp(const char * st) {
        printf("%s\n", st);
    }

     
    
     
    void d2h(key & amounth, const xmr_amount in) {
        sc_0(amounth.bytes);
        memcpy_swap64le(amounth.bytes, &in, 1);
    }
    
     
    key d2h(const xmr_amount in) {
        key amounth;
        d2h(amounth, in);
        return amounth;
    }

     
    void d2b(bits  amountb, xmr_amount val) {
        int i = 0;
        while (i < 64) {
            amountb[i++] = val & 1;
            val >>= 1;
        }
    }
    
     
     
     
    xmr_amount h2d(const key & test) {
        xmr_amount vali = 0;
        int j = 0;
        for (j = 7; j >= 0; j--) {
            vali = (xmr_amount)(vali * 256 + (unsigned char)test.bytes[j]);
        }
        return vali;
    }
    
     
    void h2b(bits amountb2, const key & test) {
        int val = 0, i = 0, j = 0;
        for (j = 0; j < 8; j++) {
            val = (unsigned char)test.bytes[j];
            i = 0;
            while (i < 8) {
                amountb2[j*8+i++] = val & 1;
                val >>= 1;
            }
        }
    }
    
     
    void b2h(key & amountdh, const bits amountb2) {
        int byte, i, j;
        for (j = 0; j < 8; j++) {
            byte = 0;
            for (i = 7; i > -1; i--) {
                byte = byte * 2 + amountb2[8 * j + i];
            }
            amountdh[j] = (unsigned char)byte;
        }
        for (j = 8; j < 32; j++) {
            amountdh[j] = (unsigned char)(0x00);
        }
    }
    
     
    xmr_amount b2d(bits amountb) {
        xmr_amount vali = 0;
        int j = 0;
        for (j = 63; j >= 0; j--) {
            vali = (xmr_amount)(vali * 2 + amountb[j]);
        }
        return vali;
    }

    bool is_rct_simple(int type)
    {
        switch (type)
        {
            case RCTTypeSimple:
            case RCTTypeBulletproof:
            case RCTTypeBulletproof2:
            case RCTTypeCLSAG:
            case RCTTypeBulletproofPlus:
                return true;
            default:
                return false;
        }
    }

    bool is_rct_bulletproof(int type)
    {
        switch (type)
        {
            case RCTTypeBulletproof:
            case RCTTypeBulletproof2:
            case RCTTypeCLSAG:
                return true;
            default:
                return false;
        }
    }

    bool is_rct_bulletproof_plus(int type)
    {
        switch (type)
        {
            case RCTTypeBulletproofPlus:
                return true;
            default:
                return false;
        }
    }

    bool is_rct_borromean(int type)
    {
        switch (type)
        {
            case RCTTypeSimple:
            case RCTTypeFull:
                return true;
            default:
                return false;
        }
    }

    bool is_rct_clsag(int type)
    {
        switch (type)
        {
            case RCTTypeCLSAG:
            case RCTTypeBulletproofPlus:
                return true;
            default:
                return false;
        }
    }

    static size_t n_bulletproof_amounts_base(const size_t L_size, const size_t R_size, const size_t V_size, const size_t max_outputs)
    {
        CHECK_AND_ASSERT_MES(L_size >= 6, 0, "Invalid bulletproof L size");
        CHECK_AND_ASSERT_MES(L_size == R_size, 0, "Mismatched bulletproof L/R size");
        static const size_t extra_bits = 4;
        CHECK_AND_ASSERT_MES((1 << extra_bits) == max_outputs, 0, "log2(max_outputs) is out of date");
        CHECK_AND_ASSERT_MES(L_size <= 6 + extra_bits, 0, "Invalid bulletproof L size");
        CHECK_AND_ASSERT_MES(V_size <= (1u<<(L_size-6)), 0, "Invalid bulletproof V/L");
        CHECK_AND_ASSERT_MES(V_size * 2 > (1u<<(L_size-6)), 0, "Invalid bulletproof V/L");
        CHECK_AND_ASSERT_MES(V_size > 0, 0, "Empty bulletproof");
        return V_size;
    }

    size_t n_bulletproof_amounts(const Bulletproof &proof) { return n_bulletproof_amounts_base(proof.L.size(), proof.R.size(), proof.V.size(), BULLETPROOF_MAX_OUTPUTS); }
    size_t n_bulletproof_plus_amounts(const BulletproofPlus &proof) { return n_bulletproof_amounts_base(proof.L.size(), proof.R.size(), proof.V.size(), BULLETPROOF_PLUS_MAX_OUTPUTS); }

    size_t n_bulletproof_amounts(const std::vector<Bulletproof> &proofs)
    {
        size_t n = 0;
        for (const Bulletproof &proof: proofs)
        {
            size_t n2 = n_bulletproof_amounts(proof);
            CHECK_AND_ASSERT_MES(n2 < std::numeric_limits<uint32_t>::max() - n, 0, "Invalid number of bulletproofs");
            if (n2 == 0)
                return 0;
            n += n2;
        }
        return n;
    }

    size_t n_bulletproof_plus_amounts(const std::vector<BulletproofPlus> &proofs)
    {
        size_t n = 0;
        for (const BulletproofPlus &proof: proofs)
        {
            size_t n2 = n_bulletproof_plus_amounts(proof);
            CHECK_AND_ASSERT_MES(n2 < std::numeric_limits<uint32_t>::max() - n, 0, "Invalid number of bulletproofs");
            if (n2 == 0)
                return 0;
            n += n2;
        }
        return n;
    }

    static size_t n_bulletproof_max_amounts_base(size_t L_size, size_t R_size, size_t max_outputs)
    {
        CHECK_AND_ASSERT_MES(L_size >= 6, 0, "Invalid bulletproof L size");
        CHECK_AND_ASSERT_MES(L_size == R_size, 0, "Mismatched bulletproof L/R size");
        static const size_t extra_bits = 4;
        CHECK_AND_ASSERT_MES((1 << extra_bits) == max_outputs, 0, "log2(max_outputs) is out of date");
        CHECK_AND_ASSERT_MES(L_size <= 6 + extra_bits, 0, "Invalid bulletproof L size");
        return 1 << (L_size - 6);
    }
    size_t n_bulletproof_max_amounts(const Bulletproof &proof) { return n_bulletproof_max_amounts_base(proof.L.size(), proof.R.size(), BULLETPROOF_MAX_OUTPUTS); }
    size_t n_bulletproof_plus_max_amounts(const BulletproofPlus &proof) { return n_bulletproof_max_amounts_base(proof.L.size(), proof.R.size(), BULLETPROOF_PLUS_MAX_OUTPUTS); }

    size_t n_bulletproof_max_amounts(const std::vector<Bulletproof> &proofs)
    {
        size_t n = 0;
        for (const Bulletproof &proof: proofs)
        {
            size_t n2 = n_bulletproof_max_amounts(proof);
            CHECK_AND_ASSERT_MES(n2 < std::numeric_limits<uint32_t>::max() - n, 0, "Invalid number of bulletproofs");
            if (n2 == 0)
                return 0;
            n += n2;
        }
        return n;
    }

    size_t n_bulletproof_plus_max_amounts(const std::vector<BulletproofPlus> &proofs)
    {
        size_t n = 0;
        for (const BulletproofPlus &proof: proofs)
        {
            size_t n2 = n_bulletproof_plus_max_amounts(proof);
            CHECK_AND_ASSERT_MES(n2 < std::numeric_limits<uint32_t>::max() - n, 0, "Invalid number of bulletproofs");
            if (n2 == 0)
                return 0;
            n += n2;
        }
        return n;
    }

}
