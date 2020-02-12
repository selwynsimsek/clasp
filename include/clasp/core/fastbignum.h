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
#ifndef _core_fastbignum_H_
#define _core_fastbignum_H_

#include <clasp/core/clasp_gmpxx.h>
#include <clasp/core/object.h>
#include <clasp/core/numbers.h>

namespace core {
class FastBignum_O;
}
// Is this OK? Copied from slow bignums (selwyn feb '20)
template <>
struct gctools::GCInfo<core::FastBignum_O> {
  static bool constexpr CanAllocateWithNoArguments = true;
  static bool constexpr NeedsInitialization = false;
  static bool constexpr NeedsFinalization = false;
  static GCInfo_policy constexpr Policy = normal;
};

namespace core {
class FastBignum_O : public Integer_O {
  LISP_CLASS(core, ClPkg, FastBignum_O, "FastBignum",Integer_O);
  //    DECLARE_ARCHIVE();
public: // Simple default ctor/dtor
  DEFAULT_CTOR_DTOR(FastBignum_O);

public: // ctor/dtor for classes with shared virtual base
        //    explicit Bignum_O(core::Instance_sp const& mc) : T_O(mc), Integer(mc) {};
        //    virtual ~Bignum_O() {};
public:
  //	void initialize();

private: // instance variables here
  mp_limb_t *limbs;
  mp_size_t numberoflimbs;

public: // Functions here
  static FastBignum_sp make(const string &value_in_string);

  static FastBignum_sp create( gc::Fixnum i )
  {
    GC_ALLOCATE(FastBignum_O, b);
    //b->_value 
    std::cout << "FastBignum_sp create fixnum: i am not implemented yet!";
    return b;
  };

#if !defined( CLASP_FIXNUM_IS_INT64 )

  static FastBignum_sp create( int64_t v )
  {
    GC_ALLOCATE(FastBignum_O, b);
    return b;
  };

#endif

  static FastBignum_sp create( uint64_t v )
  {
    GC_ALLOCATE(FastBignum_O, b);
    return b;
  };

#if !defined( CLASP_LONG_LONG_IS_INT64 )

  static FastBignum_sp create( long long v )
  {
    GC_ALLOCATE(FastBignum_O, b);
    return b;
  };

#endif

#if !defined( CLASP_UNSIGNED_LONG_LONG_IS_UINT64 )

  static FastBignum_sp create( unsigned long long v )
  {
    GC_ALLOCATE(FastBignum_O, b);
    return b;
  };

#endif

 public:

  NumberType number_type_() const { return number_Bignum; };
  
  string __repr__() const;

  Number_sp signum_() const;

  /*! Return true if the number fits in a signed int */
  bool fits_sint_p();

  virtual void increment() {  };
  virtual void decrement() {  };
  //virtual Number_sp copy() const;
  string description() const {
    stringstream ss;
    //ss << this->_value;
    return ss.str();
  };
  void set(gc::Fixnum val) {  };
  void setFixnum(gctools::Fixnum val) { };
  //FastBignum get() const;
  //FastBignum get_or_if_nil_default(FastBignum default_value) const;
  Number_sp abs_() const;
  //Number_sp sqrt_() const;
  //Number_sp reciprocal_() const;
  Number_sp rational_() const final { return this->asSmartPtr(); };
  void increment(gc::Fixnum i) {  };
  int sign() const { return 0; };

  virtual bool zerop_() const { return false; }
  virtual bool plusp_() const { return false; }
  virtual bool minusp_() const { return false; }

  virtual Number_sp negate_() const {
    return Integer_O::create(0);
  }

  virtual Number_sp oneMinus_() const {
    return Integer_O::create(0);
  }
  virtual Number_sp onePlus_() const {
    return Integer_O::create(0);
  }

  virtual gc::Fixnum bit_length_() const;

  /*! Return the value shifted by BITS bits.
	  If BITS < 0 shift right, if BITS >0 shift left. */
  Integer_sp shift_(gc::Fixnum bits) const;

  //	virtual	bool	eqn(T_sp obj) const;
  virtual bool eql_(T_sp obj) const;

 public:
  virtual string valueAsString() const {
    stringstream ss;
    //ss << this->_value;
    return ss.str();
  };
  virtual void setFromString(const string &strVal);

  // --- TRANSLATION METHODS ---

  // virtual short as_short() const;
  // virtual unsigned short as_ushort() const;

