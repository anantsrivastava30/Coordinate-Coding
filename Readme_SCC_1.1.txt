Sparse Coordinate Coding 

version 1.1

Author:  Qingyang Li  qingyang.li@asu.edu
	 Binbin Lin   binbinlinzju@gmail.com
	 
Log:
Feb. 15 version SCC_1.0
1. Support single sample file as input, the size of input files should not exceed the memory size. 

Mar. 12 version SCC_1.1
1. Support multiple sample files as input and no limitation on the size of input files. 


Input:

1. Sample 
Input sample is a matrix, and its size is NxM.
N is the total number of samples. 
M is the number of elements in one sample.

SCC1.1 support to read multiple sample files as the input. Please put all the sample files in the "Input" folder.


Output:
1. Sparse code
Ouput sparse code is a matrix, and its size is NxK.
N is the number of sparse codes, equal to the number of samples.
K is the number of elements in one sparse code.

If there are multiple input files, SCC1.1 generates multiple sparse code files in the "Output" folder.

2. Dictionary
The Dictionary is a matrix, and its size is MxK. 
M is the number of elements in one sample.
K is the number of bases in the dictionary, i.e., the number of elements in one sparse code.

SCC1.1 store the initial dictionary and learned dictionary in the "Dictionary" folder.


Variables need to configure:

File name:
1. initializedDictionaryName ( the name of initial dictionary )

2. savedDictionaryName ( the name of learned dictionary )

variable number: 
1. SampleFileNumber ( the number of sample files )

2. featureDim ( number of elements in each sparse code )

3. sampleDim ( number of elements in each sample )

4. epochs ( number of epochs )

5. layers ( number of layers in training process, default value is 3 )

6. lambda ( value of lambda, default is 0.13 )

7. blockSize ( number of samples loaded in memory in every iteration. It is up to the memory size of PC. Suggest the range from 10000 to 1000000 )

8. DictionaryGenerationState ( true/false )
   true: generate an initial dictionary 
   false:  use the initial dictionary provided by the user, the name of initialized dictionary file should be the value of initializedDictionaryName.

9. NonNegative ( true/false )
   true: the sample, dictionary and sparse code are non-negative.
   false: no constraint on the sign of the sample, dictionary and sparse code.


Operating Environment:
Linux (ubuntu/red hat)
g++ 4.6 and above version


How to run the program:

Version SCC_1.1:
Command to compile the program:  g++ run.cpp -o run -O3
command to run the program:   ./run



Version SCC_1.1:
Example: 
Two sample files: Input1.txt, Input2.txt in the "Input" folder

Output:
Dictionary: in the "Dictionary" folder
Sparse code: in the "Output" folder



Transpose Matrix:
The shape of input sample files is NxM. If you want to transpose the input matrix, you could use the TransposeMatrix.cpp to do it.

Command to compile the TransposeMatrix.cpp file:  g++ TransposeMatrix.cpp -o TransposeMatrix
Command to run the TransposeMatrix file: ./TransposeMatrix

When you run the "TransposeMatrix.cpp" file, you need to type the name of input file and output file. 





