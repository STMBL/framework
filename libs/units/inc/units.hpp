#pragma once
#include <stdint.h>
#ifdef unit_tS_PRINT
#include <iostream>
#endif

// template<typename l_T, int l_second, int l_meter, int l_kilogram, int l_ampere, int l_kelvin, int l_mole, int l_candela, typename r_T, int r_second, int r_meter, int r_kilogram, int r_ampere, int r_kelvin, int r_mole, int r_candela>
// concept same_unit = ((l_second == r_second) && (l_meter == r_meter));

struct units_t {
  int second = 0;
  int meter = 0;
  int kilogram = 0;
  int ampere = 0;
  int kelvin = 0;
  int mole = 0;
  int candela = 0;
  int two_pi_is_1 = 0;

};

constexpr units_t X = {};
constexpr units_t S = {.second = 1};
constexpr units_t M = {.second = 0, .meter = 1};
constexpr units_t Kg = {.second = 0, .meter = 0, .kilogram = 1};
constexpr units_t A = {.second = 0, .meter = 0, .kilogram = 0, .ampere = 1};
constexpr units_t K = {.second = 0, .meter = 0, .kilogram = 0, .ampere = 0, .kelvin = 1};
constexpr units_t Mo = {.second = 0, .meter = 0, .kilogram = 0, .ampere = 0, .kelvin = 0, .mole = 1};
constexpr units_t Cd = {.second = 0, .meter = 0, .kilogram = 0, .ampere = 0, .kelvin = 0, .mole = 0, .candela = 1};
constexpr units_t TP = {.second = 0, .meter = 0, .kilogram = 0, .ampere = 0, .kelvin = 0, .mole = 0, .candela = 0, .two_pi_is_1 = 1};


constexpr bool operator== (const units_t left, const units_t right){
  return(left.second == right.second && left.meter == right.meter && left.kilogram == right.kilogram && left.ampere == right.ampere && left.kelvin == right.kelvin && left.mole == right.mole && left.candela == right.candela && left.two_pi_is_1 == right.two_pi_is_1);
}

constexpr units_t operator* (const int left, const units_t right){
  units_t r = {.second = left * right.second, .meter = left * right.meter, .kilogram = left * right.kilogram, .ampere = left * right.ampere, .kelvin = left * right.kelvin, .mole = left * right.mole, .candela = left * right.candela, .two_pi_is_1 = left * right.two_pi_is_1};
  return(r);
}

constexpr units_t operator* (const units_t left, const int right){
  units_t r = right * left;
  return(r);
}

constexpr units_t operator+ (const units_t left, const units_t right){
  units_t r = {.second = left.second + right.second, .meter = left.meter + right.meter, .kilogram = left.kilogram + right.kilogram, .ampere = left.ampere + right.ampere, .kelvin = left.kelvin + right.kelvin, .mole = left.mole + right.mole, .candela = left.candela + right.candela, .two_pi_is_1 = left.two_pi_is_1 + right.two_pi_is_1};
  return(r);
}

constexpr units_t operator- (const units_t left, const units_t right){
  units_t r = {.second = left.second - right.second, .meter = left.meter - right.meter, .kilogram = left.kilogram - right.kilogram, .ampere = left.ampere - right.ampere, .kelvin = left.kelvin - right.kelvin, .mole = left.mole - right.mole, .candela = left.candela - right.candela, .two_pi_is_1 = left.two_pi_is_1 - right.two_pi_is_1};
  return(r);
}

constexpr units_t operator- (const units_t right){
    return(X - right);
}

// constexpr units_t operator/ (const int left, const units_t right){
//   units_t r = {.second = -right.second, .meter = -right.meter, .kilogram = -right.kilogram, .ampere = -right.ampere, .kelvin = -right.kelvin, .mole = -right.mole, .candela = -right.candela, .two_pi_is_1 = -right.two_pi_is_1};
//   return(r);
// }

template<typename T, units_t U>
class unit_t{
  public:
    T value;

    template<typename r_T, units_t r_U>
    unit_t<T, U> constexpr & operator =(unit_t<r_T, r_U> right) {
      static_assert(U.second == r_U.second);
      static_assert(U.meter == r_U.meter);
      static_assert(U.kilogram == r_U.kilogram);
      static_assert(U.ampere == r_U.ampere);
      static_assert(U.kelvin == r_U.kelvin);
      static_assert(U.mole == r_U.mole);
      static_assert(U.candela == r_U.candela);
      static_assert(U.two_pi_is_1 == r_U.two_pi_is_1);
  
      value = right.value;
      return(*this);
    }

