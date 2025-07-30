#include "TimeSlowWidget.h"

void UTimeSlowWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (TimeSlowBar)
    {
        TimeSlowBar->SetPercent(1.0f);
    }
}

void UTimeSlowWidget::UpdateTimeSlowBar(float Ratio)
{
    if (TimeSlowBar)
    {
        TimeSlowBar->SetPercent(Ratio);
    }
}
