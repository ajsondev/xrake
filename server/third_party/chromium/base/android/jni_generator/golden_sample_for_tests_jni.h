// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file is autogenerated by
//     base/android/jni_generator/jni_generator.py
// For
//     org/chromium/example/jni_generator/SampleForTests

#ifndef org_chromium_example_jni_generator_SampleForTests_JNI
#define org_chromium_example_jni_generator_SampleForTests_JNI

#include <jni.h>

#include "base/android/jni_generator/jni_generator_helper.h"

#include "base/android/jni_int_wrapper.h"

// Step 1: forward declarations.
namespace {
const char kInnerStructAClassPath[] =
    "org/chromium/example/jni_generator/SampleForTests$InnerStructA";
const char kSampleForTestsClassPath[] =
    "org/chromium/example/jni_generator/SampleForTests";
const char kInnerStructBClassPath[] =
    "org/chromium/example/jni_generator/SampleForTests$InnerStructB";
// Leaking this jclass as we cannot use LazyInstance from some threads.
jclass g_InnerStructA_clazz = NULL;
// Leaking this jclass as we cannot use LazyInstance from some threads.
jclass g_SampleForTests_clazz = NULL;
// Leaking this jclass as we cannot use LazyInstance from some threads.
jclass g_InnerStructB_clazz = NULL;

}  // namespace

