// Copyright 2008 the CProxyV8 project authors. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef CPROXYV8_H_
#define CPROXYV8_H_

#include <v8/v8.h>
#include <string>

template <class T>
class InstaceHandle;

using namespace v8;




template <class T>
class InstaceHandle
{
public:
  InstaceHandle(bool toBeCreated)
    : t_(NULL), toBeDestroy_(toBeCreated),toBeCreated_(toBeCreated) {};

  explicit InstaceHandle(T* t)
    : t_(t), toBeDestroy_(false), toBeCreated_(false) {}

  ~InstaceHandle() { if (t_ && toBeDestroy_) delete t_; }

  inline void set(T*t, bool toBeDestroy);

  /**
   * Returns true if the handle is empty.
   */
  inline bool IsEmpty();

  T* operator->();

  T* operator*();

  /**
   * Checks whether two handles are the same.
   * Returns true if both are empty, or if the objects
   * to which they refer are identical.
   * The handles' references are not checked.
   */
  template <class S> bool operator==(Handle<S> that);

  /**
   * Checks whether two handles are different.
   * Returns true if only one of the handles is empty, or if
   * the objects to which they refer are different.
   * The handles' references are not checked.
   */
  template <class S> bool operator!=(Handle<S> that)
  {
    return !operator==(that);
  }

  static InstaceHandle<T>* Cast(void* that)
  {
    return static_cast<InstaceHandle<T>*>(that);
  }

private:
  T* t_;
  bool toBeDestroy_;
  bool toBeCreated_;
};

template <class T>
T* InstaceHandle<T>::operator->()
{
  if (!t_ && toBeCreated_)
  {
    //t_ = Type::New((T*)NULL);
	  t_ = new T();
    toBeCreated_ = false;
  }

  return t_;
}

template <class T>
void InstaceHandle<T>::set(T*t, bool toBeDestroy)
{
  if (t_ && toBeDestroy_)
    delete t_;
  toBeDestroy_ = toBeDestroy;
  t_ = t;
}


template <class T>
template <class S>
bool InstaceHandle<T>::operator==(Handle<S> that)
{
  if (!t_ && toBeCreated_)
    return false;

  void** a = reinterpret_cast<void**>(**this);
  void** b = reinterpret_cast<void**>(*that);
  if (a == 0) return b == 0;
  if (b == 0) return false;
  return *a == *b;
}


template <class T>
bool InstaceHandle<T>::IsEmpty()
{
    return t_ == 0 && !toBeCreated_;
}

template <class T>
T* InstaceHandle<T>::operator*()
{
  if (!t_ && toBeCreated_)
  {
    //t_ = Type::New((T*)NULL);
	  t_ = new T();
    toBeCreated_ = false;
  }

  return t_;
}


static std::map<std::string, Handle<ObjectTemplate> > templs;

namespace Type
{

  /** Constructor C++ */
  template <class T>
  inline T* New(T*t)
  {
    //ASSERT(!t);
    return new T();
  }

  /** Cast from V8 to C++ */
  inline bool Cast(bool*, Local<Value> value, Handle<ObjectTemplate>, const char *v="")
  {
    return value->BooleanValue();
  }

  inline double Cast(double*, Local<Value> value, Handle<ObjectTemplate>, const char *v="")
  {
    return value->NumberValue();
  }

  inline uint32_t Cast(uint32_t*, Local<Value> value, Handle<ObjectTemplate>, const char *v="")
  {
    return value->Uint32Value();
  }

  inline int32_t  Cast(int32_t*, Local<Value> value, Handle<ObjectTemplate>, const char *v="")
  {
    return value->Int32Value();
  }

  inline int64_t  Cast(int64_t*, Local<Value> value, Handle<ObjectTemplate>, const char *v="")
  {
    return value->IntegerValue();
  }

  inline Handle<Value> Cast(Handle<Value> value, Handle<ObjectTemplate>, const char *v="")
  {
    return value;
  };

  template<class T>
	T* UnwrapObject(v8::Local<v8::Object> holder)
	{
	  v8::Local<v8::External> external = v8::Local<v8::External>::Cast(holder->GetInternalField(0));
	  InstaceHandle<T>* tHandle = InstaceHandle<T>::Cast(external->Value());
	  return *(*tHandle);
	}

