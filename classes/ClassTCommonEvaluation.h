#ifndef ClassTCommonEvaluationH
#define ClassTCommonEvaluationH

#include <smooth.cpp>
#include <class_ParameterTMI_old.h>
#include <class_ParameterTMI.h>
#include <ClassTStepMotor.h>
#include <class_Errors_old.h>
#include <math.h>
#include <IniFiles.hpp>
#include <SomeFunctions.h>
//---------------------------------------------------------------------------
struct TCommonEvaluationParameters  //Параметры общего анализа (первичной оценки)
{
    double QantumStep;                                  //Шаг квантования (мера шума и расхождения)
    double Measure;                                     //Допуск на расхождение
    int MinimumFailedPoints;                            //Минимальное кол-во точек, в "сбойном интервале"
    bool LoadFormIniFile(AnsiString InputFileName);       //Чтение из .ini и загрузка
    void SaveToIniFile(AnsiString OutputFileName);
    TCommonEvaluationParameters();
    TCommonEvaluationParameters(double QantumStep, double Measure, int MinimumFailedPoints);
    TCommonEvaluationParameters(AnsiString InputFileName);
};
//---------------------------------------------------------------------------
struct TStepMotorCharacteristics    //Параметры ШМ
{
    double DeltaVoltage;
    double MaximumLuft;
    double CurrentLuftPosition;
    bool LoadFormIniFile(AnsiString InputFileName);
    void SaveToIniFile(AnsiString OutputFileName);
    TStepMotorCharacteristics();
    TStepMotorCharacteristics(double DeltaVoltage, double MaximumLuft, double CurrentLuftPosition);
    TStepMotorCharacteristics(AnsiString InputFileName);
};
//---------------------------------------------------------------------------
template <class Type>
struct TDigitalQuantumParameters    //Параметры АЦП
{
    bool UsageFlag;
    char RoundingMethod;
    Type QuantumLineBottomBorder;
    Type QuantumLineTopBorder;
    int QuantumIntervals;
    bool LoadFormIniFile(AnsiString InputFileName);
    void SaveToIniFile(AnsiString OutputFileName);
    TDigitalQuantumParameters();
    TDigitalQuantumParameters(bool UsageFlag, char RoundingMethod, Type QuantumLineBottomBorder, Type QuantumLineTopBorder, int QuantumIntervals);
    TDigitalQuantumParameters(AnsiString InputFileName);
};
//---------------------------------------------------------------------------
template <class Type>
struct TPOSModellingParameters      //Параметры моделирования
{
    double StartVoltage;
    double PointsFrequancy;
    Type TimeShift;
    bool LoadFormIniFile(AnsiString InputFileName);
    void SaveToIniFile(AnsiString OutputFileName);
    TPOSModellingParameters();
    TPOSModellingParameters(double StartVoltage, double PointsFrequancy, Type TimeShift);
    TPOSModellingParameters(AnsiString InputFileName);
};
//---------------------------------------------------------------------------

template <class TypeOfX, class TypeOfY>
class TCommonEvaluation
{
    private:
        TStepMotor <TypeOfX, TypeOfY> *StepMotor;                   //Содержит модель ПОС
        TParameterTMI_Old <TypeOfX, TypeOfY> *POSModel;
        TParameterTMI_Old <TypeOfX, TypeOfY> *POSParameter;         //Параметр ПОС, полученный в результате испытаний
        TParameterTMI_Old <TypeOfX, TypeOfY> *POSParameterSmoothed; //Сглаженный параметр ПОС
        TIntervalCharacteristics *FailedIntervals;                  //Массив интервалов, не удовалетворяющих критерию оценки
        TIntervalCharacteristics *LongestIntervalWithoutImpulses;   //Самый длительный интервал, на котором не выдаются импульсы
        TErrors_old Errors;
        TCommonEvaluationParameters Parameters;
        /*
        double QantumStep;                                  //Шаг квантования (мера шума и расхождения)
        double Measure;                                     //Допуск на расхождение
        unsigned int MinimumFailedPoints;
        */

