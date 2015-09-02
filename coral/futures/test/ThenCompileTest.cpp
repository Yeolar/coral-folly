// This file is @generated by then_compile_test.rb. Do not edit directly.

#include <coral/futures/test/ThenCompileTest.h>

using namespace coral;

TEST(Basic, thenVariants) {
  SomeClass anObject;

  {Future<B> f = someFuture<A>().then(&aFunction<Future<B>, Try<A>&&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aStaticMethod<Future<B>, Try<A>&&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aMethod<Future<B>, Try<A>&&>, &anObject);}
  {Future<B> f = someFuture<A>().then(aStdFunction<Future<B>, Try<A>&&>());}
  {Future<B> f = someFuture<A>().then([&](Try<A>&&){return someFuture<B>();});}
  {Future<B> f = someFuture<A>().then(&aFunction<Future<B>, Try<A> const&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aStaticMethod<Future<B>, Try<A> const&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aMethod<Future<B>, Try<A> const&>, &anObject);}
  {Future<B> f = someFuture<A>().then(aStdFunction<Future<B>, Try<A> const&>());}
  {Future<B> f = someFuture<A>().then([&](Try<A> const&){return someFuture<B>();});}
  {Future<B> f = someFuture<A>().then(&aFunction<Future<B>, Try<A>>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aStaticMethod<Future<B>, Try<A>>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aMethod<Future<B>, Try<A>>, &anObject);}
  {Future<B> f = someFuture<A>().then(aStdFunction<Future<B>, Try<A>>());}
  {Future<B> f = someFuture<A>().then([&](Try<A>){return someFuture<B>();});}
  {Future<B> f = someFuture<A>().then(&aFunction<Future<B>, Try<A>&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aStaticMethod<Future<B>, Try<A>&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aMethod<Future<B>, Try<A>&>, &anObject);}
  {Future<B> f = someFuture<A>().then(aStdFunction<Future<B>, Try<A>&>());}
  {Future<B> f = someFuture<A>().then([&](Try<A>&){return someFuture<B>();});}
  {Future<B> f = someFuture<A>().then(&aFunction<Future<B>, A&&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aStaticMethod<Future<B>, A&&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aMethod<Future<B>, A&&>, &anObject);}
  {Future<B> f = someFuture<A>().then(aStdFunction<Future<B>, A&&>());}
  {Future<B> f = someFuture<A>().then([&](A&&){return someFuture<B>();});}
  {Future<B> f = someFuture<A>().then(&aFunction<Future<B>, A const&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aStaticMethod<Future<B>, A const&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aMethod<Future<B>, A const&>, &anObject);}
  {Future<B> f = someFuture<A>().then(aStdFunction<Future<B>, A const&>());}
  {Future<B> f = someFuture<A>().then([&](A const&){return someFuture<B>();});}
  {Future<B> f = someFuture<A>().then(&aFunction<Future<B>, A>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aStaticMethod<Future<B>, A>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aMethod<Future<B>, A>, &anObject);}
  {Future<B> f = someFuture<A>().then(aStdFunction<Future<B>, A>());}
  {Future<B> f = someFuture<A>().then([&](A){return someFuture<B>();});}
  {Future<B> f = someFuture<A>().then(&aFunction<Future<B>, A&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aStaticMethod<Future<B>, A&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aMethod<Future<B>, A&>, &anObject);}
  {Future<B> f = someFuture<A>().then(aStdFunction<Future<B>, A&>());}
  {Future<B> f = someFuture<A>().then([&](A&){return someFuture<B>();});}
  {Future<B> f = someFuture<A>().then([&](){return someFuture<B>();});}
  {Future<B> f = someFuture<A>().then(&aFunction<B, Try<A>&&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aStaticMethod<B, Try<A>&&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aMethod<B, Try<A>&&>, &anObject);}
  {Future<B> f = someFuture<A>().then(aStdFunction<B, Try<A>&&>());}
  {Future<B> f = someFuture<A>().then([&](Try<A>&&){return B();});}
  {Future<B> f = someFuture<A>().then(&aFunction<B, Try<A> const&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aStaticMethod<B, Try<A> const&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aMethod<B, Try<A> const&>, &anObject);}
  {Future<B> f = someFuture<A>().then(aStdFunction<B, Try<A> const&>());}
  {Future<B> f = someFuture<A>().then([&](Try<A> const&){return B();});}
  {Future<B> f = someFuture<A>().then(&aFunction<B, Try<A>>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aStaticMethod<B, Try<A>>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aMethod<B, Try<A>>, &anObject);}
  {Future<B> f = someFuture<A>().then(aStdFunction<B, Try<A>>());}
  {Future<B> f = someFuture<A>().then([&](Try<A>){return B();});}
  {Future<B> f = someFuture<A>().then(&aFunction<B, Try<A>&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aStaticMethod<B, Try<A>&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aMethod<B, Try<A>&>, &anObject);}
  {Future<B> f = someFuture<A>().then(aStdFunction<B, Try<A>&>());}
  {Future<B> f = someFuture<A>().then([&](Try<A>&){return B();});}
  {Future<B> f = someFuture<A>().then(&aFunction<B, A&&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aStaticMethod<B, A&&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aMethod<B, A&&>, &anObject);}
  {Future<B> f = someFuture<A>().then(aStdFunction<B, A&&>());}
  {Future<B> f = someFuture<A>().then([&](A&&){return B();});}
  {Future<B> f = someFuture<A>().then(&aFunction<B, A const&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aStaticMethod<B, A const&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aMethod<B, A const&>, &anObject);}
  {Future<B> f = someFuture<A>().then(aStdFunction<B, A const&>());}
  {Future<B> f = someFuture<A>().then([&](A const&){return B();});}
  {Future<B> f = someFuture<A>().then(&aFunction<B, A>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aStaticMethod<B, A>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aMethod<B, A>, &anObject);}
  {Future<B> f = someFuture<A>().then(aStdFunction<B, A>());}
  {Future<B> f = someFuture<A>().then([&](A){return B();});}
  {Future<B> f = someFuture<A>().then(&aFunction<B, A&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aStaticMethod<B, A&>);}
  {Future<B> f = someFuture<A>().then(&SomeClass::aMethod<B, A&>, &anObject);}
  {Future<B> f = someFuture<A>().then(aStdFunction<B, A&>());}
  {Future<B> f = someFuture<A>().then([&](A&){return B();});}
  {Future<B> f = someFuture<A>().then([&](){return B();});}
}
