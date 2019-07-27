#include "RSA.h"
#include <time.h>
#include <math.h>
#include <fstream>

RSA::RSA() 
{
	produce_keys();
}

KEY RSA::get_keys(){
	return _key;
}


void RSA::encrypt(const char* file_in, const char* file_password, int ekey, int pkey)
{
	std::ifstream fin(file_in);
	std::ofstream fout(file_password, std::ofstream::app);
	if (!fin.is_open()){
		std::cout << "open file failed" << std::endl;
		return;
	}
	const int NUM = 256;
	char buffer[NUM];
	int buffer_password[NUM];
	int curnum;
	while (!fin.eof()){
		fin.read(buffer, NUM);
		curnum = fin.gcount();
		for (int i = 0; i < curnum; i++){
			buffer_password[i] = encrypt(ekey, pkey, (int)buffer[i]);
		}
		fout.write((char*)buffer_password, curnum*sizeof(int));
	}
	fin.close();
	fout.close();
}

void RSA::decode(const char* file_password, const char* file_out, int dkey,int pkey )
{
	std::ifstream fin(file_password);
	std::ofstream fout(file_out,std::ofstream::app);
	if (!fin.is_open()){
		std::cout << "open file failed" << std::endl;
		return;
	}
	const int NUM = 256;
	int buffer[NUM];
	char buffer_out[NUM];
	int curnum;
	while (!fin.eof()){
		fin.read((char*)buffer, NUM * sizeof(int));
		curnum = fin.gcount();
		curnum /= sizeof(int);
		for (int i = 0; i < curnum; i++){
			buffer_out[i] = (char)encrypt(dkey, pkey,buffer[i]);
		}
		fout.write(buffer_out, curnum);
	}
	fin.close();
	fout.close();
}

std::vector<int> RSA::encrypt(int ekey, int pkey, std::string& str_in)
{
	std::vector<int> vec;
	for (const auto& e : str_in){
		vec.push_back(encrypt(ekey, pkey, e));
	}
	return vec;
}

std::string RSA::decode(int dkey, int pkey, std::vector<int> vec_out) {
	std::string str;
	for (const auto& e : vec_out){
		str.push_back((char)encrypt(dkey, pkey, e));
	}
	return str;
}

void RSA::printInfo(std::vector<int> str_in)
{
	for (const auto& e : str_in){
		std::cout << e << " ";
	}
	std::cout << std::endl;
}

//模幂运算
int RSA::encrypt(int key, int pkey, int msg) {
	int msg_out = 1;
	while (key){
		if (key & 1){
			msg_out = (msg_out*msg) % pkey;
		}
		key >>= 1;
		msg = (msg*msg) % pkey;
	}
	return msg_out;
}

void RSA::produce_keys()
{
	int p = produce_prime();
	int q = produce_prime();
	while (p == q)
		q = produce_prime();
	std::cout << p << std::endl;
	std::cout << q << std::endl;
	_key.pkey = produce_pkey(p, q);
	int num_orla = produce_orla(p, q);
	_key.ekey =  produce_ekey(num_orla);
	_key.dkey = produce_dkey(_key.ekey, num_orla);
}

bool RSA::is_prime(int prime)
{
	if (prime < 2){
		return false;
	}
	for (int i = 2; i <= sqrt(prime); i++){
		if (prime%i == 0){
			return false;
		}
	}
	return true;
}

int RSA::produce_prime()
{
	srand(time(nullptr));
	int num ;
	while(1){
		num = rand() % 50 + 2;
		if (is_prime(num)){
			break;
		}
	}
	return num;
}

int RSA::produce_orla(int p, int q)
{
	return (p-1)*(q-1);
}

int RSA::produce_pkey(int p,int q)
{
	return p*q;
}

int RSA::produce_ekey(int orla)//1<e<orla,e 与orla互质
{
	int num;
	srand(time(nullptr));
	while (1){
		num = rand() % orla;
		if (num > 1 && produce_gcd(num, orla) == 1){
			break;
		}
	}
	return num;
}

int RSA::produce_gcd(int ekey, int orla)
{
	int residual;
	while (residual = ekey%orla){
		ekey = orla;
		orla = residual;
	}
	return orla;
}

int RSA::produce_dkey(int ekey, int orla)
{

	int dkey=orla/ekey;
	while (1)
	{
		if (( dkey * ekey ) % orla == 1&& dkey!=ekey){
			break;
		}
		++dkey;
	}
	return dkey;
}