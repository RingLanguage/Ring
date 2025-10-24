#include "ring.hpp"


// 都得使用这个方法
size_t bc_common_undump(RingUndumpContext* ctx, void* dst, size_t size) {
    memcpy(dst, &(ctx->input_buffer[ctx->read_pos]), size);
    ctx->read_pos += size;
    return size;
}

void bc_undump_block(RingUndumpContext* ctx, void* dst, size_t size) {
    bc_common_undump(ctx, dst, size);
}

#define bc_undump_vector(ctx, vector, n) bc_undump_block(ctx, vector, (n) * sizeof((vector)[0]))

RVM_Byte bc_undump_byte(RingUndumpContext* ctx) {
    RVM_Byte data = ctx->input_buffer[ctx->read_pos++];
    return data;
}


void check_str_literal(RingUndumpContext* ctx, const char* s, const char* err_msg) {
    // TODO: 这里实现的不太好
    char   literal_buf[MAX_LITERAL_LEN] = {0};
    size_t len                          = bc_undump_byte(ctx);

    bc_undump_vector(ctx, literal_buf, len);
    if (memcmp(s, literal_buf, len) != 0) {
        printf("checkliteral valid---------- %s\n", err_msg);
    }
}


static size_t bc_undump_unsigned(RingUndumpContext* ctx, size_t limit) {
    size_t x = 0;
    int    b;
    limit >>= 7;
    do {
        b = bc_undump_byte(ctx);
        if (x >= limit)
            printf("bc_undump_unsigned integer overflow\n");
        x = (x << 7) | (b & 0x7f);
    } while ((b & 0x80) == 0);
    return x;
}


static size_t bc_undump_size(RingUndumpContext* ctx) {
    return bc_undump_unsigned(ctx, ~(size_t)0);
}


#define bc_check_byte(ctx, byte, err_msg)             \
    if (bc_undump_byte(ctx) != byte) {                \
        printf("check byte failed, `%s`\n", err_msg); \
    }


#define bc_undump_value(ctx, value) bc_undump_vector(ctx, &value, 1)
int bc_undump_int(RingUndumpContext* ctx) {
    int data;
    bc_undump_value(ctx, data);
    return data;
}
#define bc_check_int(ctx, int_value, err_msg)        \
    if (bc_undump_int(ctx) != int_value) {           \
        printf("check int failed, `%s`\n", err_msg); \
    }


long long bc_undump_int64(RingUndumpContext* ctx) {
    long long data;
    bc_undump_value(ctx, data);
    return data;
}
#define bc_check_int64(ctx, int64_value, err_msg)      \
    if (bc_undump_int64(ctx) != int64_value) {         \
        printf("check int64 failed, `%s`\n", err_msg); \
    }


double bc_undump_double(RingUndumpContext* ctx) {
    double data;
    bc_undump_value(ctx, data);
    return data;
}
#define bc_check_double(ctx, double_value, err_msg)     \
    if (bc_undump_double(ctx) != double_value) {        \
        printf("check double failed, `%s`\n", err_msg); \
    }


char* bc_undump_cstring(RingUndumpContext* ctx) {
    size_t len = bc_undump_int64(ctx);
    char*  str = (char*)mem_alloc(NULL_MEM_POOL, len + 1); // TODO:
    bc_undump_vector(ctx, str, len);
    str[len] = '\0';
    return str;
}

// TODO: 添加 -g 调试选项
void bc_check_header(RingUndumpContext* ctx) {
    bc_check_byte(ctx, RING_MAGIC, "check header magic failed");
    check_str_literal(ctx, RING_SIGNATURE, "check signature");
    bc_check_byte(ctx, RING_MAJOR_VERSION, "check major version failed");
    bc_check_byte(ctx, RING_MINOR_VERSION, "check minor version failed");
    check_str_literal(ctx, RINGC_DATA, "check data failed");

    bc_check_byte(ctx, RING_MAGIC, "check magic failed");
    bc_check_byte(ctx, sizeof(RVM_Byte), "check sizeof(RVM_Byte) failed");
    bc_check_byte(ctx, sizeof(int), "check sizeof(int) failed");
    bc_check_byte(ctx, sizeof(long long), "check sizeof(long long) failed");
    bc_check_byte(ctx, sizeof(double), "check sizeof(double) failed");


    bc_check_byte(ctx, RING_MAGIC, "check magic failed");
    bc_check_int(ctx, 0x5678, "check 0x5678 failed");
    bc_check_int64(ctx, 0x1234567890abcdef, "check 0x1234567890abcdef failed");
    bc_check_double(ctx, 370.5, "check 370.5 failed");
}

