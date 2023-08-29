// // #include "ast.h"
// // #include "llvm_wrapper.h"
// // #include "llvm/IR/Module.h"
// // #include <clocale>
// // #include <fstream>
// // #include <iostream>

// // #include <llvm-c/Core.h>

// // LLVMValueRef createStrxFunction(LLVMModuleRef moduleRef) {
// //   // create function type
// //   auto structTT = LLVMStructTypeInContext(
// //       LLVMGetGlobalContext(), (LLVMTypeRef[]){LLVMInt32Type(),
// LLVMInt32Type()},
// //       2, false);
// //   LLVMTypeRef paramTypes[] = {LLVMPointerType(structTT, 0)};
// //   LLVMTypeRef fnType = LLVMFunctionType(LLVMVoidType(), paramTypes, 1, 0);

// //   // create function
// //   LLVMValueRef strxFunc = LLVMAddFunction(moduleRef, "strx", fnType);
// //   LLVMBasicBlockRef entryBB = LLVMAppendBasicBlock(strxFunc, "entry");
// //   LLVMBuilderRef builderRef = LLVMCreateBuilder();
// //   LLVMPositionBuilderAtEnd(builderRef, entryBB);

// //   // 定义结构体类型
// //   LLVMTypeRef elementTypes[] = {LLVMInt32Type(), LLVMInt32Type()};
// //   LLVMTypeRef structType = LLVMStructType(elementTypes, 2, false);

// //   // 创建指向结构体类型的指针
// //   LLVMTypeRef structPointerType = LLVMPointerType(structType, 0);

// //   // 创建指向结构体类型的指针的指针
// //   LLVMTypeRef structPointerPointerType =
// LLVMPointerType(structPointerType, 0);

// //   // 分配内存并加载指针
// //   LLVMValueRef value = LLVMGetParam(strxFunc, 0); //
// 指向结构体类型的指针的指针
// //   LLVMValueRef allocaInst = LLVMBuildAlloca(
// //       builderRef, structPointerPointerType, "myStructPointerPointer");
// //   LLVMBuildStore(builderRef, value, allocaInst); //
// 将指针存储到分配的内存中
// //   LLVMValueRef loadedValue = LLVMBuildLoad2(
// //       builderRef, structPointerPointerType, allocaInst, "myLoad");

// //   // add return statement
// //   LLVMBuildRetVoid(builderRef);

// //   LLVMDisposeBuilder(builderRef);

// //   return strxFunc;
// // }

// // LLVMValueRef CreateSwitchFunc(LLVMModuleRef Module) {
// //   LLVMTypeRef int32Type = LLVMInt32Type();
// //   LLVMTypeRef  agg[] = {int32Type, LLVMInt1Type()};
// //   auto retType = LLVMStructTypeInContext(LLVMGetGlobalContext(), agg, 2,
// false);
// //   LLVMTypeRef oFun = LLVMFunctionType(retType, (LLVMTypeRef[]){int32Type,
// int32Type}, 2,false);
// //   LLVMAddFunction(Module, "llvm.sadd.with.overflow.i32",oFun);
// //   LLVMTypeRef funcType = LLVMFunctionType(int32Type,
// (LLVMTypeRef[]){int32Type,int32Type}, 2, false);
// //   LLVMValueRef func = LLVMAddFunction(Module, "switch_test", funcType);

// //   LLVMBasicBlockRef entry =
// LLVMAppendBasicBlockInContext(LLVMGetGlobalContext(), func, "entry");
// //   LLVMBasicBlockRef zero_case =
// LLVMAppendBasicBlockInContext(LLVMGetGlobalContext(), func, "zero_case");
// //   LLVMBasicBlockRef one_case =
// LLVMAppendBasicBlockInContext(LLVMGetGlobalContext(), func, "one_case");
// //   LLVMBasicBlockRef default_case =
// LLVMAppendBasicBlockInContext(LLVMGetGlobalContext(), func, "default_case");

// //   LLVMBuilderRef builder = LLVMCreateBuilder();
// //   LLVMPositionBuilderAtEnd(builder, entry);

