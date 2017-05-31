#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <list>
#include <algorithm>
#include <vector>
//#include <bitset>
using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

typedef struct InputData {
	char name[20];
	unsigned int studentID;
	float score;
	unsigned int advisorID;	
}StudentData;

typedef struct Hashform {
	//int binaryNum;
	int pointBlockNum = 0;
}HashData;

typedef struct Blockform {
	int thisBlockNum;
	//int recordedNum = 0;
	vector <StudentData> studentData;
	//StudentData studentData[128];
}BlockData;

class StudentsFileStruct {
	private:
		int studentNum;
		StudentData *studentsData;
		vector <HashData> hashNode;
		vector <HashData>::iterator hash_iter;
		vector <BlockData> blockNode;
		vector <BlockData>::iterator block_iter;
		//list <BlockData>::iterator block_iter;
		
	public:
		void readStudentTable() {
			ifstream readStudFile("sampleData.csv");
			
			string tmp_studentNum;
			getline(readStudFile, tmp_studentNum, '\n');
			studentNum = atoi(tmp_studentNum.c_str());
			//cout << studentNum << endl;
			studentsData = new StudentData[studentNum];
			
			int dataLocate = 0;
			string tmp_name, tmp_studentID, tmp_score, tmp_advisorID;
			while(getline(readStudFile, tmp_name,',')&&getline(readStudFile, tmp_studentID,',')&&getline(readStudFile, tmp_score,',')&&getline(readStudFile,tmp_advisorID,'\n')) {
				strcpy(studentsData[dataLocate].name,tmp_name.c_str());
				studentsData[dataLocate].studentID = atoi(tmp_studentID.c_str());
				studentsData[dataLocate].score = atof(tmp_score.c_str());
				studentsData[dataLocate].advisorID = atoi(tmp_advisorID.c_str());
				//cout << studentsData[dataLocate].name << "," << studentsData[dataLocate].studentID << "," << studentsData[dataLocate].score << "," << studentsData[dataLocate].advisorID << endl;
				dataLocate ++;
				
			}
			
			readStudFile.close();
			
		}
		
		/*int calPowingNum(int powNumber) {
			int thisPowingNum = 1;
			for(int i = 0; i < powNumber; i++) {
				thisPowingNum *= 10;
			}
			
			return thisPowingNum;
		}*/
		
		/*int binToDec(string number) {
			int result = 0, pow = 1;
			for(int i = number.length()-1;i>=0;i--,pow <<= 1) {
				result += (number[i] - '0') * pow;
			}
			return result;
		}*/
		
		string hashFunction(unsigned int studentID) {
			/*string binary = std::bitset<8>(128).to_string();
			std::cout << binary << endl;*/
			//int hashingValue = 0;
			//int decimal_pow = 0;
			
			if(studentID == 0) {
				return "0";
			} else if (studentID== 1) {
				return "1";
			} else if (studentID % 2 == 0) {
				return hashFunction(studentID / 2) + "0";
			} else {
				return hashFunction(studentID / 2) + "1";
			}
			
		}
		
		/*int hashFunction(unsigned int studentID, int binaryDigit) {
			int whatPow = 1;
			
			for(int i = 0; i < binaryDigit; i++) {
				whatPow *= 2; 
			}
			
			int hashingValue;
			
			hashingValue = studentID % whatPow;
			
			return hashingValue;
		}*/
		
