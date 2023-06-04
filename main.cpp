#include <iostream>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// 宏编程看看就行，一般不会使用，基本上都会换为模板

#define PP_REMOVE_PARENS(T)        PP_REMOVE_PARENS_IMPL T
#define PP_REMOVE_PARENS_IMPL(...) __VA_ARGS__

#define PP_COMMA()  ,
#define PP_LPAREN() (
#define PP_RPAREN() )
#define PP_EMPTY()


#define PP_CONCAT(A, B)      PP_CONCAT_IMPL(A, B)
#define PP_CONCAT_IMPL(A, B) A##B

#define PP_NOT(N) PP_CONCAT(PP_NOT_, N)
#define PP_NOT_0 1
#define PP_NOT_1 0

#define PP_AND(A, B) PP_CONCAT(PP_AND_, PP_CONCAT(A, B))
#define PP_AND_00 0
#define PP_AND_01 0
#define PP_AND_10 0
#define PP_AND_11 1

#define PP_BOOL(N) PP_CONCAT(PP_BOOL_, N)
#define PP_BOOL_0 0
#define PP_BOOL_1 1
#define PP_BOOL_2 1

#define PP_IF(PRED, THEN, ELSE) PP_CONCAT(PP_IF_, PP_BOOL(PRED))(THEN, ELSE)
#define PP_IF_1(THEN, ELSE)     THEN
#define PP_IF_0(THEN, ELSE)     ELSE

#define PP_COMMA_IF(N) PP_IF(N, PP_COMMA(), PP_EMPTY())

#define PP_GET_N(N, ...) PP_CONCAT(PP_GET_N_, N)(__VA_ARGS__)
#define PP_GET_N_0(_0, ...)                                 _0
#define PP_GET_N_1(_0, _1, ...)                             _1
#define PP_GET_N_2(_0, _1, _2, ...)                         _2
#define PP_GET_N_3(_0, _1, _2, _3, ...)                     _3
#define PP_GET_N_4(_0, _1, _2, _3, _4, ...)                 _4
#define PP_GET_N_5(_0, _1, _2, _3, _4, _5, ...)             _5
#define PP_GET_N_6(_0, _1, _2, _3, _4, _5, _6, ...)         _6
#define PP_GET_N_7(_0, _1, _2, _3, _4, _5, _6, _7, ...)     _7
#define PP_GET_N_8(_0, _1, _2, _3, _4, _5, _6, _7, _8, ...) _8

#define PP_IS_EMPTY(...)                                                                                          \
  PP_AND(PP_AND(PP_NOT(PP_HAS_COMMA(           __VA_ARGS__)), PP_NOT(      PP_HAS_COMMA(__VA_ARGS__()))),         \
         PP_AND(PP_NOT(PP_HAS_COMMA(PP_COMMA_V __VA_ARGS__)), PP_HAS_COMMA(PP_COMMA_V   __VA_ARGS__())))
#define PP_HAS_COMMA(...) PP_GET_N_8(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 0, 0)
#define PP_COMMA_V(...) ,

#define NEW(type, name, ...) type * name; do { type##_ctor(&name PP_IF(PP_IS_EMPTY(__VA_ARGS__), PP_EMPTY, PP_COMMA)() __VA_ARGS__); } while (0)
#define DELETE(type, name) type##_dtor(name)

typedef struct String
{
    char*  data;
    size_t capasity;
} String;

void String_ctor(String** str)
{
    assert( str != NULL);
    //assert(*str == NULL);  *str == NULL报错

    String* ptr   = (String*)malloc(sizeof(String));
    ptr->data     = (char  *)malloc(sizeof(char));
    ptr->data[0]  = 0;
    ptr->capasity = 1;

    *str = ptr;
}

void String_ctor(String** str, const char* cstr)
{
    // TODO: 实现String类型从const char*字符串构造的构造函数
    assert( str != NULL);
    //assert(*str == NULL);
    int len_cstr  = strlen(cstr);

    String* ptr   = (String*) malloc(sizeof(String));
    ptr->data     = (char*  ) malloc((len_cstr+1)*sizeof (*cstr));
    strcpy(ptr->data,cstr);
    ptr->capasity = len_cstr+1;

    *str = ptr;
}

