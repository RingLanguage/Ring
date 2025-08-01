%{


#include "ring.hpp"
#include <cstdio>
#define YYDEBUG 1

int yylex();


%}


%locations                       // 开启locations
%glr-parser                      // 使用 GLR 解析
%expect    4                     // legitimate 0 shift/reduce conflicts
%expect-rr 5                     // legitimate 0 reduce/reduce conflicts

// 在 array_literal_expression 的 class_type_specifier dimension_expression TOKEN_LC expression_list TOKEN_RC
// 存在 reduce/reduce conflicts, 需要使用 %code_completion 进行处理

%define parse.error detailed     // 调用 yyerror时, 可以传递更多的细节进去

%require "3.8.2"                 // bison 版本 要求

%start  translation_unit_list    // 解析的开始位置

// %define api.pure true

// %define api.location.type {location_t}


%union {
    char*                               m_comment_value;
    char*                               m_literal_interface;
    char*                               m_identifier_str;
    Identifier*                         m_identifier;
    Statement*                          m_statement_list;
    Expression*                         m_expression;
    AssignExpression*                   m_assign_expression;
    FunctionCallExpression*             m_function_call_expression;
    MemberCallExpression*               m_member_call_expression;
    ArrayLiteralExpression*             m_array_literal_expression;
    ClassObjectLiteralExpression*       m_class_object_literal_expression;
    FieldInitExpression*                m_field_init_expression;
    ImmediateInvokFuncExpression*       m_iife;
    ArgumentList*                       m_argument_list;
    FunctionTuple*                      m_function_tuple;
    Function*                           m_function_definition;
    AnonymousFunc*                      m_anonymous_func;
    Parameter*                          m_parameter_list;
    IfStatement*                        m_if_statement;
    ElseIfStatement*                    m_elseif_statement;
    ForStatement*                       m_for_statement;
    DoForStatement*                     m_dofor_statement;
    FunctionReturnList*                 m_return_list;
    Block*                              m_block;
    CastExpression*                     m_cast;
    BreakStatement*                     m_break_statement;
    TagDefinitionStatement*             m_tag_definition_statement;
    JumpTagStatement*                   m_jump_tag_statement;
    DeferStatement*                     m_defer_statement;
    ContinueStatement*                  m_continue_statement;
    ReturnStatement*                    m_return_statement;
    DimensionExpression*                m_dimension_expression;
    SubDimensionExpression*             m_sub_dimension_expression;
    SliceExpression*                    m_slice_expression;
    SubSliceExpression*                 m_sub_slice_expression;

    RangeExpression*                    m_range_expression;
    SubStepRangeExpression*             m_sub_step_range_expression;
    SubLinearRangeExpression*           m_sub_linear_range_expression;

    Package*                            m_package;

    EnumDeclaration*                    m_enum_declaration;
    EnumItemDeclaration*                m_enum_item_declaration;

    TypeAlias*                          m_type_alias_def;
    ClassDefinition*                    m_class_definition;
    ClassMemberDeclaration*             m_class_member_declaration;
    FieldMember*                        m_field_member;
    MethodMember*                       m_method_member;

    AttributeInfo*                      m_attribute_info;

    TypeSpecifier*                      m_type_specifier;
    Ring_BasicType                      m_basic_type_specifier;
    AttributeType                       m_attribute;

    ExpressionType                      m_expression_type;

    Location*                           m_location;
}

%token TOKEN_TYPEDEF
%token TOKEN_BOOL
%token TOKEN_INT 
%token TOKEN_INT64
%token TOKEN_DOUBLE 
%token TOKEN_STRING 
%token TOKEN_STRUCT 
%token TOKEN_BIND 
%token TOKEN_LAMBDA 
%token TOKEN_RETURN
%token TOKEN_DEFER
%token TOKEN_RANGE
%token TOKEN_IN
%token TOKEN_LAUNCH

%token TOKEN_ENUM
%token TOKEN_CLASS
%token TOKEN_PRIVATE
%token TOKEN_PUBLIC
%token TOKEN_ATTRIBUTE
%token TOKEN_CONSTRUCTOR

%token TOKEN_GLOBAL
%token TOKEN_IF
%token TOKEN_ELSE
%token TOKEN_FOR
%token TOKEN_DO
%token TOKEN_BREAK
%token TOKEN_JUMP
%token TOKEN_CONTINUE
%token TOKEN_NULL

%token TOKEN_TRUE
%token TOKEN_FALSE

%token TOKEN_VAR
%token TOKEN_AUTO
%token TOKEN_ANY
%token TOKEN_CONST
%token TOKEN_FN
%token TOKEN_NEW
%token TOKEN_DELETE
%token TOKEN_DOT
%token TOKEN_2DOT
%token TOKEN_3DOT
%token TOKEN_ARROW

%token TOKEN_PACKAGE
%token TOKEN_IMPORT

%token TOKEN_ADD
%token TOKEN_SUB
%token TOKEN_MUL
%token TOKEN_DIV
%token TOKEN_MOD
%token TOKEN_INCREASE
%token TOKEN_DECREASE
%token TOKEN_ADD_ASSIGN
%token TOKEN_SUB_ASSIGN
%token TOKEN_MUL_ASSIGN
%token TOKEN_DIV_ASSIGN
%token TOKEN_MOD_ASSIGN

%token TOKEN_AND
%token TOKEN_OR
%token TOKEN_NOT

%token TOKEN_EQ
%token TOKEN_NE

%token TOKEN_GT
%token TOKEN_GE
%token TOKEN_LT
%token TOKEN_LE

%token TOKEN_LP 
%token TOKEN_RP 
%token TOKEN_LC 
%token TOKEN_RC 
%token TOKEN_LB 
%token TOKEN_RB 
%token TOKEN_COMMA 
%token TOKEN_COLON
%token TOKEN_2COLON
%token TOKEN_SEMICOLON 
%token TOKEN_QUESTION_MARK
%token TOKEN_EXCLAM_MARK
%token TOKEN_ASSIGN
%token TOKEN_NUM_SIGN

%token TOKEN_BIT_NOT
%token TOKEN_BIT_AND
%token TOKEN_BIT_OR
%token TOKEN_BIT_XOR
%token TOKEN_BIT_LSHIFT
%token TOKEN_BIT_RSHIFT

%token INT_LITERAL
%token INT64_LITERAL
%token DOUBLE_LITERAL
%token STRING_LITERAL
%token IDENTIFIER

%type <m_literal_interface> INT_LITERAL INT64_LITERAL DOUBLE_LITERAL STRING_LITERAL TOKEN_TRUE TOKEN_FALSE
%type <m_identifier_str>  IDENTIFIER
%type <m_identifier>      identifier
%type <m_identifier>      identifier_list
%type <m_statement_list> statement statement_list
%type <m_statement_list> multi_variable_definition_statement
%type <m_statement_list> global_variable_definition global_variable_definition_list
%type <m_expression> expression expression_list expression_list_maybe_empty
%type <m_expression> postfix_expression
%type <m_expression> unitary_expression
%type <m_expression> ternary_expression
%type <m_expression> launch_expression
%type <m_expression> primary_not_new_array
%type <m_expression> primary_not_call_expression
%type <m_expression> primary_new_creation
%type <m_expression> member_expression
%type <m_expression> basic_value_literal_expression
%type <m_expression> derive_value_literal_expression
%type <m_expression> primary_expression
%type <m_expression> expression_arithmetic_operation_additive 
%type <m_expression> expression_arithmetic_operation_multiplicative 
%type <m_expression> logical_expression_or
%type <m_expression> logical_expression_and
%type <m_expression> relational_expression
%type <m_expression> bitwise_expression_or
%type <m_expression> bitwise_expression_xor
%type <m_expression> bitwise_expression_and
%type <m_expression> bitwise_expression_shift
%type <m_expression> equality_expression
%type <m_expression> maybe_empty_expression
%type <m_expression> left_value_expression_list
%type <m_expression> left_value_expression
%type <m_expression> right_value_expression_list
%type <m_expression> right_value_expression
%type <m_expression> slice_literal_expression
%type <m_assign_expression> assign_expression

