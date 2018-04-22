#ifndef ITERATED_H
#define ITERATED_H

template <typename T>
class Iterated {
public:
    virtual T cbegin() const = 0;
    virtual T cend() const = 0;
    virtual ~Iterated() {}
};

#endif // ITERATED_H
