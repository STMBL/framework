#pragma once
#include <stdint.h>
#ifdef FP_USE_IOSTREAM
#include <iostream>
#endif

#ifndef M_PI
#define M_PI  3.1415926535
#endif

template<uint8_t b>
class fixed{
    public:
    static const uint8_t fbits = b;
    int32_t data;

    template<uint8_t bl>
    constexpr static fixed<b> sat(fixed<bl> l){
      fixed<b> r = fixed<b>(l);
      if(l > r.max()){
        r = r.max();
      }
      if(l < r.min()){
        r = r.min();
      }
      return(r);
    }

    // explicit cast fixed -> fixed
    template<uint8_t bl>
    explicit constexpr operator fixed<bl>() {
        fixed<bl> r;
        if(fbits > bl){ // TODO: check >> with negative numbers, change to / (1 << fbits)
            r.data = data >> (fbits - bl);
        }
        else{
            // TODO: auto sat for smaller target range
            r.data = data << (bl - fbits);
        }
        return(r);
    }

    // explicit cast fixed -> int32
    explicit constexpr operator int32_t() {
        return(data >> fbits);
    }

    #ifdef FP_USE_FLOAT
    // explicit cast fixed -> float
    explicit constexpr operator float() {
        return (static_cast<float>(data) / static_cast<float>(1 << fbits));
    }

    // explicit cast fixed -> double
    explicit constexpr operator double() {
        return (static_cast<double>(data) / static_cast<double>(1 << fbits));
    }
    #endif

    // return integral part of fixed
    constexpr fixed<b> integral() {
        fixed<b> r;
        if(data >= 0){
            r.data = data & (0xffffffff << b);
        }
        else{
            r.data = -data;
            r.data &= (0xffffffff << b);
            r.data = -r.data;
        }
        return(r);
    }

    // return fractional part of fixed
    constexpr fixed<b> fractional() {
        fixed<b> r;
        if(data >= 0){
            r.data = data & (0xffffffff >> (32 - b));
        }
        else{
            r.data = -data;
            r.data &= (0xffffffff >> (32 - b));
            r.data = -r.data;
        }
        return(r);
    }

    // constexpr fixed<b> inv() {
    //     fixed<32 - b> r = fixed<24>(1) / *(this);
    //     return(r);
    // }

    // pre increment
    constexpr fixed<b>& operator ++() {
        data += (1 << b);
        return(*this);
    }

    // pre decrement
    constexpr fixed<b>& operator --() {
        data -= (1 << b);
        return(*this);
    }

    // post increment
    constexpr fixed<b> operator ++(int) {
        fixed<b> r;
        r.data = data;
        ++(*this);
        return(r);
    }

    // post decrement
    constexpr fixed<b> operator --(int) {
        fixed<b> r;
        r.data = data;
        --(*this);
        return(r);
    }

    //smallest positive number of type
    constexpr static fixed<b> delta(){
      fixed<b> r;
      r.data = 1;
      return(r);
    }

    //largest number of type
    constexpr static fixed<b> max(){
      fixed<b> r;
      r.data = 0x7fffffff;
      return(r);
    }

    //smallest number of type
    constexpr static fixed<b> min(){
      fixed<b> r;
      r.data = 0x80000000;
      return(r);
    }

    // constructors
    fixed() = default;
    fixed(int v) : data (v * (1 << fbits)) {}
    fixed(unsigned int v) : data (v * (1 << fbits)) {}
    fixed(long int v) : data (v * (1 << fbits)) {}
    fixed(unsigned long int v) : data (v * (1 << fbits)) {} 
    fixed(float v) : data (v * (1 << fbits)) {}
    fixed(double v) : data (v * (1 << fbits)) {}
};


class _sat_{
  public:
    int64_t data;
    uint8_t fbits;

    _sat_(int64_t data, uint8_t fbits) {
      this->data = data;
      this->fbits = fbits;
    }

