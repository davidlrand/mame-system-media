
/*
   define if the compiler doesnt support oldstyle (K&R syntax)
   function declarations.
*/
#define NO_OLD_FUNC_DECL
#define NO_NEW_PROTOTYPES_FOR_OLD_FUNC_DECL

/*

   define if the compiler doesnt support floats (and doubles)

   floats are substituted by "signed" in tests that dont
   explicitly test floats themselves (these are left out).

*/
#define NO_FLOATS

/*

   define if the compiler doesnt support bitfields

*/
#define NO_BITFIELDS

/*
   define if the compiler doesnt support empty arguments in
   function calls
*/
#define NO_EMPTY_FUNC_ARGS


//#define NO_STRINGS_IN_FOR
//#define NO_LOCAL_STRING_INIT

//#define NO_LOCAL_STRUCT_INIT
#define NO_SLOPPY_STRUCT_INIT

/*
   define if the compiler doesnt support passing structs
   as arguments function calls
*/
#define NO_FUNCS_TAKE_STRUCTS
/*
   define if the compiler doesnt support returning structs
   from function calls
*/
#define NO_FUNCS_RETURN_STRUCTS

//#define NO_SLOPPY_EXTERN
//#define NO_LOCAL_PROTOTYPES

//#define NO_BACKSLASH_B
//#define NO_BACKSLASH_V
//#define NO_BACKSLASH_CHARCODE

//#define ASSUME_32BIT_INT
//#define ASSUME_32BIT_UNSIGNED

#define NO_TYPELESS_INT
#define NO_TYPELESS_INT_PTR
#define NO_TYPELESS_STRUCT_PTR


/*
  bit types are not ANSI - so provide a way of disabling bit types
  if this file is used to test other compilers besides SDCC
 */
//#define SUPPORT_BIT_TYPES

/*
  Some compilers that support bit types do not support bit arithmetic
  (like bitx = bity + bitz;)
*/
//#define SUPPORT_BIT_ARITHMETIC


/*
   define to cast some pointers where some compilers complain
*/
//#define FORCE_POINTERS

#define NO_IMPLICIT_FUNCPTR_CONV

#define SIZEOF_INT_16BIT

