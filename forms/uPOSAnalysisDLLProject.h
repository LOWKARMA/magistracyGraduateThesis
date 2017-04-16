#ifndef _uPOSAnalysisDLLProject_H
#define _uPOSAnalysisDLLProject_H
#ifdef __DLL__
#define DLL_EI __declspec(dllexport)
#else
# define DLL_EI __declspec(dllimport)
#endif
extern "C" AnsiString DLL_EI Hello(AnsiString Name);
extern "C" int DLL_EI CreatePOSAnalysisForm(char* CurrentFileName);
#endif