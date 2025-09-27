// Source code implementation by Ephraim Gbadebo.


#include "FFYOptionWidget.h"

#include "FFYMenuWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "FFY/FFYDataEnums.h"


//Enabled for flicker animation, timers not working while game is paused.
void UFFYOptionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (bIsFlickering)
	{
		if (FlickTime += InDeltaTime >= 0.7f)
		{
			Unflicker();
			FlickTime = 0.0f;
		}
		else
		{
			FlickTime += InDeltaTime;
		}
		
	}
}

void UFFYOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, "OPTION CONSTRUCT");
	//this->TickActionsAndAnimation();
	if (!bIsInitialized)
	{
		Button->OnHovered.AddDynamic(this, &UFFYOptionWidget::OnSelected);
		Button->OnUnhovered.AddDynamic(this, &UFFYOptionWidget::OnUnselected);
		Button->OnClicked.AddDynamic(this, &UFFYOptionWidget::PerformAction);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, "OPTION DELEGATES BOUND");
		if (DisplayText)
		{
			DisplayText->SetText(DisplayTextLabel);
		}
		if (bIsDisabled)
		{
			SetIsDisabled(true);
		}
		
		bIsInitialized = true;
	}

}

/*
 * Used by menus to populate DescriptionTextBlock. 
 */
FText UFFYOptionWidget::GetDescription()
{
	return FText::GetEmpty();
}

void UFFYOptionWidget::SetIsDisabled(bool Value)
{
	bIsDisabled = Value;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("DISABLED->%s"), bIsDisabled ? "true" : "false"));
	if (DisplayText)
	{
		FLinearColor InColor = bIsDisabled ? FLinearColor::Gray : FLinearColor::White;
		DisplayText->SetColorAndOpacity(FSlateColor(InColor));
	}
}

/*
 * Used in cases of selecting multiple options or keeping track of last option selected. 
 */
UFFYOptionWidget* UFFYOptionWidget::Flicker_Implementation(bool Value)
{
	bIsFlickering = Value;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Flicker func called...");
	if (bIsFlickering)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Flicker starting");
		//GetWorld()->GetTimerManager().SetTimer(FlickerTimerHandle, this, &UFFYOptionWidget::Unflicker, 0.25f, true, -1);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(FlickerTimerHandle);
		SelectedImage->SetVisibility((bIsHovered) ? ESlateVisibility::Visible : DefaultUnselectedVisibility);
	}
	
	return this;
}

/*
 * Called through timer function on a loop. Toggles flicker state. 
 */
void UFFYOptionWidget::Unflicker()
{
	if (SelectedImage)
	{
		if (SelectedImage->IsVisible())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, "On");
			SelectedImage->SetVisibility(DefaultUnselectedVisibility);
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, "On");
			SelectedImage->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UFFYOptionWidget::OnSelected()
{
	bIsHovered = true;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "Hovered");
	if (SelectedImage && !bIsFlickering && OwnerMenu->GetCurrentMenuMode() == EMenuMode::NONE)
	{
		SelectedImage->SetVisibility(ESlateVisibility::Visible);
		if (OwnerMenu)
		{
			OwnerMenu->SetCurrentOption(this); 
		}
	}
}

void UFFYOptionWidget::OnUnselected()
{
	bIsHovered = false;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "Unhovered");
	if (SelectedImage && !bIsFlickering && OwnerMenu->GetCurrentMenuMode() == EMenuMode::NONE)
	{
		SelectedImage->SetVisibility(DefaultUnselectedVisibility);
	}
}

void UFFYOptionWidget::PerformAction()
{
	if ( bIsDisabled || OwnerMenu->GetCurrentMenuMode() != EMenuMode::NONE )
	{
		if (OwnerMenu)
		{
			OwnerMenu->PlayMenuSound_Implementation(2);
		}
		return;
	}
	else
	{
		if (OwnerMenu)
		{
			OwnerMenu->PlayMenuSound_Implementation(1);
		}
	}
}