    template<uint8_t b>
    constexpr operator fixed<b>() {
      fixed<b> r;
        int64_t temp;

        if(fbits > b){ // TODO: check >> with negative numbers, change to / (1 << fbits)
            temp = data >> (fbits - b);
        }
        else{
            // TODO: auto sat for smaller target range
            temp = data << (b - fbits);
        }

        r.data = temp;
        if(r.data < temp){
          r = r.max();
        }
        if(r.data > temp){
          r = r.min();
        }
        return(r);
    }
};

// temporary type, result of a fixed calculation
class tfixed{
    public:
    int64_t data;
    uint8_t fbits;
    uint8_t ibits;

    // cast to fixed
    template<uint8_t b>
    constexpr operator fixed<b>() {
        fixed<b> r;
        if(fbits > b){ // TODO: check >> with negative numbers, change to / (1 << fbits)
            r.data = data >> (fbits - b);
        }
        else{
          // TODO: auto sat for smaller target range
            r.data = data << (b - fbits);
        }
        return(r);
    }

    auto sat(){
      _sat_ r = _sat_(data, fbits);
      return(r);
    }

    // explicit cast to int32
    explicit constexpr operator int32_t() {
        return(data >> fbits);
    }

    #ifdef FP_USE_FLOAT
    // explicit cast fixed -> float
    explicit constexpr operator float() {
        return (static_cast<float>(data) / static_cast<float>(1 << fbits));
    }

    // explicit cast fixed -> double
    explicit constexpr operator double() {
        return (static_cast<double>(data) / static_cast<double>(1 << fbits));
    }
    #endif
};

// -fixed
template<uint8_t bl>
constexpr fixed<bl> operator -(fixed<bl> right) {
    fixed<bl> r;
    r.data = -right.data;
    return(r);
}

// fixed * int32
template<uint8_t bl>
constexpr tfixed operator *(fixed<bl> left, int32_t right) {
    tfixed r;
    r.fbits = bl;
    r.data = left.data;
    r.data *= right;
    return(r);
}

// fixed / int32
template<uint8_t bl>
constexpr tfixed operator /(fixed<bl> left, int32_t right) {
    tfixed r;
    r.fbits = bl;
    r.data = left.data / right;
    return(r);
}

// fixed + int32
template<uint8_t bl>
constexpr tfixed operator +(fixed<bl> left, int32_t right) {
    tfixed r;
    r.fbits = bl;
    r.data = left.data;
    r.data += (right << bl);
    return(r);
}

// fixed - int32
template<uint8_t bl>
constexpr tfixed operator -(fixed<bl> left, int32_t right) {
    tfixed r;
    r.fbits = bl;
    r.data = left.data;
    r.data -= (right << bl);
    return(r);
}

// int32 * fixed
template<uint8_t br>
constexpr tfixed operator *(int32_t left, fixed<br> right) {
    tfixed r;
    r.fbits = br;
    r.data = left;
    r.data *= right.data;
    return(r);
}

// int32 / fixed
template<uint8_t br>
tfixed operator /(const int32_t left, const fixed<br> right){
    tfixed r;
    r.fbits = br;
    r = fixed<br>(left) / right;
    return(r);
}

// int32 + fixed
template<uint8_t br>
constexpr tfixed operator +(int32_t left, fixed<br> right) {
    tfixed r;
    r.fbits = br;
    r.data = (left << br);
    r.data += right.data;
    return(r);
}

// int32 - fixed
template<uint8_t br>
constexpr tfixed operator -(int32_t left, fixed<br> right) {
    tfixed r;
    r.fbits = br;
    r.data = (left << br);
    r.data -= right.data;
    return(r);
}


// fixed * fixed
template<uint8_t bl, uint8_t br>
constexpr tfixed operator *(fixed<bl> left, fixed<br> right) {
    tfixed r;
    r.fbits = left.fbits + right.fbits;
    r.ibits = 64 - r.fbits;
    r.data = (int64_t)left.data * (int64_t)right.data;
    return(r);
}