		void calculate_DB_HashTable() {
			HashData tmp_hashData;
			BlockData tmp_blockData;
			
			int howManyInput = 0;                                                    
			int useBinaryDigit = 0;
			int endBlockLocate = 1;
			
			/*for(int init=0; init < 2; init++) {
				//tmp_hashData.binaryNum = init;
				hashNode.push_back(tmp_hashData);
			}*/
			
			while(howManyInput < studentNum) {
				string hasingValue = hashFunction(studentsData[howManyInput].studentID);
				//cout << hasingValue << endl;
				if(hashNode.empty()) {
					hashNode.push_back(tmp_hashData);
				}
				unsigned int thisHashLoc = 0;
				
				for(hash_iter = hashNode.begin(); hash_iter != hashNode.end(); hash_iter++) {
					
					if(useBinaryDigit == 0) {
						// this hashLoc = 0;
						break;
					} else {
						bool outThisFor = false;
						string hashLocBi = hashFunction(thisHashLoc);
						if(hashLocBi.length() < useBinaryDigit) {
							while(hashLocBi.length() != useBinaryDigit) {
								hashLocBi = "0" + hashLocBi;
							}
						}
						
						if(hasingValue.substr(hasingValue.length() - useBinaryDigit, useBinaryDigit) == hashLocBi) {
							//cout << hasingValue.substr(hasingValue.length() - useBinaryDigit, useBinaryDigit) << endl;
							outThisFor = true;
						}
						if(outThisFor) {
							break;
						}
					}
					
					thisHashLoc ++;
				}
				
				
				//cout << hashNode[thisHashLoc].pointBlockNum;
				if(hashNode[thisHashLoc].pointBlockNum == 0) {
					hashNode[thisHashLoc].pointBlockNum = endBlockLocate;
					tmp_blockData.thisBlockNum = endBlockLocate;
					tmp_blockData.studentData.push_back(studentsData[howManyInput]);
					blockNode.push_back(tmp_blockData);
					endBlockLocate ++;
				} else {
					int thisBlockLoc = 0;
					
					for(block_iter = blockNode.begin(); block_iter != blockNode.end(); block_iter++) {
						if(hashNode[thisHashLoc].pointBlockNum == blockNode[thisBlockLoc].thisBlockNum) {
							break;
						}
						thisBlockLoc ++;
					}
					
					int hashPointerNum = 0;
					for(int i = 0; i < hashNode.size(); i++) {
						if(hashNode[i].pointBlockNum == blockNode[thisBlockLoc].thisBlockNum) {
							hashPointerNum ++;
						}
					}
					
					if(blockNode[thisBlockLoc].studentData.size() <= 128 ) {
						blockNode[thisBlockLoc].studentData.push_back(studentsData[howManyInput]);
					} else {
						while() {
							if(hashPointerNum != 1) {
							
							} else {
							
							}	
						}
						
						/*vector <HashData> tmp_hashNode = hashNode;
						hashNode.insert(hashNode.end(), tmp_hashNode.begin(), tmp_hashNode.end());*/
					}
				}
				
				/*for(hash_iter = hashNode.begin(); hash_iter != hashNode.end(); hash_iter++) {
					unsigned int thisHashLoc = 0;
					
					if(useBinaryDigit == 0) {
						if((*hash_iter).pointBlockNum == NULL) {
							(*hash_iter).pointBlockNum = endBlockLocate;
							tmp_blockData.thisBlockNum = endBlockLocate;
							tmp_blockData.recordedNum = 0;
							tmp_blockData.studentData[tmp_blockData.recordedNum] = studentsData[howManyInput];
							//cout << tmp_blockData.recordedNum;
							blockNode.push_back(tmp_blockData);
							//cout << "n";
							endBlockLocate ++;
						} else if((*block_iter).recordedNum < 128) {
						//	cout << (*block_iter).recordedNum;
							//(*block_iter).recordedNum = (*block_iter).recordedNum + 1;
							//(*block_iter).studentData[(*block_iter).recordedNum] = studentsData[howManyInput];
							//(*block_iter).recordedNum ++;
							//cout << (*block_iter).recordedNum;
						} else {
							cout << "no" << endl;
						}
					} else {
						//if(hasingValue.substr(hasingValue.length() - useBinaryDigit, useBinaryDigit) == hashFunction(thisHashLoc)) {
						//	cout << hasingValue.substr(hasingValue.length() - useBinaryDigit, useBinaryDigit);
						//}
					}*/
					
					
					/*if((*hash_iter).pointBlockNum == NULL) {
						(*hash_iter).pointBlockNum = endBlockLocate
						tmp_blockData.thisBlockNum = endBlockLocate;
						tmp_blockData.recordedNum = 0;
						tmp_blockData.studentData[tmp_blockData.recordedNum] = studentsData[howManyInput];
						blockNode.push_back(tmp_blockData);
						//cout << "n";
						endBlockLocate ++;
					}
					
					thisHashLoc ++;
					
					
				}*/
				/*for(hash_iter = hashNode.begin(); hash_iter != hashNode.end(); hash_iter++) {
					if((*hash_iter).binaryNum == hasingValue) {
						if((*hash_iter).pointBlockNum == NULL) {
							(*hash_iter).pointBlockNum = endBlockLocate;
							tmp_blockData.thisBlockNum = endBlockLocate;
							tmp_blockData.recordedNum = 0;
							tmp_blockData.studentData[tmp_blockData.recordedNum] = studentsData[howManyInput];
							blockNode.push_back(tmp_blockData);
							//cout << "n";
							endBlockLocate ++;
						}
						else {
							for(block_iter = blockNode.begin(); block_iter != blockNode.end(); block_iter++) {
								if((*block_iter).thisBlockNum == (*hash_iter).pointBlockNum) {
									//(*block_iter)
									
								}
							}
							//block_iter = find(blockNode.begin(), blaockNode.end(), ???);
						}
						
					}
					
				}*/
				//if(hasingValue == )
				//hash_iter = hashNode.begin();
				//cout << hasingValue << endl;
				
				//if(hash_iter=find(hashNode.begin(), hashNode.end(), binaryNum))
				
				
				howManyInput ++;
			}
		}
		
		
		void writeStudentDB() {
			//readStudentTable();
		} 
		
		void writeHashTable() {
			
		}
		
		void calculate_B_plusTree() {
			
		}
		
		void writeB_PlusTree() {
			
		}
		
		void hashTablePrint() {
			
		}
		
		void kthNodePrint() {
			
		}
	
	
	
};
int main(int argc, char** argv) {
	StudentsFileStruct studentsFS;
	studentsFS.readStudentTable();
	studentsFS.calculate_DB_HashTable();
	return 0;
}
