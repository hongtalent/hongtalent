#include"ipmatch.h"
#include<algorithm>
#include<sstream>

#define TO_IPBININT(r1, r2, r3, r4) (((r1) << 24) | ((r2) << 16) | ((r3) << 8) | r4) //将ip地址转换成整型
#define BININDEX(val, index) (val & (1 << index)) //获取到val的第index个二进制位

using std::string;
using std::vector;
using namespace std;
IPMatch::IPMatch(){
	this->root = newTrieNode();
	this->UpdateRoot = newUpdateTrieNode();
}

//整型数字转换二进制字符串
std::string IPMatch::INT_TO_BIN(ull num){
	if(num == 0)
		return "0";
	string ret;
	while(num){
		ret += num % 2 + '0';
		num /= 2;
	}
	reverse(ret.begin(), ret.end());
	return ret;
}


//16进制字符串转2进制字符串
std::string IPMatch::HEX_TO_BIN(std::string num){
	return this->INT_TO_BIN(HEXSTR_TO_INT(num));
}
//16进制字符串转整型
ull IPMatch::HEXSTR_TO_INT(std::string num){
	ull sum = 0;
	for(int i = 0; i < num.size(); i++){
		if(num[i] == 'x')
			continue;
		int ch = (num[i] <= '9' ? num[i] - '0' : (num[i] | 32) - 'a' + 10);
		sum = sum * 16 + ch;
	}
	return sum;
}
//ip地址转10进制数
ull IPMatch::IPSTR_TO_INT(std::string ip){

	return 0;
}
//10进制字符串转整型
ull IPMatch::INTSTR_TO_INT(std::string num){
	ull ret = 0;
	for(int i = 0; i < num.size(); i++){
		ret = ret * 10 + num[i] - '0';
	}
	return ret;
}



//新建TrieNode
TrieNode* IPMatch::newTrieNode(){
	TrieNode* node = new TrieNode;
	node->left = node->right = NULL;
	node->flag = false;
	node->rule.clear();
	return node;
}



//新建字典树节点
UpdateTrieNode* IPMatch::newUpdateTrieNode(){
	UpdateTrieNode* ret = new UpdateTrieNode;
	ret->left = ret->right = NULL;
	return ret;
}
//插入五元组
void IPMatch::Insert_Query(IPIMP imp){
	auto node = this->UpdateRoot;

	for(int i = 31; i >= 0; --i){
		if(BININDEX(imp.s_ip, i) == 0){
			if(node->left == NULL)
				node->left = new UpdateTrieNode();
			node = node->left;
		}else{
			if(node->right == NULL)
				node->right = new UpdateTrieNode();
			node = node->right;
		}
	}

	for(int i = 31; i >= 0; --i){
		if(BININDEX(imp.t_ip, i) == 0){
			if(node->left == NULL)
				node->left = new UpdateTrieNode();
			node = node->left;
		}else{
			if(node->right == NULL)
				node->right = new UpdateTrieNode();
			node = node->right;
		}
	}


	_node add;
	add.proto = imp.proto;
	add.s_port = imp.s_point;
	add.t_port = imp.t_point;
	add.index = vec.size();
	this->vec.push_back(0x3f3f3f3f);
	node->res.push_back(add);

	// cout << node->s_port << ' ' << node->t_port << "   " << node->proto << endl;
}
void IPMatch::Find1(UpdateTrieNode* node, IPRULE rule, int line, ull bin1, int yms1, ull bin2, int yms2){

	using namespace std;
	if(yms1){
		if(node->left)
			Find1(node->left, rule, line, bin1, yms1 - 1, bin2, yms2);
		if(node->right)
			Find1(node->right, rule, line, bin1, yms1 - 1, bin2, yms2);
	}else{
		int i = 0;
		for(i = 31; i >= 0 && yms2; --i, --yms2){
			if(BININDEX(bin2, i) == 0){
				if(node->left != NULL)
					node = node->left;
				else 
					return ;
			}else{
				if(node->right != NULL)
					node = node->right;
				else return;
			}
		}

		Find2(node, rule, line, bin2, i + 1);
	}

}
void IPMatch::Find2(UpdateTrieNode* node, IPRULE rule, int line, ull bin2, int yms2){
	using namespace std;
	if(yms2){
		if(node->left)
			Find2(node->left, rule, line, bin2, yms2 - 1);
		if(node->right)
			Find2(node->right, rule, line, bin2, yms2 - 1);
	}
	else{
		std::stringstream in;
		in << rule.proto;
		string hexproto;
		in >> hexproto;
		int proleft = HEXSTR_TO_INT(hexproto.substr(2, 2));
		int proright = HEXSTR_TO_INT(hexproto.substr(7, 2));

		for(const auto& v : node->res){
			if(rule.spoint_left > v.s_port || rule.spoint_right < v.s_port)
				continue;
			if(rule.tpoint_left > v.t_port || rule.tpoint_right < v.t_port)
				continue;
			if(proright == 0xff){
				if(proleft == v.proto){
					vec[v.index] = std::min(vec[v.index], line);
				}else{
					continue;
				}
			}else if(proright == 0x00){
				vec[v.index] = std::min(vec[v.index], line);
			}else{
				if(v.proto >= proleft && v.proto <= proright){
					vec[v.index] = std::min(vec[v.index], line);
				}
			}
		}

	}
}
//查找匹配rule的数据集
void IPMatch::Search(IPRULE rule, int line){
	char ops;
	ull r1, r2, r3, r4, yms;
	std::stringstream in;
	in << rule.s_ip << ' ' << rule.t_ip << ' '
		<< rule.proto << ' ';

	ull insertBin1 = 0;
	in >> r1 >> ops >> r2 >> ops >> r3 >> ops >> r4 >> ops >> yms;
	insertBin1 = TO_IPBININT(r1, r2, r3, r4);

	UpdateTrieNode* node = UpdateRoot;

	int i = 0;
	for(i = 31; i >= 0 && yms; --i, --yms){
		if(BININDEX(insertBin1, i) == 0){
			if(node->left != NULL){
				node = node->left;
			}
			else{
				return ;
			}
		}else{
			if(node->right != NULL){
				node = node->right;
			}
			else{
				return ;
			}
		}

	}


	in >> r1  >> ops >> r2 >> ops >> r3 >> ops >> r4 >> ops >> yms;
	ull insertBin2 = 0;
	insertBin2 = TO_IPBININT(r1, r2, r3, r4);

	Find1(node, rule, line, insertBin1, i + 1, insertBin2, yms);


}