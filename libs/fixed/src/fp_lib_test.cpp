//#define FP_USE_64Bit_DIV

#include <fp_lib.hpp>
#include <iostream>

// TODO
// %
// sqrt
// atan2
// +-*/ int, float
// int, float constructor
// math const, pi, sqrt(2), sqrt(3), ...
// <>==!= int, fixed

template <int bits>
float round(float a){
  a *= 1 << bits;
  a = (int)a;
  a /= 1 << bits;
  return(a);
}

template <int bits>
bool test(fixed<bits> a, float b){
  float f = a.data / (float)(1 << bits);
  float delta = 10.0 / (float)(1 << bits);

  float diff = b - f;
  if(diff < 0.0){
    diff *= -1.0;
  }

  if(diff < delta){
    return(false);
  }
  std::cout << "delta: " << delta << " diff: " << diff << "  ";
  return(true);
}

template<int bits>
bool sign_test(float a, float b){
  fixed<bits> f = a;

  bool result = false;
  
  if(a > 1.0 / (1 << bits)){
    result = test<bits>(SIGN(f), 1.0);
  }
  else if(a < -1.0 / (1 << bits)){
    result = test<bits>(SIGN(f), -1.0);
  }
  else{
    result = test<bits>(SIGN(f), 0.0);
  }

  if(result){
    std::cout << "SIGN " << a << "<" << bits << "> failed" << std::endl;
  }
  
  return(result);
}

template<int lbits, int rbits>
bool l_test(float a, float b){
  fixed<lbits> l = a;
  fixed<rbits> r = b;

  bool result = l < r != round<lbits>(a) < round<rbits>(b);
  if(result){
    std::cout << "< " << round<lbits>(a) << "<" << lbits << "> " << round<lbits>(b) << "<" << rbits << "> failed" << std::endl;
  }
  
  return(result);
}

template<int bits, int lbits, int rbits>
bool mult_test(float a, float b){
  fixed<lbits> l = a;
  fixed<rbits> r = b;

  bool result = test<bits>((fixed<bits>)(l * r), round<lbits>(a) * round<rbits>(b));
  if(result){
    std::cout << "*<" << bits << "> " << round<lbits>(a) << "<" << lbits << "> " << round<rbits>(b) << "<" << rbits << "> failed" << std::endl;
  }
  
  return(result);
}

template<int bits, int lbits, int rbits>
bool div_test(float a, float b){
  fixed<lbits> l = a;
  fixed<rbits> r = b;

  bool result = false;

  if(r.data != 0){
    result = test<bits>((fixed<bits>)(l / r), round<lbits>(a) / round<rbits>(b));
    if(result){
      std::cout << "/<" << bits << "> " << round<lbits>(a) << "<" << lbits << "> " << round<rbits>(b) << "<" << rbits << "> failed" << std::endl;
    }
  }
  
  
  return(result);
}

template<int bits, int lbits, int rbits>
bool sub_test(float a, float b){
  fixed<lbits> l = a;
  fixed<rbits> r = b;

  bool result = false;

  if(r.data != 0){
    result = test<bits>((fixed<bits>)(l - r), round<lbits>(a) - round<rbits>(b));
    if(result){
      std::cout << "-<" << bits << "> " << round<lbits>(a) << "<" << lbits << "> " << round<rbits>(b) << "<" << rbits << "> failed" << std::endl;
    }
  }
  
  
  return(result);
}