%type <m_function_call_expression> function_call_expression
%type <m_member_call_expression>   member_call_expression

%type <m_array_literal_expression> array_literal_expression
%type <m_class_object_literal_expression> class_object_literal_expression
%type <m_field_init_expression> class_field_init_element_list class_field_init_element
%type <m_argument_list> argument_list argument
%type <m_function_tuple>      function_tuple
%type <m_function_definition> function_definition
%type <m_expression>             closure_definition
%type <m_iife>             iife_expression
%type <m_parameter_list> parameter_list 
                            parameter 
                            parameter_list_maybe_empty 
                            simple_parameter_list_maybe_empty 
                            simple_parameter_list 
                            simple_parameter
%type <m_if_statement> if_statement
%type <m_for_statement> for_statement
%type <m_dofor_statement> dofor_statement
%type <m_elseif_statement> elseif_statement_list
%type <m_elseif_statement> elseif_statement
%type <m_return_list> return_list
%type <m_block> block
%type <m_break_statement> break_statement
%type <m_tag_definition_statement> tag_definition_statement
%type <m_jump_tag_statement> jump_tag_statement
%type <m_defer_statement> defer_statement
%type <m_continue_statement> continue_statement
%type <m_return_statement> return_statement
%type <m_dimension_expression> dimension_expression
%type <m_sub_dimension_expression> sub_dimension_expression sub_dimension_expression_list

%type <m_slice_expression> slice_expression
%type <m_sub_slice_expression> sub_slice_expression

%type <m_range_expression>          range_expression
%type <m_sub_step_range_expression>     sub_step_range_expression
%type <m_sub_linear_range_expression>   sub_linear_range_expression


%type <m_package> package_definition

%type <m_enum_declaration>      enum_declaration
%type <m_enum_item_declaration> enum_item_declaration_list enum_item_declaration

%type <m_class_definition>         class_definition
%type <m_type_alias_def>           type_alias_def
%type <m_class_member_declaration> class_member_declaration_list class_member_declaration
%type <m_field_member>             field_member
%type <m_method_member>            method_member

%type <m_attribute_info> attribute_list attribute

%type <m_type_specifier>        type_specifier class_type_specifier
%type <m_basic_type_specifier>  basic_type_specifier 

%type <m_expression_type>      self_incr_decr_token


%%


translation_unit_list
    : translation_unit
    | translation_unit_list translation_unit
    ;

translation_unit
    : package_definition
    {
        debug_bison_info_with_green("[RULE::translation_unit:1]\t ");
    }
    | import_block
    {
        debug_bison_info_with_green("[RULE::translation_unit:2]\t ");
    }
    | global_variable_definition_block
    {
        debug_bison_info_with_green("[RULE::translation_unit:4]\t ");
    }
    | definition_or_statement
    {
        debug_bison_info_with_green("[RULE::translation_unit:3]\t ");
    }
    ;

package_definition
    : TOKEN_PACKAGE identifier
    {
        $$ = create_package_info($2);
    }
    ;

import_block 
    : TOKEN_IMPORT TOKEN_LC import_package_list TOKEN_RC
    | TOKEN_IMPORT TOKEN_LC TOKEN_RC
    ;
import_package_list
    : import_package_info 
    | import_package_list import_package_info
    ;
import_package_info
    : IDENTIFIER TOKEN_SEMICOLON
    {
        import_package_list_add_item($1, nullptr);
    }
    | IDENTIFIER TOKEN_ARROW IDENTIFIER TOKEN_SEMICOLON
    {
        import_package_list_add_item($1, $3);
    }
    | error
    {
        ring_grammar_error(GRAMMAR_IMPORT_PACKAGE);
        yylex();
        yyin_move_to_next_line();
        yyerrok;
    }
    ;

global_variable_definition_block
    : TOKEN_GLOBAL TOKEN_LC { $<m_block>$ = start_new_block(); } global_variable_definition_list TOKEN_RC
    {
        debug_bison_info_with_green("[RULE::global_variable_definition_block:2]\t ");
        finish_global_block($<m_block>3, $4);
    }
    | TOKEN_GLOBAL TOKEN_LC TOKEN_RC
    {
        debug_bison_info_with_green("[RULE::global_variable_definition_block:1]\t ");
    }
    ;
global_variable_definition_list
    : global_variable_definition
    {
        debug_bison_info_with_green("[RULE::global_variable_definition_list:1]\t ");
        $$ = $1;
    }
    | global_variable_definition_list global_variable_definition
    {
        debug_bison_info_with_green("[RULE::global_variable_definition_list:2]\t ");
        $$ = statement_list_add_item($1, $2);
    }
    ;
global_variable_definition
    : multi_variable_definition_statement TOKEN_SEMICOLON
    {
        debug_bison_info_with_green("[RULE::global_variable_definition]\t ");
        $$ = $1;
    }
    ;


definition_or_statement
    : function_definition
    {
        debug_bison_info_with_green("[RULE::statement:function_definition]\t ");
        add_function_definition(nullptr, $1);
    }
    | attribute_list function_definition
    {
        debug_bison_info_with_green("[RULE::statement:function_definition]\t ");
        add_function_definition($1, $2);
    }
    | class_definition
    {
        debug_bison_info_with_green("[RULE::statement:class_definition]\t ");
    }
    | enum_declaration
    {
        debug_bison_info_with_green("[RULE::statement:enum_declaration]\t ");
    }
    | type_alias_def TOKEN_SEMICOLON
    {
        debug_bison_info_with_green("[RULE::statement:type_definition]\t ");
    }
    ;

class_definition
    : TOKEN_TYPEDEF IDENTIFIER TOKEN_ASSIGN TOKEN_CLASS {$<m_class_definition>$ = start_class_definition($2);} TOKEN_LC class_member_declaration_list TOKEN_RC
    {
        debug_bison_info_with_green("[RULE::class_definition]\t ");
        $<m_class_definition>$ = finish_class_definition($<m_class_definition>5, $7, $2);
        $<m_type_alias_def>$ = add_type_alias_class($2, $<m_class_definition>5);
    }
    ;

/*
* parameter_list_maybe_empty 需要单独需要一个版本
* 需要兼容 typedef function(var int a) Func1;
*         typedef function(var int) Func1;
*         typedef function(int) Func1;
*/
type_alias_def
    : TOKEN_TYPEDEF identifier TOKEN_ASSIGN TOKEN_FN TOKEN_LP simple_parameter_list_maybe_empty TOKEN_RP
    {
        debug_bison_info_with_green("[RULE::type_definition]\t ");
         $<m_type_alias_def>$ = add_type_alias_func($6, nullptr, $2);
    }
    | TOKEN_TYPEDEF identifier TOKEN_ASSIGN TOKEN_FN TOKEN_LP simple_parameter_list_maybe_empty TOKEN_RP TOKEN_ARROW TOKEN_LP return_list TOKEN_RP
    {
        debug_bison_info_with_green("[RULE::type_definition]\t ");
         $<m_type_alias_def>$ = add_type_alias_func($6, $10, $2);
    }
    ;

