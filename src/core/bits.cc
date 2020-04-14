#include <clasp/core/foundation.h>
#include <clasp/core/corePackage.h>
#include <clasp/core/symbolTable.h>
#include <clasp/core/bformat.h>
#include <clasp/core/array.h>
#include <clasp/core/bits.h>
#include <clasp/core/wrappers.h>
#include <clasp/core/bignum.h>

/* -*- mode: c; c-basic-offset: 8 -*- */
/*
    bits.cc  -- Logical operations on numbers.
If you're looking for bit array stuff, try array_bit.cc.
*/
/*
    Copyright (c) 1984, Taiichi Yuasa and Masami Hagiya.
    Copyright (c) 1990, Giuseppe Attardi.
    Copyright (c) 2001, Juan Jose Garcia Ripoll.

    ECL is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    See file '../Copyright' for full details.
*/

namespace core {

/*
 * BIT OPERATIONS FOR FIXNUMS
 */

static gctools::Fixnum
ior_op(gctools::Fixnum i, gctools::Fixnum j) {
  return (i | j);
}

static gctools::Fixnum
xor_op(gctools::Fixnum i, gctools::Fixnum j) {
  return (i ^ j);
}

static gctools::Fixnum
and_op(gctools::Fixnum i, gctools::Fixnum j) {
  return (i & j);
}

static gctools::Fixnum
eqv_op(gctools::Fixnum i, gctools::Fixnum j) {
  return (~(i ^ j));
}

static gctools::Fixnum
nand_op(gctools::Fixnum i, gctools::Fixnum j) {
  return (~(i & j));
}

static gctools::Fixnum
nor_op(gctools::Fixnum i, gctools::Fixnum j) {
  return (~(i | j));
}

static gctools::Fixnum
andc1_op(gctools::Fixnum i, gctools::Fixnum j) {
  return ((~i) & j);
}

static gctools::Fixnum
andc2_op(gctools::Fixnum i, gctools::Fixnum j) {
  return (i & (~j));
}

static gctools::Fixnum
orc1_op(gctools::Fixnum i, gctools::Fixnum j) {
  return ((~i) | j);
}

static gctools::Fixnum
orc2_op(gctools::Fixnum i, gctools::Fixnum j) {
  return (i | (~j));
}

static gctools::Fixnum
b_clr_op(gctools::Fixnum i, gctools::Fixnum j) {
  return (0);
}

static gctools::Fixnum
b_set_op(gctools::Fixnum i, gctools::Fixnum j) {
  return (-1);
}

static gctools::Fixnum
b_1_op(gctools::Fixnum i, gctools::Fixnum j) {
  return (i);
}

static gctools::Fixnum
b_2_op(gctools::Fixnum i, gctools::Fixnum j) {
  return (j);
}

static gctools::Fixnum
b_c1_op(gctools::Fixnum i, gctools::Fixnum j) {
  return (~i);
}

static gctools::Fixnum
b_c2_op(gctools::Fixnum i, gctools::Fixnum j) {
  return (~j);
}

typedef gctools::Fixnum (*bit_operator)(gctools::Fixnum, gctools::Fixnum);

static bit_operator fixnum_operations[boolOpsMax] = {
    b_clr_op,
    and_op,
    andc2_op,
    b_1_op,
    andc1_op,
    b_2_op,
    xor_op,
    ior_op,
    nor_op,
    eqv_op,
    b_c2_op,
    orc2_op,
    b_c1_op,
    orc1_op,
    nand_op,
    b_set_op};

// ----------------------------------------------------------------------

static void
mpz_ior_op(Bignum_sp out, Bignum_sp i, Bignum_sp j) {
  //mpz_ior(out->get().get_mpz_t(), i->get().get_mpz_t(), j->get().get_mpz_t());
  SIMPLE_ERROR(BF("implement mpz_ior_op for bignums"));
}

static void
mpz_xor_op(Bignum_sp out, Bignum_sp i, Bignum_sp j) {
  //mpz_xor(out->get().get_mpz_t(), i->get().get_mpz_t(), j->get().get_mpz_t());
  SIMPLE_ERROR(BF("implement mpz_xor_op for bignums"));
}

static void
mpz_and_op(Bignum_sp out, Bignum_sp i, Bignum_sp j) {
  //mpz_and(out->get().get_mpz_t(), i->get().get_mpz_t(), j->get().get_mpz_t());
  SIMPLE_ERROR(BF("implement mpz_and_op for bignums"));
}

static void
mpz_eqv_op(Bignum_sp out, Bignum_sp i, Bignum_sp j) {
  //mpz_xor(out->get().get_mpz_t(), i->get().get_mpz_t(), j->get().get_mpz_t());
  //mpz_com(out->get().get_mpz_t(), out->get().get_mpz_t());
  SIMPLE_ERROR(BF("implement mpz_eqv_op for bignums"));
}

static void
mpz_nand_op(Bignum_sp out, Bignum_sp i, Bignum_sp j) {
 // mpz_and(out->get().get_mpz_t(), i->get().get_mpz_t(), j->get().get_mpz_t());
  //mpz_com(out->get().get_mpz_t(), out->get().get_mpz_t());
  SIMPLE_ERROR(BF("implement mpz_nand_op for bignums"));
}

static void
mpz_nor_op(Bignum_sp out, Bignum_sp i, Bignum_sp j) {
  //mpz_ior(out->get().get_mpz_t(), i->get().get_mpz_t(), j->get().get_mpz_t());
  //mpz_com(out->get().get_mpz_t(), out->get().get_mpz_t());
  SIMPLE_ERROR(BF("implement mpz_nor_op for bignums"));
}

static void
mpz_andc1_op(Bignum_sp out, Bignum_sp i, Bignum_sp j) {
  //mpz_com(out->get().get_mpz_t(), i->get().get_mpz_t());
  //mpz_and(out->get().get_mpz_t(), out->get().get_mpz_t(), j->get().get_mpz_t());  
  SIMPLE_ERROR(BF("implement mpz_andc1_op for bignums"));
}

static void
mpz_orc1_op(Bignum_sp out, Bignum_sp i, Bignum_sp j) {
  //mpz_com(out->get().get_mpz_t(), i->get().get_mpz_t());
  //mpz_ior(out->get().get_mpz_t(), out->get().get_mpz_t(), j->get().get_mpz_t());
  SIMPLE_ERROR(BF("implement mpz_orc1_op for bignums"));
}

static void
mpz_andc2_op(Bignum_sp out, Bignum_sp i, Bignum_sp j) {
  /* (i & ~j) = ~((~i) | j) */
  //mpz_orc1_op(out, i, j);
  //mpz_com(out->get().get_mpz_t(), out->get().get_mpz_t());
  SIMPLE_ERROR(BF("implement mpz_andc2_op for bignums"));
}

static void
mpz_orc2_op(Bignum_sp out, Bignum_sp i, Bignum_sp j) {
  /* (i | ~j) = ~((~i) & j) */
  //mpz_andc1_op(out, i, j);
  //mpz_com(out->get().get_mpz_t(), out->get().get_mpz_t());
  SIMPLE_ERROR(BF("implement mpz_orc2_op for bignums"));
}

static void
mpz_b_clr_op(Bignum_sp out, Bignum_sp i, Bignum_sp j) {
  //mpz_set_si(out->get().get_mpz_t(), 0);
  SIMPLE_ERROR(BF("implement mpz_b_clr_op for bignums"));
}

static void
mpz_b_set_op(Bignum_sp o, Bignum_sp i, Bignum_sp j) {
  //mpz_set_si(o->get().get_mpz_t(), -1);
  SIMPLE_ERROR(BF("implement mpz_b_set_op for bignums"));
}

static void
mpz_b_1_op(Bignum_sp out, Bignum_sp i, Bignum_sp j) {
  if (i != out)
    SIMPLE_ERROR(BF("implement mpz_b_1_op for bignums"));
    //mpz_set(out->get().get_mpz_t(), i->get().get_mpz_t());
}

static void
mpz_b_2_op(Bignum_sp out, Bignum_sp i, Bignum_sp j) {
  //mpz_set(out->get().get_mpz_t(), j->get().get_mpz_t());
  SIMPLE_ERROR(BF("implement mpz_b_2_op for bignums"));
}

static void
mpz_b_c1_op(Bignum_sp out, Bignum_sp i, Bignum_sp j) {
  //mpz_com(out->get().get_mpz_t(), i->get().get_mpz_t());
  SIMPLE_ERROR(BF("implement mpz_b_c1_op for bignums"));
}

static void
mpz_b_c2_op(Bignum_sp out, Bignum_sp i, Bignum_sp j) {
  //mpz_com(out->get().get_mpz_t(), j->get().get_mpz_t());
  SIMPLE_ERROR(BF("implement mpz_b_c2_op for bignums"));
}

typedef void (*_clasp_big_binary_op)(Bignum_sp out, Bignum_sp o1, Bignum_sp o2);

static _clasp_big_binary_op bignum_operations[boolOpsMax] = {
    mpz_b_clr_op,
    mpz_and_op,
    mpz_andc2_op,
    mpz_b_1_op,
    mpz_andc1_op,
    mpz_b_2_op,
    mpz_xor_op,
    mpz_ior_op,
    mpz_nor_op,
    mpz_eqv_op,
    mpz_b_c2_op,
    mpz_orc2_op,
    mpz_b_c1_op,
    mpz_orc1_op,
    mpz_nand_op,
    mpz_b_set_op};

// ----------------------------------------------------------------------

T_sp clasp_boole(int op, T_sp x, T_sp y) {
  if (x.nilp())
    ERROR_WRONG_TYPE_NTH_ARG(cl::_sym_boole, 2, x, cl::_sym_integer);
  else if (y.nilp())
    ERROR_WRONG_TYPE_NTH_ARG(cl::_sym_boole, 3, y, cl::_sym_integer);
  if ((op < 0) || (op >= boolOpsMax))
    // issue #438
    ERROR_WRONG_TYPE_NTH_ARG(cl::_sym_boole, 1, make_fixnum(op), Cons_O::createList(cl::_sym_Integer_O, make_fixnum(0), make_fixnum(boolOpsMax-1)));
  if (x.fixnump()) {
    Fixnum_sp fnx = gc::As<Fixnum_sp>(x);
    if (y.fixnump()) {
      Fixnum_sp fny = gc::As<Fixnum_sp>(y);
      gctools::Fixnum z = fixnum_operations[op](unbox_fixnum(fnx), unbox_fixnum(fny));
      return make_fixnum(z);
    } else if (Bignum_sp bny = y.asOrNull<Bignum_O>()) {
      Bignum_sp x_copy = my_thread->bigRegister0();
      x_copy->setFixnum(unbox_fixnum(fnx));
      (bignum_operations[op])(x_copy, x_copy, bny);
      return x_copy;// what does that do? //_clasp_big_register_normalize(x_copy);
    } else {
      ERROR_WRONG_TYPE_NTH_ARG(cl::_sym_boole, 3, y, cl::_sym_integer);
    }
  } else if (Bignum_sp bnx = x.asOrNull<Bignum_O>()) {
    Bignum_sp x_copy = my_thread->bigRegister0();
    if (y.fixnump()) {
      Fixnum_sp fny(gc::As<Fixnum_sp>(y));
      Bignum_sp bny = my_thread->bigRegister1();
      bny->setFixnum(unbox_fixnum(fny));
      (bignum_operations[op])(x_copy, bnx, bny);
      clasp_big_register_free(bny);
    } else if (Bignum_sp bny = y.asOrNull<Bignum_O>()) {
      (bignum_operations[op])(x_copy, bnx, bny);
    } else {
      ERROR_WRONG_TYPE_NTH_ARG(cl::_sym_boole, 3, y, cl::_sym_integer);
    }
    //return _clasp_big_register_normalize(x_copy);
    SIMPLE_ERROR(BF("implement clasp_big_register_normalize for bignums"));
  } else {
    ERROR_WRONG_TYPE_NTH_ARG(cl::_sym_boole, 2, x, cl::_sym_integer);
  }
  return x;
}

/*! Copied from ECL */
CL_DEFUN T_sp cl__logbitp(Integer_sp p, Integer_sp x) {
  // Arguments and Values:p - a non-negative integer,  x - an integer.
  if (clasp_minusp(p))
      // Expected type for p is (Integer 0 *) or cl::_sym_UnsignedByte
    TYPE_ERROR(p, cl::_sym_UnsignedByte);
  bool i;
  if (p.fixnump()) {
    cl_index n = clasp_to_size(p);
    if (x.fixnump()) {
      gctools::Fixnum y = x.unsafe_fixnum();
      if (n >= FIXNUM_BITS) {
        i = (y < 0);
      } else {
        i = ((y >> n) & 1);
      }
    } else {
      //i = mpz_tstbit(gc::As<Bignum_sp>(x)->as_mpz_().get_mpz_t(), n);
      SIMPLE_ERROR(BF("implement logbitp for bignums"));
    }
  } else {
    if (x.fixnump())
      i = (x.unsafe_fixnum() < 0);
    else
      if (clasp_minusp(x))
        i = true;
      else i = false;
  }
  return i ? _lisp->_true() : _Nil<T_O>();
}

CL_LAMBDA(op arg1 arg2);
CL_DECLARE();
CL_DOCSTRING("boole");
CL_DEFUN T_sp cl__boole(T_sp op, T_sp arg1, T_sp arg2) {
  if (op.nilp()) {
    // the type of this error should be one of values of cl::_sym_boole_1 .. cl::_sym_boole_xor
    ERROR_WRONG_TYPE_NTH_ARG(cl::_sym_boole, 1, op, Cons_O::createList(cl::_sym_Integer_O, make_fixnum(0), make_fixnum(boolOpsMax-1)));
  }
  Fixnum_sp fnop = gc::As<Fixnum_sp>(op);
  return clasp_boole(unbox_fixnum(fnop), arg1, arg2);
};

