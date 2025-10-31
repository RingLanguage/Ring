%{


#include <cstdio>
#include <cstring>
#include <cstdarg>
#include "ring.hpp"
#include "y.tab.hpp"

char linebuf[1024];

extern "C" int yywrap(){
    return 1;
}


%}

%start LINE_COMMENT     BLOCK_COMMENT    LINE_STRING_LITERAL_STATE    BLOCK_STRING_LITERAL_STATE

%option yylineno

%%



<INITIAL>"typedef"      { package_unit_update_line_content(yytext); return TOKEN_TYPEDEF;        }
<INITIAL>"bool"         { package_unit_update_line_content(yytext); return TOKEN_BOOL;           }
<INITIAL>"int"          { package_unit_update_line_content(yytext); return TOKEN_INT;            }
<INITIAL>"int64"        { package_unit_update_line_content(yytext); return TOKEN_INT64;          }
<INITIAL>"double"       { package_unit_update_line_content(yytext); return TOKEN_DOUBLE;         }
<INITIAL>"string"       { package_unit_update_line_content(yytext); return TOKEN_STRING;         }
<INITIAL>"struct"       { package_unit_update_line_content(yytext); return TOKEN_STRUCT;         }
<INITIAL>"bind"         { package_unit_update_line_content(yytext); return TOKEN_BIND;           }
<INITIAL>"lambda"       { package_unit_update_line_content(yytext); return TOKEN_LAMBDA;         }
<INITIAL>"return"       { package_unit_update_line_content(yytext); return TOKEN_RETURN;         }
<INITIAL>"defer"        { package_unit_update_line_content(yytext); return TOKEN_DEFER;          }
<INITIAL>"range"        { package_unit_update_line_content(yytext); return TOKEN_RANGE;          }
<INITIAL>"in"           { package_unit_update_line_content(yytext); return TOKEN_IN;             }
<INITIAL>"launch"       { package_unit_update_line_content(yytext); return TOKEN_LAUNCH;         }

<INITIAL>"enum"         { package_unit_update_line_content(yytext); return TOKEN_ENUM;           }
<INITIAL>"class"        { package_unit_update_line_content(yytext); return TOKEN_CLASS;          }
<INITIAL>"private"      { package_unit_update_line_content(yytext); return TOKEN_PRIVATE;        }
<INITIAL>"public"       { package_unit_update_line_content(yytext); return TOKEN_PUBLIC;         }
<INITIAL>"@"            { package_unit_update_line_content(yytext); return TOKEN_ATTRIBUTE;      }
<INITIAL>"constructor"  { package_unit_update_line_content(yytext); return TOKEN_CONSTRUCTOR;    }

<INITIAL>"global"       { package_unit_update_line_content(yytext); return TOKEN_GLOBAL;         }
<INITIAL>"if"           { package_unit_update_line_content(yytext); return TOKEN_IF;             }
<INITIAL>"else"         { package_unit_update_line_content(yytext); return TOKEN_ELSE;           }
<INITIAL>"for"          { package_unit_update_line_content(yytext); return TOKEN_FOR;            }
<INITIAL>"do"           { package_unit_update_line_content(yytext); return TOKEN_DO;             }
<INITIAL>"break"        { package_unit_update_line_content(yytext); return TOKEN_BREAK;          }
<INITIAL>"jump"         { package_unit_update_line_content(yytext); return TOKEN_JUMP;           }
<INITIAL>"continue"     { package_unit_update_line_content(yytext); return TOKEN_CONTINUE;       }
<INITIAL>"null"         { package_unit_update_line_content(yytext); return TOKEN_NULL;           }

<INITIAL>"true"         { package_unit_update_line_content(yytext); return TOKEN_TRUE;           }
<INITIAL>"false"        { package_unit_update_line_content(yytext); return TOKEN_FALSE;          }

