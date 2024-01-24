#include "pch.h"
#include "Deligate.h"




void TestDeligate()
{
	Deligate1<int> deligate;

	std::function<void(int)> func[4];
	func[0] = [](int a) {std::cout << "Hello1" << std::endl; };
	func[1] = [](int a) {std::cout << "Hello2" << std::endl; };
	func[2] = [](int a) {std::cout << "Hello3" << std::endl; };
	func[3] = [](int a) {std::cout << "Hello4" << std::endl; };

	size_t hash[4];
	hash[0] = func[0].target_type().hash_code();
	hash[1] = func[1].target_type().hash_code();
	hash[2] = func[2].target_type().hash_code();
	hash[3] = func[3].target_type().hash_code();

	deligate += func[0];
	deligate += func[1];
	deligate += func[2];
	deligate += func[3];
	deligate += std::bind([]() {std::cout << "bind" << std::endl; });
	std::cout << "Invoke" << std::endl;
	deligate.Invoke(30);
	
	deligate -= func[3];
	deligate -= std::bind([]() {std::cout << "bind" << std::endl; });
	std::cout << "Invoke" << std::endl;
	deligate.Invoke(332);
}