namespace base {
namespace android {

static jlong Init(JNIEnv* env, jobject jcaller,
    jstring param);

static jdouble GetDoubleFunction(JNIEnv* env, jobject jcaller);

static jfloat GetFloatFunction(JNIEnv* env, jclass jcaller);

static void SetNonPODDatatype(JNIEnv* env, jobject jcaller,
    jobject rect);

static jobject GetNonPODDatatype(JNIEnv* env, jobject jcaller);

// Step 2: method stubs.
static void Destroy(JNIEnv* env, jobject jcaller,
    jlong nativeCPPClass) {
  CPPClass* native = reinterpret_cast<CPPClass*>(nativeCPPClass);
  CHECK_NATIVE_PTR(env, jcaller, native, "Destroy");
  return native->Destroy(env, jcaller);
}

static jint Method(JNIEnv* env, jobject jcaller,
    jlong nativeCPPClass) {
  CPPClass* native = reinterpret_cast<CPPClass*>(nativeCPPClass);
  CHECK_NATIVE_PTR(env, jcaller, native, "Method", 0);
  return native->Method(env, jcaller);
}

static jdouble MethodOtherP0(JNIEnv* env, jobject jcaller,
    jlong nativePtr) {
  CPPClass::InnerClass* native =
      reinterpret_cast<CPPClass::InnerClass*>(nativePtr);
  CHECK_NATIVE_PTR(env, jcaller, native, "MethodOtherP0", 0);
  return native->MethodOtherP0(env, jcaller);
}

static void AddStructB(JNIEnv* env, jobject jcaller,
    jlong nativeCPPClass,
    jobject b) {
  CPPClass* native = reinterpret_cast<CPPClass*>(nativeCPPClass);
  CHECK_NATIVE_PTR(env, jcaller, native, "AddStructB");
  return native->AddStructB(env, jcaller, b);
}

static void IterateAndDoSomethingWithStructB(JNIEnv* env, jobject jcaller,
    jlong nativeCPPClass) {
  CPPClass* native = reinterpret_cast<CPPClass*>(nativeCPPClass);
  CHECK_NATIVE_PTR(env, jcaller, native, "IterateAndDoSomethingWithStructB");
  return native->IterateAndDoSomethingWithStructB(env, jcaller);
}

static jstring ReturnAString(JNIEnv* env, jobject jcaller,
    jlong nativeCPPClass) {
  CPPClass* native = reinterpret_cast<CPPClass*>(nativeCPPClass);
  CHECK_NATIVE_PTR(env, jcaller, native, "ReturnAString", NULL);
  return native->ReturnAString(env, jcaller).Release();
}

static base::subtle::AtomicWord g_SampleForTests_javaMethod = 0;
static jint Java_SampleForTests_javaMethod(JNIEnv* env, jobject obj,
    JniIntWrapper foo,
    JniIntWrapper bar) {
  /* Must call RegisterNativesImpl()  */
  CHECK_CLAZZ(env, obj,
      g_SampleForTests_clazz, 0);
  jmethodID method_id =
      base::android::MethodID::LazyGet<
      base::android::MethodID::TYPE_INSTANCE>(
      env, g_SampleForTests_clazz,
      "javaMethod",

"("
"I"
"I"
")"
"I",
      &g_SampleForTests_javaMethod);

  jint ret =
      env->CallIntMethod(obj,
          method_id, as_jint(foo), as_jint(bar));
  jni_generator::CheckException(env);
  return ret;
}

static base::subtle::AtomicWord g_SampleForTests_staticJavaMethod = 0;
static jboolean Java_SampleForTests_staticJavaMethod(JNIEnv* env) {
  /* Must call RegisterNativesImpl()  */
  CHECK_CLAZZ(env, g_SampleForTests_clazz,
      g_SampleForTests_clazz, false);
  jmethodID method_id =
      base::android::MethodID::LazyGet<
      base::android::MethodID::TYPE_STATIC>(
      env, g_SampleForTests_clazz,
      "staticJavaMethod",

"("
")"
"Z",
      &g_SampleForTests_staticJavaMethod);

  jboolean ret =
      env->CallStaticBooleanMethod(g_SampleForTests_clazz,
          method_id);
  jni_generator::CheckException(env);
  return ret;
}

static base::subtle::AtomicWord g_SampleForTests_packagePrivateJavaMethod = 0;
static void Java_SampleForTests_packagePrivateJavaMethod(JNIEnv* env, jobject
    obj) {
  /* Must call RegisterNativesImpl()  */
  CHECK_CLAZZ(env, obj,
      g_SampleForTests_clazz);
  jmethodID method_id =
      base::android::MethodID::LazyGet<
      base::android::MethodID::TYPE_INSTANCE>(
      env, g_SampleForTests_clazz,
      "packagePrivateJavaMethod",

"("
")"
"V",
      &g_SampleForTests_packagePrivateJavaMethod);

     env->CallVoidMethod(obj,
          method_id);
  jni_generator::CheckException(env);

}

static base::subtle::AtomicWord g_SampleForTests_methodThatThrowsException = 0;
static void Java_SampleForTests_methodThatThrowsException(JNIEnv* env, jobject
    obj) {
  /* Must call RegisterNativesImpl()  */
  CHECK_CLAZZ(env, obj,
      g_SampleForTests_clazz);
  jmethodID method_id =
      base::android::MethodID::LazyGet<
      base::android::MethodID::TYPE_INSTANCE>(
      env, g_SampleForTests_clazz,
      "methodThatThrowsException",

"("
")"
"V",
      &g_SampleForTests_methodThatThrowsException);

     env->CallVoidMethod(obj,
          method_id);

}

static base::subtle::AtomicWord g_InnerStructA_create = 0;
static base::android::ScopedJavaLocalRef<jobject>
    Java_InnerStructA_create(JNIEnv* env, jlong l,
    JniIntWrapper i,
    jstring s) {
  /* Must call RegisterNativesImpl()  */
  CHECK_CLAZZ(env, g_InnerStructA_clazz,
      g_InnerStructA_clazz, NULL);
  jmethodID method_id =
      base::android::MethodID::LazyGet<
      base::android::MethodID::TYPE_STATIC>(
      env, g_InnerStructA_clazz,
      "create",

"("
"J"
"I"
"Ljava/lang/String;"
")"
"Lorg/chromium/example/jni_generator/SampleForTests$InnerStructA;",
      &g_InnerStructA_create);

  jobject ret =
      env->CallStaticObjectMethod(g_InnerStructA_clazz,
          method_id, l, as_jint(i), s);
  jni_generator::CheckException(env);
  return base::android::ScopedJavaLocalRef<jobject>(env, ret);
}

static base::subtle::AtomicWord g_SampleForTests_addStructA = 0;
static void Java_SampleForTests_addStructA(JNIEnv* env, jobject obj, jobject a)
    {
  /* Must call RegisterNativesImpl()  */
  CHECK_CLAZZ(env, obj,
      g_SampleForTests_clazz);
  jmethodID method_id =
      base::android::MethodID::LazyGet<
      base::android::MethodID::TYPE_INSTANCE>(
      env, g_SampleForTests_clazz,
      "addStructA",

"("
"Lorg/chromium/example/jni_generator/SampleForTests$InnerStructA;"
")"
"V",
      &g_SampleForTests_addStructA);

     env->CallVoidMethod(obj,
          method_id, a);
  jni_generator::CheckException(env);

}

static base::subtle::AtomicWord g_SampleForTests_iterateAndDoSomething = 0;
static void Java_SampleForTests_iterateAndDoSomething(JNIEnv* env, jobject obj)
    {
  /* Must call RegisterNativesImpl()  */
  CHECK_CLAZZ(env, obj,
      g_SampleForTests_clazz);
  jmethodID method_id =
      base::android::MethodID::LazyGet<
      base::android::MethodID::TYPE_INSTANCE>(
      env, g_SampleForTests_clazz,
      "iterateAndDoSomething",

"("
")"
"V",
      &g_SampleForTests_iterateAndDoSomething);

     env->CallVoidMethod(obj,
          method_id);
  jni_generator::CheckException(env);

}

static base::subtle::AtomicWord g_InnerStructB_getKey = 0;
static jlong Java_InnerStructB_getKey(JNIEnv* env, jobject obj) {
  /* Must call RegisterNativesImpl()  */
  CHECK_CLAZZ(env, obj,
      g_InnerStructB_clazz, 0);
  jmethodID method_id =
      base::android::MethodID::LazyGet<
      base::android::MethodID::TYPE_INSTANCE>(
      env, g_InnerStructB_clazz,
      "getKey",

"("
")"
"J",
      &g_InnerStructB_getKey);

  jlong ret =
      env->CallLongMethod(obj,
          method_id);
  jni_generator::CheckException(env);
  return ret;
}

static base::subtle::AtomicWord g_InnerStructB_getValue = 0;
static base::android::ScopedJavaLocalRef<jstring>
    Java_InnerStructB_getValue(JNIEnv* env, jobject obj) {
  /* Must call RegisterNativesImpl()  */
  CHECK_CLAZZ(env, obj,
      g_InnerStructB_clazz, NULL);
  jmethodID method_id =
      base::android::MethodID::LazyGet<
      base::android::MethodID::TYPE_INSTANCE>(
      env, g_InnerStructB_clazz,
      "getValue",

"("
")"
"Ljava/lang/String;",
      &g_InnerStructB_getValue);

  jstring ret =
      static_cast<jstring>(env->CallObjectMethod(obj,
          method_id));
  jni_generator::CheckException(env);
  return base::android::ScopedJavaLocalRef<jstring>(env, ret);
}

// Step 3: RegisterNatives.

static const JNINativeMethod kMethodsSampleForTests[] = {
    { "nativeInit",
"("
"Ljava/lang/String;"
")"
"J", reinterpret_cast<void*>(Init) },
    { "nativeDestroy",
"("
"J"
")"
"V", reinterpret_cast<void*>(Destroy) },
    { "nativeGetDoubleFunction",
"("
")"
"D", reinterpret_cast<void*>(GetDoubleFunction) },
    { "nativeGetFloatFunction",
"("
")"
"F", reinterpret_cast<void*>(GetFloatFunction) },
    { "nativeSetNonPODDatatype",
"("
"Landroid/graphics/Rect;"
")"
"V", reinterpret_cast<void*>(SetNonPODDatatype) },
    { "nativeGetNonPODDatatype",
"("
")"
"Ljava/lang/Object;", reinterpret_cast<void*>(GetNonPODDatatype) },
    { "nativeMethod",
"("
"J"
")"
"I", reinterpret_cast<void*>(Method) },
    { "nativeMethodOtherP0",
"("
"J"
")"
"D", reinterpret_cast<void*>(MethodOtherP0) },
    { "nativeAddStructB",
"("
"J"
"Lorg/chromium/example/jni_generator/SampleForTests$InnerStructB;"
")"
"V", reinterpret_cast<void*>(AddStructB) },
    { "nativeIterateAndDoSomethingWithStructB",
"("
"J"
")"
"V", reinterpret_cast<void*>(IterateAndDoSomethingWithStructB) },
    { "nativeReturnAString",
"("
"J"
")"
"Ljava/lang/String;", reinterpret_cast<void*>(ReturnAString) },
};

static bool RegisterNativesImpl(JNIEnv* env) {
  g_InnerStructA_clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
      base::android::GetClass(env, kInnerStructAClassPath).obj()));
  g_SampleForTests_clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
      base::android::GetClass(env, kSampleForTestsClassPath).obj()));
  g_InnerStructB_clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
      base::android::GetClass(env, kInnerStructBClassPath).obj()));

  const int kMethodsSampleForTestsSize = arraysize(kMethodsSampleForTests);

  if (env->RegisterNatives(g_SampleForTests_clazz,
                           kMethodsSampleForTests,
                           kMethodsSampleForTestsSize) < 0) {
    jni_generator::HandleRegistrationError(
        env, g_SampleForTests_clazz, __FILE__);
    return false;
  }

  return true;
}

}  // namespace android
}  // namespace base

#endif  // org_chromium_example_jni_generator_SampleForTests_JNI
