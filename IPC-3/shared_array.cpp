#include "shared_array.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdexcept>

shared_array::shared_array(const std::string& name, size_t element_count)
    : shm_name(name),
      length(element_count),
      shm_fd(-1),
      buffer(nullptr),
      semaphore(nullptr),
      is_owner(false)
{
    if (length < 1 || length > 1'000'000'000)
        throw std::invalid_argument("invalid array length");

    std::string shm_full_name = "/" + shm_name;
    std::string sem_full_name = "/" + shm_name + "_sem";

    shm_fd = shm_open(shm_full_name.c_str(), O_CREAT | O_EXCL | O_RDWR, 0666);
    if (shm_fd != -1)
    {
        is_owner = true;
        if (ftruncate(shm_fd, length * sizeof(int)) == -1)
        {
            shm_unlink(shm_full_name.c_str());
            throw std::runtime_error("ftruncate failed");
        }
    }
    else
    {
        shm_fd = shm_open(shm_full_name.c_str(), O_RDWR, 0666);
        if (shm_fd == -1)
            throw std::runtime_error("shm_open failed");

        struct stat st;
        if (fstat(shm_fd, &st) == -1)
            throw std::runtime_error("fstat failed");

        if (st.st_size != static_cast<off_t>(length * sizeof(int)))
            throw std::runtime_error("shared memory size mismatch");
    }

    buffer = static_cast<int*>(
        mmap(nullptr, length * sizeof(int),
             PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)
    );

    if (buffer == MAP_FAILED)
    {
        if (is_owner)
            shm_unlink(shm_full_name.c_str());
        throw std::runtime_error("mmap failed");
    }

    semaphore = sem_open(sem_full_name.c_str(), O_CREAT, 0666, 1);
    if (semaphore == SEM_FAILED)
        throw std::runtime_error("sem_open failed");
}

shared_array::shared_array(shared_array&& other) noexcept
    : shm_name(std::move(other.shm_name)),
      length(other.length),
      shm_fd(other.shm_fd),
      buffer(other.buffer),
      semaphore(other.semaphore),
      is_owner(other.is_owner)
{
    other.shm_fd = -1;
    other.buffer = nullptr;
    other.semaphore = nullptr;
    other.length = 0;
    other.is_owner = false;
}

shared_array::~shared_array()
{
    if (buffer)
        munmap(buffer, length * sizeof(int));
    if (shm_fd != -1)
        close(shm_fd);
    if (semaphore)
        sem_close(semaphore);
}

int& shared_array::operator[](size_t index)
{
    if (index >= length)
        throw std::out_of_range("index out of range");
    return buffer[index];
}

void shared_array::acquire()
{
    sem_wait(semaphore);
}

void shared_array::release()
{
    sem_post(semaphore);
}

size_t shared_array::size() const noexcept
{
    return length;
}

void shared_array::unlink(const std::string& name)
{
    shm_unlink(("/" + name).c_str());
    sem_unlink(("/" + name + "_sem").c_str());
}

