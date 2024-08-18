#pragma once

#ifdef ENGINE_EXPORTS

#define KOSMO_API __declspec(dllexport)

#else

#define KOSMO_API __declspec(dllimport)


#endif // ENGINE_EXPORTS

