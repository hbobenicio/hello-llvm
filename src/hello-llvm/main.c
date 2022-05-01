#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>

int main()
{
    LLVMContextRef ctx = LLVMContextCreate();
    LLVMModuleRef module = LLVMModuleCreateWithNameInContext("my-main-module", ctx);
    LLVMBuilderRef builder = LLVMCreateBuilderInContext(ctx);

    // types
    LLVMTypeRef int_8_type = LLVMInt8TypeInContext(ctx);
    LLVMTypeRef int_8_type_ptr = LLVMPointerType(int_8_type, 0);
    LLVMTypeRef int_32_type = LLVMInt32TypeInContext(ctx);

    // puts function
    LLVMTypeRef puts_func_args_type[] = {
        int_8_type_ptr
    };
    LLVMTypeRef  puts_func_type = LLVMFunctionType(int_32_type, puts_func_args_type, 1, false);
    LLVMValueRef puts_func = LLVMAddFunction(module, "puts", puts_func_type);

    // main function
    LLVMTypeRef  main_func_type = LLVMFunctionType(int_32_type, NULL, 0, false);
    LLVMValueRef main_func = LLVMAddFunction(module, "main", main_func_type);
    LLVMBasicBlockRef main_func_block = LLVMAppendBasicBlockInContext(ctx, main_func, "entry");
    LLVMPositionBuilderAtEnd(builder, main_func_block);

    LLVMValueRef puts_func_args[] = {
        LLVMBuildPointerCast(builder, // cast [14 x i8] type to int8 pointer
            LLVMBuildGlobalString(builder, "Hello, World!", "hello"), // build hello string constant
            int_8_type_ptr, "0"
        )
    };
    LLVMBuildCall(builder, puts_func, puts_func_args, 1, "i");
    LLVMBuildRet(builder, LLVMConstInt(int_32_type, 0, false));

    char *error = NULL;
    LLVMVerifyModule(module, LLVMAbortProcessAction, &error);
    if (error) {
        fprintf(stderr, "error: module verification failed: %s.\n", error);
        LLVMDisposeMessage(error);
        LLVMDisposeBuilder(builder);
        LLVMDisposeModule(module);
        LLVMContextDispose(ctx);
        return 1;
    }
    // LLVMDisposeMessage(error);

    //LLVMDumpModule(module); // dump module to STDOUT
    LLVMPrintModuleToFile(module, "hello-llvm.ll", NULL);

    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(module);
    LLVMContextDispose(ctx);
    return 0;
}
