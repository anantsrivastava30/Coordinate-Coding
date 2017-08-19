#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <string>
using namespace std;

std::vector<std::string> split(std::string const &input) { 
	std::istringstream buffer(input);
	std::vector<std::string> ret;
	std::copy(std::istream_iterator<std::string>(buffer),
	std::istream_iterator<std::string>(),
	std::back_inserter(ret));
	return ret;
}

int getSampleNumber( char *sampleFileName ) {
	std::string line;
  	std::ifstream file (sampleFileName);
	std::vector<std::string> firstLine;
  	if (file.is_open()){
    		if( getline (file,line) )
      			firstLine = split(line);
    		file.close();
  	}
	return firstLine.size();
}

int main()
{
	char inputFileName[10000];
	char outputFileName[10000];
	int sampleNumber;
	int sampleElementNumber;

	cout<<"Please type the input file name: "<<endl;
	cin>>inputFileName;
	cout<<"Please type the output file name: "<<endl;
	cin>>outputFileName;
	sampleNumber = getSampleNumber(inputFileName);
	cout<<"Total number of samples is "<<sampleNumber<<endl;
	cout<<"Please type number of elements in each sample "<<endl;
	cin>>sampleElementNumber;

	double **sample = (double**)malloc(sampleNumber*sizeof(double*));
	for( unsigned int i=0; i<sampleNumber; i++ )
		sample[i] = (double*)malloc(sampleElementNumber*sizeof(double));
	
	FILE *fp;
       	fp = fopen( inputFileName, "r");
        if( fp == NULL ){
		cout<<"Could not find the file "<<inputFileName<<endl;
        	exit(0);	
	}
	for( unsigned int j=0; j<sampleElementNumber; j++ ){
		for( unsigned int i=0; i<sampleNumber; i++ )
			fscanf(fp, "%lf", &sample[i][j]);
	}
	fclose(fp);

       	fp = fopen( outputFileName, "w");
        if( fp == NULL ){
		cout<<"Could not find the file "<<inputFileName<<endl;
        	exit(0);	
	}
	for( unsigned int i=0; i<sampleNumber; i++ ){
		for( unsigned int j=0; j<sampleElementNumber; j++ )
			fprintf(fp, "%.15lf ", sample[i][j]);
		fprintf(fp, "\n");
	}
	
	cout << "Transpose finished. " << endl;
	for( unsigned int i=0; i<sampleNumber; i++ )
		free(sample[i]); 
	free(sample);
	return 0;
}