// //   LLVMValueRef arg = LLVMGetParam(func, 0);
// //   LLVMValueRef result = LLVMBuildAlloca(builder, int32Type, "result");
// //   LLVMBuildStore(builder, LLVMConstInt(int32Type, 0, false), result);

// //   LLVMValueRef is_zero = LLVMBuildICmp(builder, LLVMIntEQ, arg,
// LLVMConstInt(int32Type, 0, false), "is_zero");

// //   LLVMBuildCondBr(builder, is_zero, zero_case, one_case);

// //   LLVMPositionBuilderAtEnd(builder, zero_case);
// //   LLVMBuildStore(builder, LLVMConstInt(int32Type, 100, false), result);
// //   LLVMBuildBr(builder, default_case);

// //   LLVMPositionBuilderAtEnd(builder, one_case);
// //   LLVMBuildStore(builder, LLVMConstInt(int32Type, 200, false), result);
// //   LLVMBuildBr(builder, default_case);

// //   LLVMPositionBuilderAtEnd(builder, default_case);
// //   LLVMValueRef load_result = LLVMBuildLoad2(builder,int32Type, result,
// "load_result");
// //   LLVMValueRef switchInst = LLVMBuildSwitch(builder, load_result,
// default_case, 2);

// //   LLVMAddCase(switchInst, LLVMConstInt(int32Type, 100, false), zero_case);
// //   LLVMAddCase(switchInst, LLVMConstInt(int32Type, 200, false), one_case);

// //   LLVMDisposeBuilder(builder);

// //   return func;
// // }

// // int main(int argc, char **argv) {
// //   initLLVM();
// //   auto m = createKModule("hello");
// //   KModule *module = unwrap(m);

// //   // module->init();
// //   // auto f = createStrxFunction(getModule(m));
// //   CreateSwitchFunc(getModule(m));
// //   LLVMDumpModule(getModule(m));
// //   destory(m);
// //   // auto v = vector<Param>();
// //   // auto func = FunctionAst(Pos(0, 0, 0), "main", LLType::Double, v);
// //   // auto function = func.codegen(*module);
// //   // module->writeOutput();
// //   // // std::string filename{"../hllo.txt"};
// //   // std::fstream s(filename, s.in | s.out);

// //   // std::string ss;
// //   // while(true) {
// //   //   if(s.eof()) break;
// //   //   s >> ss;
// //   //   std::cout << "char: " << ss << "    + "<< ss.size() << std::endl;
// //   // }
// //   // std::wstring str = L"你好";
// //   // std::wcout << str;
// //   // std::cout << std::endl;
// //   // for (auto iter = str.begin(); iter != str.end(); ++iter) {
// //   //   std::cout << *iter << std::endl;
// //   // }
// //   return 0;
// // }

// #include <stdio.h>
// #include <stdlib.h>
// #include "llvm-c/Core.h"
// #include "llvm-c/ExecutionEngine.h"
// #include "llvm-c/Target.h"
// #include "llvm-c/Types.h"
// #include "llvm-c/Analysis.h"

// int main() {
//   LLVMModuleRef module = LLVMModuleCreateWithName("my_module");

//   LLVMTypeRef printf_arg_types[] = { LLVMPointerType(LLVMInt8Type(), 0) };
//   LLVMTypeRef printf_type = LLVMFunctionType(LLVMInt32Type(),
//   printf_arg_types, 1, 1); LLVMValueRef printf_func = LLVMAddFunction(module,
//   "printf", printf_type);

//   LLVMTypeRef main_type = LLVMFunctionType(LLVMInt32Type(), NULL, 0, 0);
//   LLVMValueRef main_func = LLVMAddFunction(module, "main", main_type);
//   LLVMBasicBlockRef entry_block = LLVMAppendBasicBlock(main_func, "entry");

//   LLVMBuilderRef builder = LLVMCreateBuilder();
//   LLVMPositionBuilderAtEnd(builder, entry_block);

//   LLVMValueRef printf_arg1 = LLVMBuildGlobalStringPtr(builder, "Hello,
//   %s!\n", "printf_arg1");

//   LLVMValueRef printf_args[] = { printf_arg1 };

