#include "ring.hpp"


void bc_dump_block(RingDumpContext* ctx, RVM_Byte* data, size_t size) {
    if (size > 0) {
        size_t old_size = ctx->output_buffer.size();
        ctx->output_buffer.reserve(old_size + size);
        ctx->output_buffer.insert(ctx->output_buffer.end(), data, data + size);
    }
}

#define bc_dump_str_literal(ctx, data)                    \
    bc_dump_byte(ctx, (sizeof(data) - sizeof(RVM_Byte))); \
    bc_dump_block(ctx, (RVM_Byte*)data, (sizeof(data) - sizeof(RVM_Byte)))

#define bc_dump_vector(ctx, vector, n) bc_dump_block(ctx, (RVM_Byte*)vector, ((n) * sizeof((vector)[0])))

void bc_dump_byte(RingDumpContext* ctx, RVM_Byte data) {
    bc_dump_block(ctx, (RVM_Byte*)&data, 1);
}


#define bc_dump_value(ctx, value) bc_dump_vector(ctx, &value, 1)

void bc_dump_int(RingDumpContext* ctx, int data) {
    bc_dump_value(ctx, data);
}
void bc_dump_uint(RingDumpContext* ctx, unsigned int data) {
    bc_dump_value(ctx, data);
}
void bc_dump_int64(RingDumpContext* ctx, long long data) {
    bc_dump_value(ctx, data);
}
void bc_dump_double(RingDumpContext* ctx, double data) {
    bc_dump_value(ctx, data);
}

#define bc_dump_string(ctx, c_str, len) \
    bc_dump_int64(ctx, len);            \
    bc_dump_block(ctx, (RVM_Byte*)c_str, (len));

#define bc_dump_cstring(ctx, c_str) \
    bc_dump_string(ctx, c_str, strlen(c_str));

#define bc_dump_cppstring(ctx, str) \
    bc_dump_int64(ctx, str.size()); \
    bc_dump_block(ctx, (RVM_Byte*)str.data(), str.size());


/*
 ** 'dumpSize' buffer size: each byte can store up to 7 bits. (The "+6"
 ** rounds up the division.)
 */
#define DIBS ((sizeof(size_t) * CHAR_BIT + 6) / 7)

static void bc_dump_size(RingDumpContext* ctx, size_t x) {
    RVM_Byte buff[DIBS];
    int      n = 0;
    do {
        buff[DIBS - (++n)] = x & 0x7f; /* fill buffer in reverse order */
        x >>= 7;
    } while (x != 0);
    buff[DIBS - 1] |= 0x80; /* mark last byte */
    bc_dump_vector(ctx, buff + DIBS - n, n);
}

void bc_dump_header(RingDumpContext* ctx) {
    bc_dump_byte(ctx, RING_MAGIC);
    bc_dump_str_literal(ctx, RING_SIGNATURE);
    bc_dump_byte(ctx, RING_MAJOR_VERSION);
    bc_dump_byte(ctx, RING_MINOR_VERSION);
    bc_dump_str_literal(ctx, RINGC_DATA);

    bc_dump_byte(ctx, RING_MAGIC);
    bc_dump_byte(ctx, sizeof(RVM_Byte));
    bc_dump_byte(ctx, sizeof(RVM_Int));
    bc_dump_byte(ctx, sizeof(RVM_Int64));
    bc_dump_byte(ctx, sizeof(RVM_Double));


    bc_dump_byte(ctx, RING_MAGIC);
    bc_dump_int(ctx, 0x5678);
    bc_dump_int64(ctx, 0x1234567890abcdef);
    bc_dump_double(ctx, 370.5);
}

void bc_dump_constant(RingDumpContext* ctx, RVM_Constant* constant) {
    bc_dump_byte(ctx, constant->type);
    switch (constant->type) {
    case CONSTANTPOOL_TYPE_INT:
        bc_dump_int(ctx, constant->u.int_value);
        break;
    case CONSTANTPOOL_TYPE_INT64:
        bc_dump_int64(ctx, constant->u.int64_value);
        break;
    case CONSTANTPOOL_TYPE_DOUBLE:
        bc_dump_double(ctx, constant->u.double_value);
        break;
    case CONSTANTPOOL_TYPE_STRING:
        bc_dump_cstring(ctx, constant->u.string_value->data);
        break;
    default:
        assert(false && "unknown constant type");
        break;
    }
}

void bc_dump_function(RingDumpContext* ctx, RVM_Function* function) {
    debug_bytecode("----------dump a function----------\n");

    debug_bytecode("function_name: %s\n", function->identifier);


    bc_dump_cppstring(ctx, function->source_file);
    bc_dump_uint(ctx, function->start_line_number);
    bc_dump_uint(ctx, function->end_line_number);

    bc_dump_uint(ctx, function->parameter_size);
    bc_dump_uint(ctx, function->return_value_size);
    bc_dump_uint(ctx, function->local_variable_size);
    bc_dump_uint(ctx, function->free_value_size);
    bc_dump_uint(ctx, function->estimate_runtime_stack_capacity);

    bc_dump_cstring(ctx, function->identifier);
    bc_dump_byte(ctx, function->type);

    if (function->type == RVM_FUNCTION_TYPE_DERIVE) {
        RVM_Byte*    code_list = nullptr;
        unsigned int code_size = 0;

        code_list              = function->u.derive_func->code_list;
        code_size              = function->u.derive_func->code_size;

        debug_bytecode("code_size: %u\n", code_size);

        bc_dump_size(ctx, code_size);
        bc_dump_vector(ctx, code_list, code_size);
    } else if (function->type == RVM_FUNCTION_TYPE_NATIVE) {
    }
}