class_member_declaration_list 
    : class_member_declaration
    {
        $$ = $1;
    }
    | class_member_declaration_list class_member_declaration
    {
        $$ = class_member_declaration_list_add_item($1, $2);
    }
    ;

class_member_declaration
    : field_member
    {
        debug_bison_info_with_green("[RULE::class_member_declaration:field_member]\t ");
        $$ = create_class_member_field_declaration(ACCESS_PUBLIC, $1);
    }
    | method_member
    {
        debug_bison_info_with_green("[RULE::class_member_declaration:method_member]\t ");
        $$ = create_class_member_method_declaration(ACCESS_PUBLIC, $1);
    }
    ;

field_member
    : TOKEN_VAR type_specifier identifier_list TOKEN_SEMICOLON
    {
        debug_bison_info_with_green("[RULE::filed_member]\t ");
        $$ = create_class_member_field($2, $3);
    }
    ;

method_member
    : TOKEN_FN identifier TOKEN_LP parameter_list_maybe_empty TOKEN_RP block
    {
        debug_bison_info_with_green("[RULE::method_member]\t ");
        $$ = create_class_member_method(FUNCTION_TYPE_UNKNOW, $2, $4, nullptr, $6);
    }
    | TOKEN_FN identifier TOKEN_LP parameter_list_maybe_empty TOKEN_RP TOKEN_SEMICOLON
    {
        debug_bison_info_with_green("[RULE::method_member]\t ");
        $$ = create_class_member_method(FUNCTION_TYPE_UNKNOW, $2, $4, nullptr, nullptr);
    }
    | TOKEN_FN identifier TOKEN_LP parameter_list_maybe_empty TOKEN_RP TOKEN_ARROW TOKEN_LP return_list TOKEN_RP block
    {
        debug_bison_info_with_green("[RULE::method_member]\t ");
        $$ = create_class_member_method(FUNCTION_TYPE_UNKNOW, $2, $4, $8, $10);
    }
    | TOKEN_FN identifier TOKEN_LP parameter_list_maybe_empty TOKEN_RP TOKEN_ARROW TOKEN_LP return_list TOKEN_RP TOKEN_SEMICOLON
    {
        debug_bison_info_with_green("[RULE::method_member]\t ");
        $$ = create_class_member_method(FUNCTION_TYPE_UNKNOW, $2, $4, $8, nullptr);
    }
    ;


attribute_list
    : attribute
    {
    }
    | attribute_list attribute
    {
        $$ = attribute_info_add_item($1, $2);
    }
    ;

attribute
    : TOKEN_ATTRIBUTE IDENTIFIER
    {
        $$ = create_attribute_info($2);
    }
    ;

enum_declaration
    : TOKEN_TYPEDEF identifier TOKEN_ASSIGN TOKEN_ENUM type_specifier 
    TOKEN_LC 
    { $<m_enum_declaration>$ = start_enum_declaration($5, $2); } 
    enum_item_declaration_list 
    TOKEN_RC
    {
        $<m_enum_declaration>$ = finish_enum_declaration($<m_enum_declaration>7, $8);
        $<m_type_alias_def>$ = add_type_alias_enum($2, $<m_enum_declaration>7);
    }
    ;

enum_item_declaration_list
    : enum_item_declaration
    {
        $$ = $1;
    }
    | enum_item_declaration_list enum_item_declaration
    {
        $$ = enum_item_declaration_list_add_item($1, $2);
    }
    ;

enum_item_declaration
    : IDENTIFIER TOKEN_ASSIGN expression TOKEN_SEMICOLON
    {
        $$ = create_enum_item_declaration($1, $3);
    }
    ;

maybe_empty_expression
    :
    {
        $$ = nullptr;
    }
    | expression
    | assign_expression
    {
        debug_bison_info_with_green("[RULE:expression-assign_expression]\t ");
        $$ = create_expression_assign($1);
    }
    ;

statement_list
    : statement
    {
        debug_bison_info_with_green("[RULE::statement:statement]\t ");
        $$ = $1;
    }
    | statement_list statement
    {
        // 这个地方不应该加在全局的 statment_list 中
        // 应该只放到函数的 statement_list 中
        debug_bison_info_with_green("[RULE::statement:statement_list]\t ");
        $$ = statement_list_add_item($1, $2);
    }
    ;
statement
    : maybe_empty_expression TOKEN_SEMICOLON
    {
        debug_bison_info_with_green("[RULE::statement:expression]\t ");
        $$ = create_statemen_from_expression($1);
    }
    | multi_variable_definition_statement TOKEN_SEMICOLON
    | if_statement
    {
        debug_bison_info_with_green("[RULE::statement:if_statement]\t ");
        $$ = create_statement_from_if($1);
    }
    | for_statement
    {
        debug_bison_info_with_green("[RULE::statement:for_statement]\t ");
        $$ = create_statement_from_for($1);
    }
    | dofor_statement TOKEN_SEMICOLON
    {
        debug_bison_info_with_green("[RULE::statement:dofor_statement]\t ");
        $$ = create_statement_from_dofor($1);
    }
    | break_statement TOKEN_SEMICOLON
    {
        $$ = create_statement_from_break($1);
    }
    | continue_statement TOKEN_SEMICOLON
    {
        $$ = create_statement_from_continue($1);
    }
    | return_statement
    {
        debug_bison_info_with_green("[RULE::statement:return_statement]\t ");
        $$ = create_statement_from_return($1);
    }
    | tag_definition_statement
    {
        debug_bison_info_with_green("[RULE::statement:jump_tag_definition_statement]\t ");
        $$ = create_statement_from_tag_definition($1);
    }
    | jump_tag_statement
    {
        debug_bison_info_with_green("[RULE::statement:jump_tag_statement]\t ");
        $$ = create_statement_from_jump_tag($1);
    }
    | defer_statement
    {
        debug_bison_info_with_green("[RULE::statement:defer_statement]\t ");
        $$ = create_statement_from_defer($1);
    }
    ;


if_statement
    : TOKEN_IF  right_value_expression  block // if condition_expression {}
    {
        $$ = create_if_statement($2, $3, nullptr, nullptr);
    }
    | TOKEN_IF  right_value_expression  block TOKEN_ELSE block // if condition_expression {} else {}
    {
        $$ = create_if_statement($2, $3, nullptr, $5);
    }
    | TOKEN_IF  right_value_expression  block elseif_statement_list // if condition_expression {} else if condition_expression {} elseif {}
    {
        $$ = create_if_statement($2, $3, $4, nullptr);
    }
    | TOKEN_IF  right_value_expression  block elseif_statement_list TOKEN_ELSE block // if condition_expression {} else if condition_expression {} else if {} else {}
    {
        $$ = create_if_statement($2, $3, $4, $6);
    }
    ;

elseif_statement_list
    : elseif_statement {}
    | elseif_statement_list elseif_statement
    {
        $$ = elseif_statement_add_item($1, $2);
    }
    ;

elseif_statement
    : TOKEN_ELSE TOKEN_IF  expression  block
    {
        $$ = create_elseif_statement($3, $4);
    }
    ;

