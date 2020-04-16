/*
    File: bignum.cc
*/

/*
Copyright (c) 2014, Christian E. Schafmeister

CLASP is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

See directory 'clasp/licenses' for full details.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
/* -^- */
//#define DEBUG_LEVEL_FULL

#include <boost/format.hpp>
#include <clasp/core/foundation.h>
#include <clasp/core/common.h>
#include <clasp/core/numbers.h>
#include <clasp/core/symbol.h>
#include <clasp/core/evaluator.h>
#include <clasp/core/hashTable.h>
#include <clasp/core/bignum.h>
#include <clasp/core/wrappers.h>

namespace core {

CL_PKG_NAME(CorePkg,make-bignum);
CL_DEFUN Bignum_sp Bignum_O::make(const string &value_in_string, unsigned int base) {
  GC_ALLOCATE(Bignum_O, bn);
  bn->setFromString(value_in_string,base);
  return bn->normalize();
};

__attribute__((optnone)) Bignum_sp Bignum_O::create(gc::Fixnum i){
  GC_ALLOCATE(Bignum_O, b);
  if(i){
    b->numberoflimbs = ( i>0)? (sizeof(gc::Fixnum))/sizeof(mp_limb_t) : -(mp_size_t)((sizeof(gc::Fixnum))/sizeof(mp_limb_t));
  //std::cout << "b->numberoflimbs " << b->numberoflimbs << "\n";
    b->limbs = (mp_limb_t*)GC_MALLOC(abs(b->numberoflimbs)*sizeof(mp_limb_t));
  //b->debug_print();
  //fixnum is 62 bits
  // what if GMP limbs are smaller than 62 bits?
    if(i<0){
      b->limbs[0]=(mp_limb_t)(-i); 
      //if(b->numberoflimbs>1)b->limbs[1]=(mp_limb_t)((-i)>>GMP_LIMB_BITS);
    }
    else if (i>= 0){
      b->limbs[0]=(mp_limb_t)i;
      //if(b->numberoflimbs>1)b->limbs[1]=(mp_limb_t)((-i)>>GMP_LIMB_BITS);
    }// need to fix this to get types to deal with potential type mismatch
    
    return ((b));
  }
  b->numberoflimbs=0;
  return b;
};

LongLongInt Bignum_O::as_LongLongInt_() const {
  SIMPLE_ERROR(BF("Implement as_LongLongInt_ for %s") % this->__repr__());
}

__attribute__((optnone)) Integer_sp Bignum_O::maybe_as_fixnum() {
  //std::cout << "in maybe_as_fixnum \n";
  //this->debug_print();
  if(this->numberoflimbs ==1 && this->limbs[0] <= MOST_POSITIVE_FIXNUM){
 //can check to see if less than 2^62 using an or
   // std::cout << "making a fixnum " << make_fixnum(this->limbs[0]) ;
      return make_fixnum(this->limbs[0]);
  }
    if((this->numberoflimbs==-1) && this->limbs[0] <= MOST_POSITIVE_FIXNUM+1)
      return make_fixnum(-((Fixnum)this->limbs[0]));
  if(this->numberoflimbs == 0)
    return make_fixnum(0);
  return this->asSmartPtr();
}

unsigned long long Bignum_O::as_unsigned_long_long_() const {
  if (sizeof(unsigned long long) == sizeof(uint64_t)) {
    return this->as_uint64_();
  }
  SIMPLE_ERROR(BF("Handle unsigned long long != uint64_t"));
  //	TYPE_ERROR(this->asSmartPtr(),Cons_O::createList(cl::_sym_Integer_O,make_fixnum(0),Integer_O::create(gc::most_positive_unsigned_long_long)));
}

void test_bignum(Bignum_sp b){std::cout << "test bignum"; }

void Bignum_O::sxhash_(HashGenerator &hg) const {
  hg.addValue(this->numberoflimbs);
  for(int i=0;i<abs(numberoflimbs);i++)
    hg.addValue(this->limbs[i]);
}

gc::Fixnum Bignum_O::as_int_() const {
  IMPLEMENT_MEF("Implement conversion of Bignum to Fixnum");
  SIMPLE_ERROR(BF("implement as_int_()"));
  TYPE_ERROR(this->asSmartPtr(), Cons_O::createList(cl::_sym_Integer_O, make_fixnum(gc::most_negative_int), make_fixnum(gc::most_positive_int)));
}

int64_t Bignum_O::as_int64_() const
{
  SIMPLE_ERROR(BF("implement as_int64_()"));
}

uint64_t Bignum_O::as_uint64_() const
{
  SIMPLE_ERROR(BF("implement as_uint64_()"));
}

/*! This helps us debug the as_uint64 function by returning a string representation of the uint64 */
CL_LISPIFY_NAME("core:asUint64String");
CL_DEFMETHOD string Bignum_O::as_uint64_string() const {
  uint64_t ui64 = clasp_to_uint64(this->asSmartPtr());
  stringstream ss;
  ss << ui64;
  return ((ss.str()));
}

CL_LISPIFY_NAME("core:fitsSintP");
CL_DEFMETHOD bool Bignum_O::fits_sint_p() {
  int maxlimbs = ((sizeof(signed int) *8) /GMP_LIMB_BITS);
  return (this->numberoflimbs<maxlimbs) && (this->numberoflimbs>-maxlimbs); //may need to improve this
}

// --- TRANSLATION METHODS ---

// -- SHORT --

inline short Bignum_O::as_short() const {
  SIMPLE_ERROR(BF("implement as_short"));
}

inline unsigned short Bignum_O::as_ushort() const {
  SIMPLE_ERROR(BF("implement as_ushort"));
}

// -- INT --

inline int Bignum_O::as_int() const {
  SIMPLE_ERROR(BF("implement as_int"));
}

inline unsigned int Bignum_O::as_uint() const {
  SIMPLE_ERROR(BF("implement as_uint"));
}

// --  LONG --

inline long Bignum_O::as_long() const {
  //SIMPLE_ERROR(BF("implement as_long"));
  if(numberoflimbs>0)return (long)(this->limbs[0]);
  else return -(long)this->limbs[0]; // will this work? it looks like we need little-endian
}

inline unsigned long Bignum_O::as_ulong() const {
  SIMPLE_ERROR(BF("implement as_ulong"));
}

// -- LONG LONG --

inline long long Bignum_O::as_longlong() const {
#ifdef CLASP_MS_WINDOWS_HOST
#error "Add support for windows and long long bignum conversions"
#endif
  return this->as_int64_();
}

inline unsigned long long Bignum_O::as_ulonglong() const {
#ifdef CLASP_MS_WINDOWS_HOST
#error "Add support for windows and unsigned long long bignum conversions"
#endif
  return this->as_uint64_();
}

// -- INT8 --

inline int8_t Bignum_O::as_int8_t() const {
  SIMPLE_ERROR(BF("implement as_int8_t"));
}

inline uint8_t Bignum_O::as_uint8_t() const {
  SIMPLE_ERROR(BF("implement as_uint8_t"));
}

// -- INT16 --

inline int16_t Bignum_O::as_int16_t() const {
  SIMPLE_ERROR(BF("implement as_int16_t"));
}

inline uint16_t Bignum_O::as_uint16_t() const {
  SIMPLE_ERROR(BF("implement as_uint16_t"));
}

// -- INT32 --

inline int32_t Bignum_O::as_int32_t() const {
  SIMPLE_ERROR(BF("implement as_int32_t"));
}

inline uint32_t Bignum_O::as_uint32_t() const {
  SIMPLE_ERROR(BF("implement as_uint32_t"));
}

// -- INT64 --

inline int64_t Bignum_O::as_int64_t() const {
  SIMPLE_ERROR(BF("implement as_int64_t"));
}

inline uint64_t Bignum_O::as_uint64_t() const {
  SIMPLE_ERROR(BF("implement as_uint64_t"));
}

// -- UINTPTR_T --

inline uintptr_t Bignum_O::as_uintptr_t() const
{
  SIMPLE_ERROR(BF("implement as_uintptr_t"));
}

// -- PTRDIFF_T --

inline ptrdiff_t Bignum_O::as_ptrdiff_t() const {
  SIMPLE_ERROR(BF("implement as_uintptr_t"));
  
}

// -- SIZE_T --

inline size_t Bignum_O::as_size_t() const {
  SIMPLE_ERROR(BF("implement as_size_t"));
}

// -- SSIZE_T --

inline ssize_t Bignum_O::as_ssize_t() const {
 
  SIMPLE_ERROR(BF("implement as_size_t"));
}

// --- ---

float Bignum_O::as_float_() const {
  // Using Horner's method.. maybe there is a better way
  //std::cout << "Using horner's method to cast to float - maybe not good\n";
  if(this->numberoflimbs==0)return 1.0f;
  float acc = 0.0f;//this->numberoflimbs<0? -1.0f : 1.0f;
  for(int i=abs(this->numberoflimbs)-1;i>=0;i--){
    acc*=powf(2.0f,GMP_LIMB_BITS);
    acc+=((float)this->limbs[i]);
    //acc*=powf(2.0f,GMP_LIMB_BITS);
  }
  acc*=this->numberoflimbs<0?-1.0f:1.0f;
  //std::cout << this->__repr__();
  //std::cout << "float was " << acc << "\n...\n";
  return acc;
  //SIMPLE_ERROR(BF("implement as_float_"));
}

double Bignum_O::as_double_() const {
  
  SIMPLE_ERROR(BF("implement as_double_"));
}

LongFloat Bignum_O::as_long_float_() const {
  
  SIMPLE_ERROR(BF("implement as_long_float_"));
}

// --- END OF TRANSLATION METHODS ---

__attribute__((optnone)) void Bignum_O::debug_print() const {
  std::cout << "---\nBignum " << this->__repr__() << "\n" << this->numberoflimbs << " limbs\n";
  for(int i=0;i<abs(numberoflimbs);i++){
    std::cout << "Limb " << i << ": " << this->limbs[i] << "\n";
  }
  std::cout << "---\n";
}

__attribute__((optnone)) void Bignum_O::setFromString(const string &value_in_string, unsigned int base) {
  this->numberoflimbs=(mp_size_t)ceil((log(base)/(log(2)*GMP_LIMB_BITS)) * value_in_string.length());
  this->limbs=(mp_limb_t*)GC_MALLOC(this->numberoflimbs*sizeof(mp_limb_t));
  const char* c_string = value_in_string.c_str();
  unsigned char* c_out_string = (unsigned char*)malloc(sizeof(unsigned char)*value_in_string.length());
  if(c_string[0]!='-'){
    for(int i=0;i<value_in_string.length();i++){
      c_out_string[i]=(unsigned char)(c_string[i]-(('0'<=c_string[i] && c_string[i] <= '9')?'0':'A')); // string not in ASCII. put a bounds check here?
    }  
    this->numberoflimbs=mpn_set_str(this->limbs,c_out_string,value_in_string.length(),base);
  }
  else{
    for(int i=1;i<value_in_string.length();i++){
      c_out_string[i-1]=(unsigned char)(c_string[i]-(('0'<=c_string[i] && c_string[i] <= '9')?'0':'A')); // string not in ASCII. put a bounds check here?
    }  
    this->numberoflimbs=-mpn_set_str(this->limbs,c_out_string,value_in_string.length()-1,base);
  }
  //std::cout << "Bignum created from string: "+value_in_string + "\n";
  free(c_out_string);
}

gc::Fixnum Bignum_O::bit_length_() const {
  SIMPLE_ERROR(BF("implement bit_length")); //this could be tricky
}
//better way to do this?
#if (GMP_LIMB_BITS==64)
#define BIGNUM_LIMB_SHIFT 6
#endif
#if (GMP_LIMB_BITS==32)
#define BIGNUM_LIMB_SHIFT 5
#endif
#if (GMP_LIMB_BITS==16)
#define BIGNUM_LIMB_SHIFT 4
#endif 
/*! Return the value shifted by BITS bits.
      If BITS < 0 shift right, if BITS >0 shift left. */
Bignum_sp Bignum_O::shift_big(gc::Fixnum bits) const{
  if(this->numberoflimbs==0)return Bignum_O::create((Fixnum)0); // we are zero,
  // so shifting has no effect
  if(bits < 0){ // right shift
    bits=-bits;
    bool change_rounding_p=false;
    GC_ALLOCATE_VARIADIC(Bignum_O,shifted);
    if(abs(this->numberoflimbs)<=(bits/GMP_LIMB_BITS))return Bignum_O::create((Fixnum)0);
    shifted->realloc_limbs(abs(this->numberoflimbs)-(bits/GMP_LIMB_BITS));
    if(bits & (GMP_LIMB_BITS-1)){ // need to shift the limbs
      mp_limb_t shifted_bits=
        mpn_rshift(shifted->limbs,this->limbs+(bits/GMP_LIMB_BITS),abs(this->numberoflimbs)-
                   (bits/GMP_LIMB_BITS),
                   (bits)& (GMP_LIMB_BITS -1));
      change_rounding_p=(shifted_bits >> (GMP_LIMB_BITS - 1));
    }
    else {
      mpn_copyi(shifted->limbs,this->limbs+(bits/GMP_LIMB_BITS),abs(this->numberoflimbs)
                -(bits/GMP_LIMB_BITS));
      change_rounding_p=( (this->limbs[abs(this->numberoflimbs)-(bits/GMP_LIMB_BITS)])
                          >> (GMP_LIMB_BITS - 1));
        }
    if(this->numberoflimbs > 0)
      return shifted->normalize();
    else{ // arithmetic right shift rounds downwards for negative numbers
      shifted->negate_in_place();
      shifted->normalize();
      //std::cout << "change_rounding_p is " << change_rounding_p << "\n";
// Fix the assignment of change_rounding_p
      
      if(change_rounding_p)return shifted->_big_oneMinus();
      else return shifted; // already normalized
    }
  }
  else if (bits >0){ // left shift
    GC_ALLOCATE_VARIADIC(Bignum_O,shifted);
    shifted->realloc_limbs(abs(this->numberoflimbs)+(bits/GMP_LIMB_BITS)+1);
    if(bits & (GMP_LIMB_BITS-1)){
      shifted->limbs[abs(this->numberoflimbs)+(bits/GMP_LIMB_BITS)]=
        mpn_lshift(shifted->limbs+(bits/GMP_LIMB_BITS),this->limbs,abs(this->numberoflimbs),
                   bits & (GMP_LIMB_BITS -1));
    }
    else mpn_copyi(shifted->limbs+(bits/GMP_LIMB_BITS),this->limbs,abs(this->numberoflimbs));
    shifted->normalize();
    if(this->numberoflimbs < 0)shifted->negate_in_place();
    return shifted;
  }
  else return this->copy_();
}
Integer_sp Bignum_O::shift_(gc::Fixnum bits) const{
  return this->shift_big(bits)->maybe_as_fixnum();
}

string Bignum_O::__repr__() const {
  
  stringstream ss;
  unsigned char* rawcstring=(unsigned char*)malloc(abs(this->numberoflimbs)*GMP_LIMB_BITS* (int)ceil(log(2)/log(10)));
  mp_size_t stringlength=mpn_get_str(rawcstring,10,this->limbs,abs(this->numberoflimbs));
  if(this->numberoflimbs<0)ss << "-";
  for(int i=0;i<stringlength;i++){
    ss << (char)(rawcstring[i]+'0');
  }
  free(rawcstring);
  return ((ss.str()));
}

Number_sp Bignum_O::signum_() const {
  if (this->zerop_())
    return immediate_fixnum<Number_O>(0);
  else if (this->plusp_())
    return immediate_fixnum<Number_O>(1);
  else
    return immediate_fixnum<Number_O>(-1);
}
Number_sp Bignum_O::abs_() const {return this->abs_big_()->maybe_as_fixnum();}
Bignum_sp Bignum_O::abs_big_() const {
  GC_ALLOCATE_VARIADIC(Bignum_O,absolute);
  absolute->realloc_limbs(abs(this->numberoflimbs));
  mpn_copyi(absolute->limbs,this->limbs,absolute->numberoflimbs);
  return absolute; // normalise here?
}

bool Bignum_O::eql_(T_sp o) const {
  if(o.fixnump()){
    if(this->numberoflimbs==1)return this->limbs[0]==(mp_limb_t)o.unsafe_fixnum();
    if(this->numberoflimbs==-1)return this->limbs[0]==(mp_limb_t)(-o.unsafe_fixnum());
    if(this->numberoflimbs==0)return o.unsafe_fixnum()==0;
    return false;
  }
  if(Bignum_sp ny = o.asOrNull<Bignum_O>())
    return 0==Bignum_O::compare(this->asSmartPtr(),ny);
  else return false;
}

Integer_mv big_ceiling(Bignum_sp a, Bignum_sp b) {
  SIMPLE_ERROR(BF("implement big_ceiling"));
}

Integer_mv big_floor(Bignum_sp a, Bignum_sp b) {
  SIMPLE_ERROR(BF("implement big_floor"));
}



// int _clasp_compare_big(Bignum_sp a,Bignum_sp b){ // Returns positive if a<b, negative if a>b, 0 otherwise
//   if(a->numberoflimbs>0){ //a>=0
//     if(b->numberoflimbs>0){// a>=0,b>=0
//       bool result=false;
//       mp_limb_t* temp;
//       if(a->numberoflimbs<b->numberoflimbs){
//         temp=(mp_limb_t*)malloc(b->numberoflimbs*sizeof(mp_limb_t));
//         mpn_copyi(temp,a->limbs,a->numberoflimbs); //temp=a
//         result= (mpn_cmp(b->limbs,temp,b->numberoflimbs)); 
//       }
//       else{
//         temp=(mp_limb_t*)malloc(a->numberoflimbs*sizeof(mp_limb_t));
//         mpn_copyi(temp,b->limbs,b->numberoflimbs);//temp=b
//         result=(mpn_cmp(temp,a->limbs,a->numberoflimbs));
//       }
//       free(temp);
//       return result;
//     }
//     else{//a>=0,b<=0
//       return (a->zerop_() && b->zerop_())?0:-1; //a>b unless a=b=0
//     }
//   }
//   else{//a<=0
//     if(b->numberoflimbs>0){//a<=0,b>=0 
//       return (a->zerop_() && b->zerop_())?0:1; //a<b unless a=b=0
//     }
//     else{//a<=0,b<=0
//       bool result=false;
//       mp_limb_t* temp;
//       if(a->numberoflimbs<b->numberoflimbs){
//         temp=(mp_limb_t*)malloc(b->numberoflimbs*sizeof(mp_limb_t));
//         mpn_copyi(temp,a->limbs,a->numberoflimbs);//temp=|a|
//         result=(mpn_cmp(temp,b->limbs,b->numberoflimbs)); // a<b <==> |a|>|b|
//       }
//       else{
//         temp=(mp_limb_t*)malloc(a->numberoflimbs*sizeof(mp_limb_t));
//         mpn_copyi(temp,b->limbs,b->numberoflimbs);//temp=|b|
//         result=(mpn_cmp(a->limbs,temp,a->numberoflimbs)); // a<b <==> |a|<|b|
//       }
//       free(temp);
//       return result;
//     }
//   }
// }


void clasp_big_register_free(Bignum_sp b) {
  // ECL just returns but we
  // could clear out the bignum register if it's too big
  return;
}

Bignum_sp Bignum_O::magnitude_sum(Bignum_sp a,Bignum_sp b){
  //Returns a Bignum_sp |a| + |b|.
  GC_ALLOCATE(Bignum_O,result);
  if(abs(a->numberoflimbs)<abs(b->numberoflimbs)){
    Bignum_sp temp=a;
    a=b;
    b=temp;
  }
  result->realloc_limbs(abs(a->numberoflimbs)+1);
  mpn_copyi(result->limbs,b->limbs,abs(b->numberoflimbs));
  mpn_add(result->limbs,a->limbs,abs(a->numberoflimbs)+1,result->limbs,abs(b->numberoflimbs));
  return result->normalize();
}

Bignum_sp Bignum_O::magnitude_difference(Bignum_sp a, Bignum_sp b){
  //Returns a Bignum_sp | |b| - |a||
  a=a->abs_big_();
  b=b->abs_big_();
  if(Bignum_O::compare(a,b)>0){
    Bignum_sp temp=a;
    a=b;
    b=temp;
  }
  GC_ALLOCATE(Bignum_O,result);
  result->realloc_limbs(abs(a->numberoflimbs)+1);
  mpn_copyi(result->limbs,b->limbs,abs(b->numberoflimbs));
  mpn_sub(result->limbs,a->limbs,abs(a->numberoflimbs)+1,result->limbs,abs(b->numberoflimbs));
  return result->normalize();
}


Bignum_sp Bignum_O::magnitude_xnor(Bignum_sp a,Bignum_sp b){
  //Returns a Bignum_sp ~(|a| ^ |b|).
  GC_ALLOCATE(Bignum_O,result);
  if(abs(a->numberoflimbs)<abs(b->numberoflimbs)){
    Bignum_sp temp=a;
    a=b;
    b=temp;
  }
  result->realloc_limbs(a->numberoflimbs); 
  if(b->numberoflimbs==0)return Bignum_O::create((Fixnum)0);
  mpn_copyi(result->limbs,b->limbs,abs(b->numberoflimbs));
  mpn_xnor_n(result->limbs,a->limbs,result->limbs,abs(a->numberoflimbs));
  return result;
}

Bignum_sp Bignum_O::magnitude_nior(Bignum_sp a,Bignum_sp b){
  //Returns a Bignum_sp ~(|a| | |b|).
  GC_ALLOCATE(Bignum_O,result);
  if(abs(a->numberoflimbs)<abs(b->numberoflimbs)){
    Bignum_sp temp=a;
    a=b;
    b=temp;
  }
  result->realloc_limbs(a->numberoflimbs); 
  if(b->numberoflimbs==0)return Bignum_O::create((Fixnum)0);
  mpn_copyi(result->limbs,b->limbs,abs(b->numberoflimbs));
  mpn_nior_n(result->limbs,a->limbs,result->limbs,abs(a->numberoflimbs));
  return result;
}

Bignum_sp Bignum_O::divide(Bignum_sp n,Bignum_sp d){
  GC_ALLOCATE(Bignum_O,result);
  result->realloc_limbs(abs(n->numberoflimbs)-abs(d->numberoflimbs)+1); // check quotient rounding
  GC_ALLOCATE(Bignum_O,remainder);
  remainder->realloc_limbs(abs(d->numberoflimbs));
  mpn_tdiv_qr(result->limbs,remainder->limbs,0,n->limbs,abs(n->numberoflimbs),d->limbs,abs(d->numberoflimbs));
  if((n->numberoflimbs * d->numberoflimbs) < 0)result->numberoflimbs*=-1;
  return result->normalize();
}

int Bignum_O::compare(Bignum_sp a,Bignum_sp b){ // Returns positive if a<b, negative if a>b, 0 otherwise
  if(a->numberoflimbs<b->numberoflimbs)return -1;
  if(a->numberoflimbs>b->numberoflimbs)return 1;
  if(a->numberoflimbs==0)return 0; //they are both zero
  // have to do a comparison between two non-zero bignums of the same limb length
  int cmp_result=mpn_cmp(a->limbs,b->limbs,abs(a->numberoflimbs));
  return (a->numberoflimbs>0)?-cmp_result:cmp_result;
};

Bignum_sp Bignum_O::magnitude_and(Bignum_sp a,Bignum_sp b){
  //Returns a Bignum_sp |a| & |b|.
  GC_ALLOCATE(Bignum_O,result);
  if(abs(a->numberoflimbs)<abs(b->numberoflimbs)){
    Bignum_sp temp=a;
    a=b;
    b=temp;
  }
  result->realloc_limbs(a->numberoflimbs); 
  if(b->numberoflimbs==0)return Bignum_O::create((Fixnum)0);
  mpn_copyi(result->limbs,b->limbs,abs(b->numberoflimbs));
  mpn_and_n(result->limbs,a->limbs,result->limbs,abs(a->numberoflimbs));
  return result;
}

Bignum_sp Bignum_O::magnitude_ior(Bignum_sp a,Bignum_sp b){
  //Returns a Bignum_sp |a| | |b|.
  GC_ALLOCATE(Bignum_O,result);
  if(abs(a->numberoflimbs)<abs(b->numberoflimbs)){
    Bignum_sp temp=a;
    a=b;
    b=temp;
  }
  result->realloc_limbs(a->numberoflimbs); 
  if(b->numberoflimbs==0)return Bignum_O::create((Fixnum)0);
  mpn_copyi(result->limbs,b->limbs,abs(b->numberoflimbs));
  mpn_ior_n(result->limbs,a->limbs,result->limbs,abs(a->numberoflimbs));
  return result;
}

Bignum_sp Bignum_O::magnitude_xor(Bignum_sp a,Bignum_sp b){
  //Returns a Bignum_sp |a| ^ |b|.
  GC_ALLOCATE(Bignum_O,result);
  if(abs(a->numberoflimbs)<abs(b->numberoflimbs)){
    Bignum_sp temp=a;
    a=b;
    b=temp;
  }
  result->realloc_limbs(a->numberoflimbs); 
  if(b->numberoflimbs==0)return Bignum_O::create((Fixnum)0);
  mpn_copyi(result->limbs,b->limbs,abs(b->numberoflimbs));
  mpn_xor_n(result->limbs,a->limbs,result->limbs,abs(a->numberoflimbs));
  return result;
}

Bignum_sp Bignum_O::magnitude_andn(Bignum_sp a,Bignum_sp b){
  //Returns a Bignum_sp |a| & ~|b|.
  GC_ALLOCATE(Bignum_O,result);
  if(abs(a->numberoflimbs)<abs(b->numberoflimbs)){
    Bignum_sp temp=a;
    a=b;
    b=temp;
  }
  result->realloc_limbs(a->numberoflimbs); 
  if(b->numberoflimbs==0)return Bignum_O::create((Fixnum)0);
  mpn_copyi(result->limbs,b->limbs,abs(b->numberoflimbs));
  mpn_andn_n(result->limbs,a->limbs,result->limbs,abs(a->numberoflimbs));
  return result;
}

Bignum_sp Bignum_O::magnitude_iorn(Bignum_sp a,Bignum_sp b){
  //Returns a Bignum_sp |a| | ~|b|.
  GC_ALLOCATE(Bignum_O,result);
  if(abs(a->numberoflimbs)<abs(b->numberoflimbs)){
    Bignum_sp temp=a;
    a=b;
    b=temp;
  }
  result->realloc_limbs(a->numberoflimbs); 
  if(b->numberoflimbs==0)return Bignum_O::create((Fixnum)0);
  mpn_copyi(result->limbs,b->limbs,abs(b->numberoflimbs));
  mpn_iorn_n(result->limbs,a->limbs,result->limbs,abs(a->numberoflimbs));
  return result;
}

Bignum_sp Bignum_O::magnitude_nand(Bignum_sp a,Bignum_sp b){
  //Returns a Bignum_sp ~(|a| & |b|).
  GC_ALLOCATE(Bignum_O,result);
  if(abs(a->numberoflimbs)<abs(b->numberoflimbs)){
    Bignum_sp temp=a;
    a=b;
    b=temp;
  }
  result->realloc_limbs(a->numberoflimbs); 
  if(b->numberoflimbs==0)return Bignum_O::create((Fixnum)0);
  mpn_copyi(result->limbs,b->limbs,abs(b->numberoflimbs));
  mpn_nand_n(result->limbs,a->limbs,result->limbs,abs(a->numberoflimbs));
  return result;
}


Bignum_sp Bignum_O::gcd(Bignum_sp x, Bignum_sp y) {
  while(x->evenp_() && y->evenp_()){ // per mpn_gcd need to ensure that both x and y are not even.
    if(x->zerop_())return y->copy_();
    if(y->zerop_())return x->copy_();
    Integer_sp newx=x->shift_(-1);
    Integer_sp newy=y->shift_(-1);
    x=(newx.fixnump())?Bignum_O::create(newx.unsafe_fixnum()):gc::As<Bignum_sp>(newx);
    y=(newy.fixnump())?Bignum_O::create(newy.unsafe_fixnum()):gc::As<Bignum_sp>(newy);
    
  }
  if(abs(x->numberoflimbs) < abs(y->numberoflimbs)){
    Bignum_sp temp=x;
    x=y;
    y=temp;
  }
  mp_limb_t* copy_x=(mp_limb_t*)malloc(sizeof(mp_limb_t)*abs(x->numberoflimbs));
  mp_limb_t* copy_y=(mp_limb_t*)malloc(sizeof(mp_limb_t)*abs(x->numberoflimbs));
  mpn_copyi(copy_x,x->limbs,abs(x->numberoflimbs));
  mpn_copyi(copy_y,y->limbs,abs(y->numberoflimbs));
  GC_ALLOCATE(Bignum_O,result);
  result->realloc_limbs(y->numberoflimbs);
  result->numberoflimbs=
    mpn_gcd(result->limbs,copy_x,abs(x->numberoflimbs),copy_y,abs(y->numberoflimbs));
  free(copy_x);
  free(copy_y);
  return result->normalize();
}
Bignum_sp Bignum_O::product(Bignum_sp big_a, Bignum_sp big_b){
  GC_ALLOCATE_VARIADIC(Bignum_O,multiply_result);
  multiply_result->realloc_limbs(sgn(big_a->numberoflimbs) * big_b->numberoflimbs
                                 + sgn(big_b->numberoflimbs)*big_a->numberoflimbs);
  if(abs(big_a->numberoflimbs)<abs(big_b->numberoflimbs)){
    Bignum_sp temp = big_a;
    big_a=big_b;
    big_b=temp;
  }
  mpn_mul(multiply_result->limbs,
          big_a->limbs,abs(big_a->numberoflimbs),
          big_b->limbs,abs(big_b->numberoflimbs));
  return multiply_result->normalize();
}


};
