#pragma once

#ifdef EASEP_EXPORTS
#	define EASEP_EXPORT __declspec(dllexport)
#endif // EASEP_EXPORTS

#ifndef SIZEOF_ARRAY
#	define SIZEOF_ARRAY(x) (sizeof(x) / sizeof(x[0]))
#endif // SIZEOF_ARRAY

#ifndef SAFE_FREE
#	define SAFE_FREE(x) if (x) { free(x); (x) = NULL; }
#endif // SAFE_FREE