//   LLVMValueRef printf_result = LLVMBuildCall2(builder,printf_type,
//   printf_func, printf_args, 1, "printf"); LLVMBuildRet(builder,
//   LLVMConstInt(LLVMInt32Type(), 0, 0));

//   // char *error = NULL;
//   // LLVMVerifyModule(module, LLVMAbortProcessAction, &error);
//   // LLVMDisposeMessage(error);

//   // LLVMExecutionEngineRef engine;
//   // if (LLVMCreateJITCompilerForModule(&engine, module, 2, &error) != 0) {
//   //   fprintf(stderr, "error: %s\n", error);
//   //   LLVMDisposeMessage(error);
//   //   exit(EXIT_FAILURE);
//   // }

//   // int (*main_func_ptr)() = (int (*)())LLVMGetFunctionAddress(engine,
//   "main");
//   // int result = main_func_ptr();
//   // printf("Result: %d\n", result);

//   LLVMDumpModule(module);
//   LLVMDisposeBuilder(builder);
//   // LLVMDisposeExecutionEngine(engine);
//   LLVMDisposeModule(module);

//   return 0;
// }

#include <llvm-c/Core.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>
#include <vector>

int main() {
  // Initialize LLVM
  LLVMInitializeAllTargetInfos();
  LLVMInitializeAllTargets();
  LLVMInitializeAllTargetMCs();
  LLVMInitializeAllAsmPrinters();
  LLVMInitializeAllAsmParsers();

  // Create LLVM module and function
  LLVMModuleRef module = LLVMModuleCreateWithName("my_module");
  LLVMContextRef context = LLVMGetModuleContext(module);

  LLVMTypeRef paramType = LLVMStructCreateNamed(LLVMGetGlobalContext(), "Base");
  LLVMTypeRef pp = LLVMIntTypeInContext(context, 32);
std::vector<LLVMTypeRef> vv;
  vv.push_back(pp);
  vv.push_back(pp);
  vv.push_back(pp);
  vv.push_back(pp);
  vv.push_back(pp);
  vv.push_back(pp);

  LLVMStructSetBody(paramType, vv.data(), 6, false);
  LLVMTypeRef paramTypes[] = {paramType};
  LLVMTypeRef funcType = LLVMFunctionType(LLVMVoidType(), paramTypes, 1, 0);
  LLVMValueRef function = LLVMAddFunction(module, "test", funcType);
  LLVMBasicBlockRef entry = LLVMAppendBasicBlock(function, "entry");

  // Generate function body
  LLVMBuilderRef builder = LLVMCreateBuilder();
  LLVMPositionBuilderAtEnd(builder, entry);

  LLVMValueRef paramBase = LLVMGetParam(function, 0);

  // Do something with the Base struct here...
  // For now, let's just print the address of the struct
  LLVMValueRef addrParamBase = LLVMBuildAlloca(builder, paramType, "");
  LLVMBuildStore(builder, paramBase, addrParamBase);
  // LLVMBuildCall2(builder, LLVMGetNamedFunction(module, "printf"),
  //               &addrParamBase, 1, "");

  LLVMBuildRetVoid(builder);

  // Create target machine
  LLVMTargetRef target;
  char *error = NULL;
  if (LLVMGetTargetFromTriple(LLVMGetDefaultTargetTriple(), &target, &error) !=
      0) {
    fprintf(stderr, "Failed to get target for default triple\n");
    return 1;
  }

  LLVMTargetMachineRef targetMachine = LLVMCreateTargetMachine(
      target, LLVMGetDefaultTargetTriple(), "", "", LLVMCodeGenLevelDefault,
      LLVMRelocDefault, LLVMCodeModelDefault);

  // Generate target code
  char *error2 = NULL;
  LLVMPrintModuleToFile(module, "output.ll", &error2);
  if (error2) {
    fprintf(stderr, "Error: %s\n", error2);
    LLVMDisposeMessage(error2);
    return 1;
  }

  LLVMDisposeTargetMachine(targetMachine);
  LLVMDisposeBuilder(builder);
  LLVMDisposeModule(module);

  return 0;
}
