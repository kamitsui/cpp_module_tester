#ifndef CUSTOM_ALLOCATOR_HPP
#define CUSTOM_ALLOCATOR_HPP

#include <cstddef> // For size_t
#include <cstdlib> // For malloc, free
#include <new>     // For std::bad_alloc

/**
 * @namespace AllocationController
 * @brief Controls the behavior of the custom global memory allocator for testing.
 */
namespace AllocationController {
// A countdown to failure. When this hits zero, new will fail.
// A negative value means it will never fail.
static int failIn = -1;

/**
 * @brief Sets the custom allocator to fail after a specific number of allocations.
 * @param count The number of successful allocations before failure.
 * For example, a count of 0 means the very next allocation will fail.
 * A negative count disables failure.
 */
inline void setFailAfter(int count) { failIn = count; }

/**
 * @brief Resets the allocator to its default (non-failing) state.
 */
inline void reset() { failIn = -1; }
} // namespace AllocationController

/**
 * @brief Overridden global operator new to simulate allocation failure.
 * @note The deprecated 'throw(std::bad_alloc)' specifier has been removed
 * to comply with C++11 and later standards.
 */
// void *operator new(std::size_t size) throw(std::bad_alloc) { // C++11で非推奨（deprecated）
void *operator new(std::size_t size) {
    if (AllocationController::failIn == 0) {
        // std::cout << "throw bad_alloc() --- custom new failIn[" << AllocationController::failIn << "]"
        //<< std::endl;                  // debug
        AllocationController::reset(); // Reset after failure to avoid infinite loops in exception handling
        throw std::bad_alloc();
    }

    if (AllocationController::failIn > 0) {
        AllocationController::failIn--;
    }

    void *p = std::malloc(size);
    if (!p) {
        throw std::bad_alloc();
    }
    // std::cout << "alloc success --- custom new failIn[" << AllocationController::failIn << "]" << std::endl; // debug
    return p;
}

/**
 * @brief Overridden global operator delete.
 * @note 'throw()' has been replaced with 'noexcept' for modern C++ compatibility.
 */
void operator delete(void *p) noexcept { std::free(p); }

/**
 * @brief Overridden sized-deallocation operator delete.
 * @note This is required as of C++14 when providing a custom operator new.
 */
void operator delete(void *p, std::size_t size) noexcept {
    (void)size; // The size parameter is unused in this simple implementation.
    std::free(p);
}

#endif // CUSTOM_ALLOCATOR_HPP