<INITIAL>"var"          { package_unit_update_line_content(yytext); return TOKEN_VAR;            }
<INITIAL>"auto"         { package_unit_update_line_content(yytext); return TOKEN_AUTO;           }
<INITIAL>"any"          { package_unit_update_line_content(yytext); return TOKEN_ANY;            }
<INITIAL>"const"        { package_unit_update_line_content(yytext); return TOKEN_CONST;          }
<INITIAL>"fn"           { package_unit_update_line_content(yytext); return TOKEN_FN;             }
<INITIAL>"new"          { package_unit_update_line_content(yytext); return TOKEN_NEW;            }
<INITIAL>"delete"       { package_unit_update_line_content(yytext); return TOKEN_DELETE;         }
<INITIAL>"."            { package_unit_update_line_content(yytext); return TOKEN_DOT;            }
<INITIAL>".."           { package_unit_update_line_content(yytext); return TOKEN_2DOT;           }
<INITIAL>"..."          { package_unit_update_line_content(yytext); return TOKEN_3DOT;           }
<INITIAL>"->"           { package_unit_update_line_content(yytext); return TOKEN_ARROW;          }

<INITIAL>"package"      { package_unit_update_line_content(yytext); return TOKEN_PACKAGE;        }
<INITIAL>"import"       { package_unit_update_line_content(yytext); return TOKEN_IMPORT;         }

<INITIAL>"+"            { package_unit_update_line_content(yytext); return TOKEN_ADD;            }
<INITIAL>"-"            { package_unit_update_line_content(yytext); return TOKEN_SUB;            }
<INITIAL>"*"            { package_unit_update_line_content(yytext); return TOKEN_MUL;            }
<INITIAL>"/"            { package_unit_update_line_content(yytext); return TOKEN_DIV;            }
<INITIAL>"%"            { package_unit_update_line_content(yytext); return TOKEN_MOD;            }
<INITIAL>"++"           { package_unit_update_line_content(yytext); return TOKEN_INCREASE;       }
<INITIAL>"--"           { package_unit_update_line_content(yytext); return TOKEN_DECREASE;       }
<INITIAL>"+="           { package_unit_update_line_content(yytext); return TOKEN_ADD_ASSIGN;     }
<INITIAL>"-="           { package_unit_update_line_content(yytext); return TOKEN_SUB_ASSIGN;     }
<INITIAL>"*="           { package_unit_update_line_content(yytext); return TOKEN_MUL_ASSIGN;     }
<INITIAL>"/="           { package_unit_update_line_content(yytext); return TOKEN_DIV_ASSIGN;     }
<INITIAL>"%="           { package_unit_update_line_content(yytext); return TOKEN_MOD_ASSIGN;     }

<INITIAL>"and"          { package_unit_update_line_content(yytext); return TOKEN_AND;            }
<INITIAL>"or"           { package_unit_update_line_content(yytext); return TOKEN_OR;             }
<INITIAL>"not"          { package_unit_update_line_content(yytext); return TOKEN_NOT;            }

<INITIAL>"=="           { package_unit_update_line_content(yytext); return TOKEN_EQ;             }
<INITIAL>"!="           { package_unit_update_line_content(yytext); return TOKEN_NE;             }

<INITIAL>">"            { package_unit_update_line_content(yytext); return TOKEN_GT;             }
<INITIAL>">="           { package_unit_update_line_content(yytext); return TOKEN_GE;             }
<INITIAL>"<"            { package_unit_update_line_content(yytext); return TOKEN_LT;             }
<INITIAL>"<="           { package_unit_update_line_content(yytext); return TOKEN_LE;             }