int main(){
  
  float data[] = {0.0, 1.5, -1.5, 3.141526, 10.0, -11.23, 0.00123, -0.0, -0.01234};
  int result = 0;

  for(int i = 0; i < sizeof(data) / sizeof(float); i++){
    for(int j = 0; j < sizeof(data) / sizeof(float); j++){
      // SIGN
      result += sign_test<8>(data[i], data[j]);
      result += sign_test<16>(data[i], data[j]);
      result += sign_test<24>(data[i], data[j]);
      

      // <
      result += l_test<8, 8>(data[i], data[j]);
      result += l_test<8, 16>(data[i], data[j]);
      result += l_test<8, 24>(data[i], data[j]);
      result += l_test<16, 8>(data[i], data[j]);
      result += l_test<16, 16>(data[i], data[j]);
      result += l_test<16, 24>(data[i], data[j]);
      result += l_test<24, 8>(data[i], data[j]);
      result += l_test<24, 16>(data[i], data[j]);
      result += l_test<24, 24>(data[i], data[j]);
      

      // *
      result += mult_test<8, 8, 8>(data[i], data[j]);
      result += mult_test<8, 8, 16>(data[i], data[j]);
      result += mult_test<8, 8, 24>(data[i], data[j]);
      result += mult_test<8, 16, 8>(data[i], data[j]);
      result += mult_test<8, 16, 16>(data[i], data[j]);
      result += mult_test<8, 16, 24>(data[i], data[j]);
      result += mult_test<8, 24, 8>(data[i], data[j]);
      result += mult_test<8, 24, 16>(data[i], data[j]);
      result += mult_test<8, 24, 24>(data[i], data[j]);
      
      result += mult_test<16, 8, 8>(data[i], data[j]);
      result += mult_test<16, 8, 16>(data[i], data[j]);
      result += mult_test<16, 8, 24>(data[i], data[j]);
      result += mult_test<16, 16, 8>(data[i], data[j]);
      result += mult_test<16, 16, 16>(data[i], data[j]);
      result += mult_test<16, 16, 24>(data[i], data[j]);
      result += mult_test<16, 24, 8>(data[i], data[j]);
      result += mult_test<16, 24, 16>(data[i], data[j]);
      result += mult_test<16, 24, 24>(data[i], data[j]);
      
      result += mult_test<24, 8, 8>(data[i], data[j]);
      result += mult_test<24, 8, 16>(data[i], data[j]);
      result += mult_test<24, 8, 24>(data[i], data[j]);
      result += mult_test<24, 16, 8>(data[i], data[j]);
      result += mult_test<24, 16, 16>(data[i], data[j]);
      result += mult_test<24, 16, 24>(data[i], data[j]);
      result += mult_test<24, 24, 8>(data[i], data[j]);
      result += mult_test<24, 24, 16>(data[i], data[j]);
      result += mult_test<24, 24, 24>(data[i], data[j]);

      // /
      // result += div_test<8, 8, 8>(data[i], data[j]);
      // result += div_test<8, 8, 16>(data[i], data[j]);
      // result += div_test<8, 8, 24>(data[i], data[j]);
      // result += div_test<8, 16, 8>(data[i], data[j]);
      // result += div_test<8, 16, 16>(data[i], data[j]);
      // result += div_test<8, 16, 24>(data[i], data[j]);
      // result += div_test<8, 24, 8>(data[i], data[j]);
      // result += div_test<8, 24, 16>(data[i], data[j]);
      // result += div_test<8, 24, 24>(data[i], data[j]);
      
      // result += div_test<16, 8, 8>(data[i], data[j]);
      // result += div_test<16, 8, 16>(data[i], data[j]);
      // result += div_test<16, 8, 24>(data[i], data[j]);
      // result += div_test<16, 16, 8>(data[i], data[j]);
      // result += div_test<16, 16, 16>(data[i], data[j]);
      // result += div_test<16, 16, 24>(data[i], data[j]);
      // result += div_test<16, 24, 8>(data[i], data[j]);
      // result += div_test<16, 24, 16>(data[i], data[j]);
      // result += div_test<16, 24, 24>(data[i], data[j]);
      
      // result += div_test<24, 8, 8>(data[i], data[j]);
      // result += div_test<24, 8, 16>(data[i], data[j]);
      // result += div_test<24, 8, 24>(data[i], data[j]);
      // result += div_test<24, 16, 8>(data[i], data[j]);
      // result += div_test<24, 16, 16>(data[i], data[j]);
      // result += div_test<24, 16, 24>(data[i], data[j]);
      // result += div_test<24, 24, 8>(data[i], data[j]);
      // result += div_test<24, 24, 16>(data[i], data[j]);
      // result += div_test<24, 24, 24>(data[i], data[j]);

      // -
      result += sub_test<8, 8, 8>(data[i], data[j]);
      result += sub_test<8, 8, 16>(data[i], data[j]);
      result += sub_test<8, 8, 24>(data[i], data[j]);
      result += sub_test<8, 16, 8>(data[i], data[j]);
      result += sub_test<8, 16, 16>(data[i], data[j]);
      result += sub_test<8, 16, 24>(data[i], data[j]);
      result += sub_test<8, 24, 8>(data[i], data[j]);
      result += sub_test<8, 24, 16>(data[i], data[j]);
      result += sub_test<8, 24, 24>(data[i], data[j]);
      
      result += sub_test<16, 8, 8>(data[i], data[j]);
      result += sub_test<16, 8, 16>(data[i], data[j]);
      result += sub_test<16, 8, 24>(data[i], data[j]);
      result += sub_test<16, 16, 8>(data[i], data[j]);
      result += sub_test<16, 16, 16>(data[i], data[j]);
      result += sub_test<16, 16, 24>(data[i], data[j]);
      result += sub_test<16, 24, 8>(data[i], data[j]);
      result += sub_test<16, 24, 16>(data[i], data[j]);
      result += sub_test<16, 24, 24>(data[i], data[j]);
      
      result += sub_test<24, 8, 8>(data[i], data[j]);
      result += sub_test<24, 8, 16>(data[i], data[j]);
      result += sub_test<24, 8, 24>(data[i], data[j]);
      result += sub_test<24, 16, 8>(data[i], data[j]);
      result += sub_test<24, 16, 16>(data[i], data[j]);
      result += sub_test<24, 16, 24>(data[i], data[j]);
      result += sub_test<24, 24, 8>(data[i], data[j]);
      result += sub_test<24, 24, 16>(data[i], data[j]);
      result += sub_test<24, 24, 24>(data[i], data[j]);
      
    }
  }

  std::cout << result << " tests failed" << std::endl;
  return(0);
}






// g++ -I../inc  -fsingle-precision-constant -fno-strict-aliasing -Wall -Wmaybe-uninitialized -Wuninitialized -Wdouble-promotion -Wundef -O2 -ggdb -std=c++20 -fno-rtti  -fno-exceptions -Wno-volatile  fp_lib_test.cpp -o fp_lib_test