//---------------------------------------------------------------------------
#ifndef ClassTStepMotorH
#define ClassTStepMotorH
//---------------------------------------------------------------------------
#include <class_ParameterTMI_old.h>
#include <class_FileTP.h>
#include <class_Errors_old.h>
#include <IniFiles.hpp>
//---------------------------------------------------------------------------
struct TIntervalCharacteristics
{
    double IntervalStartTime;
    double IntervalEndTime;
    double Noise;
    TIntervalCharacteristics();
};
//---------------------------------------------------------------------------
struct TControlImpulse
{
    float Level_12Min;
    float Level_12Max;
    float Level_23Min;
    float Level_23Max;
    float Level_34Min;
    float Level_34Max;
    float Level_41Min;
    float Level_41Max;
    float Level_0Min;
    float Level_0Max;
    bool LoadFormIniFile(AnsiString IniFileName);
};
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
class TStepMotor
{
    private:
        TErrors_old Errors;
        TControlImpulse ControlImpulseParam;            //пороговые уровни напряжения на ШМ
        TParameterTMI_Old <TypeOfX, TypeOfY> ControlImpulse; //сигнал управления обмотками ШМ
        TParameterTMI_Old <TypeOfX, TypeOfY> POS;            //модель потенциометра обратной связи

        double PointFrequency;                          //частота опроса
        double DeltaU;                                  //изменение напряжения ПОС за 1 шаг ШМ
        double StartU;                                  //начальный уровень напряжения
        double Luft;                                    //максимальный люфт
        double CurrentLuftPosition;                     //текущее положение люфта
        TIntervalCharacteristics LongestIntervalWithoutImpulses; //самый длительный интервал на котором не выдаются управлюящие импульсы
        double LuftCalc(double Increment);              //расчет люфта ШМ