<INITIAL>"("            { package_unit_update_line_content(yytext); return TOKEN_LP;             }
<INITIAL>")"            { package_unit_update_line_content(yytext); return TOKEN_RP;             }
<INITIAL>"{"            { package_unit_update_line_content(yytext); return TOKEN_LC;             }
<INITIAL>"}"            { package_unit_update_line_content(yytext); return TOKEN_RC;             }
<INITIAL>"["            { package_unit_update_line_content(yytext); return TOKEN_LB;             }
<INITIAL>"]"            { package_unit_update_line_content(yytext); return TOKEN_RB;             }
<INITIAL>","            { package_unit_update_line_content(yytext); return TOKEN_COMMA;          }
<INITIAL>":"            { package_unit_update_line_content(yytext); return TOKEN_COLON;          }
<INITIAL>"::"           { package_unit_update_line_content(yytext); return TOKEN_2COLON;         }
<INITIAL>";"            { package_unit_update_line_content(yytext); return TOKEN_SEMICOLON;      }
<INITIAL>"?"            { package_unit_update_line_content(yytext); return TOKEN_QUESTION_MARK;  }
<INITIAL>"!"            { package_unit_update_line_content(yytext); return TOKEN_EXCLAM_MARK;    }
<INITIAL>"="            { package_unit_update_line_content(yytext); return TOKEN_ASSIGN;         }
<INITIAL>"#"            { package_unit_update_line_content(yytext); return TOKEN_NUM_SIGN;       }

<INITIAL>"~"            { package_unit_update_line_content(yytext); return TOKEN_BIT_NOT;        }
<INITIAL>"&"            { package_unit_update_line_content(yytext); return TOKEN_BIT_AND;        }
<INITIAL>"|"            { package_unit_update_line_content(yytext); return TOKEN_BIT_OR;         }
<INITIAL>"^"            { package_unit_update_line_content(yytext); return TOKEN_BIT_XOR;        }
<INITIAL>"<<"           { package_unit_update_line_content(yytext); return TOKEN_BIT_LSHIFT;     }
<INITIAL>">>"           { package_unit_update_line_content(yytext); return TOKEN_BIT_RSHIFT;     }

<INITIAL>[A-Za-z_][A-Za-z_0-9]* {
    debug_flex("[STATUS:INITIAL]\t line_num(%d),column_num(%d),identifier(%s)",
        package_unit_get_line_number(), 
        package_unit_get_column_number(), 
        yytext);

    package_unit_update_line_content(yytext);
    init_string_literal_buffer();
    unsigned int i = 0;
    for (i=0;i<strlen(yytext);i++) {
        string_literal_add_char(yytext[i]);
    }
    yylval.m_identifier_str = get_string_literal();
    reset_string_literal_buffer();
    
    return IDENTIFIER;
}
<INITIAL>([0-9]*(_[0-9]+)*)|"0" {
    debug_flex("[STATUS:INITIAL]\t line_num(%d),column_num(%d),int_literal(%s)",
        package_unit_get_line_number(), 
        package_unit_get_column_number(), 
        yytext);
    package_unit_update_line_content(yytext);

    // TODO: 注意，这里没有去释放，是否会造成大量内存的浪费
    unsigned int length = strlen(yytext);
    char* decimal_str = (char*)mem_alloc(get_front_mem_pool(), ROUND_UP8(length));

    // erase '_'
    char* dst = decimal_str;
    for(unsigned int i = 0; i < length; i++) {
        if (yytext[i] == '_') {
            continue;
        }
        *dst = yytext[i];
        dst++;
    }

    yylval.m_literal_interface = decimal_str;
    return INT_LITERAL;
}
<INITIAL>([0-9]*(_[0-9]+)*L)|"0L" { 
    // 十进制常量，支持下划线分隔
    debug_flex("[STATUS:INITIAL]\t line_num(%d),column_num(%d),int64_literal(%s)",
        package_unit_get_line_number(), 
        package_unit_get_column_number(), 
        yytext);
    package_unit_update_line_content(yytext);

    // TODO: 注意，这里没有去释放，是否会造成大量内存的浪费
    unsigned int length = strlen(yytext);
    char* decimal_str = (char*)mem_alloc(get_front_mem_pool(), ROUND_UP8(length));

    // erase '_'
    char* dst = decimal_str;
    for(unsigned int i = 0; i < length; i++) {
        if (yytext[i] == '_') {
            continue;
        }
        *dst = yytext[i];
        dst++;
    }

    yylval.m_literal_interface = decimal_str;
    return INT64_LITERAL;
}

