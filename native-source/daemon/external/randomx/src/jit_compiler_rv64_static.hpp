  
                                              

                    

                                                                  
                                                                           
                                                                 
                                                                
                                                                    
                                                                      
                                                                       
                                                   
                                                                       
                                                                        

                                                                               
                                                                             
                                                                      
                                                                            
                                                                          
                                                                          
                                                                          
                                                                             
                                                                             
                                                                    
  

#pragma once

extern "C" {
	void randomx_riscv64_literals();
	void randomx_riscv64_literals_end();
	void randomx_riscv64_data_init();
	void randomx_riscv64_fix_data_call();
	void randomx_riscv64_prologue();
	void randomx_riscv64_loop_begin();
	void randomx_riscv64_data_read();
	void randomx_riscv64_data_read_light();
	void randomx_riscv64_fix_loop_call();
	void randomx_riscv64_spad_store();
	void randomx_riscv64_spad_store_hardaes();
	void randomx_riscv64_spad_store_softaes();
	void randomx_riscv64_loop_end();
	void randomx_riscv64_fix_continue_loop();
	void randomx_riscv64_epilogue();
	void randomx_riscv64_softaes();
	void randomx_riscv64_program_end();
	void randomx_riscv64_ssh_init();
	void randomx_riscv64_ssh_load();
	void randomx_riscv64_ssh_prefetch();
	void randomx_riscv64_ssh_end();
}
