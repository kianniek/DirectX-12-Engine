#pragma once

#define KBs(x) (x) * 1024

#define KOSMO_EVAL_HR(hr, error) if ((hr) != S_OK) { std::cout << "API Error: " << error << " At file: " << __FILE__ << std::endl;}
#define PRINT_W_N(message) std::wcout << message << std::endl;
#define PRINT_N(message) std::cout << message << std::endl;

#ifdef _DEBUG

#define KOSMO_ASSERT(exp) if(!(exp)) { std::cout << "Critical error in file " << __FILE__ << std::endl; throw; }

#else 

#define KOSMO_ASSERT(exp)

#endif // _DEBUG