        TIntervalCharacteristics* GetLongestIntervalTime(TParameterTMI_Old <TypeOfX, TypeOfY> *SourceParameter);
    public:
        TStepMotor();
        bool LoadParameter(TParameterTMI_Old <TypeOfX, TypeOfY> ParameterUI, double Delta_U,               //Параметры моделирования
                            double Start_U, TControlImpulse ControlLevels, double Point_Freq,
                            double Luft, double CurrentLuftPosition);
        TParameterTMI_Old <TypeOfX, TypeOfY>* GetPOS(TypeOfY TimeShift, bool FlagUsage,
                                                    TypeOfY QuantumLineBottomBorder, TypeOfY QuantumLineTopBorder,
                                                    unsigned int QuantumLineIntervals, char RoundingMethod,
                                                    TIntervalCharacteristics* LongestIntervalWithoutControlImpulses);       //Моделирование ПОС
        int GetError(AnsiString &Message);
        int GetWarning(AnsiString &Message);
        void Clear();
};
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
TStepMotor<TypeOfX, TypeOfY> ::TStepMotor()
{

    DeltaU = 0;
    StartU = 0;
    PointFrequency = 0;
    ControlImpulseParam.Level_12Min = 1;
    ControlImpulseParam.Level_12Max = 2;
    ControlImpulseParam.Level_23Min = 3;
    ControlImpulseParam.Level_23Max = 4;
    ControlImpulseParam.Level_34Min = 5;
    ControlImpulseParam.Level_34Max = 6;
    ControlImpulseParam.Level_41Min = 7;
    ControlImpulseParam.Level_41Max = 9;
    ControlImpulseParam.Level_0Min = -1;
    ControlImpulseParam.Level_0Max = 0;

    Luft = 0;
    CurrentLuftPosition = 0;

    Errors.SetErrorMessage(1, "Некорректный шаг.");
    Errors.SetErrorMessage(4, "Некорректная частота.");
    Errors.SetErrorMessage(2, "Запрашиваемый параметр указан неверно.");
    Errors.SetErrorMessage(3, "Значения уровней шагового мотора заданы некорректно.");
    Errors.SetErrorMessage(5, "Чередование обмоток непоследовательно.");
    Errors.SetErrorMessage(6, "Функцию GetPOS невозможно вполнить. Параметр УИ не загружен.");
    Errors.SetErrorMessage(7, "Значение текущего положения люфта больше чем границы самого люфта.");
    Errors.SetWarningMessage(1, "Функция DigitalQuantum не выполнена.");
}
//---------------------------------------------------------------------------
TIntervalCharacteristics::TIntervalCharacteristics()
{
    IntervalStartTime = 0;
    IntervalEndTime = 0;
    Noise = 0;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
TIntervalCharacteristics* TStepMotor<TypeOfX, TypeOfY> :: GetLongestIntervalTime(TParameterTMI_Old <TypeOfX, TypeOfY> *SourceParameter)
{
    double DeltaTime = 0;
    double MaxDeltaTime = 0;
    TIntervalCharacteristics* LongestIntervalWithoutImpulses = new TIntervalCharacteristics;

    for(unsigned int i = 1; i < SourceParameter->CountPoint - 1; i++)
    {
        DeltaTime = fabs(fabs(SourceParameter->Points[i+1].X) - fabs(SourceParameter->Points[i].X));
        if(DeltaTime >= MaxDeltaTime)
        {
            MaxDeltaTime = DeltaTime;
            LongestIntervalWithoutImpulses->IntervalStartTime = SourceParameter->Points[i].X;
            LongestIntervalWithoutImpulses->IntervalEndTime = SourceParameter->Points[i+1].X;
        }
    }

    return LongestIntervalWithoutImpulses;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
double TStepMotor<TypeOfX, TypeOfY> ::LuftCalc(double Increment)
{
    if(Increment == 0)
        return 0;

    if(Increment > 0)
    {
        if((CurrentLuftPosition + Increment) <= Luft)
        {
            CurrentLuftPosition += Increment;
            return 0;
        }
        else
        {
            Increment = (Increment + CurrentLuftPosition) - Luft;
            CurrentLuftPosition = Luft;
            return Increment;
        }
    }

    if(Increment < 0)
    {
        if((CurrentLuftPosition + Increment) >= 0)
        {
            CurrentLuftPosition += Increment;
            return 0;
        }
        else
        {
            Increment = CurrentLuftPosition + Increment;
            CurrentLuftPosition = 0;
            return Increment;
        }
    }
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
int TStepMotor<TypeOfX, TypeOfY> ::GetError(AnsiString &Message)
{
    return Errors.GetError(Message);
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
int TStepMotor<TypeOfX, TypeOfY> ::GetWarning(AnsiString &Message)
{
    return Errors.GetWarning(Message);
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
bool TStepMotor<TypeOfX, TypeOfY>::LoadParameter(   TParameterTMI_Old <TypeOfX, TypeOfY> ParameterUI, double Delta_U,               //Параметры моделирования
                                                    double Start_U, TControlImpulse ControlLevels, double Point_Freq,
                                                    double Luft, double CurrentLuftPosition)
{
    // Проверка правильности введенных шага двигателя и частоты опроса
    if(Delta_U == 0)
    {
        Errors.SetError(1);
        return false;
    }

    if(Point_Freq <= 0)
    {
        Errors.SetError(4);
        return false;
    }

    //Проверка правильности значений уровней напряжения на обмотках ШМ
    if (ControlLevels.Level_12Min > ControlLevels.Level_12Max || ControlLevels.Level_23Min > ControlLevels.Level_23Max ||
        ControlLevels.Level_34Min > ControlLevels.Level_34Max || ControlLevels.Level_41Min > ControlLevels.Level_41Max ||
        ControlLevels.Level_0Min > ControlLevels.Level_0Max || ControlLevels.Level_0Max > ControlLevels.Level_12Min ||
        ControlLevels.Level_12Max > ControlLevels.Level_23Min || ControlLevels.Level_23Max > ControlLevels.Level_34Min ||
        ControlLevels.Level_34Max > ControlLevels.Level_41Min)
    {
        Errors.SetError(3);
        return false;
    }

    DeltaU = Delta_U;
    PointFrequency = Point_Freq;
    StartU = Start_U;
    ControlImpulseParam = ControlLevels;
    ControlImpulse = ParameterUI;

    this->Luft = Luft;
    this->CurrentLuftPosition = CurrentLuftPosition;

    if(CurrentLuftPosition > Luft)
    {
        Errors.SetError(7);
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
TParameterTMI_Old <TypeOfX, TypeOfY>* TStepMotor<TypeOfX, TypeOfY>::GetPOS( TypeOfY TimeShift, bool FlagUsage,
                                                                            TypeOfY QuantumLineBottomBorder, TypeOfY QuantumLineTopBorder,
                                                                            unsigned int QuantumLineIntervals, char RoundingMethod,
                                                                            TIntervalCharacteristics* LongestIntervalWithoutControlImpulses)
{
    //AnsiString FName = "D:\\2134241";

    if(PointFrequency == 0)
    {
        Errors.SetError(6);
        return NULL;
    }
    unsigned int Index = 0;
    bool Flag = false;
    double Level, LastLevel;

    //Преобразование в "ступеньку"
    for(unsigned int i = 0; i < ControlImpulse.CountPoint; i++)
    {
        Level = 0;
        if(ControlImpulse.Points[i].Y >= ControlImpulseParam.Level_12Min && ControlImpulse.Points[i].Y <= ControlImpulseParam.Level_12Max)
            Level = -12;
        if(ControlImpulse.Points[i].Y >= ControlImpulseParam.Level_23Min && ControlImpulse.Points[i].Y <= ControlImpulseParam.Level_23Max)
            Level = -23;
        if(ControlImpulse.Points[i].Y >= ControlImpulseParam.Level_34Min && ControlImpulse.Points[i].Y <= ControlImpulseParam.Level_34Max)
            Level = -34;
        if(ControlImpulse.Points[i].Y >= ControlImpulseParam.Level_41Min && ControlImpulse.Points[i].Y <= ControlImpulseParam.Level_41Max)
            Level = -41;
        if(ControlImpulse.Points[i].Y >= ControlImpulseParam.Level_0Min && ControlImpulse.Points[i].Y <= ControlImpulseParam.Level_0Max)
            Level = -10;

        if(!i)
            LastLevel = Level;


        if(LastLevel == Level && Level != 0)
        {
            if(!Flag)
                ControlImpulse.Points[i].Y = Level;
            else
            {
                for(unsigned int j = Index; j <= i; j++)
                    ControlImpulse.Points[j].Y = Level;
                Flag = false;
            }
        }
        else
        {
            if(!Flag)
            {
                Index = i;
                Flag = true;
            }
        }
        LastLevel = Level;
    }

    int LvlSwitch = 1;

    //Сохранение промежуточных результатов
    //SaveToFile(ControlImpulse,FName+"_1.d2");

    //счетчик переключения уровней
    for(unsigned int i = 1; i < ControlImpulse.CountPoint; i++)
    {
        if(ControlImpulse.Points[i].Y != ControlImpulse.Points[i-1].Y)
            LvlSwitch++;
    }

    TParameterTMI_Old <TypeOfX, TypeOfY> CuttedContImp;

    //Преобразование в "пилу"
    CuttedContImp.CountPoint = LvlSwitch;
    CuttedContImp.Points[0].X = ControlImpulse.Points[0].X;
    CuttedContImp.Points[0].Y = ControlImpulse.Points[0].Y;
    LvlSwitch = 1;
    for(unsigned int i = 1; i < ControlImpulse.CountPoint; i++)
    {
        if(ControlImpulse.Points[i].Y != ControlImpulse.Points[i-1].Y)
        {
            CuttedContImp.Points[LvlSwitch].X = ControlImpulse.Points[i].X;
            CuttedContImp.Points[LvlSwitch].Y = ControlImpulse.Points[i].Y;
            LvlSwitch++;
        }
    }

    LongestIntervalWithoutImpulses = *GetLongestIntervalTime(&CuttedContImp);

    if(!LongestIntervalWithoutControlImpulses)
        LongestIntervalWithoutControlImpulses = new TIntervalCharacteristics;

    *LongestIntervalWithoutControlImpulses = LongestIntervalWithoutImpulses;


    //Сохранение промежуточных результатов
    //SaveToFile(CuttedContImp, FName+"_2.d2");

    for(unsigned int i = 0; i < CuttedContImp.CountPoint; i++)
    {
        if(CuttedContImp.Points[i].Y == -12)
            CuttedContImp.Points[i].Y = 1;
        if(CuttedContImp.Points[i].Y == -23)
            CuttedContImp.Points[i].Y = 2;
        if(CuttedContImp.Points[i].Y == -34)
            CuttedContImp.Points[i].Y = 3;
        if(CuttedContImp.Points[i].Y == -41)
            CuttedContImp.Points[i].Y = 4;
        if(CuttedContImp.Points[i].Y == -10)
            CuttedContImp.Points[i].Y = 0;
     }

    //Проверка правильности чередования обмоток
    int BrokenPointsCounter = 1;
    AnsiString ErrLine = "\n";
    AnsiString No1 = "";
    AnsiString No2 = "";
    for(unsigned int i = 1; i < CuttedContImp.CountPoint; i++)
    {
        if(CuttedContImp.Points[i].Y != 0 && CuttedContImp.Points[i-1].Y != 0)
        {
            int delta_lvl = abs(CuttedContImp.Points[i-1].Y - CuttedContImp.Points[i].Y);
            if (delta_lvl == 2)
            {
                if(CuttedContImp.Points[i].Y == 1)
                    No2 = "1-2";
                if(CuttedContImp.Points[i].Y == 2)
                    No2 = "2-3";
                if(CuttedContImp.Points[i].Y == 3)
                    No2 = "3-4";
                if(CuttedContImp.Points[i].Y == 4)
                    No2 = "4-1";
                if(CuttedContImp.Points[i].Y == 0)
                    No2 = "0";
                if(CuttedContImp.Points[i-1].Y == 1)
                    No1 = "1-2";
                if(CuttedContImp.Points[i-1].Y == 2)
                    No1 = "2-3";
                if(CuttedContImp.Points[i-1].Y == 3)
                    No1 = "3-4";
                if(CuttedContImp.Points[i-1].Y == 4)
                    No1 = "4-1";
                if(CuttedContImp.Points[i-1].Y == 0)
                    No1 = "0";

                ErrLine = ErrLine + (AnsiString)BrokenPointsCounter + ". C " + (AnsiString)CuttedContImp.Points[i-1].X + " - обм. " + No1 + " на " + (AnsiString)CuttedContImp.Points[i].X + " - обм. " + No2 + "\n";
                BrokenPointsCounter++;
            }
        }
    }

    //Сохранение промежуточных результатов
    //SaveToFile(CuttedContImp,FName+"_3.d2");

    //Если чередование нарушено, то выводим пользователю точки с ошибочным чередованием
    if(BrokenPointsCounter != 1)
    {
        Errors.SetError(5, ErrLine);
        return NULL;
    }

    //Формирование значений времени параметра ПОС
    double Len = (ControlImpulse.Points[ControlImpulse.CountPoint-1].X - ControlImpulse.Points[0].X) * PointFrequency;
    unsigned int k = Len;

    if (Len - k > 0)
        k++;
    POS.CountPoint = k;
    for(unsigned int i = 0; i < POS.CountPoint; i++)
    {
        POS.Points[i].X = ControlImpulse.Points[0].X + (1 / PointFrequency) * i;
        POS.Points[i].Y = 0;
    }

    //Отслеживание направления шага
    int *Temp = new int [CuttedContImp.CountPoint];
    Temp[0] = 0;
    for(unsigned int i = 1; i < CuttedContImp.CountPoint; i++)
    {
        Temp[i] = 0;
        if(CuttedContImp.Points[i].Y != 0 && CuttedContImp.Points[i-1].Y != 0)
        {
            if(CuttedContImp.Points[i].Y - CuttedContImp.Points[i-1].Y == 1 || CuttedContImp.Points[i].Y - CuttedContImp.Points[i-1].Y == -3)
            {
                Temp[i] = 1;
            }
            if(CuttedContImp.Points[i].Y - CuttedContImp.Points[i-1].Y == -1 || CuttedContImp.Points[i].Y - CuttedContImp.Points[i-1].Y == 3)
            {
                Temp[i] = -1;
            }
        }
    }

    for(unsigned int i = 1; i < CuttedContImp.CountPoint; i++)
        CuttedContImp.Points[i].Y = Temp[i];

    CuttedContImp.Points[0].Y = StartU;

    for(unsigned int i = 1; i < CuttedContImp.CountPoint; i++)
        CuttedContImp.Points[i].Y = CuttedContImp.Points[i-1].Y + LuftCalc(DeltaU * CuttedContImp.Points[i].Y);

    delete [] Temp;

    POS.Points[0].Y = CuttedContImp.Points[0].Y;
    for(unsigned int i = 1; i < POS.CountPoint; i++)
        POS.Points[i].Y = CuttedContImp.Points[CuttedContImp.IndexOfLeftPoint(POS.Points[i].X)].Y;

    //Сдвиг по времени
    if(TimeShift != 0)
        for(unsigned int i = 1; i < POS.CountPoint; i++)
            POS.Points[i].X = POS.Points[i].X + TimeShift;

    //Quantum Web
    bool DQ;
    if(FlagUsage)
        DQ = DigitalQuantum(POS, RoundingMethod, QuantumLineBottomBorder, QuantumLineTopBorder, QuantumLineIntervals);

    if(FlagUsage == true && DQ == false)
        Errors.SetWarning(1);

    TParameterTMI_Old <TypeOfX, TypeOfY>* ReturnParameter = new TParameterTMI_Old <TypeOfX, TypeOfY>;

    (*ReturnParameter) = POS;

    return ReturnParameter;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
void TStepMotor<TypeOfX, TypeOfY>::Clear()
{
    Errors.ClearError();
    Errors.ClearWarning();
    ControlImpulseParam.Level_12Min = 1;
    ControlImpulseParam.Level_12Max = 2;
    ControlImpulseParam.Level_23Min = 3;
    ControlImpulseParam.Level_23Max = 4;
    ControlImpulseParam.Level_34Min = 5;
    ControlImpulseParam.Level_34Max = 6;
    ControlImpulseParam.Level_41Min = 7;
    ControlImpulseParam.Level_41Max = 9;
    ControlImpulseParam.Level_0Min = -1;
    ControlImpulseParam.Level_0Max = 0;
    POS.Clear();
    DeltaU = 0;
    StartU = 0;
    PointFrequency = 0;
    Luft = 0;
    CurrentLuftPosition = 0;
    LongestIntervalWithoutImpulses.IntervalStartTime = 0;
    LongestIntervalWithoutImpulses.IntervalEndTime = 0;
    LongestIntervalWithoutImpulses.Noise = 0;
}
//---------------------------------------------------------------------------
bool TControlImpulse::LoadFormIniFile(AnsiString IniFileName)
{
    if(IniFileName.IsEmpty())
        return false;

    TIniFile *IniFile = new TIniFile(IniFileName);
    TStringList *List = new TStringList;
    IniFile->ReadSection("ControlImpulsesLevels", List);
    IniFile->ReadSectionValues("ControlImpulsesLevels", List);

    if(10 != List->Count)
    {
        delete List;
        delete IniFile;
        return false;
    }

    for(unsigned int i = 0; i < List->Count; ++i)
    {
        if(List->Names[i] == "Level_12Min")
        {
            if(!TryStrToFloat(List->Values[List->Names[i]], this->Level_12Min))
            {
                delete List;
                delete IniFile;
                return false;
            }
            continue;
        }
        else if(List->Names[i] == "Level_12Max")
        {
            if(!TryStrToFloat(List->Values[List->Names[i]], this->Level_12Max))
            {
                delete List;
                delete IniFile;
                return false;
            }
            continue;
        }
        else if(List->Names[i] == "Level_23Min")
        {
            if(!TryStrToFloat(List->Values[List->Names[i]], this->Level_23Min))
            {
                delete List;
                delete IniFile;
                return false;
            }
            continue;
        }
        else if(List->Names[i] == "Level_23Max")
        {
            if(!TryStrToFloat(List->Values[List->Names[i]], this->Level_23Max))
            {
                delete List;
                delete IniFile;
                return false;
            }
            continue;
        }
        else if(List->Names[i] == "Level_34Min")
        {
            if(!TryStrToFloat(List->Values[List->Names[i]], this->Level_34Min))
            {
                delete List;
                delete IniFile;
                return false;
            }
            continue;
        }
        else if(List->Names[i] == "Level_34Max")
        {
            if(!TryStrToFloat(List->Values[List->Names[i]], this->Level_34Max))
            {
                delete List;
                delete IniFile;
                return false;
            }
            continue;
        }
        else if(List->Names[i] == "Level_41Min")
        {
            if(!TryStrToFloat(List->Values[List->Names[i]], this->Level_41Min))
            {
                delete List;
                delete IniFile;
                return false;
            }
            continue;
        }
        else if(List->Names[i] == "Level_41Max")
        {
            if(!TryStrToFloat(List->Values[List->Names[i]], this->Level_41Max))
            {
                delete List;
                delete IniFile;
                return false;
            }
            continue;
        }
        else if(List->Names[i] == "Level_0Min")
        {
            if(!TryStrToFloat(List->Values[List->Names[i]], this->Level_0Min))
            {
                delete List;
                delete IniFile;
                return false;
            }
            continue;
        }
        else if(List->Names[i] == "Level_0Max")
        {
            if(!TryStrToFloat(List->Values[List->Names[i]], this->Level_0Max))
            {
                delete List;
                delete IniFile;
                return false;
            }
            continue;
        }
        else
        {
            delete List;
            delete IniFile;
            return false;
        }
    }

    delete List;
    delete IniFile;
    return true;
}
//---------------------------------------------------------------------------
#endif