void String_ctor(String** str, const String* rhs)
{
    // TODO: 实现String类型的拷贝构造函数
    assert( str != NULL);
    //assert(*str == NULL);

    String* ptr = (String*) malloc(sizeof(String));
    memcpy(ptr,rhs,sizeof(String));

    *str = ptr;
}

void String_dtor(String* str)
{
    // TODO: 实现String类型的析构函数
    free(str->data);
    free(str);
}

void print(const String* str)
{
    printf("%s\n", str->data);
}

typedef struct Base
{
    void* vfptr;
    int x;
} Base;

//Base的虚表
typedef struct Base_VFTable
{
    int xx;
    void (*Base_fun1)(const Base* obj);
} Base_VFTable;

void Base_init(Base* obj)
{
    assert(obj != NULL);

    obj->x = 1;
}

void Base_ctor(Base** obj)
{
    assert( obj != NULL);
    //assert(*obj == NULL);

    Base* ptr = (Base*)malloc(sizeof(Base));
    Base_init(ptr);

    *obj = ptr;
}

typedef struct Derive
{
    void* vfptr;
    //Base  base;  直接定义Base base则实例化Derive时，会有两个虚函数指针
    int x;
    float y;

} Derive;

//Derive的虚表
typedef struct Derive_VFTable
{
    void (*Derive_fun1)(const Derive* obj);
}Derive_VFTable;

void Derive_ctor(Derive** obj)
{
    assert( obj != NULL);
    //assert(*obj == NULL);

    Derive* ptr = (Derive*)malloc(sizeof(Derive));
    Base_init((Base*)ptr);
    ptr->y      = 0;

    *obj = ptr;
}

// TODO: 实现以下宏，使得调用它时等价于obj->x (Derive obj)。
#define PARENT_FIELD(obj, field) obj##->##field

void print(const Base* obj)
{
    assert(obj != NULL);
    printf("%d\n", obj->x);
}

void print(const Derive* obj)
{
    assert(obj != NULL);
    printf("%f\n", obj->y);
}

// TODO: 实现以下宏，使得根据obj是Base对象还是Derive对象，分别调用以上两个print函数。
// 提示：参考虚指针&虚表的思路，添加函数指针
#define VIRTUAL_PRINT(obj, func)

int main()
{
    printf("hello world\n");
    //实例化虚函数表
    Base_VFTable base_VFTable;
    Base_VFTable* bb = &base_VFTable;
    void* cc = &base_VFTable;

    base_VFTable.Base_fun1 = print;
    Derive_VFTable derive_VfTable;
    derive_VfTable.Derive_fun1 = print;

    {

        const char* k_str = "oop";
        //String* str1;
        //String_ctor(&str1);
        //String* str2;
        //String_ctor(&str2,k_str);
        //String* str3;
        //String_ctor(&str3,str2);
        NEW(String, str1);
        NEW(String, str2, k_str);
        NEW(String, str3, str2);

        print(str1);  // 打印 ""
        print(str2);  // 打印 "oop"
        print(str3);  // 打印 "oop"

        DELETE(String, str1);
        DELETE(String, str2);
        //DELETE(String, str3);  // 会不会报错？   会报错 DELETE str2之后str3也被释放了 为什么？？
    }

    {


        NEW(Derive, obj);           // 子类对象
        obj->vfptr = &derive_VfTable;

        Base* ptr = (Base*)obj;     // 基类指针指向子类对象
        ((Derive_VFTable*)(ptr->vfptr))->Derive_fun1((Derive*)ptr);
        //derive_VfTable.Derive_fun1((Derive*)ptr);
        //VIRTUAL_PRINT(ptr, print);  // 应该调用Derive对应的print

        obj->x = 2;
        int x = obj->x;
        //PARENT_FIELD(obj, x) = 2; //预编译文本替换成了obj->x  为什么还会报错？
        //int x = PARENT_FIELD(obj, x);  
        printf("%d\n", ptr->x);  // 输出2
    }
}
