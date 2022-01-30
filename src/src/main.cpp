#include<iostream>
#include<string>
#include<fstream>
#include"ipmatch.h"



int main(int argc, char** argv){
//调试
#ifndef DEBUG
using namespace std;
	IPRULE rule;

	if(argc != 3){
		cout << "ERROR" << endl;
		return 0;
	}

	const string rulefile = argv[1];
	const string queryfile = argv[2];
    //定义一个IPMatch的类
	IPMatch solve;

	ifstream in(rulefile);
	ifstream query(queryfile);
	ofstream ans("ans.txt");


	if(in.is_open() == false || query.is_open() == false){
		cout << "FILE NOT EXISTS" << endl;
		return 0;
	}else{

		IPIMP ip;
		while(query >> ip.s_ip >> ip.t_ip >> ip.s_point >> ip.t_point >> ip.proto){
			solve.Insert_Query(ip);
		}
		cout << "Insert Compile" << std::endl;
/***************************************************  solve *****************************************************/
		char ops;
		int level = 0;
		int line = 0;
		while(in >> ops >> rule.s_ip >> rule.t_ip >> rule.spoint_left >> ops >> rule.spoint_right 
			>> rule.tpoint_left >> ops >> rule.tpoint_right
			>> rule.proto){
			rule.level = level++;

			solve.Search(rule, line++);
		}

		cout << "Done" << endl;
		solve.getAnswer(ans);

	}	




	query.close();
	in.close();
	ans.close();
#else


#endif



	return 0;
}