for_statement
    : TOKEN_FOR      maybe_empty_expression TOKEN_SEMICOLON maybe_empty_expression TOKEN_SEMICOLON maybe_empty_expression    block
    {
        $$ = create_for_ternary_statement($2, $4, $6, $7);
    }
    | TOKEN_FOR      left_value_expression_list TOKEN_ASSIGN TOKEN_RANGE range_expression      block
    {
        $$ = create_for_range_statement($2, $5, $6);
    }
    ;

dofor_statement
    : TOKEN_DO  maybe_empty_expression  block TOKEN_FOR  TOKEN_LP maybe_empty_expression TOKEN_SEMICOLON maybe_empty_expression TOKEN_RP
    {
        $$ = create_dofor_statement($2, $3, $6, $8);
    }
    ;


break_statement
    : TOKEN_BREAK 
    {
        debug_bison_info_with_green("[RULE::break_statement]\t ");
        $$ = create_break_statement(nullptr);
    }
    | TOKEN_BREAK INT_LITERAL
    {
        debug_bison_info_with_green("[RULE::break_statement]\t ");
        $$ = create_break_statement($2);
    }
    ;

continue_statement
    : TOKEN_CONTINUE 
    {
        debug_bison_info_with_green("[RULE::continue_statement]\t ");
        $$ = create_continue_statement();
    }
    ;

return_statement
    : TOKEN_RETURN                 TOKEN_SEMICOLON
    {
        debug_bison_info_with_green("[RULE::return_statement]\t ");
        $$ = create_return_statement(nullptr);
    }
    | TOKEN_RETURN expression_list TOKEN_SEMICOLON
    {
        debug_bison_info_with_green("[RULE::return_statement]\t ");
        $$ = create_return_statement($2);
    }
    ;


multi_variable_definition_statement
    : TOKEN_VAR type_specifier identifier_list
    {
        debug_bison_info_with_green("[RULE::multi_variable_definition_statement]\t ");
        $$ = create_multi_declaration_statement($2, $3, nullptr);
    }
    | TOKEN_VAR type_specifier identifier_list TOKEN_ASSIGN expression_list
    {
        debug_bison_info_with_green("[RULE::multi_variable_definition_statement]\t ");
        $$ = create_multi_declaration_statement($2, $3, $5);
    }
    ;

defer_statement
    : TOKEN_DEFER iife_expression
    {
        debug_bison_info_with_green("[RULE::defer_statement]\t ");
        $$ = create_defer_statement($2);
    }

function_definition
    : TOKEN_FN identifier TOKEN_LP parameter_list_maybe_empty TOKEN_RP block
    {
        debug_bison_info_with_green("[RULE::function_definition]\t ");
        $$ = create_function_definition(FUNCTION_TYPE_DERIVE, $2, $4, nullptr, $6);
    }
    | TOKEN_FN identifier TOKEN_LP parameter_list_maybe_empty TOKEN_RP TOKEN_SEMICOLON
    {
        debug_bison_info_with_green("[RULE::function_definition]\t ");
        $$ = create_function_definition(FUNCTION_TYPE_DERIVE, $2, $4, nullptr, nullptr);
    }
    | TOKEN_FN identifier TOKEN_LP parameter_list_maybe_empty TOKEN_RP TOKEN_ARROW TOKEN_LP return_list TOKEN_RP block
    {
        debug_bison_info_with_green("[RULE::function_definition]\t ");
        $$ = create_function_definition(FUNCTION_TYPE_DERIVE, $2, $4, $8, $10);
    }
    | TOKEN_FN identifier TOKEN_LP parameter_list_maybe_empty TOKEN_RP TOKEN_ARROW TOKEN_LP return_list TOKEN_RP TOKEN_SEMICOLON
    {
        debug_bison_info_with_green("[RULE::function_definition]\t ");
        $$ = create_function_definition(FUNCTION_TYPE_DERIVE, $2, $4, $8, nullptr);
    }
    ;


closure_definition
    : TOKEN_FN function_tuple
    {
        debug_bison_info_with_green("[RULE::anonymous_function]\t ");
        $$ = create_expression_anonymous_func((AnonymousFunc*)($2));
    }
    ;

// TODO: 后续 closure function method 都使用这个
function_tuple
    : TOKEN_LP parameter_list_maybe_empty TOKEN_RP {$<m_location>$ = a_location();} block
    {
        debug_bison_info_with_green("[RULE::function_tuple:1]\t ");
        $$ = create_function_tuple($<m_location>4, $2, nullptr, $5);
    }
    | TOKEN_LP parameter_list_maybe_empty TOKEN_RP TOKEN_ARROW TOKEN_LP return_list TOKEN_RP {$<m_location>$ = a_location();} block
    {
        debug_bison_info_with_green("[RULE::function_tuple:2]\t ");
        $$ = create_function_tuple($<m_location>8, $2, $6, $9);
    }
    ;


parameter_list_maybe_empty
    : 
    {
        $$ = nullptr;
    }
    | parameter
    {
        debug_bison_info_with_green("[RULE::parameter_list_maybe_empty]\t ");
    }
    | parameter_list TOKEN_COMMA parameter
    {
        debug_bison_info_with_green("[RULE::parameter_list_maybe_empty]\t ");
        $$ = parameter_list_add_statement($1, $3);
    }
    ;
parameter_list
    : parameter
    {
        debug_bison_info_with_green("[RULE::parameter_list]\t ");
    }
    | parameter_list TOKEN_COMMA parameter
    {
        debug_bison_info_with_green("[RULE::parameter_list]\t ");
        $$ = parameter_list_add_statement($1, $3);
    }
    ;
parameter
    : TOKEN_VAR type_specifier IDENTIFIER
    {
        debug_bison_info_with_green("[RULE::parameter]\t ");
        $$ = create_parameter($2, $3, false);
    }
    | TOKEN_VAR type_specifier TOKEN_3DOT IDENTIFIER
    {
        debug_bison_info_with_green("[RULE::parameter:TOKEN_3DOT]\t ");
        $$ = create_parameter($2, $4, true);
    }
    ;

simple_parameter_list_maybe_empty
    : 
    {
        $$ = nullptr;
    }
    | simple_parameter
    {
        debug_bison_info_with_green("[RULE::parameter_list_maybe_empty]\t ");
    }
    | simple_parameter_list TOKEN_COMMA simple_parameter
    {
        debug_bison_info_with_green("[RULE::parameter_list_maybe_empty]\t ");
        $$ = parameter_list_add_statement($1, $3);
    }
    ;
simple_parameter_list
    : simple_parameter
    {

    }
    | simple_parameter_list TOKEN_COMMA simple_parameter
    {
        $$ = parameter_list_add_statement($1, $3);
    }
    ;
simple_parameter
    : type_specifier
    {
        $$ = create_parameter($1, nullptr, false);
    }
    | type_specifier TOKEN_3DOT
    {
        $$ = create_parameter($1, nullptr, true);
    }
    ;


return_list
    : type_specifier
    {
        $$ = create_function_return_list($1);
    }
    | return_list TOKEN_COMMA type_specifier
    {
        $$ = function_return_list_add_item($1, $3);
    }
    ;

block
    : TOKEN_LC { $<m_block>$ = start_new_block(); } statement_list TOKEN_RC
    {
        debug_bison_info_with_green("[RULE::block: statment_list]\t ");
        $<m_block>$ = finish_block($<m_block>2, $3);
    }
    | TOKEN_LC TOKEN_RC
    {
        debug_bison_info_with_green("[RULE::block: empty statement_list]\t ");
        $$ = finish_block(start_new_block(), nullptr);
    }
    ;


