// Fill out your copyright notice in the Description page of Project Settings.

#include "MyClass.h"

MyClass::MyClass()
{
	active = true;
	//CollisionComp->OnComponentHit.AddDynamic(this, &ATestProjectProjectile::OnHit);
}

MyClass::~MyClass()
{
}

void MyClass::Remove() {
	active = false;
}