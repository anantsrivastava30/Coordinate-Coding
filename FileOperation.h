#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

namespace FileOperation{

template <typename S>
char  *getFolderName( S sampleSize, char *sampleType){
   	char *folderName = getCharValue( sampleSize, sampleType);
   	return folderName;
}

template <typename S>
char *getCharValue( S value, char *indexType ){
    	char *name = new char[100];
    	sprintf(name, indexType, value);
    	return name;
}

template <typename T, typename S>
void readVector( T &data, S vectorSize, char *wholeFileName, char *dataType ){

	FILE *fp;
	fp = fopen( wholeFileName, "r");
        if( fp == NULL ){
		std::cout<<"Could not read file "<<wholeFileName<<"."<<std::endl;
            	exit(0);
	}

	for( S index=0; index<vectorSize; index++ ){
        	if( fscanf(fp, dataType, &data[index])==0 )
			exit(0);
	}
	fclose(fp);
}

template <typename T, typename S>
void readVector( T &data, S vectorSize, char *wholeFileName, char *dataType, FILE *fp ){

    	if( fp == NULL ){
		std::cout<<"Could not read file "<<wholeFileName<<"."<<std::endl;
        	exit(0);
	}

	for( S index=0; index<vectorSize; index++ ){
        	if( fscanf(fp, dataType, &data[index])==0 )
			exit(0);
	}
}


template <typename T, typename S>
void writeVector( T &data, S vectorSize, char *wholeFileName, char *dataType ){

	FILE *fp;
	fp = fopen( wholeFileName, "w");
        if( fp == NULL ){
		std::cout<<"Could not read file "<<wholeFileName<<"."<<std::endl;
            	exit(0);
	}

	for( S index=0; index<vectorSize; index++ ){
        	fprintf(fp, dataType, data[index]);
		fprintf(fp, " " );
	}
	fprintf(fp, "\n" );
	fclose(fp);
}

template <typename T, typename S>
void writeVector( T &data, S vectorSize, char *wholeFileName, char *dataType, FILE *fp ){

    	if( fp == NULL ){
		std::cout<<"Could not read file "<<wholeFileName<<"."<<std::endl;
        	exit(0);
	}

	for( S index=0; index<vectorSize; index++ ){
        	fprintf(fp, dataType, data[index]);
		fprintf(fp, " " );
	}
	fprintf(fp, "\n" );
}

template <typename S>
char *getFileName( S index, char *currentDataPwd, char *folderName, char *indexType, char *fileType ){
	char *name = getCharValue(index, indexType);
    	char *wholeFileName = new char[strlen(currentDataPwd)+strlen(folderName)+strlen(name)+strlen(fileType)+1];
    	strcpy( wholeFileName, currentDataPwd );
    	strncat( wholeFileName, folderName, strlen(folderName));
    	//wholeFileName[strlen(currentDataPwd)+strlen(folderName)]='/';
    	strncat( wholeFileName, name, strlen(name));
    	strncat( wholeFileName, fileType, strlen(fileType));
    	wholeFileName[strlen(currentDataPwd)+strlen(folderName)+strlen(name)+strlen(fileType)+1]='\0';
	free(name);
	return wholeFileName;
}

template <typename T, typename S>
void readSample( T *data, S dataSize, S index, char *currentDataPwd, char *folderName, char *dataType, char *indexType, char *fileType ){

    	char *wholeFileName = getFileName(index, currentDataPwd, folderName, indexType, fileType);
    	readVector( data, dataSize, wholeFileName, dataType );
    	free(wholeFileName);
}

char *combineFileName( char *filename, char *currentDataPwd ){
    	char *wholeFileName = new char[strlen(currentDataPwd)+strlen(filename)+1];
    	strcpy( wholeFileName, currentDataPwd  );
    	strncat( wholeFileName, filename, strlen(filename));
        wholeFileName[strlen(currentDataPwd)+strlen(filename)]='\0';
    	return wholeFileName;
}

template <typename T, typename S>
void readMatrix( T **matrix, char *wholeFileName, char *dataType, S row, S column ){

    	FILE *fp;
    	fp = fopen( wholeFileName, "r");
    	if( fp == NULL ){
        	std::cout<<"Could not read file "<<wholeFileName<<"."<<std::endl;
        	exit(0);
    	}

    	for( S i=0; i<row; i++ ){
		for( S j=0; j<column; j++){
            		if( fscanf(fp, dataType, &matrix[i][j])==0 )
				exit(0);
		}
    	}
    	fclose(fp);
}

template <typename T, typename S>
void saveMatrix( T **matrix, char *wholeFileName, char *dataType, S row, S column ){

	FILE *fp;
	fp = fopen( wholeFileName, "w");
    	if( fp == NULL ){
		std::cout<<"Could not read file "<<wholeFileName<<"."<<std::endl;
        	exit(0);
	}

	for( S i=0; i<row; i++ ){
		for( S j=0; j<column; j++){
        		fprintf(fp, dataType, matrix[i][j]);
			fprintf(fp, " " );
		}
		fprintf(fp, "\n" );
	}
	fclose(fp);
}

template <typename T, typename S>
void readMatrix( T **matrix, char *wholeFileName, char *dataType, S row, S column, FILE *fp ){

    	if( fp == NULL ){
        	std::cout<<"Could not read file "<<wholeFileName<<"."<<std::endl;
        	exit(0);
    	}

    	for( S i=0; i<row; i++ ){
		for( S j=0; j<column; j++){
            		if( fscanf(fp, dataType, &matrix[i][j])==0 )
				exit(0);
		}
    	}
}

template <typename T, typename S>
void saveMatrix( T **matrix, char *wholeFileName, char *dataType, S row, S column, FILE *fp ){

    	if( fp == NULL ){
		std::cout<<"Could not read file "<<wholeFileName<<"."<<std::endl;
        	exit(0);
	}

	for( S i=0; i<row; i++ ){
		for( S j=0; j<column; j++){
        		fprintf(fp, dataType, matrix[i][j]);
			fprintf(fp, " " );
		}
		fprintf(fp, "\n" );
	}
}

template <typename T, typename D, typename S>
void readSparseMatrix( T **matrix, D *index, char *wholeFileName, char *dataType, S row ){

	FILE *fp;
	fp = fopen( wholeFileName, "r");
    	if( fp == NULL ){
        	std::cout<<"Could not read file "<<wholeFileName<<"."<<std::endl;
        	exit(0);
    	}

    	for( S i=0; i<row; i++ ){
		for( S j=0; j<index[i].size(); j++){
            		if( fscanf(fp, dataType, &matrix[i][index[i][j]])==0 )
				exit(0);
		}
    	}
	fclose(fp);
}

template <typename T, typename D, typename S>
void saveSparseMatrix( T **matrix, D *index, char *wholeFileName, char *dataType, S row ){

	FILE *fp;
	fp = fopen( wholeFileName, "w");
    	if( fp == NULL ){
		std::cout<<"Could not read file "<<wholeFileName<<"."<<std::endl;
        	exit(0);
	}

	for( S i=0; i<row; i++ ){
		for( S j=0; j<index[i].size(); j++){
        		fprintf(fp, dataType, matrix[i][index[i][j]]);
			fprintf(fp, " " );
		}
		fprintf(fp, "\n" );
	}
	fclose(fp);
}

template <typename T, typename D, typename S>
void saveSparseMatrix( T **matrix, D *index, char *wholeFileName, char *dataType, S row, FILE *fp ){

    	if( fp == NULL ){
		std::cout<<"Could not read file "<<wholeFileName<<"."<<std::endl;
        	exit(0);
	}

	for( S i=0; i<row; i++ ){
		for( S j=0; j<index[i].size(); j++){
        		fprintf(fp, dataType, matrix[i][index[i][j]]);
			fprintf(fp, " " );
		}
		fprintf(fp, "\n" );
	}
}

}

#endif /* File_operation*/
