/*
	Sparse Coordinate Coding  version 1.1
*/
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <ctime>
#include <iomanip>
#include <omp.h>
#include <string.h>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <string.h>
#include "Matrix.h"
#include "LR.h"
#include "FileOperation.h"
#include "SCC.h"
#include "DictionaryGeneration.h"
#include "SampleNormalization.h"

int main(int argc, char* argv[])
{
    	int sampleFileNumber = 2;
	int featureDim = 1000;
	int sampleDim = 128;
    	int epochs = 10;
    	int layers = 3;
	double lambda = 0.1;
	long long blockSize = 200000;
	bool DictionaryGenerationState = true;
	bool NonNegative = false;
	char initializedDictionaryName[1000] = "RandomPatch.txt";
	char savedDictionaryName[1000] = "Dictionary.txt";

	char currentDataPwd[1000] = "";
	char sampleFolderName[1000] = "Sample/";
	char featureFolderName[1000] = "Feature/";
	char indexFolderName[1000] = "Index/";
    	std::string inputFolderName = "Input/";
	char outputFolderName[1000] = "Output/";
	char dictionaryFolderName[1000] = "Dictionary/";
	char indexType[1000] = "%d";
	char fileType[1000] = ".txt";
	char inputDataType[1000] = "%lf";
	char outputDataType[1000] = "%.15lf";

	double **Wd;
	double **feature;
	double **sample;
    	long long iterationNumber;
	long long sampleNumber;
	std::vector<std::string> sampleFileName;
	std::vector<long long> sampleFileSize;
	std::cout<<"********************************************************************************"<<std::endl;
    	std::cout<<"********************************************************************************"<<std::endl;
    	std::cout<<"*****************Welcome to use the Sparse Coordinate Coding system*************"<<std::endl;
    	std::cout<<"********************************************************************************"<<std::endl;
    	std::cout<<"********************************************************************************"<<std::endl;
	std::cout<<"Begin to read sample."<<std::endl;
	sampleNumber = dpl::InputSampleFile( sampleFileName, inputFolderName, sampleFileSize, sampleFileNumber);
	iterationNumber = epochs*sampleNumber;

	std::cout<<"The number of samples is "<<sampleNumber<<std::endl;
	std::cout<<"The dimension of each sample is "<<sampleDim<<std::endl;
	std::cout<<"The dimension of each sparse code is "<<featureDim<<std::endl;
	std::cout<<"Total number of iterations is "<<iterationNumber<<std::endl;
	std::cout<<"lambda is "<<lambda<<std::endl;

	std::cout<<"Begin to normalize sample."<<std::endl;
	sample = dpl::SampleInitialization( sampleDim, blockSize );

	dpl::ReadAndNormalizeSample( sample, sampleFileName, sampleFileSize, sampleFileNumber, sampleNumber, sampleDim, blockSize, NonNegative, currentDataPwd, indexFolderName, sampleFolderName, indexType, fileType, inputDataType, outputDataType );

	std::cout<<"Begin to initialize dictionary."<<std::endl;

	if( DictionaryGenerationState )
		Wd = dpl::GenerateRandomPatchDictionaryUniform( featureDim, sampleDim, sampleNumber, blockSize,  currentDataPwd, sampleFolderName, indexType, fileType, inputDataType, outputDataType );
	else
		Wd = dpl::readDictionary( initializedDictionaryName, currentDataPwd, dictionaryFolderName, featureDim, sampleDim );

	dpl::DictionaryNormalization( featureDim, sampleDim, Wd );
	if( DictionaryGenerationState )
		dpl::saveDictionary( featureDim, sampleDim, Wd, initializedDictionaryName, currentDataPwd, dictionaryFolderName );

	std::cout<<"Start training "<<std::endl;
    	feature = dpl::FeatureInitialization( featureDim, blockSize );
	dpl::trainDecoder( Wd, feature, sample, lambda, layers, featureDim, sampleNumber, sampleDim, blockSize, iterationNumber, NonNegative, currentDataPwd, sampleFolderName, featureFolderName, indexFolderName, indexType, fileType, inputDataType, outputDataType );
	std::cout<<"Finish training "<<std::endl;

	dpl::saveDictionary( featureDim, sampleDim, Wd, savedDictionaryName, currentDataPwd, dictionaryFolderName );

	dpl::combineFeature( feature, sampleNumber, featureDim, blockSize, sampleFileSize, sampleFileNumber, currentDataPwd, featureFolderName, outputFolderName, indexFolderName, indexType, fileType, inputDataType, outputDataType );

	dpl::clearSample( sample, sampleNumber, blockSize, currentDataPwd, sampleFolderName, indexType, fileType );
	dpl::clearFeature( feature, sampleNumber, blockSize, currentDataPwd, featureFolderName, indexType, fileType );
	dpl::clearIndex( sampleNumber, blockSize, currentDataPwd, indexFolderName, indexType, fileType );
	dpl::clearDictionary( featureDim, Wd );
	return 0;
}