#define BC_CHECK_END(ctx)                            \
    if (ctx->read_pos != ctx->input_buffer.size()) { \
        printf("check end failed\n");                \
    }

#define PRINT_STAT(value, fmt) printf(#value ": " fmt "\n", value)


void bc_undump_constant(RingUndumpContext* ctx, RVM_Constant* constant) {
    ConstantPoolType type = (ConstantPoolType)bc_undump_byte(ctx);
    constant->type        = type;

    switch (type) {
    case CONSTANTPOOL_TYPE_INT:
        constant->u.int_value = bc_undump_int(ctx);
        printf("dumpConstantPool int:%d\n", constant->u.int_value);
        break;
    case CONSTANTPOOL_TYPE_INT64:
        constant->u.int64_value = bc_undump_int64(ctx);
        printf("dumpConstantPool int64:%lld\n", constant->u.int64_value);
        break;
    case CONSTANTPOOL_TYPE_DOUBLE:
        constant->u.double_value = bc_undump_double(ctx);
        printf("dumpConstantPool double:%f\n", constant->u.double_value);
        break;
    case CONSTANTPOOL_TYPE_STRING: {
        char*       str          = bc_undump_cstring(ctx);
        RVM_String* rvm_str      = (RVM_String*)mem_alloc(NULL_MEM_POOL, sizeof(RVM_String));
        rvm_str->data            = str;
        rvm_str->length          = strlen(str);
        constant->u.string_value = rvm_str;
        printf("dumpConstantPool string: %s\n", str);
        break;
    }
    default:
        assert(false && "unknown constant type");
        break;
    }
}

void bc_undump_function(RingUndumpContext* ctx, RVM_Function* function) {
    printf("----------undump a function----------\n");

    function->identifier = bc_undump_cstring(ctx);
    function->type       = (RVMFunctionType)bc_undump_byte(ctx);
    PRINT_STAT(function->identifier, "%s");

    if (function->type == RVM_FUNCTION_TYPE_DERIVE) {

        unsigned int code_size = bc_undump_size(ctx);
        RVM_Byte*    code_list = (RVM_Byte*)mem_alloc(NULL_MEM_POOL,
                                                      sizeof(RVM_Byte) * code_size);
        bc_undump_vector(ctx, code_list, code_size);


        // TODO: 这个地方需要重新规划
        DeriveFunction* derive_func  = (DeriveFunction*)mem_alloc(NULL_MEM_POOL, sizeof(DeriveFunction));
        derive_func->code_list       = code_list;
        derive_func->code_size       = code_size;
        derive_func->need_stack_size = 0;
        derive_func->code_line_map   = std::vector<RVM_SourceCodeLineMap>{};

        function->u.derive_func      = derive_func;
    } else if (function->type == RVM_FUNCTION_TYPE_NATIVE) {
    }
}

