                                       
                                              

                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
#pragma once

#include <atomic>
#include <cstddef>
#include <lmdb.h>
#include <memory>
#include <type_traits>

#include "common/expect.h"
#include "lmdb/error.h"
#include "lmdb/transaction.h"

namespace lmdb
{
     
    struct close_env
    {
        void operator()(MDB_env* ptr) const noexcept
        {
            if (ptr)
                mdb_env_close(ptr);
        }
    };

    using environment = std::unique_ptr<MDB_env, close_env>;

     
    expect<environment> open_environment(const char* path, MDB_dbi max_dbs) noexcept;

     
    struct context
    {
        std::atomic<std::size_t> active;
        std::atomic_flag lock;
    };

     
    class database
    {
        environment env;
        context ctx;

         
        MDB_env* handle() const noexcept { return env.get(); }

        expect<write_txn> do_create_txn(unsigned int flags) noexcept;

    public: 
        database(environment env);

        database(database&&) = delete;
        database(database const&) = delete;

        virtual ~database() noexcept;

        database& operator=(database&&) = delete;
        database& operator=(database const&) = delete;

         
 
 
 
        expect<void> resize() noexcept;

         
        expect<read_txn> create_read_txn(suspended_txn txn = nullptr) noexcept;

         
        expect<suspended_txn> reset_txn(read_txn txn) noexcept;

         
        expect<write_txn> create_write_txn() noexcept;

         
        expect<void> commit(write_txn txn) noexcept;

         
 
 

 
 

 
 
        template<typename F>
        typename std::result_of<F(MDB_txn&)>::type try_write(F f, unsigned attempts = 3)
        {
            for (unsigned i = 0; i < attempts; ++i)
            {
                expect<write_txn> txn = create_write_txn();
                if (!txn)
                    return txn.error();

                MONERO_PRECOND(*txn != nullptr);
                const auto wrote = f(*(*txn));
                if (wrote)
                {
                    MONERO_CHECK(commit(std::move(*txn)));
                    return wrote;
                }
                if (wrote != lmdb::error(MDB_MAP_FULL))
                    return wrote;

                txn->reset();
                MONERO_CHECK(this->resize());
            }
            return {lmdb::error(MDB_MAP_FULL)};
        }
    };
}  

