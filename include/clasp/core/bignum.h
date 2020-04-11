/*
    File: bignum.h
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
#ifndef _core_bignum_H_
#define _core_bignum_H_

#include <clasp/core/clasp_gmpxx.h>
#include <clasp/core/object.h>
#include <clasp/core/numbers.h>
#include <clasp/core/bignum.fwd.h>

namespace core {
class Bignum_O;
}

template <>
struct gctools::GCInfo<core::Bignum_O> {
  static bool constexpr CanAllocateWithNoArguments = true;
  static bool constexpr NeedsInitialization = false;
  static bool constexpr NeedsFinalization = false; // finalizer no longer needed since we are switching to the low-level GMP interface
  static GCInfo_policy constexpr Policy = normal;
};

namespace core {
SMART(Bignum);
class Bignum_O : public Integer_O {
  LISP_CLASS(core, ClPkg, Bignum_O, "Bignum",Integer_O);
  //    DECLARE_ARCHIVE();
public: // Simple default ctor/dtor
  DEFAULT_CTOR_DTOR(Bignum_O);

public: // ctor/dtor for classes with shared virtual base
  //	void initialize();

public: // instance variables here
  // =====
  // Fast Bignum specification: (selwyn 6 apr 2020)
  // The low-level GMP routines beginning mpn_ (n for natural number) are written with natural numbers (positive non-zero integers) in mind. Per the GMP
  // manual, the interfaces are not necessarily consistent, and in particular, no method can be assumed to work with negative numbers, since there is no
  //  explicit specification of them.

  // We here define a negative number S to have minus the numberoflimbs of the positive integer |S| and the limbs of |S|, i.e. the magnitude. This choice
  // is consistent with that of mpn_gcdext, for example, but care should be taken not to use the low level routines without first checking to see if they
  // can be used with negative arguments. This choice is similar to that made by the higher level GMP implementation

  // https://gmplib.org/manual/Integer-Internals.html

  // If numberoflimbs=/=0, limbs[|numberoflimbs|-1] may not be 0, and 0 is represented by having numberoflimbs=0.
  // This ensures uniqueness of representation. All returned bignums and bignums passed as arguments must satisfy this.
  // maybe_as_fixnum() should be called on bignums being returned as results of computations.
  // c.f. ecl:src/c/big.d for comparison.
  // ====
  
  mp_limb_t *limbs;
  mp_size_t numberoflimbs; 

public: // Functions here
  static Bignum_sp make(const string &value_in_string, unsigned int base=10);



#if !defined( CLASP_FIXNUM_IS_INT64 )

  static Bignum_sp create( int64_t v )
  {
    GC_ALLOCATE_VARIADIC(Bignum_O, b);
    b->set_to_signed_long_int((signed long int) v);
    return b;
  };

#endif

  static Bignum_sp create( uint64_t v )
  {
    GC_ALLOCATE_VARIADIC(Bignum_O, b);
    b->set_to_unsigned_long_int((unsigned long int) v);
    return b;
  };

  static Bignum_sp as_bignum(Integer_sp integer){
    if(integer.fixnump()){
      GC_ALLOCATE(Bignum_O,result);
      result->set_to_fixnum(integer.unsafe_fixnum());
      return result;
    }
    else return gc::As_unsafe<Bignum_sp>(integer);
  };

#if !defined( CLASP_LONG_LONG_IS_INT64 )

  static Bignum_sp create( long long v )
  {
    GC_ALLOCATE(Bignum_O, b);
#ifdef _TARGET_OS_DARWIN
    //b->_value = (long long)v;
#else
    //b->_value = (int64_t)v;
#endif
    SIMPLE_ERROR(BF("implement create (long long)"));
    return b;
  };

  static Bignum_sp create(gc::Fixnum b);

#endif

#if !defined( CLASP_UNSIGNED_LONG_LONG_IS_UINT64 )

  static Bignum_sp create( unsigned long long v )
  {
    GC_ALLOCATE(Bignum_O, b);
#ifdef _TARGET_OS_DARWIN
    //b->_value = (unsigned long long)v;
#else
    //b->_value = (uint64_t)v;
#endif
    return b;
  };

#endif

 public:

  NumberType number_type_() const { return number_Bignum; };

  string __repr__() const;

  Integer_sp maybe_as_fixnum();

  Number_sp signum_() const;

  virtual void debug_print() const;

  /*! Return true if the number fits in a signed int */
  bool fits_sint_p();

  void set_to_signed_long_int(signed long int i) {
    SIMPLE_ERROR(BF("implement set_to_signed_long_int"));} ;
  void set_to_fixnum(gc::Fixnum i) {
    this->numberoflimbs = ( i>0)? (sizeof(gc::Fixnum))/sizeof(mp_limb_t) :
      -(sizeof(gc::Fixnum))/sizeof(mp_limb_t);
    this->limbs = (mp_limb_t*)GC_MALLOC(abs(this->numberoflimbs)*sizeof(mp_limb_t));
    if(i<0){
      this->limbs[0]=(mp_limb_t)(-i);
    }
    else{
      this->limbs[0]=(mp_limb_t)i;
    } // need to fix this
    } ;
  inline Bignum_sp normalize(){ // Ensure that |numberoflimbs| is not too large.
    if(this->numberoflimbs>0){
      while(this->limbs[numberoflimbs-1]==0 && this->numberoflimbs>0)
        numberoflimbs--;
    }
    else if(this->numberoflimbs<0){
      while(this->limbs[-numberoflimbs-1]==0 && this->numberoflimbs<0)
        numberoflimbs++;
    }
    return this->asSmartPtr();
  };
  void set_to_unsigned_long_int(unsigned long int i) {
    this->realloc_limbs(1);
    this->limbs[0]=i;
  };
  

  virtual void increment() {
    Bignum_sp incf=this->onePlus_();
    this->realloc_limbs(incf->numberoflimbs);
    mpn_copyi(this->limbs,incf->limbs,abs(incf->numberoflimbs));
  };
  virtual void decrement() {
    Bignum_sp decf=this->oneMinus_();
    this->realloc_limbs(decf->numberoflimbs);
    mpn_copyi(this->limbs,decf->limbs,abs(decf->numberoflimbs));
  };
  //virtual Number_sp copy() const;
  string description() const {
    stringstream ss;
    ss << this->__repr__(); // good use of __repr__ ?
    return ss.str();
  };
  void set(gc::Fixnum val) ;
  void setFixnum(gctools::Fixnum val) {
    this->set_to_fixnum(val);};
  Number_sp abs_() const;
  Number_sp sqrt_() const;
  Number_sp reciprocal_() const;
  Number_sp rational_() const final { return this->asSmartPtr(); };
  void increment(gc::Fixnum i) ;
  int sign() const {
    if(this->numberoflimbs==0)return 0;
    return ((this->numberoflimbs) > 0) ? 1 : -1; };

  void realloc_limbs(int n){
    this->numberoflimbs=n;
    this->limbs=(mp_limb_t*)GC_MALLOC((1+abs(n))*sizeof(mp_limb_t)); // We allocate one more limb in order not to copy again when adding or subtracting.
  }
  Number_sp copy_() const
  {
    GC_ALLOCATE_VARIADIC(Bignum_O,copy);
    copy->realloc_limbs(this->numberoflimbs);
    mpn_copyi(copy->limbs,this->limbs,abs(this->numberoflimbs));
    return copy->normalize();
  };

  virtual bool zerop_() const {
    return (this->numberoflimbs==0); } 
  virtual bool plusp_() const { return this->numberoflimbs>0; }
  virtual bool minusp_() const { return this->numberoflimbs<0; }

  virtual Number_sp negate_() const
  {
    GC_ALLOCATE_VARIADIC(Bignum_O,negated);
    negated->realloc_limbs(-this->numberoflimbs);
    mpn_copyi(negated->limbs,this->limbs,abs(this->numberoflimbs));
    return negated;
  };
  

  virtual Number_sp oneMinus_() {
    GC_ALLOCATE_VARIADIC(Bignum_O,decremented);
    if(this->numberoflimbs>0){
      decremented->realloc_limbs(this->numberoflimbs);
      if(mpn_sub_1(decremented->limbs,this->limbs,this->numberoflimbs,1))//we had to borrow, so 
        return Bignum_O::create((Fixnum) -1); //we must actually have -1.
      else return decremented->normalize();
    }
    else if(this->numberoflimbs<0){
      decremented->realloc_limbs(this->numberoflimbs);
      if(mpn_add_1(decremented->limbs,this->limbs,-this->numberoflimbs,1)){
        decremented->realloc_limbs(this->numberoflimbs-1);
        mpn_zero(decremented->limbs,-decremented->numberoflimbs);
        decremented->limbs[-decremented->numberoflimbs-1]=1;
        return decremented;
      }
      else return decremented->normalize();
    }
    else return Bignum_O::create((Fixnum) -1);
  };
  virtual Number_sp onePlus_() {
    GC_ALLOCATE_VARIADIC(Bignum_O,incremented);
    if(this->numberoflimbs>0){ //positive
      incremented->realloc_limbs(this->numberoflimbs);
      if(mpn_add_1(incremented->limbs,this->limbs,this->numberoflimbs,1)){
        //we overflowed, so need to reallocate. this way is slightly wasteful
        incremented->realloc_limbs(this->numberoflimbs+1);
        mpn_zero(incremented->limbs,incremented->numberoflimbs);
        incremented->limbs[incremented->numberoflimbs-1]=1;
        return incremented;
      }
      else return incremented->normalize();
    }
    else if(this->numberoflimbs<0){ //negative
      //We probably won't have to reallocate
      incremented->realloc_limbs(-this->numberoflimbs);
      if(mpn_sub_1(incremented->limbs,this->limbs,abs(this->numberoflimbs),1)){
        // we had to borrow, so we are actually zero and the result must be +1. very rare(?)
        return Bignum_O::create((Fixnum) 1);
      }
      else return incremented->normalize();
    }
    return Bignum_O::create((Fixnum) 1);
  };

  virtual gc::Fixnum bit_length_() const;


  //	virtual	bool	eqn(T_sp obj) const;
  virtual bool eql_(T_sp obj) const;

 public:
  
  /*! Return the value shifted by BITS bits.
	  If BITS < 0 shift right, if BITS >0 shift left. */
  virtual Integer_sp shift_(gc::Fixnum bits) const ;
  
  virtual string valueAsString() const {
    stringstream ss;
    ss << this->__repr__();
    return ss.str();
  };
  virtual void setFromString(const string &strValue, unsigned int base=10);

  // --- TRANSLATION METHODS ---

  virtual short as_short() const;
  virtual unsigned short as_ushort() const;

  virtual int as_int() const;
  virtual unsigned int as_uint() const;

  virtual long as_long() const;
  virtual unsigned long as_ulong() const;

  virtual long long as_longlong() const;
  virtual unsigned long long as_ulonglong() const;

  virtual int8_t as_int8_t() const;
  virtual uint8_t as_uint8_t() const;

  virtual int16_t as_int16_t() const;
  virtual uint16_t as_uint16_t() const;

  virtual int32_t as_int32_t() const;
  virtual uint32_t as_uint32_t() const;

  virtual int64_t as_int64_t() const;
  virtual uint64_t as_uint64_t() const;

  virtual uintptr_t as_uintptr_t() const;
  virtual ptrdiff_t as_ptrdiff_t() const;
  virtual size_t as_size_t() const;
  virtual ssize_t as_ssize_t() const;

  // --- THESE FUNCTIONS RETAINED FOR COMPATIBILITY ---
  // TODO: Code Cleanup: Replace with newer translation functions above
  // frgo, 2016-09-06

  virtual gc::Fixnum as_int_() const;
  virtual int64_t as_int64_() const;
  virtual uint64_t as_uint64_() const;
  string as_uint64_string() const;

  virtual LongLongInt as_LongLongInt_() const;
  virtual unsigned long long as_unsigned_long_long_() const;
  virtual float as_float_() const;
  virtual double as_double_() const;
  virtual LongFloat as_long_float_() const;

  // --- END OF TRANSLATION METHODS ---

  void sxhash_(HashGenerator &hg) const;

  virtual bool evenp_() const {
    return this->numberoflimbs==0 || !(1 & this->limbs[0]);
  };
  virtual bool oddp_() const {
    return (this->numberoflimbs!=0) && ( 1 & this->limbs[0]);
  };

  Number_sp log1() const;

}; // Bignum class

}; // core namespace

 // we probably don't want a type translator for the Bignum class if it is no longer being used..

