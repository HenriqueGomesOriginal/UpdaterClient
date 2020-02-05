#include "Updater.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Net;
using namespace System::Threading;

#define server "yourServerDomain"
#define program "yourProgramName"

Updater::Updater()
{
}

void Updater::connectServerUpdate(Object^ form)
{
	//Cria handle to start program
	STARTUPINFO lpStartupInfo;
	PROCESS_INFORMATION lpProcessInfo;

	ZeroMemory(&lpStartupInfo, sizeof(lpStartupInfo));
	lpStartupInfo.cb = sizeof(lpStartupInfo);
	ZeroMemory(&lpProcessInfo, sizeof(lpProcessInfo));

	Form^ formAux = (Form^)form;
	lb_inicial = (TextBox^)formAux->Controls->Find("tb_principal", true)[0];

	lb_inicial->Text = "Checking internet access";
	int version = getCurrentIdVersion();
	try
	{
		System::Net::HttpWebRequest^ httpWebRequest = (System::Net::HttpWebRequest^)System::Net::WebRequest::Create("http://" + server + "/yourWebService");
		httpWebRequest->Method = "POST";
		httpWebRequest->KeepAlive = true;
		httpWebRequest->ContentType = "application/json";
		httpWebRequest->Timeout = 50000;

		System::IO::StreamWriter^ streamWriter = gcnew System::IO::StreamWriter(httpWebRequest->GetRequestStream());
		streamWriter->Write("{\"version\":\"" + version + "\"}");
		streamWriter->Flush();
		streamWriter->Close();

		System::Net::HttpWebResponse^ resp = (System::Net::HttpWebResponse^)httpWebRequest->GetResponse();
		System::IO::StreamReader^ streamReader = gcnew System::IO::StreamReader(resp->GetResponseStream());
		String^ result = streamReader->ReadToEnd();

		AppendText("Checking version...");
		resp->Close();

		streamReader->Close();

		rapidjson::Document document;
		document.Parse((char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(result));

		if (document.IsObject())
		{
			String^ aux = gcnew String(document["version"].GetString());
			int serverVersion = Convert::ToInt32(aux);

			if (version < serverVersion)
			{
				DialogResult result;
				result = MessageBox::Show("New version avaliable, do you want update?", "Update", MessageBoxButtons::YesNo, MessageBoxIcon::Information);
				if (result == DialogResult::Yes)
				{
					long total = 0;
					long received = 0;

					try
					{
						// Download program from server
						AppendText("Downloading update");
						WebClient^ wc = gcnew WebClient();
						wc->DownloadFile(gcnew Uri("http://" + server + "/yourWebServiceDownload"), program);
					}
					catch (System::Net::WebException^ we)
					{
						MessageBox::Show("Fail on file download", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
						CreateProcess((LPCWSTR)program, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &lpStartupInfo, &lpProcessInfo);
						exit(EXIT_FAILURE);
					}
					setCurretnIdVersion(serverVersion);

					AppendText("Program updated!");
					MessageBox::Show("EvaCom foi atualizado!", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
				}
			}
			else
			{
				MessageBox::Show("Program updated", "Update", MessageBoxButtons::OK, MessageBoxIcon::Information);
				CreateProcess((LPCWSTR)program, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &lpStartupInfo, &lpProcessInfo);
				exit(EXIT_SUCCESS);
			}
		}
		else
		{
			MessageBox::Show("Update service is not avaliable!", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			CreateProcess((LPCWSTR)program, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &lpStartupInfo, &lpProcessInfo);
			exit(EXIT_SUCCESS);
		}
	}
	catch (System::Net::WebException^ ex)
	{
		MessageBox::Show("Not possible to make a connection", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}

	CreateProcess((LPCWSTR)program, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &lpStartupInfo, &lpProcessInfo);

	exit(EXIT_SUCCESS);
}

int Updater::getCurrentIdVersion()
{
	System::IO::StreamReader^ streamReader = gcnew System::IO::StreamReader("version");
	String^ reader = streamReader->ReadLine();
	streamReader->Close();

	return Convert::ToInt32(reader);
}

void Updater::setCurretnIdVersion(int version)
{
	System::IO::StreamWriter^ streamWriter = gcnew System::IO::StreamWriter("version");
	streamWriter->Write("" + version);
	streamWriter->Close();
}

void Updater::AppendText(System::String^ text)
{
	if (lb_inicial->InvokeRequired)
	{
		StringArgReturningVoidDelegate^ d =
			gcnew StringArgReturningVoidDelegate(&Updater::AppendText);
		lb_inicial->Invoke(d, gcnew array<Object^> { text });
	}
	else
	{
		lb_inicial->Text = text;
	}
}