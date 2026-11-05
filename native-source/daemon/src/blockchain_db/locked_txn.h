                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               
  
                                                                                      

#pragma once

namespace cryptonote
{
     
     
     
     
    class LockedTXN {
    public:
      LockedTXN(BlockchainDB &db): m_db(db), m_batch(false), m_active(false) {
        m_batch = m_db.batch_start();
        m_active = true;
      }
      void commit() { try { if (m_batch && m_active) { m_db.batch_stop(); m_active = false; } } catch (const std::exception &e) { MWARNING("LockedTXN::commit filtering exception: " << e.what()); } }
      void abort() { try { if (m_batch && m_active) { m_db.batch_abort(); m_active = false; } } catch (const std::exception &e) { MWARNING("LockedTXN::abort filtering exception: " << e.what()); } }
      ~LockedTXN() { abort(); }
    private:
      BlockchainDB &m_db;
      bool m_batch;
      bool m_active;
    };
}
