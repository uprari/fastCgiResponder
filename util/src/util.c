#include<stdio.h>
#include<util.h>
#include<stdbool.h>
#include<errno.h>
#include<string.h>

int utilAddTwoNumbers(int a, int b){

	return a+b;

}

int noOfChars(const char *word){

	if(word == NULL) {

		return 0;
	}
	
	int count = 0;
	while( *word != '\0' && *word != ' '){
		count++;
		word++;
	}

	return count;
}

int escapeSpace(char *buffer, int index){

	while(buffer[index] == ' ' || buffer[index] == '\t')
		index++;
        return index;

}

bool extractKeyVal(char *buf, int *k, int *v , int index){
	
	if(buf[index] == '\0')
		return false;
	*k = index ;
	
	while(buf[index] != '\0'){
		if( buf[index ++] != ' ')
			continue;
        	buf[index - 1] = '\0';
		break;
	}
	escapeSpace(buf, index);
	if(buf[index] != '\0')
		*v = index;
       
	if(*v != 0 ){
		return true;
	}
	return false;
}

void captureConfig( char *filePath, void (*configAdd)(char*,int,int)){

	if(filePath == NULL || configAdd == NULL ){
		printf("NUll Exception CaptureConfig");
	}
	
	// reading config file
	FILE * filePtr = NULL;
	char buffer[1000];
	
	if((filePtr = fopen("../config/dbConfig.txt", "r")) == NULL ) {
		printf("ERROR! Opening file\n");
		printf("Configuration reading failed, with error no %d: %s \n", errno, strerror(errno));
                return;
	}
	
	
	int key,val,index;

        while( (fgets(buffer,MAX_LINE_SIZE,filePtr)) != NULL ){

              key = val = index = 0;
              if(buffer[index] == '#')
                        continue;
              index = escapeSpace(buffer,index);
              if ( extractKeyVal(buffer,&key,&val,index) == false )
                      continue;

              configAdd(buffer,key,val);
        }
}
