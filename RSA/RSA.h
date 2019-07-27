#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/random.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
namespace bm = boost::multiprecision;
struct KEY
{
	bm::int1024_t ekey;//Ë½Ô¿	
	bm::int1024_t dkey;//¹«Ô¿
	bm::int1024_t pkey;
};

class RSA{
public:
	RSA();
	KEY get_keys();

	void encrypt(const char* file_in, const char* file_password, bm::int1024_t ekey, bm::int1024_t pkey);
	void decode(const char* file_password, const char* file_out, bm::int1024_t pkey, bm::int1024_t dkey);

	std::vector<bm::int1024_t> encrypt(bm::int1024_t ekey, bm::int1024_t pkey, std::string& str_in);
	std::string decode(bm::int1024_t dkey, bm::int1024_t pkey, std::vector<bm::int1024_t> vec_out);
	void printInfo(std::vector<bm::int1024_t> str_in);
private:
	bm::int1024_t produce_prime();
	bool is_prime(bm::int1024_t prime);
	bm::int1024_t produce_pkey(bm::int1024_t p,bm::int1024_t q);
	bm::int1024_t produce_orla(bm::int1024_t p, bm::int1024_t q);
	bm::int1024_t produce_ekey(bm::int1024_t orla);
	bm::int1024_t produce_gcd(bm::int1024_t ekey, bm::int1024_t orla);
	bm::int1024_t exgcd(bm::int1024_t ekey, bm::int1024_t orla,bm::int1024_t& x,bm::int1024_t& y);
	bm::int1024_t produce_dkey(bm::int1024_t ekey,bm::int1024_t orla);

	bm::int1024_t encrypt(bm::int1024_t ekey, bm::int1024_t pkey, bm::int1024_t msg);
	void produce_keys();

private:
	KEY _key;
};