<INITIAL>0b[01]+(_[01]+)*L? { 
    // 二进制常量，支持下划线分隔
    debug_flex("[STATUS:INITIAL]\t line_num(%d),column_num(%d),binary_literal(%s)",
        package_unit_get_line_number(), 
        package_unit_get_column_number(), 
        yytext);
    package_unit_update_line_content(yytext);

    unsigned long long value = 0;
    unsigned int length = strlen(yytext);
    for(unsigned int i = 0; i < length; i++) {
        if (yytext[i] == '_' || yytext[i] == 'L') {
            continue;
        }
        if ('0' <= yytext[i] && yytext[i] <= '1') {
            value = (value << 1) | (yytext[i] - '0');
        }
    }

    // TODO: 注意，这里没有去释放，是否会造成大量内存的浪费
    char* decimal_str = (char*)mem_alloc(get_front_mem_pool(), 32);
    snprintf(decimal_str, 32, "%llu", value);

    yylval.m_literal_interface = decimal_str;
    if(yytext[length-1] == 'L') {
        return INT64_LITERAL;
    } else {
        return INT_LITERAL;
    }
}

<INITIAL>0o[0-7]+(_[0-7]+)*L? {
    // 八进制常量，支持下划线分隔
    debug_flex("[STATUS:INITIAL]\t line_num(%d),column_num(%d),hex_literal(%s)",
        package_unit_get_line_number(), 
        package_unit_get_column_number(), 
        yytext);
    package_unit_update_line_content(yytext);

    unsigned long long value = 0;
    unsigned int length = strlen(yytext);
    for(unsigned int i = 0; i < length; i++) {
        if (yytext[i] == '_' || yytext[i] == 'L') {
            continue;
        }
        if ('0' <= yytext[i] && yytext[i] <= '7') {
            value = (value << 3) | (yytext[i] - '0');
        } 
    }

    // TODO: 注意，这里没有去释放，是否会造成大量内存的浪费
    char *decimal_str = (char*)mem_alloc(get_front_mem_pool(), 32);
    snprintf(decimal_str, 32, "%llu", value);

    yylval.m_literal_interface = decimal_str;
    if(yytext[length-1] == 'L') {
        return INT64_LITERAL;
    } else {
        return INT_LITERAL;
    }
}

<INITIAL>0x[0-9a-fA-F]+(_[0-9a-fA-F]+)*L? {
    // 十六进制常量，支持下划线分隔
    debug_flex("[STATUS:INITIAL]\t line_num(%d),column_num(%d),hex_literal(%s)",
        package_unit_get_line_number(), 
        package_unit_get_column_number(), 
        yytext);
    package_unit_update_line_content(yytext);

    unsigned long long value = 0;
    unsigned int length = strlen(yytext);
    for(unsigned int i = 0; i < length; i++) {
        if (yytext[i] == '_' || yytext[i] == 'L') {
            continue;
        }
        if ('0' <= yytext[i] && yytext[i] <= '9') {
            value = (value << 4) | (yytext[i] - '0');
        } else if ('a' <= yytext[i] && yytext[i] <= 'f') {
            value = (value << 4) | (yytext[i] - 'a' + 10);
        } else if ('A' <= yytext[i] && yytext[i] <= 'F') {
            value = (value << 4) | (yytext[i] - 'A' + 10);
        }
    }

    // TODO: 注意，这里没有去释放，是否会造成大量内存的浪费
    char *decimal_str = (char*)mem_alloc(get_front_mem_pool(), 32);
    snprintf(decimal_str, 32, "%llu", value);

    yylval.m_literal_interface = decimal_str;
    if(yytext[length-1] == 'L') {
        return INT64_LITERAL;
    } else {
        return INT_LITERAL;
    }
}