void bc_dump_package(RingDumpContext* ctx, Package_Executer* package_executer) {
    debug_bytecode("----------dump a package----------\n");
    debug_bytecode("package_name: %s\n", package_executer->package_name);
    debug_bytecode("constant_pool_size:%u\n", package_executer->constant_pool->size);
    debug_bytecode("global_variable_size:%u\n", package_executer->global_variable_size);
    debug_bytecode("function_size:%u\n", package_executer->function_size);
    debug_bytecode("class_size:%u\n", package_executer->class_size);
    debug_bytecode("bootloader_code_size:%u\n", package_executer->bootloader_code_size);
    debug_bytecode("exist_main_func:%d\n", package_executer->exist_main_func);
    debug_bytecode("main_func_index:%u\n", package_executer->main_func_index);
    debug_bytecode("exist_global_init_func:%d\n", package_executer->exist_global_init_func);
    debug_bytecode("global_init_func_index:%u\n", package_executer->global_init_func_index);
    debug_bytecode("estimate_runtime_stack_capacity:%u\n", package_executer->estimate_runtime_stack_capacity);

    bc_dump_cstring(ctx, package_executer->package_name);
    bc_dump_size(ctx, package_executer->constant_pool->size);
    bc_dump_size(ctx, package_executer->global_variable_size);
    bc_dump_size(ctx, package_executer->function_size);
    bc_dump_size(ctx, package_executer->class_size);
    bc_dump_size(ctx, package_executer->bootloader_code_size);
    bc_dump_vector(ctx, package_executer->bootloader_code_list, package_executer->bootloader_code_size);
    bc_dump_size(ctx, package_executer->exist_main_func);
    bc_dump_size(ctx, package_executer->main_func_index);
    bc_dump_size(ctx, package_executer->exist_global_init_func);
    bc_dump_size(ctx, package_executer->global_init_func_index);
    bc_dump_size(ctx, package_executer->estimate_runtime_stack_capacity);


    for (unsigned int i = 0; i < package_executer->constant_pool->size; i++) {
        bc_dump_constant(ctx, &package_executer->constant_pool->list[i]);
    }

    for (unsigned int i = 0; i < package_executer->function_size; i++) {
        bc_dump_function(ctx, &package_executer->function_list[i]);
    }


    bc_dump_byte(ctx, RING_MAGIC);
}

void bc_dump_root(RingDumpContext* ctx) {
    bc_dump_header(ctx);

    ExecuterEntry* executer_entry = ctx->package_executer->executer_entry;


    // package 的数量
    debug_bytecode("package_count: %lu\n", executer_entry->package_executer_list.size());
    bc_dump_size(ctx, executer_entry->package_executer_list.size());

    for (Package_Executer* package_executer : executer_entry->package_executer_list) {
        bc_dump_package(ctx, package_executer);
    }
}

void bc_dump_binary_file(const std::vector<RVM_Byte>& data, const std::string& filename) {
    FILE* file = fopen(filename.c_str(), "wb");
    if (!file) {
        printf("dump to binary file failed: cannot open file: %s, err:%s\n",
               filename.c_str(), strerror(errno));
        return;
    }

    size_t written = fwrite(data.data(), sizeof(RVM_Byte), data.size(), file);
    fclose(file);

    if (written != data.size()) {
        printf("dump to binary file: write data not correct\n");
    }
}


/*
 * bc_display_binary_file
 *
 * dis play binary data in a human-readable format
 */
void bc_display_binary_file(const std::vector<RVM_Byte>& data) {
    printf("----------------------------------------------------------------------------------------------------------\n");
    printf("Offset    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F  Decoded Text\n");
    printf("--------- -----------------------------------------------  -----------------------------------------------\n");

    for (size_t i = 0; i < data.size(); i++) {
        // 每行开头显示偏移量
        if (i % 16 == 0) {
            printf("%08lX: ", i);
        }

        printf("%02X ", RVM_Byte(data[i]));

        // 每行结尾打印ASCII表示
        if (i % 16 == 15 || i == data.size() - 1) {
            // 填充不完整的行
            int remaining = 15 - (i % 16);
            for (int j = 0; j < remaining; j++) {
                printf("   ");
            }

            printf(" ");

            // 打印ASCII字符
            for (size_t j = i - (i % 16); j <= i; j++) {
                if (data[j] >= 32 && data[j] <= 126) { // 可打印字符
                    printf(" %c ", data[j]);
                } else {
                    printf(" . "); // 非打印字符用点表示
                }
            }
            printf("\n");
        }
    }
    printf("----------------------------------------------------------------------------------------------------------\n");
}