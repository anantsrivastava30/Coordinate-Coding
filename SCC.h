#ifndef SPARSE_COORDINATE_CODING_H
#define SPARSE_COORDINATE_CODING_H

namespace dpl{

static unsigned int myseed;

double getAbs( double value ){
	if( value < 0 )
		return -1*value;
	else
		return value;
}

double **FeatureInitialization( int featureDim, int blockSize ){

	double **feature = (double**)malloc(blockSize*sizeof(double*));
        for( unsigned int i=0; i<blockSize; i++ )
		feature[i] = (double*)malloc(featureDim*sizeof(double));
	return feature;
}

void FeatureInitialization( double **feature, int featureDim, int blockSize ){

    for( unsigned int i=0; i<blockSize; i++ ){
		for( unsigned int j=0; j<featureDim; j++ )
			feature[i][j] = 0;
	}
}

std::vector<int> *NonZeroIndexInitialization( int blockSize  ){
	std::vector<int> *nonZeroIndex = new std::vector<int> [blockSize];
	return nonZeroIndex;
}

void NonZeroIndexInitialization( std::vector<int> *nonZeroIndex, int blockSize ){
	 for( unsigned int i=0; i<blockSize; i++ )
		nonZeroIndex[i].resize(0);
}

double ShrinkageFunction( double value, double theta ){

	if( value < -theta )
		return value+theta;
	else if( value > theta )
		return value-theta;
	else
		return 0;
}

double *Initialize_A_Copy( int featureDim ){

	double *A_Copy = (double*)malloc(featureDim*sizeof(double));
	for( unsigned int i=0; i<featureDim; i++ )
		A_Copy[i]=0;
	return A_Copy;
}

double *Initialize_A( int featureDim ){

	double *A = (double*)malloc(featureDim*sizeof(double));
	for( unsigned int i=0; i<featureDim; i++ )
		A[i]=0;
	return A;
}

void Initialize_A( double *A, double *A_Copy, int featureDim ){
	for( unsigned int i=0; i<featureDim; i++ ){
		A[i]=A_Copy[i];
		A_Copy[i]=0;
    }
}

void Update_A( double *A, double *A_Copy, double *feature, std::vector<int> &nonZeroIndex ){
	for( unsigned int i=0; i<nonZeroIndex.size(); i++ ){
		A[nonZeroIndex[i]] += feature[nonZeroIndex[i]]*feature[nonZeroIndex[i]];
		A_Copy[nonZeroIndex[i]] += feature[nonZeroIndex[i]]*feature[nonZeroIndex[i]];
	}
}

double getNonNegativeFeature( double featureElement, double optimalT ){
	if( featureElement+optimalT>=0 )
		return optimalT;
	else
		return -1*featureElement;
}

int *getRandomIndex( int size ){

	std::vector<int> index (size);
	int *data=(int*)malloc(size*sizeof(int));
	for( unsigned int i=0; i<size; i++ )
	        index[i] = i;

	for( unsigned int i=0; i<size; i++ ){
    		int randomIndex = rand_r(&myseed)%index.size();
        	data[i] = index[randomIndex];
        	index.erase(index.begin()+randomIndex);
    	}
    	return data;
}

void UpdateFeature( double **Wd, double *sample, double *residuals, double *feature, std::vector<int> &nonZeroIndex, double lambda, int layers, int featureDim, int sampleDim, bool NonNegative ){

    	for( unsigned int i = 0; i<sampleDim; i++ ){
  		residuals[i] = -sample[i];
    		for( unsigned int j = 0; j<nonZeroIndex.size(); j++ )
    		        residuals[i] += Wd[nonZeroIndex[j]][i]*feature[nonZeroIndex[j]];
    	}

	nonZeroIndex.resize(0);
	int *randomIndex = getRandomIndex(featureDim );

	for ( unsigned int i = 0; i < featureDim; i++ ){

        	double optimalT;
        	double derivative = 0;

        	for (unsigned int j = 0;j < sampleDim; j++)
                	derivative += (residuals[j]*Wd[randomIndex[i]][j]);

		optimalT = ShrinkageFunction( feature[randomIndex[i]]-derivative, lambda )-feature[randomIndex[i]];

		if( NonNegative )
			optimalT = getNonNegativeFeature( feature[randomIndex[i]], optimalT );

		feature[randomIndex[i]] += optimalT;

        	if ( optimalT!=0 ){
            		for (unsigned int j = 0;j < sampleDim; j++)
                		residuals[j] += optimalT*Wd[randomIndex[i]][j];
        	}

		if( feature[randomIndex[i]]!=0 )
			nonZeroIndex.push_back(randomIndex[i]);

	}

	for ( unsigned int k = 1; k < layers; k++ ){
		for ( unsigned int i = 0; i < nonZeroIndex.size(); i++ ){
        		double optimalT;
        		double derivative = 0;
        		for (unsigned int j = 0;j < sampleDim; j++)
                		derivative += (residuals[j]*Wd[nonZeroIndex[i]][j]);

			optimalT = ShrinkageFunction( feature[nonZeroIndex[i]]-derivative, lambda )-feature[nonZeroIndex[i]];

			if( NonNegative )
				optimalT = getNonNegativeFeature( feature[nonZeroIndex[i]], optimalT );
			feature[nonZeroIndex[i]] += optimalT;

        		if ( optimalT!=0 ){
            			for (unsigned int j = 0;j < sampleDim; j++)
                			residuals[j] += optimalT*Wd[nonZeroIndex[i]][j];
        		}
		}
	}

	nonZeroIndex.resize(0);
	for ( unsigned int i = 0; i < featureDim; i++ ){
		if( feature[i]!=0 )
			nonZeroIndex.push_back(i);
	}
	free(randomIndex);
}


void UpdateWd( double **Wd, double *residuals, double *feature, double *A, std::vector<int> &nonZeroIndex, int sampleDim, bool NonNegative ){

    	for ( unsigned int i = 0; i < sampleDim; i++ ){
        	for ( unsigned int j = 0; j < nonZeroIndex.size(); j++ ){
			if( NonNegative && Wd[nonZeroIndex[j]][i] - feature[nonZeroIndex[j]]*residuals[i]*dpl::learningRate(A,nonZeroIndex[j])<0 )
                    		Wd[nonZeroIndex[j]][i] = 0;
                	else
            			Wd[nonZeroIndex[j]][i] = Wd[nonZeroIndex[j]][i] - feature[nonZeroIndex[j]]*residuals[i]*dpl::learningRate(A,nonZeroIndex[j]);
		}
    	}
}

void NormalizeWd( double **Wd, std::vector<int> &nonZeroIndex, int sampleDim ){
	for( unsigned int i=0; i<nonZeroIndex.size(); i++ ){
		double sum = 0;
		for( unsigned int j=0; j<sampleDim; j++ )
			sum += Wd[nonZeroIndex[i]][j]*Wd[nonZeroIndex[i]][j];
		sum = sqrt(sum);

		if( sum!=0 ){
			for( unsigned int j=0; j<sampleDim; j++ )
				Wd[nonZeroIndex[i]][j] = Wd[nonZeroIndex[i]][j]/sum;	
		}
	}
}

void readSampleByBlock( double **matrix, int row, int column, int index, char *currentDataPwd, char *FolderName, char *indexType, char *fileType, char *dataType ){
	char *wholeFileName = FileOperation::getFileName<int>(index, currentDataPwd, FolderName, indexType, fileType);
	FileOperation::readMatrix<double,int>( matrix, wholeFileName, dataType, row, column );
	free(wholeFileName);
}

void readFeatureByBlock( double **matrix, std::vector<int> *nonZeroIndex, int row, int column, int index, char *currentDataPwd, char *FolderName, char *indexType, char *fileType, char *dataType ){
	char *wholeFileName = FileOperation::getFileName<int>(index, currentDataPwd, FolderName, indexType, fileType);
	FileOperation::readSparseMatrix<double, std::vector<int>, int>( matrix, nonZeroIndex, wholeFileName, dataType, row );
	free(wholeFileName);
}

void readNonIndexByBlock( std::vector<int> *nonZeroIndex, int blockSize, int index, char *currentDataPwd, char *FolderName, char *indexType, char *fileType, char *dataType ){

	char *wholeFileName = FileOperation::getFileName<int>(index, currentDataPwd, FolderName, indexType, fileType);
	FILE *fp;
        fp = fopen( wholeFileName, "r");
        if( fp == NULL ){
		printf("could not find index file %s\n", wholeFileName);
            	exit(0);
	}

	int nonZeroNumber;
	for( int i=0; i<blockSize; i++ ){
		if( fscanf(fp, indexType, &nonZeroNumber)==0 )
			exit(0);
		nonZeroIndex[i].resize(nonZeroNumber);
		FileOperation::readVector<std::vector<int>,int>( nonZeroIndex[i], nonZeroIndex[i].size(), wholeFileName, dataType, fp );
	}
	free(wholeFileName);
	fclose(fp);
}

void saveSampleByBlock( double **matrix, int row, int column, int index, char *currentDataPwd, char *FolderName, char *indexType, char *fileType, char *dataType ){

	char *wholeFileName = FileOperation::getFileName<int>(index, currentDataPwd, FolderName, indexType, fileType);
	FileOperation::saveMatrix<double,int>( matrix, wholeFileName, dataType, row, column );
	free(wholeFileName);
}

void saveFeatureByBlock( double **matrix, std::vector<int> *nonZeroIndex, int row, int column, int index, char *currentDataPwd, char *FolderName, char *indexType, char *fileType, char *dataType ){

	char *wholeFileName = FileOperation::getFileName<int>(index, currentDataPwd, FolderName, indexType, fileType);
	FileOperation::saveSparseMatrix<double,std::vector<int>,int>( matrix, nonZeroIndex, wholeFileName, dataType, row );
	free(wholeFileName);
}

void saveNonIndexByBlock( std::vector<int> *nonZeroIndex, int blockSize, int index, char *currentDataPwd, char *FolderName, char *indexType, char *fileType, char *dataType ){

	char *wholeFileName = FileOperation::getFileName<int>(index, currentDataPwd, FolderName, indexType, fileType);
	FILE *fp;
        fp = fopen( wholeFileName, "w");
        if( fp == NULL ){
		printf("could not find index file %s\n", wholeFileName);
            	exit(0);
	}

	for( int i=0; i<blockSize; i++ ){
		fprintf(fp, indexType, nonZeroIndex[i].size());
		fprintf(fp, " ");
		FileOperation::writeVector<std::vector<int>,int>( nonZeroIndex[i], nonZeroIndex[i].size(), wholeFileName, dataType, fp );
	}
	free(wholeFileName);
	fclose(fp);
}

void clearFeature( double **feature, int sampleNumber, int blockSize, char *currentDataPwd, char *featureFolderName, char *indexType, char *fileType ){

	for( unsigned int i=0; i<blockSize; i++ )
		free(feature[i]);
	free(feature);

	unsigned int samplingBlock = (sampleNumber % blockSize == 0? sampleNumber/blockSize: sampleNumber/blockSize + 1);
	for( unsigned int i=0; i<samplingBlock; i++ ){
        char *wholeFileName = FileOperation::getFileName<int>(i+1, currentDataPwd, featureFolderName, indexType, fileType);
        remove(wholeFileName);
        free(wholeFileName);
	}
}

void clearIndex( int sampleNumber, int blockSize, char *currentDataPwd, char *indexFolderName, char *indexType, char *fileType ){

	unsigned int samplingBlock = (sampleNumber % blockSize == 0? sampleNumber/blockSize: sampleNumber/blockSize + 1);
	for( unsigned int i=0; i<samplingBlock; i++ ){
        char *wholeFileName = FileOperation::getFileName<int>(i+1, currentDataPwd, indexFolderName, indexType, fileType);
        remove(wholeFileName);
        free(wholeFileName);
	}
}


int getRangeNumber( int sampleNumber, int blockSize, int index ){
	if( sampleNumber%blockSize!=0 && sampleNumber-index<blockSize )
		return sampleNumber%blockSize;
	else
		return blockSize;
}

void combineSparseFeature( double **feature, int sampleNumber, int featureDim, int blockSize, char *FeatureFileName, char *currentDataPwd, char *featureFolderName, char *indexFolderName, char *indexType, char *fileType, char *inputDataType, char *outputDataType ){

	FILE *fp;
	fp = fopen( FeatureFileName, "w");
        if( fp == NULL ){
		printf("Could not find sparse code file.\n");
		exit(0);
	}
	int rangeNumber = 0;
	std::vector<int> *nonZeroIndex = NonZeroIndexInitialization( blockSize );
	for( unsigned int t=0; t<sampleNumber; t++ ){
		if( t%blockSize==0 ){
			rangeNumber = getRangeNumber( sampleNumber, blockSize, t );
			FeatureInitialization( feature, featureDim, blockSize );
			NonZeroIndexInitialization( nonZeroIndex, blockSize );
			readNonIndexByBlock( nonZeroIndex, rangeNumber, t/blockSize+1, currentDataPwd, indexFolderName,  indexType, fileType, indexType );
			readFeatureByBlock( feature, nonZeroIndex, rangeNumber, featureDim, t/blockSize+1, currentDataPwd, featureFolderName, indexType, fileType, inputDataType );
			FileOperation::saveSparseMatrix<double,std::vector<int>,int>( feature, nonZeroIndex, FeatureFileName, outputDataType, rangeNumber, fp );
		}
	}
	fclose(fp);
}

void combineFeature( double **feature, int sampleNumber, int featureDim, int blockSize, std::vector<long long> sampleFileSize, int featureFileNumber, char *currentDataPwd, char *featureFolderName, char *outputFolderName, char *indexFolderName, char *indexType, char *fileType, char *inputDataType, char *outputDataType ){

	std::cout<<"Generate output sparse code files."<<std::endl;
	int rangeNumber = 0;
	int featureFileIndex = 0;
	int featureIndex = 0;

	char *wholeFileName = FileOperation::getFileName<int>(++featureFileIndex, currentDataPwd, outputFolderName, indexType, fileType);
	FILE *fp;
	fp = fopen( wholeFileName, "w");
        if( fp == NULL ){
		std::cout<<"Could not find the file: "<<wholeFileName<<std::endl;
		exit(0);
	}

	std::vector<int> *nonZeroIndex = NonZeroIndexInitialization( blockSize );
	for( unsigned int t=0; t<sampleNumber; t++ ){
		if( t%blockSize==0 ){
			rangeNumber = getRangeNumber( sampleNumber, blockSize, t );
			FeatureInitialization( feature, featureDim, blockSize );
			NonZeroIndexInitialization( nonZeroIndex, blockSize );
			readNonIndexByBlock( nonZeroIndex, rangeNumber, t/blockSize+1, currentDataPwd, indexFolderName,  indexType, fileType, indexType );
			readFeatureByBlock( feature, nonZeroIndex, rangeNumber, featureDim, t/blockSize+1, currentDataPwd, featureFolderName, indexType, fileType, inputDataType );
			for( unsigned int k=0; k<rangeNumber; k++ ){

				FileOperation::writeVector( feature[k], featureDim, wholeFileName, outputDataType, fp );
				featureIndex++;

                if( t+k == sampleNumber-1 ){
					fclose(fp);
                    free(wholeFileName);
					break;
				}
				else if( featureIndex==sampleFileSize[featureFileIndex-1] && featureFileIndex<featureFileNumber ){
					free(wholeFileName);
					fclose(fp);
					wholeFileName = FileOperation::getFileName<int>(++featureFileIndex, currentDataPwd, outputFolderName, indexType, fileType);
					fp = fopen( wholeFileName, "w");
        				if( fp == NULL ){
						std::cout<<"Could not find the file: "<<wholeFileName<<std::endl;
						exit(0);
					}
					featureIndex = 0;
				}
			}
		}
	}
	delete [] nonZeroIndex;
}


double computeLassoResult( double **Wd, double *sample, double *feature, double lambda, int sampleDim, int featureDim ){

	double LassoResult = 0;
	double residuals;
	for( unsigned int i=0; i<sampleDim; i++ ){
		residuals = -sample[i];
		for( unsigned int j=0; j<featureDim; j++ )
			residuals += Wd[j][i]*feature[j];

		LassoResult += residuals*residuals;
	}

	double sum_feature = 0;
	for( unsigned int j=0; j<featureDim; j++ )
		sum_feature += getAbs(feature[j]);

    	return 0.5*LassoResult+lambda*sum_feature;
}

void calculateError(  double **Wd,  double **sample, double **feature, std::vector<int> *nonZeroIndex, double lambda, int sampleNumber, int sampleDim, int featureDim, char *currentDataPwd, char *sampleFolderName, char *featureFolderName, char *indexFolderName, char *indexType, char *fileType, char *inputDataType, int blockSize ) {

	double TotalDecError = 0;
	int rangeNumber;
	for( unsigned int t=0; t<sampleNumber; t++ ){
		if( t%blockSize==0 ){
			rangeNumber = getRangeNumber( sampleNumber, blockSize, t );
			FeatureInitialization( feature, featureDim, blockSize );
			NonZeroIndexInitialization( nonZeroIndex, blockSize );

			readSampleByBlock( sample, rangeNumber, sampleDim, t/blockSize+1, currentDataPwd, sampleFolderName, indexType, fileType, inputDataType );
			readNonIndexByBlock( nonZeroIndex, rangeNumber, t/blockSize+1, currentDataPwd, indexFolderName,  indexType, fileType, indexType );
			readFeatureByBlock( feature, nonZeroIndex, rangeNumber, featureDim, t/blockSize+1, currentDataPwd, featureFolderName, indexType, fileType, inputDataType );
		}
		TotalDecError += computeLassoResult( Wd, sample[t%blockSize], feature[t%blockSize], lambda, sampleDim, featureDim);
	}
	TotalDecError /= sampleNumber;
	std::cout<<"Total Decode Error is "<<TotalDecError<<std::endl;
}

void trainDecoder( double **Wd, double **feature, double **sample, double lambda, int layers, int featureDim, int sampleNumber, int sampleDim, int blockSize, int iterationNumber, bool NonNegative, char *currentDataPwd, char *sampleFolderName, char *featureFolderName, char *indexFolderName, char *indexType, char *fileType, char *inputDataType, char *outputDataType ){

	double *residuals = (double*)malloc(sampleDim*sizeof(double));
	double *A = Initialize_A( featureDim );
	double *A_Copy = Initialize_A_Copy( featureDim );
	std::vector<int> *nonZeroIndex = NonZeroIndexInitialization( blockSize );
	int rangeNumber;

	srand((unsigned)time(0));
	myseed = (unsigned int) RAND_MAX * rand();

	std::cout<<"Train decoder"<<std::endl;
	double ComputionalTime = 0;
	clock_t BeginTime = clock();
    	for( unsigned int it=0; it<iterationNumber; it++ ){

		int index = it%sampleNumber;
		if( index%blockSize==0 ){

			rangeNumber = getRangeNumber( sampleNumber, blockSize, index );
			readSampleByBlock( sample, rangeNumber, sampleDim, index/blockSize+1, currentDataPwd, sampleFolderName, indexType, fileType, inputDataType );
			FeatureInitialization( feature, featureDim, blockSize );
			if( it/sampleNumber>0 ){
				readNonIndexByBlock( nonZeroIndex, rangeNumber, index/blockSize+1, currentDataPwd, indexFolderName,  indexType, fileType, indexType );
				readFeatureByBlock( feature, nonZeroIndex, rangeNumber, featureDim, index/blockSize+1, currentDataPwd, featureFolderName, indexType, fileType, inputDataType );
			}
			else
				NonZeroIndexInitialization( nonZeroIndex, blockSize );
		}

		if( index==0 )
			Initialize_A( A, A_Copy, featureDim );

		UpdateFeature( Wd, sample[index%blockSize], residuals, feature[index%blockSize], nonZeroIndex[index%blockSize], lambda, layers, featureDim, sampleDim, NonNegative );

		Update_A( A, A_Copy, feature[index%blockSize], nonZeroIndex[index%blockSize] );
		UpdateWd( Wd, residuals, feature[index%blockSize], A, nonZeroIndex[index%blockSize], sampleDim, NonNegative );

		NormalizeWd( Wd, nonZeroIndex[index%blockSize], sampleDim );

		if( index%blockSize==blockSize-1 || index%sampleNumber==sampleNumber-1 ){

			//rangeNumber = getRangeNumber( sampleNumber, blockSize, index );
			saveNonIndexByBlock( nonZeroIndex, rangeNumber, index/blockSize+1, currentDataPwd, indexFolderName,  indexType, fileType, indexType );

			saveFeatureByBlock( feature, nonZeroIndex, rangeNumber, featureDim, index/blockSize+1, currentDataPwd, featureFolderName, indexType, fileType, outputDataType );
		}

		if( it%sampleNumber==sampleNumber-1 )
            		std::cout<<it+1<<" iterations finished"<<std::endl;
	}
    	clock_t EndTime = clock();
	ComputionalTime = (double)(EndTime-BeginTime)/CLOCKS_PER_SEC;
	calculateError( Wd, sample, feature, nonZeroIndex, lambda, sampleNumber, sampleDim, featureDim, currentDataPwd, sampleFolderName, featureFolderName, indexFolderName, indexType, fileType, inputDataType, blockSize );
   	std::cout<<"Finish decoding process:"<<std::endl;
	std::cout<<"Train Decode Time is "<<ComputionalTime<<" seconds."<<std::endl;
	free(A_Copy);
	free(residuals);
	delete [] nonZeroIndex;
}


}

#endif /* Sparse Coordinate Coding */

