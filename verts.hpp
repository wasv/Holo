#ifndef MY_HEADER_HPP
#define MY_HEADER_HPP

#include <stdio.h>
#include <iostream>

template<typename T, unsigned int N>
struct Vert {
  public:
    T &operator[](std::size_t idx) { return data[idx]; }
    T data[N];
};

template<>
struct Vert<float, 3> {
  public:
    float &operator[](std::size_t idx) { return data[idx]; }
    float data[3] = {0.0f, 0.0f, 0.0f};
};

template<>
struct Vert<float, 6> {
  public:
    float &operator[](std::size_t idx) { return data[idx]; }
    float data[6] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
};

template<typename T, unsigned int N>
std::ostream &operator<< (std::ostream &os, const Vert<T,N> &m) {
  for (int i = 0; i < N-1; ++i) {
    os << m.data[i] << ' ';
  }
  os << m.data[N-1];
  return os;
}

template<typename T, unsigned int N>
void maximum(std::vector<Vert<T,N>> list, Vert<T,N> &max) {
  max = list[0];
  for (int i = 0; i < N-1; ++i) {
    for(int j = 0; j < list.size(); ++j) {
      if(list[j][i] > max[i]) {
        max[i] = list[j][i];
      }
    }
  }
}

template<typename T, unsigned int N>
void minimum(std::vector<Vert<T,N>> list, Vert<T,N> &min) {
  min = list[0];
  for (int i = 0; i < N-1; ++i) {
    for(int j = 0; j < list.size(); ++j) {
      if(list[j][i] < min[i]) {
        min[i] = list[j][i];
      }
    }
  }
}

#endif
