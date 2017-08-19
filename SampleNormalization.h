#ifndef SAMPLE_NORMALIZATION_H
#define SAMPLE_NORMALIZATION_H

namespace dpl{

double **SampleInitialization( int sampleDim, int blockSize ){
	double **sample = (double**)malloc(blockSize*sizeof(double*));
	for( unsigned int i=0; i<blockSize; i++ )
		sample[i] = (double*)malloc(sampleDim*sizeof(double));
	return sample;
}

char *StringToCharArray( std::string value ){
	char *array =  (char*)malloc(value.length()+1*sizeof(char));
	for( int i=0; i<value.length(); i++ )
		array[i] = value[i];
	array[value.length()]='\0';
	return array;
}

int InputSampleFile( std::vector<std::string> &sampleFileName, std::string inputFolderName, std::vector<long long> &sampleFileSize, int sampleFileNumber){
	std::string inputFileName;
	char *fileName;
	std::cout<<"Total number of sample files is "<<sampleFileNumber<<std::endl;
	for( unsigned int i=0; i<sampleFileNumber; i++ ){
		std::cout<<"Please input the name of "<<i+1<<"th sample file:"<<std::endl;
		std::cin>>inputFileName;
        inputFileName = inputFolderName+inputFileName;
		sampleFileName.push_back(inputFileName);
	}

	int sampleNumber = 0;
	for( unsigned int i=0; i<sampleFileNumber; i++ ){
		fileName = StringToCharArray(sampleFileName[i]);
		std::ifstream fin( fileName, std::ifstream::in);
        	if( !fin ){
			std::cout<<"Could not find sample file: "<<sampleFileName[i]<<std::endl;
			exit(0);
		}
		std::string content;
		long long count = 0;
		while( getline(fin,content) ){
			count++;
		}
		sampleFileSize.push_back(count);
		sampleNumber += count;
		fin.close();
		free(fileName);
	}
	return sampleNumber;
}

void ReadAndNormalizeSample( double **sample, std::vector<std::string> sampleFileName, std::vector<long long> sampleFileSize, int sampleFileNumber, int sampleNumber, int sampleDim, int blockSize, bool NonNegative, char *currentDataPwd, char *indexFolderName, char *sampleFolderName, char *indexType, char *fileType, char *inputDataType, char *outputDataType ){

	unsigned int sampleFileIndex = 0;
	unsigned int sampleIndex = 0;
	unsigned int rangeNumber = blockSize;
	unsigned int samplingBlock = (sampleNumber % blockSize == 0? sampleNumber/blockSize: sampleNumber/blockSize + 1);
	char *fileName;
	fileName = StringToCharArray(sampleFileName[sampleFileIndex]);
	FILE *fp;
	fp = fopen( fileName, "r");
    if( fp == NULL ){
		std::cout<<"Could not find sample file: "<<sampleFileName[sampleFileIndex]<<std::endl;
		exit(0);
	}
	free(fileName);

	for( unsigned int i=0; i<samplingBlock; i++ ){

		if( i==samplingBlock-1 )
			rangeNumber = sampleNumber-i*blockSize;

		if( rangeNumber!=0 ){

			for( unsigned int k=0; k<rangeNumber; k++ ){

				fileName = StringToCharArray(sampleFileName[sampleFileIndex]);
				FileOperation::readVector( sample[k], sampleDim, fileName, inputDataType, fp );
				sampleIndex++;
				free(fileName);

				if( i*blockSize+k == sampleNumber-1 ){
					fclose(fp);
					break;
				}

				if( sampleIndex==sampleFileSize[sampleFileIndex] && sampleFileIndex<sampleFileNumber ){
					fclose(fp);
					fileName = StringToCharArray(sampleFileName[++sampleFileIndex]);
					fp = fopen( fileName, "r");
        				if( fp == NULL ){
						std::cout<<"Could not find sample file:   "<<sampleFileName[sampleFileIndex]<<std::endl;
						exit(0);
					}
					sampleIndex = 0;
					free(fileName);
				}
			}

			char *wholeFileName = FileOperation::getFileName<int>( i+1, currentDataPwd, sampleFolderName, indexType, fileType );
			FILE *fp1 = fopen( wholeFileName, "w");

			for( unsigned int k=0; k<rangeNumber; k++ ){
				double mean = 0;
				for( unsigned int j=0; j<sampleDim; j++ )
             				mean += sample[k][j];

				mean = mean/sampleDim;

				double sum = 0;
				for( unsigned int j=0; j<sampleDim; j++ ){
					if( !NonNegative)
                    				sample[k][j] -= mean;
					sum += sample[k][j]*sample[k][j];
				}

				double sqrtSum = sqrt(sum);
				if( sqrtSum!=0 ){
					for( unsigned int j=0; j<sampleDim; j++ )
						sample[k][j] = (sample[k][j]/sqrtSum);
				}
				FileOperation::writeVector<double*,int>( sample[k], sampleDim, wholeFileName, outputDataType, fp1 );
			}
			free(wholeFileName);
			fclose(fp1);
		}
	}
}

void clearSample( double **sample, int sampleNumber, int blockSize, char *currentDataPwd, char *sampleFolderName, char *indexType, char *fileType ){

	for( unsigned int i=0; i<blockSize; i++ )
		free(sample[i]);
	free(sample);

	unsigned int samplingBlock = (sampleNumber % blockSize == 0? sampleNumber/blockSize: sampleNumber/blockSize + 1);
	for( unsigned int i=0; i<samplingBlock; i++ ){
        char *wholeFileName = FileOperation::getFileName<int>(i+1, currentDataPwd, sampleFolderName, indexType, fileType);
        remove(wholeFileName);
        free(wholeFileName);
	}
}

}
#endif /* Sample Normalization */
