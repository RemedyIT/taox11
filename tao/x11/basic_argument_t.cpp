/**
 * @file    basic_argument_t.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 stub argument traits for all types
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_BASIC_ARGUMENT_T_CPP
#define TAOX11_BASIC_ARGUMENT_T_CPP

#include "tao/x11/basic_argument_t.h"

namespace TAOX11_NAMESPACE
{

template<typename S,
         template <typename> class Insert_Policy>
TAO_CORBA::Boolean
In_Basic_Argument_T<S, Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         template <typename> class Insert_Policy>
void
In_Basic_Argument_T<S, Insert_Policy>::interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

template<typename S,
         template <typename> class Insert_Policy>
In_Basic_Clonable_Argument_T<S,Insert_Policy>::~In_Basic_Clonable_Argument_T ()
{
  if (this->is_clone_)
    {
      S* tmp = const_cast<S*> (std::addressof(this->x_));
      delete tmp;
    }
}

template<typename S,
         template <typename> class Insert_Policy>
TAO_TAO::Argument*
In_Basic_Clonable_Argument_T<S,Insert_Policy>::clone ()
{
  S* clone_x = new S (this->x_);
  In_Basic_Clonable_Argument_T<S,Insert_Policy>* clone_arg =
          new In_Basic_Clonable_Argument_T<S,Insert_Policy> (*clone_x);
  clone_arg->is_clone_ = true;
  return clone_arg;
}

// ===========================================================

template<typename S,
         template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Inout_Basic_Argument_T<S, Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

template<typename S,
         template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Inout_Basic_Argument_T<S, Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         template <typename> class Insert_Policy>
void
Inout_Basic_Argument_T<S, Insert_Policy>::interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S,
         template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Out_Basic_Argument_T<S, Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         template <typename> class Insert_Policy>
void
Out_Basic_Argument_T<S, Insert_Policy>::interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S,
         template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Ret_Basic_Argument_T<S, Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         template <typename> class Insert_Policy>
void
Ret_Basic_Argument_T<S, Insert_Policy>::interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_BASIC_ARGUMENT_T_CPP */