// fixed / fixed
#if defined FP_USE_64Bit_DIV
template<uint8_t bl, uint8_t br>
constexpr tfixed operator /(fixed<bl> left, fixed<br> right) {
    tfixed r;
    r.fbits = left.fbits;
    r.ibits = 64 - left.fbits;
    r.data = left.data / right.data;
    r.data <<= right.fbits;
    return(r);
}
#elif defined FP_USE_32Bit_DIV
template<uint8_t bl, uint8_t br>
constexpr tfixed operator /(fixed<bl> left, fixed<br> right) {
    tfixed r;
    r.fbits = left.fbits;
    r.ibits = 64 - left.fbits - right.fbits;
    r.data = left.data;
    r.data <<= right.fbits;
    r.data /= right.data;
    return(r);
}
#else
template<uint8_t bl, uint8_t br>
constexpr tfixed operator /(fixed<bl> left, fixed<br> right) {
    fixed<32 - right.fbits> inv;
    inv.data = ((int32_t)(1 << 30)) / (right.data >> 2);
    return(left * inv);
}
#endif

// fixed + fixed
template<uint8_t bl, uint8_t br>
constexpr tfixed operator +(fixed<bl> left, fixed<br> right) {
    tfixed r;
    tfixed l;
    r.data = right.data;
    l.data = left.data;

    if(left.fbits > right.fbits){
        r.fbits = left.fbits;
        l.fbits = left.fbits;
        r.ibits = 32 - right.fbits + 1;
        r.data <<= left.fbits - right.fbits;
    }
    else{
        r.fbits = right.fbits;
        l.fbits = right.fbits;
        r.ibits = 32 - left.fbits + 1;
        l.data <<= right.fbits - left.fbits;
    }

    r.data += l.data;
    return(r);
}

// fixed - fixed
template<uint8_t bl, uint8_t br>
constexpr tfixed operator -(fixed<bl> left, fixed<br> right) {
    tfixed r;
    tfixed l;
    r.data = right.data;
    l.data = left.data;

    if(left.fbits > right.fbits){
        r.fbits = left.fbits;
        l.fbits = left.fbits;
        l.ibits = 32 - right.fbits + 1;
        r.data <<= left.fbits - right.fbits;
    }
    else{
        r.fbits = right.fbits;
        l.fbits = right.fbits;
        l.ibits = 32 - left.fbits + 1;
        l.data <<= right.fbits - left.fbits;
    }

    l.data -= r.data;
    return(l);
}

// fixed *= fixed
template<uint8_t bl, uint8_t br>
fixed<bl>& operator *=(fixed<bl>& left, const fixed<br>& right){
    left = left * right;
    return(left);
}

// fixed *= int32
template<uint8_t bl>
fixed<bl>& operator *=(fixed<bl>& left, const int32_t& right){
    left = left * right;
    return(left);
}

// fixed /= fixed
template<uint8_t bl, uint8_t br>
fixed<bl>& operator /=(fixed<bl>& left, const fixed<br>& right){
    left = left / right;
    return(left);
}

// fixed /= int32
template<uint8_t bl>
fixed<bl>& operator /=(fixed<bl>& left, const int32_t& right){
    left = left / right;
    return(left);
}

// fixed += fixed
template<uint8_t bl, uint8_t br>
fixed<bl>& operator +=(fixed<bl>& left, const fixed<br>& right){
    left = left + right;
    return(left);
}

// fixed += int32
template<uint8_t bl>
fixed<bl>& operator +=(fixed<bl>& left, const int32_t& right){
    left = left + right;
    return(left);
}

// fixed -= fixed
template<uint8_t bl, uint8_t br>
fixed<bl>& operator -=(fixed<bl>& left, const fixed<br>& right){
    left = left - right;
    return(left);
}

// fixed -= int32
template<uint8_t bl>
fixed<bl>& operator -=(fixed<bl>& left, const int32_t& right){
    left = left - right;
    return(left);
}

// template<uint8_t bl, uint8_t br>
// bool operator< (fixed<bl>left, fixed<br> right){
//   tfixed r;
//   tfixed l;

//   l.data = left.data << br;
//   r.data = right.data << bl;
//   return(l.data < r.data);
// }

// template<uint8_t bl, uint8_t br>
// bool operator> (fixed<bl>left, fixed<br> right){
//   tfixed r;
//   tfixed l;