  // virtual int as_int() const;
  // virtual unsigned int as_uint() const;

  // virtual long as_long() const;
  // virtual unsigned long as_ulong() const;

  // virtual long long as_longlong() const;
  // virtual unsigned long long as_ulonglong() const;

  // virtual int8_t as_int8_t() const;
  // virtual uint8_t as_uint8_t() const;

  // virtual int16_t as_int16_t() const;
  // virtual uint16_t as_uint16_t() const;

  // virtual int32_t as_int32_t() const;
  // virtual uint32_t as_uint32_t() const;

  // virtual int64_t as_int64_t() const;
  // virtual uint64_t as_uint64_t() const;

  // virtual uintptr_t as_uintptr_t() const;
  // virtual ptrdiff_t as_ptrdiff_t() const;
  // virtual size_t as_size_t() const;
  // virtual ssize_t as_ssize_t() const;

  // --- THESE FUNCTIONS RETAINED FOR COMPATIBILITY ---
  // TODO: Code Cleanup: Replace with newer translation functions above
  // frgo, 2016-09-06

  virtual gc::Fixnum as_int_() const;
  virtual int64_t as_int64_() const;
  virtual uint64_t as_uint64_() const;
  string as_uint64_string() const;

  //virtual Bignum as_mpz_() const;
  virtual LongLongInt as_LongLongInt_() const;
  virtual unsigned long long as_unsigned_long_long_() const;
  //virtual float as_float_() const;
  //virtual double as_double_() const;
  //virtual LongFloat as_long_float_() const;

  // --- END OF TRANSLATION METHODS ---

  void sxhash_(HashGenerator &hg) const;

  virtual bool evenp_() const { return 0; };
  virtual bool oddp_() const { return 0; };

  Number_sp log1() const;

}; // Bignum class

}; // core namespace

// namespace translate {
//   template <>
//     struct from_object<const FastBignum &, std::true_type> {
//     typedef FastBignum DeclareType;
//     DeclareType _v;
//     from_object(core::T_sp o) {
//       _G();
//       if (core::FastBignum_sp bn = o.asOrNull<core::FastBignum_O>()) {
//         _v = bn->ref();
//         ;
//         return;
//       }
//       SIMPLE_ERROR_SPRINTF("Handle conversions of %s to Bignum", _rep_(o).c_str());
//     }
//   };
// };

namespace core {

  Integer_mv fast_big_ceiling(FastBignum_sp a, FastBignum_sp b);
  Integer_mv fast_big_floor(FastBignum_sp a, FastBignum_sp b);

  inline Integer_sp _clasp_fast_big_register_normalize(FastBignum_sp x) {
    return Integer_O::create(0);
  }

  inline Integer_sp _clasp_fast_big_floor(FastBignum_sp a, FastBignum_sp b, Real_sp *rP) {
    Integer_mv res_mv = fast_big_floor(a, b);
    *rP = gc::As<Real_sp>(res_mv.valueGet_(1));
    return res_mv;
  };

  inline Integer_sp _clasp_big_ceiling(FastBignum_sp a, FastBignum_sp b, Real_sp *rP) {
    Integer_mv res_mv = fast_big_ceiling(a, b);
    *rP = gc::As<Real_sp>(res_mv.valueGet_(1));
    return res_mv;
  }

  inline double _clasp_fast_big_to_double(FastBignum_sp a) {
    //return a->as_double_();
    return 0.0;
  }

  void clasp_fast_big_register_free(FastBignum_sp x);

  //Integer_sp _clasp_fix_divided_by_fast_big(const Fixnum &x, const FastBignum &y);
  //Integer_sp _clasp_fast_big_divided_by_fix(const FastBignum &x, const Fixnum &y);
  //Integer_sp _clasp_fast_big_divided_by_fast_big(const FastBignum &x, const FastBignum &y);

  Integer_sp _clasp_fast_big_gcd(FastBignum_sp x, FastBignum_sp y);

#define CLASP_BIGNUM_SIZE(x) ((x)->_mp_size)
#define CLASP_BIGNUM_ABS_SIZE(x) \
  (CLASP_BIGNUM_SIZE(x) < 0 ? -CLASP_BIGNUM_SIZE(x) : CLASP_BIGNUM_SIZE(x))

  /*! Parse a cstring to a Bignum */
   //FastBignum CStrToFastBignum(const char* c);

 };

#endif /* _bignum_H_ */