type_specifier
    : basic_type_specifier
    {
        debug_bison_info_with_green("[RULE::type_specifier:basic_type_specifier]");
        $$ = create_type_specifier($1);
    }
    | class_type_specifier
    {
        debug_bison_info_with_green("[RULE::type_specifier:class_type_specifier]");
    }
    | basic_type_specifier dimension_expression
    {
        debug_bison_info_with_green("[RULE::type_specifier:array_type_specifier]");
        $$ = create_type_specifier_array(create_type_specifier($1), $2);
    }
    | class_type_specifier dimension_expression
    {
        debug_bison_info_with_green("[RULE::type_specifier:array-class]");
        $$ = create_type_specifier_array($1, $2);
    }
    ;

basic_type_specifier
    : TOKEN_BOOL
    {
        debug_bison_info_with_green("[RULE::basic_type_specifier]\t variable_type(TOKEN_BOOL) ");
        $$ = RING_BASIC_TYPE_BOOL;
    }
    | TOKEN_INT
    {
        debug_bison_info_with_green("[RULE::basic_type_specifier]\t variable_type(TOKEN_INT) ");
        $$ = RING_BASIC_TYPE_INT;
    }
    | TOKEN_INT64
    {
        debug_bison_info_with_green("[RULE::basic_type_specifier]\t variable_type(TOKEN_INT64) ");
        $$ = RING_BASIC_TYPE_INT64;
    }
    | TOKEN_DOUBLE
    {
        debug_bison_info_with_green("[RULE::basic_type_specifier]\t variable_type(TOKEN_DOUBLE) ");
        $$ = RING_BASIC_TYPE_DOUBLE;
    }
    | TOKEN_STRING
    {
        debug_bison_info_with_green("[RULE::basic_type_specifier]\t variable_type(TOKEN_STRING) ");
        $$ = RING_BASIC_TYPE_STRING;
    }
    | TOKEN_ANY
    {
        debug_bison_info_with_green("[RULE::basic_type_specifier]\t variable_type(TOKEN_ANY) ");
        $$ = RING_BASIC_TYPE_ANY;
    }
    ;

// TODO: 这里的名称需要修改一下，应该叫 type_specifier_alias
class_type_specifier
    : IDENTIFIER
    {
        debug_bison_info_with_green("[RULE::class_type_specifier]\t variable_type(TOKEN_ANY) ");
        $$ = create_type_specifier_alias(nullptr, $1);
    }
    | IDENTIFIER TOKEN_2COLON IDENTIFIER
    {
        // TODO: 目前只能一级查找，后续支持多级
        $$ = create_type_specifier_alias($1, $3);
    }
    ;

class_field_init_element_list
    : class_field_init_element
    | class_field_init_element_list TOKEN_COMMA class_field_init_element
    {
        debug_bison_info_with_green("[RULE::class_field_init_element_list]\t ");
        $$ = field_init_list_add_item($1, $3);
    }
    ;

class_field_init_element
    : IDENTIFIER TOKEN_COLON expression
    {
        debug_bison_info_with_green("[RULE::class_field_init_element]\t ");
        $$ = create_field_init_expression($1, $3);
    }
    ;

expression_list_maybe_empty
    : 
    {
        $$ = nullptr;
    }
    | expression_list
    {
    }
    ;

expression_list
    : expression
    | expression_list TOKEN_COMMA expression
    {
        debug_bison_info_with_green("[RULE::expression_list]\t ");
        $$ = expression_list_add_item($1, $3);
    }
    ;
expression
    : right_value_expression
    ;

ternary_expression
    : logical_expression_or TOKEN_QUESTION_MARK expression_arithmetic_operation_additive TOKEN_COLON expression_arithmetic_operation_additive
    {
        debug_bison_info_with_green("[RULE::expression:condition ternary expression]\t ");
        $$ = create_expression_ternary($1, $3, $5);
    }
    ;

launch_expression
    : TOKEN_LAUNCH function_call_expression
    {
        debug_bison_info_with_green("[RULE::launch_expression:function_call_expression]\t ");
        $$ = create_expression_launch(LAUNCH_EXPRESSION_TYPE_FUNCTION_CALL, $2, nullptr, nullptr);
    }
    | TOKEN_LAUNCH member_call_expression
    {
        debug_bison_info_with_green("[RULE::launch_expression:member_call_expression]\t ");
        $$ = create_expression_launch(LAUNCH_EXPRESSION_TYPE_MEMBER_CALL, nullptr, $2, nullptr);
    }
    | TOKEN_LAUNCH iife_expression
    {
        debug_bison_info_with_green("[RULE::launch_expression:iife_expression]\t ");
        $$ = create_expression_launch(LAUNCH_EXPRESSION_TYPE_IIFE, nullptr, nullptr, $2);
    }
    ;

// left_value_expression 是 right_value_expression的子集
left_value_expression_list
    : left_value_expression
    {
        debug_bison_info_with_green("[RULE::left_value_expression_list]\t ");
    }
    | left_value_expression_list TOKEN_COMMA left_value_expression
    {
        debug_bison_info_with_green("[RULE::left_value_expression_list]\t ");
        $$ = expression_list_add_item($1, $3);
    }
    ;
left_value_expression
    : primary_not_new_array
    {
        debug_bison_info_with_green("[RULE::left_value_expression:primary_not_new_array]\t ");
    }
    | member_expression
    {
        debug_bison_info_with_green("[RULE::left_value_expression:member_expression]\t ");
    }
    ;


right_value_expression_list
    : right_value_expression
    {
        debug_bison_info_with_green("[RULE::right_value_expression_list]\t ");
    }
    | right_value_expression_list TOKEN_COMMA right_value_expression
    {
        debug_bison_info_with_green("[RULE::right_value_expression_list]\t ");
        $$ = expression_list_add_item($1, $3);
    }
    ;
right_value_expression
    : derive_value_literal_expression
    {
        debug_bison_info_with_green("[RULE::right_value_expression:derive_value_literal_expression]\t ");
    }
    | logical_expression_or
    {
        debug_bison_info_with_green("[RULE::right_value_expression:logical_expression_or]\t ");
    }
    | ternary_expression
    {
        debug_bison_info_with_green("[RULE::right_value_expression:ternary_expression]\t ");
    }
    | launch_expression
    {
        debug_bison_info_with_green("[RULE::right_value_expression:launch_expression]\t ");
    }
    | closure_definition
    {
        debug_bison_info_with_green("[RULE::right_value_expression:closure_definition]\t ");
    }
    ;


logical_expression_or
    : logical_expression_and
    {
        debug_bison_info_with_green("[RULE::logical_expression_or:1]\t ");
    }
    | logical_expression_or TOKEN_OR logical_expression_and
    {
        debug_bison_info_with_green("[RULE::logical_expression_or:2]\t ");
        $$ = create_expression_binary(EXPRESSION_TYPE_LOGICAL_OR, $1, $3);
    }
    ;

logical_expression_and
    : equality_expression
    {
        debug_bison_info_with_green("[RULE::logical_expression_and]\t ");
    }
    | logical_expression_and TOKEN_AND equality_expression
    {
        debug_bison_info_with_green("[RULE::logical_expression_and]\t ");
        $$ = create_expression_binary(EXPRESSION_TYPE_LOGICAL_AND, $1, $3);
    }
    ;

