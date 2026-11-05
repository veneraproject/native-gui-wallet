  
                                                    

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

extern "C" {
	void randomx_prefetch_scratchpad();
	void randomx_prefetch_scratchpad_end();
	void randomx_program_prologue();
	void randomx_program_loop_begin();
	void randomx_program_loop_load();
	void randomx_program_start();
	void randomx_program_read_dataset();
	void randomx_program_read_dataset_sshash_init();
	void randomx_program_read_dataset_sshash_fin();
	void randomx_program_loop_store();
	void randomx_program_loop_end();
	void randomx_dataset_init();
	void randomx_program_epilogue();
	void randomx_sshash_load();
	void randomx_sshash_prefetch();
	void randomx_sshash_end();
	void randomx_sshash_init();
	void randomx_program_end();
}