<INITIAL>[0-9]+(_[0-9]+)*\.[0-9]+(_[0-9]+)* {
    debug_flex("[STATUS:INITIAL]\t line_num(%d),column_num(%d),double_literal(%s)",
        package_unit_get_line_number(), 
        package_unit_get_column_number(), 
        yytext);
    package_unit_update_line_content(yytext);

    // TODO: 注意，这里没有去释放，是否会造成大量内存的浪费
    unsigned int length = strlen(yytext);
    char* decimal_str = (char*)mem_alloc(get_front_mem_pool(), ROUND_UP8(length));

    // erase '_'
    char* dst = decimal_str;
    for(unsigned int i = 0; i < length; i++) {
        if (yytext[i] == '_') {
            continue;
        }
        *dst = yytext[i];
        dst++;
    }

    yylval.m_literal_interface = decimal_str;
    return DOUBLE_LITERAL;
}

<INITIAL>\" {
    package_unit_update_line_content(yytext);
    init_string_literal_buffer();
    BEGIN LINE_STRING_LITERAL_STATE;
}
<INITIAL>"`" {
    package_unit_update_line_content(yytext);
    init_string_literal_buffer();
    BEGIN BLOCK_STRING_LITERAL_STATE;
}
<INITIAL>\n.* {
    debug_flex("[STATUS:INITIAL]\t new line(LF) line_number(%d)\n\n", package_unit_get_line_number());

    package_unit_update_line_content((char*)"\n");

    package_unit_reset_column_number();
    package_unit_reset_line_content();
    package_unit_increa_line_number();

    memcpy(linebuf, yytext+1, sizeof(linebuf));
    yyless(1);
}
<INITIAL>\r\n.* {
    debug_flex("[STATUS:INITIAL]\t new line(CRLF) line_number(%d)\n\n", package_unit_get_line_number());

    package_unit_update_line_content((char*)"\r\n");

    package_unit_reset_column_number();
    package_unit_reset_line_content();
    package_unit_increa_line_number();

    memcpy(linebuf, yytext+1, sizeof(linebuf));
    yyless(2);
}
<INITIAL>"//" {
    package_unit_update_line_content(yytext);
    BEGIN LINE_COMMENT;
}
<INITIAL>"/*" {
    package_unit_update_line_content(yytext);
    BEGIN BLOCK_COMMENT;
}
<INITIAL>. {
    package_unit_update_line_content(yytext);
}



 /* BLOCK_STRING_LITERAL_STATE */
<BLOCK_STRING_LITERAL_STATE>\` {
    package_unit_update_line_content(yytext);
    yylval.m_literal_interface = get_string_literal();
    BEGIN INITIAL;
    return STRING_LITERAL;
}
<BLOCK_STRING_LITERAL_STATE>"\\`" {
    package_unit_update_line_content(yytext);
    string_literal_add_char('`');
    BEGIN BLOCK_STRING_LITERAL_STATE;
}
<BLOCK_STRING_LITERAL_STATE>. {
    package_unit_update_line_content(yytext);
    string_literal_add_char(yytext[0]);
    BEGIN BLOCK_STRING_LITERAL_STATE;
}
<BLOCK_STRING_LITERAL_STATE>\n {
    package_unit_update_line_content(yytext);
    string_literal_add_char('\n');
    package_unit_reset_column_number();
    package_unit_reset_line_content();
    package_unit_increa_line_number();
    BEGIN BLOCK_STRING_LITERAL_STATE;
}
 /* BLOCK_STRING_LITERAL_STATE */



 /* LINE_STRING_LITERAL_STATE */
