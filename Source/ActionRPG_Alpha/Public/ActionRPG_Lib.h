// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// ���ο� ��ũ ī�װ� ����
DECLARE_LOG_CATEGORY_EXTERN(RPGProject, Log, All);

// �Լ�, ���� ���
#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(")+FString::FromInt(__LINE__)+TEXT(")"))
// UE_LOG�� Verbosity(�α� ����) �� ȣ��� �Լ�, ������ ���
#define RLOG_L(Verbosity) UE_LOG(RPGProject,Verbosity, TEXT("%s"), *LOG_CALLINFO)
// RLOG_L�� �߰����� Text ��� ����
#define RLOG(Verbosity, Format, ...) UE_LOG(RPGProject,Verbosity,TEXT("%s%s"),*LOG_CALLINFO,*FString::Printf(Format,##__VA_ARGS__))
// Assertion ��� Ŀ����
#define RCHECK(Expr, ...) {if(!(Expr))\
{RLOG(Error,TEXT("ASSERTION : %s"),TEXT("'"#Expr"'"));\
return __VA_ARGS__;}}