equality_expression
        : relational_expression
        | equality_expression TOKEN_EQ relational_expression
        {
            debug_bison_info_with_green("[RULE::equality_expression]\t ");
            $$ = create_expression_binary(EXPRESSION_TYPE_RELATIONAL_EQ, $1, $3);
        }
        | equality_expression TOKEN_NE relational_expression
        {
            debug_bison_info_with_green("[RULE::equality_expression]\t ");
            $$ = create_expression_binary(EXPRESSION_TYPE_RELATIONAL_NE, $1, $3);
        }
        ;

relational_expression
    : bitwise_expression_or
    {
        debug_bison_info_with_green("[RULE::relational_expression]\t ");
    }
    | relational_expression TOKEN_GT bitwise_expression_or
    {
        debug_bison_info_with_green("[RULE::relational_expression]\t ");
        $$ = create_expression_binary(EXPRESSION_TYPE_RELATIONAL_GT, $1, $3);
    }
    | relational_expression TOKEN_GE bitwise_expression_or
    {
        debug_bison_info_with_green("[RULE::relational_expression]\t ");
        $$ = create_expression_binary(EXPRESSION_TYPE_RELATIONAL_GE, $1, $3);
    }
    | relational_expression TOKEN_LT bitwise_expression_or
    {
        debug_bison_info_with_green("[RULE::relational_expression]\t ");
        $$ = create_expression_binary(EXPRESSION_TYPE_RELATIONAL_LT, $1, $3);
    }
    | relational_expression TOKEN_LE bitwise_expression_or
    {
        debug_bison_info_with_green("[RULE::relational_expression]\t ");
        $$ = create_expression_binary(EXPRESSION_TYPE_RELATIONAL_LE, $1, $3);
    }
    ;

bitwise_expression_or
    : bitwise_expression_xor
    {
    }
    | bitwise_expression_or TOKEN_BIT_XOR bitwise_expression_xor
    {
        debug_bison_info_with_green("[RULE::bitwise_expression_or]\t ");
        $$ = create_expression_binary(EXPRESSION_TYPE_BITWISE_XOR, $1, $3);
    }
    ;

bitwise_expression_xor
    : bitwise_expression_and
    {
    }
    | bitwise_expression_xor TOKEN_BIT_OR bitwise_expression_and
    {
        debug_bison_info_with_green("[RULE::bitwise_expression_xor]\t ");
        $$ = create_expression_binary(EXPRESSION_TYPE_BITWISE_XOR, $1, $3);
    }
    ;

bitwise_expression_and
    : bitwise_expression_shift
    {
    }
    | bitwise_expression_and TOKEN_BIT_AND bitwise_expression_shift
    {
        debug_bison_info_with_green("[RULE::bitwise_expression_and]\t ");
        $$ = create_expression_binary(EXPRESSION_TYPE_BITWISE_AND, $1, $3);
    }
    ;

bitwise_expression_shift
    : expression_arithmetic_operation_additive
    {
    }
    | bitwise_expression_shift TOKEN_BIT_LSHIFT expression_arithmetic_operation_additive
    {
        debug_bison_info_with_green("[RULE::bitwise_expression_shift]\t ");
        $$ = create_expression_binary(EXPRESSION_TYPE_BITWISE_LSH, $1, $3);
    }
    | bitwise_expression_shift TOKEN_BIT_RSHIFT expression_arithmetic_operation_additive
    {
        debug_bison_info_with_green("[RULE::bitwise_expression_shift]\t ");
        $$ = create_expression_binary(EXPRESSION_TYPE_BITWISE_RSH, $1, $3);
    }
    ;

expression_arithmetic_operation_additive
    : expression_arithmetic_operation_multiplicative 
    {
        debug_bison_info_with_green("[RULE::expression_arithmetic_operation_additive]\t ");
    }
    | expression_arithmetic_operation_additive TOKEN_ADD expression_arithmetic_operation_multiplicative
    {
        debug_bison_info_with_green("[RULE::expression_arithmetic_operation_additive]\t ");
        $$ = create_expression_binary(EXPRESSION_TYPE_ARITHMETIC_ADD, $1, $3);
    }
    | expression_arithmetic_operation_additive TOKEN_SUB expression_arithmetic_operation_multiplicative
    {
        debug_bison_info_with_green("[RULE::expression_arithmetic_operation_additive]\t ");
        $$ = create_expression_binary(EXPRESSION_TYPE_ARITHMETIC_SUB, $1, $3);
    }
    ;

expression_arithmetic_operation_multiplicative
    : unitary_expression
    {
        debug_bison_info_with_green("[RULE::expression_arithmetic_operation_multiplicative]\t ");
    }
    | expression_arithmetic_operation_multiplicative TOKEN_MUL unitary_expression
    {
        debug_bison_info_with_green("[RULE::expression_arithmetic_operation_multiplicative]\t ");
        $$ = create_expression_binary(EXPRESSION_TYPE_ARITHMETIC_MUL, $1, $3);
    }
    | expression_arithmetic_operation_multiplicative TOKEN_DIV unitary_expression
    {
        debug_bison_info_with_green("[RULE::expression_arithmetic_operation_multiplicative]\t ");
        $$ = create_expression_binary(EXPRESSION_TYPE_ARITHMETIC_DIV, $1, $3);
    }
    | expression_arithmetic_operation_multiplicative TOKEN_MOD unitary_expression
    {
        debug_bison_info_with_green("[RULE::expression_arithmetic_operation_multiplicative]\t ");
        $$ = create_expression_binary(EXPRESSION_TYPE_ARITHMETIC_MOD, $1, $3);
    }
    ;

unitary_expression
    : TOKEN_NOT unitary_expression
    {
        debug_bison_info_with_green("[RULE::unitary_expression:TOKEN_NOT]\t ");
        $$ = create_expression_unitary(EXPRESSION_TYPE_LOGICAL_UNITARY_NOT, $2);
    }
    | TOKEN_SUB unitary_expression
    {
        debug_bison_info_with_green("[RULE::unitary_expression:TOKEN_SUB]\t ");
        $$ = create_expression_unitary(EXPRESSION_TYPE_ARITHMETIC_UNITARY_MINUS, $2);
    }
    | TOKEN_BIT_NOT unitary_expression
    {
        debug_bison_info_with_green("[RULE::unitary_expression:TOKEN_BIT_NOT]\t ");
        $$ = create_expression_unitary(EXPRESSION_TYPE_BITWISE_UNITARY_NOT, $2);
    }
    | postfix_expression
    | slice_literal_expression
    ;

postfix_expression
    : IDENTIFIER self_incr_decr_token
    {
        $$ = create_expression_unitary($2, create_expression_identifier($1));
    }
    | IDENTIFIER dimension_expression self_incr_decr_token
    {
        $$ = create_expression_unitary($3, 
                                        create_expression_identifier_with_index(create_expression_identifier($1), $2));
    }
    | member_expression self_incr_decr_token
    {
        $$ = create_expression_unitary($2, $1);
    }
    | primary_expression
    ;

self_incr_decr_token
    : TOKEN_INCREASE
    {
        $$ = EXPRESSION_TYPE_UNITARY_INCREASE;
    }
    | TOKEN_DECREASE
    {
        $$ = EXPRESSION_TYPE_UNITARY_DECREASE;
    }
    ;

primary_expression
    : primary_not_new_array
    | member_expression
    | basic_value_literal_expression
    | primary_new_creation
    ;

