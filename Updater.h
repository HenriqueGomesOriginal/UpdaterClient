#pragma once
#include <Windows.h>
#include "rapidjson/document.h"

ref class Updater
{
public:
	Updater();
	static void connectServerUpdate(Object^ form);
	static int getCurrentIdVersion();
	static void setCurretnIdVersion(int version);
	static void AppendText(System::String^ text);

	delegate void StringArgReturningVoidDelegate(System::String^ text);
	static System::Windows::Forms::TextBox^ lb_inicial;
};