//   l.data = left.data << br;
//   r.data = right.data << bl;
//   return(l.data > r.data);
// }

// fixed == fixed
template<uint8_t b>
constexpr bool operator== (fixed<b> left, fixed<b> right){
  return(left.data == right.data);
}

// fixed == int32
template<uint8_t b>
constexpr bool operator== (fixed<b> left, int32_t right){
  return(left == fixed<b>(right));
}

// int32 == fixed
template<uint8_t b>
constexpr bool operator== (int32_t left, fixed<b> right){
  return(fixed<b>(left) == right);
}

// fixed < fixed
template<uint8_t bl, uint8_t br>
constexpr bool operator< (fixed<bl> left, fixed<br> right){
  int64_t l, r;
  l = left.data;
  r = right.data;

  if(bl < br){
    l <<= br - bl;
  }
  if(bl > br){
    r <<= bl - br;
  }

  // if(bl > br){
  //   l >>= bl - br;
  // }
  // if(bl < br){
  //   r >>= br - bl;
  // }

  return(l < r);
}

// fixed > fixed
template<uint8_t bl, uint8_t br>
constexpr bool operator> (fixed<bl> left, fixed<br> right){
  return(right < left);
}

// fixed < int32
template<uint8_t bl>
constexpr bool operator< (fixed<bl> left, int32_t right){
  return(left < fixed<bl>(right));
}

// fixed > int32
template<uint8_t bl>
constexpr bool operator> (fixed<bl> left, int32_t right){
  return(left > fixed<bl>(right));
}

// int32 < fixed
template<uint8_t br>
constexpr bool operator< (int32_t left, fixed<br> right){
  return(right > fixed<br>(left));
}

// int32 > fixed
template<uint8_t br>
constexpr bool operator> (int32_t left, fixed<br> right){
  return(right < fixed<br>(left));
}

// cos(fixed)
fixed<16> cosfp(fixed<16> v);

// sin(fixed)
inline fixed<16> sinfp(fixed<16> v){
  const fixed<16> p = 0.25;
  return(cosfp(v - p));
}

#ifdef FP_USE_IOSTREAM
// ostream << fixed
template<uint8_t b>
std::ostream& operator<<(std::ostream& os, const fixed<b>& p)
{
    fixed v = p;
    os << static_cast<const int32_t>(v.integral()) << ".";
    v = ABS(v.fractional());
    do{
        v *= 10;
        os << static_cast<const int32_t>(v.integral());
        v = v.fractional();
    }while(v > 0);
    return os;
}
#endif

// default types
typedef fixed<32> q0_32;
typedef fixed<24> q8_24;
typedef fixed<16> q16_16;
typedef fixed<8> q24_8;

constexpr int MAX(const int l, const int r){
  if(l > r){
    return(l);
  }
  return(r);
}

constexpr int MIN(const int l, const int r){
  if(l < r){
    return(l);
  }
  return(r);
}

//template<uint8_t bl, uint8_t br>
//constexpr fixed<MIN(bl, br)> MAX(const fixed<bl> l, const fixed<br> r){
template<uint8_t b>
constexpr fixed<b> MAX(const fixed<b> l, const fixed<b> r){  
  if(l > r){
    return(l);
  }
  return(r);
}

//template<uint8_t bl, uint8_t br>
//constexpr fixed<MAX(bl, br)> MIN(const fixed<bl> l, const fixed<br> r){
template<uint8_t b>
constexpr fixed<b> MIN(const fixed<b> l, const fixed<b> r){  
  if(l < r){
    return(l);
  }
  return(r);
}

template<uint8_t b>
constexpr auto MAX(const fixed<b> l, const int r){
  return(MAX(l, fixed<b>(r)));
}

template<uint8_t b>
constexpr auto MIN(const fixed<b> l, const int r){
  return(MIN(l, fixed<b>(r)));
}

template<uint8_t b>
constexpr auto MAX(const int l, const fixed<b> r){
  return(MAX(fixed<b>(l), r));
}

