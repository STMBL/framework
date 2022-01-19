#include <stdint.h>
#pragma once

#ifndef M_PI
#define M_PI  3.1415926535
#endif

template<uint8_t b>
class fixed{
    public:
    static const uint8_t fbits = b;
    int32_t data;

    template<uint8_t bl>
    constexpr operator fixed<bl>() {
        fixed<bl> r;
        if(fbits > bl){ // check >> with negative numbers, change to / (1 << fbits)
            r.data = data >> (fbits - bl);
        }
        else{
            r.data = data << (bl - fbits);
        }
        return(r);
    }

    explicit constexpr operator int32_t() {
        return(data >> fbits);
    }

    // constexpr operator int32_t(){  
    //     int32_t r;
    //     r = data >> fbits;
    // return(r);
    // }

    // constexpr int32_t intcast(){
    //     int32_t r;
    //     r = data >> fbits;
    // return(r);
    // }

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


    constexpr fixed<b>& operator ++() {
        data += (1 << b);
        return(*this);
    }

    constexpr fixed<b>& operator --() {
        data -= (1 << b);
        return(*this);
    }

    constexpr fixed<b> operator ++(int) {
        fixed<b> r;
        r.data = data;
        ++(*this);
        return(r);
    }

    constexpr fixed<b> operator --(int) {
        fixed<b> r;
        r.data = data;
        --(*this);
        return(r);
    }

    fixed() = default;
    fixed(int v) : data (v * (1 << fbits)) {}
    fixed(int32_t v) : data (v * (1 << fbits)) {}
    fixed(uint32_t v) : data (v * (1 << fbits)) {}  
    fixed(float v) : data (v * (1 << fbits)) {}
    fixed(double v) : data (v * (1 << fbits)) {}
};

class tfixed{
    public:
    uint8_t fbits;
    uint8_t ibits;
    int64_t data;

    template<uint8_t b>
    constexpr operator fixed<b>() {
        fixed<b> r;
        if(fbits > b){ // check >> with negative numbers, change to / (1 << fbits)
            r.data = data >> (fbits - b);
        }
        else{
            r.data = data << (b - fbits);
        }
        return(r);
    }

    explicit constexpr operator int32_t() {
        return(data >> fbits);
    }
};

template<uint8_t bl>
constexpr fixed<bl> operator -(fixed<bl> right) {
    fixed<bl> r;
    r.data = -right.data;
    return(r);
}

    template<uint8_t bl>
    constexpr fixed<bl> operator *(fixed<bl> left, int32_t right) {
        fixed<bl> r;
        r.data = left.data * right;
        return(r);
    }

template<uint8_t bl>
constexpr fixed<bl> operator /(fixed<bl> left, int32_t right) {
    fixed<bl> r;
    r.data = left.data / right;
    return(r);
}

template<uint8_t bl>
constexpr fixed<bl> operator +(fixed<bl> left, int32_t right) {
    fixed<bl> r;
    r.data = left.data + (right << bl);
    return(r);
}

template<uint8_t bl>
constexpr fixed<bl> operator -(fixed<bl> left, int32_t right) {
    fixed<bl> r;
    r.data = left.data - (right << bl);
    return(r);
}

template<uint8_t bl, uint8_t br>
constexpr tfixed operator *(fixed<bl> left, fixed<br> right) {
    tfixed r;
    r.fbits = left.fbits + right.fbits;
    r.ibits = 64 - r.fbits;
    r.data = (int64_t)left.data * (int64_t)right.data;
    return(r);
}


#ifndef FP_USE_64Bit_DIV
template<uint8_t bl, uint8_t br>
constexpr tfixed operator /(fixed<bl> left, fixed<br> right) {
    tfixed r;
    r.fbits = left.fbits;
    r.ibits = 64 - left.fbits;
    r.data = left.data / right.data;
    r.data <<= right.fbits;
    return(r);
}
#else
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
#endif

template<uint8_t br>
fixed<br> operator /(const int32_t left, const fixed<br> right){
    fixed<br>r = fixed<br>(left) / right;
    return(r);
}

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

template<uint8_t bl, uint8_t br>
fixed<bl>& operator *=(fixed<bl>& left, const fixed<br>& right){
    left = left * right;
    return(left);
}

template<uint8_t bl>
fixed<bl>& operator *=(fixed<bl>& left, const int32_t& right){
    left = left * right;
    return(left);
}

template<uint8_t bl, uint8_t br>
fixed<bl>& operator /=(fixed<bl>& left, const fixed<br>& right){
    left = left / right;
    return(left);
}

template<uint8_t bl>
fixed<bl>& operator /=(fixed<bl>& left, const int32_t& right){
    left = left / right;
    return(left);
}

template<uint8_t bl, uint8_t br>
fixed<bl>& operator +=(fixed<bl>& left, const fixed<br>& right){
    left = left + right;
    return(left);
}

template<uint8_t bl>
fixed<bl>& operator +=(fixed<bl>& left, const int32_t& right){
    left = left + right;
    return(left);
}

template<uint8_t bl, uint8_t br>
fixed<bl>& operator -=(fixed<bl>& left, const fixed<br>& right){
    left = left - right;
    return(left);
}

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

template<uint8_t bl, uint8_t br>
constexpr bool operator< (fixed<bl> left, fixed<br> right){

  int32_t l, r;
  l = left.data;
  r = right.data;

  if(bl < br){
    l >>= br - bl;
  }
  if(bl > br){
    r >>= bl - br;
  }

  return(l < r);
}

template<uint8_t bl, uint8_t br>
constexpr bool operator> (fixed<bl> left, fixed<br> right){
  return(right < left);
}

template<uint8_t bl>
constexpr bool operator< (fixed<bl> left, int right){
  return(left < fixed<bl>(right));
}

template<uint8_t bl>
constexpr bool operator> (fixed<bl> left, int right){
  return(left > fixed<bl>(right));
}

template<uint8_t br>
constexpr bool operator< (int left, fixed<br> right){
  return(right > fixed<br>(left));
}

template<uint8_t br>
constexpr bool operator> (int left, fixed<br> right){
  return(right < fixed<br>(left));
}

fixed<16> cosfp(fixed<16> v);

inline fixed<16> sinfp(fixed<16> v){
  const fixed<16> p = 0.25;
  return(cosfp(v - p));
}

typedef fixed<24> q8_24;
typedef fixed<16> q16_16;
typedef fixed<8> q24_8;

template<uint8_t b>
constexpr auto MAX(const fixed<b> l, const fixed<b> r){
  if(l > r){
    return(l);
  }
  return(r);
}

template<uint8_t b>
constexpr auto MIN(const fixed<b> l, const fixed<b> r){
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
  return(MIN(MAX(x, low), high));
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
constexpr q8_24 SCALE(const fixed<bl>x, const fixed<bm>low, const fixed<br>high){
  fixed<br> delta = high - low;
  fixed<bl>tx = x - low;
  q8_24 r = tx / delta;
  r = CLAMP(r, 0, 1);

  return(r);
}

template<uint8_t b>
constexpr auto ABS(const fixed<b>x){
  if(x < fixed<b>(0)){
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