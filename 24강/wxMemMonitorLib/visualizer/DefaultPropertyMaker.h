/**
Name:   DefaultPropertyMaker.h
Author:  jjuiddong
Date:    1/30/2013


2014-06-14
	- ����ȭ (�Լ� ���� ���۷����� �ٲ�)
	- ������Ƽ�� ���� ��, ������ SVisDispDesc ����ü�� �����Ѵ�.

*/
#pragma once


#include "VisualizerGlobal.h"


class wxPGProperty;
namespace visualizer
{
	using namespace memmonitor;


	// Make Property
	SVisDispDesc MakeProperty_DefaultForm( const SVisDispDesc &parentDispdesc, 
		const std::string &symbolName, const bool IsApplyVisualizer, const int depth );

	SVisDispDesc MakeProperty_DefaultForm( const SVisDispDesc &parentDispdesc, 
		const SSymbolInfo &symbol, const bool IsApplyVisualizer, const int depth );

	bool	MakePropertyChild_DefaultForm(const SVisDispDesc &parentDispdesc, 
		const SSymbolInfo &symbol, const bool IsApplyVisualizer, const int depth );

	SVisDispDesc MakeProperty_Dummy( const SVisDispDesc &parentDispdesc, const string &str );

}