  template<class T>
	T* UnwrapObject(v8::Local<v8::Value> value)
	{
	  v8::Local<v8::External> external = v8::Local<v8::External>::Cast(value);
	  InstaceHandle<T>* tHandle = static_cast<InstaceHandle<T>*>(external->Value());
	  return *(*tHandle);
	}

  /** Cast V8:Value to primitive value */

  template<class T>
  inline T Cast(T*, Local<Value> value, Handle<ObjectTemplate> templ)
  {
	  return *(UnwrapObject<T>(value));
    //return ProxyClass<T>::instance()->UnwrapObject(value);
  }

  template<class T>
  inline T Cast(T&, Local<Value> value, Handle<ObjectTemplate> templ)
  {
	  return *(UnwrapObject<T>(value));
    //return ProxyClass<T>::instance()->UnwrapObject(value);
  }

  /** Cast from C++ to V8 */
  inline Handle<Value> Cast(bool caseParam, Handle<ObjectTemplate> templ, const char *v="")     { return Boolean::New(caseParam); }
  inline Handle<Value> Cast(double caseParam, Handle<ObjectTemplate> templ, const char *v="")   { return Number::New(caseParam); }
  inline Handle<Value> Cast(uint32_t caseParam, Handle<ObjectTemplate> templ, const char *v="") { return Uint32::New(caseParam); }
  inline Handle<Value> Cast(int32_t  caseParam, Handle<ObjectTemplate> templ, const char *v="") { return Int32::New(caseParam); }

  template<class T>
  Handle<Value> ExposeObject(T* t, bool withDestroy, Handle<ObjectTemplate> objectTemplate_, const char *v)
  {
	    if(templs.count(v) > 0) {

	    }
	    else {
	    	printf("init first");
	    	exit(-1);
	    }

	  // Handle scope for temporary handles.

    HandleScope handle_scope;

    Handle<ObjectTemplate> templ = templs[v];

    //Handle<ObjectTemplate> templ = objectTemplate_;

    // Create an empty http request wrapper.
    Handle<Object> result = templ->NewInstance();

    // Wrap the raw C++ pointer in an External so it can be referenced
    // from within JavaScript.
    Local<External> externalVal = Local<External>::Cast(result->GetInternalField(0));

    InstaceHandle<T>* tHandle = static_cast<InstaceHandle<T>*>(externalVal->Value());
    tHandle->set(t, withDestroy);

    // Return the result through the current handle scope.  Since each
    // of these handles will go away when the handle scope is deleted
    // we need to call Close to let one, the result, escape into the
    // outer handle scope.
    return handle_scope.Close(result);
  }

  template<class T>
  inline Handle<Value> Cast(T* t, Handle<ObjectTemplate> templ, const char *v="")
  {
	  return ExposeObject(t, false, templ, v);
    //return ProxyClass<T>::instance()->ExposeObject(t, false);
  };

  template<class T>
  inline Handle<Value> Cast(T& t, Handle<ObjectTemplate> templ, const char *v="")
  {
	  return ExposeObject(&t, false, templ, v);
    //return ProxyClass<T>::instance()->ExposeObject(&t, false);
  };

  /** Function return value Cast from C++ to V8 */
  inline Handle<Value> FCast(bool caseParam, Handle<ObjectTemplate> templ)     { return Cast(caseParam, templ); }
  inline Handle<Value> FCast(double caseParam, Handle<ObjectTemplate> templ)   { return Cast(caseParam, templ); }
  inline Handle<Value> FCast(uint32_t caseParam, Handle<ObjectTemplate> templ) { return Cast(caseParam, templ); }
  inline Handle<Value> FCast(int32_t  caseParam, Handle<ObjectTemplate> templ) { return Cast(caseParam, templ); }


  template<class T>
  inline Handle<Value> FCast(T* t, Handle<ObjectTemplate> templ)
  {
	  return ExposeObject(t, true, templ);
    //return ProxyClass<T>::instance()->ExposeObject(t, true);
  };

  template<class T>
  inline Handle<Value> FCast(T& t, Handle<ObjectTemplate> templ)
  {
	  return ExposeObject(&t, true, templ);
    //return ProxyClass<T>::instance()->ExposeObject(&t, true);
  };
};



template <class T>
class THandlePrimitive;

template<class T>
class ProxyClass;

