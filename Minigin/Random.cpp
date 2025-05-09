#include "Random.h"
#include <random>

void Random::SetRandomSeet()
{
    std::srand(clock());
}

//int Random::GetRandom(int min, int max)
//{
//    if (max <= min) return min;
//    return min + std::rand() % (max - min + 1);
//}

size_t Random::GetRandom(size_t min, size_t max)
{
    if (max <= min) return min;
    return min + std::rand() % (max - min + 1);
}