<LINE_STRING_LITERAL_STATE>\" {
    package_unit_update_line_content(yytext);
    yylval.m_literal_interface = get_string_literal();
    BEGIN INITIAL;
    return STRING_LITERAL;
}
<LINE_STRING_LITERAL_STATE>\\n {
    package_unit_update_line_content(yytext);
    string_literal_add_char('\n');
    BEGIN LINE_STRING_LITERAL_STATE;
}
<LINE_STRING_LITERAL_STATE>\\r {
    package_unit_update_line_content(yytext);
    string_literal_add_char('\r');
    BEGIN LINE_STRING_LITERAL_STATE;
}
<LINE_STRING_LITERAL_STATE>\\t {
    package_unit_update_line_content(yytext);
    string_literal_add_char('\t');
    BEGIN LINE_STRING_LITERAL_STATE;
}
<LINE_STRING_LITERAL_STATE>\n {
    // Ring-Compiler-Error-Report unclosed string literal
    if(true) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
                 "unclosed string literal, match `\\n` in string literal; E:%d.",
                 ERROR_UNCLOSED_STRING_LITERAL);
        compile_adv_buf = sprintf_string(
                " \"<string-literal>\" only use to definite line string, `<string-literal>` can definite block string.");

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(package_unit_get_line_number()),
            .line_number             = package_unit_get_line_number(),
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }
        

    yyless(1);
    BEGIN LINE_STRING_LITERAL_STATE;
}
<LINE_STRING_LITERAL_STATE>\r\n {
    // Ring-Compiler-Error-Report unclosed string literal
    if(true) {
        DEFINE_ERROR_REPORT_STR;

        compile_err_buf = sprintf_string(
                 "unclosed string literal, match `\\r\\n` in string literal; E:%d.",
                 ERROR_UNCLOSED_STRING_LITERAL);
        compile_adv_buf = sprintf_string(
                " \"<string-literal>\" only use to definite line string, `<string-literal>` can definite block string.");

        ErrorReportContext context = {
            .package                 = nullptr,
            .package_unit            = get_package_unit(),
            .source_file_name        = get_package_unit()->current_file_name,
            .line_content            = package_unit_get_line_content(package_unit_get_line_number()),
            .line_number             = package_unit_get_line_number(),
            .column_number           = package_unit_get_column_number(),
            .error_message           = std::string(compile_err_buf),
            .advice                  = std::string(compile_adv_buf),
            .report_type             = ERROR_REPORT_TYPE_COLL_ERR,
            .ring_compiler_file      = (char*)__FILE__,
            .ring_compiler_file_line = __LINE__,
        };
        ring_compile_error_report(&context);
    }


    yyless(2);
    BEGIN LINE_STRING_LITERAL_STATE;
}
<LINE_STRING_LITERAL_STATE>"\\\"" {
    package_unit_update_line_content(yytext);
    string_literal_add_char('"');
    BEGIN LINE_STRING_LITERAL_STATE;
}
<LINE_STRING_LITERAL_STATE>. {
    package_unit_update_line_content(yytext);
    string_literal_add_char(yytext[0]);
    BEGIN LINE_STRING_LITERAL_STATE;
}
 /* LINE_STRING_LITERAL_STATE */




 /* LINE_COMMENT */
<LINE_COMMENT>\n {
    package_unit_update_line_content(yytext);
    reset_string_literal_buffer();

    package_unit_reset_column_number();
    package_unit_reset_line_content();
    package_unit_increa_line_number();
    
    BEGIN INITIAL;
}
<LINE_COMMENT>\r\n {
    package_unit_update_line_content(yytext);
    reset_string_literal_buffer();

    package_unit_reset_column_number();
    package_unit_reset_line_content();
    package_unit_increa_line_number();

    BEGIN INITIAL;
}
<LINE_COMMENT>. {
    package_unit_update_line_content(yytext);
    string_literal_add_char(yytext[0]);
}
 /* LINE_COMMENT */



 /* BLOCK_COMMENT */
<BLOCK_COMMENT>\n {
    package_unit_update_line_content(yytext);
    reset_string_literal_buffer();
    package_unit_reset_column_number();
    package_unit_reset_line_content();
    package_unit_increa_line_number();
}
<BLOCK_COMMENT>\r\n {
    package_unit_update_line_content(yytext);
    reset_string_literal_buffer();
    package_unit_reset_column_number();
    package_unit_reset_line_content();
    package_unit_increa_line_number();
}
<BLOCK_COMMENT>. {
    package_unit_update_line_content(yytext);
    string_literal_add_char(yytext[0]);
}
<BLOCK_COMMENT>"*/" {
    package_unit_update_line_content(yytext);
    reset_string_literal_buffer();
    BEGIN INITIAL;
}
 /* BLOCK_COMMENT */


%%


extern char *yytext;





