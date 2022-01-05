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

    // constexpr operator int32_t(){
    //     int32_t r;
    //     r = data >> fbits;
    // return(r);
    // }

    constexpr int32_t intcast(){
        int32_t r;
        r = data >> fbits;
    return(r);
    }

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
            r.data &= (0xffffffff << (32 - b));
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

    constexpr operator int32_t() {
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
    r.data >>= right.fbits;
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

fixed<16> cosfp(fixed<16> v);

inline fixed<16> sinfp(fixed<16> v){
  const fixed<16> p = 0.25;
  return(cosfp(v - p));
}

typedef fixed<24> q8_24;
typedef fixed<16> q16_16;
typedef fixed<8> q24_8;