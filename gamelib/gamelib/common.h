#ifdef _MSC_VER

// Disable warning 4786:

// "identifier was truncated to '255' characters in the debug information"

#pragma warning( disable : 4786)

#endif



#ifndef __common_h__
#define __common_h__
#ifdef WIN32
#define veryLong __int64
#else
#define veryLong long long
#endif


#endif // __common_h__
