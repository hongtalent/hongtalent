#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#ifndef IPMATCH_H
#define IPMATCH_H

typedef unsigned long long ull;

struct _node{
	int proto;
	int s_port;
	int t_port;
	int index;
};

typedef struct _UpdateTrieNode{
	_UpdateTrieNode* left;
	_UpdateTrieNode* right;

	std::vector<_node> res;
}UpdateTrieNode;

//匹配规则
typedef struct _IPRULE{
	//源IP地址
	std::string s_ip;
	//目的IP地址
	std::string t_ip;
	//源端口号范围
	int spoint_left;
	int spoint_right;
	//目的端口号范围
	int tpoint_left;
	int tpoint_right;
	//协议范围
	std::string proto;
	//优先级
	int level;
}IPRULE;

//查找路由
typedef struct _IPIMP{
	ull s_ip;
	ull t_ip;
	//协议号
	int proto;
	//源端口号
	int s_point;
	//目的端口号
	int t_point;
}IPIMP;


typedef struct _RuleNode{
	//源端口号
	int sleft, sright;
	//目的端口号
	int pleft, pright;
	//协议号
	int level;
	int proleft, proright;
}RuleNode;

typedef struct _TrieNode{
	_TrieNode* left;
	_TrieNode* right;
	bool flag;
	std::vector<RuleNode> rule;
}TrieNode;


class IPMatch{
private:
	TrieNode* root;

	UpdateTrieNode* UpdateRoot;
	UpdateTrieNode* newUpdateTrieNode();
	TrieNode* newTrieNode();
	//int转化为二进制字符串
	std::string INT_TO_BIN(ull num);
	//十六进制转化为二进制字符串
	std::string HEX_TO_BIN(std::string num);
	//ip转换成32位int
	ull IPSTR_TO_INT(std::string ip);
	//整形16进制转换成int
	ull HEXSTR_TO_INT(std::string num);
	//整形字符串转换int
	ull INTSTR_TO_INT(std::string num);

	std::vector<int> vec;

	void Find1(UpdateTrieNode* node, IPRULE rule, int line, ull bin1, int yms1, ull bin2, int yms2);
	void Find2(UpdateTrieNode* node, IPRULE rule, int line, ull bin2, int yms2);

public:
	IPMatch();

	void Insert_Rule(IPRULE rule) = delete;
	void Insert_Query(IPIMP imp);
	void Search(IPRULE rule, int line);
	void getAnswer(std::ofstream& oss){
		std::cout << vec.size() << std::endl;
		for(const auto& v : vec){
			if(v == 0x3f3f3f3f){
				oss << -1 << std::endl;
			}else
				oss << v << std::endl;
		}
	}



	void debug(){

		using std::cout;
		using std::endl;
		// cout << INT_TO_BIN(12) << endl;
		// cout << HEX_TO_BIN("FF");
		// IPRULE rule;
		// rule.s_ip = "16.28.115.124/32";
		// rule.t_ip = "239.224.253.93/32";
		// rule.spoint_left = 0;
		// rule.spoint_right = 65523;
		// rule.tpoint_left = 1717;
		// rule.tpoint_right = 12332;
		// rule.proto = "0x06/0xFF";
		// rule.level = 1;
		// Insert_Rule(rule);
	}

};



#endif
