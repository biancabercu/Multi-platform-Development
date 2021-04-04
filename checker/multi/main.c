#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include "HashMap.h"
#include "xreadwrite.h"

#define MAXLINECHARS 256


int main(int argc, char *argv[])
{
	int hasError=0, writetoaFile=0;
	if (argc==1) 
		return -1;
	if(argc>1){
		hashmap *hm = createHashMap();
		
		int fd_write,i;
		int argCounter=1,isSpace=0, atleastaFile=0;
		char *argLine;
		int index=0, idxDir=0; 
		char s[256],m[256];
		char helper[256];
		char directories[100][256];
		char outFile[256];
		char program[256];
		int hasOutputFile=0,hasInputFile=0;
		int afterDef=0,pastDef=0;

		if (strncmp(argv[argCounter], "-D", 2) == 0) {
			while (strncmp(argv[argCounter], "-D", 2) == 0) {
			
				memcpy(helper,argv[argCounter],256);
				
				memset(s, 0, 256);
				memset(m, 0, 256);

				isSpace = 0;
				if(strlen(helper) == 2) 
					isSpace = 1;
				
				argCounter+=isSpace;
				argLine = argv[argCounter];
				int pos=-1;
				for(i = 0; i<strlen(argLine); i++) {
					if (argLine[i] == '=') {
						pos = i; 
						break;
					}
				}


				int lenSym, lenMapp;
				char *substrMapp, *substrSym;
				if(isSpace==0) {
					substrSym=argLine+2;
					if(pos == -1) {
						lenSym=strlen(argLine);
						substrMapp=" ";
					}else{
						lenSym=pos-2;
						substrMapp = argLine+pos+1;
					}
				}else{
					substrSym=argLine;
					if(pos == -1) {
						lenSym=strlen(argLine);
						substrMapp=" ";
					}else {
						lenSym=pos;
						substrMapp=argLine+pos+1;
					}
				}
				memcpy(s, substrSym, lenSym);
				memcpy(m, substrMapp, strlen(argLine));

				put(hm, s, m);
				if (argCounter <argc-1)
					argCounter++;
				else
					break;
			}
		}
			
		if (strncmp(argv[argCounter], "-I", 2) == 0) {
			int fd_read, fd_write, rc, bytesRead;
			char buffer[MAXLINECHARS];
			int openedFile=0;

			while(strncmp(argv[argCounter], "-I", 2) == 0) {
				isSpace=0;
				memcpy(helper,argv[argCounter],256);
				if (strlen(helper) == 2) 
					isSpace=1;				
			
				argCounter+=isSpace;
				argLine=argv[argCounter];
				if(isSpace) 
					memcpy(directories[idxDir], argLine, strlen(argLine));	
				else 
					memcpy(directories[idxDir], argLine +2, strlen(argLine));
				idxDir++;
				if(argCounter < argc-1)	
					argCounter++;
				else
					break;
			}
		}
		
		if (strncmp(argv[argCounter], "-o", 2) != 0
				&& strncmp(argv[argCounter-1],"-I",2)!=0) {
			int fd_read=1, rc, bytesRead;
			char buffer[MAXLINECHARS];
			int openedFile=0;
			char *lineHelper;
			while(strncmp(argv[argCounter], "-o", 2) != 0) {
				argLine=argv[argCounter];
				if(hasOutputFile && hasInputFile) {
					hasError=1;
					break;
				}
				if(hasInputFile && !hasOutputFile)
					hasOutputFile=1;
				if(!hasInputFile) 
					hasInputFile=1;

				if(hasOutputFile && hasInputFile){
					fd_write=open(argLine,O_RDWR);
					if(fd_write<0){
						writetoaFile=0;
					}
					else
						writetoaFile=1;
				}

				if(!hasOutputFile && hasInputFile) {
					fd_read = open(argLine, O_RDONLY);
					if (fd_read > 0) { 
				
						while((bytesRead= xread(fd_read, buffer, MAXLINECHARS)) != 0) {
							if (bytesRead < 0)
								break;
							if (hasOutputFile)
								writetoaFile=1;
							buffer[bytesRead] = '\0';
							memcpy(program, buffer, MAXLINECHARS);	
							lineHelper=buffer;

							index++;
						}
					}else if(fd_read<0) {
						
						if(strncmp(argv[argCounter],"-",1)!=0){
							int lengths=0;
							while(argCounter<argc){
								memcpy(program+lengths,
										argv[argCounter],
										strlen(argv[argCounter]));
								lengths+=strlen(argv[argCounter]);
								if(argCounter+1>argc) {
									memcpy(program+lengths," ",1);
									lengths++; 
								}
								argCounter++;
							}
						}else 
							hasError=1;
					}
				}

					if(argCounter < argc - 1)
						argCounter++;
					else
						break;
			}
		}
		

		if (argCounter<argc-1 && 
				strncmp(argv[argCounter], "-o", 2) == 0) {
		
			if(hasOutputFile)
				hasError=1;
			else if(!hasOutputFile){

				memcpy(helper, argv[argCounter], 256);
				isSpace=0;

				if(strlen(helper)==2)
					isSpace=1;
				argCounter+=isSpace;

				if(argCounter < argc - 1) 
					hasError=1 ;
			
				argLine=argv[argCounter];
				if(isSpace==1)
					fd_write = open(argLine, O_WRONLY | O_CREAT);
				else
					fd_write = open(argLine+2, O_WRONLY | O_CREAT);
				if(fd_write < 0 ) 
					hasError=1;
				else 
					writetoaFile=1;
			}
		}
			
		

		int rc;
		char space[26]={'\t','[',']','{','}',' ','<','>','=','+'
				,'-','*','/','%','!','&','|','^','.',',',':',';','('
					,')','\n'};
		char *pch,*token,*nexttoken,*tokenpastDef,*tokenpastUndef;
		int nrDefs=0,nrUndefs=0,pastUndef=0,beforeUndef=1;
		int replacePos;
		char prgLine[256];
		hashmapNode *match;
		int lenMapp=0,diff;
		int once=0;	
		
		memcpy(prgLine,program,256);
		if(strlen(prgLine)!=0) {
			token=strtok(prgLine,space);
			while(token!=NULL) {
				if(strcmp(token,"#define")==0) {
					nrDefs++;
					memset(s,0,256);
					memset(m,0,256);
					token=strtok(NULL,space);
					memcpy(s,token,strlen(token));
					token=strtok(NULL,space);
					memcpy(m,token,strlen(token));
					put(hm,s,m);
					pastDef=1;
					afterDef=1;
				}else if(contains(hm,token)){ 
					match=get(hm,token);
					lenMapp=strlen(match->mapping);
					
					pch=strstr(program,token);
					replacePos=pch-program;
					if(nrDefs>0){  
						pch=strstr(program+replacePos+strlen(token)
								,token);
						replacePos=pch-program;
					}
					strncpy(program+replacePos,match->mapping,lenMapp);

					diff=strlen(token)-lenMapp;
					if(diff>0) {
						memmove(program+replacePos+lenMapp,
							program+replacePos+lenMapp+diff,
							strlen(program+replacePos+lenMapp));
					}
				}else if(strcmp(token,"#undef")==0){
					nrUndefs++;
					nexttoken=token;
					token=strtok(NULL,space);
					memset(s,0,256);
					memcpy(s,token,strlen(token));
					deleteOnce(hm,s);
					pastUndef=1;
				}
				token=strtok(NULL,space);
				if(pastDef){
					tokenpastDef=token;
					pastDef=0;
				}
				if(pastUndef){
					tokenpastUndef=token;
					pastUndef=0;
				}
			}
			
		}
		if(nrDefs>0) {
			pch=strstr(program,tokenpastDef);
			replacePos=pch-program;
			memmove(program,program+replacePos,strlen(program));
			nrDefs=0;
		}
		if(nrUndefs>0) {
			char *pch2,*pch1;
			int posbef,posafter;


			pch1=strstr(program,nexttoken);
			pch2=strstr(program,tokenpastUndef);
			
			posbef=pch1-program;
			posafter=pch2-program;
			while(posafter < posbef) {
				pch2=strstr(program+posbef,tokenpastUndef);
				posafter=pch2-program;
			}
			memmove(program+posbef,program+posafter,strlen(program));
			nrUndefs=0;
		
		}


		if(!writetoaFile)
			rc=xwrite(STDOUT_FILENO,program,strlen(program));		
		else 
			rc=xwrite(fd_write,program,strlen(program));		

		destroy(hm);

		if(hasError)
			return -1;
	}
	return 0;
}
