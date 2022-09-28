#ifndef FORWARDDECLARATIONS_H
#define FORWARDDECLARATIONS_H

namespace GraphicModel
{
template<typename T> struct traits;
template<typename T> struct traits<const T> : traits<T> {};  
} // namespace GraphicModel

#endif