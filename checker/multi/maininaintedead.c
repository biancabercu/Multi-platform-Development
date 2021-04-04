#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
//#include "utils.h"
#include "HashMap.h"
#include "xreadwrite.h"

#define MAXLINECHARS 256


int main(int argc, char *argv[])
{
	int hasError=0, writetoaFile=0;
	//DIE(argc==1,"Sunt necesare argumente");
	if (argc==1) 
		return -1;
	//primul argv[0] e chiar ./so-cpp
	if(argc>1){
		hashmap *hm = createHashMap();
		
		int fd_write;
		int argCounter=1,isSpace=0, atleastaFile=0;
		char *argLine;
		int index=0, idxDir=0; //index pt programe;
		char s[256],m[256];
		char helper[256];
		char directories[100][256];
		char outFile[256];
		char program[256];
		int hasOutputFile=0,hasInputFile=0;
		int afterDef=0,pastDef=0;
		
/*		printf("DECI ");		
		for(int i=0;i<argc;i++)
			printf("%s ",argv[i]);
		printf("\n");
*/	


		//		ARGV DELIMITATI PRIN SPATII
		if (strncmp(argv[argCounter], "-D", 2) == 0) {
			while (strncmp(argv[argCounter], "-D", 2) == 0) {
				//avem de facut mapping
			
				memcpy(helper,argv[argCounter],256);
				
				memset(s, 0, 256);
				memset(m, 0, 256);

				isSpace = 0;
				if(strlen(helper) == 2) 
					isSpace = 1;
				
				argCounter+=isSpace;//inseamna ca e spatiu intre -D si

				argLine = argv[argCounter];
				int pos=-1;
				for(int i = 0; i<strlen(argLine); i++) {
					if (argLine[i] == '=') {
						pos = i; //pos incepe de la 0
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
				//len(argc) e mereu +1 pt ca so-cpp
				if (argCounter <argc-1)
					argCounter++;
				else
					break;
			}
		}
		//cum facem cand deja am citit un elem??
			
		if (strncmp(argv[argCounter], "-I", 2) == 0) {
			int fd_read, fd_write, rc, bytesRead;
			char buffer[MAXLINECHARS];
			int openedFile=0;
			//verifica din #include salvate 
			//in dir dat , acele fisiere incluse
			//?? se fac operatii si acolo 

			while(strncmp(argv[argCounter], "-I", 2) == 0) {
				//avem de cautat in dir respectiv fisiere sursa
				isSpace=0;
				memcpy(helper,argv[argCounter],256);
				if (strlen(helper) == 2) 
					isSpace=1;//este spatiu intre
				
			
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
			//avem drept argument fisiere sursa
			int fd_read=1, rc, bytesRead;
			char buffer[MAXLINECHARS];
			int openedFile=0;
			char *lineHelper;
			while(strncmp(argv[argCounter], "-o", 2) != 0) {
				//poate sa fac o verificarE??? 
				argLine=argv[argCounter];
			//	if(fd_read<0) {
					//scriem in program toare arugmentele					
			//	}
				if(hasOutputFile && hasInputFile) {
					hasError=1;
					break;
				}
				if(hasInputFile && !hasOutputFile)
					hasOutputFile=1;
				if(!hasInputFile) 
					hasInputFile=1;

//				printf("Input stdin: %s \n",argLine);				
				if(hasOutputFile && hasInputFile){
					fd_write=open(argLine,O_RDWR);
					if(fd_write<0){
						//hasError=1;
						writetoaFile=0;
					}
					else
						writetoaFile=1;
				}

				if(!hasOutputFile && hasInputFile) {
					fd_read = open(argLine, O_RDONLY);
					if (fd_read > 0) { //s-a deschis file
				
						while((bytesRead= xread(fd_read, buffer, MAXLINECHARS)) != 0) {
							if (bytesRead < 0)
								break;
							if (hasOutputFile)
								writetoaFile=1;
							buffer[bytesRead] = '\0';
							memcpy(program, buffer, MAXLINECHARS);	
							//dar verificam daca exista define?
							lineHelper=buffer;

							index++;
						}
					}else if(fd_read<0) {
						
						if(strncmp(argv[argCounter],"-",1)!=0){
							int lengths=0;
							//hasError=1; presupunem ca citim stdin?
							while(argCounter<argc){
								//citim toate argumentele at this point
								memcpy(program+lengths,
										argv[argCounter],
										strlen(argv[argCounter]));
								lengths+=strlen(argv[argCounter]);
//								printf("leng= %d\n",lengths);
								if(argCounter+1>argc) {
									memcpy(program+lengths," ",1);
									lengths++; 
								}
								argCounter++;
							}
						}else 
							hasError=1;
					}
//					printf("strklen=%d\n",strlen(program));
				}

					if(argCounter < argc - 1)
						argCounter++;
					else
						break;
			}
		}
		

		if (argCounter<argc-1 && 
				strncmp(argv[argCounter], "-o", 2) == 0) {
		
//			hashmapNode *yes=get(hm,"VAR0"); if(yes!=NULL) printf("Exista in o\n");
			if(hasOutputFile)
				hasError=1;
			else if(!hasOutputFile){

				memcpy(helper, argv[argCounter], 256);
				isSpace=0;

				if(strlen(helper)==2)
					isSpace=1;
				argCounter+=isSpace;

				if(argCounter < argc - 1) //argc e nr elem, argCounter mereu e -1
					hasError=1 ;//daca mai exista alte fisiere out
			
				argLine=argv[argCounter];
/*	
				if(isSpace==1)
					printf("Output to %s \n",argLine);
				else
					printf("Output to %s \n",argLine+2);
*/				if(isSpace==1)
					fd_write = open(argLine, O_WRONLY | O_CREAT);
				else
					fd_write = open(argLine+2, O_WRONLY | O_CREAT);
				if(fd_write < 0 ) 
					hasError=1;
				else 
					writetoaFile=1;
				//scriem in fisier
				int rc;
				//int rc = xwrite(STDOUT_FILENO, argLine, strlen(argLine));
/*				for(int i=0;i < 100; i++){ 
					//printf("strlen =  %ld\n",strlen(directories[i]));
					if(strlen(directories[i])==0)
						break;
					else
						rc = xwrite(STDOUT_FILENO, directories[i], strlen(directories[i]));
*/			
			}
		}
			
		

		// PARTEA IN CARE PUNEM PIESELE LA LOC
		//am directories? , am programel, am datele in hashmap	
		// if idxDir, index, isEmpty*
		int rc;
		char space[26]={'\t','[',']','{','}',' ','<','>','=','+'
				,'-','*','/','%','!','&','|','^','.',',',':',';','('
					,')','\n'};
			//[5]={' ',')',','};
		char *pch,*token,*nexttoken,*tokenpastDef,*tokenpastUndef;
		int nrDefs=0,nrUndefs=0,pastUndef=0,beforeUndef=1;
		int replacePos;
		char prgLine[256];
		hashmapNode *match;
		int lenMapp=0,diff;
		int once=0;	

//			hashmapNode *yes=get(hm,"VAR0"); if(yes!=NULL) printf("Exista la final\n");
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
					//wish- se cauta doar celelate tokens, fara def
			//		printf("X");//deci automat 2 la numar, doar ca in program
					match=get(hm,token);//se gasesc primele 2, incluzand define
					lenMapp=strlen(match->mapping);
					
					pch=strstr(program,token);
					replacePos=pch-program;
					if(nrDefs>0){ //sa nu inlocuiesc var din define 
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
			//		printf("exista undef");
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
		//delete the #def lines
		//stergem tot pana la lastToken
		if(nrDefs>0) {
			pch=strstr(program,tokenpastDef);
			replacePos=pch-program;
			memmove(program,program+replacePos,strlen(program));
			nrDefs=0;
		}
		if(nrUndefs>0) {
			char *pch2,*pch1;
			int posbef,posafter;

		//	printf("BEFOREe %s\n",nexttoken);	
		//	printf("PAST %s\n",tokenpastUndef);

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


//		printf("strlenprg= %ld\n",strlen(program));
		if(!writetoaFile)
			rc=xwrite(STDOUT_FILENO,program,strlen(program));		
		else 
			rc=xwrite(fd_write,program,strlen(program));		
/*			if(strlen(programel[i])!=0) {
				token=strtok(programel[i],space);
				while(token!=NULL) {
					printf("%s \n",token);
					token=strtok(NULL,space);
				}
			}

			pch = strstr(programel[i],"VAR0");
			while(pch!=NULL) {	
				strncpy(pch, "1   ",4);
				pch = strstr(programel[i],"VAR0");
			}
				rc=xwrite(STDOUT_FILENO,programel[i],strlen(programel[i]));		
*/		
/*		if(writetoaFile) {
			//scriem in acel fisier out

		}else{
			//scriem la stdout
		}
*/

		destroy(hm);

		if(hasError)
			return -1;
//		free(argLine);	
	}
	return 0;
}
