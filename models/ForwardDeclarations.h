#ifndef FORWARDDECLARATIONS_H
#define FORWARDDECLARATIONS_H

namespace gm
{
#define EACH_VERTEX_SIZE 5
#define PI 3.1415926

template<typename T> struct traits;
template<typename T> struct traits<const T> : traits<T> {};

enum MODEL_TYPE {
    MODEL_CUBE,
    MODEL_CYLINDER,
    MODEL_SPHERE,
};

} // namespace gm

#endif