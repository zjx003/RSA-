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


void RSA::encrypt(const char* file_in, const char* file_password, bm::int1024_t ekey, bm::int1024_t pkey)
{
	std::ifstream fin(file_in);
	std::ofstream fout(file_password, std::ofstream::app);
	if (!fin.is_open()){
		std::cout << "open file failed" << std::endl;
		return;
	}
	const int NUM = 256;
	char buffer[NUM];
	bm::int1024_t buffer_password[NUM];
	int curnum=0;
	while (!fin.eof()){
		fin.read(buffer, NUM);
		curnum = fin.gcount();
		for (int i = 0; i < curnum; i++){
			buffer_password[i] = encrypt(ekey, pkey, (bm::int1024_t)buffer[i]);
		}
		fout.write((char*)buffer_password, curnum*sizeof(bm::int1024_t));
	}
	fin.close();
	fout.close();
}

void RSA::decode(const char* file_password, const char* file_out, bm::int1024_t dkey,bm::int1024_t pkey )
{
	std::ifstream fin(file_password);
	std::ofstream fout(file_out, std::ofstream::app);
	if (!fin.is_open()){
		std::cout << "open file failed" << std::endl;
		return;
	}
	const int NUM = 256;
	bm::int1024_t buffer[NUM];
	char buffer_out[NUM];
	int curnum=0;
	while (!fin.eof()){
		fin.read((char*)buffer, NUM * sizeof(bm::int1024_t));
		curnum = fin.gcount();
		curnum /= sizeof(bm::int1024_t);
		for (int i = 0; i < curnum; i++){
			buffer_out[i] = (char)encrypt(dkey, pkey, buffer[i]);
		}
		fout.write(buffer_out, curnum);
	}
	fin.close();
	fout.close();
}

std::vector<bm::int1024_t> RSA::encrypt(bm::int1024_t ekey, bm::int1024_t pkey, std::string& str_in)
{
	std::vector<bm::int1024_t> vec;
	for (const auto& e : str_in){
		vec.push_back(encrypt(ekey, pkey, e));
	}
	return vec;
}

std::string RSA::decode(bm::int1024_t dkey, bm::int1024_t pkey, std::vector<bm::int1024_t> vec_out) {
	std::string str;
	for (const auto& e : vec_out){
		str.push_back((char)encrypt(dkey, pkey, e));
	}
	return str;
}

void RSA::printInfo(std::vector<bm::int1024_t> str_in)
{
	for (const auto& e : str_in){
		std::cout << e << " ";
	}
	std::cout << std::endl;
}

//模幂运算
bm::int1024_t RSA::encrypt(bm::int1024_t key, bm::int1024_t pkey, bm::int1024_t msg) {
	bm::int1024_t msg_out = 1;
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
	bm::int1024_t p = produce_prime();
	bm::int1024_t q = produce_prime();
	while (p == q)
		q = produce_prime();
	std::cout << p << std::endl;
	std::cout << q << std::endl;
	_key.pkey = produce_pkey(p, q);
	std::cout << "pkey"<<_key.pkey << std::endl;
	bm::int1024_t num_orla = produce_orla(p, q);
	std::cout << "orla" << num_orla << std::endl;
	_key.ekey =  produce_ekey(num_orla);
	std::cout << "ekey" << _key.ekey << std::endl;
	_key.dkey = produce_dkey(_key.ekey, num_orla);
	std::cout << "dkey" << _key.dkey << std::endl;
}

bool RSA::is_prime(bm::int1024_t prime)
{
	boost::random::mt11213b gen(time(nullptr));
	if (miller_rabin_test(prime, 25, gen)){
		if (miller_rabin_test((prime - 1) / 2, 25, gen)){
			return true;
		}
	}
	return false;
}

bm::int1024_t RSA::produce_prime()
{
	boost::random::mt19937 gen(time(nullptr));
	boost::random::uniform_int_distribution<bm::int1024_t> dist(2, bm::int1024_t(1) << 128);
	bm::int1024_t num=0;
	while(1){
		num = dist(gen);
		if (is_prime(num)){
			break;
		}
	}
	return num;
}

bm::int1024_t RSA::produce_orla(bm::int1024_t p, bm::int1024_t q)
{
	return (p-1)*(q-1);
}

bm::int1024_t RSA::produce_pkey(bm::int1024_t p,bm::int1024_t q)
{
	return p*q;
}

bm::int1024_t RSA::produce_ekey(bm::int1024_t orla)//1<e<orla,e 与orla互质
{
	bm::int1024_t num;
	srand(time(nullptr));
	while (1){
		num = rand() % orla;
		if (num > 1 && produce_gcd(num, orla) == 1){
			break;
		}
	}
	return num;
}

bm::int1024_t RSA::produce_gcd(bm::int1024_t ekey, bm::int1024_t orla)
{
	bm::int1024_t residual;
	while (residual = ekey%orla){
		ekey = orla;
		orla = residual;
	}
	return orla;
}

bm::int1024_t RSA::produce_dkey(bm::int1024_t ekey, bm::int1024_t orla)
{
	bm::int1024_t x, y;
	exgcd(ekey, orla, x, y);
	return (x%orla + orla) % orla;
}

bm::int1024_t RSA::exgcd(bm::int1024_t ekey, bm::int1024_t orla, bm::int1024_t& x, bm::int1024_t& y)
{
	if (orla == 0){
		x = 1;
		y = 0;
		return ekey;
	}
	bm::int1024_t ret = exgcd(orla, ekey%orla, x, y);
	bm::int1024_t x1 = x, y1 = y;
	x = y1;
	y = x1 - (ekey / orla)*y1;
	return ret;
}