namespace core {

  Integer_mv big_ceiling(Bignum_sp a, Bignum_sp b);
  Integer_mv big_floor(Bignum_sp a, Bignum_sp b);
  Integer_sp _clasp_big_register_normalize(Bignum_sp x);

  inline Integer_sp _clasp_big_floor(Bignum_sp a, Bignum_sp b, Real_sp *rP) {
    Integer_mv res_mv = big_floor(a, b);
    *rP = gc::As<Real_sp>(res_mv.valueGet_(1));
    return res_mv;
  };

  inline Integer_sp _clasp_big_ceiling(Bignum_sp a, Bignum_sp b, Real_sp *rP) {
    Integer_mv res_mv = big_ceiling(a, b);
    *rP = gc::As<Real_sp>(res_mv.valueGet_(1));
    return res_mv;
  }

  inline double _clasp_big_to_double(Bignum_sp a) {
    return a->as_double_();
  }

inline Integer_sp fixnum_to_bignum(Fixnum fixnum){
  return Bignum_O::create(fixnum);
}

  void clasp_big_register_free(Bignum_sp x);

  //Integer_sp _clasp_fix_divided_by_big(const Fixnum &x, const Bignum &y);
  //Integer_sp _clasp_big_divided_by_fix(const Bignum &x, const Fixnum &y);
  //Integer_sp _clasp_big_divided_by_big(const Bignum &x, const Bignum &y);

