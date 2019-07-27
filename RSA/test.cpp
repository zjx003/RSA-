#include "RSA.h"


void teststring()
{
	RSA rsa;
	KEY key = rsa.get_keys();
	std::string str_in;
	std::cin >> str_in;
	std::vector<bm::int1024_t> str_encrypt = rsa.encrypt(key.ekey, key.pkey, str_in);
	std::string str_out = rsa.decode(key.dkey, key.pkey, str_encrypt);
	rsa.printInfo(str_encrypt);
	std::cout << str_out << std::endl;
}

void testfile()
{
	RSA rsa;
	KEY key = rsa.get_keys();
	rsa.encrypt("plain.txt", "ecrept.out.txt", key.ekey, key.pkey);
	rsa.decode("ecrept.out.txt", "decode.txt", key.dkey, key.pkey);
}

int main()
{
	teststring();
	//testfile();
	system("pause");
	return 0;
}