    template<typename r_T, units_t r_U>
    unit_t(unit_t<r_T, r_U> right) {
      static_assert(U.second == r_U.second);
      static_assert(U.meter == r_U.meter);
      static_assert(U.kilogram == r_U.kilogram);
      static_assert(U.ampere == r_U.ampere);
      static_assert(U.kelvin == r_U.kelvin);
      static_assert(U.mole == r_U.mole);
      static_assert(U.candela == r_U.candela);
      static_assert(U.two_pi_is_1 == r_U.two_pi_is_1);
      value = right.value;
    }
              
    constexpr unit_t(auto v) : value (v) {}
    constexpr unit_t() : value (0) {}
};

template<typename l_T, units_t l_U, typename r_T, units_t r_U>
constexpr bool operator ==(unit_t<l_T, l_U> left, unit_t<r_T, r_U> right) {
  static_assert(l_U.second == r_U.second);
  static_assert(l_U.meter == r_U.meter);
  static_assert(l_U.kilogram == r_U.kilogram);
  static_assert(l_U.ampere == r_U.ampere);
  static_assert(l_U.kelvin == r_U.kelvin);
  static_assert(l_U.mole == r_U.mole);
  static_assert(l_U.candela == r_U.candela);
  static_assert(l_U.two_pi_is_1 == r_U.two_pi_is_1);

  return(left.value == right.value);
}

template<typename l_T, units_t l_U, typename r_T, units_t r_U>
constexpr bool operator <(unit_t<l_T, l_U> left, unit_t<r_T, r_U> right) {
  static_assert(l_U.second == r_U.second);
  static_assert(l_U.meter == r_U.meter);
  static_assert(l_U.kilogram == r_U.kilogram);
  static_assert(l_U.ampere == r_U.ampere);
  static_assert(l_U.kelvin == r_U.kelvin);
  static_assert(l_U.mole == r_U.mole);
  static_assert(l_U.candela == r_U.candela);
  static_assert(l_U.two_pi_is_1 == r_U.two_pi_is_1);

  return(left.value < right.value);
}

template<typename l_T, units_t l_U, typename r_T, units_t r_U>
constexpr bool operator >(unit_t<l_T, l_U> left, unit_t<r_T, r_U> right) {

  return(right < left);
}
template<typename l_T, units_t l_U, typename r_T, units_t r_U>
constexpr auto operator *(unit_t<l_T, l_U> left, unit_t<r_T, r_U> right) {
  auto v = left.value * right.value;
  unit_t<decltype(v), l_U + r_U> r = v;
  return(r);
}
template<typename l_T, units_t l_U>
constexpr auto operator *(unit_t<l_T, l_U> left, auto right) {
  auto v = left.value * right;
  unit_t<decltype(v), l_U> r = v;
  return(r);
}

template<typename r_T, units_t r_U>
constexpr auto operator *(auto left, unit_t<r_T, r_U> right) {
  auto v = left * right.value;
  unit_t<decltype(v), r_U> r = v;
  return(r);
}

constexpr auto& operator *=(auto& left, const auto& right){
  left = left * right;
  return(left);
}

template<typename l_T, units_t l_U, typename r_T, units_t r_U>
constexpr auto operator /(unit_t<l_T, l_U> left, unit_t<r_T, r_U> right) {
  auto v = left.value / right.value;
  unit_t<decltype(v), l_U - r_U> r = v;
  return(r);
}

template<typename l_T, units_t l_U>
constexpr auto operator /(unit_t<l_T, l_U> left, auto right) {
  auto v = left.value / right;
  unit_t<decltype(v), l_U> r = v;
  return(r);
}

template<typename r_T, units_t r_U>
constexpr auto operator /(auto left, unit_t<r_T, r_U> right) {
  auto v = left / right.value;
  unit_t<decltype(v), r_U> r = v;
  return(r);
}

// constexpr auto& operator /=(auto& left, const auto& right){
//   left = left / right;
//   return(left);
// }

