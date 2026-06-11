#ifdef STANDARD_C89
/*****************************************************************************************************************
   - NO c++ style comments
*****************************************************************************************************************/

#endif

#ifdef STANDARD_C99
/*****************************************************************************************************************
   - NO K&R style
*****************************************************************************************************************/

/*
   define if the compiler doesnt support oldstyle (K&R syntax)
   function declarations.
*/
#define NO_OLD_FUNC_DECL


#define NO_TYPELESS_INT
#define NO_TYPELESS_INT_PTR

/*
   define to NOT use ansi-c prototypes for k&r style functions
*/
/*#define NO_NEW_PROTOTYPES_FOR_OLD_FUNC_DECL*/

#define MAIN_RETURNS_INT

#endif



/*

*/

/*
   define if the compiler gives an error instead of just a warning
   when a function is declared implicitly (used before a prototype
   or the function itself appears)
*/
#define NO_IMPLICIT_FUNC_PROTOTYPES

/*

   define if the compiler doesnt support floats (and doubles)

   floats are substituted by "signed" in tests that dont
   explicitly test floats themselves (these are left out).

*/
#define NO_FLOATS

#define NO_WCHAR

/*

   define if the compiler doesnt support bitfields

*/
/*#define NO_BITFIELDS*/

/*
   define if the compiler doesnt support empty arguments in
   function calls
*/
#define NO_EMPTY_FUNC_ARGS


/*#define NO_STRINGS_IN_FOR*/
/*#define NO_LOCAL_STRING_INIT*/

/*#define NO_LOCAL_STRUCT_INIT*/
#define NO_SLOPPY_STRUCT_INIT

/*
   define if the compiler doesnt support passing structs
   as arguments to function calls
*/
#define NO_FUNCS_TAKE_STRUCTS
/*
   define if the compiler doesnt support returning structs
   from function calls
*/
#define NO_FUNCS_RETURN_STRUCTS

/*
  define to cast struct pointers when passing them to a
  function that takes a struct pointer of a different type
*/
#define CAST_STRUCT_PTR

/*#define NO_SLOPPY_EXTERN*/
/*#define NO_LOCAL_PROTOTYPES*/

/*#define NO_BACKSLASH_B*/
/*#define NO_BACKSLASH_V*/
/*#define NO_BACKSLASH_CHARCODE*/

/*#define ASSUME_32BIT_INT*/
/*#define ASSUME_32BIT_UNSIGNED*/

#define NO_TYPELESS_STRUCT_PTR

/*
  bit types are not ANSI - so provide a way of disabling bit types
  if this file is used to test other compilers besides SDCC
 */
/*#define SUPPORT_BIT_TYPES*/

/*
  Some compilers that support bit types do not support bit arithmetic
  (like bitx = bity + bitz;)
*/
/*#define SUPPORT_BIT_ARITHMETIC*/


/*
   define to cast some pointers where some compilers complain
*/
/*#define FORCE_POINTERS*/

#define NO_IMPLICIT_FUNCPTR_CONV

/*
  target properties
*/
#define SIZEOF_INT_16BIT
#define SIZEOF_LONG_32BIT
#define UNSIGNED_CHARS
#define UNSIGNED_BITFIELDS
