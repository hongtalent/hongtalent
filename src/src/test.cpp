#include<bits/stdc++.h>


using namespace std;

int main(int argc, char**argv){
	if(argc != 3){
		cout << "ERROR" << endl;
		return 0;
	}
	
	ifstream in1(argv[1]), in2(argv[2]);
	if(in1.is_open() == false || in2.is_open() == false){
		cout << "FILE NOT EXISTS" << endl;
		return 0;
	}

	string ans1, ans2;
	int line = 1;
	bool flag = true;
	while(in1 >> ans1 && in2 >> ans2){
		in2 >> ans2;
		in2 >> ans2;
		in2 >> ans2;
		in2 >> ans2;
		in2 >> ans2;	
		if(ans1 != ans2){
			cout << "Wrong answer, LINE: " << line << endl;
			cout << "file: " << argv[1] << "\tans1: " << ans1 << endl;
			cout << "file: " << argv[2] << "\tans2: " << ans2 << endl;
			flag = false;
			cout << endl;
		}
		++line;
	}
	if(flag == false){
		return 0;
	}
	if(in1 >> ans1){
		cout << "Output Limit Exceed" << endl;
		return 0;
	}

	if(in2 >> ans2){
		cout << "Output Limit Exceed2" << endl;
		return 0;		
	}

	cout << "Accept" << endl;
	cout << "Check Point: " << line << endl;

	in1.close();
	in2.close();	


	return 0;
}