primary_new_creation
    : TOKEN_NEW basic_type_specifier dimension_expression
    {
        $$ = create_new_array_expression(create_type_specifier($2), $3);
    }
    | TOKEN_NEW class_type_specifier dimension_expression
    {
        $$ = create_new_array_expression($2, $3);
    }
    ;

dimension_expression
    : TOKEN_LB sub_dimension_expression_list TOKEN_RB
    {
        debug_bison_info_with_green("[RULE::dimension_expression]");
        $$ = create_dimension_expression($2);
    }
    | TOKEN_LB TOKEN_EXCLAM_MARK INT_LITERAL TOKEN_RB
    {
        $$ = create_dimension_expression_with_exclam($3);
    }
    ;


sub_dimension_expression_list
    : sub_dimension_expression
    | sub_dimension_expression_list TOKEN_COMMA sub_dimension_expression
    {
        debug_bison_info_with_green("[RULE::sub_dimension_expression_list]");
        $$ = sub_dimension_expression_list_add_item($1, $3);
    }
    ;

sub_dimension_expression
    : expression_arithmetic_operation_additive
    {
        debug_bison_info_with_green("[RULE::sub_dimension_expression:1]");
        $$ = create_sub_dimension_expression($1);
    }
    | // empty
    {
        debug_bison_info_with_green("[RULE::sub_dimension_expression:2]");
        $$ = create_sub_dimension_expression(nullptr);
    }

slice_literal_expression
    : slice_expression
    {
        debug_bison_info_with_green("[RULE::slice_literal_expression:slice_expression]\t ");
        $$ = create_expression_from_slice_expression($1);
    }
    ;
slice_expression
    : IDENTIFIER TOKEN_LB sub_slice_expression TOKEN_RB
    {
        $$ = create_slice_expression(create_expression_identifier($1), $3);
    }
    ;

sub_slice_expression
    : expression_arithmetic_operation_additive TOKEN_COLON expression_arithmetic_operation_additive
    {
        debug_bison_info_with_green("[RULE::slice_expression:1] low:high");
        $$ = create_sub_slice_expression($1, $3);
    }
    | TOKEN_COLON expression_arithmetic_operation_additive
    {
        debug_bison_info_with_green("[RULE::slice_expression:3] :high");
        $$ = create_sub_slice_expression(nullptr, $2);
    }
    | expression_arithmetic_operation_additive TOKEN_COLON
    {
        debug_bison_info_with_green("[RULE::slice_expression:4] low:");
        $$ = create_sub_slice_expression($1, nullptr);
    }
    | TOKEN_COLON
    {
        debug_bison_info_with_green("[RULE::slice_expression:6] :");
        $$ = create_sub_slice_expression(nullptr, nullptr);
    }
    ;

primary_not_new_array
    : IDENTIFIER
    {
        debug_bison_info_with_green("[RULE::literal_term:identifier]\t ");
        $$ = create_expression_identifier($1);
    }
    | IDENTIFIER dimension_expression
    {
        $$ = create_expression_identifier_with_index(create_expression_identifier($1), $2);
    }
    | function_call_expression 
    {
        debug_bison_info_with_green("[RULE::literal_term:function_call_expression]\t ");
        $$ = create_expression_from_function_call($1);
    }
    | IDENTIFIER TOKEN_2COLON primary_not_new_array
    {
        $$ = expression_add_package_posit($3, $1);
    }
    | iife_expression
    {
        $$ = create_expression_from_iife($1);
    }
    ;

primary_not_call_expression
    : IDENTIFIER
    {
        debug_bison_info_with_green("[RULE::literal_term:identifier]\t ");
        $$ = create_expression_identifier($1);
    }
    | IDENTIFIER dimension_expression
    {
        $$ = create_expression_identifier_with_index(create_expression_identifier($1), $2);
    }
    | function_call_expression
    {
        $$ = create_expression_from_function_call($1);
    }
    | slice_literal_expression 
    {
        debug_bison_info_with_green("[RULE::primary_new_creation:slice_literal_expression]\t ");
    }
    ;

member_expression
    : primary_not_new_array TOKEN_DOT IDENTIFIER
    {
        $$ = create_member_expression($1, $3);
    }
    | member_expression TOKEN_DOT IDENTIFIER
    {
        $$ = create_member_expression($1, $3);
    }
    | member_call_expression
    {
        $$ = create_expression_from_member_call($1);
    }
    ;

member_call_expression
    : primary_not_new_array TOKEN_DOT IDENTIFIER TOKEN_LP               TOKEN_RP
    {
        $$ = create_member_call_expression($1, $3, nullptr);
    }
    | member_expression     TOKEN_DOT IDENTIFIER TOKEN_LP               TOKEN_RP
    {
        $$ = create_member_call_expression($1, $3, nullptr);
    }
    | primary_not_new_array TOKEN_DOT IDENTIFIER TOKEN_LP argument_list TOKEN_RP
    {
        $$ = create_member_call_expression($1, $3, $5);
    }
    | member_expression     TOKEN_DOT IDENTIFIER TOKEN_LP argument_list TOKEN_RP
    {
        $$ = create_member_call_expression($1, $3, $5);
    }
    ; 

basic_value_literal_expression
    : INT_LITERAL
    {
        debug_bison_info_with_green("[RULE::literal_term:INT_LITERAL]\t ");
        $$ = create_expression_literal(EXPRESSION_TYPE_LITERAL_INT, $1);
    }
    | INT64_LITERAL
    {
        debug_bison_info_with_green("[RULE::literal_term:INT64_LITERAL]\t ");
        $$ = create_expression_literal(EXPRESSION_TYPE_LITERAL_INT64, $1);
    }
    | DOUBLE_LITERAL
    {
        debug_bison_info_with_green("[RULE::literal_term:DOUBLE_LITERAL]\t ");
        $$ = create_expression_literal(EXPRESSION_TYPE_LITERAL_DOUBLE, $1);
    }
    | STRING_LITERAL
    {
        debug_bison_info_with_green("[RULE::literal_term:STRING_LITERAL]\t ");
        $$ = create_expression_literal(EXPRESSION_TYPE_LITERAL_STRING, $1);
    }
    | TOKEN_TRUE
    {
        debug_bison_info_with_green("[RULE::literal_term:TOKEN_TRUE]\t ");
        $$ = create_expression_bool_literal(EXPRESSION_TYPE_LITERAL_BOOL, true);
    }
    | TOKEN_FALSE
    {
        debug_bison_info_with_green("[RULE::literal_term:TOKEN_FALSE]\t ");
        $$ = create_expression_bool_literal(EXPRESSION_TYPE_LITERAL_BOOL, false);
    }
    ;

derive_value_literal_expression
    : array_literal_expression
    {
        debug_bison_info_with_green("[RULE::derive_value_literal_expression:array_literal_expression]\t ");
        $$ = create_expression_from_array_literal($1);
    }
    | class_object_literal_expression
    {
        debug_bison_info_with_green("[RULE::derive_value_literal_expression:class_object_literal_expression]\t ");
        $$ = create_expression_from_class_object_literal($1);
    }
    ;

function_call_expression
    : primary_not_call_expression TOKEN_LP argument_list TOKEN_RP
    {
        debug_bison_info_with_green("[RULE::function_call_expression]\t ");
        $$ = create_function_call_expression($1, $3);
    }
    | primary_not_call_expression TOKEN_LP               TOKEN_RP
    {
        debug_bison_info_with_green("[RULE::function_call_expression]\t ");
        $$ = create_function_call_expression($1, nullptr);
    }
    ;

