#ifndef QUADRATIC_PROBING_H
#define QUADRATIC_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>


namespace {

// Internal method to test if a positive number is prime.
bool IsPrime(size_t n) {
  if( n == 2 || n == 3 )
    return true;
  
  if( n == 1 || n % 2 == 0 )
    return false;
  
  for( int i = 3; i * i <= n; i += 2 )
    if( n % i == 0 )
      return false;
  
  return true;
}


// Internal method to return a prime number at least as large as n.
int NextPrime(size_t n) {
  if (n % 2 == 0)
    ++n;  
  while (!IsPrime(n)) n += 2;  
  return n;
}

}  // namespace


// Quadratic probing implementation.
template <typename HashedObj, typename HashedValue>
class HashTable {
 public:
  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit HashTable(size_t size = 101) : array_(NextPrime(size)), Collisions(0)
    { MakeEmpty(); }
  
  bool Contains(const HashedObj & x) const {
    return IsActive(FindPos(x));
  }
  
  void MakeEmpty() {
    current_size_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }

  HashedValue Find(const HashedObj & key) const
  {
    size_t current_pos = FindPos(key);
    if (IsActive(current_pos))
      return array_[current_pos].pointer_;
    else
    {
      HashedValue empty = nullptr; 
      return empty;
    }
  }

  void Modify_Ptr(const HashedObj & key, const HashedValue & newValue)
  {
    size_t current_pos = FindPos(key);
    array_[current_pos].pointer_ = newValue;
  }
  

  bool Insert(const HashedObj & x, const HashedValue & ptr ) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos].element_ = x;
    array_[current_pos].info_ = ACTIVE;
    array_[current_pos].pointer_ = ptr;
    
    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();    
    return true;
  }
    
  bool Insert(HashedObj && x, const HashedValue & ptr) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos].element_ = std::move(x);
    array_[current_pos].info_ = ACTIVE;
    array_[current_pos].pointer_ = ptr;



    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();

    return true;
  }

  bool Remove(const HashedObj & x) {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    return true;
  }
  int NumberOfElements()
  {
    return current_size_;
  }
  int SizeOfTable()
  {
    return array_.size();
  }
  int NumberOfCollisions()
  {
    return Collisions;
  }

 private:        
  struct HashEntry {
    HashedObj element_;
    HashedValue pointer_;
    EntryType info_;
    
   HashEntry( const HashedObj & e = HashedObj{ }, const HashedValue & p = HashedValue{ }, EntryType i = EMPTY )
        : element_{ e }, pointer_{ p }, info_{ i } { }
        
        HashEntry( HashedObj && e, HashedValue && p, EntryType i = EMPTY )
        : element_{ std::move( e ) }, pointer_{ std::move( p ) }, info_{ i } { }
  };
    

  std::vector<HashEntry> array_;
  size_t current_size_;
  mutable int Collisions; 

  bool IsActive(size_t current_pos) const
  { return array_[current_pos].info_ == ACTIVE; }

  size_t FindPos(const HashedObj & x) const {
    size_t offset = 1;
    size_t current_pos = InternalHash(x);
      
    while (array_[current_pos].info_ != EMPTY &&
	   array_[current_pos].element_ != x) {
      current_pos += offset;  // Compute ith probe.
      offset += 2;
      if (current_pos >= array_.size())
	current_pos -= array_.size();
  Collisions++;
    }
    return current_pos;
  }

  void Rehash() {
    std::vector<HashEntry> old_array = array_;

    // Create new double-sized, empty table.
    array_.resize(NextPrime(2 * old_array.size()));
    for (auto & entry : array_)
      entry.info_ = EMPTY;
    
    // Copy table over.
    current_size_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
	Insert(std::move(entry.element_), std::move(entry.pointer_));
  }
  
  size_t InternalHash(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return hf(x) % array_.size( );
  }
};

#endif  // QUADRATIC_PROBING_H
