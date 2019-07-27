#pragma once
#include <vector>
#include <iostream>
#include <string>
struct KEY
{
	int ekey;//Ë½Ô¿	
	int dkey;//¹«Ô¿
	int pkey;
};

class RSA{
public:
	RSA();
	KEY get_keys();

	void encrypt(const char* file_in, const char* file_password, int ekey, int pkey);
	void decode(const char* file_password, const char* file_out, int pkey, int dkey);

	std::vector<int> encrypt(int ekey, int pkey, std::string& str_in);
	std::string decode(int dkey, int pkey, std::vector<int> vec_out);
	void printInfo(std::vector<int> str_in);
private:
	int produce_prime();
	bool is_prime(int prime);
	int produce_pkey(int p,int q);
	int produce_orla(int p, int q);
	int produce_ekey(int orla);
	int produce_gcd(int ekey, int orla);
	int produce_dkey(int ekey,int orla);

	int encrypt(int ekey, int pkey, int msg);
	void produce_keys();

private:
	KEY _key;
};
