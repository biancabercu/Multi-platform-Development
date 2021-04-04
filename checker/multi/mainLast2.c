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
	//DIE(argc==1, "Sunt necesare argumente");
	if (argc==1) 
		return -1;
	//primul argv[0] e chiar ./so-cpp
	if(argc>1){
		hashmap *hm = createHashMap();
		
		char programel[100][256];
		int argCounter=1,isSpace=0, atleastaFile=0, writetoaFile=0;
		char *argLine;
		int index=0, idxDir=0; //index pt programe;
		char s[256],m[256];
		char helper[256];
		char directories[100][256];
		char finalResult[100][256];
		char outFile[256];
		int hasError=0;
	
		for(int i=0;i<argc;i++)
			printf("%s ",argv[i]);
		printf("\n");
	


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
/*
				argLine = argv[argCounter];
				int pos=-1;
				for(int i = 0; i<strlen(argLine); i++) {
					if (argLine[i] == '=') {
						pos = i; //pos incepe de la 0
						break;
					}
				}

				if(isSpace==0)
					memcpy(s, argLine + 2, pos - 2);
				else
					memcpy(s, argLine, pos);
				memcpy(m, argLine + pos + 1, strlen(argLine));
				put(hm, s, m);
*/				
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

			while(strncmp(argv[argCounter], "-o", 2) != 0) {
				//poate sa fac o verificarE??? 
				argLine=argv[argCounter];
			
				
//				printf("Input stdin: %s \n",argLine);				
				fd_read = open(argLine, O_RDONLY);
				if (fd_read > 0) { //s-a deschis file
					atleastaFile=1;
				
					while((bytesRead= xread(fd_read, buffer, MAXLINECHARS)) != 0) {
						if (bytesRead < 0)
							break;
						buffer[bytesRead] = '\0';
						memcpy(programel[index], buffer, MAXLINECHARS);		
						index++;
					}
				}else
					hasError=1;

				if(argCounter < argc - 1)
					argCounter++;
				else
					break;
			}
			//DIE(atleastaFile ==0, "No input file");
			if (atleastaFile==0)
				hasError=1;
		}
		

		if (strncmp(argv[argCounter], "-o", 2) == 0) {
			writetoaFile =1;
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
			if(fd_write < 0 ) {
//				printf("Failed output " );
				hasError=1; 
			}
			//scriem in fisier
			int rc;
/*			//int rc = xwrite(STDOUT_FILENO, argLine, strlen(argLine));
			for(int i=0;i < 100; i++){ 
				printf("strlen =  %d\n",strlen(directories[i]));
				rc = xwrite(STDOUT_FILENO, directories[i], strlen(directories[i]));
			}
*/		}

//		if(writetoaFile) {
			//scriem in acel fisier out
//		}else{
			//scriem la stdout
//		}

		destroy(hm);

		if(hasError)
			return -1;
//		free(argLine);	
	}
	return 0;
}