class Accessor
{
};
/**
 * AccessorProperty allow the program to make the abstraction betweem the real
 * cppObj->property and the generic callback from JS
 *
 * The idea here is to take advantage of the CTemplates and the pure virtual
 * methods, to avoid the need to specify the CTemplate type in the access to the
 * cppObj->property
 *
 * This kind of integration requires args.Data() to be an implementation
 * of AccessorProperty and args.Holder() the instance of the specific object.
 *
 * This are the current supported properties:
 *
 * \code
 *    class Example
 *    {
 *    public:
 *      TPrimitive cppObj->Method();   //see FuncRetPrimitive
 *      TClass cppObj->Method();       //see FuncRetObj
 *      v8::Handle<v8::Value> Method(const v8::Arguments& args);  //FuncV8
 *    ...
 * \endcode
 *
 */
class AccessorProperty : public Accessor
{
public:

  /** Changet the value of the property in the Cpp generated program */
	virtual void AccessorSetProperty(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info) = 0;

  /** returns the value of the property to JS enviroment */
	virtual v8::Handle<v8::Value> AccessorGetProperty(v8::Local<v8::String> property, const v8::AccessorInfo& info) = 0;
};

/**
 * JS callback
 *
 * This function obtains the generic AccessorProperty from args,
 * and set the value of cppObj->property with the value
 */
void Accessor_SetProperty(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

/**
 * JS callback
 *
 * This function obtains the generic AccessorProperty from args,
 * and return the value of cppObj->property
 */
v8::Handle<v8::Value> Accessor_GetProperty(v8::Local<v8::String> property, const v8::AccessorInfo& info);


void Accessor_SetProperty(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
  v8::Local<v8::External> external = v8::Local<v8::External>::Cast(info.Data());
  AccessorProperty* callBack = static_cast<AccessorProperty*>(external->Value());
  callBack->AccessorSetProperty(property, value, info);
}

v8::Handle<v8::Value> Accessor_GetProperty(v8::Local<v8::String> property, const v8::AccessorInfo& info)
{
  v8::Local<v8::External> external = v8::Local<v8::External>::Cast(info.Data());
  AccessorProperty* callBack = static_cast<AccessorProperty*>(external->Value());
  return callBack->AccessorGetProperty(property,  info);
}



/**
 * AccessorFunction allow the program to make the abstraction betweem the real
 * cppObj->Method() and the generic callback from JS
 *
 * The idea here is to take advantage of the CTemplates and the pure virtual
 * methods, to avoid the need to specify the CTemplate type in the call to the
 * cppObj->Method()
 *
 * This kind of integration requires args.Data() to be an implementation
 * of AccessorFunction and args.Holder() the instance of the specific object.
 *
 * This are the current supported methods:
 *
 * \code
 *    class Example
 *    {
 *    public:
 *      TPrimitive cppObj->Method();   //see FuncRetPrimitive
 *      TClass cppObj->Method();       //see FuncRetObj
 *      v8::Handle<v8::Value> Method(const v8::Arguments& args);  //FuncV8
 *    ...
 * \endcode
 *
 */
class AccessorFunction : public Accessor
{
public:

  /** Entry point from JS. */
	virtual v8::Handle<v8::Value> call(const v8::Arguments& args) = 0;
};


v8::Handle<v8::Value> Accessor_Function(const v8::Arguments& args)
{
  v8::Local<v8::External> external = v8::Local<v8::External>::Cast(args.Data());
  AccessorFunction* callBack = static_cast<AccessorFunction*>(external->Value());
  return callBack->call(args);
}


/**
 * JS callback
 *
 * This function obtains the generic AccessorFunction from args,
 * and call the proxy to call cppObj->method()
 */
v8::Handle<v8::Value> Accessor_Function(const v8::Arguments& args);

namespace internalUtil
{
  /**
   * Utility class to keep track of all accessors in order to be delete when 
   * the ProxyClass is disposed.
   */
  class AccessorList
  {
  public:
    AccessorList() : accessor_(NULL), next_(NULL) {}
    AccessorList(Accessor* accessor) : accessor_(accessor), next_(NULL) {}

    inline virtual ~AccessorList();
    
    inline void push(Accessor * accessor);

  private:
    Accessor* accessor_;
    AccessorList* next_;
  };


  AccessorList::~AccessorList()
  { 
    if (accessor_)
      delete accessor_; 

    if (next_)
      delete next_;
  }

  void AccessorList::push(Accessor * accessor)
  {
    AccessorList* accessorList = new AccessorList(accessor);
    accessorList = next_;
    next_ = accessorList;
  }
}
#endif
