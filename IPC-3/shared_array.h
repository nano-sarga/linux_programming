#ifndef SHARED_ARRAY_H
#define SHARED_ARRAY_H

#include <string>
#include <semaphore.h>
#include <cstddef>

class shared_array
{
private:
    std::string shm_name;
    size_t      length;
    int         shm_fd;
    int*        buffer;
    sem_t*      semaphore;
    bool        is_owner;

public:
    shared_array(const std::string& shm_name, size_t element_count);

    shared_array(const shared_array&) = delete;
    shared_array& operator=(const shared_array&) = delete;

    shared_array(shared_array&& other) noexcept;

    ~shared_array();

    int& operator[](size_t index);

    void acquire();
    void release();

    size_t size() const noexcept;

    static void unlink(const std::string& shm_name);
};

#endif