  SYMBOL_EXPORT_SC_(ClPkg, boole_1);
  SYMBOL_EXPORT_SC_(ClPkg, boole_2);
  SYMBOL_EXPORT_SC_(ClPkg, boole_and);
  SYMBOL_EXPORT_SC_(ClPkg, boole_andc1);
  SYMBOL_EXPORT_SC_(ClPkg, boole_andc2);
  SYMBOL_EXPORT_SC_(ClPkg, boole_c1);
  SYMBOL_EXPORT_SC_(ClPkg, boole_c2);
  SYMBOL_EXPORT_SC_(ClPkg, boole_clr);
  SYMBOL_EXPORT_SC_(ClPkg, boole_eqv);
  SYMBOL_EXPORT_SC_(ClPkg, boole_ior);
  SYMBOL_EXPORT_SC_(ClPkg, boole_nand);
  SYMBOL_EXPORT_SC_(ClPkg, boole_nor);
  SYMBOL_EXPORT_SC_(ClPkg, boole_orc1);
  SYMBOL_EXPORT_SC_(ClPkg, boole_orc2);
  SYMBOL_EXPORT_SC_(ClPkg, boole_set);
  SYMBOL_EXPORT_SC_(ClPkg, boole_xor);


void initialize_bits() {
  cl::_sym_boole_1->defconstant(make_fixnum(boole_1));
  cl::_sym_boole_2->defconstant(make_fixnum(boole_2));
  cl::_sym_boole_and->defconstant(make_fixnum(boole_and));
  cl::_sym_boole_andc1->defconstant(make_fixnum(boole_andc1));
  cl::_sym_boole_andc2->defconstant(make_fixnum(boole_andc2));
  cl::_sym_boole_c1->defconstant(make_fixnum(boole_c1));
  cl::_sym_boole_c2->defconstant(make_fixnum(boole_c2));
  cl::_sym_boole_clr->defconstant(make_fixnum(boole_clr));
  cl::_sym_boole_eqv->defconstant(make_fixnum(boole_eqv));
  cl::_sym_boole_ior->defconstant(make_fixnum(boole_ior));
  cl::_sym_boole_nand->defconstant(make_fixnum(boole_nand));
  cl::_sym_boole_nor->defconstant(make_fixnum(boole_nor));
  cl::_sym_boole_orc1->defconstant(make_fixnum(boole_orc1));
  cl::_sym_boole_orc2->defconstant(make_fixnum(boole_orc2));
  cl::_sym_boole_set->defconstant(make_fixnum(boole_set));
  cl::_sym_boole_xor->defconstant(make_fixnum(boole_xor));

//  af_def(ClPkg, "logbitp", &cl_logbitp);
};

Integer_sp log_operation_2op(boole_ops operation, Integer_sp first, Integer_sp second) {
  // if the arguments are all fixnum, don't convert everything to mpz, but stay in fixnums
  if (first.fixnump() && second.fixnump()){
    gc::Fixnum first_internal = first.unsafe_fixnum();
    gc::Fixnum second_internal = second.unsafe_fixnum();
    gc::Fixnum result;
    switch (operation) {
    case boole_and:
        result = first_internal & second_internal;
        break;
    case boole_xor:
        result = first_internal ^ second_internal;
        break;
    case boole_ior:
        result = first_internal | second_internal;
        break;
    case boole_eqv:
        result = (~(first_internal ^ second_internal));
        break;
    case boole_andc1:
        result = (~first_internal) & second_internal;
        break;
    case boole_andc2:
        result = first_internal & (~second_internal);
        break;
    case boole_orc1:
        result = (~first_internal) | second_internal;
        break;
    case boole_orc2:
        result = first_internal | (~second_internal);
        break;
    case boole_nand:
        result = ~(first_internal & second_internal);
        break;
    case boole_nor:
        result = ~(first_internal | second_internal);
        break;
    default:
        SIMPLE_ERROR(BF("Unknown operation in log_operation_2op"));
    }
    return clasp_make_fixnum(result);
  }
  else {
    Bignum_sp b1; // need to convert everything to a bignum
    Bignum_sp b2; // a potentially faster way to do it would be to treat a fixnum as one limb
    
    if(first.fixnump())b1=Bignum_O::create(first.unsafe_fixnum());
    else b1 = gc::As<Bignum_sp>(first);
    if(b2.fixnump())b2=Bignum_O::create(second.unsafe_fixnum());
    else b2 = gc::As<Bignum_sp>(second);
    //GC_ALLOCATE_VARIADIC(Bignum_O,ret_val); // allocate the result
    //ret_val->realloc_limbs(std::max(abs(first_big_internal->numberoflimbs),abs(second_big_internal->numberoflimbs)));
    switch (operation) { // We make use here of binary identities
      //in the sign-magnitude representation.
    case boole_and:
        if(!b1->minusp_()){
          if(!b2->minusp_()){ // b1>=0,b2>=0
            return _clasp_big_and_magnitude(b1,b2)->normalize()->maybe_as_fixnum();
          }
          else{ //b1>=0,b2<0
            return _clasp_big_andn_magnitude(b1, gc::As<Bignum_sp>(b2->abs_()->oneMinus_()))
              ->normalize()->maybe_as_fixnum();
          }
        } else{
          if(!b2->minusp_()){ // b1<0,b2>=0
            return _clasp_big_andn_magnitude(b2,gc::As<Bignum_sp>(b1->abs_()->oneMinus_()))
              ->normalize()->maybe_as_fixnum();
          }
          else{// b1<0,b2<0
            return 
              gc::As<Integer_sp>(_clasp_big_ior_magnitude(gc::As<Bignum_sp>(b1->abs_()->oneMinus_()),gc::As<Bignum_sp>((b2->abs_()->oneMinus_())))->negate_()->oneMinus_());
          }
        }
        break;
    case boole_xor:
        if(!b1->minusp_()){
          if(!b2->minusp_()){ // b1>=0,b2>=0
            return _clasp_big_xor_magnitude(b1,b2)->normalize()->maybe_as_fixnum();
          }
          else{ //b1>=0,b2<0
            
          }
        } else{
          if(!b2->minusp_()){ // b1<0,b2>=0
          }
          else{// b1<0,b2<0
          }
        }
        break;
    case boole_ior:
        if(!b1->minusp_()){
          if(!b2->minusp_()){ // b1>=0,b2>=0
            return _clasp_big_ior_magnitude(b1,b2)->normalize()->maybe_as_fixnum();
          }
          else{ //b1>=0,b2<0
            
          }
        } else{
          if(!b2->minusp_()){ // b1<0,b2>=0
          }
          else{// b1<0,b2<0
          }
        }
        break;
    case boole_eqv:
        if(!b1->minusp_()){
          if(!b2->minusp_()){ // b1>=0,b2>=0
            
          }
          else{ //b1>=0,b2<0
            
          }
        } else{
          if(!b2->minusp_()){ // b1<0,b2>=0
          }
          else{// b1<0,b2<0
          }
        }
        break;
    case boole_andc1:
        if(!b1->minusp_()){
          if(!b2->minusp_()){ // b1>=0,b2>=0
            return _clasp_big_andn_magnitude(b2,b1)->normalize()->maybe_as_fixnum();
          }
          else{ //b1>=0,b2<0
            
          }
        } else{
          if(!b2->minusp_()){ // b1<0,b2>=0
          }
          else{// b1<0,b2<0
          }
        }
        break;
    case boole_andc2:
        if(!b1->minusp_()){
          if(!b2->minusp_()){ // b1>=0,b2>=0
            return _clasp_big_andn_magnitude(b1,b2)->normalize()->maybe_as_fixnum();
          }
          else{ //b1>=0,b2<0
            return _clasp_big_and_magnitude(b1,gc::As<Bignum_sp>(b2->abs_()->oneMinus_()));
          }
        } else{
          if(!b2->minusp_()){ // b1<0,b2>=0
          }
          else{// b1<0,b2<0
          }
        }
        break;
    case boole_orc1:
        if(!b1->minusp_()){
          if(!b2->minusp_()){ // b1>=0,b2>=0
            return _clasp_big_iorn_magnitude(b2,b1)->normalize()->maybe_as_fixnum();
          }
          else{ //b1>=0,b2<0
            
          }
        } else{
          if(!b2->minusp_()){ // b1<0,b2>=0
          }
          else{// b1<0,b2<0
          }
        }
        break;
    case boole_orc2:
        if(!b1->minusp_()){
          if(!b2->minusp_()){ // b1>=0,b2>=0
            return _clasp_big_iorn_magnitude(b1,b2)->normalize()->maybe_as_fixnum();
          }
          else{ //b1>=0,b2<0
            
          }
        } else{
          if(!b2->minusp_()){ // b1<0,b2>=0
          }
          else{// b1<0,b2<0
          }
        }
        break;
    case boole_nand:
        if(!b1->minusp_()){
          if(!b2->minusp_()){ // b1>=0,b2>=0
            return _clasp_big_nand_magnitude(b1,b2)->normalize()->maybe_as_fixnum();
          }
          else{ //b1>=0,b2<0
            
          }
        } else{
          if(!b2->minusp_()){ // b1<0,b2>=0
          }
          else{// b1<0,b2<0
          }
        }
        break;
    case boole_nor:
        if(!b1->minusp_()){
          if(!b2->minusp_()){ // b1>=0,b2>=0
            return _clasp_big_nand_magnitude(b1,b2)->normalize()->maybe_as_fixnum();
          }
          else{ //b1>=0,b2<0
            
          }
        } else{
          if(!b2->minusp_()){ // b1<0,b2>=0
          }
          else{// b1<0,b2<0
          }
        }
        break;
    default:
        SIMPLE_ERROR(BF("Unknown operation in cl__log_operation_rest"));
    }
    std::cout << "The operation was" << operation << "\n";
    b1->debug_print();
    b2->debug_print();
    
    SIMPLE_ERROR(BF("Operation unimplemented "));

    //return ret_val;
    //return Integer_O::create(result_bignum);
  }
}

CL_LAMBDA(first second);
CL_DECLARE();
CL_DOCSTRING("logand_2op");
CL_DEFUN Integer_sp core__logand_2op(Integer_sp first, Integer_sp second) {
  return log_operation_2op(boole_and, first, second);
}

CL_LAMBDA(first second);
CL_DECLARE();
CL_DOCSTRING("logxor_2op");
CL_DEFUN Integer_sp core__logxor_2op(Integer_sp first, Integer_sp second) {
  return log_operation_2op(boole_xor, first, second);
}

CL_LAMBDA(first second);
CL_DECLARE();
CL_DOCSTRING("logior_2op");
CL_DEFUN Integer_sp core__logior_2op(Integer_sp first, Integer_sp second) {
  return log_operation_2op(boole_ior, first, second);
}

CL_LAMBDA(first second);
CL_DECLARE();
CL_DOCSTRING("logeqv_2op");
CL_DEFUN Integer_sp core__logeqv_2op(Integer_sp first, Integer_sp second) {
  return log_operation_2op(boole_eqv, first, second);
}

Integer_sp log_operation_rest(List_sp integers, boole_ops operation) {
  // if the arguments are all fixnum, don't convert everything to mpz, but stay in fixnums
  bool acc_fixnum_p = true;
  Integer_sp first = gc::As<Integer_sp>(oCar(integers));
  gc::Fixnum acc_fixnum;
  Bignum_sp acc_bignum;
  if (first.fixnump()) {
    acc_fixnum = first.unsafe_fixnum();
  }
  else {
    acc_fixnum_p = false;
    acc_bignum = gc::As<Bignum_sp>(first);
  }
  for (auto cur : (List_sp)oCdr(integers)) {
    Integer_sp icur = gc::As<Integer_sp>(oCar(cur));
    if (acc_fixnum_p) {
      if (icur.fixnump()) {
        // we stay in fixnum
        switch (operation) {
        case boole_and:
            acc_fixnum = acc_fixnum & icur.unsafe_fixnum(); continue;
        case boole_xor:
            acc_fixnum = acc_fixnum ^ icur.unsafe_fixnum(); continue;
        case boole_ior:
            acc_fixnum = acc_fixnum | icur.unsafe_fixnum(); continue;
        case boole_eqv:
            acc_fixnum = (~(acc_fixnum ^ icur.unsafe_fixnum())); continue;
        default:
            SIMPLE_ERROR(BF("Unknown operation in cl__log_operation_rest"));
        }
      } else {
        // need to go bignum
        acc_fixnum_p=false; // i think this is broken
        acc_bignum = Bignum_O::create(acc_fixnum);
      }
    }
    // Now either acc_fixnum_p was false and icur is a fixnum, or acc_fixnum_p was true.
    Bignum_sp icur_big;
    if(icur.fixnump())icur_big = Bignum_O::create(icur.unsafe_fixnum());
    else icur_big= gc::As<Bignum_sp>(icur);
    switch (operation) {
     case boole_and:
         if(acc_bignum->numberoflimbs!=0)
           //mpn_and_n(acc_bignum->limbs,acc_bignum->limbs,icur_big->limbs,abs(acc_bignum->numberoflimbs)); // this is broken
           //std::cout << "should have done a proper and here\n";
           acc_bignum=gc::As<Bignum_sp>(log_operation_2op(boole_and,acc_bignum,icur_big));
         break;
     case boole_xor:
         //mpz_xor(temp.get_mpz_t(),  acc_bignum.get_mpz_t(), clasp_to_mpz(icur).get_mpz_t());
         acc_bignum=gc::As<Bignum_sp>(log_operation_2op(boole_xor,acc_bignum,icur_big));
         break;
    case boole_ior:
         //mpz_ior(temp.get_mpz_t(),  acc_bignum.get_mpz_t(), clasp_to_mpz(icur).get_mpz_t());
         //if(acc_bignum->numberoflimbs!=0)mpn_ior_n(acc_bignum->limbs,acc_bignum->limbs,icur_big->limbs,abs(acc_bignum->numberoflimbs)); // this is broken
                                      
        acc_bignum=gc::As<Bignum_sp>(log_operation_2op(boole_ior,acc_bignum,icur_big));
        break;
     case boole_eqv:
    //     mpz_xor(temp1.get_mpz_t(), acc_bignum.get_mpz_t(), clasp_to_mpz(icur).get_mpz_t());
    //     mpz_com(temp.get_mpz_t(), temp1.get_mpz_t());
         acc_bignum=gc::As<Bignum_sp>(log_operation_2op(boole_eqv,acc_bignum,icur_big));
         break;
    default:
        SIMPLE_ERROR(BF("Unknown operation in cl__log_operation_rest"));
    }
  } // loop over integers
  if (acc_fixnum_p)
    return Integer_O::create(acc_fixnum);
  else 
    return acc_bignum;
    //return Integer_O::create(acc_bignum);
}

CL_LAMBDA(&rest integers);
CL_DECLARE();
CL_DOCSTRING("logand");
CL_DEFUN Integer_sp cl__logand(List_sp integers) {
  // if the arguments are all fixnum, don't convert everything to mpz, but stay in fixnums
  if (integers.nilp())
    return clasp_make_fixnum(-1);
  else
    return log_operation_rest(integers, boole_and); 
};

CL_LAMBDA(&rest integers);
CL_DECLARE();
CL_DOCSTRING("logior");
CL_DEFUN Integer_sp cl__logior(List_sp integers) {
  if (integers.nilp())
    return clasp_make_fixnum(0);
  else
    return log_operation_rest(integers, boole_ior); 
};

CL_LAMBDA(&rest integers);
CL_DECLARE();
CL_DOCSTRING("logxor");
CL_DEFUN Integer_sp cl__logxor(List_sp integers) {
  if (integers.nilp())
    return clasp_make_fixnum(0);
  else
    return log_operation_rest(integers, boole_xor);
};

CL_LAMBDA(&rest integers);
CL_DECLARE();
CL_DOCSTRING("logeqv");
CL_DEFUN Integer_sp cl__logeqv(List_sp integers) {
  if (integers.nilp())
    return Integer_O::create((gc::Fixnum) - 1);
  else
    return log_operation_rest(integers, boole_eqv);
};

CL_LAMBDA(a b);
CL_DECLARE();
CL_DOCSTRING("logandc1");
CL_DEFUN Integer_sp cl__logandc1(Integer_sp a, Integer_sp b) {
  return log_operation_2op(boole_andc1, a, b);
};

CL_LAMBDA(a b);
CL_DECLARE();
CL_DOCSTRING("logandc2");
CL_DEFUN Integer_sp cl__logandc2(Integer_sp a, Integer_sp b) {
  return log_operation_2op(boole_andc2, a, b);
};

CL_LAMBDA(a b);
CL_DECLARE();
CL_DOCSTRING("logorc1");
CL_DEFUN Integer_sp cl__logorc1(Integer_sp a, Integer_sp b) {
  return log_operation_2op(boole_orc1, a, b);
};

CL_LAMBDA(a b);
CL_DECLARE();
CL_DOCSTRING("logorc2");
CL_DEFUN Integer_sp cl__logorc2(Integer_sp a, Integer_sp b) {
  return log_operation_2op(boole_orc2, a, b);
};

CL_LAMBDA(a);
CL_DECLARE();
CL_DOCSTRING("lognot");
CL_DEFUN Integer_sp cl__lognot(Integer_sp a) {
  if (a.fixnump()) {
    // in ecl return @logxor(2,x,ecl_make_fixnum(-1))
    return clasp_make_fixnum(a.unsafe_fixnum() ^ -1);   
  }
  else { // In sign-magnitude representation, ~x ==> -x-1
    
    return gc::As<Integer_sp>((gc::As<Bignum_sp>(a)->negate_()->oneMinus_()));
  }
};

CL_LAMBDA(a b);
CL_DECLARE();
CL_DOCSTRING("lognand");
CL_DEFUN Integer_sp cl__lognand(Integer_sp a, Integer_sp b) {
  return log_operation_2op(boole_nand, a, b);
};

CL_LAMBDA(a b);
CL_DECLARE();
CL_DOCSTRING("lognor");
CL_DEFUN Integer_sp cl__lognor(Integer_sp a, Integer_sp b) {
  return log_operation_2op(boole_nor, a, b);
};

}; // namespace core
