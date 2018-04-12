#ifndef ITERATED_H
#define ITERATED_H


template <typename T>
class Iterated {
public:
    virtual T cbegin() const = 0;
    virtual T cend() const = 0;
    virtual T begin() const = 0;
    virtual T end() const = 0;
};

#endif // ITERATED_H
