//---------------------------------------------------------------------------
#ifndef SomeFunctionsH
#define SomeFunctionsH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <class_ParameterTMI_old.h>
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
bool SaveToFile(TParameterTMI_Old <TypeOfX, TypeOfY> &Parameter,AnsiString OutputFileName);  //—охранение параметра в файл

template <class type>
type* WriteToArr(type *array, int FileHandle, int ParAmount, int FileLength);            //—читать файл в массив заданного типа

template <class type>
type RoundTo(type Value, int SymbolsAfterComma);//очень плохо

AnsiString GetFileNameWithoutExt(AnsiString FileName); //

AnsiString ChangeDecimalSeparator(AnsiString Number); // changes "," to "." or vise-versa

bool TryToInt(AnsiString &Number,  int &Variable);
bool TryToFloat(AnsiString &Number,  float &Variable);
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
bool SaveToFile(TParameterTMI_Old <TypeOfX, TypeOfY> &Parameter,AnsiString FileName)
{
   if(Parameter.CountPoint>0)
    {
        TFileStream* File1=new TFileStream(FileName,fmCreate);
        for(unsigned int i=0;i<Parameter.CountPoint;i++)
        {
            File1->Write(&Parameter.Points[i].X,sizeof(TypeOfX));
            File1->Write(&Parameter.Points[i].Y,sizeof(TypeOfY));
        }
        delete File1;
        return true;
    }
   else
    return false;
}

//---------------------------------------------------------------------------
template <class type>
type* WriteToArr(type *arr, int fh, int pa, int fl)
{
    int e=sizeof(type)*pa; //sizeof(type)
    if (fl%e!=0)
        return 0;

    arr=new type[fl];

    for (int i=0;i<=fl;i++)
    {
        read(fh,&arr[i],sizeof(type));
    }
    return arr;
}
//---------------------------------------------------------------------------
template <class type>
type RoundTo(type val, int sac)
{
    int a=1;
    for(int i=0;i<sac;i++)
        a=a*10;

    type b=val;
    b=(int)(val*a)/(a*1.0);

    return b;
}
//---------------------------------------------------------------------------
AnsiString GetFileNameWithoutExt(AnsiString FileName)
{
    for(unsigned int i = FileName.Length(); i >= 1; i--)
        if(FileName[i] == '.')
            return FileName.SubString(0, i - 1);
    return FileName;
}
//---------------------------------------------------------------------------
AnsiString ChangeDecimalSeparator(AnsiString Text)
{
    unsigned int Separators = 0;
    for(int i = 1; i < Text.Length(); i++)
    {
        if(Text[i] == ',' || Text[i] == '.')
        {
            Text[i] = DecimalSeparator;
            Separators++;
            if(Separators > 1)
                return Text.SubString(0, i - 1);
        }
    }
    return Text;
}
//---------------------------------------------------------------------------
bool TryToFloat(AnsiString &Number, double &Variable)
{
    if(!TryStrToFloat(Number, Variable))
    {
        Number = ChangeDecimalSeparator(Number);
        if(!TryStrToFloat(Number, Variable))
            return false;
        else
            return true;
    }
    return true;
}
//---------------------------------------------------------------------------
bool TryToInt(AnsiString &Number,  int &Variable)
{
    if(!TryStrToInt(Number, Variable))
    {
        Number = ChangeDecimalSeparator(Number);
        if(!TryStrToInt(Number, Variable))
            return false;
        else
            return true;
    }
    return true;
}
//---------------------------------------------------------------------------
#endif