                                                           
                     
  
                     
                                     
                          
                                    
                                                             
                                              
     
#ifndef MINIXML_H_INCLUDED
#define MINIXML_H_INCLUDED
#define IS_WHITE_SPACE(c) ((c)==' ' || (c)=='\t' || (c)=='\r' || (c)=='\n')

 
 
struct xmlparser {
	const char *xmlstart;
	const char *xmlend;
	const char *xml;	 
	int xmlsize;
	void * data;
	void (*starteltfunc) (void *, const char *, int);
	void (*endeltfunc) (void *, const char *, int);
	void (*datafunc) (void *, const char *, int);
	void (*attfunc) (void *, const char *, int, const char *, int);
};

 
 
 
 
 
void parsexml(struct xmlparser *);

#endif

