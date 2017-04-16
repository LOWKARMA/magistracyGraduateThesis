//---------------------------------------------------------------------------
#ifndef DigitalQuantumH
#define DigitalQuantumH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <class_ParameterTMI_old.h>
//---------------------------------------------------------------------------
//����������� � ������ ������� ���
template <class TypeOfX, class TypeOfY>
bool DigitalQuantum(TParameterTMI_Old <TypeOfX, TypeOfY> &Parameter, char RoundingMethod,
                    TypeOfY QuantumLineBottomBorder, TypeOfY QuantumLineTopBorder, unsigned int QuantumIntervals);
/*
������� �������� �� ���� �������� ��� � �������������� ��� �������� � ����������� �� ������� �����������, ��� �����,
�������� ������ ���.
����������� ������� ���������: �������� ���, ������ ��������� ('U' - ���������� � �������� ������, 'D' - � �������, 'N' � ����������),
������ ������� ����� �����������, ������� ������� ����� �����������, ���������� ���������� �����������.
� ������ ��������� ��������� ������� ����������� true. ������� ���������� false ����:
���������� ���������� �� ����� ������ ���������� ������ �������;
������� ����� ����������� �������;
���������� ������� ����������� ������ ������.
*/
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
bool DigitalQuantum(TParameterTMI_Old <TypeOfX, TypeOfY> &Parameter, char RoundingMethod,
                    TypeOfY QuantumLineBottomBorder, TypeOfY QuantumLineTopBorder, unsigned int QuantumIntervals)
{

    if(RoundingMethod != 'd' && RoundingMethod != 'u' && RoundingMethod != 'n')
        return false;

    if(QuantumIntervals < 1)
        return false;

    if(QuantumLineBottomBorder >= QuantumLineTopBorder)
        return false;

    TParameterTMI_Old <TypeOfX, TypeOfY> QuantumLine;
    unsigned int QLPoints = QuantumIntervals;

    TypeOfY QLTop = QuantumLineTopBorder;
    TypeOfY QLBot = QuantumLineBottomBorder;
    TypeOfY QLStep = (QLTop - QLBot) / QLPoints;
    QuantumLine.CountPoint = QLPoints + 1;
    QuantumLine.Points[0].Y = QLBot;
    for(unsigned int i = 1; i < QuantumLine.CountPoint; i++)
        QuantumLine.Points[i].Y = QuantumLine.Points[i - 1].Y + QLStep;

    //AnsiString FName="D:\\0\\2\\514112709.1a1211";
    //SaveToFile(QuantumLine,FName+"_qw.d2");

    bool flag;
    switch(RoundingMethod)
    {
        case 'd':   for(unsigned int i = 0; i < Parameter.CountPoint; i++)
                    {
                        for (unsigned int j = 0; j < QuantumLine.CountPoint - 1; j++)
                        {
                            if(Parameter.Points[i].Y < QuantumLine.Points[j + 1].Y && Parameter.Points[i].Y > QuantumLine.Points[j].Y)
                                Parameter.Points[i].Y = QuantumLine.Points[j].Y;
                        }
                    }
                    break;

        case 'u':   for(unsigned int i = 0; i < Parameter.CountPoint; i++)
                    {
                        for (unsigned int j = 0; j < QuantumLine.CountPoint - 1; j++)
                        {
                            if(Parameter.Points[i].Y < QuantumLine.Points[j + 1].Y && Parameter.Points[i].Y > QuantumLine.Points[j].Y)
                                Parameter.Points[i].Y = QuantumLine.Points[j + 1].Y;
                        }
                    }
                    break;


        case 'n':   for(unsigned int i = 0; i < Parameter.CountPoint; i++)
                    {
                        for (unsigned int j = 0; j < QuantumLine.CountPoint - 1; j++)
                        {
                            if(Parameter.Points[i].Y < QuantumLine.Points[j + 1].Y && Parameter.Points[i].Y > QuantumLine.Points[j].Y)
                            {
                                if(Parameter.Points[0].Y - QuantumLine.Points[j].Y == QuantumLine.Points[j + 1].Y - Parameter.Points[0].Y)
                                {
                                    Parameter.Points[0].Y = QuantumLine.Points[j + 1].Y;
                                    flag = false;
                                }
                                if(Parameter.Points[i].Y - QuantumLine.Points[j].Y < QuantumLine.Points[j + 1].Y - Parameter.Points[i].Y)
                                {
                                    Parameter.Points[i].Y = QuantumLine.Points[j].Y;
                                    flag = true;
                                }
                                if(Parameter.Points[i].Y - QuantumLine.Points[j].Y > QuantumLine.Points[j + 1].Y - Parameter.Points[i].Y)
                                {
                                    Parameter.Points[i].Y = QuantumLine.Points[j + 1].Y;
                                    flag = false;
                                }
                                if(Parameter.Points[i].Y - QuantumLine.Points[j].Y == QuantumLine.Points[j + 1].Y - Parameter.Points[i].Y)
                                {
                                    if(!flag)
                                    {
                                        Parameter.Points[i].Y = QuantumLine.Points[j].Y;
                                        flag = true;
                                    }
                                    else
                                    {
                                        Parameter.Points[i].Y = QuantumLine.Points[j + 1].Y;
                                        flag = false;
                                    }
                                }
                            }
                        }
                    }
                    break;
    }

    for(unsigned int i = 0; i < Parameter.CountPoint; i++)
    {
        if(Parameter.Points[i].Y >= QLTop)
            Parameter.Points[i].Y = QLTop;
        if(Parameter.Points[i].Y <= QLBot)
            Parameter.Points[i].Y = QLBot;
    }
    return true;
}
//---------------------------------------------------------------------------
#endif