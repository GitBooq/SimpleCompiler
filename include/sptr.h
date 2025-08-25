/*

*/
#ifndef SPTR_H_
#define SPTR_H_

#include <memory>

template <typename T>
using sptr = std::shared_ptr<T>;

#endif  // SPTR_H_