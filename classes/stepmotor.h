//---------------------------------------------------------------------------

#ifndef stepmotorH
#define stepmotorH
//---------------------------------------------------------------------------
#include <class_ParameterTMI.h>
#include <class_FileTP.h>
#include <class_Errors.h>
#include <noise.h>

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
};
//---------------------------------------------------------------------------
template <class TypeOfX, class TypeOfY>
class TStepMotor
{
    private:
        TErrors Errors;
        TControlImpulse ControlImpulseParam;            //��������� ������ ���������� �� ��
        TParameterTMI<TypeOfX, TypeOfY> ControlImpulse; //������ ���������� ��������� ��
        TParameterTMI<TypeOfX, TypeOfY> POS;            //������ ������������� �������� �����

        double PointFrequency;                          //������� ������
        double DeltaU;                                  //��������� ���������� ��� �� 1 ��� ��
        double StartU;                                  //��������� ������� ����������
        double Luft;
        double CurrentLuftPosition;

        double LuftCalc(double Increment);              //������ ����� ��


    public:
        TStepMotor();

        bool LoadParameter(TParameterTMI<TypeOfX, TypeOfY> ParameterUI, double Delta_U,               //��������� �������������
                            double Start_U, TControlImpulse ControlLevels, double Point_Freq,
                            double Luft, double CurrentLuftPosition);

        bool GetPOS(TParameterTMI<TypeOfX, TypeOfY> &POS, TypeOfY TimeShift, bool FlagUsage,
                    TypeOfY QuantumLineBottomBorder, TypeOfY QuantumLineTopBorder,
                    unsigned int QuantumLineIntervals, char RoundingMethod,
                    TIntervalCharacteristics* &LongestIntervalWithoutImpulses);       //������������� ���

        int GetError(AnsiString &Message);

        int GetWarning(AnsiString &Message);
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

    Errors.SetErrorMessage(1, "������������ ���.");
    Errors.SetErrorMessage(4, "������������ �������.");
    Errors.SetErrorMessage(2, "������������� �������� ������ �������.");
    Errors.SetErrorMessage(3, "�������� ������� �������� ������ ������ �����������.");
    Errors.SetErrorMessage(5, "����������� ������� �����������������.");
    Errors.SetErrorMessage(6, "������� GetPOS ���������� ��������. �������� �� �� ��������.");
    Errors.SetErrorMessage(7, "�������� �������� ��������� ����� ������ ��� ������� ������ �����.");
    Errors.SetWarningMessage(1, "������� DigitalQuantum �� ���������.");
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

template <class TypeOfX, class TypeOfY>
int TStepMotor<TypeOfX, TypeOfY> ::GetError(AnsiString &Message)
{
    return Errors.GetError(Message);
}

template <class TypeOfX, class TypeOfY>
int TStepMotor<TypeOfX, TypeOfY> ::GetWarning(AnsiString &Message)
{
    return Errors.GetWarning(Message);
}

template <class TypeOfX, class TypeOfY>
bool TStepMotor<TypeOfX, TypeOfY>::LoadParameter(TParameterTMI<TypeOfX, TypeOfY> ParameterUI, double Delta_U,               //��������� �������������
                                                double Start_U, TControlImpulse ControlLevels, double Point_Freq,
                                                double Luft, double CurrentLuftPosition)
{
    // �������� ������������ ��������� ���� ��������� � ������� ������
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

    //�������� ������������ �������� ������� ���������� �� �������� ��
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

template <class TypeOfX, class TypeOfY>
bool TStepMotor<TypeOfX, TypeOfY>::GetPOS(  TParameterTMI<TypeOfX, TypeOfY> &POS, TypeOfY TimeShift, bool FlagUsage,
                                            TypeOfY QuantumLineBottomBorder, TypeOfY QuantumLineTopBorder,
                                            unsigned int QuantumLineIntervals, char RoundingMethod,
                                            TIntervalCharacteristics* &LongestIntervalWithoutImpulses)
{
    AnsiString FName = "D:\\2134241";

    if(PointFrequency == 0)
    {
        Errors.SetError(6);
        return false;
    }
    unsigned int Index = 0;
    bool Flag = false;
    double Level, LastLevel;

    //�������������� � "���������"
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

    //���������� ������������� �����������
    //SaveToFile(ControlImpulse,FName+"_1.d2");

    //������� ������������ �������
    for(unsigned int i = 1; i < ControlImpulse.CountPoint; i++)
        {
            if(ControlImpulse.Points[i].Y != ControlImpulse.Points[i-1].Y)
                LvlSwitch++;
        }

    TParameterTMI<TypeOfX, TypeOfY> CuttedContImp;

    //�������������� � "����"
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

    LongestIntervalWithoutImpulses = GetLongestIntervalTime(&CuttedContImp);



    //���������� ������������� �����������
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

    //�������� ������������ ����������� �������
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

                ErrLine = ErrLine + (AnsiString)BrokenPointsCounter + ". C " + (AnsiString)CuttedContImp.Points[i-1].X + " - ���. " + No1 + " �� " + (AnsiString)CuttedContImp.Points[i].X + " - ���. " + No2 + "\n";
                BrokenPointsCounter++;
            }
        }
    }

    //���������� ������������� �����������
    //SaveToFile(CuttedContImp,FName+"_3.d2");

    //���� ����������� ��������, �� ������� ������������ ����� � ��������� ������������
    if(BrokenPointsCounter != 1)
    {
        Errors.SetError(5, ErrLine);
        return false;
    }

    //������������ �������� ������� ��������� ���
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

    //������������ ����������� ����
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

    //����� �� �������
    if(TimeShift != 0)
        for(unsigned int i = 1; i < POS.CountPoint; i++)
            POS.Points[i].X = POS.Points[i].X + TimeShift;

    //Quantum Web
    bool DQ;
    if(FlagUsage)
        DQ = DigitalQuantum(POS, RoundingMethod, QuantumLineBottomBorder, QuantumLineTopBorder, QuantumLineIntervals);

    if(FlagUsage == true && DQ == false)
        Errors.SetWarning(1);


    return true;
}
//---------------------------------------------------------------------------
#endif

