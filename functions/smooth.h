#ifndef smoothH
#define smoothH
#include <class_ParameterTMI_old.h>

template <class typeX, class typeY>
TParameterTMI_Old <typeX, typeY>* smooth(TParameterTMI_Old <typeX, typeY> *sourceParameter,
                                        TParameterTMI_Old <typeX, typeY> *returnParameter,
                                        unsigned int iterationsNumber)
{
    if(!sourceParameter->CountPoint)
        return NULL;
    if(0 == iterationsNumber)
        return sourceParameter;

    //TParameterTMI <typeX, typeY>* returnParameter = new TParameterTMI <typeX, typeY> [sourceParameter->CountPoint];
    //returnParameter->CountPoint = sourceParameter->CountPoint;

    returnParameter->Points[0].Y = (sourceParameter->Points[0].Y + sourceParameter->Points[1].Y) / 2;
    returnParameter->Points[0].X = sourceParameter->Points[0].X;
    returnParameter->Points[returnParameter->CountPoint - 1].Y = (sourceParameter->Points[returnParameter->CountPoint - 1].Y + sourceParameter->Points[returnParameter->CountPoint - 2].Y) / 2;
    returnParameter->Points[returnParameter->CountPoint - 1].X = sourceParameter->Points[returnParameter->CountPoint - 1].X;

    for(unsigned int j = 1; j < returnParameter->CountPoint - 1; j++)
    {
        returnParameter->Points[j].Y = (sourceParameter->Points[j - 1].Y + 2 * sourceParameter->Points[j].Y + sourceParameter->Points[j + 1].Y) / 4;
        returnParameter->Points[j].X = sourceParameter->Points[j].X;
    }

    if(iterationsNumber > 1)
        return smooth(returnParameter, returnParameter, iterationsNumber - 1);
    else
        return  returnParameter;
}
#endif


/* не оптимально по памяти
template <class typeX, class typeY>
TParameterTMI<typeX, typeY>* smooth(TParameterTMI <typeX, typeY> *sourceParameter,
                                    unsigned int iterationsNumber)
{
    if(!sourceParameter->CountPoint)
        return NULL;
    if(0 == iterationsNumber)
        return sourceParameter;

    TParameterTMI <typeX, typeY>* returnParameter = new TParameterTMI <typeX, typeY> [sourceParameter->CountPoint];
    returnParameter->CountPoint = sourceParameter->CountPoint;

    returnParameter->Points[0].Y = (sourceParameter->Points[0].Y + sourceParameter->Points[1].Y) / 2;
    returnParameter->Points[0].X = sourceParameter->Points[0].X;
    returnParameter->Points[returnParameter->CountPoint - 1].Y = (sourceParameter->Points[returnParameter->CountPoint - 1].Y + sourceParameter->Points[returnParameter->CountPoint - 2].Y) / 2;
    returnParameter->Points[returnParameter->CountPoint - 1].X = sourceParameter->Points[returnParameter->CountPoint - 1].X;

    for(unsigned int j = 1; j < returnParameter->CountPoint - 1; j++)
    {
        returnParameter->Points[j].Y = (sourceParameter->Points[j - 1].Y + 2 * sourceParameter->Points[j].Y + sourceParameter->Points[j + 1].Y) / 4;
        returnParameter->Points[j].X = sourceParameter->Points[j].X;
    }

    if(iterationsNumber > 1)
        return smooth(returnParameter, iterationsNumber - 1);
    else
        return  returnParameter;
}
*/