void bc_undump_package(RingUndumpContext* ctx, Package_Executer* package_executer) {
    printf("----------undump a package----------\n");
    char*        package_name         = bc_undump_cstring(ctx);
    unsigned int constant_pool_size   = bc_undump_size(ctx);
    unsigned int global_variable_size = bc_undump_size(ctx);
    unsigned int function_size        = bc_undump_size(ctx);
    unsigned int class_size           = bc_undump_size(ctx);
    unsigned int bootloader_code_size = bc_undump_size(ctx);
    RVM_Byte*    bootloader_code_list = (RVM_Byte*)mem_alloc(NULL_MEM_POOL,
                                                             sizeof(RVM_Byte) * bootloader_code_size);
    bc_undump_vector(ctx, bootloader_code_list, bootloader_code_size);
    unsigned int exist_main_func                 = bc_undump_size(ctx);
    unsigned int main_func_index                 = bc_undump_size(ctx);
    unsigned int exist_global_init_func          = bc_undump_size(ctx);
    unsigned int global_init_func_index          = bc_undump_size(ctx);
    unsigned int estimate_runtime_stack_capacity = bc_undump_size(ctx);


    PRINT_STAT(package_name, "%s");
    PRINT_STAT(constant_pool_size, "%u");
    PRINT_STAT(global_variable_size, "%u");
    PRINT_STAT(function_size, "%u");
    PRINT_STAT(class_size, "%u");
    PRINT_STAT(bootloader_code_size, "%u");
    PRINT_STAT(exist_main_func, "%u");
    PRINT_STAT(main_func_index, "%u");
    PRINT_STAT(exist_global_init_func, "%u");
    PRINT_STAT(global_init_func_index, "%u");
    PRINT_STAT(estimate_runtime_stack_capacity, "%u");


    RVM_ConstantPool* constant_pool = package_executer->constant_pool;
    constant_pool->size             = constant_pool_size;
    constant_pool->list             = (RVM_Constant*)mem_alloc(NULL_MEM_POOL,
                                                               sizeof(RVM_Constant) * constant_pool_size);
    for (unsigned int i = 0; i < constant_pool_size; i++) {
        bc_undump_constant(ctx, &constant_pool->list[i]);
    }


    RVM_Function* function_list = (RVM_Function*)mem_alloc(NULL_MEM_POOL,
                                                           sizeof(RVM_Function) * function_size);
    for (unsigned int i = 0; i < function_size; i++) {
        bc_undump_function(ctx, &function_list[i]);
    }


    bc_check_byte(ctx, RING_MAGIC, "check magic failed");


    package_executer->package_name         = package_name;
    package_executer->constant_pool        = constant_pool;
    package_executer->global_variable_size = global_variable_size;
    package_executer->global_variable_list = nullptr; // TODO:
    package_executer->function_size        = function_size;
    package_executer->function_list        = function_list;

    package_executer->bootloader_code_size = bootloader_code_size;
    package_executer->bootloader_code_list = bootloader_code_list;
}

void bc_undump_root(RingUndumpContext* ctx) {
    bc_check_header(ctx);


    int package_count = bc_undump_size(ctx);
    PRINT_STAT(package_count, "%d");

    ExecuterEntry* executer_entry = executer_entry_create();
    executer_entry->package_executer_list.resize(package_count);

    for (int i = 0; i < package_count; i++) {
        Package_Executer* package_executer = package_executer_create(executer_entry, nullptr, 0);

        bc_undump_package(ctx, package_executer);

        if (str_eq(package_executer->package_name, "main")) {
            executer_entry->main_package_executer = package_executer;
        }

        executer_entry->package_executer_list[i] = package_executer;
    }

    ctx->executer_entry = executer_entry;

    BC_CHECK_END(ctx);
}


// TODO: 测试读取大文件
std::vector<RVM_Byte> bc_load_binary_file(const std::string& filename) {
    std::vector<RVM_Byte> buffer;
    FILE*                 file = fopen(filename.c_str(), "rb");
    if (!file) {
        printf("undump from binary file failed: cannot open file: %s, err:%s\n",
               filename.c_str(), strerror(errno));
        return buffer;
    }

    const size_t CHUNK_SIZE = 4096;
    RVM_Byte     chunk[CHUNK_SIZE];

    while (true) {
        size_t bytes_read = fread(chunk, 1, CHUNK_SIZE, file);
        if (bytes_read > 0) {
            buffer.insert(buffer.end(), chunk, chunk + bytes_read);
        }

        if (bytes_read < CHUNK_SIZE) {
            if (feof(file)) {
                break;
            } else if (ferror(file)) {
                printf("undump from binary file failed: read err:%s\n", strerror(errno));
                break;
            }
        }
    }

    fclose(file);
    return buffer;
}