template<typename l_T, units_t l_U, typename r_T, units_t r_U>
constexpr auto operator +(unit_t<l_T, l_U> left, unit_t<r_T, r_U> right) {
  static_assert(l_U.second == r_U.second);
  static_assert(l_U.meter == r_U.meter);
  static_assert(l_U.kilogram == r_U.kilogram);
  static_assert(l_U.ampere == r_U.ampere);
  static_assert(l_U.kelvin == r_U.kelvin);
  static_assert(l_U.mole == r_U.mole);
  static_assert(l_U.candela == r_U.candela);
  static_assert(l_U.two_pi_is_1 == r_U.two_pi_is_1);

  auto v = left.value + right.value;
  unit_t<decltype(v), l_U> r = v;
  return(r);
}

constexpr auto& operator +=(auto& left, const auto& right){
  left = left + right;
  return(left);
}

template<typename l_T, units_t l_U, typename r_T, units_t r_U>
constexpr auto operator -(unit_t<l_T, l_U> left, unit_t<r_T, r_U> right) {
  static_assert(l_U.second == r_U.second);
  static_assert(l_U.meter == r_U.meter);
  static_assert(l_U.kilogram == r_U.kilogram);
  static_assert(l_U.ampere == r_U.ampere);
  static_assert(l_U.kelvin == r_U.kelvin);
  static_assert(l_U.mole == r_U.mole);
  static_assert(l_U.candela == r_U.candela);
  static_assert(l_U.two_pi_is_1 == r_U.two_pi_is_1);

  auto v = left.value - right.value;
  unit_t<decltype(v), l_U> r = v;
  return(r);
}

constexpr auto& operator -=(auto& left, const auto& right){
  left = left - right;
  return(left);
}


constexpr units_t N = Kg + M - 2 * S;
constexpr units_t Nm = Kg + 2 * M - 2 * S;
constexpr units_t V = Kg + 2 * M - A - 3 * S;
constexpr units_t Hz = -S;
constexpr units_t W = V + A;
constexpr units_t Wb = V + S;
constexpr units_t H = Wb - A;
constexpr units_t Oh = V - A;
constexpr units_t C = A + S;
constexpr units_t J = W + S;
constexpr units_t F = C - V;


template<typename T>
using Rad = unit_t<T, X>; // [2 * PI]

template<typename T>
using Frad = unit_t<T, TP>; // [1]

template<typename T>
using Length = unit_t<T, M>; // [m]

template<typename T>
using Duration = unit_t<T, S>; // [s]

template<typename T>
using Frequency = unit_t<T, Hz>; // [s^-1]

template<typename T>
using Velocity = unit_t<T, M - S>; // [m * s^-1]

template<typename T>
using Ang_velocity = unit_t<T, -S>; // [2 * PI * s^-1]

template<typename T>
using Fang_velocity = unit_t<T, Hz + TP>; // [s^-1]

template<typename T>
using Acceleration = unit_t<T, M + 2 * S>; // [m * s^-2]

template<typename T>
using Ang_acceleration = unit_t<T, -2 * S>; // [2 * PI * s^-2]

template<typename T>
using Fang_acceleration = unit_t<T, TP - 2 * S>; // [s^-2]

template<typename T>
using Inertia = unit_t<T, Kg + 2 * M>; // [kg * m^2]

template<typename T>
using Finertia = unit_t<T, Kg + 2 * M + TP>; // [2 * PI * kg * m^2]

template<typename T>
using Force = unit_t<T, N>; // [kg * m * s^-2]

template<typename T>
using Torque = unit_t<T, Nm>; // [kg * m^2 * s^-2]

template<typename T>
using Temperature = unit_t<T, K>; // [K]

template<typename T>
using Mass = unit_t<T, Kg>; // [kg]

template<typename T>
using Energy = unit_t<T, Nm>; // [kg * m^2 * s^-2]

template<typename T>
using Power = unit_t<T, V + A>; // [kg * m^2 * s^-3]

template<typename T>
using Current = unit_t<T, A>; // [A]

template<typename T>
using Voltage = unit_t<T, V>; // [kg * m^2 * A^-1 * s^-3]

template<typename T>
using Resistance = unit_t<T, V - A>; // [kg * m^2 * A^-2 * s^-3]

template<typename T>
using Inductance = unit_t<T, V + S - A>; // [kg * m^2 * s^-2 * A^-2]

template<typename T>
using Capacitance = unit_t<T, A + S - V>; // [s^4 * A^2 * kg^-1 * m^-2]

template<typename T>
using Mag_flux = unit_t<T, V + S>; // [kg * m^2 * s^-2 * A^-1]


template<typename T>
constexpr auto SIGN(const T x){
  if(x < T(0)){
    return(-1);
  }
  if(x > T(0)){
    return(1);
  }
  return(0);
}