  Integer_sp _clasp_big_gcd(Bignum_sp x, Bignum_sp y);

inline Integer_sp _clasp_big_divided_by_big(Bignum_sp n,Bignum_sp d){
  n->normalize();
  d->normalize();
  GC_ALLOCATE(Bignum_O,result);
  result->realloc_limbs(abs(n->numberoflimbs)-abs(d->numberoflimbs)+1); // check quotient rounding
  GC_ALLOCATE(Bignum_O,remainder);
  remainder->realloc_limbs(abs(d->numberoflimbs));
  mpn_tdiv_qr(result->limbs,remainder->limbs,0,n->limbs,abs(n->numberoflimbs),d->limbs,abs(d->numberoflimbs));
  if((n->numberoflimbs * d->numberoflimbs) < 0)result->numberoflimbs*=-1;
  return result->normalize();
}

inline int _clasp_compare_big(Bignum_sp a,Bignum_sp b){ // Returns positive if a<b, negative if a>b, 0 otherwise
  if(a->numberoflimbs<b->numberoflimbs)return -1;
  if(a->numberoflimbs>b->numberoflimbs)return 1;
  if(a->numberoflimbs==0)return 0; //they are both zero
  // have to do a comparison between two non-zero bignums of the same limb length
  int cmp_result=mpn_cmp(a->limbs,b->limbs,abs(a->numberoflimbs));
  return (a->numberoflimbs>0)?-cmp_result:cmp_result;
};

inline Bignum_sp _clasp_big_and_magnitude(Bignum_sp a,Bignum_sp b){
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

inline Bignum_sp _clasp_big_ior_magnitude(Bignum_sp a,Bignum_sp b){
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

inline Bignum_sp _clasp_big_xor_magnitude(Bignum_sp a,Bignum_sp b){
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

inline Bignum_sp _clasp_big_andn_magnitude(Bignum_sp a,Bignum_sp b){
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

inline Bignum_sp _clasp_big_iorn_magnitude(Bignum_sp a,Bignum_sp b){
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

inline Bignum_sp _clasp_big_nand_magnitude(Bignum_sp a,Bignum_sp b){
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

inline Bignum_sp _clasp_big_nior_magnitude(Bignum_sp a,Bignum_sp b){
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

inline Bignum_sp _clasp_big_xnor_magnitude(Bignum_sp a,Bignum_sp b){
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

inline Number_sp _clasp_maybe_as_fixnum(Bignum_sp num){
  if(num.fixnump())return immediate_fixnum<Number_sp>(num.unsafe_fixnum());
  return num;
}

inline Bignum_sp _clasp_big_add_magnitude(Bignum_sp a,Bignum_sp b){
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

inline Bignum_sp _clasp_big_difference_magnitude(Bignum_sp a, Bignum_sp b){
  //Returns a Bignum_sp | |b| - |a||
  a=a->abs_();
  b=b->abs_();
  if(_clasp_compare_big(a,b)>0){
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




//#define CLASP_BIGNUM_SIZE(x) ((x)->_mp_size)
//#define CLASP_BIGNUM_ABS_SIZE(x) \
//  (CLASP_BIGNUM_SIZE(x) < 0 ? -CLASP_BIGNUM_SIZE(x) : CLASP_BIGNUM_SIZE(x))

  /*! Parse a cstring to a Bignum */
   //Bignum CStrToBignum(const char* c);
};

#endif /* _bignum_H_ */
