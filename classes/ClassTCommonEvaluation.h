#ifndef ClassTCommonEvaluationH
#define ClassTCommonEvaluationH

#include <smooth.cpp>
#include <class_ParameterTMI.h>


struct TIntervalCharacteristics
{
    double IntervalStartTime;
    double IntervalEndTime;
    double Noise;
    TIntervalCharacteristics();
};
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
class TCommonEvaluation
{
    private:
        TParameterTMI_Old <TypeOfX, TypeOfY> *POSModel;      //Модель ПОС, полученная в результате работы класса TSpetMotor
        TParameterTMI_Old <TypeOfX, TypeOfY> *POSParameter;  //Параметр ПОС, полученный в результате испытаний
        TParameterTMI_Old <TypeOfX, TypeOfY> *POSParameterSmoothed; //Сглаженный параметр ПОС
        TIntervalCharacteristics* FailedIntervals;           //Массив интервалов, не удовалетворяющих критерию оценки
        TIntervalCharacteristics* LongestIntervalWithoutImpulses;   //Самый длительный интервал, на котором не выдаются импульсы

        double QantumStep;                                  //Шаг квантования (мера шума и расхождения)
        double Measure;                                     //Допуск на расхождение

        double CalculateNoiseOnInterval(TParameterTMI_Old <TypeOfX, TypeOfY> *SourceParameter, TIntervalCharacteristics *FailedIntervals, double QantumStep);  //Посчитать шум в шагах квантования.
        void Clear();

    public:

        TCommonEvaluation();
        ~TCommonEvaluation();
        bool Load(TParameterTMI_Old <TypeOfX, TypeOfY> *InputPOSModel,
                    TParameterTMI_Old <TypeOfX, TypeOfY> *InputPOSParameter,
                    TIntervalCharacteristics *LongestIntervalWithoutImpulses,
                    double QuantumStep, double Measure);
        bool isNoiseLessThanMeasure();
        bool isNoiseLessThanMeasure(double AlternativeMeasure);
        void SmoothInnerPOSParameter(unsigned int SmoothIterrationsNumber);
        unsigned int CalculateVariationsAndGetFailedIntervals(TIntervalCharacteristics* &FailedIntervalsList);
};
//---------------------------------------------------------------------------
TIntervalCharacteristics::TIntervalCharacteristics()
{
    IntervalStartTime = 0;
    IntervalEndTime = 0;
    Noise = 0;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
TCommonEvaluation <TypeOfX, TypeOfY>::TCommonEvaluation()
{
    POSModel = NULL;
    POSParameter = NULL;
    POSParameterSmoothed = NULL;
    FailedIntervals = NULL;
    LongestIntervalWithoutImpulses = NULL;
    QantumStep = 0;
    Measure = 0;
}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
TCommonEvaluation <TypeOfX, TypeOfY>::~TCommonEvaluation()
{
    delete POSModel;
    delete POSParameter;
    delete POSParameterSmoothed;
    delete [] FailedIntervals;
}

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
    QantumStep = 0;
    Measure = 0;
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
    //Noise >= fabs(AverageAmplitude - MaxAmplitude) ? return Noise : return fabs(AverageAmplitude - MinAmplitude);

}
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
bool TCommonEvaluation  <TypeOfX, TypeOfY>::Load(TParameterTMI_Old <TypeOfX, TypeOfY>* POSModel, TParameterTMI_Old <TypeOfX, TypeOfY>* POSParameter, TIntervalCharacteristics* LongestIntervalWithoutImpulses, double QuantumStep, double Measure)
{
    if(this->POSModel && this->POSParameter && this->LongestIntervalWithoutImpulses)
        Clear();

    if(POSModel && POSParameter && LongestIntervalWithoutImpulses)
    {
        this->POSModel = new TParameterTMI_Old <TypeOfX, TypeOfY>;
        this->POSParameter = new TParameterTMI_Old <TypeOfX, TypeOfY>;
        this->LongestIntervalWithoutImpulses = new TIntervalCharacteristics;
        this->POSModel = POSModel;
        this->POSParameter = POSParameter;
        this->LongestIntervalWithoutImpulses = LongestIntervalWithoutImpulses;
    }
    else
        return false;

    this->QantumStep = QuantumStep;
    this->Measure = Measure;
    this->LongestIntervalWithoutImpulses->Noise = CalculateNoiseOnInterval(this->POSParameter, this->LongestIntervalWithoutImpulses, this->QantumStep);

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
int TCommonEvaluation <TypeOfX, TypeOfY>::CalculateVariationsAndGetFailedIntervals(TIntervalCharacteristics* &FailedIntervalsList)
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

        if(FailedIntervalPointsCounter >= 4 && !isInsideFailedInterval)  //////////////////////////////////////////////////////////////
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
#endif
