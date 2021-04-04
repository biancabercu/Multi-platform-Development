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

/*
void readthefile(char *line, char *arg) 
{
	int fd_read, fd_write, rc, bytesRead;
	char buffer[MAXLINECHARS];
	int openedFile=0, isSpace=0;
	char helper[256];
	char *argLine;
	int atleastaFile;
	char programel[100][256];
	int index=0; 
	int argc =2;//???de transmis
	int argCounter=1;

	while(strncmp(line, arg, 2) == 0) {
	
		memcpy(helper,line,256);
	
		isSpace=0;
		if (strlen(helper) == 2) 
			isSpace=1;//este spatiu intre
	
		argCounter+=isSpace;
		argLine=line;
		if(isSpace)
			printf("Input %s\n",argLine);
		else
			printf("Input %s\n",argLine+2);

		if(isSpace==0) 	
			fd_read = open(argLine + 2, O_RDONLY);
		else
			fd_read = open(argLine, O_RDONLY);
		if (fd_read > 0) { //s-a deschis file
			printf("e fisier bun\n");
			atleastaFile=1;
			while((bytesRead= xread(fd_read, buffer, MAXLINECHARS)) != 0) {
				if (bytesRead < 0)
					break;
				buffer[bytesRead] = '\0';
				memcpy(programel[index], buffer, MAXLINECHARS);
				index++;
			}
		}	
		if(argCounter < argc -1 )
			argCounter++;
		else
			break;
	}
			
} */

int main(int argc, char *argv[])
{
	int hasError=0, writetoaFile=0;
	//DIE(argc==1, "Sunt necesare argumente");
	if (argc==1) 
		return -1;
	//primul argv[0] e chiar ./so-cpp
	if(argc>1){
		hashmap *hm = createHashMap();
		
		char programel[100][256];
		int argCounter=1,isSpace=0, atleastaFile=0;
		char *argLine;
		int index=0, idxDir=0; //index pt programe;
		char s[256],m[256];
		char helper[256];
		char directories[100][256];
		char finalResult[100][256];
		char outFile[256];
/*	
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
//				printf("s= %s , m= %s \n",s,m);				
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
/*				
				if(isSpace)
					printf("Input %s \n",argLine);
				else 
					printf("Input %s \n", argLine+2);
*/				if(isSpace) 
					memcpy(directories[idxDir], argLine, strlen(argLine));	
				else 
					memcpy(directories[idxDir], argLine +2, strlen(argLine));
				idxDir++;
/*				if(isSpace==0) 	
					fd_read = open(argLine + 2, O_RDONLY);
				else
					fd_read = open(argLine, O_RDONLY);
				
				if(fd_read > 0 ) printf("Este un fisier bun\n");				
				if (fd_read > 0) { //s-a deschis file
					 //unde salvez?? fac o lista de liste
					atleastaFile=1;
					while((bytesRead= xread(fd_read, buffer, MAXLINECHARS)) != 0) {
						if (bytesRead < 0)
							break;
						buffer[bytesRead] = '\0';
						//punem in lista de liste:
						memcpy(programel[index], buffer, MAXLINECHARS);		
						index++;
					}
				}
*/				if(argCounter < argc-1)	
					argCounter++;
				else
					break;
			}
		}
		
		if (strncmp(argv[argCounter], "-o", 2) != 0
				&& strncmp(argv[argCounter-1],"-I",2)!=0) {
			//avem drept argument fisiere sursa
			int fd_read, fd_write, rc, bytesRead;
			char buffer[MAXLINECHARS];
			int openedFile=0;
			char *lineHelper;
			while(strncmp(argv[argCounter], "-o", 2) != 0) {
				//poate sa fac o verificarE??? 
				argLine=argv[argCounter];
			
				
//				printf("Input stdin: %s \n",argLine);				
				fd_read = open(argLine, O_RDONLY);
				if (fd_read > 0) { //s-a deschis file
				
					while((bytesRead= xread(fd_read, buffer, MAXLINECHARS)) != 0) {
						if (bytesRead < 0)
							break;
						buffer[bytesRead] = '\0';
						memcpy(programel[index], buffer, MAXLINECHARS);	
						//dar verificam daca exista define?
						lineHelper=buffer;

						if(strncmp(programel[index],"#define",7)==0) {

							//de adaugat D
							lineHelper=buffer+7 +1;

							int space1=-1,space2=-1;
							for (int i = 0; i < strlen(lineHelper) ; i++) {
								if(space1==-1 && lineHelper[i]==' ')
									space1=i;
								else if(space2==-1 &&
									(lineHelper[i]==' ' || i==strlen(lineHelper)-1)){
									space2=i;
									break;
								}
							}
							memset(s,0,256);
							memset(m,0,256);
							memcpy(s,lineHelper,space1);
							memcpy(m,lineHelper+space1,space2-space1);
							put(hm,s,m);					

						}
						if(strncmp(programel[index],"#undef",6)==0){
							//de sters D
							lineHelper=buffer+6+1;
							memset(s,0,256);
							memcpy(s,lineHelper,strlen(lineHelper));
							deleteOnce(hm,s);

						}
						if(strncmp(programel[index],"#include",8)==0) {
							//de adaugat I
							

						}
						index++;
					}
				}else
					hasError=1;

				if(argCounter < argc - 1)
					argCounter++;
				else
					break;
			}
		}
		

		if (strncmp(argv[argCounter], "-o", 2) == 0) {
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
*/			int fd_write;
			if(isSpace==1)
				fd_write = open(argLine, O_RDWR);
			else
				fd_write = open(argLine+2, O_RDWR);
			if(fd_write < 0 ) 
				hasError=1;
			else 
				writetoaFile=1;
			//scriem in fisier
			int rc;
			//int rc = xwrite(STDOUT_FILENO, argLine, strlen(argLine));
/*			for(int i=0;i < 100; i++){ 
				//printf("strlen =  %ld\n",strlen(directories[i]));
				if(strlen(directories[i])==0)
					break;
				else
					rc = xwrite(STDOUT_FILENO, directories[i], strlen(directories[i]));
*/			
		}
		

		// PARTEA IN CARE PUNEM PIESELE LA LOC
		//am directories? , am programel, am datele in hashmap	
		// if idxDir, index, isEmpty
/*
		if(writetoaFile) {
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
