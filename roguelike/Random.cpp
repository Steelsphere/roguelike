#include "Random.h"

std::random_device Random::device;

std::mt19937_64 Random::generator(device());

std::uniform_int_distribution<int> Random::random(0, 2147483647);

std::uniform_int_distribution<int> Random::one_to_four(1, 4);

std::uniform_int_distribution<int> Random::big_number(0, 500000);

std::uniform_int_distribution<int> Random::one_to_sixteen(1, 16);

std::uniform_int_distribution<int> Random::coinflip(1, 2);

std::uniform_int_distribution<int> Random::one_to_thirty_two(1, 32);

std::uniform_int_distribution<int> Random::one_to_sixty_four(1, 64);

std::uniform_int_distribution<int> Random::one_to_one_twenty_eight(1, 128);

std::uniform_int_distribution<int> Random::one_to_two_fifty_six(1, 256);

std::uniform_int_distribution<int> Random::one_to_eight(1, 8);

int Random::randc(int min, int max) {
	std::uniform_int_distribution<int> r(min, max);
	return r(Random::generator);
}

std::string Random::rand_string(int size) {
	std::string str = "";
	for (int i = 0; i < size; i++) {
		char rnd = static_cast<char>(randc(65, 90));
		str += rnd;
	}
	return str;
}
