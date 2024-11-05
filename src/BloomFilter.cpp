#include "BloomFilter.h"

// Constructor initializing the number of hash functions
template <std::size_t N>
BloomFilter<N>::BloomFilter(unsigned int num_hashes) : num_hashes(num_hashes), server(new CDNServer())
{
    std::random_device rd;
    srd::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> dist(0, std::numeric_limits<std::size_t>::max());

    seeds.reserve(num_hashes);
    for (std::size_t i i = 0; i < num_hashes; ++i)
    {
        seeds.push_back(dist(gen));
    }
}

// Copy constructor
template <std::size_t N>
BloomFilter<N>::BloomFilter(const BloomFilter &other)
    : num_hashes(other.num_hashes), bits(other.bits),
      seeds(other.seeds), server(other.server)
{
}

// Move constructor with noexcept specifier for optimal performance
template <std::size_t N>
BloomFilter<N>::BloomFilter(BloomFilter &&other) noexcept
    : num_hashes(other.num_hashes), bits(std::move(other.bits)), seeds(std::move(other.seeds)), server(other.server)
{
    other.server = nullptr;
}

// Destructor
template <std::size_t N>
BloomFilter<N>::~BloomFilter()
{
    delete server;
}

// Add an item to the Bloom filter
template <std::size_t N>
void BloomFilter<N>::add(const std::string &item)
{
    for (std::size_t i = 0; i < num_hashes; ++i)
    {
        size_t hasher = hash(item, seeds[i]);
        bits.set(hasher % N, true);
    }
    server->addWord(item);
}

// Overload for adding items from a file, where words are assumed to be separated by ", "
template <std::size_t N>
void BloomFilter<N>::add(std::string &&file_name = "../Resource/Word_DataSet_1.txt")
{
    std::ifstream file(file_name);
    std::string word;
    while (std::getline(file, word, ','))
    {
        add(word);
    }
}

template <std::size_t N>
bool BloomFilter<N>::possiblyContains(const std::string &item) const
{
    for (std::size_t i = 0; i < num_hashes; ++i)
    {
        auto hash_value = hash(item, seeds[i]);
        if (!bits.test(hash_value % N))
        {
            return false;
        }
    }
    return true;
}

template <std::size_t N>
bool BloomFilter<N>::possiblyContains(std::string &&item) const
{
    return possiblyContains(item);
}

template <std::size_t N>
bool BloomFilter<N>::certainlyContains(const std::string &item) const
{
    if (possiblyContains(item))
    {
        return server ? server->checkWord(item) : false;
    }
    return false;
}

template <std::size_t N>
bool BloomFilter<N>::certainlyContains(std::string &&item) const
{
    return certainlyContains(item);
}

template <std::size_t N>
void BloomFilter<N>::reset()
{
    bits.reset();
}

template <std::size_t N>
BloomFilter<N> &BloomFilter<N>::operator&(const BloomFilter &other)
{
    bits &= other.bits;
    return *this;
}

template <std::size_t N>
BloomFilter<N> &BloomFilter<N>::operator|(const BloomFilter &other)
{
    bits |= other.bits;
    return *this;
}

template <std::size_t N>
bool BloomFilter<N>::operator()(const std::string &item) const
{
    return possiblyContains(item);
}

// Output stream operator
template <size_t N>
std::ostream &operator<<(std::ostream &os, const BloomFilter<N> &bloom_filter)
{
    os << bloom_filter.bits<<std::endl;
    os << "Num hasher: " << bloom_filter.num_hashes << std::endl;
    os << " the number of times the server has been queried "<<bloom_filter.server->getUsageCount()<<std::endl;
    os << "memory usage of the server's data: "<<bloom_filter.server->RAMUsage()<<std::endl;
    return os;
}

// Input stream operator
template <size_t N>
std::istream &operator>>(std::istream &is, BloomFilter<N> &bloom_filter)
{
    std::string line;
    is >> bloom_filter.bits;
    is >> line;
    bloom_filter.num_hashes = std::stoi(line);
    return is;
}