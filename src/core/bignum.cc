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
  return ((bn));
};

__attribute__((optnone)) Bignum_sp Bignum_O::create(gc::Fixnum i){
  GC_ALLOCATE(Bignum_O, b);
  b->numberoflimbs = ( i>0)? (sizeof(gc::Fixnum))/sizeof(mp_limb_t) : -(mp_size_t)((sizeof(gc::Fixnum))/sizeof(mp_limb_t));
  //std::cout << "b->numberoflimbs " << b->numberoflimbs << "\n";
  b->limbs = (mp_limb_t*)GC_MALLOC(abs(b->numberoflimbs)*sizeof(mp_limb_t));
  //b->debug_print();
  if(i<0){
    b->limbs[0]=(mp_limb_t)(-i);
  }
  else if (i>= 0){
    b->limbs[0]=(mp_limb_t)i;
  }// need to fix this to get types to deal with potential type mismatch
  //std::cout << "malloc size:" << abs(b->numberoflimbs)*sizeof(mp_limb_t) << "\n";
  //std::cout << "fixnum i was " << i << "\n";
  //std::cout << "in create(fixnum)\n" ;
  return ((b));
};

LongLongInt Bignum_O::as_LongLongInt_() const {
  SIMPLE_ERROR(BF("Implement as_LongLongInt_ for %s") % this->__repr__());
}

unsigned long long Bignum_O::as_unsigned_long_long_() const {
  if (sizeof(unsigned long long) == sizeof(uint64_t)) {
    return this->as_uint64_();
  }
  SIMPLE_ERROR(BF("Handle unsigned long long != uint64_t"));
  //	TYPE_ERROR(this->asSmartPtr(),Cons_O::createList(cl::_sym_Integer_O,make_fixnum(0),Integer_O::create(gc::most_positive_unsigned_long_long)));
}

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
  return (this->numberoflimbs<maxlimbs) && (this->numberoflimbs>-maxlimbs);
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
  if(numberoflimbs>0)return (unsigned long)(this->limbs[0]);
  else return -(unsigned long)this->limbs[0]; // will this work? it looks like we need little-endian
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
  
  SIMPLE_ERROR(BF("implement as_float_"));
}

double Bignum_O::as_double_() const {
  
  SIMPLE_ERROR(BF("implement as_double_"));
}

LongFloat Bignum_O::as_long_float_() const {
  
  SIMPLE_ERROR(BF("implement as_long_float_"));
}

// --- END OF TRANSLATION METHODS ---

 void Bignum_O::debug_print() {
  std::cout << this->numberoflimbs << " limbs\n";
  for(int i=0;i<numberoflimbs;i++){
    std::cout << "Limb " << i << ": " << this->limbs[i] << "\n";
  }
}

__attribute__((optnone)) void Bignum_O::setFromString(const string &value_in_string, unsigned int base) {
  this->numberoflimbs=(mp_size_t)ceil((log(base)/(log(2)*GMP_LIMB_BITS)) * value_in_string.length()); // doesn't deal with negative numbers yet!
  this->limbs=(mp_limb_t*)GC_MALLOC(this->numberoflimbs*sizeof(mp_limb_t));
  const char* c_string = value_in_string.c_str();
  unsigned char* c_out_string = (unsigned char*)malloc(sizeof(unsigned char)*value_in_string.length());
  for(int i=0;i<value_in_string.length();i++){
    c_out_string[i]=(unsigned char)(c_string[i]-'0'); // string not in ASCII. put a bounds check here?
  }  
  this->numberoflimbs=mpn_set_str(this->limbs,c_out_string,value_in_string.length(),base);
  //std::cout << "Bignum created from string: "+value_in_string + "\n";
  free(c_out_string);
}

gc::Fixnum Bignum_O::bit_length_() const {
  SIMPLE_ERROR(BF("implement bit_length"));
}

/*! Return the value shifted by BITS bits.
      If BITS < 0 shift right, if BITS >0 shift left. */
Integer_sp Bignum_O::shift_(gc::Fixnum bits) const {
  std::cout << "in bignum shift_()";
  return this->asSmartPtr();
}

string Bignum_O::__repr__() const {
  stringstream ss;
  unsigned char* rawcstring=(unsigned char*)malloc(abs(this->numberoflimbs));
  mp_size_t stringlength=mpn_get_str(rawcstring,10,this->limbs,this->numberoflimbs); // base 8 for now for testing purposes
  for(int i=0;i<stringlength;i++){
    rawcstring[i]+='0';
  }
  ss << rawcstring;
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

Number_sp Bignum_O::abs_() const {
  SIMPLE_ERROR(BF("implement abs_"));
}

bool Bignum_O::eql_(T_sp o) const {
  SIMPLE_ERROR(BF("implement eql_"));
}

Integer_mv big_ceiling(Bignum_sp a, Bignum_sp b) {
  SIMPLE_ERROR(BF("implement big_ceiling"));
}

Integer_mv big_floor(Bignum_sp a, Bignum_sp b) {
  SIMPLE_ERROR(BF("implement big_floor"));
}

Integer_sp _clasp_big_gcd(Bignum_sp x, Bignum_sp y) {
  SIMPLE_ERROR(BF("implement clasp_big_gcd"));
}


void clasp_big_register_free(Bignum_sp b) {
  // ECL just returns but we
  // could clear out the bignum register if it's too big
  return;
}


};
