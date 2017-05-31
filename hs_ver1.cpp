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
		
		void calculate_DB_HashTable() {
			HashData tmp_hashData;
			BlockData tmp_blockData;
			
			int howManyInput = 0;                                        
			int useBinaryDigit = 0;
			int endBlockLocate = 1;
			
			//start input studentData
			while(howManyInput < studentNum) {
				string hasingValue = hashFunction(studentsData[howManyInput].studentID);
				//cout << hasingValue << endl;
				if(hashNode.empty()) {
					hashNode.push_back(tmp_hashData);
				}
				unsigned int thisHashLoc = 0;
				string hashLocBi;
				
				//find hashNode's location mathing this data's hasingValue
				for(hash_iter = hashNode.begin(); hash_iter != hashNode.end(); hash_iter++) {
					hashLocBi = hashFunction(thisHashLoc);
					if(hashLocBi.length() < useBinaryDigit) {
						while(hashLocBi.length() != useBinaryDigit) {
							hashLocBi = "0" + hashLocBi;
						}
					}
					
					if(useBinaryDigit == 0) {
						// this hashLoc = 0;
						break;
					} else {
						bool outThisFor = false;
						
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
				
				//start this data input block
				if(hashNode[thisHashLoc].pointBlockNum == 0) { // this hashNode's pointBlock is null
					hashNode[thisHashLoc].pointBlockNum = endBlockLocate;
					tmp_blockData.thisBlockNum = endBlockLocate;
					tmp_blockData.studentData.push_back(studentsData[howManyInput]);
					blockNode.push_back(tmp_blockData);
					endBlockLocate ++;
				} else { //this hashNode's pointBlock is exist .. this excute all case, except first do
					int thisBlockLoc = 0;
					
					//find blockNode's location mathing this data's hasingValue
					for(block_iter = blockNode.begin(); block_iter != blockNode.end(); block_iter++) {
						if(hashNode[thisHashLoc].pointBlockNum == blockNode[thisBlockLoc].thisBlockNum) {
							break;
						}
						thisBlockLoc ++;
					}
					
					
					//overflow is exist or not .. input this data in this block
					if(blockNode[thisBlockLoc].studentData.size() <= 128 ) { // if blockData's size is smaller 128
						blockNode[thisBlockLoc].studentData.push_back(studentsData[howManyInput]);
					} else { // if overflow is exist
						//bool is_overflow = true;
						
						//calculate hash's pointer number about this node
						vector <int> useHashNode;
						int hashPointerNum = 0;
						for(int i = 0; i < hashNode.size(); i++) {
							if(hashNode[i].pointBlockNum == blockNode[thisBlockLoc].thisBlockNum) {
								useHashNode.push_back(hashPointerNum);
							hashPointerNum ++;
							}
						}
							
						BlockData tmp_Allo_blockData;
						
						//according to hash's pointer number, calculate change
						if(hashPointerNum == 1) { // if hash's pointer number = 1, this hashtable is double.
							//cout << "pointerNum 1" << endl;
							useBinaryDigit ++;
							vector <HashData> tmp_hashNode = hashNode;
							hashNode.insert(hashNode.end(), tmp_hashNode.begin(), tmp_hashNode.end());
							/*int j = 0;
							
							for(hash_iter = hashNode.begin(); hash_iter != hashNode.end(); hash_iter++) {
								cout << hashNode[j].pointBlockNum << endl;
								j++;
							}
							cout << endl;*/
							BlockData tmp2_blockData;
							tmp_Allo_blockData.thisBlockNum = endBlockLocate;
							blockNode.push_back(tmp_Allo_blockData);
							vector <StudentData>::iterator studData_iter;
							
							int thisBloData_loc = 0;
							//vector <StudentData> newBlockData;
							
							for(studData_iter = blockNode[thisBlockLoc].studentData.begin(); studData_iter != blockNode[thisBlockLoc].studentData.end();) {
								
								string reHasingValue = hashFunction(blockNode[thisBlockLoc].studentData[thisBloData_loc].studentID);
								
								string RehashLocBi;
								if(useBinaryDigit == 1) {
									RehashLocBi = "1";
								} else {
									
								RehashLocBi = "1" + hashLocBi;
								
								}
								
								if(reHasingValue.substr(reHasingValue.length() - useBinaryDigit, useBinaryDigit) == RehashLocBi) {
									blockNode[endBlockLocate - 1].studentData.push_back(blockNode[thisBlockLoc].studentData[thisBloData_loc]);
									
									//cout << reHasingValue.substr(reHasingValue.length() - useBinaryDigit, useBinaryDigit) << endl;
									studData_iter = blockNode[thisBlockLoc].studentData.erase(studData_iter);
								} else {
									studData_iter ++;
								}
								
								thisBloData_loc++;									
							}
							
							
							int newHashLoc = 1;
							for(int i = 1; i < useBinaryDigit; i ++) {
								newHashLoc *= 2;
							}
							hashNode[thisHashLoc + newHashLoc].pointBlockNum = endBlockLocate;
							endBlockLocate ++;
							
						} else { // if hash's pointer number is not 1, this hashTable use.
								//and hash's pointer number is 2, 4, 8, ....
							
							/*¹Ì±¸Çö*/
							 
							string RehasingValue = hashFunction(studentsData[howManyInput].studentID);
						
							tmp_Allo_blockData.thisBlockNum = endBlockLocate;
							//blockNode.push_back(tmp_Allo_blockData);
							//cout << "pointerNum not 1" << endl;
							
					 	}
								
							
							
						
						/*vector<int>::iterator it;
    					it=find(vi.begin(),vi.end(),55);
     					cout << *it << endl;
     					vi.erase(it+1);
     					cout << *it << endl;*/
     					
						/*vector <HashData> tmp_hashNode = hashNode;
						hashNode.insert(hashNode.end(), tmp_hashNode.begin(), tmp_hashNode.end());*/
					}
				}
				
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
