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

#define MM 1			// �� ���� �����Ű�� B+ Ʈ���� ������ �����Ҽ� �ִ�.
#define M (MM * 2 + 1)  // Ȧ�� ���� ���� 
#define TR 500 // ������ ũ��

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


typedef struct Node
{
	int count;		// ��忡 ����� Key�� ��, �ε��� ���� ������� ���� �÷���	
	int Key[M-1];			 // Key 
	struct Node* branch[M];  // �ּ� 
	StudentData data;
} node;

class Bptree{
	public:
		StudentData* insertItem(StudentData* k);		// Key ���� �Լ�
		void sequencialSearch(int k);
	private:
		node* root;// root�� ����Ű�� ���
};

class StudentsFileStruct {
	private:
		int studentNum;
		StudentData *studentsData;
		vector <HashData> hashNode;
		vector <HashData>::iterator hash_iter;
		vector <BlockData> blockNode;
		vector <BlockData>::iterator block_iter;
		Bptree studentTree;
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
				studentTree.insertItem(studentsData+dataLocate);
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
							
							/*�̱���*/
							 
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
			studentTree.sequencialSearch(0);
		}
	
	
	
};


StudentData* Bptree::insertItem(StudentData* k)
{
	
	node* trace[TR];	// ���Ե� ��θ� ������ ���ÿ뵵�� �迭
	int dir[TR];
	int Key, i;				

	node* upRight, *p;
	StudentData* insertFileLocation;
	insertFileLocation = k;
	upRight = (node*)insertFileLocation;
	i = 0;	// trace[]�� index

	p = root;	// p�� ������ ���Ե� ��ġ�� Ž��
	
	upRight = (node*)k;
	

			
	if (root == NULL)
	{
		root = new node();
		root->branch[0] = NULL;
		root->Key[0] = k->studentID;
		root->branch[1] = (node*)insertFileLocation;
		root->count = M + 1;
		return insertFileLocation;
	}

	while (true)	// p�� leaf��� �϶����� Ž��
	{
		int j;
		trace[i] = p;
		for (j=0; j<p->count%M; j++)	// �� ��忡�� ��θ� ����
			if (p->Key[j] >= k->studentID)
			{
				dir[i] = j;
				break;
			}
		if (j == p->count%M)
			dir[i] = p->count%M;
		if (p->count/M == 1)
			break;
		p = p->branch[j];
		i++;
	}						// �� �������� ������ p�� Key���� ���ϵ� ���. 

	
	// ���� �������� ������ ����. 
	Key = k->studentID;
	while (i != -1)
	{
		int path = dir[i];
		p = trace[i];
		if (p->count%M != M-1)	// �����ص� overflow�� ������ ������
		{
			int m;
			for (m=p->count%M; m>path; m--)	// ���Ե� ĭ���� ������ ��ĭ�� �ڷ�. 
			{
				p->Key[m] = p->Key[m-1];
				p->branch[m+1] = p->branch[m];
			}
			p->Key[path] = Key;		// Key���� ����
			p->branch[path+1] = upRight;	// branch�� ����. 
			p->count++;
			break;
		}
	
		else	// �����ϸ� overflow�� ����� ���
		{
			int nodeKey[M];
			node* nodeBranch[M+1];
			node* newNode;
			int j, j2;
			newNode = (node*)malloc(sizeof(node));
			
			nodeBranch[0] = p->branch[0];
			for (j=0, j2=0; j<M; j++, j2++)		// �ӽ÷� ũ�� M+1�� ��忡 ������� ����. 
			{
				if (j == path)
				{
					nodeKey[j] = Key;
					nodeBranch[j+1] = upRight;
					j++;
					if (j>=M) 
						break;
				}
				nodeKey[j] = p->Key[j2];
				nodeBranch[j+1] = p->branch[j2+1];
			}
			for (j=0; j<M/2; j++)
			{
				p->Key[j] = nodeKey[j];
				p->branch[j+1] = nodeBranch[j+1];
			}
			newNode->branch[0] = nodeBranch[M/2+1];
			for (j=0; j<M/2; j++)	// ��� Key �������ʹ� ���λ��� ��忡 �����Ѵ�. 
			{
				newNode->Key[j] = nodeKey[M/2+1+j];
				newNode->branch[j+1] = nodeBranch[M/2+2+j];
			}

			// ���࿡ p�� ��������̸� �ణ�� ����
			if (p->count/M == 1)
			{
				newNode->branch[0] = p->branch[0];	// sequencial pointer ����
				p->branch[0] = newNode;
				p->Key[M/2] = nodeKey[M/2];		// �ø� Key���� ������忡�� ����. 
				p->branch[M/2+1] = nodeBranch[M/2+1];
				p->count = M + M/2 + 1;
				newNode->count = M + M/2;
			}
			else
			{
				p->count = newNode->count = M/2;
				p->branch[0] = nodeBranch[0];
			}

			Key = nodeKey[M/2];	// ��� Key�� �ø��� Key�� �Ѵ�. 
			upRight = newNode;	// ���� ���� node�� �ø��� ���� ������ �ڽ����� 
		}
		i--;
	}
	if (i == -1)	// root���� overflow�� ������ ���
	{
		root = (node*)malloc(sizeof(node));
		root->count = 1;
		root->branch[0] = trace[0];
		root->branch[1] = upRight;
		root->Key[0] = Key;
	}

	return insertFileLocation;
}


void Bptree::sequencialSearch(int k)
{
	int path, j;
	node* p = root;
	int findcnt = 0;

	if (p != NULL)
	{	
		while (true)	// p�� leaf��� �϶����� Ž��
		{
			int j;
			for (j=0; j<p->count%M; j++)	// �� ��忡�� ��θ� ����
			{	
				if (p->Key[j] >= k)
				{
					path = j;
					break;
				}
			}
			if (j == p->count%M)
				path = p->count%M;
			if (p->count/M == 1)
				break;
			p = p->branch[j];
		}				
		if (p->Key[path] == k || k == 0)	// k�� 0�̸� minimum���� �� ����Ѵ�. 
		{
			while (p != NULL)
			{
				for (j=0; j<p->count%M; j++)
				{
					cout << "(" << ((StudentData*)p->branch[j+1])->studentID << " , " << ((StudentData*)p->branch[j+1])-> score << ")" << endl;
				}
				p = p->branch[0];
			}
		}
	}
}


int main(int argc, char** argv) {
	StudentsFileStruct studentsFS;
	studentsFS.readStudentTable();
	studentsFS.calculate_DB_HashTable();
	studentsFS.kthNodePrint();
	return 0;
}
