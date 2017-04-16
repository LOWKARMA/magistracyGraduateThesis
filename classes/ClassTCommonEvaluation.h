#ifndef ClassTCommonEvaluationH
#define ClassTCommonEvaluationH

#include <smooth.cpp>
#include <class_ParameterTMI_old.h>
#include <ClassTStepMotor.h>
#include <class_Errors_old.h>
//---------------------------------------------------------------------------
struct TCommonEvaluationParameters  //Параметры общего анализа (первичной оценки)
{
    double QantumStep;                                  //Шаг квантования (мера шума и расхождения)
    double Measure;                                     //Допуск на расхождение
    unsigned int MinimumFailedPoints;
    TCommonEvaluationParameters();
    TCommonEvaluationParameters(double QantumStep, double Measure, unsigned int MinimumFailedPoints);
};
//---------------------------------------------------------------------------
struct TStepMotorCharacteristics    //Параметры ШМ
{
    double DeltaVoltage;
    double MaximumLuft;
    double CurrentLuftPosition;
    TStepMotorCharacteristics();
    TStepMotorCharacteristics(double DeltaVoltage, double MaximumLuft, double CurrentLuftPosition);
};
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
struct TDigitalQuantumParameters    //Параметры АЦП
{
    bool UsageFlag;
    char RoundingMethod;
    TypeOfY QuantumLineBottomBorder;
    TypeOfY QuantumLineTopBorder;
    unsigned int QuantumIntervals;
    TDigitalQuantumParameters();
};
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
struct TPOSModellingParameters      //Параметры моделирования
{
    double StartVoltage;
    double PointsFrequancy;
    TypeOfY TimeShift;
    TPOSModellingParameters();
    //TDigitalQuantumParameters ADCParameters;
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
        void Clear();

    public:
        TCommonEvaluation();
        ~TCommonEvaluation();

        bool Load(TParameterTMI_Old <TypeOfX, TypeOfY> *ControlImpulsesParameter, TControlImpulse *ControlLevels,
                    TPOSModellingParameters <TypeOfX, TypeOfY> *POSModelParameters, TDigitalQuantumParameters <TypeOfX, TypeOfY> *ADCParameters,
                    TParameterTMI_Old <TypeOfX, TypeOfY> *InputPOSParameter,
                    TIntervalCharacteristics *LongestIntervalWithoutImpulses,
                    TCommonEvaluationParameters *EvaluationParmeters);
        bool isNoiseLessThanMeasure();
        bool isNoiseLessThanMeasure(double AlternativeMeasure);
        void SmoothInnerPOSParameter(unsigned int SmoothIterrationsNumber);
        unsigned int CalculateVariationsAndGetFailedIntervals(TIntervalCharacteristics* &FailedIntervalsList);
        int GetError(AnsiString &Message);
        int GetWarning(AnsiString &Message);
};
//---------------------------------------------------------------------------
TCommonEvaluationParameters::TCommonEvaluationParameters()
{
    QantumStep = 0;
    Measure = 0;
    MinimumFailedPoints = 0;
}
//---------------------------------------------------------------------------
TCommonEvaluationParameters::TCommonEvaluationParameters(double QantumStep, double Measure, unsigned int MinimumFailedPoints)
{
    this->QantumStep = QantumStep;
    this->Measure = Measure;
    this->MinimumFailedPoints = MinimumFailedPoints;
}
//---------------------------------------------------------------------------
TStepMotorCharacteristics::TStepMotorCharacteristics()
{
    DeltaVoltage = 0;
    MaximumLuft = 0;
    CurrentLuftPosition = 0;
}
//---------------------------------------------------------------------------
TStepMotorCharacteristics::TStepMotorCharacteristics(double DeltaVoltage, double MaximumLuft, double CurrentLuftPosition)
{
    this->DeltaVoltage = DeltaVoltage;
    this->MaximumLuft = MaximumLuft;
    this->CurrentLuftPosition = CurrentLuftPosition;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
TDigitalQuantumParameters <TypeOfX, TypeOfY>::TDigitalQuantumParameters()
{
    UsageFlag = false;
    RoundingMethod = 'n';
    QuantumLineBottomBorder = 0;
    QuantumLineTopBorder = 0;
    QuantumIntervals = 0;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
TPOSModellingParameters <TypeOfX, TypeOfY>::TPOSModellingParameters()
{
    StartVoltage = 0;
    PointsFrequancy = 0;
    TimeShift = 0;
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

    Errors.SetErrorMessage(1, "В функцию \"Load\" переданы неверные параметры.");
    Errors.SetErrorMessage(2, "Параметры моделирования не заданы.");
    Errors.SetErrorMessage(3, "Пороговые уровни напряжения не заданы.");
    Errors.SetErrorMessage(4, "Параметры сетки АЦП не заданы.");
    Errors.SetErrorMessage(5, "Натурная модель ПОС не задана.");
    Errors.SetErrorMessage(6, "Параметры общего анализа не заданы.");
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
                                                TPOSModellingParameters <TypeOfX, TypeOfY> *POSModelParameters, TDigitalQuantumParameters <TypeOfX, TypeOfY> *ADCParameters,
                                                TParameterTMI_Old <TypeOfX, TypeOfY> *InputPOSParameter,
                                                TIntervalCharacteristics *LongestIntervalWithoutImpulses,
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

    if(this->StepMotor)
        StepMotor->Clear();
    else
        StepMotor = new TStepMotor <TypeOfX, TypeOfY>;

    if(!StepMotor->LoadParameter(*ControlImpulsesParameter, POSModelParameters->DeltaVoltage, POSModelParameters->StartVoltage,
                            *ControlLevels, POSModelParameters->PointsFrequancy, POSModelParameters->MaximumLuft,
                            POSModelParameters->CurrentLuftPosition))
        return false;

    if(this->POSModel)
        delete POSModel;
    else
        POSModel = new TParameterTMI_Old <TypeOfX, TypeOfY>;

    POSModel = StepMotor->GetPOS(POSModelParameters->TimeShift, ADCParameters->UsageFlag,
                                    ADCParameters->QuantumLineBottomBorder, ADCParameters->QuantumLineTopBorder,
                                    ADCParameters->QuantumIntervals, ADCParameters->RoundingMethod, LongestIntervalWithoutImpulses);
    if(!POSModel)
        return false;

    if(!LoadInnerParameters(InputPOSParameter, LongestIntervalWithoutImpulses, QuantumStep, Measure, MinimumFailedPoints))
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
        this->LongestIntervalWithoutImpulses = new TIntervalCharacteristics;
        *this->POSParameter = *InputPOSParameter;
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

    for(unsigned int i = 0; i < FirstParameter->CountPoint; i++)
    {
        if( fabs(FirstParameter->Points[i].Y - SecondParameter->Points[i].Y) > Measure )
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
#endif