template<uint8_t b>
constexpr auto MIN(const int l, const fixed<b> r){
  return(MIN(fixed<b>(l), r));
}

constexpr auto MAX3(const auto l, const auto m, const auto r){
  return(MAX(MAX(l, m), r));
}

constexpr auto MIN3(const auto l, const auto m, const auto r){
  return(MIN(MIN(l, m), r));
}

constexpr int CLAMP(const int x, const int low, const int high){
  return(MIN(MAX(x, low), high));
}

template<uint8_t bl, uint8_t bm, uint8_t br>
constexpr fixed<bl> CLAMP(const fixed<bl>x, const fixed<bm>low, const fixed<br>high){
  return(fixed<bl>(MIN(MAX(x, low), high)));
}

template<uint8_t bl, uint8_t bm>
constexpr fixed<bl> CLAMP(const fixed<bl>x, const fixed<bm>low, const int high){
  return(CLAMP(x, low, fixed<bl>(high)));
}

template<uint8_t bl, uint8_t br>
constexpr fixed<bl> CLAMP(const fixed<bl>x, const int low, const fixed<br>high){
  return(CLAMP(x, fixed<bl>(low), high));
}

template<uint8_t bl>
constexpr fixed<bl> CLAMP(const fixed<bl>x, const int low, const int high){
  return(CLAMP(x, fixed<bl>(low), fixed<bl>(high)));
}

template<uint8_t bl, uint8_t br>
constexpr fixed<bl> LIMIT(const fixed<bl>x, const fixed<br>lowhigh){
  return(CLAMP(x, -lowhigh, lowhigh));
}

template<uint8_t bl>
constexpr fixed<bl> LIMIT(const fixed<bl>x, const int lowhigh){
  return(CLAMP(x, -lowhigh, lowhigh));
}

template<uint8_t bl, uint8_t bm, uint8_t br>
constexpr q16_16 SCALE(const fixed<bl>x, const fixed<bm>low, const fixed<br>high){
  fixed<MIN(bm, br)> delta = (high - low).sat();
  if(delta == 0){
    return(0);
  }
  fixed<MIN(bl, bm)> tx = (x - low).sat();
  q16_16 r = (tx / delta).sat();
  r = CLAMP(r, 0, 1);

  return(r);
}

constexpr auto ABS(auto x){
  if(x < 0){
    return(-x);
  }
  return(x);
}

template<uint8_t b>
constexpr auto SIGN(const fixed<b>x){
  if(x < fixed<b>(0)){
    return(-1);
  }
  if(x > fixed<b>(0)){
    return(1);
  }
  return(0);
}

template<uint8_t bl>
constexpr auto SIGN2(const fixed<bl> x, const auto a){
  return(CLAMP(fixed<bl>(x * a), fixed<bl>(-1), fixed<bl>(1)));
}

template<uint8_t bl, uint8_t bm, uint8_t br>
constexpr bool SAT(const fixed<bl>x, const fixed<bm>low, const fixed<br>high){
  return((x < low) | (x > high));
}

template<uint8_t bl, uint8_t bm>
constexpr bool SAT(const fixed<bl>x, const fixed<bm>low, const int high){
  return(SAT(x, low, fixed<bl>(high)));
}

template<uint8_t bl, uint8_t br>
constexpr bool SAT(const fixed<bl>x, const int low, const fixed<br>high){
  return(SAT(x, fixed<bl>(low), high));
}

template<uint8_t bl, uint8_t br>
constexpr bool SAT(const fixed<bl>x, const fixed<br>lowhigh){
  return(SAT(x, -lowhigh, lowhigh));
}

template<uint8_t bl>
constexpr bool SAT(const fixed<bl>x, const int lowhigh){
  return(SAT(x, fixed<bl>(-lowhigh), fixed<bl>(-lowhigh)));
}

const q8_24 PI = 3.14159265359;
const q8_24 SQRT2 = 1.41421356237;
const q8_24 SQRT3 = 1.73205080757;
const q8_24 SQRT2_INV = 0.70710678118;
const q8_24 SQRT3_INV = 0.57735026919;