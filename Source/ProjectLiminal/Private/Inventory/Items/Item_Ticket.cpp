// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Items/Item_Ticket.h"
#include "Components/SphereComponent.h"


AItem_Ticket::AItem_Ticket()
{
	TesterSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TesterSphere"));
	TesterSphere->SetupAttachment(GetRootComponent());
}