iife_expression
    : TOKEN_FN function_tuple TOKEN_LP argument_list TOKEN_RP
    {
        $$ = create_expression_iife((AnonymousFunc*)($2), $4);
    }
    | TOKEN_FN function_tuple TOKEN_LP               TOKEN_RP
    {
        $$ = create_expression_iife((AnonymousFunc*)($2), nullptr);
    }
    ;


/*
 * 数组常量规则
 *
 * 1. {} 内 可以为空
 * 2. {} 内 可以有 一个或多个元素，最后一个元素后边，逗号为可选
 */ 
array_literal_expression
    : basic_type_specifier dimension_expression TOKEN_LC    expression_list_maybe_empty    TOKEN_RC
    {
        debug_bison_info_with_green("[RULE::array_literal_expression:basic]\t ");
        $$ = create_array_literal_expression(create_type_specifier($1), $2, $4);
    }
    | basic_type_specifier dimension_expression TOKEN_LC    expression_list TOKEN_COMMA    TOKEN_RC
    {
        debug_bison_info_with_green("[RULE::array_literal_expression:basic]\t ");
        $$ = create_array_literal_expression(create_type_specifier($1), $2, $4);
    }
    | class_type_specifier dimension_expression TOKEN_LC    expression_list_maybe_empty    TOKEN_RC
    {
        debug_bison_info_with_green("[RULE::array_literal_expression:class]\t ");
        $$ = create_array_literal_expression($1, $2, $4);
    }
    | class_type_specifier dimension_expression TOKEN_LC    expression_list TOKEN_COMMA    TOKEN_RC
    {
        debug_bison_info_with_green("[RULE::array_literal_expression:class]\t ");
        $$ = create_array_literal_expression($1, $2, $4);
    }
    ;

class_object_literal_expression
    : class_type_specifier TOKEN_LC class_field_init_element_list             TOKEN_RC
    {
        debug_bison_info_with_green("[RULE::class_object_literal_expression]\t ");
        $$ = create_class_object_literal_expression($1, $3);
    }
    | class_type_specifier TOKEN_LC                                           TOKEN_RC
    {
        debug_bison_info_with_green("[RULE::class_object_literal_expression]\t ");
        $$ = create_class_object_literal_expression($1, nullptr);
    }
    | class_type_specifier TOKEN_LC class_field_init_element_list TOKEN_COMMA TOKEN_RC
    {
        debug_bison_info_with_green("[RULE::class_object_literal_expression]\t ");
        $$ = create_class_object_literal_expression($1, $3);
    }
    ;

assign_expression
    : left_value_expression_list TOKEN_ASSIGN right_value_expression_list
    {
        debug_bison_info_with_green("[RULE::assign_expression]\t ");
        $$ = create_assign_expression(ASSIGN_EXPRESSION_TYPE_ASSIGN, $1, $3);
    }
    | IDENTIFIER TOKEN_ADD_ASSIGN right_value_expression
    {
        debug_bison_info_with_green("[RULE::assign_expression]\t ");
        $$ = create_assign_expression(ASSIGN_EXPRESSION_TYPE_ADD_ASSIGN, create_expression_identifier($1), $3);
    }
    | IDENTIFIER TOKEN_SUB_ASSIGN right_value_expression
    {
        debug_bison_info_with_green("[RULE::assign_expression]\t ");
        $$ = create_assign_expression(ASSIGN_EXPRESSION_TYPE_SUB_ASSIGN, create_expression_identifier($1), $3);
    }
    | IDENTIFIER TOKEN_MUL_ASSIGN right_value_expression
    {
        debug_bison_info_with_green("[RULE::assign_expression]\t ");
        $$ = create_assign_expression(ASSIGN_EXPRESSION_TYPE_MUL_ASSIGN, create_expression_identifier($1), $3);
    }
    | IDENTIFIER TOKEN_DIV_ASSIGN right_value_expression
    {
        debug_bison_info_with_green("[RULE::assign_expression]\t ");
        $$ = create_assign_expression(ASSIGN_EXPRESSION_TYPE_DIV_ASSIGN, create_expression_identifier($1), $3);
    }
    | IDENTIFIER TOKEN_MOD_ASSIGN right_value_expression
    {
        debug_bison_info_with_green("[RULE::assign_expression]\t ");
        $$ = create_assign_expression(ASSIGN_EXPRESSION_TYPE_MOD_ASSIGN, create_expression_identifier($1), $3);
    }
    ;

identifier_list
    : IDENTIFIER
    {
        debug_bison_info_with_green("[RULE::identifier_list]\t ");
        $$ = create_identifier(IDENTIFIER_TYPE_VARIABLE, $1);
    }
    | identifier_list TOKEN_COMMA IDENTIFIER
    {
        debug_bison_info_with_green("[RULE::identifier_list]\t ");
        $$ = identifier_list_add_item($1, create_identifier(IDENTIFIER_TYPE_VARIABLE, $3));
    }
    ;

identifier
    : IDENTIFIER
    {
        debug_bison_info_with_green("[RULE::identifier]\t identifier(%s)", $1);
        $$ = create_identifier(IDENTIFIER_TYPE_UNKNOW, $1);
    }
    ;


argument_list
    : argument 
    {
    }
    | argument_list TOKEN_COMMA argument
    {
        $$ = argument_list_add_item($1, $3);
    }
    ;
argument
    : right_value_expression
    {
        debug_bison_info_with_green("[RULE::argument_list:expression]\t ");
        $$ = create_argument_list_from_expression($1);
    }
    ;

tag_definition_statement
    : TOKEN_NUM_SIGN IDENTIFIER
    {
        debug_bison_info_with_green("[RULE::tag_definition_statement]\t ");
        $$ = create_tag_definition_statement($2);
    }
    ;

jump_tag_statement
    : TOKEN_JUMP TOKEN_NUM_SIGN IDENTIFIER
    {
        debug_bison_info_with_green("[RULE::jump_tag_statement]\t ");
        $$ = create_jump_tag_statement($3);
    }
    ;


range_expression
    : sub_step_range_expression
    {
        $$ = create_range_expression_from_step_range($1);
    }
    | sub_linear_range_expression
    {
        $$ = create_range_expression_from_linear_range($1);
    }
    ;

sub_step_range_expression
    : expression_arithmetic_operation_additive TOKEN_2DOT expression_arithmetic_operation_additive
    {
        $$ = create_step_range_expression($1, $3, nullptr, false);
    }
    | expression_arithmetic_operation_additive TOKEN_2DOT expression_arithmetic_operation_additive TOKEN_COLON expression_arithmetic_operation_additive
    {
        $$ = create_step_range_expression($1, $3, $5, false);
    }
    | expression_arithmetic_operation_additive TOKEN_2DOT TOKEN_ASSIGN expression_arithmetic_operation_additive
    {
        $$ = create_step_range_expression($1, $4, nullptr, true);
    }
    | expression_arithmetic_operation_additive TOKEN_2DOT TOKEN_ASSIGN expression_arithmetic_operation_additive TOKEN_COLON expression_arithmetic_operation_additive
    {
        $$ = create_step_range_expression($1, $4, $6, true);
    }
    ;

sub_linear_range_expression
    : left_value_expression
    {
        $$ = create_linear_range_expression($1);
    }
    | derive_value_literal_expression
    {
        $$ = create_linear_range_expression($1);
    }
    ;

%%
