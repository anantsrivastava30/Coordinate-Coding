#ifndef DICTIONARY_GENERATION_H
#define DICTIONARY_GENERATION_H

namespace dpl{

double **InitializeDictionary( int featureDim, int sampleDim ){

	double **Wd = (double**)malloc(featureDim*sizeof(double*));
	for( unsigned int i=0; i<featureDim; i++ )
		Wd[i] = (double*)malloc(sampleDim*sizeof(double));

	return Wd;
}

double **GenerateRandomDictionary( int featureDim, int sampleDim ){

	double **Wd = InitializeDictionary( featureDim, sampleDim );
	srand((unsigned)time(0));
	unsigned int myseed = (unsigned int) RAND_MAX * rand();

	for( unsigned int i=0; i<sampleDim; i++ ){
		for( unsigned int j=0; j<featureDim; j++ ){
			Wd[j][i] = 2*(double) (rand_r(&myseed) / (RAND_MAX + 1.0))-1;
		}
	}
	return Wd;
}

double **GenerateRandomPatchDictionaryUniform( int featureDim, int sampleDim, int sampleNumber, int blockSize,
char *currentDataPwd, char *sampleFolderName, char *indexType, char *fileType, char *inputDataType, char *outputDataType ){

    	double **Wd = InitializeDictionary( featureDim, sampleDim );
    	srand((unsigned)time(0));
    	unsigned int myseed = (unsigned int) RAND_MAX * rand();
    	unsigned int samplingBlock = (sampleNumber % blockSize == 0? sampleNumber/blockSize: sampleNumber/blockSize + 1);
    	unsigned int samplingBlockSize = (featureDim % samplingBlock == 0? featureDim/samplingBlock : featureDim/samplingBlock + 1);

    	double content;

    	int wcount = 0;
    	for(unsigned int i = 0; i < samplingBlock; i++){
        	std::vector<unsigned int> index;
        	int tsamplingNum = samplingBlockSize;

        	int blockSampleNum = blockSize;
        	if (i == samplingBlock - 1){
                    blockSampleNum = sampleNumber - blockSize * (samplingBlock - 1);
            		tsamplingNum = featureDim - samplingBlockSize * (samplingBlock - 1);
        	}

        	for (unsigned int j = 0; j < tsamplingNum;){
            		unsigned int tidx = rand_r(&myseed) % blockSampleNum;
            		if(find(index.begin(),index.end(),tidx) == index.end()){
               			index.push_back(tidx);
               			j++;
            		}

        	}
        	sort(index.begin(),index.end());
        	char *wholeFileName = FileOperation::getFileName<int>( i/blockSize+1, currentDataPwd, sampleFolderName, indexType, fileType );
        	FILE *fp = fopen( wholeFileName, "r");

        	int count = 0;
        	int idx = 0;
        	while(count <= index[idx]){
              		for( unsigned int k=0; k<sampleDim; k++ ){
                		if( count==index[idx]){
                    			if( fscanf(fp, inputDataType, &Wd[wcount][k])==0 )
						exit(0);
				}
                		else{
                    			if( fscanf(fp, inputDataType, &content)==0 )
						exit(0);
				}
            	}
            	if(count == index[idx]){
                	wcount ++;
                	idx ++;
                	if(idx==tsamplingNum)
                    		break;
            	}
            	count++;
            }
            fclose(fp);
            free(wholeFileName);
        }
        return Wd;
}

void DictionaryNormalization( int featureDim, int sampleDim, double **Wd ){
	for( unsigned int i=0; i<featureDim; i++ ){
		double sum = 0;
		for( unsigned int j=0; j<sampleDim; j++ )
			sum += Wd[i][j]*Wd[i][j];
		sum = sqrt(sum);	
		if( sum!=0 ){
			for( unsigned int j=0; j<sampleDim; j++ )
				Wd[i][j] = Wd[i][j]/sum;
		}
	}
}

void saveDictionary( int featureDim, int sampleDim, double **Wd, char *dictionaryName, char *currentDataPwd, char *dictionaryFolderName ){

    	char *wholeName = FileOperation::combineFileName( dictionaryFolderName, currentDataPwd );
    	wholeName = FileOperation::combineFileName( dictionaryName, wholeName );
	FILE *fp;
    	fp = fopen( wholeName, "w");
    	if( fp == NULL ){
		printf("could not create dictionary file\n");
        	exit(0);
	}
	for( unsigned int i=0; i<sampleDim; i++ ){
		for( unsigned int j=0; j<featureDim; j++)
            		fprintf(fp, "%.50lf ", Wd[j][i]);
		fprintf(fp, "\n");
	}
	free(wholeName);
	fclose(fp);
}

double **readDictionary( char *dictionaryName, char *currentDataPwd, char *dictionaryFolderName, int featureDim, int sampleDim ) {

    double **Wd = InitializeDictionary( featureDim, sampleDim );
    char *wholeName = FileOperation::combineFileName( dictionaryFolderName, currentDataPwd );
    wholeName = FileOperation::combineFileName( dictionaryName, wholeName );
    FILE *fp;
    fp = fopen( wholeName, "r");
    if( fp == NULL ){
		printf("could not create dictionary file\n");
        	exit(0);
	}

    	for( unsigned int i=0; i<sampleDim; i++ ){
        	for( unsigned int j=0; j<featureDim; j++){
            		if( fscanf(fp, "%lf", &Wd[j][i])==0 )
				exit(0);
		}
	}
	free(wholeName);
	fclose(fp);
	return Wd;
}

void clearDictionary( int featureDim, double **Wd ){
	for( unsigned int i=0; i<featureDim; i++ )
		free(Wd[i]);
	free(Wd);
}

}

#endif /* Dictionary Generation*/