        double CalculateNoiseOnInterval(TParameterTMI_Old <TypeOfX, TypeOfY> *SourceParameter, TIntervalCharacteristics *FailedIntervals, double QantumStep);  //Посчитать шум в шагах квантования.
        bool LoadInnerParameters(TParameterTMI_Old <TypeOfX, TypeOfY> *InputPOSParameter, TIntervalCharacteristics* LongestIntervalWithoutImpulses, TCommonEvaluationParameters *EvaluationParmeters);
        void Clear(); //rabotaet li?

    public:
        TCommonEvaluation();
        ~TCommonEvaluation();

        bool Load(TParameterTMI_Old <TypeOfX, TypeOfY> *ControlImpulsesParameter, TControlImpulse *ControlLevels,
                    TParameterTMI_Old <TypeOfX, TypeOfY> *InputPOSParameter,
                    TIntervalCharacteristics *LongestIntervalWithoutImpulses,
                    TStepMotorCharacteristics* StepMotorCharacteristics,
                    TPOSModellingParameters <TypeOfY> *POSModelParameters,
                    TDigitalQuantumParameters <TypeOfY> *ADCParameters,
                    TCommonEvaluationParameters *EvaluationParmeters);

        bool Load(TParameterTMI_Old <TypeOfX, TypeOfY> *InputPOSParameter, TParameterTMI_Old <TypeOfX, TypeOfY> *InputPOSEtalon,
                    TIntervalCharacteristics *LongestIntervalWithoutImpulses, TCommonEvaluationParameters *EvaluationParmeters);
        bool isNoiseLessThanMeasure();
        bool isNoiseLessThanMeasure(double AlternativeMeasure);
        void SmoothInnerPOSParameter(unsigned int SmoothIterrationsNumber);
        unsigned int CalculateVariationsAndGetFailedIntervals(TIntervalCharacteristics* &FailedIntervalsList);
        int GetError(AnsiString &Message);
        int GetWarning(AnsiString &Message);
        void SaveModelToFile(AnsiString FileName);
        TPointTMI* GetModelAsArrayOfPoints(long &PointsCounter);
};
//---------------------------------------------------------------------------
TCommonEvaluationParameters::TCommonEvaluationParameters()
{
    QantumStep = 0;
    Measure = 0;
    MinimumFailedPoints = 0;
}
//---------------------------------------------------------------------------
bool TCommonEvaluationParameters::LoadFormIniFile(AnsiString InputFileName)
{
    if(InputFileName.IsEmpty())
        return false;

    TIniFile *IniFile = new TIniFile(InputFileName);
    TStringList *List = new TStringList;
    //IniFile->ReadSection("CommonEvaluationParameters", List);
    IniFile->ReadSectionValues("CommonEvaluationParameters", List);

    if(3 != List->Count)
    {
        delete List;
        delete IniFile;
        return false;
    }

    for(int i = 0; i < List->Count; ++i)
    {
        if(List->Names[i] == "QantumStep")
        {
            if(!TryStrToFloat(List->Values[List->Names[i]], this->QantumStep))
            {
                delete List;
                delete IniFile;
                return false;
            }
            continue;
        }
        else if(List->Names[i] == "Measure")
        {
            if(!TryStrToFloat(List->Values[List->Names[i]], this->Measure))
            {
                delete List;
                delete IniFile;
                return false;
            }
            continue;
        }
        else if(List->Names[i] == "MinimumFailedPoints")
        {
            int tmp;
            if(!TryStrToInt(List->Values[List->Names[i]], tmp))
            {
                delete List;
                delete IniFile;
                return false;
            }
            this->MinimumFailedPoints = abs(tmp);
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
void TCommonEvaluationParameters::SaveToIniFile(AnsiString OutputFileName)
{
    TIniFile *IniFile = new TIniFile(OutputFileName);

    IniFile->WriteFloat("CommonEvaluationParameters", "QantumStep", QantumStep);
    IniFile->WriteFloat("CommonEvaluationParameters", "Measure", Measure);
    IniFile->WriteInteger("CommonEvaluationParameters", "MinimumFailedPoints", MinimumFailedPoints);

    delete IniFile;
}
//---------------------------------------------------------------------------
TCommonEvaluationParameters::TCommonEvaluationParameters(double QantumStep, double Measure, int MinimumFailedPoints)
{
    this->QantumStep = QantumStep;
    this->Measure = Measure;
    this->MinimumFailedPoints = MinimumFailedPoints;
}
//---------------------------------------------------------------------------
TCommonEvaluationParameters::TCommonEvaluationParameters(AnsiString InputFileName)
{
    if(InputFileName.IsEmpty())
    {
        QantumStep = 0;
        Measure = 0;
        MinimumFailedPoints = 0;
        return;
    }
    TIniFile *IniFile = new TIniFile(InputFileName);

    QantumStep = IniFile->ReadFloat("CommonEvaluationParameters", "QantumStep", 0);
    Measure = IniFile->ReadFloat("CommonEvaluationParameters", "Measure", 0);
    MinimumFailedPoints = IniFile->ReadInteger("CommonEvaluationParameters", "MinimumFailedPoints", 0);

    delete IniFile;
    return;
}
//---------------------------------------------------------------------------
TStepMotorCharacteristics::TStepMotorCharacteristics()
{
    DeltaVoltage = 0;
    MaximumLuft = 0;
    CurrentLuftPosition = 0;
}
//---------------------------------------------------------------------------
bool TStepMotorCharacteristics::LoadFormIniFile(AnsiString InputFileName)
{
    if(InputFileName.IsEmpty())
        return false;

    TIniFile *IniFile = new TIniFile(InputFileName);
    TStringList *List = new TStringList;
    IniFile->ReadSection("StepMotorCharacteristics", List);
    IniFile->ReadSectionValues("StepMotorCharacteristics", List);

    if(3 != List->Count)
    {
        delete List;
        delete IniFile;
        return false;
    }

    for(int i = 0; i < List->Count; ++i)
    {
        if(List->Names[i] == "DeltaVoltage")
        {
            if(!TryStrToFloat(List->Values[List->Names[i]], this->DeltaVoltage))
            {
                delete List;
                delete IniFile;
                return false;
            }
            continue;
        }
        else if(List->Names[i] == "MaximumLuft")
        {
            if(!TryStrToFloat(List->Values[List->Names[i]], this->MaximumLuft))
            {
                delete List;
                delete IniFile;
                return false;
            }
            continue;
        }
        else if(List->Names[i] == "CurrentLuftPosition")
        {
            if(!TryStrToFloat(List->Values[List->Names[i]], this->CurrentLuftPosition))
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
void TStepMotorCharacteristics::SaveToIniFile(AnsiString OutputFileName)
{
    TIniFile *IniFile = new TIniFile(OutputFileName);

    IniFile->WriteFloat("StepMotorCharacteristics", "DeltaVoltage", DeltaVoltage);
    IniFile->WriteFloat("StepMotorCharacteristics", "MaximumLuft", MaximumLuft);
    IniFile->WriteInteger("StepMotorCharacteristics", "CurrentLuftPosition", CurrentLuftPosition);

    delete IniFile;
}
//---------------------------------------------------------------------------
TStepMotorCharacteristics::TStepMotorCharacteristics(double DeltaVoltage, double MaximumLuft, double CurrentLuftPosition)
{
    this->DeltaVoltage = DeltaVoltage;
    this->MaximumLuft = MaximumLuft;
    this->CurrentLuftPosition = CurrentLuftPosition;
}
//---------------------------------------------------------------------------
TStepMotorCharacteristics::TStepMotorCharacteristics(AnsiString InputFileName)
{
    if(InputFileName.IsEmpty())
    {
        DeltaVoltage = 0;
        MaximumLuft = 0;
        CurrentLuftPosition = 0;
        return;
    }
    TIniFile *IniFile = new TIniFile(InputFileName);

    DeltaVoltage = IniFile->ReadFloat("StepMotorCharacteristics", "DeltaVoltage", 0);
    MaximumLuft = IniFile->ReadFloat("StepMotorCharacteristics", "MaximumLuft", 0);
    CurrentLuftPosition = IniFile->ReadFloat("StepMotorCharacteristics", "CurrentLuftPosition", 0);

    delete IniFile;
    return;
}
//---------------------------------------------------------------------------
template <class Type>
TDigitalQuantumParameters <Type>::TDigitalQuantumParameters()
{
    UsageFlag = false;
    RoundingMethod = 'n';
    QuantumLineBottomBorder = 0;
    QuantumLineTopBorder = 0;
    QuantumIntervals = 0;
}
//---------------------------------------------------------------------------
template <class Type>
TDigitalQuantumParameters <Type>::TDigitalQuantumParameters(bool UsageFlag, char RoundingMethod, Type QuantumLineBottomBorder, Type QuantumLineTopBorder, int QuantumIntervals)
{
    this->UsageFlag = UsageFlag;
    this->RoundingMethod = RoundingMethod;
    this->QuantumIntervals = QuantumIntervals;
    this->QuantumLineBottomBorder = QuantumLineBottomBorder;
    this->QuantumLineTopBorder = QuantumLineTopBorder;
}
//---------------------------------------------------------------------------
template <class Type>
bool TDigitalQuantumParameters <Type>::LoadFormIniFile(AnsiString InputFileName)
{
    if(InputFileName.IsEmpty())
        return false;

    TIniFile *IniFile = new TIniFile(InputFileName);
    TStringList *List = new TStringList;
    IniFile->ReadSection("DigitalQuantumParameters", List);
    IniFile->ReadSectionValues("DigitalQuantumParameters", List);

    if(5 != List->Count)
    {
        delete List;
        delete IniFile;
        return false;
    }

    for( int i = 0; i < List->Count; ++i)
    {
        if(List->Names[i] == "UsageFlag")
        {
            if(!TryStrToBool(List->Values[List->Names[i]], this->UsageFlag))
            {
                delete List;
                delete IniFile;
                return false;
            }
            continue;
        }
        else if(List->Names[i] == "RoundingMethod")
        {
            this->RoundingMethod = List->Values[List->Names[i]][1];
            continue;
        }
        else if(List->Names[i] == "QuantumLineBottomBorder")
        {
            if(!TryStrToFloat(List->Values[List->Names[i]], this->QuantumLineBottomBorder))
            {
                delete List;
                delete IniFile;
                return false;
            }
            continue;
        }
        else if(List->Names[i] == "QuantumLineTopBorder")
        {
            if(!TryStrToFloat(List->Values[List->Names[i]], this->QuantumLineTopBorder))
            {
                delete List;
                delete IniFile;
                return false;
            }
            continue;
        }

        else if(List->Names[i] == "QuantumIntervals")
        {
            int tmp;
            if(!TryStrToInt(List->Values[List->Names[i]], tmp))
            {
                delete List;
                delete IniFile;
                return false;
            }
            this->QuantumIntervals = abs(tmp);
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
template <class Type>
void TDigitalQuantumParameters <Type>::SaveToIniFile(AnsiString OutputFileName)
{
    TIniFile *IniFile = new TIniFile(OutputFileName);

    IniFile->WriteBool("DigitalQuantumParameters", "UsageFlag", UsageFlag);
    IniFile->WriteString("DigitalQuantumParameters", "RoundingMethod", RoundingMethod);
    IniFile->WriteFloat("DigitalQuantumParameters", "QuantumLineBottomBorder", QuantumLineBottomBorder);
    IniFile->WriteFloat("DigitalQuantumParameters", "QuantumLineTopBorder", QuantumLineTopBorder);
    IniFile->WriteInteger("DigitalQuantumParameters", "QuantumIntervals", QuantumIntervals);

    delete IniFile;
}
//---------------------------------------------------------------------------
template <class Type>
TDigitalQuantumParameters <Type>::TDigitalQuantumParameters(AnsiString InputFileName)
{
    if(InputFileName.IsEmpty())
    {
        UsageFlag = false;
        RoundingMethod = 'n';
        QuantumLineBottomBorder = 0;
        QuantumLineTopBorder = 0;
        QuantumIntervals = 0;
        return;
    }

    TIniFile *IniFile = new TIniFile(InputFileName);

    AnsiString tmp;
    UsageFlag = IniFile->ReadBool("DigitalQuantumParameters", "UsageFlag", false);
    tmp = IniFile->ReadString("DigitalQuantumParameters", "RoundingMethod", "n");
    RoundingMethod = tmp[1];
    QuantumLineBottomBorder = IniFile->ReadFloat("DigitalQuantumParameters", "QuantumLineBottomBorder", 0);
    QuantumLineTopBorder = IniFile->ReadFloat("DigitalQuantumParameters", "QuantumLineTopBorder", 0);
    QuantumIntervals = IniFile->ReadInteger("DigitalQuantumParameters", "QuantumIntervals", 0);

    delete IniFile;
    return;

}
//---------------------------------------------------------------------------
template <class Type>
TPOSModellingParameters <Type>::TPOSModellingParameters()
{
    StartVoltage = 0;
    PointsFrequancy = 0;
    TimeShift = 0;
}
//---------------------------------------------------------------------------
template <class Type>
TPOSModellingParameters <Type>::TPOSModellingParameters(double StartVoltage, double PointsFrequancy, Type TimeShift)
{
    this->StartVoltage = StartVoltage;
    this->PointsFrequancy = PointsFrequancy;
    this->TimeShift = TimeShift;
}
//---------------------------------------------------------------------------
template <class Type>
bool TPOSModellingParameters <Type>::LoadFormIniFile(AnsiString InputFileName)
{
    if(InputFileName.IsEmpty())
        return false;

    TIniFile *IniFile = new TIniFile(InputFileName);
    TStringList *List = new TStringList;
    IniFile->ReadSection("ModellingParmeters", List);
    IniFile->ReadSectionValues("ModellingParmeters", List);

    if(3 != List->Count)
    {
        delete List;
        delete IniFile;
        return false;
    }

    double tmp;
    for(int i = 0; i < List->Count; ++i)
    {
        if( !TryStrToFloat( List->Values[List->Names[i]], tmp ))
        {
            delete List;
            delete IniFile;
            return false;
        }
        else
        {
            if(List->Names[i] == "StartVoltage")
                this->StartVoltage = tmp;

            else if(List->Names[i] == "PointsFrequancy")
                this->PointsFrequancy = tmp;

            else if(List->Names[i] == "TimeShift")
                this->TimeShift = tmp;
        }
    }

    delete List;
    delete IniFile;
    return true;
}
//---------------------------------------------------------------------------
template <class Type>
void TPOSModellingParameters <Type>::SaveToIniFile(AnsiString OutputFileName)
{
    TIniFile *IniFile = new TIniFile(OutputFileName);

    IniFile->WriteFloat("ModellingParmeters", "StartVoltage", StartVoltage);
    IniFile->WriteFloat("ModellingParmeters", "PointsFrequancy", PointsFrequancy);
    IniFile->WriteInteger("ModellingParmeters", "TimeShift", TimeShift);

    delete IniFile;
}
//---------------------------------------------------------------------------
template <class Type>
TPOSModellingParameters <Type>::TPOSModellingParameters(AnsiString InputFileName)
{
    if(InputFileName.IsEmpty())
    {
        StartVoltage = 0;
        PointsFrequancy = 0;
        TimeShift = 0;
        return;
    }
    TIniFile *IniFile = new TIniFile(InputFileName);

    StartVoltage = IniFile->ReadFloat("ModellingParmeters", "StartVoltage", 0);
    PointsFrequancy = IniFile->ReadFloat("ModellingParmeters", "PointsFrequancy", 0);
    TimeShift = IniFile->ReadFloat("ModellingParmeters", "TimeShift", 0);

    delete IniFile;
    return;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
TCommonEvaluation <TypeOfX, TypeOfY>::TCommonEvaluation()
{
    StepMotor = NULL;
    POSModel = NULL;
    POSParameter = NULL;
    POSParameterSmoothed = NULL;
    FailedIntervals = NULL;
    LongestIntervalWithoutImpulses = NULL;
    Parameters.QantumStep = 0;
    Parameters.Measure = 0;
    Parameters.MinimumFailedPoints = 0;

    Errors.SetErrorMessage(1, "В функцию \"LoadInnerParameters\" переданы неверные параметры.");
    Errors.SetErrorMessage(2, "Параметры моделирования не заданы.");
    Errors.SetErrorMessage(3, "Пороговые уровни напряжения не заданы.");
    Errors.SetErrorMessage(4, "Параметры сетки АЦП не заданы.");
    Errors.SetErrorMessage(5, "Натурная модель ПОС не задана.");
    Errors.SetErrorMessage(6, "Параметры общего анализа не заданы.");
    Errors.SetErrorMessage(7, "Параметры характеристики ШМ не заданы.");
    Errors.SetErrorMessage(8, "Эталонная модель ПОС не задана.");
    Errors.SetErrorMessage(9, "Самый длительный интервал, на котором не выдаются управляющие импульсы не задан.");
    Errors.SetWarningMessage(1, "Параметр не загружен. Функция Smooth не выполнена.");

    /*
    Errors.SetErrorMessage(6, "TEST.");
    Errors.SetErrorMessage(7, "Значение текущего положения люфта больше чем границы самого люфта.");
    Errors.SetWarningMessage(1, "Функция DigitalQuantum не выполнена.");
    */
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
TCommonEvaluation <TypeOfX, TypeOfY>::~TCommonEvaluation()
{
    delete POSModel;
    delete StepMotor;
    delete POSParameter;
    delete POSParameterSmoothed;
    delete [] FailedIntervals;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
void TCommonEvaluation <TypeOfX, TypeOfY>::Clear()
{
    delete POSModel;
    delete POSParameter;
    delete POSParameterSmoothed;
    delete [] FailedIntervals;

    POSModel = NULL;
    POSParameter = NULL;
    POSParameterSmoothed = NULL;
    FailedIntervals = NULL;
    LongestIntervalWithoutImpulses = NULL;
    Parameters.QantumStep = 0;
    Parameters.Measure = 0;
    Parameters.MinimumFailedPoints = 0;
    StepMotor->Clear();
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
bool TCommonEvaluation <TypeOfX, TypeOfY>::Load(TParameterTMI_Old <TypeOfX, TypeOfY> *ControlImpulsesParameter, TControlImpulse *ControlLevels,
                                                TParameterTMI_Old <TypeOfX, TypeOfY> *InputPOSParameter,
                                                TIntervalCharacteristics *LongestIntervalWithoutImpulses,
                                                TStepMotorCharacteristics* StepMotorCharacteristics,
                                                TPOSModellingParameters <TypeOfY> *POSModelParameters,
                                                TDigitalQuantumParameters <TypeOfY> *ADCParameters,
                                                TCommonEvaluationParameters *EvaluationParmeters)
{
    if(!ControlImpulsesParameter)
    {
        Errors.SetError(2);
        return false;
    }
    if(!ControlLevels)
    {
        Errors.SetError(3);
        return false;
    }
    if(!ADCParameters)
    {
        Errors.SetError(4);
        return false;
    }
    if(!InputPOSParameter)
    {
        Errors.SetError(5);
        return false;
    }
    if(!EvaluationParmeters)
    {
        Errors.SetError(6);
        return false;
    }
    if(!StepMotorCharacteristics)
    {
        Errors.SetError(7);
        return false;
    }

    if(this->StepMotor)
        StepMotor->Clear();
    else
        StepMotor = new TStepMotor <TypeOfX, TypeOfY>;

    if(!StepMotor->LoadParameter(*ControlImpulsesParameter, StepMotorCharacteristics->DeltaVoltage, POSModelParameters->StartVoltage,
                            *ControlLevels, POSModelParameters->PointsFrequancy, StepMotorCharacteristics->MaximumLuft,
                            StepMotorCharacteristics->CurrentLuftPosition))
        return false;

    if(this->POSModel)
        delete POSModel;

    POSModel = new TParameterTMI_Old <TypeOfX, TypeOfY>;

    POSModel = StepMotor->GetPOS(POSModelParameters->TimeShift, ADCParameters->UsageFlag,
                                    ADCParameters->QuantumLineBottomBorder, ADCParameters->QuantumLineTopBorder,
                                    ADCParameters->QuantumIntervals, ADCParameters->RoundingMethod, LongestIntervalWithoutImpulses);
    if(!POSModel)
        return false;

    if(!LoadInnerParameters(InputPOSParameter, LongestIntervalWithoutImpulses, EvaluationParmeters))
        return false;

    return true;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
bool TCommonEvaluation <TypeOfX, TypeOfY>::Load(TParameterTMI_Old <TypeOfX, TypeOfY> *InputPOSParameter, TParameterTMI_Old <TypeOfX, TypeOfY> *InputPOSEtalon,
                    TIntervalCharacteristics *LongestIntervalWithoutImpulses, TCommonEvaluationParameters *EvaluationParmeters)
{
    if(!InputPOSParameter)
    {
        Errors.SetError(5);
        return false;
    }
    if(!EvaluationParmeters)
    {
        Errors.SetError(6);
        return false;
    }
    if(!InputPOSEtalon)
    {
        Errors.SetError(8);
        return false;
    }
    if(!LongestIntervalWithoutImpulses)
    {
        Errors.SetError(9);
        return false;
    }

    if(this->POSModel)
        delete POSModel;

    POSModel = new TParameterTMI_Old <TypeOfX, TypeOfY>;
    POSModel = *InputPOSEtalon;

    if(!LoadInnerParameters(InputPOSParameter, LongestIntervalWithoutImpulses, EvaluationParmeters))
        return false;

    return true;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
bool TCommonEvaluation <TypeOfX, TypeOfY>::LoadInnerParameters(TParameterTMI_Old <TypeOfX, TypeOfY> *InputPOSParameter, TIntervalCharacteristics* LongestIntervalWithoutImpulses, TCommonEvaluationParameters *EvaluationParmeters)
{
    if(this->POSParameter && this->LongestIntervalWithoutImpulses)
    {
        this->Clear();
        StepMotor->Clear();
    }

    if(InputPOSParameter && LongestIntervalWithoutImpulses)
    {
        this->POSParameter = new TParameterTMI_Old <TypeOfX, TypeOfY>;
        *this->POSParameter = *InputPOSParameter;
        this->LongestIntervalWithoutImpulses = new TIntervalCharacteristics;
        *this->LongestIntervalWithoutImpulses = *LongestIntervalWithoutImpulses;
    }
    else
    {
        Errors.SetError(1);
        return false;
    }

    Parameters = *EvaluationParmeters;

    /*
    this->QantumStep = QuantumStep;
    this->Measure = Measure;
    this->MinimumFailedPoints = MinimumFailedPoints;
    */

    this->LongestIntervalWithoutImpulses->Noise = CalculateNoiseOnInterval(this->POSParameter, this->LongestIntervalWithoutImpulses, Parameters.QantumStep);

    return true;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
bool TCommonEvaluation  <TypeOfX, TypeOfY>::isNoiseLessThanMeasure()
{
    if(LongestIntervalWithoutImpulses->Noise <= Measure)
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
bool TCommonEvaluation  <TypeOfX, TypeOfY>::isNoiseLessThanMeasure(double AlternativeMeasure)
{
    if(LongestIntervalWithoutImpulses->Noise <= AlternativeMeasure)
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
void TCommonEvaluation  <TypeOfX, TypeOfY>::SmoothInnerPOSParameter(unsigned int SmoothIterrationsNumber)
{
    if(!POSParameter)
    {
        Errors.SetWarning(1);
        return;
    }
    if(POSParameterSmoothed)
        delete POSParameterSmoothed;
    else
    {
        POSParameterSmoothed = new TParameterTMI_Old <TypeOfX, TypeOfY>;
        POSParameterSmoothed->CountPoint = POSParameter->CountPoint;
    }
    smooth(POSParameter, POSParameterSmoothed, SmoothIterrationsNumber);
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
double TCommonEvaluation <TypeOfX, TypeOfY>::CalculateNoiseOnInterval(TParameterTMI_Old <TypeOfX, TypeOfY> *SourceParameter, TIntervalCharacteristics *FailedIntervals, double QantumStep)
{
    TParameterTMI_Old <TypeOfX, TypeOfY> TempPar = SourceParameter->SubInterval(LongestIntervalWithoutImpulses->IntervalStartTime, LongestIntervalWithoutImpulses->IntervalEndTime);
    TPointTMI_Old <TypeOfX, TypeOfY> TempPoint;
    TempPoint = TempPar.Min_Y();
    double MinAmplitude = TempPoint.Y;
    TempPoint = TempPar.Max_Y();
    double MaxAmplitude = TempPoint.Y;
    double AverageAmplitude = TempPar.AverageY();
    double Noise = fabs(AverageAmplitude - MinAmplitude);
    if(Noise >= fabs(AverageAmplitude - MaxAmplitude))
        return Noise;
    else
        return fabs(AverageAmplitude - MaxAmplitude);
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
unsigned int TCommonEvaluation <TypeOfX, TypeOfY>::CalculateVariationsAndGetFailedIntervals(TIntervalCharacteristics* &FailedIntervalsList)
{
    if(FailedIntervals)
        delete [] FailedIntervals;

    TParameterTMI_Old <TypeOfX, TypeOfY> *FirstParameter = POSModel;
    TParameterTMI_Old <TypeOfX, TypeOfY> *SecondParameter = POSParameterSmoothed;
    unsigned int FailedIntervalsCounter = 0;
    unsigned int FailedIntervalPointsCounter = 0;
    TIntervalCharacteristics Temp;
    bool isInsideFailedInterval = false;

    for(unsigned int i = 0; i < FirstParameter->CountPoint - 1; i++)
    {
        if( fabs(FirstParameter->Points[i].Y - SecondParameter->Points[i].Y) > Parameters.Measure )
        {
            if(!isInsideFailedInterval)
            {
                isInsideFailedInterval = true;
                Temp.IntervalStartTime = FirstParameter->Points[i].X;
            }
            FailedIntervalPointsCounter++;

            if(FirstParameter->CountPoint - 1 == i)
            {
                Temp.IntervalEndTime = FirstParameter->Points[i].X;
                isInsideFailedInterval = false;
            }
        }
        else
        {
            if(isInsideFailedInterval)
            {
                Temp.IntervalEndTime = FirstParameter->Points[i - 1].X;
                isInsideFailedInterval = false;
            }
        }

        if(FailedIntervalPointsCounter >= Parameters.MinimumFailedPoints && !isInsideFailedInterval)  //////////////////////////////////////////////////////////////
        {
            FailedIntervalsCounter++;
            TIntervalCharacteristics* TempPTR = FailedIntervals;
            FailedIntervals = new TIntervalCharacteristics[FailedIntervalsCounter];

            if(FailedIntervalsCounter > 1)
            {
                for(unsigned int j = 0; j < FailedIntervalsCounter - 1; j++)
                    FailedIntervals[j] = TempPTR[j];

                FailedIntervals[FailedIntervalsCounter - 1] = Temp;
                delete [] TempPTR;
            }
            else
                FailedIntervals[0] = Temp;

            FailedIntervalPointsCounter = 0;
        }
    }

    FailedIntervalsList = new TIntervalCharacteristics [FailedIntervalsCounter];
    for(unsigned int i = 0; i < FailedIntervalsCounter; i++)
        FailedIntervalsList[i] = FailedIntervals[i];

    return FailedIntervalsCounter;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
int TCommonEvaluation <TypeOfX, TypeOfY>::GetError(AnsiString &Message)
{
    int ReturnValue = Errors.GetError(Message);
    if(StepMotor)
    {
        AnsiString StepMotorErrorMessage;
        StepMotor->GetError(StepMotorErrorMessage);
        StepMotorErrorMessage = "Ошибка класса TStepMotor: " + StepMotorErrorMessage;
        if((AnsiString)"\0" != StepMotorErrorMessage)
            Message = Message + StepMotorErrorMessage;

        return ReturnValue;
    }
    else ReturnValue;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
int TCommonEvaluation <TypeOfX, TypeOfY>::GetWarning(AnsiString &Message)
{
    int ReturnValue = Errors.Warning(Message);
    if(StepMotor)
    {
        AnsiString StepMotorWarningMessage;
        StepMotor->Warning(StepMotorWarningMessage);
        StepMotorErrorMessage = "Сообщение класса TStepMotor: " + StepMotorWarningMessage;
        if((AnsiString)"\0" != StepMotorWarningessage)
            Message = Message + StepMotorWarningMessage;

        return ReturnValue;
    }
    else ReturnValue;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
TPointTMI* TCommonEvaluation <TypeOfX, TypeOfY>::GetModelAsArrayOfPoints(long &PointsCounter)
{
    if(!POSModel)
        return NULL;

    TPointTMI *RetVal = new TPointTMI [POSModel->CountPoint];
    TPointTMI_Old <TypeOfX, TypeOfY> *TMP = new TPointTMI_Old <TypeOfX, TypeOfY>;
    TMP = POSModel->GetMasPointTMI();
    for(int i = 0; i < POSModel->CountPoint; i++)
        RetVal[i] = TMP[i].Convert();

    PointsCounter = POSModel->CountPoint;
    delete TMP;
    return RetVal;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
void TCommonEvaluation <TypeOfX, TypeOfY>::SaveModelToFile(AnsiString FileName)
{
    if(POSModel)
        SaveToFile(*POSModel,  FileName+"_model.d2");
    else
        return;
}
//---------------------------------------------------------------------------
#endif
